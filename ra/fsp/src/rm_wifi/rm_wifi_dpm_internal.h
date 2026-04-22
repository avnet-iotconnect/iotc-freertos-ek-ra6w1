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

#ifndef RM_WIFI_DPM_INTERNAL_H
#define RM_WIFI_DPM_INTERNAL_H

#if CFG_PMGR
/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include "sleep_mgmt_regs.h"
#include "romac4rtos.h"

#include "bsp_api.h"
#include "FreeRTOS.h"
#include "custom_config_sdk.h"

#include <stdarg.h>

#include "defs.h"
#include "lwip/err.h"

#include "rm_pmgr_w_instance.h"

#include "ra6w1_dpm_system.h"
#include "common_def.h"
#include "rm_vee_flash_w_rrq_nvram.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

#define DPM_SOCK_MAX_UDP (8)

/***********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

void RM_WIFI_dpm_arp_filter_set(unsigned long accept_addr, unsigned long subnet_addr);

void RM_WIFI_dpm_ptim_event_set(int event);

unsigned int RM_WIFI_dpm_ptim_event_get(void);

uint32_t RM_WIFI_dpm_ptim_event_get_pre(void);

unsigned int RM_WIFI_dpm_get_cur_bcn_count();

int	RM_WIFI_dpm_supp_state_get(void);

int	RM_WIFI_dpm_supp_is_connected(void);

void RM_WIFI_dpm_supp_conn_state_set(int state);

void RM_WIFI_dpm_conn_info_clear(void);

void RM_WIFI_dpm_ptim_auto_arp_enable(int mode);
#else

/* Dummy functions for rwnx_drv / macsw */
unsigned int RM_WIFI_dpm_ptim_event_get(void);

#endif /* CFG_PMGR */

#endif/*RM_WIFI_DPM_INTERNAL_H*/