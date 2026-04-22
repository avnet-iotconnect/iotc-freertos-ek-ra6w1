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

#ifndef RM_LWIP_W_HELPER_H
#define RM_LWIP_W_HELPER_H

#include "lwip/stats.h"

/* helper functions used by other modules internally */
long pow_long(long x, int order);

#if defined (__SUPPORT_IPV4__)
int isvalidip(char *theip);
int is_in_valid_ip_class(char *theip);
int isvalidmask(char *theip);
int isvalidIPrange(long ip, long firstIP, long lastIP);
void longtoip(long ip, char *ipbuf);
int isvalidIPsubnetRange(long ip, long subnetip, long subnet);
long subnetRangeFirstIP(long ip, long subnet);
#endif /* __SUPPORT_IPV4__ */

#if defined (__SUPPORT_IPV6__)
/* LONG[4] ==> String */
void ipv6long2str(unsigned long *ipv6_long, char *ipv6_str);

/* Stirng ==> LONG[4] */
int parse_IPv6_to_long(const char *pszText, unsigned long *ipv6addr, int *pnPort);
#endif    /* __SUPPORT_IPV6__ */

/* Stats */

typedef enum {
    stats_all,
    stats_ipv4,
    stats_ip,
    stats_udp,
    stats_tcp,
    stats_icmp,
    stats_igmp,
    stats_arp,
    stats_ipfrag,
#if LWIP_IPV6
    stats_ipv6,
    stats_ip6,
    stats_nd6,
    stats_mld6,
    stats_icmp6,
    stats_ip6frag,
#endif /* LWIP_IPV6 */
    stats_link,
    stats_memory,
    stats_system
} stats_opt;

#if TCP_STATS
#define TCP_STATS_RESET()   reset_stats_proto(&lwip_stats.tcp, "TCP")
#else
#define TCP_STATS_RESET()
#endif

#if UDP_STATS
#define UDP_STATS_RESET() reset_stats_proto(&lwip_stats.udp, "UDP")
#else
#define UDP_STATS_RESET()
#endif

#if ICMP_STATS
#define ICMP_STATS_RESET() reset_stats_proto(&lwip_stats.icmp, "ICMP")
#else
#define ICMP_STATS_RESET()
#endif

#if IGMP_STATS
#define IGMP_STATS_RESET() reset_stats_igmp(&lwip_stats.igmp, "IGMP")
#else
#define IGMP_STATS_RESET()
#endif

#if IP_STATS
#define IP_STATS_RESET()    reset_stats_proto(&lwip_stats.ip, "IP")
#else
#define IP_STATS_RESET()
#endif

#if IPFRAG_STATS
#define IPFRAG_STATS_RESET()    reset_stats_proto(&lwip_stats.ip_frag, "IP_FRAG")
#else
#define IPFRAG_STATS_RESET()
#endif

#if ETHARP_STATS
#define ETHARP_STATS_RESET()    reset_stats_proto(&lwip_stats.etharp, "ETHARP")
#else
#define ETHARP_STATS_RESET()
#endif

#if LINK_STATS
#define LINK_STATS_RESET()   reset_stats_proto(&lwip_stats.link, "LINK")
#else
#define LINK_STATS_RESET()
#endif

#if IP6_STATS
#define IP6_STATS_RESET()   reset_stats_proto(&lwip_stats.ip6, "IPv6")
#else
#define IP6_STATS_RESET()
#endif

#if ICMP6_STATS
#define ICMP6_STATS_RESET()   reset_stats_proto(&lwip_stats.icmp6, "ICMPv6")
#else
#define ICMP6_STATS_RESET()
#endif

#if IP6_FRAG_STATS
#define IP6_FRAG_STATS_RESET()   reset_stats_proto(&lwip_stats.ip6_frag, "IPv6 FRAG")
#else
#define IP6_FRAG_STATS_RESET()
#endif

#if MLD6_STATS
#define MLD6_STATS_RESET()   reset_stats_igmp(&lwip_stats.mld6, "MLDv1")
#else
#define MLD6_STATS_RESET()
#endif

#if ND6_STATS
#define ND6_STATS_RESET()   reset_stats_proto(&lwip_stats.nd6, "ND")
#else
#define ND6_STATS_RESET()
#endif

/* Display of statistics */
#if LWIP_STATS_DISPLAY
void rm_lwip_w_stats_display(stats_opt option);
void rm_lwip_w_stats_reset(stats_opt option);
void reset_stats_proto(struct stats_proto *proto, const char *name);
void reset_stats_igmp(struct stats_igmp *igmp, const char *name);
#else /* LWIP_STATS_DISPLAY */
#define rm_lwip_w_stats_display(option)
#define rm_lwip_w_stats_reset(option)
#define reset_stats_proto(proto, name)
#define reset_stats_igmp(igmp, name)
#endif /* LWIP_STATS_DISPLAY */

/* RTC_W helper */
#if CFG_RTC_W
#include "r_rtc_w.h"
#include "r_rtc_w_helper.h"
#endif /* CFG_RTC_W */
void sntp_calendar_system_time_set ( __time64_t * corrtime);
extern unsigned long long get_systimeoffset_from_rtm(void);

#endif // RM_LWIP_W_HELPER_H
