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

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

#ifndef BSP_IO_ISOLATION_H
#define BSP_IO_ISOLATION_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Isolate pads, by disabling the latches of all GPIOs
 *
 * @note    Effectively, the GPIO pads are disconnected from their internal drives and retain their
 *          current level.
 **********************************************************************************************************************/
__STATIC_INLINE void bsp_io_pads_isolation_enable_all (void)
{
    CRG_TOP->P0_RESET_PAD_LATCH_REG = CRG_TOP_P0_RESET_PAD_LATCH_REG_P0_RESET_LATCH_OPEN_Msk;
    CRG_TOP->P1_RESET_PAD_LATCH_REG = CRG_TOP_P1_RESET_PAD_LATCH_REG_P1_RESET_LATCH_OPEN_Msk;
#ifdef CRG_TOP_P2_RESET_PAD_LATCH_REG_P2_RESET_LATCH_EN_Msk
    CRG_TOP->P2_RESET_PAD_LATCH_REG = CRG_TOP_P2_RESET_PAD_LATCH_REG_P2_RESET_LATCH_EN_Msk;
#endif
}

/*******************************************************************************************************************//**
 * De-isolate pads, by enabling the latches of all GPIOs
 *
 * @note    Effectively, the GPIO pads are connected to their internal drives
 **********************************************************************************************************************/
__STATIC_INLINE void bsp_io_pads_isolation_disable_all (void)
{
    CRG_TOP->P0_SET_PAD_LATCH_REG = CRG_TOP_P0_SET_PAD_LATCH_REG_P0_SET_LATCH_OPEN_Msk;
    CRG_TOP->P1_SET_PAD_LATCH_REG = CRG_TOP_P1_SET_PAD_LATCH_REG_P1_SET_LATCH_OPEN_Msk;
#ifdef CRG_TOP_P2_SET_PAD_LATCH_REG_P2_SET_LATCH_EN_Msk
    CRG_TOP->P2_SET_PAD_LATCH_REG = CRG_TOP_P2_SET_PAD_LATCH_REG_P2_SET_LATCH_EN_Msk;
#endif
}

/*******************************************************************************************************************//**
 * Disables the latch for the specific gpio.
 *
 * @param [in] pin The IOPORT pin to unlatch
 ***********************************************************************************************************************/
__STATIC_INLINE void bsp_io_pad_isolation_enable (bsp_io_port_pin_t pin)
{
    bsp_io_port_pin_t pin_id  = (bsp_io_port_pin_t) (BSP_IO_PIN_BITS & (uint16_t) pin);
    bsp_io_port_t     port_id =
        (bsp_io_port_t) ((BSP_IO_PORT_BITS & (uint16_t) pin) >> BSP_IO_PORT_OFFSET);
    BSP_CHECK_DEBUG((BSP_IO_PORT_00 == port_id) || (BSP_IO_PORT_01 == port_id));

    if (BSP_IO_PORT_00 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT0_GPIO_COUNT);
        CRG_TOP->P0_RESET_PAD_LATCH_REG = 1 << pin_id;
    }
    else if (BSP_IO_PORT_01 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT1_GPIO_COUNT);
        CRG_TOP->P1_RESET_PAD_LATCH_REG = 1 << pin_id;
    }
}

/*******************************************************************************************************************//**
 * Enables the latch for the specific gpio.
 *
 * @param[in] pin The IOPORT pin to latch
 **********************************************************************************************************************/
__STATIC_INLINE void bsp_io_pad_isolation_disable (bsp_io_port_pin_t pin)
{
    bsp_io_port_pin_t pin_id  = (bsp_io_port_pin_t) (BSP_IO_PIN_BITS & (uint16_t) pin);
    bsp_io_port_t     port_id =
        (bsp_io_port_t) ((BSP_IO_PORT_BITS & (uint16_t) pin) >> BSP_IO_PORT_OFFSET);
    BSP_CHECK_DEBUG((BSP_IO_PORT_00 == port_id) || (BSP_IO_PORT_01 == port_id));

    if (BSP_IO_PORT_00 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT0_GPIO_COUNT);
        CRG_TOP->P0_SET_PAD_LATCH_REG = 1 << pin_id;
    }
    else if (BSP_IO_PORT_01 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT1_GPIO_COUNT);
        CRG_TOP->P1_SET_PAD_LATCH_REG = 1 << pin_id;
    }
}

/*******************************************************************************************************************//**
 * Checks if the specific GPIO is latched or not.
 *
 * @param[in] pin The GPIO pin to check latch status for
 **********************************************************************************************************************/
__STATIC_INLINE bool bsp_io_pad_isolation_is_disabled (bsp_io_port_pin_t pin)
{
    bsp_io_port_pin_t pin_id  = (bsp_io_port_pin_t) (BSP_IO_PIN_BITS & (uint16_t) pin);
    bsp_io_port_t     port_id =
        (bsp_io_port_t) ((BSP_IO_PORT_BITS & (uint16_t) pin) >> BSP_IO_PORT_OFFSET);
    BSP_CHECK_DEBUG((BSP_IO_PORT_00 == port_id) || (BSP_IO_PORT_01 == port_id));

    if (BSP_IO_PORT_00 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT0_GPIO_COUNT);

        return CRG_TOP->P0_PAD_LATCH_REG & (1 << pin_id);
    }
    else if (BSP_IO_PORT_01 == port_id)
    {
        BSP_CHECK_DEBUG(pin_id < BSP_FEATURE_IO_PORT1_GPIO_COUNT);

        return CRG_TOP->P1_PAD_LATCH_REG & (1 << pin_id);
    }

    return false;
}

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
