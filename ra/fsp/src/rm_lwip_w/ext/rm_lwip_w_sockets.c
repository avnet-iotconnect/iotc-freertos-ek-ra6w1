/***********************************************************************************************************************
 * Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*
 * rm_lwip_w_sockets.c
 *
 *  Created on: 26-Sep-2024
 *      Author: Renesas
 */

#include "lwip/opt.h"

#if LWIP_SOCKET
#include "lwip/sockets.h"
#include "lwip/priv/sockets_priv.h"
#include "lwip/priv/tcpip_priv.h"
#include "lwip/api.h"
#include "rm_lwip_w_hooks.h"

#if CFG_PMGR
struct lwip_sock_name *socket_names;

static int load_socket_idx()
{
  extern void *RM_PMGR_W_socket_dpm_lwip_sock_names_get(void);

  if (!socket_names) {
    socket_names = RM_PMGR_W_socket_dpm_lwip_sock_names_get();
  }

  if (socket_names == NULL) {
    LWIP_DEBUGF(SOCKETS_DEBUG, ("Failed to get socket_names\r\n"));
  }

  return socket_names ? 0 : -1;
}

int lwip_close_dpm(struct netconn *newconn)
{
  int ret = 0;
  int i = 0;

  if (strlen(newconn->name) > 0) {
    if (!socket_names) {
      ret = load_socket_idx();
      if (ret) {
        return -1;
      }
    }

    for (i = 0; i < NUM_SOCKETS ; ++i) {
      if (strcmp(socket_names[i].name, newconn->name) == 0) {
        memset(socket_names[i].name, 0x00, LWIP_SOCK_MAX_NAME);
        break;
      }
    }
  }
  return 0;
}

int alloc_socket_dpm(struct netconn *newconn, int accepted)
{
  int i;
  int ret;
  SYS_ARCH_DECL_PROTECT(lev);
  LWIP_UNUSED_ARG(accepted);
  struct lwip_sock *sockets = get_lwip_sockets_pvt();

  //Load socket names
  if (RM_PMGR_W_dpm_is_enabled() && !socket_names) {
    ret = load_socket_idx();
    if (ret) {
      return -1;
    }
  }

  if (strlen(newconn->name) > 0) {
    //Find empty socket identifier
    for (i = 0; i < NUM_SOCKETS ; ++i) {
      if (strcmp(socket_names[i].name, newconn->name) == 0) {
        SYS_ARCH_PROTECT(lev);
#if LWIP_NETCONN_FULLDUPLEX
        sockets[i].fd_used    = 1;
        sockets[i].fd_free_pending = 0;
#endif
        sockets[i].conn       = newconn;
        /* The socket is not yet known to anyone, so no need to protect
           after having marked it as used. */
        SYS_ARCH_UNPROTECT(lev);
        sockets[i].lastdata.pbuf = NULL;
#if LWIP_SOCKET_SELECT || LWIP_SOCKET_POLL
        LWIP_ASSERT("sockets[i].select_waiting == 0", sockets[i].select_waiting == 0);
        sockets[i].rcvevent   = 0;
        /* TCP sendbuf is empty, but the socket is not yet writable until connected
         * (unless it has been created by accept()). */
        sockets[i].sendevent  = (NETCONNTYPE_GROUP(newconn->type) == NETCONN_TCP ? (accepted != 0) : 1);
        sockets[i].errevent   = 0;
#endif /* LWIP_SOCKET_SELECT || LWIP_SOCKET_POLL */
        return i + LWIP_SOCKET_OFFSET;
      }
    }

    //allocate new socket identifier

    for (i = 0; i < NUM_SOCKETS ; ++i) {
      if (strlen(socket_names[i].name) == 0) {
        /* Protect socket array */
        SYS_ARCH_PROTECT(lev);
        if (!sockets[i].conn) {
#if LWIP_NETCONN_FULLDUPLEX
          if (sockets[i].fd_used) {
              SYS_ARCH_UNPROTECT(lev);
              continue;
          }
          sockets[i].fd_used    = 1;
          sockets[i].fd_free_pending = 0;
#endif
          sockets[i].conn       = newconn;
          /* The socket is not yet known to anyone, so no need to protect
             after having marked it as used. */
          SYS_ARCH_UNPROTECT(lev);
          sockets[i].lastdata.pbuf = NULL;
#if LWIP_SOCKET_SELECT || LWIP_SOCKET_POLL
          LWIP_ASSERT("sockets[i].select_waiting == 0", sockets[i].select_waiting == 0);
          sockets[i].rcvevent   = 0;
          /* TCP sendbuf is empty, but the socket is not yet writable until connected
           * (unless it has been created by accept()). */
          sockets[i].sendevent  = (NETCONNTYPE_GROUP(newconn->type) == NETCONN_TCP ? (accepted != 0) : 1);
          sockets[i].errevent   = 0;
#endif /* LWIP_SOCKET_SELECT || LWIP_SOCKET_POLL */
          strcpy(socket_names[i].name, newconn->name);
          return i + LWIP_SOCKET_OFFSET;
        }
        SYS_ARCH_UNPROTECT(lev);
      }
    }
  }
  return -1;
}
#endif /* CFG_PMGR */

#endif /* LWIP_SOCKET */
