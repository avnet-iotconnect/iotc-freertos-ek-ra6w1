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

#if CFG_PMGR

#if CFG_WIFI
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "lwip/priv/sockets_priv.h" //for struct lwip_sock
#include "lwip/api.h"               //for struct netconn
#include "lwip/priv/tcp_priv.h"

#include "rm_pmgr_w_instance.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Private Functions
 *********************************************************************************************************************/



/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
//Only for BSD socket
int RM_PMGR_W_socket_rx_data_is_remaining(int fd)
{
	extern struct lwip_sock *get_socket_dpm(int fd);
	extern void done_socket_dpm(struct lwip_sock *sock);

	struct lwip_sock *sock = NULL;

	int remaining = 0;

	sock = get_socket_dpm(fd);
	if (sock) {
		if (sock->lastdata.pbuf) {
			if (NETCONNTYPE_GROUP(netconn_type(sock->conn)) == NETCONN_TCP) {
				remaining = sock->lastdata.pbuf->tot_len;
			} else {
				remaining = sock->lastdata.netbuf->p->tot_len;
			}
		}

		done_socket_dpm(sock);
	}
	
	return remaining;
}

#endif /* CFG_WIFI */

#endif /* CFG_PMGR */