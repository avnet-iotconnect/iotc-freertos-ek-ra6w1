/* ${REA_DISCLAIMER_PLACEHOLDER} */

#include "bsp_cfg.h"

#define SYS_CTRL_REG                            0x400C0018
#define SYS_CTRL_REG_DEBUGGER_ENABLE_MSK        0x10

                .syntax	unified
                .arch	armv8-m.main

                #define __INITIAL_SP     __StackTop
                #define __STACK_LIMIT    __StackLimit

                .section .stack, "a"
                .align	3
                .globl	__StackTop
                .globl	__StackLimit
__StackLimit:
                .space	BSP_CFG_STACK_MAIN_BYTES
                .size	__StackLimit, . - __StackLimit
__StackTop:
                .size	__StackTop, . - __StackTop


                #ifdef BSP_CFG_HEAP_BYTES
                .equ    Heap_Size, BSP_CFG_HEAP_BYTES
                #else
                .equ    Heap_Size, 0
                #endif

                .if     Heap_Size != 0                     /* Heap is provided */
                .section .heap
                .align	3
                .globl	__HeapBase
                .globl	__HeapLimit
__HeapBase:
                .space	Heap_Size
                .size	__HeapBase, . - __HeapBase
__HeapLimit:
                .size	__HeapLimit, . - __HeapLimit
                .endif

                .section .isr_vector, "a"
                .align 2
                .globl	__Vectors
                .globl  __Vectors_end
                .globl  __Vectors_size
__Vectors:
                .long   __INITIAL_SP            /* Top of Stack */
                .long   Reset_Handler           /* Reset Handler */
                .long   NMI_Handler             /* NMI Handler */
                .long   HardFault_Handler       /* Hard Fault Handler */
                .long   MemManage_Handler       /* MPU Fault Handler */
                .long   BusFault_Handler        /* Bus Fault Handler */
                .long   UsageFault_Handler      /* Usage Fault Handler */
                .long   SecureFault_Handler     /* Secure Fault Handler */
                .long   0                       /* Reserved */
                .long   0                       /* Reserved */
                .long   0                       /* Reserved */
                .long   SVC_Handler             /* SVCall Handler */
                .long   DebugMon_Handler        /* Debug Monitor Handler */
                .long   0                       /* Reserved */
                .long   PendSV_Handler          /* PendSV Handler */
                .long   SysTick_Handler         /* SysTick Handler */

__Vectors_end:
                .equ     __Vectors_size, __Vectors - __Vectors
                .size	__Vectors, . - __Vectors

                /*
                 * Patch Table
                 * Notice: it crosses the 0x100 boundary
                 *         in cached mode, the first entries will be copied to
                 *         (and accessed from) RAM and others will only lie on
                 *         QSPI flash or OTP.
                 *
                 * In C declare: extern uint32_t __patch_table_start[];
                 */
                .section .patch_table, "a"
                .align 4
                .globl	__patch_table_start
__patch_table_start:
                /*
                 * 28 32-bit entries, initialized to "pointer to Default_Patch_Code"
                 *
                 * NOTE: we assume that SYS_CTRL_REG.REMAP_INTVECT == 1
                 */
                .long Default_Patch_Code_Handler        /*  0, in RAM */
                .long Default_Patch_Code_Handler        /*  1, in RAM */
                .long Default_Patch_Code_Handler        /*  2, in RAM */
                .long Default_Patch_Code_Handler        /*  3, in RAM */
                .long Default_Patch_Code_Handler        /*  4, in RAM */
                .long Default_Patch_Code_Handler        /*  5, in RAM */
                .long Default_Patch_Code_Handler        /*  6, in RAM */
                .long Default_Patch_Code_Handler        /*  7, in RAM */
                .long Default_Patch_Code_Handler        /*  8, in RAM */
                .long Default_Patch_Code_Handler        /*  9, in RAM */
                .long Default_Patch_Code_Handler        /* 10, in RAM */
                .long Default_Patch_Code_Handler        /* 11, in RAM */
                /*
                 * Chip version: AD
                 * Entries 12, 13, 14 and 15 are unusable and skipped in the QSPI image,
                 * to account for the overhead of the QSPI header (16 bytes).
                 *
                 * Chip version: AA, AC, AE
                 * Entries 14 and 15 are unusable and skipped in the QSPI image,
                 * to account for the overhead of the QSPI header (8 bytes).
                 *
                 * The mkimage tool is responsible for stripping the proper entries from the QSPI image.
                 */
                 // TODO Check again when IC_REV_B arrives!
                .long Default_Patch_Code_Handler        /* 12 */
                .long Default_Patch_Code_Handler        /* 13 */
                .long 0xDEADBEEF                        /* 14 */
                .long 0xDEADBEEF                        /* 15 */
                .size	__patch_table_start, . - __patch_table_start

__patch_table_end:

                .section .default_patch_code_handler_section, "a"
                .globl Default_Patch_Code_Handler
                .align	1
                .thumb
                .thumb_func
                .type	Default_Patch_Code_Handler, %function
Default_Patch_Code_Handler:
                bkpt	#0
                bkpt	#0

                .section text_reset
                .text
                .align  2
        	.thumb_func
                .type   Default_Handler, %function
        	.weak	Default_Handler
Default_Handler:
                /* Enable debugger */
                ldr     r1, =SYS_CTRL_REG
                ldrh    r2, [r1, #0]
                movs    r3, #SYS_CTRL_REG_DEBUGGER_ENABLE_MSK
                orrs    r2, r2, r3
                strh    r2, [r1, #0]
        	b	.
        	.size	Default_Handler, . - Default_Handler

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
        	.macro	def_irq_handler	handler_name
        	.weak	\handler_name
        	.set	\handler_name, Default_Handler
        	.endm

        	.thumb
        	.thumb_func
        	.align	1
                .type   SVC_Handler, %function
        	.weak	SVC_Handler
SVC_Handler:
        	b	.
        	.size	SVC_Handler, . - SVC_Handler
                def_irq_handler MemManage_Handler
                def_irq_handler BusFault_Handler
                def_irq_handler UsageFault_Handler
                def_irq_handler SecureFault_Handler
                def_irq_handler DebugMon_Handler
                def_irq_handler PendSV_Handler
                def_irq_handler SysTick_Handler

                def_irq_handler DMA_Handler
                def_irq_handler KDMA_Handler
                def_irq_handler uart_w_isr
                def_irq_handler uart_w_isr
                def_irq_handler uart_w_isr
                def_irq_handler I2C_Handler
                def_irq_handler I2C2_Handler
                def_irq_handler I3C_Handler
                def_irq_handler spi_w_isr
                def_irq_handler DAI_TX_Handler
                def_irq_handler DAI_RX_Handler
                def_irq_handler SRC_In_Handler
                def_irq_handler SRC_Out_Handler
                def_irq_handler sdhimmc_sdio_isr
                def_irq_handler TDES_CBC_Handler
                def_irq_handler PSK_SHA1_Handler
                def_irq_handler CLKCAL_Handler
                def_irq_handler Timer_Handler
                def_irq_handler Timer2_Handler
                def_irq_handler Timer3_Handler
                def_irq_handler Timer4_Handler
                def_irq_handler Timer5_Handler
                def_irq_handler Timer6_Handler
                def_irq_handler Timer7_Handler
                def_irq_handler Timer8_Handler
                def_irq_handler CAPTIMER_Handler
                def_irq_handler SYSPLL_Lock_Handler
                def_irq_handler adc_w_scan_end_isr
                def_irq_handler RTC_IF_EXTWK_Handler
                def_irq_handler RTC_IF_BLACK_Handler
                def_irq_handler RTC_IF_BROWN_Handler
                def_irq_handler RTC_IF_PCNT_Handler
                def_irq_handler RTC_IF_BCF_MSR_Handler
                def_irq_handler RTC_IF_RTC_ACC_Handler
                def_irq_handler RTC_IF_EXP_Handler
                def_irq_handler RTC_IF_LMR_Handler
                def_irq_handler MRM_Handler
                def_irq_handler DCACHE_MRM_Handler
                def_irq_handler CC312_Handler
                def_irq_handler r_ext_irq_w_isr
                def_irq_handler FPLL_Lock_Handler
                def_irq_handler CM33_CTI_0_Handler
                def_irq_handler CM33_CTI_1_Handler
                def_irq_handler hsu_isr
                /*def_irq_handler phy_mdm_isr*/
                def_irq_handler rxl_mpdu_isr
                /*def_irq_handler WIFI_TX_RX_MISC_Handler*/
                def_irq_handler rxl_mpdu_isr
                def_irq_handler txl_transmit_trigger
                def_irq_handler txl_prot_trigger
                def_irq_handler hal_machw_gen_handler
                def_irq_handler hal_machw_bcn_cancellation_handler
                def_irq_handler phy_rc_isr
                def_irq_handler sdhimmc_accs_isr
        	.end
