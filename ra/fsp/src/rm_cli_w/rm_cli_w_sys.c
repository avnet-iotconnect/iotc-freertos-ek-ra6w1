/**
 ****************************************************************************************
 *
 * @file rm_cli_w_sys.c
 *
 * @brief SYSTEM command functions
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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <event_groups.h>
#include <semphr.h>
#include <task.h>
#include <timers.h>
#include <interrupts.h>
#include "rm_cli_w_utils.h"
#include "rm_cli_w_sys.h"
#include "rm_cli_w_debug_utils.h"
#include "sdk_defs.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SEGGER_RTT.h"

#include "sys_clock_mgr.h"

#if CFG_PMGR
#include "r_pm_if.h"
#endif /* CFG_PMGR */

#include "common_def.h"
#include "rm_vee_flash_w_rrq_nvram.h"
#ifdef RM_MAP_PERSISTANT_W
#include "rm_map_persistant_w.h"
#endif

#if CFG_PMGR
extern int RM_PMGR_W_dpm_is_enabled(void);
#endif /* CFG_PMGR */

///////////////////////////////////////////////////////////////////////////////
#define TEST_MAC_CLK_SETUP      (0)
#define TEST_MAC_PWR_SETUP      (0)


static void ulltoa(unsigned long long value, char *buf, int radix)
{
    char    tmp[64 + 1];        /* Lowest radix is 2, so 64-bits plus a null */
    char    *p1 = tmp, *p2;
    static const char xlat[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    if (!buf) {
        return;
    }

    if (radix < 2 || radix > 36) {
        return;
    }

    do {
        *p1++ = xlat[value % (unsigned)radix];
    } while((value /= (unsigned)radix));

    for(p2 = buf; p1 != tmp; *p2++ = *--p1) {
        /* nothing to do */
    }

    *p2 = '\0';
}

static char *lltoa(long long value, char *buf, int radix)
{
    char *save = buf;

    if (!buf) {
        return NULL;
    }

    if (radix == 10 && value < 0) {
        *buf++ = '-';
        value = -value;
    }

    ulltoa(value, buf, radix);

    return save;
}
#define MAX_CHAR_FOR_64BITS (21)
///////////////////////////////////////////////////////////////////////////////

static bool rtc_command(int argc, const char **argv)
{
    (void) argc;
    (void) argv;

    uint64_t rtccnt;
    char str_dec[MAX_CHAR_FOR_64BITS];
    char str_hex[MAX_CHAR_FOR_64BITS];
    bsp_wakeup_source_mask_t curwkupsrc = R_BSP_WakeupSourceGet();

    printf("current wakup source : %x\n", curwkupsrc);
    printf("  sleep id : %d, (%04x)\n", (int)bsp_prv_pd_sleep_id_get(), (unsigned int)bsp_prv_pd_sleep_id_get());
    printf("  retention flag : %ld\n",   R_BSP_RetainedMemFlagGet());

    rtccnt = R_BSP_SystemRtcCountGet();
    printf("  R_BSP_SystemRtcCountGet : %s (%s)\n",   lltoa(rtccnt, str_dec, 10), lltoa(rtccnt, str_hex, 16));

    rtccnt = bsp_prv_pd_wakeup_counter_get();
    printf("  bsp_prv_pd_wakeup_counter_get : %s, %s\n",   lltoa(rtccnt, str_dec, 10), lltoa(rtccnt, str_hex, 16));

    return true;
}
///////////////////////////////////////////////////////////////////////////////

#if (dg_configUSE_CLOCK_MGR == 1)
extern void hal_machw_setfreq(uint8_t newfreq);
extern void hal_machw_checkfreq(uint8_t use_printf);
extern void crm_set_mac_freq(int freq);
static void	mycevt_callback_test(sys_clk_is_t clksrc, uint32_t freq, void *param)
{
    (void) param;

    int mac_core_clock_freq;
    const char *clksrclist[] = {
        "IS_NONE", "IS_XTAL40M", "IS_LP" ,
        "IS_PLL240M", "IS_PLL192M", "IS_PLL160M",
        "IS_PLL137M", "IS_PLL106M", "INVALID"
    };

    printf("SYS_CLK_%s, freq: %ld\n",
           clksrclist[(uint32_t)(clksrc - SYS_CLK_IS_NONE)] , freq);

    if (freq == 0) {
            return;
    }

    if (freq > 40) {
        mac_core_clock_freq = 40;
    } else {
        mac_core_clock_freq = 20;
    }

    crm_set_mac_freq(mac_core_clock_freq);
    hal_machw_setfreq(mac_core_clock_freq);
    printf("MAC core clock freq: %dMhz\n", mac_core_clock_freq);
    hal_machw_checkfreq(1);
}
#endif

typedef enum cpu_clock_chg_ret_type {
    apply_nok,
    apply_ok,
    req_reboot
} cpu_clock_chg_ret_t;

static bool chk_support_cpu_clk(int cpu_clock)
{
    switch (cpu_clock) {
        case cpuclk_2M:          //!< 2.5 MHz, divided by 16 in XTAL
        case cpuclk_5M:          //!< 5  MHz, divided by 8 in XTAL
        case cpuclk_10M:         //!< 10 MHz, divided by 4 in XTAL
        case cpuclk_20M:         //!< 20 MHz, divided by 2 in XTAL

        case cpuclk_26M:         //!< 26 MHz, divided by 4 in SYSCLK 106MHz
        case cpuclk_34M:         //!< 34 MHz, divided by 4 in SYSCLK 137MHz
        case cpuclk_40M:         //!< 40 MHz, divided by 4 in SYSCLK 160MHz

        case cpuclk_53M:         //!< 53 MHz, divided by 2 in SYSCLK 106MHz
        case cpuclk_68M:         //!< 68 MHz, divided by 2 in SYSCLK 137MHz
        case cpuclk_80M:         //!< 80 MHz, divided by 2 in SYSCLK 160MHz
        
        case cpuclk_106M:        //!< 106 MHz
        case cpuclk_137M:        //!< 137 MHz
        case cpuclk_160M:        //!< 160 MHz
#if (HW_DESCOPED_CLOCK == 1)
        case cpuclk_192M:      //!< 192 MHz, descoped
        case cpuclk_240M:      //!< 240 MHz, descoped
#endif // HW_DESCOPED_CLOCK
           return TRUE;

        default:
            return FALSE;
    }
}

static cpu_clock_chg_ret_t cpu_clock_change(int clock_rate_mhz)
{
    static cm_clock_callback_t mycevt_callback;
    cpu_clk_t curcpuclk ;

    if (chk_support_cpu_clk(clock_rate_mhz)) {
        if ((cm_cpu_clk_get() > cpuclk_26M) && (clock_rate_mhz > cpuclk_26M)) {

            mycevt_callback.func = &mycevt_callback_test;
            mycevt_callback.priority = cm_cb_priority_coupled_hyper_loosely;
            mycevt_callback.param = NULL;

            cm_register_clock_callback(&mycevt_callback);

            cm_cpu_clk_set(clock_rate_mhz);

            curcpuclk = cm_cpu_clk_get();

            printf("CPU Clock : %s %d.%d MHz\n",
                (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_PLL)?"PLL":"XTAL"),
                curcpuclk, curcpuclk == cpuclk_2M ? 5:0);

            cm_deregister_clock_callback(&mycevt_callback);
        } else if ((cm_cpu_clk_get() < cpuclk_26M) && (clock_rate_mhz < cpuclk_26M)) {

            cm_cpu_clk_set(clock_rate_mhz);

            curcpuclk = cm_cpu_clk_get();

            printf("CPU Clock : %s %d.%d MHz\n",
                (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_PLL)?"PLL":"XTAL"),
                curcpuclk, curcpuclk == cpuclk_2M ? 5:0);            
        } else {
            return req_reboot;
        }
    } else {
        return apply_nok;
    }

    return apply_ok;
}

static void print_support_cpu_clk(void)
{
    printf("Support clock rate:\n");
    printf("\t%d(%d.5)", cpuclk_2M, cpuclk_2M);        //!< 2.5 MHz, divided by 16 in XTAL
    printf(", %d", cpuclk_5M);          //!< 5  MHz, divided by 8 in XTAL
    printf(", %d", cpuclk_10M);         //!< 10 MHz, divided by 4 in XTAL
    printf(", %d", cpuclk_20M);         //!< 20 MHz, divided by 2 in XTAL

    printf(", %d", cpuclk_26M);         //!< 26 MHz, divided by 4 in SYSCLK 106MHz
    printf(", %d", cpuclk_34M);         //!< 34 MHz, divided by 4 in SYSCLK 137MHz
    printf(", %d", cpuclk_40M);         //!< 40 MHz, divided by 4 in SYSCLK 160MHz

    printf(", %d", cpuclk_53M);         //!< 53 MHz, divided by 2 in SYSCLK 106MHz
    printf(", %d", cpuclk_68M);         //!< 68 MHz, divided by 2 in SYSCLK 137MHz
    printf(", %d", cpuclk_80M);         //!< 80 MHz, divided by 2 in SYSCLK 160MHz

    printf(", %d", cpuclk_106M);        //!< 106 MHz
    printf(", %d", cpuclk_137M);        //!< 137 MHz
    printf(", %d", cpuclk_160M);        //!< 160 MHz
#if (HW_DESCOPED_CLOCK == 1)
    printf(", %d", cpuclk_192M);        //!< 192 MHz, descoped
    printf(", %d", cpuclk_240M);        //!< 240 MHz, descoped
#endif
    printf(" Mhz\n");
}

// cpu clock save nvram
static int set_cpu_clock_nvram(int cpu_clock)
{
    int status = 0;

    if (chk_support_cpu_clk(cpu_clock)) {
#ifdef RM_MAP_PERSISTANT_W
        status = RM_MAP_PERSISTANT_W_Write_UINT(RM_MAP_PERSISTANT_W_get_ctrl(), ENV_GROUP_BOOTCFG,
		                              "clk.cpu", (void *)&cpu_clock, 4);
#endif /* RM_MAP_PERSISTANT_W */
    }

    if (status != FSP_SUCCESS) {
        return FALSE;
    }

    return TRUE;
}

static bool cmd_cpu_clock_change(int argc, const char **argv)
{
#if (dg_configUSE_CLOCK_MGR == 1)
    uint32_t        clock_mhz;
    cpu_clk_t       curcpuclk ;
#if 0 // Unused code
    static cm_clock_callback_t mycevt_callback;
#endif // 0

    curcpuclk = cm_cpu_clk_get();

    if ((argc > 1) && (parse_u32(argv[1], &clock_mhz) == true)) {
        switch (cpu_clock_change(clock_mhz)) {
            case req_reboot:
                if (argc == 3 && strcmp(argv[2], "save") == 0) {    // save nvram
                    printf("\nCPU's clock speed keeps changing on reboot.\n");
                } else {
                    printf("\nTo change the current CPU clock(%ldMhz) to %ldMhz, save parameter is required.\n\n", (long int)curcpuclk, clock_mhz);
                    return false;
                }
                //No Break
            case apply_ok:            {
                if (argc == 3 && strcmp(argv[2], "save") == 0) {    // save nvram
                    if (set_cpu_clock_nvram(clock_mhz)) {
                        printf("NVRAM save OK\n");
                    } else {
                        printf("NVRAM save failure\n");
                    }
                }
                break;
            }

            case apply_nok:

            default:
            {
                printf("\nUnsupported rate: %ld\n", clock_mhz);
                print_support_cpu_clk();
                break;
            }
        }
    } else if (argc == 1) {
        curcpuclk = cm_cpu_clk_get();
        printf("CPU clock: %s %d.%d MHz\n\n",
            (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_PLL)?"PLL":"XTAL"),
            curcpuclk, curcpuclk == cpuclk_2M ? 5:0);
    } else if (strcmp(argv[1], "help") == 0) {
        printf("\nUsage: clock [<rate>|help] [<none>|save]\n");
        print_support_cpu_clk();
    } else {
        return false;
    }

    return true;
#else
    printf("enable dg_configUSE_CLOCK_MGR !!\n");
    return false;
#endif
}


static int cmd_tcs_xtal_on_otp(int argc, const char *argv[])
{
    (void) argc;

    uint32_t value = 0;
    uint32_t option_check=0;
    
    if (strncmp(argv[1], "wr", 2) == 0) {
        option_check = htoi((char *)argv[3]);
        value = htoi((char *)argv[2]);

        if(bsp_tcs_otp_write_xtal(&value, option_check))
        {
            printf("successed\r\n");
            return true;
        }else{
            printf("failed\r\n");
            return false;   
        }    
    }else if(strncmp(argv[1], "rd",2) == 0) {
        if(bsp_tcs_otp_read_xtal(&value))
        {
            printf("\r\n0x%08x\r\n", (unsigned int)value);
            return true;
        }else{
              return false;
        }
    }else
    {
          return false;
    }
}
   
static int cmd_tcs_mac_on_otp(int argc, const char *argv[])
{
    (void) argc;

    uint32_t i;
    uint32_t option_check=0;
    uint32_t mac_addr[2] ={0,};
    uint8_t tmp_mac[6] = {0,};

    if (strncmp(argv[1], "wr", 2) == 0) {
        //option_check = htoi((char *)argv[3]);
        for (i = 0; i < 6; i++) {
		    tmp_mac[i] = toint(argv[2][i*2]) << 4 | toint(argv[2][(i*2)+1]);
	    }

        // convert  MAC_TYPE
        memcpy(&mac_addr[0], tmp_mac + 2, 4);
        memcpy(((uint8_t *)(&mac_addr[1])) + 2, tmp_mac, 2);   

        mac_addr[0] = (tmp_mac[2]<<24 & 0xff000000) | (tmp_mac[3] <<16 & 0xff0000) |  (tmp_mac[4]<<8 & 0xff00)  | (tmp_mac[5] & 0xff);
        mac_addr[1] = (tmp_mac[0] <<8 & 0xff00) | (tmp_mac[1] & 0xff);

        printf("mac_addr raw %08lx,%08lx\r\n", mac_addr[0], mac_addr[1]);
        
        if(bsp_tcs_otp_write_mac(mac_addr, option_check))
        {
            printf("successed\r\n");
            return true;

        }else{
            printf("failed\r\n");
            return false;   
        }    
    } else if(strncmp(argv[1], "rd", 2) == 0) {
        if (bsp_tcs_otp_read_mac(mac_addr)) {
            printf("\r\n%04x%08x\r\n", (unsigned int)(mac_addr[1] & 0xffff), (unsigned int)mac_addr[0]);
            return true;
        } else {
            return false;   
        }
    } else {
        return false;
    }
    return false;
}

static bool cmd_tcs(int argc, const char **argv)
{
    if (argc > 3)
    {
        if((strcmp(argv[1], "wr.xtal"))==0)
        {
            cmd_tcs_xtal_on_otp(argc, argv);
        }else if((strcmp(argv[1], "wr.mac"))==0) 
        {
            cmd_tcs_mac_on_otp(argc, argv);
        }else{
            return false;
        }
    }else if (argc > 1)
    {
        if((strcmp(argv[1], "rd.xtal"))==0) 
        {
            cmd_tcs_xtal_on_otp(argc, argv);
        }else if((strcmp(argv[1], "rd.mac"))==0) 
        {
            cmd_tcs_mac_on_otp(argc, argv);
        }else if((strcmp(argv[1], "info"))==0) 
        {
            extern void bsp_tcs_info_printf(bool match);
            bsp_tcs_info_printf(true);
        }else{
            return false;
        }
    }else{
        printf("---example----\r\n");
        printf("tcs info\r\n");
        printf("tcs wr.xtal 0020076f [0~3]  - 0: force write on otp...\r\n");
        printf("tcs wr.mac 749050b0c100 [0~3]  - 0: force write on otp...\r\n");
        printf("tcs rd.xtal\r\n");
        printf("tcs rd.mac\r\n");
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
#define  WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE    (0x60d00030)
#define  WIFI_RFHPI_MAIN_CTRL_DEPENDENCY            (0x60d00014)
#define REG_PL_WR(addr, value)       (*(volatile uint32_t *)((addr))) = (value)
#define REG_PL_RD(addr)              (*(volatile uint32_t *)((addr)))
static bool cmd_hpi_test_command(int argc, const char **argv)
{
#define PRINT_REG_VALUE(x)        do {    \
                                      printf(#x "(%p) => %08x\n", &(x), (unsigned int)(x)); \
                                  } while(0)

#define PRINT_REG_SET(x, v)       do {    \
                                      printf(#x "(%p) <= %08x\n", &(x), (unsigned int)(v)); \
                                      (x) = (v);                              \
                                      printf(#x "(%p) => %08x\n", &(x), (unsigned int)(x)); \
                                  } while(0)

    if ( (argc > 1) && (strcmp(argv[1], "hpion") == 0) ){
        // 0x60d00030 0x86  ; MAIN_CTRL_INTERFACE_MAN_MODE
        //PRINT_REG_SET(WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE, 0x86);
        printf("WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE));
        REG_PL_WR(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, 0x86);
        printf("WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE));

        // 0x60d00014 0x198 ; MAIN_CTRL_DEPENDENCY
        //PRINT_REG_SET(WIFI_RFHPI->MAIN_CTRL_DEPENDENCY, 0x198);
        printf("WIFI_RFHPI->MAIN_CTRL_DEPENDENCY (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY));
        REG_PL_WR(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, 0x198);
        printf("WIFI_RFHPI->MAIN_CTRL_DEPENDENCY (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY));

        // 0x40038124 0x4AC00001 ; CNT_TESTI_REG
        PRINT_REG_SET(RTC->CNT_TESTI_REG, 0x4AC00001);
        // 0x400B0098 0x0   ; P1_15_MODE_REG
        PRINT_REG_SET(GPIO->P1_15_MODE_REG, 0x0);
        // 0x400C0274 0x8840 ; TEST_CFG_REG
        PRINT_REG_SET(CRG_COM->TEST_CFG_REG, 0x8840);
    } else if ( (argc > 2) && (strcmp(argv[1], "rfhpi.if") == 0) ){
        uint32_t regval;
        if (parse_u32(argv[2], &regval) == true){
            //PRINT_REG_SET(WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE, regval);
            printf("WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE (%p) => %08x\n",
                (void *)WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE));
            REG_PL_WR(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, regval);
            printf("WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE (%p) => %08x\n",
                (void *)WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE));
        }
    } else if ( (argc > 2) && (strcmp(argv[1], "rfhpi.dp") == 0) ){
        uint32_t regval;
        if (parse_u32(argv[2], &regval) == true){
            //PRINT_REG_SET(WIFI_RFHPI->MAIN_CTRL_DEPENDENCY, regval);

            printf("WIFI_RFHPI->MAIN_CTRL_DEPENDENCY (%p) => %08x\n",
                (void *)WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY));
            REG_PL_WR(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, regval);
            printf("WIFI_RFHPI->MAIN_CTRL_DEPENDENCY (%p) => %08x\n",
                (void *)WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY));
        }
    } else if ( (argc > 2) && (strcmp(argv[1], "rtc.test") == 0) ){
        uint32_t regval;
        if (parse_u32(argv[2], &regval) == true){
            PRINT_REG_SET(RTC->CNT_TESTI_REG, regval);
        }
    } else if ( (argc > 2) && (strcmp(argv[1], "gpio.p1_15") == 0) ){
        uint32_t regval;
        if (parse_u32(argv[2], &regval) == true){
            PRINT_REG_SET(GPIO->P1_15_MODE_REG, regval);
        }
    } else if ( (argc > 2) && (strcmp(argv[1], "crg.test") == 0) ){
        uint32_t regval;
        if (parse_u32(argv[2], &regval) == true){
            PRINT_REG_SET(CRG_COM->TEST_CFG_REG, regval);
        }
    } else {
        //PRINT_REG_VALUE(WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE);
        printf("WIFI_RFHPI->MAIN_CTRL_INTERFACE_MAN_MODE (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_INTERFACE_MAN_MODE));
        //PRINT_REG_VALUE(WIFI_RFHPI->MAIN_CTRL_DEPENDENCY);
        printf("WIFI_RFHPI->MAIN_CTRL_DEPENDENCY (%p) => %08x\n",
            (void *)WIFI_RFHPI_MAIN_CTRL_DEPENDENCY, (unsigned int)REG_PL_RD(WIFI_RFHPI_MAIN_CTRL_DEPENDENCY));

        PRINT_REG_VALUE(RTC->CNT_TESTI_REG);
        PRINT_REG_VALUE(GPIO->P1_15_MODE_REG);
        PRINT_REG_VALUE(CRG_COM->TEST_CFG_REG);
    }

    return true;
}

static const debug_handler_t sys_handlers[] = {
    { "rtc",          "RTC cmds, ...",                     rtc_command          },
    { "clock" ,       "[<rate>|help] [<none>|save]",       cmd_cpu_clock_change },  
    { "test" ,        "hpi.test cmds",                     cmd_hpi_test_command }, 
#if dg_configSYS_TCS
    { "tcs" ,         "tcs.test cmds ",                    cmd_tcs }, 
#endif
    { NULL },
};

bool sys_command(int argc, const char *argv[], void *user_data)
{
    (void) user_data;

    return debug_handle_message(argc, argv, sys_handlers);
}
