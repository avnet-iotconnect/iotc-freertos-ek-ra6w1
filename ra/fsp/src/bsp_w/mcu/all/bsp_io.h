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

#ifndef BSP_IO_H
#define BSP_IO_H

/***********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************/
/* Gets MCU configuration information. */
#include "bsp_api.h"

#if BSP_MCU_GROUP_RRQ431
 #include "../rrq431/bsp_io.h"
#endif
#if BSP_MCU_GROUP_RA6W1
 #include "../ra6w1/bsp_io.h"
#endif

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_IO_PORT_BITS      (0xFF00U)
#define BSP_IO_PIN_BITS       (0x00FFU)

/* Shift to get port in bsp_io_port_t and bsp_io_port_pin_t enums. */
#define BSP_IO_PORT_OFFSET    (8U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Levels that can be set and read for individual pins */
typedef enum e_bsp_io_level
{
    BSP_IO_LEVEL_LOW = 0,              ///< Low
    BSP_IO_LEVEL_HIGH                  ///< High
} bsp_io_level_t;

/** Direction of individual pins */
typedef enum e_bsp_io_dir
{
    BSP_IO_DIRECTION_INPUT  = IOPORT_CFG_PORT_DIRECTION_INPUT,  ///< Input
    BSP_IO_DIRECTION_OUTPUT = IOPORT_CFG_PORT_DIRECTION_OUTPUT, ///< Output
} bsp_io_direction_t;

/** Superset list of all possible IO ports. */
typedef enum e_bsp_io_port
{
    BSP_IO_PORT_00 = 0x00,             ///< IO port 0
    BSP_IO_PORT_01 = 0x01,             ///< IO port 1
    BSP_IO_PORT_02 = 0x02,             ///< IO port 2

    /** Marks end of enum */
    BSP_IO_PORT_MAX,
} bsp_io_port_t;

/** Superset list of all possible IO port pins. */
typedef enum e_bsp_io_port_pin_t
{
    BSP_IO_PORT_00_PIN_00 = 0x0000,    ///< IO port 0 pin 0
    BSP_IO_PORT_00_PIN_01 = 0x0001,    ///< IO port 0 pin 1
    BSP_IO_PORT_00_PIN_02 = 0x0002,    ///< IO port 0 pin 2
    BSP_IO_PORT_00_PIN_03 = 0x0003,    ///< IO port 0 pin 3
    BSP_IO_PORT_00_PIN_04 = 0x0004,    ///< IO port 0 pin 4
    BSP_IO_PORT_00_PIN_05 = 0x0005,    ///< IO port 0 pin 5
    BSP_IO_PORT_00_PIN_06 = 0x0006,    ///< IO port 0 pin 6
    BSP_IO_PORT_00_PIN_07 = 0x0007,    ///< IO port 0 pin 7
    BSP_IO_PORT_00_PIN_08 = 0x0008,    ///< IO port 0 pin 8
    BSP_IO_PORT_00_PIN_09 = 0x0009,    ///< IO port 0 pin 9
    BSP_IO_PORT_00_PIN_10 = 0x000A,    ///< IO port 0 pin 10
    BSP_IO_PORT_00_PIN_11 = 0x000B,    ///< IO port 0 pin 11
    BSP_IO_PORT_00_PIN_12 = 0x000C,    ///< IO port 0 pin 12
    BSP_IO_PORT_00_PIN_13 = 0x000D,    ///< IO port 0 pin 13
    BSP_IO_PORT_00_PIN_14 = 0x000E,    ///< IO port 0 pin 14
    BSP_IO_PORT_00_PIN_15 = 0x000F,    ///< IO port 0 pin 15
    BSP_IO_PORT_00_PIN_16 = 0x0010,    ///< IO port 0 pin 16
    BSP_IO_PORT_00_PIN_17 = 0x0011,    ///< IO port 0 pin 17
    BSP_IO_PORT_00_PIN_18 = 0x0012,    ///< IO port 0 pin 18
    BSP_IO_PORT_00_PIN_19 = 0x0013,    ///< IO port 0 pin 19
    BSP_IO_PORT_00_PIN_20 = 0x0014,    ///< IO port 0 pin 20
    BSP_IO_PORT_00_PIN_21 = 0x0015,    ///< IO port 0 pin 21
    BSP_IO_PORT_00_PIN_22 = 0x0016,    ///< IO port 0 pin 22
    BSP_IO_PORT_00_PIN_23 = 0x0017,    ///< IO port 0 pin 23
    BSP_IO_PORT_00_PIN_24 = 0x0018,    ///< IO port 0 pin 24
    BSP_IO_PORT_00_PIN_25 = 0x0019,    ///< IO port 0 pin 25
    BSP_IO_PORT_00_PIN_26 = 0x001A,    ///< IO port 0 pin 26
    BSP_IO_PORT_00_PIN_27 = 0x001B,    ///< IO port 0 pin 27
    BSP_IO_PORT_00_PIN_28 = 0x001C,    ///< IO port 0 pin 28
    BSP_IO_PORT_00_PIN_29 = 0x001D,    ///< IO port 0 pin 29
    BSP_IO_PORT_00_PIN_30 = 0x001E,    ///< IO port 0 pin 30
    BSP_IO_PORT_00_PIN_31 = 0x001F,    ///< IO port 0 pin 31

    BSP_IO_PORT_01_PIN_00 = 0x0100,    ///< IO port 1 pin 0
    BSP_IO_PORT_01_PIN_01 = 0x0101,    ///< IO port 1 pin 1
    BSP_IO_PORT_01_PIN_02 = 0x0102,    ///< IO port 1 pin 2
    BSP_IO_PORT_01_PIN_03 = 0x0103,    ///< IO port 1 pin 3
    BSP_IO_PORT_01_PIN_04 = 0x0104,    ///< IO port 1 pin 4
    BSP_IO_PORT_01_PIN_05 = 0x0105,    ///< IO port 1 pin 5
    BSP_IO_PORT_01_PIN_06 = 0x0106,    ///< IO port 1 pin 6
    BSP_IO_PORT_01_PIN_07 = 0x0107,    ///< IO port 1 pin 7
    BSP_IO_PORT_01_PIN_08 = 0x0108,    ///< IO port 1 pin 8
    BSP_IO_PORT_01_PIN_09 = 0x0109,    ///< IO port 1 pin 9
    BSP_IO_PORT_01_PIN_10 = 0x010A,    ///< IO port 1 pin 10
    BSP_IO_PORT_01_PIN_11 = 0x010B,    ///< IO port 1 pin 11
    BSP_IO_PORT_01_PIN_12 = 0x010C,    ///< IO port 1 pin 12
    BSP_IO_PORT_01_PIN_13 = 0x010D,    ///< IO port 1 pin 13
    BSP_IO_PORT_01_PIN_14 = 0x010E,    ///< IO port 1 pin 14
    BSP_IO_PORT_01_PIN_15 = 0x010F,    ///< IO port 1 pin 15
    BSP_IO_PORT_01_PIN_16 = 0x0110,    ///< IO port 1 pin 16
    BSP_IO_PORT_01_PIN_17 = 0x0111,    ///< IO port 1 pin 17

    BSP_IO_PORT_02_PIN_00 = 0x0200,    ///< IO port 2 pin 0
    BSP_IO_PORT_02_PIN_01 = 0x0201,    ///< IO port 2 pin 1
    BSP_IO_PORT_02_PIN_02 = 0x0202,    ///< IO port 2 pin 2
    BSP_IO_PORT_02_PIN_03 = 0x0203,    ///< IO port 2 pin 3
    BSP_IO_PORT_02_PIN_04 = 0x0204,    ///< IO port 2 pin 4
    BSP_IO_PORT_02_PIN_05 = 0x0205,    ///< IO port 2 pin 5
    BSP_IO_PORT_02_PIN_06 = 0x0206,    ///< IO port 2 pin 6
    BSP_IO_PORT_02_PIN_07 = 0x0207,    ///< IO port 2 pin 7

    BSP_IO_PORT_FF_PIN_FF = 0xFFFF,    ///< Invalid IO port
} bsp_io_port_pin_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Read the current input level of the pin.
 *
 * @param[in]  pin             The pin
 *
 * @retval     Current input level
 **********************************************************************************************************************/
__STATIC_INLINE uint32_t R_BSP_PinRead (bsp_io_port_pin_t pin)
{
    uint32_t         pin_id = (uint8_t) pin;
    uint32_t         port   = ((uint16_t) pin) >> 8;
    __IOM uint32_t * reg    = &(GPIO->P0_DATA_REG) + port;

    return ((*reg) >> pin_id) & 1U;
}

/*******************************************************************************************************************//**
 * Set pin output level to the level provided. Call R_BSP_PinCfg() to set pin to output.
 *
 * @param[in]  pin      The pin
 * @param[in]  level    The level
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PinWrite (bsp_io_port_pin_t pin, bsp_io_level_t level)
{
    uint32_t         pin_id = (uint8_t) pin;
    uint32_t         port   = ((uint16_t) pin) >> BSP_IO_PORT_OFFSET;
    __IOM uint32_t * reg;

    if (BSP_IO_LEVEL_LOW == level)
    {
        reg = &(GPIO->P0_RESET_DATA_REG) + port;
    }
    else
    {
        reg = &(GPIO->P0_SET_DATA_REG) + port;
    }

    *reg = 1 << pin_id;
}

/*******************************************************************************************************************//**
 * Configure a pin.
 *
 * @param[in]  pin      The pin
 * @param[in]  cfg      Configuration for the pin
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PinCfg (bsp_io_port_pin_t pin, uint32_t cfg)
{
    uint32_t pin_id = (uint8_t) pin;
    uint32_t port   = ((uint16_t) pin) >> BSP_IO_PORT_OFFSET;

    BSP_IO_PXX_MODE_REG(port, pin_id) = cfg;
}

/*******************************************************************************************************************//**
 * Enable access to the PFS registers. Uses a reference counter to protect against interrupts that could occur
 * via multiple threads or an ISR re-entering this code.
 *
 * @note: The DA HW does not provide such protection, so this function is a NOP.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PinAccessEnable (void)
{
#if BSP_CFG_PFS_PROTECT
    /* Nothing to do. */
#endif
}

/*******************************************************************************************************************//**
 * Disable access to the PFS registers. Uses a reference counter to protect against interrupts that could occur via
 * multiple threads or an ISR re-entering this code.
 *
 * @note: The DA HW does not provide such protection, so this function is a NOP.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_PinAccessDisable (void)
{
#if BSP_CFG_PFS_PROTECT
    /* Nothing to do. */
#endif
}

#if BSP_MCU_GROUP_RA6W1
 #include "../ra6w1/bsp_io_isolation.h"
#endif

/**
 * \brief  Trigger a GPIO when an assertion hits.
 *
 */
BSP_PLACE_IN_SECTION(".code_in_ram") void bsp_io_on_assert_trigger(void);

/** @} (end addtogroup BSP_IO) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
