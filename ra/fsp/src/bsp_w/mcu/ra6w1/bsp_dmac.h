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

#ifndef BSP_DMAC_H
#define BSP_DMAC_H

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Channel request trigger */
typedef enum e_bsp_dmac_trig_ra6w1
{
    BSP_DMAC_TRIG_SPI_RX      = 0x0,
    BSP_DMAC_TRIG_SPI_TX      = 0x1,
    BSP_DMAC_TRIG_SPI2_RX     = 0x2,
    BSP_DMAC_TRIG_SPI2_TX     = 0x3,
    BSP_DMAC_TRIG_UART_RX     = 0x4,
    BSP_DMAC_TRIG_UART_TX     = 0x5,
    BSP_DMAC_TRIG_UART2_RX    = 0x6,
    BSP_DMAC_TRIG_UART2_TX    = 0x7,
    BSP_DMAC_TRIG_UART3_RX    = 0x8,
    BSP_DMAC_TRIG_UART3_TX    = 0x9,
    BSP_DMAC_TRIG_I2C_RX      = 0xA,
    BSP_DMAC_TRIG_I2C_TX      = 0xB,
    BSP_DMAC_TRIG_I2C2_RX     = 0xC,
    BSP_DMAC_TRIG_I2C2_TX     = 0xD,
    BSP_DMAC_TRIG_AUXADC0     = 0xE,
    BSP_DMAC_TRIG_AUXADC1     = 0xF,
    BSP_DMAC_TRIG_AUXADC2     = 0x10,
    BSP_DMAC_TRIG_AUXADC3     = 0x11,
    BSP_DMAC_TRIG_SRC_IN      = 0x12,
    BSP_DMAC_TRIG_SRC_OUT     = 0x13,
    BSP_DMAC_TRIG_DAI_TX      = 0x14,
    BSP_DMAC_TRIG_DAI_RX      = 0x15,
    BSP_DMAC_TRIG_NONE        = 0x1F
} bsp_dmac_trig_t;

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
 * Check whether specified DMA trigger is edge sensitive or not.
 *
 * @param[in] trigger        DMA trigger.
 *
 * @retval true              Trigger is (positive) edge-sensitive.
 * @retval false             Trigger is level-sensitive.
 **********************************************************************************************************************/
__STATIC_INLINE bool R_BSP_DMAC_IsEdgeSensitiveTrigger (bsp_dmac_trig_t trigger)
{
    FSP_PARAMETER_NOT_USED(trigger);
    return false;
}

/*******************************************************************************************************************/ /**
 * Enable interrupt generation by specified DMA channel.
 *
 * @param[in] channel        DMA channel.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_DMAC_ChannelInterruptsEnable (uint8_t channel)
{
    FSP_CRITICAL_SECTION_DEFINE;

    FSP_CRITICAL_SECTION_ENTER;
    DMA->DMA_INT_MASK_REG |= 1U << channel;
    FSP_CRITICAL_SECTION_EXIT;
}

/*******************************************************************************************************************/ /**
 * Disable interrupt generation by specified DMA channel.
 *
 * @param[in] channel        DMA channel.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_DMAC_ChannelInterruptsDisable (uint8_t channel)
{
    FSP_CRITICAL_SECTION_DEFINE;

    FSP_CRITICAL_SECTION_ENTER;
    DMA->DMA_INT_MASK_REG &= ~(1U << channel);
    FSP_CRITICAL_SECTION_EXIT;
}

/** @} (end addtogroup BSP_MCU) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
