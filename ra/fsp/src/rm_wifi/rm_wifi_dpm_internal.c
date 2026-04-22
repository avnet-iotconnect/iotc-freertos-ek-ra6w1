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
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_wifi_dpm_internal.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define PTIM_STATUS_AREA (dg_configDPMST_RTM_ADDR + 4324)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
RTOS4DPM_ST ptim_event = (RTOS4DPM_ST) RTOS4DPM_ST_UNDEFINED;
extern int    dpm_wpa_supp_state;
extern unsigned char dpm_cmd_autoarp_period;

/***********************************************************************************************************************
 * Private Functions
 *********************************************************************************************************************/



/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

extern unsigned char get_last_abnormal_cnt(void);

unsigned int RM_WIFI_dpm_ptim_event_get(void)
{
	return (unsigned int)ptim_event;
}

/* Check if wakeup is from sleep3 or DPM_sleep */
uint32_t RM_WIFI_dpm_ptim_event_get_pre(void)
{
    /* Direct access to pTIM STATUS area */
    return *((uint32_t *) PTIM_STATUS_AREA);
}

/**
 * IP address is saved to retention memroy For ARP Filtering.
 * &
 * Subnet Broadcast address is saved to retention memroy For GARP packet generating.
 */
void RM_WIFI_dpm_arp_filter_set(unsigned long accept_addr, unsigned long subnet_addr)
{
//    ULONG     tmp_ip_addr = accept_addr;
    UINT32     sub_bcast;

    /**(UINT *)(rtm_ipaddr_offset) = (  (tmp_ip_addr>>24)&0xff)
            | ((tmp_ip_addr<<8)&0xff0000)
            | ((tmp_ip_addr>>8)&0xff00)
            | ((tmp_ip_addr<<24)&0xff000000);*/
    //dpm_set_env_ip(GET_DPMP(), tmp_ip_addr);
#if LWIP_IPV4
    romac4rtos_set_ipv4(accept_addr, subnet_addr);
#endif

    /* Subnet broadcast address Calculation */
    sub_bcast = accept_addr | (subnet_addr ^ 0xffffffff);

    /**(UINT *)(rtm_subnet_addr_offset) = (  (sub_bcast>>24)&0xff)
                                | ((sub_bcast<<8)&0xff0000)
                                | ((sub_bcast>>8)&0xff00)
                                | ((sub_bcast<<24)&0xff000000);*/
    //dpm_set_env_ap_ip(GET_DPMP(), sub_bcast);

#ifdef FOR_DEBUG
    printf(YELLOW_COLOR " [%s] Set arp (0x%x) \n" CLEAR_COLOR, __func__, sub_bcast);
#endif

    romac4rtos_set_arp(0, sub_bcast);	// Unknown gw_mac

    return;
}


void RM_WIFI_dpm_ptim_event_set(int event)
{
	ptim_event = event;
}

/** Current Received Bcn Count. * Max value is 20
 */
unsigned int RM_WIFI_dpm_get_cur_bcn_count()
{
#if 0    // TO_DO
    struct set_beacon_int_req *set_beacon_int = dpm_get_mm_set_beacon_int_req(GET_DPMP());

    uint8_t inst_nbr = set_beacon_int->inst_nbr;
    struct vif_info_tag *vif_entry_boot = &vif_info_tab[inst_nbr];

    return vif_entry_boot->u.sta.bcn_cnt;
#else
    extern int dpm_ops_get_bcn_exist(void);
    return dpm_ops_get_bcn_exist();

#endif

}

int RM_WIFI_dpm_supp_is_connected(void)
{
    if (RM_PMGR_W_rtm_exist() == pdFAIL || RM_PMGR_W_dpm_is_enabled() == pdFALSE) {
        /* Unsupport RTM */
        if (dpm_wpa_supp_state == WPA_COMPLETED)
            return pdPASS;
        else
            return pdFAIL;
    }

    if (RTM_FLAG_PTR->dpm_supp_state == WPA_COMPLETED)
        return pdPASS;

    return pdFAIL;
}

void RM_WIFI_dpm_supp_conn_state_set(int supp_state)
{
    dpm_wpa_supp_state = supp_state;

    if (RM_PMGR_W_rtm_exist() == pdFAIL) {
        /* Unsupport RTM */
        return;
    }

    RTM_FLAG_PTR->dpm_supp_state = supp_state;
}

void RM_WIFI_dpm_conn_info_clear(void)
{
    if (RM_PMGR_W_rtm_exist() == pdFAIL) {
        /* Unsupport RTM */
        return;
    }

    /* Supplicant Basic WiFi infomation */
    memset((void *)RTM_SUPP_CONN_INFO_PTR, 0, CONN_INFO_ALLOC_SZ);

    /* Supplicant Key infomation */
    memset((void *)RTM_SUPP_KEY_INFO_PTR, 0, KEY_INFO_ALLOC_SZ);

    if (   RM_PMGR_W_dpm_is_wakeup() == pdFALSE
#if !defined ( __DISABLE_DPM_ABNORM__ )
        || get_last_abnormal_cnt() > 0
#endif // !__DISABLE_DPM_ABNORM__
       )
    {
        /* Net mode - DHCPC : 1 */
        if (RTM_SUPP_NET_INFO_PTR->net_mode == 1) {
            /* IP Address */
            memset((void *)RTM_SUPP_IP_INFO_PTR, 0, NET_IP_ALLOC_SZ);
        }
    }

#if defined (__SUPPORT_IPV4__)
    /* ARP Table */
    memset((void *)RTM_ARP_PTR, 0, ARP_ALLOC_SZ);
#endif // __SUPPORT_IPV4__
    RM_PMGR_W_dpm_sleep_ready_clear(REG_NAME_DPM_SUPPLICANT);
}


/** auto arp enable primitive
 */
void RM_WIFI_dpm_ptim_auto_arp_enable(int mode)
{
    if (romac4rtos_get_autoarp() == mode)  {
#ifdef FOR_DEBUG
        PRINTF(YELLOW_COLOR " [DPM Auto ARP TX] Already registered\n" CLEAR_COLOR);
#endif
        return;
    }

    //if (mode)
        //dpm_set_env_arp_en(dpmp);
    //else
        //dpm_reset_env_arp_en(dpmp);
    romac4rtos_set_autoarp(mode);

    //dpm_set_env_arp_period_ka(dpmp, dpm_cmd_autoarp_period);
    romac4rtos_set_autoarp_period(dpm_cmd_autoarp_period);

#ifdef FOR_DEBUG
    PRINTF(YELLOW_COLOR " [%s] Auto ARP enabled(%d)\n" CLEAR_COLOR, __func__, dpm_cmd_autoarp_period);
#endif
}

int RM_WIFI_dpm_supp_state_get(void)
{
    if (RM_PMGR_W_rtm_exist() == pdFAIL || RM_PMGR_W_dpm_is_enabled() == pdFALSE) {
        /* Unsupport RTM */
        return dpm_wpa_supp_state;
    }

    return RTM_FLAG_PTR->dpm_supp_state;
}
#else
#include "FreeRTOS.h"
#include "custom_config_sdk.h"
#include "bsp_common.h"

/* Dummy functions for rwnx_drv / macsw */
unsigned int RM_WIFI_dpm_ptim_event_get(void)
{
	return 0;
}

bool RM_PMGR_W_IsSleep3Wakeup(void)
{
    return pdFALSE;
}

int RM_PMGR_W_dpm_is_wakeup(void)
{
    return pdFALSE;
}

int RM_PMGR_W_dpm_wakeup_src_get(void)
{
    return BSP_WAKEUP_SOURCE_POR;
}

fsp_err_t RM_PMGR_W_dpm_job_name_set(char * dpm_name, int dpm_port)
{
    return FSP_SUCCESS;
}

int RM_PMGR_W_dpm_sleep_ready_set(char *dpm_name) 
{
    return 0;
}

int RM_PMGR_W_dpm_sleep_ready_clear(char *dpm_name)
{
    return 0;
}

int RM_PMGR_W_dpm_is_enabled(void)
{
    return pdFALSE ;
}

int RM_PMGR_W_dpm_wakeup_type_get(int do_print)
{
    return 0;
}

fsp_err_t RM_PMGR_W_rtm_static_get(int key, long* p_value_l, unsigned long long* p_value_ull, void** pp_ptr)
{
    return FSP_ERR_NOT_FOUND;
}

int RM_PMGR_W_dpm_sleep_is_started(void)
{
    return pdFALSE;
}

char *RM_PMGR_W_dpm_port_is_set(unsigned int port_number)
{
    return (char *)NULL;
}

int RM_PMGR_W_dpm_rcv_ready_get(char *dpm_name)
{
    return pdTRUE;
}

int RM_PMGR_W_dpm_sleep_is_set(char *mod_name)
{
    return pdFALSE;
}

#endif /* CFG_PMGR */

