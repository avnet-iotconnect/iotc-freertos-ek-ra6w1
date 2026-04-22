/**
 ****************************************************************************************
 *
 * @file rm_cli_w_dpm.c
 *
 * @brief DPM command functions
 *
 * Copyright (c) 2023 Renesas Electronics. All rights reserved.
 *
 * This software ("Software") is owned by Renesas Electronics.
 *
 * By using this Software you agree that Renesas Electronics retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Renesas Electronics is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Renesas Electronics products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * RENESAS ELECTRONICS BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */

#if CFG_PMGR
#include "FreeRTOS.h"
#include "custom_config_sdk.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "task.h"
#include "sdk_defs.h"

#include "rm_cli_w_utils.h"
#include "rm_cli_w_debug_utils.h"

#include "rm_pmgr_w_instance.h"
#include "dpmty_patch.h"
#include "lwip/ip_addr.h"
#include "rm_lwip_w_helper.h"
#ifdef RM_MAP_PERSISTANT_W
#include "rm_map_persistant_w.h"
#endif
#include "lwip/sockets.h"

extern bool RM_PMGR_W_rtm_exist(void);


/******************************************************************************
 *
 * Variables
 *
 ******************************************************************************/


/******************************************************************************
 *
 * Internal functions
 *
 ******************************************************************************/
static int chk_ssid_chg_state(void)
{
    dpm_supp_conn_info_t    * dpm_supp_conn_info_loc;
    char    *nvram_ssid = NULL;
    char    *nvram_psk = NULL;
    int    rtn;

    RM_PMGR_W_rtm_static_get(RTM_STATIC_KEY_SUPPL_CONN_INFO_PTR, NULL, NULL, (void**)(&dpm_supp_conn_info_loc));
    
    if (dpm_supp_conn_info_loc == NULL) {
        return 0;
    }

    /* Compare the status of ssid changing */
#ifdef RM_MAP_PERSISTANT_W
    RM_MAP_PERSISTANT_W_Read_STRING(RM_MAP_PERSISTANT_W_get_ctrl(), ENV_GROUP_WIFICFG, (char *)NVR_KEY_SSID_0, &nvram_ssid);
#endif

    if (nvram_ssid == NULL && strlen((char const *)(dpm_supp_conn_info_loc->ssid)) > 0) {
        rtn = -1;
    } else if (nvram_ssid != NULL && strcmp(nvram_ssid, (char const *)(dpm_supp_conn_info_loc->ssid)) == 0) {
        rtn = 0;
    } else {
        rtn = -1;
    }
#if 0
    if (nvram_ssid) {
        vPortFree(nvram_ssid);
    }
#endif /* 0 */

    /* Compare the status of psk changing */
#ifdef RM_MAP_PERSISTANT_W
    RM_MAP_PERSISTANT_W_Read_STRING(RM_MAP_PERSISTANT_W_get_ctrl(), ENV_GROUP_WIFICFG, (char *)NVR_KEY_ENCKEY_0, &nvram_psk);
#endif

    if (nvram_psk == NULL && strlen((char const *) (dpm_supp_conn_info_loc->psk)) > 0) {
        rtn = -1;
    } else if (nvram_psk != NULL
             && strcmp(nvram_psk, (char const *) (dpm_supp_conn_info_loc->psk)) == 0) {
        rtn = 0;
    } else {
        rtn = -1;
    }
#if 0
    if (nvram_psk) {
        vPortFree(nvram_psk);
    }
#endif /* 0 */
    
    return rtn;
}

/******************************************************************************
 *
 * Command
 *
 ******************************************************************************/
static bool cmd_dpm_onoff(int argc, char *argv[])
{
    (void) argc;

    if (strcasecmp(argv[0], "ON") == 0) {
        if (!RM_PMGR_W_rtm_exist()) {
            printf("FAIL\n - RTM doesn't exist.\n");
            return pdFALSE;
        }
        //OAL_DPM_FULL_BOOT. DPM Mode on.
        vTaskDelay(portCONVERT_MS_2_TICKS(50));
        RM_PMGR_W_dpm_enable();
        vTaskDelay(portCONVERT_MS_2_TICKS(100));

        /* If connection config was changed,
         * need to reboot to start as normal mode
         */
        if (!RM_PMGR_W_dpm_is_wakeup() || chk_ssid_chg_state() == -1) {
            reset();
        }
    } else if (strcasecmp(argv[0], "OFF") == 0) {

        RM_PMGR_W_dpm_hold();

        //OAL_DPM_NORMAL_BOOT. DPM Mode off.
        vTaskDelay(portCONVERT_MS_2_TICKS(5));

        /* Disable DPM running mode */
        RM_PMGR_W_dpm_disable();
        vTaskDelay(portCONVERT_MS_2_TICKS(100));

        reset();
    }
    return pdTRUE;
}

static bool cmd_dpm_hold(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

	RM_PMGR_W_dpm_hold();

	return pdTRUE;
}

static bool cmd_dpm_resume(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

	RM_PMGR_W_dpm_sleep_resume();

	return pdTRUE;
}

static bool cmd_dpm_info(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

	RM_PMGR_W_dpm_dbg_info_get();

	return pdTRUE;
}

/**
 * command for multicast address filter in TIM as white list
 */
static bool cmd_set_mc_filter(int argc, char *argv[])
{
    extern int select_ipaddr_type_from_str(char* str, struct sockaddr_storage *ipaddr);

    struct sockaddr_storage valid_ip = {0x00,};
    ip4_addr_t tmp_addr4;
    ip6_addr_t tmp_addr6;

    unsigned long mc_filter_addr = 0;
    int ip_type = 0;

    if (argc < 2) {
        PRINTF("- Usage : set_mc  [IPv4|IPv6(mc addr)]\n");
        return false;
    }

    ip_type = select_ipaddr_type_from_str(argv[1], &valid_ip);

    /* Common */
    if ((strcmp(argv[1], "0") != 0)
#if defined ( __SUPPORT_IPV4__ )
            && (ip_type != IPADDR_TYPE_V4)
#endif // __SUPPORT_IPV4__
#if defined ( __SUPPORT_IPV6__ )
            && (ip_type != IPADDR_TYPE_V6)
#endif // __SUPPORT_IPV6__
       )
    {
        PRINTF("- Usage : set_mc  [IPv4|IPv6(mc addr)]\n");
        return false;
    }

    if (ip_type == IPADDR_TYPE_V4) {
        ip4addr_aton(argv[1], &tmp_addr4);
        mc_filter_addr = lwip_htonl(ip4_addr_get_u32(&tmp_addr4)) & 0xffffffff;
        RM_WIFI_dpm_ptim_mc_filter_set(mc_filter_addr);
    } else if (ip_type == IPADDR_TYPE_V6) {
        ip6addr_aton(argv[1], &tmp_addr6);
        RM_WIFI_dpm_ptim_mcv6_filter_set((uint16_t *)tmp_addr6.addr);
    }

    return true;
}

static bool cmd_tcpd(int argc, char *argv[])
{
    uint8_t usage_en = 0;
    int32_t no;
    uint16_t dport;

    switch (argc) {
    case 3:
        dport = atoi(argv[2]);
        no = atoi(argv[1]);
        romac4rtos_set_tcport(no, dport);
        break;
    default:
        usage_en = 1;
        break;
    }

    if (usage_en) {
        printf("Usage: %s [no] [dest port]\n", argv[0]);
        return pdTRUE;
    }

    printf("%s::tcp_dest_port%d=%d(%04x)\n", argv[0], (int)no,
           romac4rtos_get_tcport(no), romac4rtos_get_tcport(no));

    return pdTRUE;
}

static bool cmd_udpd(int argc, char *argv[])
{
    uint8_t usage_en = 0;
    int32_t no;
    uint16_t dport;

    switch (argc) {
    case 3:
        dport = atoi(argv[2]);
        no = atoi(argv[1]);
        romac4rtos_set_udport(no, dport);
        break;
    default:
        usage_en = 1;
        break;
    }

    if (usage_en) {
        printf("Usage: %s [no] [dest port]\n", argv[0]);
        return pdTRUE;
    }

    printf("%s::udp_dest_port%d=%d(%04x)\n", argv[0],(int) no,
           romac4rtos_get_udport(no), romac4rtos_get_udport(no));

    return pdTRUE;
}

static const debug_handler_t dpm_handlers[] = {
    { "on",      "DPM On",                      (debug_callback_t)cmd_dpm_onoff       },
    { "off",     "DPM Off",                     (debug_callback_t)cmd_dpm_onoff       },
    { "hold",    "DPM Hold",                    (debug_callback_t)cmd_dpm_hold        },
    { "resume",  "DPM Resume",                  (debug_callback_t)cmd_dpm_resume      },
    { "status",  "DPM Status",                  (debug_callback_t)cmd_dpm_info        },
    { "set_mc",  "set_mc [mc_ip] ",             (debug_callback_t)cmd_set_mc_filter   },
    { "tcpd",    "TCP Dest Port",               (debug_callback_t)cmd_tcpd            },
    { "udpd",    "UDP Dest Port",               (debug_callback_t)cmd_udpd            },
    { NULL },
};

bool dpm_command(int argc, const char *argv[], void *user_data)
{
    (void) user_data;

    return debug_handle_message(argc, argv, dpm_handlers);
}

#endif /* CFG_PMGR */
