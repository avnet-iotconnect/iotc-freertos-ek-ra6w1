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
 * rm_lwip_w_stats.c
 *
 *  Created on: 14-Oct-2024
 *      Author: Renesas
 */

#include "lwip/opt.h"

#if LWIP_STATS /* don't build if not configured for use in lwipopts.h */

#include "lwip/def.h"
#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/debug.h"

#include "rm_lwip_w_helper.h"

#include <string.h>

#if LWIP_STATS_DISPLAY

void
reset_stats_proto(struct stats_proto *proto, const char *name)
{
    LWIP_PLATFORM_DIAG(("%s ", name));
    proto->xmit = 0;
    proto->recv = 0;
    proto->fw = 0;
    proto->drop = 0;
    proto->chkerr = 0;
    proto->lenerr = 0;
    proto->memerr = 0;
    proto->rterr = 0;
    proto->proterr = 0;
    proto->opterr = 0;
    proto->err = 0;
    proto->cachehit = 0;
}

#if IGMP_STATS || MLD6_STATS
void
reset_stats_igmp(struct stats_igmp *igmp, const char *name)
{
    LWIP_PLATFORM_DIAG(("%s ", name));
    igmp->xmit = 0;
    igmp->recv = 0;
    igmp->drop = 0;
    igmp->chkerr = 0;
    igmp->lenerr = 0;
    igmp->memerr = 0;
    igmp->proterr = 0;
    igmp->rx_v1 = 0;
    igmp->rx_group = 0;
    igmp->rx_general = 0;
    igmp->rx_report = 0;
    igmp->tx_join = 0;
    igmp->tx_leave = 0;
    igmp->tx_report = 0;
}
#endif /* IGMP_STATS || MLD6_STATS */

void
rm_lwip_w_stats_reset(stats_opt option)
{
    LWIP_PLATFORM_DIAG(("[Reset statistics] "));

    if (option == stats_all || option == stats_link)
        LINK_STATS_RESET();

#if LWIP_IPV4 // IPv4
    if (option == stats_all || option == stats_ipv4 || option == stats_ip)
        IP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_udp)
        UDP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_tcp)
        TCP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_icmp)
        ICMP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_igmp)
        IGMP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_arp)
        ETHARP_STATS_RESET();

    if (option == stats_all || option == stats_ipv4 || option == stats_ipfrag)
        IPFRAG_STATS_RESET();
#endif /* LWIP_IPV4 */

#if LWIP_IPV6 // IPv6
    if (option == stats_all || option == stats_ipv6 || option == stats_ip6)
        IP6_STATS_RESET();

    if (option == stats_all || option == stats_ipv6 || option == stats_nd6)
        ND6_STATS_RESET();

    if (option == stats_all || option == stats_ipv6 || option == stats_mld6)
        MLD6_STATS_RESET();

    if (option == stats_all || option == stats_ipv6 || option == stats_icmp6)
        ICMP6_STATS_RESET();

    if (option == stats_all || option == stats_ipv6 || option == stats_ip6frag)
        IP6_FRAG_STATS_RESET();
#endif /* LWIP_IPV6 */
}


void
rm_lwip_w_stats_display(stats_opt option)
{
    s16_t i;

    if (option == stats_all || option == stats_link)
        LINK_STATS_DISPLAY();
        
#if LWIP_IPV4 // IPv4
    if (option == stats_all || option == stats_ipv4 || option == stats_ip)
        IP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_udp)
        UDP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_tcp)
        TCP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_icmp)
        ICMP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_igmp)
        IGMP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_arp)
        ETHARP_STATS_DISPLAY();

    if (option == stats_all || option == stats_ipv4 || option == stats_ipfrag)
        IPFRAG_STATS_DISPLAY();
#endif /* LWIP_IPV4 */
    
#if LWIP_IPV6 // IPv6
    if (option == stats_all || option == stats_ipv6 || option == stats_ip6) {
        IP6_STATS_DISPLAY();
    }
  
    if (option == stats_all || option == stats_ipv6 || option == stats_nd6) {

        ND6_STATS_DISPLAY();
    }

    if (option == stats_all || option == stats_ipv6 || option == stats_mld6) {
        MLD6_STATS_DISPLAY();
    }

    if (option == stats_all || option == stats_ipv6 || option == stats_icmp6) {
        ICMP6_STATS_DISPLAY();
    }

    if (option == stats_all || option == stats_ipv6 || option == stats_ip6frag) {
        IP6_FRAG_STATS_DISPLAY();
    }
#endif /* LWIP_IPV6 */

    // MEM
    if (option == stats_all || option == stats_memory) {
        MEM_STATS_DISPLAY();
        for (i = 0; i < MEMP_MAX; i++) {
          MEMP_STATS_DISPLAY(i);
        }
    }

    // System
    if (option == stats_all || option == stats_system) {
        SYS_STATS_DISPLAY();
    }
}

#endif /* LWIP_STATS_DISPLAY */
#endif /* LWIP_STATS */
