/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (27)
#endif
/* ISR prototypes */
void KDMA_Handler(void);
void uart_w_isr(void);
void TDES_CBC_Handler(void);
void PSK_SHA1_Handler(void);
void SYSPLL_Lock_Handler(void);
void RTC_IF_EXTWK_Handler(void);
void RTC_IF_BLACK_Handler(void);
void RTC_IF_BROWN_Handler(void);
void RTC_IF_PCNT_Handler(void);
void RTC_IF_BCF_MSR_Handler(void);
void RTC_IF_RTC_ACC_Handler(void);
void RTC_IF_EXP_Handler(void);
void RTC_IF_LMR_Handler(void);
void MRM_Handler(void);
void DCACHE_MRM_Handler(void);
void CC312_Handler(void);
void r_ext_irq_w_isr(void);
void FPLL_Lock_Handler(void);
void hsu_isr(void);
void rxl_mpdu_isr(void);
void txl_transmit_trigger(void);
void txl_prot_trigger(void);
void hal_machw_gen_handler(void);
void hal_machw_bcn_cancellation_handler(void);
void phy_rc_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_KDMA_INT ((IRQn_Type) 1) /* KDMA */
#define KDMA_INT_IRQn          ((IRQn_Type) 1) /* KDMA */
#define VECTOR_NUMBER_UART0_INT ((IRQn_Type) 2) /* UART0 RXI (Receive data full) */
#define UART0_INT_IRQn          ((IRQn_Type) 2) /* UART0 RXI (Receive data full) */
#define VECTOR_NUMBER_TDES_CBC_INT ((IRQn_Type) 15) /* TDES */
#define TDES_CBC_INT_IRQn          ((IRQn_Type) 15) /* TDES */
#define VECTOR_NUMBER_PSK_SHA1_INT ((IRQn_Type) 16) /* PSK SHA1 */
#define PSK_SHA1_INT_IRQn          ((IRQn_Type) 16) /* PSK SHA1 */
#define VECTOR_NUMBER_SYSPLL_LOCK_INT ((IRQn_Type) 28) /* SYS PLL LOCK (PLL480MHz lock) */
#define SYSPLL_LOCK_INT_IRQn          ((IRQn_Type) 28) /* SYS PLL LOCK (PLL480MHz lock) */
#define VECTOR_NUMBER_RTC_IF_EXTWK_INT ((IRQn_Type) 30) /* EXT WK (External wakeup) */
#define RTC_IF_EXTWK_INT_IRQn          ((IRQn_Type) 30) /* EXT WK (External wakeup) */
#define VECTOR_NUMBER_RTC_IF_BLACK_INT ((IRQn_Type) 31) /* BLACK (Voltage blackout) */
#define RTC_IF_BLACK_INT_IRQn          ((IRQn_Type) 31) /* BLACK (Voltage blackout) */
#define VECTOR_NUMBER_RTC_IF_BROWN_INT ((IRQn_Type) 32) /* BROWN (Voltage brownout) */
#define RTC_IF_BROWN_INT_IRQn          ((IRQn_Type) 32) /* BROWN (Voltage brownout) */
#define VECTOR_NUMBER_RTC_IF_PCNT_INT ((IRQn_Type) 33) /* PCNT (Pulse count) */
#define RTC_IF_PCNT_INT_IRQn          ((IRQn_Type) 33) /* PCNT (Pulse count) */
#define VECTOR_NUMBER_RTC_IF_BCF_MSR_INT ((IRQn_Type) 34) /* BCF MSR (Bus clock measure) */
#define RTC_IF_BCF_MSR_INT_IRQn          ((IRQn_Type) 34) /* BCF MSR (Bus clock measure) */
#define VECTOR_NUMBER_RTC_IF_RTC_ACC_INT ((IRQn_Type) 35) /* RTC ACC (access to RTC Core) */
#define RTC_IF_RTC_ACC_INT_IRQn          ((IRQn_Type) 35) /* RTC ACC (access to RTC Core) */
#define VECTOR_NUMBER_RTC_IF_EXP_INT ((IRQn_Type) 36) /* EXP (RTC mirror Free running count) */
#define RTC_IF_EXP_INT_IRQn          ((IRQn_Type) 36) /* EXP (RTC mirror Free running count) */
#define VECTOR_NUMBER_RTC_IF_LMR_INT ((IRQn_Type) 37) /* LMR (FRC to mirroring, loading done) */
#define RTC_IF_LMR_INT_IRQn          ((IRQn_Type) 37) /* LMR (FRC to mirroring, loading done) */
#define VECTOR_NUMBER_MRM_INT ((IRQn_Type) 38) /* MRM (Instruction cache Miss Rate Monitor) */
#define MRM_INT_IRQn          ((IRQn_Type) 38) /* MRM (Instruction cache Miss Rate Monitor) */
#define VECTOR_NUMBER_DCACHE_MRM_INT ((IRQn_Type) 39) /* DCACHE MRM (Data cache Miss Rate Monitor) */
#define DCACHE_MRM_INT_IRQn          ((IRQn_Type) 39) /* DCACHE MRM (Data cache Miss Rate Monitor) */
#define VECTOR_NUMBER_CC312_INT ((IRQn_Type) 40) /* CC312 INT */
#define CC312_INT_IRQn          ((IRQn_Type) 40) /* CC312 INT */
#define VECTOR_NUMBER_GPIO_P0_INT ((IRQn_Type) 41) /* ICU PORT0 (External pin interrupt port 0) */
#define GPIO_P0_INT_IRQn          ((IRQn_Type) 41) /* ICU PORT0 (External pin interrupt port 0) */
#define VECTOR_NUMBER_GPIO_P1_INT ((IRQn_Type) 42) /* ICU PORT1 (External pin interrupt port 1) */
#define GPIO_P1_INT_IRQn          ((IRQn_Type) 42) /* ICU PORT1 (External pin interrupt port 1) */
#define VECTOR_NUMBER_FPLL_LOCK_INT ((IRQn_Type) 43) /* FPLL LOCK */
#define FPLL_LOCK_INT_IRQn          ((IRQn_Type) 43) /* FPLL LOCK */
#define VECTOR_NUMBER_WIFI_HSU_INT ((IRQn_Type) 44) /* HSU (Hardware Security Unit) */
#define WIFI_HSU_INT_IRQn          ((IRQn_Type) 44) /* HSU (Hardware Security Unit) */
#define VECTOR_NUMBER_WIFI_MACTIMER_INT ((IRQn_Type) 46) /* MACTIMER (MAC TX/RX Timer) */
#define WIFI_MACTIMER_INT_IRQn          ((IRQn_Type) 46) /* MACTIMER (MAC TX/RX Timer) */
#define VECTOR_NUMBER_WIFI_MACRX_INT ((IRQn_Type) 48) /* MACRX (WIFI MAC RX Trigger) */
#define WIFI_MACRX_INT_IRQn          ((IRQn_Type) 48) /* MACRX (WIFI MAC RX Trigger) */
#define VECTOR_NUMBER_WIFI_MACTX_INT ((IRQn_Type) 49) /* MACTX (WIFI MAC TX Trigger) */
#define WIFI_MACTX_INT_IRQn          ((IRQn_Type) 49) /* MACTX (WIFI MAC TX Trigger) */
#define VECTOR_NUMBER_WIFI_MACPROT_INT ((IRQn_Type) 50) /* MACPROT (MAC Protocol Trigger) */
#define WIFI_MACPROT_INT_IRQn          ((IRQn_Type) 50) /* MACPROT (MAC Protocol Trigger) */
#define VECTOR_NUMBER_WIFI_MACINTGEN_INT ((IRQn_Type) 51) /* MACINTGEN (MAC General interrupt) */
#define WIFI_MACINTGEN_INT_IRQn          ((IRQn_Type) 51) /* MACINTGEN (MAC General interrupt) */
#define VECTOR_NUMBER_WIFI_MACBCN_INT ((IRQn_Type) 52) /* MACBCN (MAC Beacon Reception) */
#define WIFI_MACBCN_INT_IRQn          ((IRQn_Type) 52) /* MACBCN (MAC Beacon Reception) */
#define VECTOR_NUMBER_WIFI_RC_INT ((IRQn_Type) 53) /* RC (Radio Controller) */
#define WIFI_RC_INT_IRQn          ((IRQn_Type) 53) /* RC (Radio Controller) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
