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

#ifndef BSP_PD_RA6W1_H
#define BSP_PD_RA6W1_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**
 * Number of Power Domain IDs defined for this device
 */
#define BSP_PD_ID_COUNT    (3)

/* For retention memory bank*/
#define RETENTION_MEM_NONE        0x00
#define RETENTION_MEM_BANK0       0x01        // 0x00180000 ~ 0x00181fff
#define RETENTION_MEM_BANK1       0x02        // 0x00182000 ~ 0x00184000
#define RETENTION_MEM_BANK2       0x04        // 0x00184000 ~
#define RETENTION_MEM_BANK_ALL    0x07        // All

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Hardware power domains.*/
typedef enum e_bsp_power_domain
{
    BSP_PD_AON = 0,                    /**< Always on power domain */
    BSP_PD_PHY = 1,                    /**< PHY power domain*/
    BSP_PD_MAC = 2,                    /**< MAC power domain*/
} bsp_power_domain_t;

/* Wakeup edge for individual pins */
typedef enum e_bsp_wakeup_edge
{
    BSP_WAKEUP_EDGE_LOW = 0,         ///< Low
    BSP_WAKEUP_EDGE_HIGH             ///< High
} bsp_io_wakeup_edge_t;

/* Wakeup pins */
typedef enum e_bsp_wakeup_pin {
    BSP_WAKEUP_GPIO_P0_00               = (0x01 << 0),
    BSP_WAKEUP_GPIO_P0_08               = (0x01 << 1),
    BSP_WAKEUP_GPIO_P0_09               = (0x01 << 2),
    BSP_WAKEUP_GPIO_P0_10               = (0x01 << 3),
    BSP_WAKEUP_GPIO_P0_11               = (0x01 << 4),
    BSP_WAKEUP_GPIO_P0_12               = (0x01 << 5),
    BSP_WAKEUP_GPIO_P0_13               = (0x01 << 6),
    BSP_WAKEUP_GPIO_P1_10               = (0x01 << 7),
    BSP_WAKEUP_GPIO_P1_11               = (0x01 << 8),
    BSP_WAKEUP_GPIO_P1_12               = (0x01 << 9),
    BSP_WAKEUP_GPIO_P1_13               = (0x01 << 10),
} bsp_io_wakeup_pin_t;

// 32k * 2 + @
#define RETENTION_MEM_NONE                  0x00
#define RETENTION_MEM_BANK0                 0x01        // 0x00180000 ~ 0x00181fff
#define RETENTION_MEM_BANK1                 0x02        // 0x00182000 ~
#define RETENTION_MEM_BANK2                 0x04        // 0x00184000 ~
#define RETENTION_MEM_BANK_ALL              0x07        // all

typedef void (*BSP_RTC_BOD_CALLBACK_TYPE)(void);

typedef struct {
        void    (*func)(void *);
        void    *param;
}       BSP_RTC_TIME_EXP_CALLBACK_TYPE;
/* Wakeup edge for individual pins */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/* Used internaly by bsp_pd.c */
void bsp_prv_pd_masks_get(bsp_power_domain_t power_domain, uint32_t * sleep_mask, uint32_t * is_up_mask);

/**
 * @brief Enable a power domain by clearing its sleep status bit
 *
 * @param [in] sleep_mask   mask for the power domain's SLEEP status bit
 *
 * @note    Multiple power domains may be enabled if more than one bits are set in \p sleep_mask
 */
__STATIC_INLINE void bsp_prv_pd_enable (uint32_t sleep_mask)
{
    CRG_TOP->PMU_CTRL_REG &= ~sleep_mask;
}

/**
 * @brief Disable a power domain by setting its sleep status bit
 *
 * @param [in] sleep_mask   mask for the power domain's SLEEP status bit
 *
 * @note    Multiple power domains may be disabled if more than one bits are set in \p sleep_mask
 */
__STATIC_INLINE void bsp_prv_pd_disable (uint32_t sleep_mask)
{
    CRG_TOP->PMU_CTRL_REG |= sleep_mask;
}

/**
 * @brief Check if a power domain by checking its is-up status bit
 *
 * @param [in] is_up_mask   mask for the power domain's IS_UP status bit
 *
 * @return  True iff the power domains corresponding to the set bits in \p is_up_mask are up.
 *
 * @note    Multiple power domains may be checked if more than one bits are set in \p is_up_mask
 */
__STATIC_INLINE bool bsp_prv_pd_is_up_check (uint32_t is_up_mask)
{
    return (CRG_TOP->SYS_STATUS_REG & is_up_mask) == is_up_mask;
}

bsp_wakeup_source_mask_t R_BSP_WakeupSourceGet(void);
void     SystemWakeupSourceUpdate(void);
void     R_BSP_WakeupSourceClear(bool clear);

bsp_io_wakeup_pin_t bsp_prv_port_pin_to_wakeup_gpio(bsp_io_port_pin_t port_pin);
bsp_io_port_pin_t bsp_prv_wakeup_pin_to_port_pin(bsp_io_wakeup_pin_t wakeup_gpio);
bsp_io_wakeup_pin_t R_BSP_WakeupSourcePinGet(void);
void     R_BSP_WakeupSourcePinClear(void);
void     R_BSP_WakeupSourcePinSet(bsp_io_wakeup_pin_t pin, bsp_io_wakeup_edge_t edge);
void     R_BSP_WakeupSourcePinUnSet(bsp_io_wakeup_pin_t pin);

void     R_BSP_RetainedMemFlagSet(void);
void     R_BSP_RetainedMemFlagClear(void);
uint32_t R_BSP_RetainedMemFlagGet(void);

void     bsp_prv_pd_wakeup_counter_set(uint32_t sleep_id, uint64_t wakeup_time);
uint64_t bsp_prv_pd_wakeup_counter_get(void);
uint32_t bsp_prv_pd_sleep_id_get(void);

void bsp_rtc_enable_bod(BSP_RTC_BOD_CALLBACK_TYPE brown_callback, BSP_RTC_BOD_CALLBACK_TYPE black_callback);
void bsp_rtc_disable_bod(void);
int bsp_rtc_get_brown_out_status(void);

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif
