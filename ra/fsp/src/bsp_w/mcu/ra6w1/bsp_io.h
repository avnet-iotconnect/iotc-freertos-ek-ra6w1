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
 * @defgroup BSP_IO BSP I/O access
 * @ingroup RENESAS_COMMON
 * @brief This module provides basic read/write access to port pins.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_IO_RA6W1_H
#define BSP_IO_RA6W1_H

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_IO_PXX_MODE_REG(_port, _pin)    *(&(GPIO->P0_00_MODE_REG) + ((_port) * 14) + (_pin))
#define BSP_IO_PXX_SEL_REG(_port)           *(&(GPIO->GPIO_SEL_P0_REG) + (_port))
#define BSP_IO_PXX_SEL1_REG(_port)          *(&(GPIO->GPIO_SEL1_P0_REG) + (_port))
#define BSP_IO_PXX_POL_REG(_port)           *(&(GPIO->GPIO_INT_POL_P0_REG) + (_port))
#define BSP_IO_PXX_INT_STS_REG(_port)       *(&(GPIO->GPIO_INT_STS_P0_REG) + (_port))
#define BSP_IO_PXX_INT_CLR_REG(_port)       *(&(GPIO->GPIO_INT_CLR_P0_REG) + (_port))

#define BSP_FEATURE_IO_PORT_COUNT          (2U)
#define BSP_FEATURE_IO_PORT0_GPIO_COUNT    (14U)
#define BSP_FEATURE_IO_PORT1_GPIO_COUNT    (18U)
#define BSP_FEATURE_IO_PORT2_GPIO_COUNT    (0U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Drive strength definitions for AA */
typedef enum e_bsp_pad_drive_strength
{
    BSP_PAD_DRIVE_STRENGTH_2MA  = 0,   ///< Sets pad drive strength 2 mA
    BSP_PAD_DRIVE_STRENGTH_12MA = 1,   ///< Sets pad drive strength 12 mA
    BSP_PAD_DRIVE_STRENGTH_4MA  = 2,   ///< Sets pad drive strength 4 mA
    BSP_PAD_DRIVE_STRENGTH_18MA = 3,   ///< Sets pad drive strength 18 mA
} bsp_pad_drive_strength_t;

/** Drive strength definitions for BA */
typedef enum e_bsp_pad_drive_strength_ba
{
    BSP_PAD_DRIVE_STRENGTH_BA_2MA  = 0, ///< Sets pad drive strength 2 mA
    BSP_PAD_DRIVE_STRENGTH_BA_4MA  = 1, ///< Sets pad drive strength 4 mA
    BSP_PAD_DRIVE_STRENGTH_BA_8MA  = 2, ///< Sets pad drive strength 8 mA
    BSP_PAD_DRIVE_STRENGTH_BA_14MA = 3, ///< Sets pad drive strength 14 mA
} bsp_pad_drive_strength_ba_t;

/** QSPI pads drive current definitions for AA */
typedef enum e_bsp_qspic_pads_drive_current
{
    BSP_QSPIC_PADS_DRIVE_CURRENT_4MA  = 0, ///< Sets pad drive current 4 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_8MA  = 1, ///< Sets pad drive current 8 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_12MA = 2, ///< Sets pad drive current 12 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_16MA = 3, ///< Sets pad drive current 16 mA
} bsp_qspic_pads_drive_current_t;

/** QSPI pads drive current definitions for BA */
typedef enum e_bsp_qspic_pads_drive_current_ba
{
    BSP_QSPIC_PADS_DRIVE_CURRENT_BA_2MA  = 0, ///< Sets pad drive current 2 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_BA_4MA  = 1, ///< Sets pad drive current 4 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_BA_8MA  = 2, ///< Sets pad drive current 8 mA
    BSP_QSPIC_PADS_DRIVE_CURRENT_BA_14MA = 3, ///< Sets pad drive current 14 mA
} bsp_qspic_pads_drive_current_ba_t;

/** Options to configure pin functions */
typedef enum e_ioport_cfg_options
{
    IOPORT_CFG_PERIPHERAL_PIN        = 0x00000000, ///< Enables pin to operate as a peripheral pin
    IOPORT_CFG_PORT_DIRECTION_INPUT  = 0x00000000, ///< Sets the pin direction to input
    IOPORT_CFG_PULLUP_ENABLE         = 0x00000100, ///< Enables pull up
    IOPORT_CFG_PULLDOWN_ENABLE       = 0x00000200, ///< Enables pull down
    IOPORT_CFG_PORT_DIRECTION_OUTPUT = 0x00000300, ///< Sets the pin direction to output
    IOPORT_CFG_OPEN_DRAIN_ENABLE     = 0x00000700, ///< Enables open-drain output
    IOPORT_CFG_ANALOG_ENABLE         = 0x00008000, ///< Enables pin to operate as an analog pin
    IOPORT_CFG_SCHMITT_TRIG_ENABLE   = 0x00000800, ///< Enables Schmitt trigger for input pin
    IOPORT_CFG_DRIVE_STRENGTH_2MA    = 0x00000000, ///< Sets pad drive strength 2 mA
    IOPORT_CFG_DRIVE_STRENGTH_12MA   = 0x00001000, ///< Sets pad drive strength 12 mA
    IOPORT_CFG_DRIVE_STRENGTH_4MA    = 0x00002000, ///< Sets pad drive strength 4 mA
    IOPORT_CFG_DRIVE_STRENGTH_18MA   = 0x00003000, ///< Sets pad drive strength 18 mA
    IOPORT_CFG_SLEW_RATE_FAST        = 0x00000000, ///< Sets fast slew rate for pin
    IOPORT_CFG_SLEW_RATE_SLOW        = 0x00004000, ///< Sets slow slew rate for pin

    /* GPIO CFG */
    IOPORT_CFG_RETENTION        = 0x20000000,      ///< Retention Setting
    IOPORT_CFG_IRQ_ENABLE       = 0x40000000,      ///< Enables IRQ
    IOPORT_CFG_PORT_OUTPUT_LOW  = 0x00000000,      ///< Sets the pin level to low
    IOPORT_CFG_PORT_OUTPUT_HIGH = 0x80000000,      ///< Sets the pin level to high
} ioport_cfg_options_t;

typedef enum e_ioport_cfg_options_ba
{
    IOPORT_CFG_DRIVE_STRENGTH_BA_2MA  = 0x00000000, ///< Sets pad drive strength 2 mA for BA
    IOPORT_CFG_DRIVE_STRENGTH_BA_4MA  = 0x00001000, ///< Sets pad drive strength 4 mA for BA
    IOPORT_CFG_DRIVE_STRENGTH_BA_8MA  = 0x00002000, ///< Sets pad drive strength 8 mA for BA
    IOPORT_CFG_DRIVE_STRENGTH_BA_14MA = 0x00003000, ///< Sets pad drive strength 14 mA for BA
} ioport_cfg_options_ba_t;

/** Clocks that can be mapped to dedicated BSP_IO */
typedef enum e_bsp_io_clk_output_ra6w1
{
    BSP_IO_CLK_XTAL40M_OUT  = (GPIO_GPIO_CLK_SEL_REG_XTAL40M_OUTPUT_EN_Msk >> 4),  /**< Map clock XTAL40M to dedicated GPIO */
    BSP_IO_CLK_RC10M_OUT    = (GPIO_GPIO_CLK_SEL_REG_RC10M_OUTPUT_EN_Msk >> 4),    /**< Map clock RC10M to dedicated GPIO */
    BSP_IO_CLK_XTAL32K_OUT  = (GPIO_GPIO_CLK_SEL_REG_XTAL32K_OUTPUT_EN_Msk >> 4),  /**< Map clock XTAL32K to dedicated GPIO */
    BSP_IO_CLK_OSC32K_OUT   = (GPIO_GPIO_CLK_SEL_REG_OSC32K_OUTPUT_EN_Msk >> 4),   /**< Map clock OSC32K to dedicated GPIO */
    BSP_IO_CLK_FPLL98M_OUT  = (GPIO_GPIO_CLK_SEL_REG_FPLL98M_OUTPUT_EN_Msk >> 4),  /**< Map clock FPLL98M to dedicated GPIO */
    BSP_IO_CLK_DPLL480M_OUT = (GPIO_GPIO_CLK_SEL_REG_DPLL480M_OUTPUT_EN_Msk >> 4), /**< Map clock FPLL98M to dedicated GPIO */
    BSP_IO_CLK_MCLK_OUT     = (GPIO_GPIO_CLK_SEL_REG_MCLK_OUTPUT_EN_Msk >> 4),     /**< Map clock MCLK to dedicated GPIO */
    BSP_IO_CLK_NONE_OUT     = 0,
} bsp_io_clk_output_t;

/** Clocks that can be mapped to GPIO function FUNC_CLOCK */
typedef enum e_bsp_io_clk_func_ra6w1
{
    BSP_IO_CLK_XTAL40M_FUNC  = 1,      /**< Map clock XTAL40M to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_RC10M_FUNC    = 2,      /**< Map clock RC10M to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_XTAL32K_FUNC  = 3,      /**< Map clock XTAL32K to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_OSC32K_FUNC   = 4,      /**< Map clock OSC32K to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_FPLL98M_FUNC  = 5,      /**< Map clock FPLL98M to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_DPLL480M_FUNC = 6,      /**< Map clock DPLL480M to GPIO function FUNC_CLOCK */
    BSP_IO_CLK_DIVN_FUNC     = 7,      /**< Map clock DIVn to GPIO function FUNC_CLOCK */
} bsp_io_clk_func_t;

/** @} (end addtogroup BSP_IO) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
