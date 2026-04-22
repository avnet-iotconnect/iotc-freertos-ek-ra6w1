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

#ifndef RM_WIFI_HELPER_H
#define RM_WIFI_HELPER_H

/// No Error
#define CC_SUCCESS                  0

/// Too long string value input
#define CC_FAILURE_STRING_LENGTH    1

/// No value input
#define CC_FAILURE_NO_VALUE         2

/// Range out
#define CC_FAILURE_RANGE_OUT        3

/// Not Supported input
#define CC_FAILURE_NOT_SUPPORTED    4

/// Invalid input
#define	CC_FAILURE_INVALID          5

/// Memory Allocation Failure
#define	CC_FAILURE_NO_ALLOCATION    6

/// Not Ready (Wi-Fi connection or Network setting)
#define CC_FAILURE_NOT_READY        7

/// Unknown Reason
#define CC_FAILURE_UNKNOWN          9

/// Enable/Disable values
typedef enum {
	/// Not used
	CC_VAL_DISABLE,
	/// Used
	CC_VAL_ENABLE,
} cc_val_bool;

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "FreeRTOS.h"
#include "event_groups.h"
#include "util_api.h"

/**********************************************************************************************************************
 * External Functions Prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Defines
 **********************************************************************************************************************/

// atoi error policy
// default leading "0" / "+" / "-0" are not allowed
#define POL_1   1 
// leading "+" / "-0" are not allowed
#define POL_2   2

#define rm_wifi_write_mac_address(...)      writeMACaddress(__VA_ARGS__)
#define rm_wifi_get_mac_adress_string(...)  getMACAddrStr(__VA_ARGS__)
#define rm_wifi_util_sflash_write(...)      util_sflash_write(__VA_ARGS__)
#define rm_wifi_util_sflash_read(...)       util_sflash_read(__VA_ARGS__)
#define rm_wifi_util_sflash_erase(...)      util_sflash_erase(__VA_ARGS__)
#define rm_wifi_is_in_softap_acs_mode(...)  is_in_softap_acs_mode(__VA_ARGS__)
/* wpa_supplicant functions */
#define rm_wifi_chk_channel_by_country(...) chk_channel_by_country(__VA_ARGS__)
/* supplicant functions */
#define rm_wifi_select_ipaddr_type_from_str(...) select_ipaddr_type_from_str(__VA_ARGS__)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/* For select_ipaddr_type_from_str() */
struct sockaddr_storage;

/**********************************************************************************************************************
 * Helper functions used by other modules internally
 **********************************************************************************************************************/
int  gen_ssid(char * prefix, int iface, int quotation, char * ssid, int size);
int  rm_wifi_atoi_custom(char * str);
int  rm_wifi_get_int_val_from_str(char * param, int * int_val, int policy);
void rm_wifi_register_wifi_notify_cb(void);
void factory_reset_sta_mode(void);
void factory_reset_ap_mode(void);
int getMacAddrMswLsw(UINT iface, ULONG * macmsw, ULONG * maclsw);
int getMACAddrStr(unsigned int iface, char * macstr, unsigned int separate);
UINT writeMACaddress(char * macaddr, int dst);
bool reset(void);
bool por_reset(void);
int get_current_rssi(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
unsigned int wait_supplicant_done(unsigned int timeout);
void wifi_conn_fail_noti_to_atcmd_host(void);
void print_sys_mode(unsigned int mode);
WIFISecurity_t rm_wifi_helper_security_str_to_type(char * security_str);
WIFISecurityExt_t rm_wifi_helper_security_type_get(UINT key_mgmt, UINT proto, UINT pairwise_cipher);

/* wpa_cli functions */
extern int  cc_set_network_str(const char * name, int iface, const char * val);
extern int  cc_set_network_int(char * name, int iface, int val);
extern int  ra6w1_cli_reply(char * cmdline, char * delimit, char * cli_reply);
extern void ra6w1_wpa_cli(int argc, char * argv[]);

/* wpa_supplicant functions */
extern int  get_run_mode(void);
extern int  select_ipaddr_type_from_str(char * str, struct sockaddr_storage * ipaddr);

extern bool twt_setup(struct twt_setup_req *req);
extern bool twt_teardown(struct twt_teardown_req *req);

#if defined ( __SUPPORT_WIFI_CONCURRENT__ )
#if defined ( __SUPPORT_FACTORY_RST_CONCURR_MODE__ )
void factory_reset_concurrent_mode(void);
#endif // __SUPPORT_FACTORY_RST_CONCURR_MODE__
int factory_reset_btn_onetouch(void);
#endif // __SUPPORT_WIFI_CONCURRENT__

#endif // RM_WIFI_HELPER_H
