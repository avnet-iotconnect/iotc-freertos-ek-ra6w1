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

#ifndef BSP_COMMON_H
#define BSP_COMMON_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/* C99 includes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

/* Different compiler support. */
#include "../../inc/api/fsp_common_api.h"
#include "bsp_compiler_support.h"
#include "bsp_cfg.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Used to signify that an ELC event is not able to be used as an interrupt. */
#define BSP_IRQ_DISABLED    (0xFFU)

/* Version of this module's code and API. */

#if 1 == BSP_CFG_RTOS                  /* ThreadX */
 #include "tx_user.h"
 #if defined(TX_ENABLE_EVENT_TRACE) || defined(TX_ENABLE_EXECUTION_CHANGE_NOTIFY)
  #include "tx_api.h"
  #define FSP_CONTEXT_SAVE       tx_isr_start((uint32_t) R_FSP_CurrentIrqGet());
  #define FSP_CONTEXT_RESTORE    tx_isr_end((uint32_t) R_FSP_CurrentIrqGet());
 #else
  #define FSP_CONTEXT_SAVE
  #define FSP_CONTEXT_RESTORE
 #endif
#else
 #define FSP_CONTEXT_SAVE       do {} while (0);
 #define FSP_CONTEXT_RESTORE    do {} while (0);
#endif

/** Macro that can be defined in order to enable logging in FSP modules. */
#ifndef FSP_LOG_PRINT
 #define FSP_LOG_PRINT(X)
#endif

/** Macro to log and return error without an assertion. */
#ifndef FSP_RETURN

 #define FSP_RETURN(err)    FSP_ERROR_LOG((err)); \
    return err;
#endif

/** This function is called before returning an error code. To stop on a runtime error, define fsp_error_log in
 * user code and do required debugging (breakpoints, stack dump, etc) in this function.*/
#if (1 == BSP_CFG_ERROR_LOG)

 #ifndef FSP_ERROR_LOG
  #define FSP_ERROR_LOG(err) \
    fsp_error_log((fsp_err_t)(err), __FILE__, __LINE__);
 #endif
#else

 #define FSP_ERROR_LOG(err)
#endif

/** Default assertion calls ::FSP_ERROR_RETURN if condition "a" is false. Used to identify incorrect use of API's in FSP
 * functions. */
#if (3 == BSP_CFG_ASSERT)
 #define FSP_ASSERT(a)
#elif (2 == BSP_CFG_ASSERT)
 #define FSP_ASSERT(a)    {assert(a);}
#else
 #define FSP_ASSERT(a)    FSP_ERROR_RETURN((a), FSP_ERR_ASSERTION)
#endif                                 // ifndef FSP_ASSERT

/** All FSP error codes are returned using this macro. Calls ::FSP_ERROR_LOG function if condition "a" is false. Used
 * to identify runtime errors in FSP functions. */

#define FSP_ERROR_RETURN(a, err)                        \
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
        else                                            \
        {                                               \
            FSP_ERROR_LOG(err);                         \
            return err;                                 \
        }                                               \
    }

/** Push ARM Cortex-M scratch registers on stack. */
#ifndef UNIT_TESTING
 #define BSP_PUSH_SCRATCH_REGISTERS                                                    \
    do {                                                                               \
        __ASM volatile (                                                               \
            "  .syntax unified              \n" /* Prevent non-unified syntax */       \
            "   push {r0, r1, r2, r3}       \n" /* Push scratch registers on stack. */ \
            :                                   /* Output operands: None */            \
            :                                   /* Input operands: None */             \
            :                                   /* Clobbers: None */                   \
            );                                                                         \
    } while (0)
#else
 #define BSP_PUSH_SCRATCH_REGISTERS    do {} while (0)
#endif

/** Get the value of the stack pointer. */
#ifndef UNIT_TESTING
 #define BSP_GET_SP()                                                                      \
    ({                                                                                     \
        uint32_t ret;                                                                      \
                                                                                           \
        __ASM volatile (                                                                   \
            "  .syntax unified    \n"  /* Prevent non-unified syntax */                    \
            "   mov %[ret], sp    \n"  /* Move SP value on a general purpose register . */ \
            :[ret] "=r" (ret)          /* Output operands: Store the SP value on ret */    \
            :                          /* Input operands: None */                          \
            :                          /* Clobbers: None */                                \
            );                                                                             \
        ret;                                                                               \
    })
#else
 #define BSP_GET_SP()    (0)
#endif

/**
 * @brief Stop execution if a condition is false, in non-production builds.
 *
 * Used to detect an unexpected state at run-time during development.
 *
 * @note In non-production builds, if the condition is false, g_halt_impl() is called.
 *
 * @note In production builds, the condition is evaluated but nothing else happens, regardless of the outcome.
 */
#define BSP_CHECK_DEBUG(a)                             \
    do {                                               \
        if (!(a)) {                                    \
            if (0 == BSP_CFG_PRODUCTION_BUILD) {       \
                BSP_PUSH_SCRATCH_REGISTERS;            \
                g_halt_impl((uintptr_t) BSP_GET_SP()); \
            }                                          \
        }                                              \
    } while (0)

#ifndef UNIT_TESTING
 #define BSP_FATAL_ERROR \
    do {                 \
        __disable_irq(); \
        __BKPT(2);       \
    } while (0)
#else
 #define BSP_FATAL_ERROR \
    g_halt_impl(0)
#endif

/**
 * @brief Stop execution if a condition is false.
 *
 * Used to log and recover from a fatal state at run-time.
 *
 * @note In non-production builds, if the condition is false, g_halt_impl() is called, which is identical to the
 *       behavior of BSP_CHECK_DEBUG().
 *
 * @note In production builds, if the condition is false, interrupts are disabled and a SW breakpoint is issued.
 *       If a debugger is attached execution will halt.
 *       If a debugger is not attached, the breakpoint will be escalated to a Hard_Fault.
 */
#define BSP_CHECK_FATAL(a)                             \
    do {                                               \
        if (!(a)) {                                    \
            if (0 == BSP_CFG_PRODUCTION_BUILD) {       \
                BSP_PUSH_SCRATCH_REGISTERS;            \
                g_halt_impl((uintptr_t) BSP_GET_SP()); \
            }                                          \
            else {                                     \
                BSP_FATAL_ERROR;                       \
            }                                          \
        }                                              \
    } while (0)

/* Function-like macro used to wait for a condition to be met, most often used to wait for hardware register updates.
 * This macro can be redefined to add a timeout if necessary. */
#ifndef FSP_HARDWARE_REGISTER_WAIT
 #define FSP_HARDWARE_REGISTER_WAIT(reg, required_value)    while (reg != required_value) { /* Wait. */}
#endif

#ifndef FSP_REGISTER_READ

/* Read a register and discard the result. */
 #define FSP_REGISTER_READ(A)    __ASM volatile ("" : : "r" (A));
#endif

#ifndef FSP_REG_VAR_FIELD_GET

/**
 * Return the value of a register field, with the register value being stored in a variable
 * e.g.
 * @code
 * uint16_t tmp;
 * int counter;
 * tmp = CRG_TOP->TRIM_CTRL_REG;
 * counter = FSP_REG_VAR_FIELD_GET(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, tmp);
 * ...
 * @endcode
 */
 #define FSP_REG_VAR_FIELD_GET(base, reg, field, var)    \
    ((var & (base ## _ ## reg ## _ ## field ## _Msk)) >> \
     (base ## _ ## reg ## _ ## field ## _Pos))
#endif

#ifndef FSP_REG_VAR_FIELD_SET

/**
 * Set the value of a register field, with the register value being stored in a variable
 * e.g.
 * @code
 * uint16_t tmp;
 *
 * tmp = CRG_TOP->TRIM_CTRL_REG;
 * FSP_REG_VAR_FIELD_SET(CRG_TOP, TRIM_CTRL_REG, XTAL_COUNT_N, tmp, 10);
 * FSP_REG_VAR_FIELD_SET(CRG_TOP, TRIM_CTRL_REG, XTAL_TRIM_SELECT, tmp, 2);
 * CRG_TOP->TRIM_CTRL_REG = tmp;
 * ...
 * @endcode
 */
 #define FSP_REG_VAR_FIELD_SET(base, reg, field, var, val)                     \
    var = (((unsigned) var & ~((base ## _ ## reg ## _ ## field ## _Msk))) |    \
           ((((unsigned) (val)) << (base ## _ ## reg ## _ ## field ## _Pos)) & \
            (base ## _ ## reg ## _ ## field ## _Msk)))
#endif

/**
 * @brief Sets register bits, indicated by the mask, to a value.
 *
 * e.g.
 * @code
 * FSP_REG_SET_MASKED(RFCU_POWER, RF_CNTRL_TIMER_5_REG, 0xFF00, 0x1818);
 * @endcode
 */
#define FSP_REG_SET_MASKED(base, reg, mask, value) \
    base->reg = ((base->reg & ~(mask)) | ((value) & (mask)))

/**
 * @brief Return the value of a memory field using a mask.
 *
 * e.g.
 * @code
 * uint32_t val;
 *
 * val = FSP_MEM_GET_MASKED(0x50000000, 0x1UL);
 * ...
 * @endcode
 */
#define FSP_MEM_GET_MASKED(addr, mask) \
    ((*(volatile uint32_t *) (addr) & (mask)) >> (__builtin_ctz(mask)))

/**
 * \brief Get the address of a register value by index (provided a register stride)
 *
 * \note The register stride should be an exact multiple of the register's base size. For example,
 * if the register size is 32-bit, then the stride should be 0x4, 0x8, etc. Otherwise, the result
 * will be undefined. The stride value must be in bytes. The index value (0,1,2...) is multiplied by
 * the stride value (in bytes) to find the actual offset of the register.
 *
 * Returns a register address value by index
 */
#define FSP_REG_ADDR_GET_INDEXED(base, reg, stride, index) \
    ((&base->reg) + (((intptr_t) (index)) * ((stride) / sizeof(base->reg))))

/**
 * \brief Return the value of a register field by index (provided a register stride).
 *
 * e.g.
 * \code
 * uint16_t val;
 * uint16_t index = 2
 *
 * val = REG_GETF_INDEXED(FTDF, FTDF_LONG_ADDR_0_0_REG, REG_EXP_SA_L, 0x10, index)
 *
 * ...
 * \endcode
 *
 * \note The register stride should be an exact multiple of the register's base size. For example,
 * if the register size is 32-bit, then the stride should be 0x4, 0x8, etc. Otherwise, the result
 * will be undefined. The stride value must be in bytes. The index value (0,1,2...) is multiplied by
 * the stride value (in bytes) to find the actual offset of the register.
 *
 */
#define FSP_REG_FIELD_GET_INDEXED(base, reg, field, stride, index) \
    (((*FSP_REG_ADDR_GET_INDEXED(base, reg, (stride), (index))) &  \
      (base ## _ ## reg ## _ ## field ## _Msk)) >> (base ## _ ## reg ## _ ## field ## _Pos))

/**
 * @brief Sets memory bits, indicated by the mask, to a value.
 *
 * e.g.
 * @code
 * FSP_MEM_SET_MASKED(0x50000000, 0xFF00, 0x1818);
 * @endcode
 */
#define FSP_MEM_SET_MASKED(addr, mask, value) \
    (*(volatile uint32_t *) (addr)) = (((*(volatile uint32_t *) (addr)) & ~(mask)) | ((value) & (mask)))

#define FSP_ARRAY_LENGTH(array)    (sizeof((array)) / sizeof((array)[0]))

/****************************************************************
 *
 * This check is performed to select suitable ASM API with respect to core
 *
 * The macros __CORE__ , __ARM7EM__ and __ARM_ARCH_8M_BASE__ are undefined for GCC, but defined(__IAR_SYSTEMS_ICC__) is false for GCC, so
 * the left half of the || expression evaluates to false for GCC regardless of the values of these macros. */

#if (defined(__IAR_SYSTEMS_ICC__) && ((__CORE__ == __ARM7EM__) || (__CORE__ == __ARM_ARCH_8M_BASE__))) || \
    defined(__ARM_ARCH_7EM__)          // CM4
 #ifndef BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION
  #define BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION    (0U)
 #endif
#else // CM23
 #ifdef BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION
  #undef BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION
 #endif
 #define BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION    (0U)
#endif

/* This macro defines a variable for saving previous mask value */
#ifndef FSP_CRITICAL_SECTION_DEFINE

 #define FSP_CRITICAL_SECTION_DEFINE    uint32_t old_mask_level = 0U
#endif

/* These macros abstract methods to save and restore the interrupt state for different architectures. */
#ifndef UNIT_TESTING
 #if (0 == BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION)
  #define FSP_CRITICAL_SECTION_GET_CURRENT_STATE    __get_PRIMASK
  #define FSP_CRITICAL_SECTION_SET_STATE            __set_PRIMASK
  #define FSP_CRITICAL_SECTION_IRQ_MASK_SET         (1U)
 #else
  #define FSP_CRITICAL_SECTION_GET_CURRENT_STATE    __get_BASEPRI
  #define FSP_CRITICAL_SECTION_SET_STATE            __set_BASEPRI
  #define FSP_CRITICAL_SECTION_IRQ_MASK_SET         ((uint8_t) (BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION << \
                                                                (8U - __NVIC_PRIO_BITS)))
 #endif
#else
 #define FSP_CRITICAL_SECTION_GET_CURRENT_STATE()    (0)
 #define FSP_CRITICAL_SECTION_SET_STATE(x)           (void) x
 #define FSP_CRITICAL_SECTION_IRQ_MASK_SET          (1U)
#endif

/** This macro temporarily saves the current interrupt state and disables interrupts. */
#ifndef FSP_CRITICAL_SECTION_ENTER
 #define FSP_CRITICAL_SECTION_ENTER                            \
    old_mask_level = FSP_CRITICAL_SECTION_GET_CURRENT_STATE(); \
    FSP_CRITICAL_SECTION_SET_STATE(FSP_CRITICAL_SECTION_IRQ_MASK_SET)
#endif

/** This macro restores the previously saved interrupt state, reenabling interrupts. */
#ifndef FSP_CRITICAL_SECTION_EXIT
 #define FSP_CRITICAL_SECTION_EXIT              FSP_CRITICAL_SECTION_SET_STATE(old_mask_level)
#endif

/* Number of Cortex processor exceptions, used as an offset from XPSR value for the IRQn_Type macro. */
#define FSP_PRIV_CORTEX_PROCESSOR_EXCEPTIONS    (16U)

/** Used to signify that the requested IRQ vector is not defined in this system. */
#define FSP_INVALID_VECTOR                      ((IRQn_Type) - 33)

/* Private definition used in bsp_clocks and R_FSP_SystemClockHzGet. Each bitfield in SCKDIVCR is up to 4 bits wide. */
#if (BSP_CFG_MCU_PART_SERIES == 8)
 #define FSP_PRV_SCKDIVCR_DIV_MASK              (0xFU)
#else
 #define FSP_PRV_SCKDIVCR_DIV_MASK              (0x7U)
#endif

/* Use the secure registers for secure projects and flat projects. */
#if !BSP_TZ_NONSECURE_BUILD && BSP_FEATURE_TZ_HAS_TRUSTZONE
 #define FSP_PRIV_TZ_USE_SECURE_REGS            (1)
#else
 #define FSP_PRIV_TZ_USE_SECURE_REGS            (0)
#endif

/* Put certain BSP variables in uninitialized RAM when initializing BSP early. */
#if BSP_CFG_EARLY_INIT
 #define BSP_SECTION_EARLY_INIT                 BSP_PLACE_IN_SECTION(BSP_SECTION_NOINIT)
#else
 #define BSP_SECTION_EARLY_INIT
#endif

#if (BSP_TZ_SECURE_BUILD || BSP_TZ_NONSECURE_BUILD) && BSP_FEATURE_TZ_VERSION == 2
BSP_CMSE_NONSECURE_ENTRY uint8_t  R_BSP_NSC_STYPE3_RegU8Read(uint8_t volatile const * p_reg);
BSP_CMSE_NONSECURE_ENTRY uint16_t R_BSP_NSC_STYPE3_RegU16Read(uint16_t volatile const * p_reg);
BSP_CMSE_NONSECURE_ENTRY uint32_t R_BSP_NSC_STYPE3_RegU32Read(uint32_t volatile const * p_reg);

#endif

#if BSP_FEATURE_TZ_HAS_TRUSTZONE && BSP_TZ_NONSECURE_BUILD && BSP_FEATURE_TZ_VERSION == 2

/*
 * If the STYPE3 register's security attribution is set to secure, the non-secure application must read the register
 * from the secure application using the provided non-secure callable functions.
 */
 #define FSP_STYPE3_REG8_READ(X, S)     (!(S) ? X : (R_BSP_NSC_STYPE3_RegU8Read((uint8_t const volatile *) &X)))
 #define FSP_STYPE3_REG16_READ(X, S)    (!(S) ? X : (R_BSP_NSC_STYPE3_RegU16Read((uint16_t const volatile *) &X)))
 #define FSP_STYPE3_REG32_READ(X, S)    (!(S) ? X : (R_BSP_NSC_STYPE3_RegU32Read((uint32_t const volatile *) &X)))
#elif BSP_FEATURE_TZ_HAS_TRUSTZONE && BSP_TZ_SECURE_BUILD && BSP_FEATURE_TZ_VERSION == 2

/*******************************************************************************************************************//**
 * Read a non-secure 8-bit STYPE3 register in the secure state.
 *
 * @param[in]  p_reg The address of the non-secure register.
 *
 * @return     Value read from the register.
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t R_BSP_S_STYPE3_RegU8Read (uint8_t volatile const * p_reg)
{
    p_reg = (uint8_t volatile const *) ((uint32_t) p_reg | BSP_FEATURE_TZ_NS_OFFSET);

    return *p_reg;
}

/*******************************************************************************************************************//**
 * Read a non-secure 16-bit STYPE3 register in the secure state.
 *
 * @param[in]  p_reg The address of the non-secure register.
 *
 * @return     Value read from the register.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_S_STYPE3_RegU16Read (uint16_t volatile const * p_reg)
{
    p_reg = (uint16_t volatile const *) ((uint32_t) p_reg | BSP_FEATURE_TZ_NS_OFFSET);

    return *p_reg;
}

/*******************************************************************************************************************//**
 * Read a non-secure 32-bit STYPE3 register in the secure state.
 *
 * @param[in]  p_reg The address of the non-secure register.
 *
 * @return     Value read from the register.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_BSP_S_STYPE3_RegU32Read (uint32_t volatile const * p_reg)
{
    p_reg = (uint32_t volatile const *) ((uint32_t) p_reg | BSP_FEATURE_TZ_NS_OFFSET);

    return *p_reg;
}

/*
 * If the STYPE3 register's security attribution is set to non-secure, the secure application must read the register
 * using the non-secure aliased address.
 */
 #define FSP_STYPE3_REG8_READ(X, S)     ((S) ? (X) : R_BSP_S_STYPE3_RegU8Read((uint8_t const volatile *) &X))
 #define FSP_STYPE3_REG16_READ(X, S)    ((S) ? (X) : R_BSP_S_STYPE3_RegU16Read((uint16_t const volatile *) &X))
 #define FSP_STYPE3_REG32_READ(X, S)    ((S) ? (X) : R_BSP_S_STYPE3_RegU32Read((uint32_t const volatile *) &X))
#else
 #define FSP_STYPE3_REG8_READ(X, S)     (X)
 #define FSP_STYPE3_REG16_READ(X, S)    (X)
 #define FSP_STYPE3_REG32_READ(X, S)    (X)
#endif

#ifdef UNIT_TESTING
 #define TESTABLE_STATIC
#else
 #define TESTABLE_STATIC        static
#endif

/*******************************************************************************************************************//**
 * Macro to enable the debugger
 *
 ***********************************************************************************************************************/
#define BSP_DEBUGGER_ENABLE     (CRG_TOP->SYS_CTRL_REG_b.DEBUGGER_ENABLE = 1)

/*******************************************************************************************************************//**
 * Macro to disable the debugger
 *
 ***********************************************************************************************************************/
#define BSP_DEBUGGER_DISABLE    (CRG_TOP->SYS_CTRL_REG_b.DEBUGGER_ENABLE = 0)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Different warm start entry locations in the BSP. */
typedef enum e_bsp_warm_start_event
{
    BSP_WARM_START_RESET = 0,          ///< Called almost immediately after reset. No C runtime environment, clocks, or IRQs.
    BSP_WARM_START_POST_CLOCK,         ///< Called after clock initialization. No C runtime environment or IRQs.
    BSP_WARM_START_POST_C              ///< Called after clocks and C runtime environment have been set up
} bsp_warm_start_event_t;

/* Private enum used in R_FSP_SystemClockHzGet.  Maps clock name to base bit in SCKDIVCR. */
typedef enum e_fsp_priv_clock
{
    FSP_PRIV_CLOCK_SYS_CLK = 0,
#ifdef BSP_MCU_GROUP_RA6W1
    FSP_PRIV_CLOCK_CC312       = 1,    ///< Security IP
    FSP_PRIV_CLOCK_OTP         = 2,
    FSP_PRIV_CLOCK_OQSPI_FLASH = 3,
    FSP_PRIV_CLOCK_QSPI_RAM    = 4,
    FSP_PRIV_CLOCK_SPI         = 5,
    FSP_PRIV_CLOCK_TIMER       = 6,
    FSP_PRIV_CLOCK_I2C         = 7,
    FSP_PRIV_CLOCK_SDIO        = 8,
    FSP_PRIV_CLOCK_UART        = 9,
    FSP_PRIV_CLOCK_AUX_ADC     = 10,
    FSP_PRIV_CLOCK_WDT         = 11,
#endif
} fsp_priv_clock_t;

/* Private enum used in R_FSP_SciSpiClockHzGe.  Maps clock name to base bit in SCISPICKCR. */
typedef enum e_fsp_priv_source_clock
{
    FSP_PRIV_CLOCK_HOCO     = 0,       ///< The high speed on chip oscillator
    FSP_PRIV_CLOCK_MOCO     = 1,       ///< The middle speed on chip oscillator
    FSP_PRIV_CLOCK_LOCO     = 2,       ///< The low speed on chip oscillator
    FSP_PRIV_CLOCK_MAIN_OSC = 3,       ///< The main oscillator
    FSP_PRIV_CLOCK_SUBCLOCK = 4,       ///< The subclock oscillator
    FSP_PRIV_CLOCK_PLL      = 5,       ///< The PLL oscillator
    FSP_PRIV_CLOCK_PLL2     = 6,       ///< The PLL2 oscillator
} fsp_priv_source_clock_t;

typedef struct st_bsp_unique_id
{
    union
    {
        uint32_t unique_id_words[4];
        uint8_t  unique_id_bytes[16];
    };
} bsp_unique_id_t;

/* Temporary global time rtm structure located in parallel to ‘_dpm_flag_in_rtm’ structure, 
   till dpm rtm mapping re-order (moving it to a common place) will be done. */
typedef struct _time_in_rtm {
    long               __timezone;      /* timezone */
    unsigned long long systime_offset;  /* msec */
    unsigned long long rtc_oldtime;     /* msec */
} time_in_rtm_t;

#define RTM_TIME_PTR                   ((time_in_rtm_t *)   dg_configAPPSUPP_RTM_ADDR)

/* Support 64bit time */
typedef unsigned long long __time64_t;
typedef __time64_t	ra6w1_time_t;

#if BSP_FEATURE_BSP_HAS_RTC_COUNT

/* Alias name of BSP_WAKEUP_RESET_WITH_RETENTION */
#define BSP_WAKEUP_RETENTION (BSP_WAKEUP_RESET_WITH_RETENTION)

/* e_wakeup_source_mask
 * The value below is consist of a bit mask.
 *  and the rest is a combination of the corresponding values.
 * This value is based on RTC->WAKEUP_SRC_CLR_SIG_REG
 * BSP_WAKEUP_RESET                     0x00
 * BSP_WAKEUP_SOURCE_GPIO               0x01
 * BSP_WAKEUP_SOURCE_WAKEUP_COUNTER     0x02
 * BSP_WAKEUP_SOURCE_POR                0x04
 * BSP_WAKEUP_SOURCE_WATCHDOG           0x08
 * BSP_WAKEUP_SENSOR                    0x10
 * BSP_WAKEUP_PULSE                     0x20
 * BSP_WAKEUP_TIMER                     0x40  - In only sleep 4/5 
 *
 * This value is based on RTC->RTM_INFO by written software
 * BSP_WAKEUP_RESET_WITH_RETENTION      0x80 
 */ 
typedef enum e_wakeup_source_mask {
    BSP_WAKEUP_RESET                                    = 0x00,         /**< Internal reset */
    BSP_WAKEUP_SOURCE_GPIO                              = 0x01,         /**< Boot from GPIO wake up signal */
    BSP_WAKEUP_SOURCE_WAKEUP_COUNTER                    = 0x02,         /**< Boot from wake up counter */
    BSP_WAKEUP_GPIO_WAKEUP_COUNTER                      = 0x03,         /**< Boot from wake up counter or GPIO wakeup */
    BSP_WAKEUP_SOURCE_POR                               = 0x04,         /**< Boot from power on reset */
    BSP_WAKEUP_SOURCE_POR_GPIO                          = 0x05,         /**< FPGA only */
    BSP_WAKEUP_SOURCE_WATCHDOG                          = 0x08,         /**< Boot from RTC_watch dog (not cpu watchdog) */
    BSP_WAKEUP_WATCHDOG_GPIO                            = 0x09,         /**< Boot from watch dog or GPIO wakeup */
    BSP_WAKEUP_SOURCE_SENSOR                            = 0x10,         /**< Boot from sensor (ADC) */
    BSP_WAKEUP_SOURCE_PULSE                             = 0x20,         /**< Boot from pulse sensor */

    BSP_WAKEUP_SENSOR_GPIO                              = 0x11,         /**< Boot from sensor or GPIO wakeup */
    BSP_WAKEUP_SENSOR_WAKEUP_COUNTER                    = 0x12,         /**< Boot from sensor or wakeup counter */
    BSP_WAKEUP_SENSOR_GPIO_COUNTER                      = 0x13,         /**< Boot from sensor or external or wakeup counter */
    BSP_WAKEUP_SENSOR_WATCHDOG                          = 0x18,         /**< Boot from sensor or RTC watch dog */
    BSP_WAKEUP_SENSOR_GPIO_WATCHDOG                     = 0x19,         /**< Boot from sensor or GPIO or watch dog */
    BSP_WAKEUP_RESET_WITH_RETENTION                     = 0x80,         /**< Boot from internal reset and have retention info */
    BSP_WAKEUP_GPIO_WITH_RETENTION                      = 0x81,         /**< Boot from GPIO and have retention info */
    BSP_WAKEUP_COUNTER_WITH_RETENTION                   = 0x82,         /**< Boot from counter and have retention info (most common) */
    BSP_WAKEUP_GPIO_WAKEUP_COUNTER_WITH_RETENTION       = 0x83,         /**< Boot from GPIO or wakeup counter with retention */
    BSP_WAKEUP_WATCHDOG_WITH_RETENTION                  = 0x88,         /**< Boot from RTC watch dog with retention */
    BSP_WAKEUP_WATCHDOG_GPIO_WITH_RETENTION             = 0x89,         /**< Boot from watch dog or GPIO */
    BSP_WAKEUP_SENSOR_WITH_RETENTION                    = 0x90,         /**< Boot from sensor with retention */
    BSP_WAKEUP_SENSOR_GPIO_WITH_RETENTION               = 0x91,         /**< Boot from sensor or GPIO with retention */
    BSP_WAKEUP_SENSOR_WAKEUP_COUNTER_WITH_RETENTION     = 0x92,         /**< Boot from sensor or wakeup counter with retention */
    BSP_WAKEUP_SENSOR_GPIO_COUNTER_WITH_RETENTION       = 0x93,         /**< Boot from sensor or GPIO or wakeup counter with retention */
    BSP_WAKEUP_SENSOR_WATCHDOG_WITH_RETENTION           = 0x98,         /**< Boot from sensor or watchdog with retention */
    BSP_WAKEUP_SENSOR_GPIO_WATCHDOG_WITH_RETENTION      = 0x99,         /**< Boot from sensor or GPIO or watchdog with retention */

    BSP_WAKEUP_ALLTIMER_WITH_RETENTION                  = 0xC2,         /**< Boot from rtc, hw & mac-timer and have retention info (most common) */
    BSP_WAKEUP_HWTIMER_WITH_RETENTION                   = 0xC0,         /**< Boot from hw & mac-timer and have retention info (most common) */

    BSP_WAKEUP_SOURCE_UNKNOWN                           = 0xff,
}bsp_wakeup_source_mask_t;

typedef uint32_t bsp_wakeup_source_t;
#endif

typedef void (* bsp_on_error_cb_t)(uintptr_t args);

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern bsp_on_error_cb_t g_halt_impl;

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

uint32_t R_BSP_SourceClockHzGet(fsp_priv_source_clock_t clock);

/***********************************************************************************************************************
 * Global variables (defined in other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Inline Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Return active interrupt vector number value
 *
 * @return     Active interrupt vector number value
 **********************************************************************************************************************/
__STATIC_INLINE IRQn_Type R_FSP_CurrentIrqGet (void)
{
    xPSR_Type xpsr_value;
    xpsr_value.w = __get_xPSR();

    return (IRQn_Type) (xpsr_value.b.ISR - FSP_PRIV_CORTEX_PROCESSOR_EXCEPTIONS);
}

/*******************************************************************************************************************//**
 * Gets the frequency of a system clock.
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_FSP_SystemClockHzGet (fsp_priv_clock_t clock)
{
    uint32_t hz;

#ifdef BSP_MCU_GROUP_RA6W1
    uint32_t HCLK = SystemCoreClock / 1; /* CLK_AMBA_REG.HCLK_DIV = 0 */
    uint32_t PERI_CLK = SystemCoreClock; /* due to XTAL mode */
#endif

    switch (clock)
    {
        case FSP_PRIV_CLOCK_SYS_CLK:
        {
            hz = SystemCoreClock;
            break;
        }

#ifdef BSP_MCU_GROUP_RA6W1
        case FSP_PRIV_CLOCK_CC312:
        case FSP_PRIV_CLOCK_OTP:
        {
            hz = HCLK; /* there are no dividers applied when SYS_CLK = XTAL40M */
            break;
        }

        case FSP_PRIV_CLOCK_OQSPI_FLASH:
        case FSP_PRIV_CLOCK_QSPI_RAM:
        {
            hz = SystemCoreClock;
            break;
        }

        case FSP_PRIV_CLOCK_SPI:
        {
            uint32_t SPI_CLK = 0;

            if (3 == CRG_TOP->CLK_CTRL_REG_b.SYS_CLK_SEL) /* SYS_CLK = SYS-PLL */
            {
                uint8_t spi_clk_bit = CRG_TOP->CLK_CTRL_REG_b.PLL_SPI_CLK_SEL;

                if (0 == spi_clk_bit) /* SPI_CLK = 120Mhz */
                {
                    SPI_CLK = 120000000;
                }
                else if (1 == spi_clk_bit) /* SPI_CLK = 96Mhz */
                {
                    SPI_CLK = 96000000;
                }
                else if (2 == spi_clk_bit) /* SPI_CLK = 80Mhz */
                {
                    SPI_CLK = 80000000;
                }
                else                       /* SPI_CLK = 53Mhz */
                {
                    SPI_CLK = 53000000;
                }
            }
            else /* SYS_CLK = XTAL40M */
            {
                SPI_CLK = SystemCoreClock; /* due to XTAL mode */
            }

            hz = SPI_CLK;
            break;
        }

        case FSP_PRIV_CLOCK_TIMER:
        case FSP_PRIV_CLOCK_I2C:
        case FSP_PRIV_CLOCK_SDIO:
        case FSP_PRIV_CLOCK_UART:
        {
            hz = CRG_TOP->CLK_CTRL_REG_b.PLL_PERI_ENABLE ? 2 * PERI_CLK : PERI_CLK;
            break;
        }

        case FSP_PRIV_CLOCK_AUX_ADC:
        {
            uint32_t AUX_CLK = 15000000; /* 15 MHz */
            hz = AUX_CLK;
            break;
        }

        case FSP_PRIV_CLOCK_WDT:
        {
#if BSP_CFG_LP_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_LP_CLK_XTAL32K
            hz = BSP_CFG_XTAL_HZ;
#else
            hz = BSP_CFG_RCX_HZ;
#endif
            break;
        }
#endif

        default:
        {
            hz = 0; /* avoid possible compilation warning/error: return of uninitialized value */
            BSP_CHECK_FATAL(0);
        }
    }
    return hz;
}

/*******************************************************************************************************************//**
 * Converts a clock's CKDIVCR register value to a clock divider (Eg: SPICKDIVCR).
 *
 * @return     Clock Divider
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_FSP_ClockDividerGet (uint32_t ckdivcr)
{
    if (2U >= ckdivcr)
    {

        /* clock_div:
         * - Clock Divided by 1: 0
         * - Clock Divided by 2: 1
         * - Clock Divided by 4: 2
         */
        return 1 << ckdivcr;
    }
    else if (3U == ckdivcr)
    {

        /* Clock Divided by 6 */
        return 6U;
    }
    else if (4U == ckdivcr)
    {

        /* Clock Divided by 8 */
        return 8U;
    }
    else if (5U == ckdivcr)
    {

        /* Clock Divided by 3 */
        return 3U;
    }

    /* Clock Divided by 5 */
    return 5U;
}

#if BSP_FEATURE_BSP_HAS_SCISPI_CLOCK

/*******************************************************************************************************************//**
 * Gets the frequency of a SCI/SPI clock.
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_FSP_SciSpiClockHzGet (void)
{
    uint32_t                scispidivcr = R_SYSTEM->SCISPICKDIVCR;
    uint32_t                clock_div   = R_FSP_ClockDividerGet(scispidivcr & FSP_PRV_SCKDIVCR_DIV_MASK);
    fsp_priv_source_clock_t scispicksel = (fsp_priv_source_clock_t) R_SYSTEM->SCISPICKCR_b.SCISPICKSEL;

    return R_BSP_SourceClockHzGet(scispicksel) / clock_div;
}

#endif
#if BSP_FEATURE_BSP_HAS_SPI_CLOCK

/*******************************************************************************************************************//**
 * Gets the frequency of a SPI clock.
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_FSP_SpiClockHzGet (void)
{
    uint32_t                spidivcr  = FSP_STYPE3_REG8_READ(R_SYSTEM->SPICKDIVCR, BSP_CFG_CLOCKS_SECURE);
    uint32_t                clock_div = R_FSP_ClockDividerGet(spidivcr & FSP_PRV_SCKDIVCR_DIV_MASK);
    fsp_priv_source_clock_t spicksel  =
        (fsp_priv_source_clock_t) ((FSP_STYPE3_REG8_READ(R_SYSTEM->SPICKCR,
                                                         BSP_CFG_CLOCKS_SECURE) & R_SYSTEM_SPICKCR_CKSEL_Msk) >>
                                   R_SYSTEM_SPICKCR_CKSEL_Pos);

    return R_BSP_SourceClockHzGet(spicksel) / clock_div;
}

#endif
#if BSP_FEATURE_BSP_HAS_SCI_CLOCK

/*******************************************************************************************************************//**
 * Gets the frequency of a SCI clock.
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_FSP_SciClockHzGet (void)
{
    uint32_t                scidivcr  = FSP_STYPE3_REG8_READ(R_SYSTEM->SCICKDIVCR, BSP_CFG_CLOCKS_SECURE);
    uint32_t                clock_div = R_FSP_ClockDividerGet(scidivcr & FSP_PRV_SCKDIVCR_DIV_MASK);
    fsp_priv_source_clock_t scicksel  =
        (fsp_priv_source_clock_t) (FSP_STYPE3_REG8_READ(R_SYSTEM->SCICKCR,
                                                        BSP_CFG_CLOCKS_SECURE) & R_SYSTEM_SCICKCR_SCICKSEL_Msk >>
                                   R_SYSTEM_SCICKCR_SCICKSEL_Pos);

    return R_BSP_SourceClockHzGet(scicksel) / clock_div;
}

#endif

#if BSP_FEATURE_BSP_HAS_RTC_COUNT

#define CLOCK2SEC(clk)  (((unsigned long long )clk) >> 15ULL)
#define __uptime()      CLOCK2SEC(R_BSP_SystemRtcCountGet()) //Get elapsed time after system power up(for 64bit)

/*******************************************************************************************************************//**
 * Gets the clock count of RTC
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE uint64_t R_BSP_SystemRtcCountGet (void)
{

    volatile uint64_t ret_time = 0;

    /* Check RTC register mirror done */
    if (RTC->RTC_IRQ_STATUS_REG_b.RTC_IRQ_STATUS == 0x01)
    {
        /* faster to access register on rtc thourgh mirror.*/
        ret_time |= RTC->RTC_MR_FRC0_REG ;
        ret_time |= (uint64_t)(RTC->RTC_MR_FRC1_REG) << 32;
    }
    else
    {
        /* direct to access register on rtc */
        ret_time |= RTC->FRC_CNT_0_REG ;
        ret_time |= (uint64_t)RTC->FRC_CNT_1_REG << 32;
    }

    return  ret_time;
}

/*******************************************************************************************************************//**
 * Calculate diff between to RTC values.
 * Both 'from' and 'to' are assumed to be 36 bits wide
 *
 * @return     RTC diff between 'from' and 'to'.
 **********************************************************************************************************************/
__STATIC_INLINE uint64_t R_BSP_SystemRtcDiff(uint64_t from, uint64_t to)
{
    const int64_t full_range = (1LL << 36);
    const int64_t half_range = full_range >> 1;
    const int64_t diff = (int64_t) to - (int64_t) from;

    if (diff >= 0)
    {
        /* 'from' comes after 'to', but it wrapped : 'to' comes after 'from', no wrapping */
        return diff >= half_range ? 0 : (uint64_t) (diff);
    }
    else
    {
        /* 'from' comes after 'to', no wrapping : 'to' comes after 'from', but it wrapped */
        return diff >= -half_range ? 0 : (uint64_t) (diff + full_range);
    }
}

/*******************************************************************************************************************//**
 * Wait the clock based on RTC clock
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_SystemRtcWaitClk(int32_t delay_clk)
{
#if defined(TESET_CODE)
    const uint64_t rtc_mask = (1LLU << 36) - 1;
    uint64_t old_clk = R_BSP_SystemRtcCountGet();
    uint64_t new_clk = old_clk;
    do {
        new_clk = R_BSP_SystemRtcCountGet();
    } while (((new_clk - old_clk) & rtc_mask) < (uint64_t) delay_clk);
#else
    uint64_t p_clk = R_BSP_SystemRtcCountGet();
    do {
        uint64_t c = R_BSP_SystemRtcCountGet();
        if (p_clk != c)
            delay_clk--;
        p_clk = c;
    } while (delay_clk > 0);

#endif
}
#endif

/*******************************************************************************************************************//**
 * Get unique ID for this device.
 *
 * @return  A pointer to the unique identifier structure
 **********************************************************************************************************************/
__STATIC_INLINE bsp_unique_id_t const * R_BSP_UniqueIdGet (void)
{

    // TODO
    return 0;
#if 0

    return (bsp_unique_id_t *) BSP_FEATURE_BSP_UNIQUE_ID_POINTER;
#endif
}

/*******************************************************************************************************************//**
 * Disables the flash cache.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_FlashCacheDisable (void)
{
#if BSP_FEATURE_BSP_FLASH_CACHE
    R_FCACHE->FCACHEE = 0U;
#endif

#if BSP_FEATURE_BSP_HAS_CODE_SYSTEM_CACHE

    /* Disable the C-Cache. */
    R_CACHE->CCACTL = 0U;
#endif
}

/*******************************************************************************************************************//**
 * Enables the flash cache.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_FlashCacheEnable (void)
{
#if BSP_FEATURE_BSP_FLASH_CACHE

    /* Invalidate the flash cache and wait until it is invalidated. (See section 55.3.2.2 "Operation" of the Flash Cache
     * in the RA6M3 manual R01UH0878EJ0100). */
    R_FCACHE->FCACHEIV = 1U;
    FSP_HARDWARE_REGISTER_WAIT(R_FCACHE->FCACHEIV, 0U);

    /* Enable flash cache. */
    R_FCACHE->FCACHEE = 1U;
#endif

#if BSP_FEATURE_BSP_HAS_CODE_SYSTEM_CACHE

    /* Configure the C-Cache line size. */
    R_CACHE->CCALCF = BSP_CFG_C_CACHE_LINE_SIZE;

    /* Enable the C-Cache. */
    R_CACHE->CCACTL = 1U;
#endif
}

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
#if ((1 == BSP_CFG_ERROR_LOG) || (1 == BSP_CFG_ASSERT))

/** Prototype of default function called before errors are returned in FSP code if BSP_CFG_LOG_ERRORS is set to 1. */
void fsp_error_log(fsp_err_t err, const char * file, int32_t line);

#endif

/** In the event of an unrecoverable error the BSP will by default call the __BKPT() intrinsic function which will
 *  alert the user of the error. The user can override this default behavior by defining their own
 *  BSP_CFG_HANDLE_UNRECOVERABLE_ERROR or CUSTOM_BSP_CFG_HANDLE_UNRECOVERABLE_ERROR macro.
 */
#if defined(CUSTOM_UNRECOVERABLE_ERROR_HANDLER)
 #define BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(x)    CUSTOM_UNRECOVERABLE_ERROR_HANDLER
#elif !defined(BSP_CFG_HANDLE_UNRECOVERABLE_ERROR)

 #define BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(x)    __BKPT((x))
#endif

/** @} (end addtogroup BSP_MCU) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
