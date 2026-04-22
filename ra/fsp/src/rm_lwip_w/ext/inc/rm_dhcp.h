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

#ifndef LWIP_RM_DHCP_H
#define LWIP_RM_DHCP_H
#include "lwip/opt.h"

#if LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include "lwip/prot/ethernet.h"
#if CFG_PMGR
#include "sleep_mgmt_regs.h"		/* TEMPORY FOR DPM_WAKEUP */
#endif /* CFG_PMGR */

#include <string.h>

#include "user_dpm.h"

#if CFG_PMGR
extern int dpm_clr_dhcpc_info(void);
extern int dpm_save_dhcpc_info(void);
extern int dpm_restore_dhcpc_info(void);
#endif /* CFG_PMGR */

/* DHCP time value type */
typedef enum {
	REQUEST_TIMEOUT =   0,
	T0_TIMEOUT,
	T1_TIMEOUT,
	T2_TIMEOUT,
	T1_RENEW_TIME,
	T1_REBIND_TIME,
	LEASE_USED,
	OFFERED_T0_LEASE,
	OFFERED_T1_RENEW,
	OFFERED_T2_REBIND,
	MAX_TIME =  9
} dhcp_time_type_t;

/* To get current DHCP state */
u8_t dhcp_get_state(const struct netif *netif);

void set_debug_dhcpc(u8_t flag);
u8_t get_debug_dhcpc(void);
void dhcp_event_send(u8 event);
void dhcp_state_change(struct dhcp *dhcp, u8_t new_state);
u32_t dhcp_get_timeout_value(const struct netif *netif, int time_type);
void debug_dhcp_msg(struct dhcp * dhcp);
void debug_dhcp_info(const struct netif * netif);
#if DHCP_DOES_ARP_CHECK
void rm_dhcp_arp_reply(struct netif *netif, const ip4_addr_t *addr, struct eth_addr *ethaddr);
#endif

#endif /* LWIP_RM_DHCP_H */
#endif /* LWIP_IPV4 && LWIP_DHCP */
