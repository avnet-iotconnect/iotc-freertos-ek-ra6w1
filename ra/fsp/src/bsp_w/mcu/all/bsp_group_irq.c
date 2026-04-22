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

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_GRP_IRQ_TOTAL_ITEMS    (16U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function prototypes
 **********************************************************************************************************************/

BSP_WEAK_REFERENCE void NMI_HandlerC(unsigned long * exception_args);
BSP_WEAK_REFERENCE void HardFault_HandlerC(unsigned long * exception_args);

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/** This array holds callback functions. */
bsp_grp_irq_cb_t g_bsp_group_irq_sources[BSP_GRP_IRQ_TOTAL_ITEMS] BSP_SECTION_EARLY_INIT;

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Register a callback function for supported interrupts. If NULL is passed for the callback argument then any
 * previously registered callbacks are unregistered.
 *
 * @param[in]  irq          Interrupt for which  to register a callback.
 * @param[in]  p_callback   Pointer to function to call when interrupt occurs.
 *
 * @retval     FSP_SUCCESS                  Callback registered
 * @retval     FSP_ERR_ASSERTION            Callback pointer is NULL
 **********************************************************************************************************************/
fsp_err_t R_BSP_GroupIrqWrite (bsp_grp_irq_t irq, void (* p_callback)(bsp_grp_irq_t irq))
{
#if BSP_CFG_PARAM_CHECKING_ENABLE

    /* Check pointer for NULL value. */
    FSP_ASSERT(p_callback);
#endif

    /* Register callback. */
    g_bsp_group_irq_sources[irq] = p_callback;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Non-maskable interrupt handler. This exception is defined by the BSP, unlike other system exceptions, because
 * there are many sources that map to the NMI exception.
 **********************************************************************************************************************/
BSP_WEAK_REFERENCE void NMI_HandlerC (unsigned long * exception_args)
{
    (void) exception_args;

    while (1)
    {
    }
}

BSP_WEAK_REFERENCE void HardFault_HandlerC (unsigned long * exception_args)
{
    (void) exception_args;

    while (1)
    {
    }
}

/** @} (end addtogroup BSP_MCU) */
