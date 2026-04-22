/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef _RWNX_ETHER_H
#define _RWNX_ETHER_H

#include "rwnx_types.h"

#define ETH_ALEN            6
#define ETH_HLEN            14 
#define ETH_P_802_2         0x0004
#define ETH_P_802_3         0x0001
#define ETH_P_802_3_MIN     0x0600
#define ETH_P_ARP           0x0806
#define ETH_P_PAE           0x888E
#define ETH_P_TDLS          0x890D

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];
	unsigned char	h_source[ETH_ALEN];
	__be16		h_proto;
} __attribute__((packed));

#endif /* _RWNX_ETHER_H */
