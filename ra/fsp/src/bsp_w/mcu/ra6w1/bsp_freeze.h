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

#ifndef BSP_FREEZE_H
#define BSP_FREEZE_H

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Peripheral to freeze */
typedef enum e_bsp_freeze_peripheral_ra6w1
{
    BSP_FREEZE_PERIPHERAL_WKUPTIM   = 0,
    BSP_FREEZE_PERIPHERAL_SWTIM     = 1,
    BSP_FREEZE_PERIPHERAL_SWTIM2    = 2,
    BSP_FREEZE_PERIPHERAL_SWTIM3    = 3,
    BSP_FREEZE_PERIPHERAL_SWTIM4    = 4,
    BSP_FREEZE_PERIPHERAL_SWTIM5    = 5,
    BSP_FREEZE_PERIPHERAL_SWTIM6    = 6,
    BSP_FREEZE_PERIPHERAL_SWTIM7    = 7,
    BSP_FREEZE_PERIPHERAL_SWTIM8    = 8,
    BSP_FREEZE_PERIPHERAL_SYS_WDOG  = 9,
    BSP_FREEZE_PERIPHERAL_DMA       = 11,
    BSP_FREEZE_PERIPHERAL_KDMA      = 12,
} bsp_freeze_peripheral_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************/ /**
 * Suspend activity of specified peripheral on MCU.
 *
 * @param[in] peripheral     Peripheral to freeze.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PeripheralFreeze (bsp_freeze_peripheral_t peripheral)
{
    CRG_TOP->SET_FREEZE_REG = (uint32_t) (1 << peripheral);
}

/*******************************************************************************************************************/ /**
 * Check whether specified peripheral is frozen or not.
 *
 * @param[in] peripheral     Peripheral to check.
 *
 * @retval true              Peripheral is frozen.
 * @retval false             Peripheral is active.
 **********************************************************************************************************************/
__STATIC_INLINE bool R_BSP_IsPeripheralFrozen (bsp_freeze_peripheral_t peripheral)
{
    return !!(CRG_TOP->SET_FREEZE_REG & ((uint32_t) (1 << peripheral)));
}

/*******************************************************************************************************************/ /**
 * Resume activity of specified peripheral on MCU.
 *
 * @param[in] peripheral     Peripheral to unfreeze.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PeripheralUnFreeze (bsp_freeze_peripheral_t peripheral)
{
    CRG_TOP->RESET_FREEZE_REG = (uint32_t) (1 << peripheral);
}

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
