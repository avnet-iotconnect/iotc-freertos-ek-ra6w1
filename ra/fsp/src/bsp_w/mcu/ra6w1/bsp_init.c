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

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "bsp_api.h"
#include "bsp_clocks.h"
#include "bsp_io_isolation.h"
#if BSP_FEATURE_USE_TCS
 #include "bsp_tcs.h"
#endif
#include "bsp_otp.h"
#include "bsp_feature.h"
#ifdef RM_STDIO_W
#include "rm_stdio_w_cfg.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#if (__GNUC__ == 10) && (__GNUC_MINOR__ == 3)
 #define USE_MALLOC_BUG_WORKAROUND
#endif

#if (dg_configUSE_CLOCK_MGR)
#define SYSTEM_BOOT_CLK_40M (0)
#else
#define SYSTEM_BOOT_CLK_40M (1)
#endif /* dg_configUSE_CLOCK_MGR && CFG_WIFI */


/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef void (* bsp_init_func_ptr)(void);

/***********************************************************************************************************************
 * Exported global variables (defined in other files)
 **********************************************************************************************************************/

extern void R_BSP_WarmStart(bsp_warm_start_event_t event);

/*******************************************************************************************************************//**
 * Linker symbols
 *
 * @note if any of them is missing, please correct your linker script.
 **********************************************************************************************************************/
extern uint32_t __copy_table_start__;  // NOLINT
extern uint32_t __copy_table_end__;    // NOLINT
extern uint32_t __zero_table_start__;  // NOLINT
extern uint32_t __zero_table_end__;    // NOLINT
extern uint8_t  __HeapLimit;           // NOLINT
/***********************************************************************************************************************
 * Function prototypes
 **********************************************************************************************************************/

void        bsp_wdt_freeze_cfg(void);
void        SystemInitPre(void);
static void ra6w1_SystemInit(void);

#ifdef USE_MALLOC_BUG_WORKAROUND
static void malloc_and_free_heap(void);

#endif

static void ra6w1_SystemInitPost(void);

#ifdef BSP_MCU_RRQ61051
static void rrq6105x_internal_pin_init(void);
#endif /* BSP_MCU_RRQ61051 */

void ra6w1_PllOn (void);

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Add function pointers to the .preinit_array section.
 * __libc_init_array() (which is called by _start()) calls all function pointers in .preinit_array.
 **********************************************************************************************************************/
BSP_PLACE_IN_SECTION(BSP_SECTION_PREINIT) BSP_ALIGN_VARIABLE(__alignof__(bsp_init_func_ptr))
static bsp_init_func_ptr ra6w1_SystemInit_preinit_array_entry[] =
{
    ra6w1_SystemInit,
};

/*******************************************************************************************************************//**
 * Add function pointers to the .init_array section.
 * __libc_init_array() (which is called by _start()) calls all function pointers in .init_array.
 **********************************************************************************************************************/
BSP_PLACE_IN_SECTION(BSP_SECTION_INIT) BSP_ALIGN_VARIABLE(__alignof__(bsp_init_func_ptr))
static bsp_init_func_ptr ra6w1_SystemInit_init_array_entry[] =
{
    ra6w1_SystemInitPost,
};

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @internal
 * @addtogroup BSP_MCU_PRV Internal BSP Documentation
 * @ingroup RENESAS_INTERNAL
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Update system core clock based on static clock configuration and/or register settings.
 **********************************************************************************************************************/
void SystemCoreClockUpdate (void)
{
    /* TODO: this function must be updated, see TVR-398 */
    SystemCoreClock = BSP_CFG_XTAL40_HZ;
}

/*******************************************************************************************************************//**
 * Enable/disable Cortex M33 Debug Access Port depending on BSP configuration.
 **********************************************************************************************************************/
__STATIC_FORCEINLINE void bsp_prv_enable_debuggers (void)
{
    if (BSP_CFG_DEBUGGER_ENABLE)
    {
        if (bsp_is_chip_revision_a())
        {
            /*
             * Serial Wire Debug (SWD) interface: set drive current to 12 mA
             * (default was 2 mA). Note: this initialization is required only
             * for hardware revision AA (BA silicon is supposed to have correct
             * defaults). The problem with default for AA 2 mA drive current is
             * observable only with high PLL frequency (the waveform is not
             * accurate at the rising/falling edges according to scope
             * measurements).
             */

            /* Set SWCLK standard PAD drive strength to 12mA. */
            GPIO->SW0_MODE_REG_b.DS = BSP_PAD_DRIVE_STRENGTH_12MA;

            /* Set SWDIO standard PAD drive strength to 12mA. */
            GPIO->SW1_MODE_REG_b.DS = BSP_PAD_DRIVE_STRENGTH_12MA;
        }

        CRG_TOP->SYS_CTRL_REG_b.DEBUGGER_ENABLE = 1;
    }
}

__STATIC_FORCEINLINE fsp_err_t copy_table_sanity_check (void)
{
    /*
     * Ensure 4-byte alignment for all elements of each entry in the Copy Table.
     * If any of the assertions below hits, please correct your linker script
     * file accordingly!
     */
    if (0 == BSP_CFG_PRODUCTION_BUILD)
    {
        uint32_t * ptr;

        for (ptr = &__copy_table_start__; ptr < &__copy_table_end__; ptr += 3)
        {
            FSP_ASSERT(0 == (ptr[0] & 0x3)); // from
            FSP_ASSERT(0 == (ptr[1] & 0x3)); // to
            FSP_ASSERT(0 == (ptr[2] & 0x3)); // size
        }
    }

    return FSP_SUCCESS;
}

__STATIC_FORCEINLINE fsp_err_t zero_table_sanity_check (void)
{
    /*
     * Ensure 4-byte alignment for all elements of each entry in the Data Table.
     * If any of the assertions below hits, please correct your linker script
     * file accordingly!
     */
    if (0 == BSP_CFG_PRODUCTION_BUILD)
    {
        uint32_t * ptr;

        for (ptr = &__zero_table_start__; ptr < &__zero_table_end__; ptr += 2)
        {
            FSP_ASSERT((ptr[0] & 0x3) == 0); // start at
            FSP_ASSERT((ptr[1] & 0x3) == 0); // size
        }
    }

    return FSP_SUCCESS;
}

/*
 * Switch to XTAL32M, power up PD_TIM
 */
#if SYSTEM_BOOT_CLK_40M
static void enable_and_switch_to_xtal40m(void)
{
    /* TODO: this function must be updated/removed, see TVR-398 */
    /*
     * If already running on XTAL32M and PD_TIM is up
     */
    if (REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_XTAL40M)
#if (0)
        &&  REG_GETF(CRG_TOP, SYS_STATUS_REG, SYS_IS_UP)
#endif
        ) {
        /* Then there is nothing more to do here.
         * Notice this is the case when QSPI flash contains no application
         * image and we use the debugger to load the M33 image into RAM.
         * Because we halt the boot ROM loader which has already turned on
         * PD_TIM, enabled XTAL32M and switched SYS_CLK to XTAL32M.
         */
        return;
    }

#if (0)
    /* Turn on PD_TIM power domain */
    REG_SETF(CRG_TOP, PMU_CTRL_REG, TIM_SLEEP, 0);
    while (REG_GETF(CRG_TOP, SYS_STAT_REG, TIM_IS_UP)  == 0) { /*empty*/ }
#endif
    /* Switch the system clock to XTAL40M */
    hw_clk_set_sysclk(SYS_CLK_IS_XTAL40M);

    /* Wait until the switch is done */
    while(REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_XTAL40M) == 0) { /*empty*/ }
}

static void setup_clocks(void)
{
    /* TODO: this function must be updated/removed, see TVR-398 */
    /* Switch SYS_CLK to XTAL40M */
    enable_and_switch_to_xtal40m();

    REG_SETF(CRG_TOP, CLK_AMBA_REG, HCLK_DIV, 0);

    /* Set up XTAL32K as LP clock */
    REG_SETF(CRG_COM, XTAL32K_CTRL_REG, XTAL32K_ENABLE, 1);
    // removed SetBits(CLK_CTRL_REG, LP_CLK_SEL, 2);
    //SetBits(CLK_RC32K_REG, RC32K_ENABLE, 0);
}
#endif // SYSTEM_BOOT_CLK_40M

/*******************************************************************************************************************//**
 * @brief Handle WDT Freeze.
 *
 * WDT is enabled after reset by default.
 * In case r_wdog_w module is not included in the project, WDT should be disabled by BSP.
 * bsp_wdt_freeze_cfg weak function - will disable WDT in case r_wdog_w module is not included.
 * bsp_wdt_freeze_cfg normal function - will do nothing, hence WDT remains enabled, in case r_wdog_w is included.
 **********************************************************************************************************************/
BSP_WEAK_REFERENCE void bsp_wdt_freeze_cfg (void)
{
    CRG_TOP->SET_FREEZE_REG_b.FRZ_SYS_WDOG = 1;
}

/*******************************************************************************************************************//**
 * @brief Turn on the PLL.
 *
 * Turn on the pll at very first step of POR.
 * This function in on IVT region.
 **********************************************************************************************************************/
BSP_PLACE_IN_SECTION(".pllon") __attribute__((optimize("Os")))
__attribute__((section(".pllon"))) void ra6w1_PllOn (void)
{
    uint32_t time_out = 0;

    REG_SETF(RTC, LDO_ENABLE_REG, LDO_EN_LDO_PLL1, 1);
    REG_SETF(CRG_COM, XTAL40M_CTRL_REG, XTAL40M_DPLL_EN, 1);

    REG_SETF(CRG_COM, PLL1_ARM_CTRL_REG, DPLL1_EN, 1);

    while (REG_GETF(CRG_COM, PLL1_ARM_CTRL_REG, PLL_LOCK) == 0)
    {
        __asm volatile (    /* Pll lock may take up to 50us */
            "nop"           /* 40Mhz, it takes 25ns */
        );
        time_out ++;

        if (time_out > 2000)    /* time out, no need to set follows code */
        {
            return;
        }

    }

    REG_SETF(OQSPIF, OQSPIF_GP_REG, OSPIC_PADS_DRV, 2);
    REG_SETF(CRG_TOP, CLK_AMBA_REG, OQSPIF_DIV, 1);
    REG_SETF(CRG_TOP, CLK_CTRL_REG, PLL_CLK_SEL, 2);
    REG_SETF(CRG_TOP, CLK_AMBA_REG, HCLK_DIV, 0);
    REG_SETF(CRG_TOP, CLK_CTRL_REG, PLL_CPU_ENABLE, 1);
    REG_SETF(CRG_TOP, CLK_CTRL_REG, SYS_CLK_SEL, 3);
#if !SYSTEM_BOOT_CLK_40M
    REG_SETF(CRG_TOP, CLK_CTRL_REG, PLL_PERI_ENABLE, 1);
#endif // SYSTEM_BOOT_CLK_40M
    return;
}

/*******************************************************************************************************************//**
 * Basic system setup.
 *
 * @brief  Setup the AMBA clocks. Ensure proper alignment of copy and zero table entries.
 *
 * @note   No variable initialization should take place here, since the C runtime
 *         has not yet been initialized yet, i.e. any modifications to variables will
 *         be discarded. For the same reason, functions that initialize or use
 *         initialized variables should not be called from here.
 **********************************************************************************************************************/
BSP_PLACE_IN_SECTION("text_reset") void SystemInitPre (void)
{
    /* Populate device information attributes. */
    BSP_CHECK_DEBUG(bsp_device_info_init());

    bsp_prv_enable_debuggers();

    if (bsp_is_chip_revision_a())
    {
        /*
         * Set QSPI pads drive current to 8 mA. Default was 4 mA.
         * The problem with default for AA 4 mA drive current is observable
         * only with high PLL frequency (the waveform is not accurate at the
         * rising/falling edges according to scope measurements).
         */
        OQSPIF->OQSPIF_GP_REG_b.OSPIC_PADS_DRV = BSP_QSPIC_PADS_DRIVE_CURRENT_8MA;
    }
    else
    {
        /* Set QSPI pads drive current to 8 mA. Default was 2 mA. */
        OQSPIF->OQSPIF_GP_REG_b.OSPIC_PADS_DRV = BSP_QSPIC_PADS_DRIVE_CURRENT_BA_8MA;

        /* The following is required for BA. */
        CRG_TOP->CLK_AMBA_REG_b.GEN_DMA_ENABLE = 1;
    }

    /* Call pre clock initialization hook. */
    R_BSP_WarmStart(BSP_WARM_START_RESET);

    /*
     * Bandgap has already been set by the bootloader.
     * Use fast clocks from now on.
     */
    /* TODO: the following clock-related functionality must be updated/removed, see TVR-398 */
    /* clocks_early_init(); */

    copy_table_sanity_check();
    zero_table_sanity_check();
}

/////////////////////////////////////////// TODO start of to be removed region.

// The following macros are copied from
// ra/fsp/src/bsp_w/cmsis/Device/RENESAS/Include/interrupts.h
# define LAST_IRQn (96) //WIFI_RC_IRQn

/*
 * Following macros allow easy way to build table with interrupt priorities.
 * See example in set_interrupt_priorities function description.
 */
#define INTERRUPT_PRIORITY_CONFIG_START(name) const int8_t name[] = {
#define PRIORITY_0      (LAST_IRQn + 1)
#define PRIORITY_1      (LAST_IRQn + 2)
#define PRIORITY_2      (LAST_IRQn + 3)
#define PRIORITY_3      (LAST_IRQn + 4)
#define PRIORITY_4      (LAST_IRQn + 5)
#define PRIORITY_5      (LAST_IRQn + 6)
#define PRIORITY_6      (LAST_IRQn + 7)
#define PRIORITY_7      (LAST_IRQn + 8)
#define PRIORITY_8      (LAST_IRQn + 9)
#define PRIORITY_9      (LAST_IRQn + 10)
#define PRIORITY_10     (LAST_IRQn + 11)
#define PRIORITY_11     (LAST_IRQn + 12)
#define PRIORITY_12     (LAST_IRQn + 13)
#define PRIORITY_13     (LAST_IRQn + 14)
#define PRIORITY_14     (LAST_IRQn + 15)
#define PRIORITY_15     (LAST_IRQn + 16)
#define PRIORITY_TABLE_END (LAST_IRQn + 17)
#define INTERRUPT_PRIORITY_CONFIG_END PRIORITY_TABLE_END };

/*
 * Dialog default priority configuration table.
 *
 * Content of this table will be applied at system start.
 *
 * \note If interrupt priorities provided by Dialog need to be changed, do not modify this file.
 * Create similar table with SAME name instead somewhere inside code without week attribute,
 * and it will be used instead of table below.
 */
#pragma weak __dialog_interrupt_priorities
INTERRUPT_PRIORITY_CONFIG_START(__dialog_interrupt_priorities)
    PRIORITY_0, /* Start interrupts with priority 0 (highest) */
                /*
                 * Note: Interrupts with priority 0 are not
                 * allowed to perform OS calls.
                 */
    PRIORITY_1, /* Start interrupts with priority 1 */
    PRIORITY_2, /* Start interrupts with priority 2 */
    WIFI_MACPROT_IRQn,
    DMA_IRQn,
    KDMA_IRQn,
    I2C_IRQn,
    I2C2_IRQn,
    SPI_IRQn,
    SPI2_IRQn,
    DAI_TX_IRQn,
    DAI_RX_IRQn,
    SRC_IN_IRQn,
    SRC_OUT_IRQn,
    AUX_ADC_IRQn,
    PRIORITY_3, /* Start interrupts with priority 3 */
    SysTick_IRQn,
    UART_IRQn,
    UART2_IRQn,
    UART3_IRQn,
    SDIO_IRQn,
    SDEMMC_IRQn,
    TDES_CBC_IRQn,
    PSK_SHA1_IRQn,
    CLKCAL_IRQn,
    TIMER_IRQn,
#if !defined(OS_FREERTOS)
    TIMER2_IRQn,
#endif
    TIMER3_IRQn,
    TIMER4_IRQn,
    TIMER5_IRQn,
    TIMER6_IRQn,
    TIMER7_IRQn,
    TIMER8_IRQn,
    CAPTIMER_IRQn,
    SYSPLL_LOCK_IRQn,
    RTC_IF_EXTWK_IRQn,
    RTC_IF_BLACK_IRQn,
    RTC_IF_BROWN_IRQn,
    RTC_IF_PCNT_IRQn,
    RTC_IF_BCF_MSR_IRQn,
    RTC_IF_RTC_ACC_IRQn,
    RTC_IF_EXP_IRQn,
    RTC_IF_LMR_IRQn,
    MRM_IRQn,
    DCACHE_MRM_IRQn,
    CC312_IRQn,
    GPIO_P0_IRQn,
    GPIO_P1_IRQn,
    FPLL_LOCK_IRQn,
    WIFI_HSU_IRQn,
    WIFI_MODEM_IRQn,
    WIFI_MACTIMER_IRQn,
    WIFI_MACOTHER_IRQn,
    WIFI_MACRX_IRQn,
    WIFI_MACTX_IRQn,
    //WIFI_MACPROT_IRQn,
    WIFI_MACINTGEN_IRQn,
    WIFI_MACBCN_IRQn,
    WIFI_RC_IRQn,
    SDEMMC_WKUP_IRQn,
    PRIORITY_4, /* Start interrupts with priority 4 */
    PRIORITY_5, /* Start interrupts with priority 5 */
    PRIORITY_6, /* Start interrupts with priority 6 */
    PRIORITY_7, /* Start interrupts with priority 7 */
    PRIORITY_8, /* Start interrupts with priority 8 */
    PRIORITY_9, /* Start interrupts with priority 9 */
    PRIORITY_10, /* Start interrupts with priority 10 */
    PRIORITY_11, /* Start interrupts with priority 11 */
    PRIORITY_12, /* Start interrupts with priority 12 */
    PRIORITY_13, /* Start interrupts with priority 13 */
    PRIORITY_14, /* Start interrupts with priority 14 */
    PRIORITY_15, /* Start interrupts with priority 15 (lowest) */
#if defined(OS_FREERTOS)
    TIMER2_IRQn,
#endif
INTERRUPT_PRIORITY_CONFIG_END

void set_interrupt_priorities(const int8_t prios[]);
void set_interrupt_priorities(const int8_t prios[])
{
    uint32_t old_primask, iser, iser2;
    int i = 0;
    uint32_t prio = 0;

    /*
     * We shouldn't change the priority of an enabled interrupt.
     *  1. Globally disable interrupts, saving the global interrupts disable state.
     *  2. Explicitly disable all interrupts, saving the individual interrupt enable state.
     *  3. Set interrupt priorities.
     *  4. Restore individual interrupt enables.
     *  5. Restore global interrupt enable state.
     */
    old_primask = __get_PRIMASK();
    __disable_irq();
    iser  = NVIC->ISER[0];
    iser2 = NVIC->ISER[1];
    NVIC->ICER[0] = iser;
    NVIC->ICER[1] = iser2;

    for (i = 0; prios[i] != PRIORITY_TABLE_END; ++i)
    {
        switch (prios[i])
        {
        case PRIORITY_0:
        case PRIORITY_1:
        case PRIORITY_2:
        case PRIORITY_3:
        case PRIORITY_4:
        case PRIORITY_5:
        case PRIORITY_6:
        case PRIORITY_7:
        case PRIORITY_8:
        case PRIORITY_9:
        case PRIORITY_10:
        case PRIORITY_11:
        case PRIORITY_12:
        case PRIORITY_13:
        case PRIORITY_14:
        case PRIORITY_15:
                prio = (uint32_t)(prios[i] - PRIORITY_0);
                break;
        default:
                NVIC_SetPriority(prios[i], prio);
                break;
        }
    }

    NVIC->ISER[0] = iser;
    NVIC->ISER[1] = iser2;
    __set_PRIMASK(old_primask);
}
/////////////////////////////////////////// TODO end of to be removed region.

#ifdef BSP_MCU_RRQ61051
/*******************************************************************************************************************//**
 * Internal pins setup of Combo Module.
 *
 * @brief  Setups internal pins for Combo Module.
 *
 * @note   There is no viable option to configure invisible pinout and have it generated using E2Studio tools.
 *         As a workaround, we could manually initialize pins without user interactions.
 *         Those pins are crucial for NVRAM operations as well as storing firmware on flash / psram.
 **********************************************************************************************************************/
static void rrq6105x_internal_pin_init(void)
{
    /* OQSPI_CLK */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_08, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
    /* OQSPI_CS */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_09, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
    /* OQSPI_D0 */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_04, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT  | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
    /* OQSPI_D1 */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_05, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT  | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
    /* OQSPI_D2 */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_06, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT  | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
    /* OQSPI_D3 */
    R_BSP_PinCfg(BSP_IO_PORT_01_PIN_07, (uint32_t) IOPORT_CFG_DRIVE_STRENGTH_BA_8MA | (uint32_t) IOPORT_CFG_PERIPHERAL_PIN |
                                        (uint32_t) IOPORT_CFG_PORT_DIRECTION_INPUT  | (uint32_t) IOPORT_CFG_SLEW_RATE_FAST);
}
#endif /* BSP_MCU_RRQ61051 */

static void ra6w1_SystemInit (void)
{
    /* Need to call bsp_device_info_init() second time here (previous results
     * were destroyed by processing zero_table in Reset_Handler() after calling
     * SystemInitPre()). */
    BSP_CHECK_DEBUG(bsp_device_info_init());

    /* TODO: the following clock-related functionality must be updated/removed, see TVR-398 */
    // back to xtal 40Mhz
#if SYSTEM_BOOT_CLK_40M
    REG_SETF(CRG_TOP, CLK_CTRL_REG, SYS_CLK_SEL, 1); // XTAL
    REG_SETF(CRG_TOP, CLK_CTRL_REG, PLL_CPU_ENABLE, 0);
    REG_SETF(CRG_TOP, CLK_AMBA_REG, OQSPIF_DIV, 0);
#endif // SYSTEM_BOOT_CLK_40M

    // TODO remove the following call:
    /*
     * Apply default priorities to interrupts.
     */
    set_interrupt_priorities(__dialog_interrupt_priorities);

    // enable Usage-, Bus-, and MMU Fault
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk |
                  SCB_SHCSR_BUSFAULTENA_Msk |
                  SCB_SHCSR_MEMFAULTENA_Msk;

    /* Initialize SystemCoreClock variable. */
    SystemCoreClockUpdate();

#if dg_configUSE_HW_OTPC
    /* enable OTP to read TCS values */
    bsp_otp_init();
    bsp_otp_mode_set(BSP_OTP_MODE_READ);

    /* get TCS values */
 #if BSP_FEATURE_USE_TCS
    bsp_tcs_get_trim_values_from_cs();
 #endif

    /* Close OTP */
    bsp_otp_close();
#endif

#if defined(CONFIG_RETARGET) || defined(CONFIG_RTT)
    /* This is needed to initialize stdout, so that it can be used by putchar (that doesn't initialize stdout,
     * contrary to printf). Putchar is needed by the Unity test framework
     * This also has the side effect of changing stdout to unbuffered (which seems more reasonable)
     */
    setvbuf(stdout, NULL, _IONBF, 0);
#endif


    /* TODO: the following clock configuration must be removed, see TVR-398 */
    REG_SETF(CRG_TOP, CLK_AMBA_REG, PERI_CLK_ENABLE, 1);
    REG_SETF(CRG_TOP, CLK_AMBA_REG, TIMER_CLK_ENABLE, 1);

    /*
     * If WDT watchdog is not used - freeze it
     */
    bsp_wdt_freeze_cfg();

    /* TODO: the following clock configuration must be removed, see TVR-398 */
#if SYSTEM_BOOT_CLK_40M
    setup_clocks();
#endif // SYSTEM_BOOT_CLK_40M

    /* Initialize SystemWakeupSource variable. */
    SystemWakeupSourceUpdate();

#ifdef BSP_MCU_RRQ61051
    rrq6105x_internal_pin_init();
#endif /* BSP_MCU_RRQ61051 */

    /* Call post clock initialization hook. */
    R_BSP_WarmStart(BSP_WARM_START_POST_CLOCK);

#if !BSP_CFG_OSPI_8_LINES_SUPPORT
    /* Use pins PIN1_00-PIN1_03 as GPIO instead of OQSPI lines. */
    CRG_TOP->CLK_AMBA_REG_b.OQSPI_GPIO_MODE = 1;
#endif
}

#ifdef USE_MALLOC_BUG_WORKAROUND

/*******************************************************************************************************************//**
 * Workaround for the 'nano_malloc' bug in ARM Embedded toolchain version 10.3-2021-10.
 *
 * The bug occurs when malloc attempts to merge a memory chunk from its 'free_list'
 * with a new memory chunk provided by _sbrk. In this case the merged chunk is not removed
 * from the 'free_list', and susequent call to malloc might return same memory region.
 *
 * To avoid this issue, we allocate and then free the entire heap at the start. This process
 * ensures that malloc has a single large free chunk of memory (essentially the entire heap)
 * that it can manage using its internal algorithms.
 *
 * When malloc eventually exhausts this large free chunk, it will call _sbrk to request
 * more memory. Since _sbrk has no additional heap memory to provide after the initial
 * allocation, it will return -1.
 *
 * This mechanism ensures that malloc never reaches the buggy code that attempts to merge
 * a free chunk with a new chunk from _sbrk, as _sbrk will not supply any new memory chunks.
 **********************************************************************************************************************/
static void malloc_and_free_heap (void)
{
    void   * p    = NULL;
    uint32_t size = BSP_CFG_HEAP_BYTES;

    if (0 == size)
    {
        return;                        /* Do nothing in case heap is disabled. */
    }

    while (NULL == p)
    {
        p = malloc(size);
        --size;
    }

    free(p);
}

#endif

static void ra6w1_SystemInitPost (void)
{
#ifdef USE_MALLOC_BUG_WORKAROUND
    malloc_and_free_heap();
#endif

    /* If no seed value is provided, the rand() function should be automatically seeded with a value of 1. */
    srand(1);

    /* Call Post C runtime initialization hook. */
    R_BSP_WarmStart(BSP_WARM_START_POST_C);
}

/** @} (end addtogroup BSP_MCU_PRV) */
