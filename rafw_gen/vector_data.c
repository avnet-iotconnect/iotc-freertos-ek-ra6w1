/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        #if BSP_FEATURE_BSP_HAS_ICU
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        #else
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_IRQ_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        #endif
        {
                        [1] = KDMA_Handler, /* KDMA */
            [2] = uart_w_isr, /* UART0 RXI (Receive data full) */
            [15] = TDES_CBC_Handler, /* TDES */
            [16] = PSK_SHA1_Handler, /* PSK SHA1 */
            [28] = SYSPLL_Lock_Handler, /* SYS PLL LOCK (PLL480MHz lock) */
            [30] = RTC_IF_EXTWK_Handler, /* EXT WK (External wakeup) */
            [31] = RTC_IF_BLACK_Handler, /* BLACK (Voltage blackout) */
            [32] = RTC_IF_BROWN_Handler, /* BROWN (Voltage brownout) */
            [33] = RTC_IF_PCNT_Handler, /* PCNT (Pulse count) */
            [34] = RTC_IF_BCF_MSR_Handler, /* BCF MSR (Bus clock measure) */
            [35] = RTC_IF_RTC_ACC_Handler, /* RTC ACC (access to RTC Core) */
            [36] = RTC_IF_EXP_Handler, /* EXP (RTC mirror Free running count) */
            [37] = RTC_IF_LMR_Handler, /* LMR (FRC to mirroring, loading done) */
            [38] = MRM_Handler, /* MRM (Instruction cache Miss Rate Monitor) */
            [39] = DCACHE_MRM_Handler, /* DCACHE MRM (Data cache Miss Rate Monitor) */
            [40] = CC312_Handler, /* CC312 INT */
            [41] = r_ext_irq_w_isr, /* ICU PORT0 (External pin interrupt port 0) */
            [42] = r_ext_irq_w_isr, /* ICU PORT1 (External pin interrupt port 1) */
            [43] = FPLL_Lock_Handler, /* FPLL LOCK */
            [44] = hsu_isr, /* HSU (Hardware Security Unit) */
            [46] = rxl_mpdu_isr, /* MACTIMER (MAC TX/RX Timer) */
            [48] = rxl_mpdu_isr, /* MACRX (WIFI MAC RX Trigger) */
            [49] = txl_transmit_trigger, /* MACTX (WIFI MAC TX Trigger) */
            [50] = txl_prot_trigger, /* MACPROT (MAC Protocol Trigger) */
            [51] = hal_machw_gen_handler, /* MACINTGEN (MAC General interrupt) */
            [52] = hal_machw_bcn_cancellation_handler, /* MACBCN (MAC Beacon Reception) */
            [53] = phy_rc_isr, /* RC (Radio Controller) */
        };
        #if BSP_FEATURE_BSP_HAS_ICU
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [1] = BSP_PRV_IELS_ENUM(EVENT_KDMA_INT), /* KDMA */
            [2] = BSP_PRV_IELS_ENUM(EVENT_UART0_INT), /* UART0 RXI (Receive data full) */
            [15] = BSP_PRV_IELS_ENUM(EVENT_TDES_CBC_INT), /* TDES */
            [16] = BSP_PRV_IELS_ENUM(EVENT_PSK_SHA1_INT), /* PSK SHA1 */
            [28] = BSP_PRV_IELS_ENUM(EVENT_SYSPLL_LOCK_INT), /* SYS PLL LOCK (PLL480MHz lock) */
            [30] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_EXTWK_INT), /* EXT WK (External wakeup) */
            [31] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_BLACK_INT), /* BLACK (Voltage blackout) */
            [32] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_BROWN_INT), /* BROWN (Voltage brownout) */
            [33] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_PCNT_INT), /* PCNT (Pulse count) */
            [34] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_BCF_MSR_INT), /* BCF MSR (Bus clock measure) */
            [35] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_RTC_ACC_INT), /* RTC ACC (access to RTC Core) */
            [36] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_EXP_INT), /* EXP (RTC mirror Free running count) */
            [37] = BSP_PRV_IELS_ENUM(EVENT_RTC_IF_LMR_INT), /* LMR (FRC to mirroring, loading done) */
            [38] = BSP_PRV_IELS_ENUM(EVENT_MRM_INT), /* MRM (Instruction cache Miss Rate Monitor) */
            [39] = BSP_PRV_IELS_ENUM(EVENT_DCACHE_MRM_INT), /* DCACHE MRM (Data cache Miss Rate Monitor) */
            [40] = BSP_PRV_IELS_ENUM(EVENT_CC312_INT), /* CC312 INT */
            [41] = BSP_PRV_IELS_ENUM(EVENT_GPIO_P0_INT), /* ICU PORT0 (External pin interrupt port 0) */
            [42] = BSP_PRV_IELS_ENUM(EVENT_GPIO_P1_INT), /* ICU PORT1 (External pin interrupt port 1) */
            [43] = BSP_PRV_IELS_ENUM(EVENT_FPLL_LOCK_INT), /* FPLL LOCK */
            [44] = BSP_PRV_IELS_ENUM(EVENT_WIFI_HSU_INT), /* HSU (Hardware Security Unit) */
            [46] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACTIMER_INT), /* MACTIMER (MAC TX/RX Timer) */
            [48] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACRX_INT), /* MACRX (WIFI MAC RX Trigger) */
            [49] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACTX_INT), /* MACTX (WIFI MAC TX Trigger) */
            [50] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACPROT_INT), /* MACPROT (MAC Protocol Trigger) */
            [51] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACINTGEN_INT), /* MACINTGEN (MAC General interrupt) */
            [52] = BSP_PRV_IELS_ENUM(EVENT_WIFI_MACBCN_INT), /* MACBCN (MAC Beacon Reception) */
            [53] = BSP_PRV_IELS_ENUM(EVENT_WIFI_RC_INT), /* RC (Radio Controller) */
        };
        #endif
        #endif
