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
 * @addtogroup GPIO_W
 * @{
 **********************************************************************************************************************/

#ifndef R_B_IOPORT_H
#define R_B_IOPORT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#include "bsp_io.h"
#include "r_ioport_api.h"
#include "r_gpio_w_cfg.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** GPIO_W private control block. DO NOT MODIFY. Initialization occurs when R_GPIO_W_Open() is called. */
typedef struct st_gpio_w_instance_ctrl
{
    uint32_t            open;
    void const        * p_context;
    volatile uint32_t * p_reg_p0_data;
    volatile uint32_t * p_reg_p0_set_data;
    volatile uint32_t * p_reg_p0_reset_data;
    volatile uint32_t * p_reg_p0_00_mode;
} gpio_w_instance_ctrl_t;

#ifndef BSP_OVERRIDE_IOPORT_PERIPHERAL_T

/** Superset of all peripheral functions.  */
typedef enum e_gpio_w_peripheral
{
    IOPORT_PERIPHERAL_GPIO             = 0,  /**< GPIO */
    IOPORT_PERIPHERAL_UART_RX          = 1,  /**< GPIO as UART RX */
    IOPORT_PERIPHERAL_UART_TX          = 2,  /**< GPIO as UART TX */
    IOPORT_PERIPHERAL_UART_CTSN        = 3,  /**< GPIO as UART CTSN */
    IOPORT_PERIPHERAL_UART_RTSN        = 4,  /**< GPIO as UART RTSN */
    IOPORT_PERIPHERAL_UART_TXDOE       = 5,  /**< GPIO as UART TXDOE */
    IOPORT_PERIPHERAL_UART1_RX         = 6,  /**< GPIO as UART1 RX */
    IOPORT_PERIPHERAL_UART1_TX         = 7,  /**< GPIO as UART1 TX */
    IOPORT_PERIPHERAL_UART1_CTSN       = 8,  /**< GPIO as UART1 CTSN */
    IOPORT_PERIPHERAL_UART1_RTSN       = 9,  /**< GPIO as UART1 RTSN */
    IOPORT_PERIPHERAL_UART1_TXDOE      = 10, /**< GPIO as UART1 TXDOE */
    IOPORT_PERIPHERAL_UART2_RX         = 11, /**< GPIO as UART2 RX */
    IOPORT_PERIPHERAL_UART2_TX         = 12, /**< GPIO as UART2 TX */
    IOPORT_PERIPHERAL_UART2_CTSN       = 13, /**< GPIO as UART2 CTSN */
    IOPORT_PERIPHERAL_UART2_RTSN       = 14, /**< GPIO as UART2 RTSN */
    IOPORT_PERIPHERAL_UART2_TXDOE      = 15, /**< GPIO as UART2 TXDOE */
    IOPORT_PERIPHERAL_SPI_DI           = 16, /**< GPIO as SPI DI */
    IOPORT_PERIPHERAL_SPI_DO           = 17, /**< GPIO as SPI DO */
    IOPORT_PERIPHERAL_SPI_CLK          = 18, /**< GPIO as SPI CLK */
    IOPORT_PERIPHERAL_SPI_CSN0         = 19, /**< GPIO as SPI CSN0 */
    IOPORT_PERIPHERAL_SPI_CSN1         = 20, /**< GPIO as SPI CSN1 */
    IOPORT_PERIPHERAL_SPI2_DI          = 21, /**< GPIO as SPI2 DI */
    IOPORT_PERIPHERAL_SPI2_DO          = 22, /**< GPIO as SPI2 DO */
    IOPORT_PERIPHERAL_SPI2_CLK         = 23, /**< GPIO as SPI2 CLK */
    IOPORT_PERIPHERAL_SPI2_CSN0        = 24, /**< GPIO as SPI2 CSN0 */
    IOPORT_PERIPHERAL_SPI2_CSN1        = 25, /**< GPIO as SPI2 CSN1 */
    IOPORT_PERIPHERAL_I2C_SCL          = 26, /**< GPIO as I2C SCL */
    IOPORT_PERIPHERAL_I2C_SDA          = 27, /**< GPIO as I2C SDA */
    IOPORT_PERIPHERAL_I2C2_SCL         = 28, /**< GPIO as I2C2 SCL */
    IOPORT_PERIPHERAL_I2C2_SDA         = 29, /**< GPIO as I2C2 SDA */
    IOPORT_PERIPHERAL_ADC              = 30, /**< GPIO as ADC (dedicated pin) */
    IOPORT_PERIPHERAL_PCM_DI           = 31, /**< GPIO as PCM DI */
    IOPORT_PERIPHERAL_PCM_DO           = 32, /**< GPIO as PCM DO */
    IOPORT_PERIPHERAL_PCM_FSC          = 33, /**< GPIO as PCM FSC */
    IOPORT_PERIPHERAL_PCM_CLK          = 34, /**< GPIO as PCM CLK */
    IOPORT_PERIPHERAL_DMICA_DI         = 35, /**< GPIO as DMICA DI */
    IOPORT_PERIPHERAL_DMIC_CLK         = 36, /**< GPIO as DMIC CLK */
    IOPORT_PERIPHERAL_MCLK             = 37, /**< GPIO as MCLK */
    IOPORT_PERIPHERAL_TIM_PWM          = 38, /**< GPIO as TIM PWM */
    IOPORT_PERIPHERAL_TIM2_PWM         = 39, /**< GPIO as TIM2 PWM */
    IOPORT_PERIPHERAL_TIM3_PWM         = 40, /**< GPIO as TIM3 PWM */
    IOPORT_PERIPHERAL_TIM4_PWM         = 41, /**< GPIO as TIM4 PWM */
    IOPORT_PERIPHERAL_TIM5_PWM         = 42, /**< GPIO as TIM5 PWM */
    IOPORT_PERIPHERAL_TIM6_PWM         = 43, /**< GPIO as TIM6 PWM */
    IOPORT_PERIPHERAL_TIM7_PWM         = 44, /**< GPIO as TIM7 PWM */
    IOPORT_PERIPHERAL_TIM8_PWM         = 45, /**< GPIO as TIM8 PWM */
    IOPORT_PERIPHERAL_TIM_1SHOT        = 46, /**< GPIO as TIM 1SHOT */
    IOPORT_PERIPHERAL_TIM2_1SHOT       = 47, /**< GPIO as TIM2 1SHOT */
    IOPORT_PERIPHERAL_TIM3_1SHOT       = 48, /**< GPIO as TIM3 1SHOT */
    IOPORT_PERIPHERAL_TIM4_1SHOT       = 49, /**< GPIO as TIM4 1SHOT */
    IOPORT_PERIPHERAL_TIM5_1SHOT       = 50, /**< GPIO as TIM5 1SHOT */
    IOPORT_PERIPHERAL_TIM6_1SHOT       = 51, /**< GPIO as TIM6 1SHOT */
    IOPORT_PERIPHERAL_TIM7_1SHOT       = 52, /**< GPIO as TIM7 1SHOT */
    IOPORT_PERIPHERAL_TIM8_1SHOT       = 53, /**< GPIO as TIM8 1SHOT */
    IOPORT_PERIPHERAL_CLOCK            = 54, /**< GPIO as CLOCK */
    IOPORT_PERIPHERAL_FEM_BS           = 55, /**< GPIO as FEM_BS */
    IOPORT_PERIPHERAL_FEM_CS           = 56, /**< GPIO as FEM_CS */
    IOPORT_PERIPHERAL_FEM_CTRL0        = 57, /**< GPIO as FEM CTRL0 */
    IOPORT_PERIPHERAL_FEM_CTRL1        = 58, /**< GPIO as FEM CTRL1 */
    IOPORT_PERIPHERAL_FEM_CTRL2        = 59, /**< GPIO as FEM CTRL2 */
    IOPORT_PERIPHERAL_BT_COEX_CBT      = 60, /**< GPIO as BT COEX CBT */
    IOPORT_PERIPHERAL_BT_WLAN_ACT      = 61, /**< GPIO as BT WLAN ACT */
    IOPORT_PERIPHERAL_BT_ACT           = 62, /**< GPIO as BT ACT */
    IOPORT_PERIPHERAL_BT_PRI           = 63, /**< GPIO as BT PRI */
    IOPORT_PERIPHERAL_RF_SW1           = 64, /**< GPIO as RF SW1 */
    IOPORT_PERIPHERAL_RF_SW2           = 65, /**< GPIO as RF SW2 */
    IOPORT_PERIPHERAL_EXT_INTR         = 66, /**< GPIO as EXT INTR */

    IOPORT_PERIPHERAL_SWCLK            = 99, /**< GPIO as SWCLK */
    IOPORT_PERIPHERAL_SWDIO            = 100, /**< GPIO as SWDIO */
    IOPORT_PERIPHERAL_WPROTECT         = 101, /**< GPIO as WPROTECT */
    IOPORT_PERIPHERAL_CDETECT          = 102, /**< GPIO as CDETECT */
    IOPORT_PERIPHERAL_ZB_WLAN_ACT      = 103, /**< GPIO as ZB WLAN ACT */
    IOPORT_PERIPHERAL_ZB_ACT           = 104, /**< GPIO as ZB ACT */
    IOPORT_PERIPHERAL_ZB_PRI           = 105, /**< GPIO as ZB PRI */
    IOPORT_PERIPHERAL_BTCOEX_ASC0      = 106, /**< GPIO as BTCOEX ASC0 */
    IOPORT_PERIPHERAL_BTCOEX_ASC1      = 107, /**< GPIO as BTCOEX ASC1 */
    IOPORT_PERIPHERAL_BTCOEX_ASC2      = 108, /**< GPIO as BTCOEX ASC2 */

} gpio_w_peripheral_t;

#endif

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ioport_api_t g_gpio_w_on_ioport;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/

fsp_err_t R_GPIO_W_Open(ioport_ctrl_t * const p_ctrl, const ioport_cfg_t * p_cfg);
fsp_err_t R_GPIO_W_Close(ioport_ctrl_t * const p_ctrl);
fsp_err_t R_GPIO_W_PinsCfg(ioport_ctrl_t * const p_ctrl, const ioport_cfg_t * p_cfg);
fsp_err_t R_GPIO_W_PinCfg(ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, uint32_t cfg);
fsp_err_t R_GPIO_W_PinEventInputRead(ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t * p_pin_event);
fsp_err_t R_GPIO_W_PinEventOutputWrite(ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t pin_value);
fsp_err_t R_GPIO_W_PinRead(ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t * p_pin_value);
fsp_err_t R_GPIO_W_PinWrite(ioport_ctrl_t * const p_ctrl, bsp_io_port_pin_t pin, bsp_io_level_t level);
fsp_err_t R_GPIO_W_PortDirectionSet(ioport_ctrl_t * const p_ctrl,
                                      bsp_io_port_t         port,
                                      ioport_size_t         direction_values,
                                      ioport_size_t         mask);
fsp_err_t R_GPIO_W_PortEventInputRead(ioport_ctrl_t * const p_ctrl, bsp_io_port_t port, ioport_size_t * event_data);
fsp_err_t R_GPIO_W_PortEventOutputWrite(ioport_ctrl_t * const p_ctrl,
                                          bsp_io_port_t         port,
                                          ioport_size_t         event_data,
                                          ioport_size_t         mask_value);
fsp_err_t R_GPIO_W_PortRead(ioport_ctrl_t * const p_ctrl, bsp_io_port_t port, ioport_size_t * p_port_value);
fsp_err_t R_GPIO_W_PortWrite(ioport_ctrl_t * const p_ctrl, bsp_io_port_t port, ioport_size_t value, ioport_size_t mask);

void     R_GPIO_W_WakeupSourcePinClear(void);
void     R_GPIO_W_WakeupSourcePinSet(bsp_io_wakeup_pin_t pin, bsp_io_wakeup_edge_t edge);
void     R_GPIO_W_WakeupSourcePinUnSet(bsp_io_wakeup_pin_t pin);

/*******************************************************************************************************************//**
 * @} (end defgroup GPIO_W)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // R_B_IOPORT_H
