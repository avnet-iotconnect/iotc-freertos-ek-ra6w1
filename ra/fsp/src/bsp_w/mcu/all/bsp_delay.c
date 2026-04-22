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
#include "bsp_api.h"
#include "bsp_delay.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_DELAY_OVERHEAD_CYCLES    (72)
#define BSP_CYCLES_PER_DELAY_REP     (4)
#define BSP_MAX_DELAY_CYCLES         (0xFFFFFFFF)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 *              Delay for at least the specified duration in units and return.
 * @param[in]   delay  The number of 'units' to delay.
 * @param[in]   units  The 'base' (bsp_delay_units_t) for the units specified. Valid values are:
 *              BSP_DELAY_UNITS_SECONDS, BSP_DELAY_UNITS_MILLISECONDS, BSP_DELAY_UNITS_MICROSECONDS.@n
 *
 * @note This function calls bsp_cpu_clock_get() which ultimately calls R_CGC_SystemClockFreqGet() and therefore requires
 *       that the BSP has already initialized the CGC (which it does as part of the Sysinit).
 *       Care should be taken to ensure this remains the case if in the future this function were to be called as part
 *       of the BSP initialization.
 *
 * @note Delays may be longer than expected when compiler optimization is turned off.
 **********************************************************************************************************************/
#pragma GCC push_options
#pragma GCC optimize ("O3")
BSP_PLACE_CODE_IN_RAM void R_BSP_SoftwareDelay (uint32_t delay, bsp_delay_units_t units)
{
    uint32_t iclk_hz;
    uint32_t hclk_div;
    uint32_t reps     = 0;
    uint32_t total_us = (delay * units); /** Convert the requested time to microseconds. */

    static const uint8_t  OVERHEAD_REPS = BSP_DELAY_OVERHEAD_CYCLES / BSP_CYCLES_PER_DELAY_REP;
    static const uint32_t DIVIDER       = 1000000;

    iclk_hz = SystemCoreClock / DIVIDER; /** Get the system clock frequency in Hz. */

    hclk_div = CRG_TOP->CLK_AMBA_REG_b.HCLK_DIV;

    const uint32_t cycles_per_usec = iclk_hz >> hclk_div;

    reps = cycles_per_usec * total_us / BSP_CYCLES_PER_DELAY_REP;

#ifdef CLK_DELAY_SANITY_CHECKS
    BSP_CHECK_DEBUG(usec <= (BSP_MAX_DELAY_CYCLES / cycles_per_usec)); // The requested delay is greater than the maximum delay this function can achieve
    BSP_CHECK_DEBUG(reps > OVERHEAD_REPS);                             // The requested delay is smaller than the minimum delay this function can achieve.
#endif

    /** Only delay if the supplied parameters constitute a delay. */
    if ((reps <= OVERHEAD_REPS) || (total_us > (BSP_MAX_DELAY_CYCLES / cycles_per_usec)))
    {
        return;
    }

    __asm volatile (
        "       nop                             \n"
        "       nop                             \n"
        "       nop                             \n"
        "       nop                             \n"
        "       nop                             \n"
        "loop:  nop                             \n"
        "       subs %[reps], %[reps], #1       \n"
        "       bne loop                        \n"
        :                                  // outputs
        :[reps] "r" (reps - OVERHEAD_REPS) // inputs
        :                                  // clobbers
        );
}

#pragma GCC pop_options

/** @} (end addtogroup BSP_MCU) */
