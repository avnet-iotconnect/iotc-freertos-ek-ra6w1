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
 * rm_lwip_w_tcp.c
 *
 *  Created on: 04-Oct-2024
 *      Author: Renesas
 */
#include "lwip/opt.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/debug.h"

#include "rm_lwip_w_hooks.h"
#include "rm_lwip_w_tcp.h"

#include <string.h>

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */
#if CFG_PMGR
struct tcp_pcb *tcp_new_ip_type_dpm(char *name, u8_t type)
{
  struct tcp_pcb *pcb;
  pcb = tcp_new_dpm(name);
#if LWIP_IPV4 && LWIP_IPV6
  if (pcb != NULL && pcb->state == CLOSED) {
    IP_SET_TYPE_VAL(pcb->local_ip, type);
    IP_SET_TYPE_VAL(pcb->remote_ip, type);
  }
#else
  LWIP_UNUSED_ARG(type);
#endif /* LWIP_IPV4 && LWIP_IPV6 */
  return pcb;
}

struct tcp_pcb *tcp_new_dpm(char *name)
{
  if (name == NULL) {
    return tcp_alloc(TCP_PRIO_NORMAL);
  }

  return tcp_alloc_dpm(name, TCP_PRIO_NORMAL);
}

int tcp_reg_cli_name(void *arg, char *buf, size_t buflen)
{
  struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen *)arg;
  unsigned int idx = 0;

  for (idx = 0 ; idx < TCP_SRV_MAX_CLI ; idx++) {
    if ((lpcb->flag >> idx) == 0) {
      TCP_GEN_CLI_NAME(lpcb->name, idx, buf, buflen);
      lpcb->flag |= (1 << idx);
      return 0;
    }
  }

  return -1;
}

int tcp_dereg_cli_name(void *arg, char *buf, size_t buflen)
{
  struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen *)arg;
  int idx = 0;

  if ((strncmp(lpcb->name, buf, strlen(lpcb->name)) == 0)
      && (buflen > (strlen(lpcb->name) + 1)))
  {
    sscanf(buf + strlen(lpcb->name) + 1, "%d", &idx);
    lpcb->flag &= (u32_t)~(1 << idx);
    return 0;
  }

  return -1;
}
#endif /* CFG_PMGR */

static void
tcp_abandon_remote_ip_pcblist(const ip_addr_t *addr, struct tcp_pcb *pcb_list)
{
  struct tcp_pcb *pcb;
  pcb = pcb_list;

  LWIP_ASSERT("tcp_abort_by_ip_pcblist: invalid addr", addr != NULL);

  while (pcb != NULL) {
    /* PCB connected to remote ip address? */
    if (ip_addr_cmp(&pcb->remote_ip, addr)) {
      /* this connection must be aborted */
      struct tcp_pcb *next = pcb->next;
      LWIP_DEBUGF(NETIF_DEBUG | LWIP_DBG_STATE, ("netif_set_ipaddr: aborting TCP pcb %p\n", (void *)pcb));
      tcp_abandon(pcb, 0);
      pcb = next;
    } else {
      pcb = pcb->next;
    }
  }
}

void
tcp_abandon_remote_ip(const ip_addr_t *addr)
{
  if (!ip_addr_isany(addr)) {
    tcp_abandon_remote_ip_pcblist(addr, tcp_active_pcbs);
    tcp_abandon_remote_ip_pcblist(addr, tcp_bound_pcbs);
  }
}

#if defined(RRQ61XX_CUSTOM_FIXES) && defined(TCP_MULTI_SESS_PCB_MUTEX)
void
tcp_multi_sess_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb)
{
  LWIP_ASSERT("tcp_pcb_remove: invalid pcb", pcb != NULL);
  LWIP_ASSERT("tcp_pcb_remove: invalid pcblist", pcblist != NULL);

  sys_multi_tcp_pcb_lock();
  TCP_RMV(pcblist, pcb);
  sys_multi_tcp_pcb_unlock();

  tcp_pcb_purge(pcb);

  /* if there is an outstanding delayed ACKs, send it */
  if ((pcb->state != TIME_WAIT) &&
      (pcb->state != LISTEN) &&
      (pcb->flags & TF_ACK_DELAY)) {
    tcp_ack_now(pcb);
    tcp_output(pcb);
  }

  if (pcb->state != LISTEN) {
    LWIP_ASSERT("unsent segments leaking", pcb->unsent == NULL);
    LWIP_ASSERT("unacked segments leaking", pcb->unacked == NULL);
#if TCP_QUEUE_OOSEQ
    LWIP_ASSERT("ooseq segments leaking", pcb->ooseq == NULL);
#endif /* TCP_QUEUE_OOSEQ */
  }

  pcb->state = CLOSED;
  /* reset the local port to prevent the pcb from being 'bound' */
  pcb->local_port = 0;

  LWIP_ASSERT("tcp_pcb_remove: tcp_pcbs_sane()", tcp_pcbs_sane());
}
#endif /* RRQ61XX_CUSTOM_FIXES && TCP_MULTI_SESS_PCB_MUTEX */

#endif /* LWIP_TCP */
