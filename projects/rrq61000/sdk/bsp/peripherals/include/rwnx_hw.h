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
 * @addtogroup WIFI
 * @{
 **********************************************************************************************************************/

#ifndef RWNX_HW_H
#define RWNX_HW_H

#define RWNX_HW_IDLE_MAX_DELAY_US       800     // Max wait time for MAC idle

// Latency measurments for wakeup procedure
#define RWNX_HW_UNBLOCK_LATENCY_US      100     // Time from unblock to wakeup
#define RWNX_HW_PLATFORM_UP_LATENCY_US  580     // Time for platform on
#define RWNX_HW_MAC_RESTORE_LATENCY_US  220     // Time for MAC restore
#define RWNX_HW_PHY_REINIT_LATENCY_US   1440    // Time for PHY restore
#define RWNX_HW_WAKEUP_LATENCY_US      (RWNX_HW_UNBLOCK_LATENCY_US     + \
                                        RWNX_HW_PLATFORM_UP_LATENCY_US + \
                                        RWNX_HW_MAC_RESTORE_LATENCY_US + \
                                        RWNX_HW_PHY_REINIT_LATENCY_US)

// Minimum RWNX core sleep time
#define RWNX_HW_MIN_SLEEP_US    ((RWNX_HW_IDLE_MAX_DELAY_US) + \
                                 (RWNX_HW_WAKEUP_LATENCY_US))

enum rwnx_hw_sleep_type {
    RWNX_HW_SLEEP_TYPE_MAC_DOZE, // MAC doze, PHY active
    RWNX_HW_SLEEP_TYPE_PHY_OFF,  // MAC doze, PHY off
    RWNX_HW_SLEEP_TYPE_MAC_OFF,  // MAC off, PHY off
};

enum rwnx_hw_powerup_mode {
    RWNX_HW_POWERUP_MODE_POR,
    RWNX_HW_POWERUP_MODE_LOWCURRENT,
    RWNX_HW_POWERUP_MODE_RTOS_LOWCURRENT,
    RWNX_HW_POWERUP_MODE_SLEEP_MODE5,
};

enum rwnx_hw_powerdown_mode {
    RWNX_HW_POWERDOWN_MODE_ALL,   ///< Turn off MAC & PHY
    RWNX_HW_POWERDOWN_MODE_MACON, ///< Turn off PHY (Turn on MAC)
};

void rwnx_hw_power_up_step0(uint32_t powerup_mode);
void rwnx_hw_power_up_step1(uint32_t powerup_mode);
void rwnx_hw_power_up_step2(uint32_t powerup_mode);
void rwnx_hw_power_up_step3(uint32_t powerup_mode);
void rwnx_hw_power_up_step4(uint32_t powerup_mode);
void rwnx_hw_power_up(uint32_t mode);
void rwnx_hw_power_down(uint32_t mode);
void rwnx_hw_power_up_pllon(uint32_t powerup_mode, int32_t clk);
void rwnx_hw_power_up_plloff(void);
void rwnx_hw_sleep(uint8_t sleep_type);
void rwnx_hw_wakeup(uint8_t sleep_type);

/**
 ****************************************************************************************
 * @brief This function is called to configure PTA registers for Coex enable
 *
 ****************************************************************************************
 */
 void rwnx_hw_coex_init(void);

 /**
  ****************************************************************************************
  * @brief This function is called to configure PTA registers for Coex disable
  *
  ****************************************************************************************
  */
 void rwnx_hw_coex_deinit(void);
 
 /**
  ****************************************************************************************
  * @brief This function is called to configure Coex pins on set channel 2.4G
  *
  ****************************************************************************************
  */
 void rwnx_hw_coex_pin_config(void);
 
 /**
  ****************************************************************************************
  * @brief This function is called to configure Coex pins on set channel 5G
  *
  ****************************************************************************************
  */
 void rwnx_hw_coex_pin_config_5g(void);
 
#endif // RWNX_HW_H

/*******************************************************************************************************************//**
 * @} (end addtogroup WIFI)
 **********************************************************************************************************************/
