/* ${RENESAS_LICENSE_CLAUSE_PLACEHOLDER} */

#ifndef _RWNX_DEV_H
#define _RWNX_DEV_H

#include <limits.h>
#include "rwnx_le_util.h"
#include "rwnx_ether.h"

#define MAX_ADDR_LEN 32
typedef unsigned long net_stats_t;

struct rwnx_dev_iface_stats {
	net_stats_t	rx_packets;
	net_stats_t	tx_packets;
	net_stats_t	rx_bytes;
	net_stats_t	tx_bytes;
	net_stats_t	rx_errors;
	net_stats_t	tx_errors;
	net_stats_t	rx_dropped;
	net_stats_t	tx_dropped;
	net_stats_t	multicast;
	net_stats_t	collisions;
	net_stats_t	rx_length_errors;
	net_stats_t	rx_over_errors;
	net_stats_t	rx_crc_errors;
	net_stats_t	rx_frame_errors;
	net_stats_t	rx_fifo_errors;
	net_stats_t	rx_missed_errors;
	net_stats_t	tx_aborted_errors;
	net_stats_t	tx_carrier_errors;
	net_stats_t	tx_fifo_errors;
	net_stats_t	tx_heartbeat_errors;
	net_stats_t	tx_window_errors;
	net_stats_t	rx_compressed;
	net_stats_t	tx_compressed;
};

struct rwnx_dev_iface {
	char name[32];
	int  ifType;
	int  ifindex;
	unsigned char perm_addr[MAX_ADDR_LEN];

	enum {
		RTNL_LINK_INITIALIZED,
		RTNL_LINK_INITIALIZING,
	} rtnl_link_state;
};

#endif	/* _RWNX_DEV_H */
