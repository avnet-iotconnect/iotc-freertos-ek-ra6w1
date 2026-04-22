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

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* TODO remove the following code when we finally drop AA chip revision: */
#ifndef DEVICE_REV_A
 #define DEVICE_REV_A    ((((1 << 0) << 8) & 0x00000F00))
#endif
#ifndef DEVICE_REV_B
 #define DEVICE_REV_B    ((((1 << 1) << 8) & 0x00000F00))
#endif
#ifndef BSP_DEVICE_REVISION
 #define BSP_DEVICE_REVISION DEVICE_REV_B
#endif

#define BSP_FEATURE_BSP_HAS_ICU                            (0U)
#define BSP_FEATURE_ICU_IRQ_CHANNELS_MASK                  (0x3FFFFFFFU)
/*
 * \brief TCS - Trimmed Configuration Script
 *
 * Enabling this feature the system will always use the trimmed configuration values
 * stored by the TCS in OTP
 */
#define BSP_FEATURE_USE_TCS                                (1U)
#define BSP_FEATURE_TCS_SYS_RAM                            (1U)

#define BSP_FEATURE_DMAC_HAS_SHARED_IRQ                    (1U)
#define BSP_FEATURE_DMAC_MAX_CHANNEL                       (16U)

#define BSP_FEATURE_IIC_VERSION                            (2U)
#define BSP_FEATURE_IIC_B_VALID_CHANNEL_MASK               (3U)

#define BSP_FEATURE_ELC_MISSING                            (1U)

#define BSP_FEATURE_GPT_VALID_CHANNEL_MASK                 (0xFFU)
#define BSP_FEATURE_TIM_W_NUMBER_OF_CAPTURE_TRIGGERS       (0x4U)

#define BSP_FEATURE_IO_HAS_PAD_ISOLATION                   (0U) // MCU does not support pad isolation

#define BSP_FEATURE_SPI_MAX_CHANNEL                        (2U)

#define BSP_FEATURE_UART_W_CHANNELS                        (7U)
#define BSP_FEATURE_UART_W_FIFO_DEPTH                      (32U)

// Crypto Features
#define BSP_FEATURE_CRYPTO_HAS_AES                        (1)
#define BSP_FEATURE_CRYPTO_HAS_AES_WRAPPED                (0)
#define BSP_FEATURE_CRYPTO_HAS_CTR_DRBG                   (1)
#define BSP_FEATURE_CRYPTO_HAS_ECC                        (1)
#define BSP_FEATURE_CRYPTO_HAS_ECC_WRAPPED                (0)
#define BSP_FEATURE_CRYPTO_HAS_HASH                       (1)
#define BSP_FEATURE_CRYPTO_HAS_RSA                        (1)
#define BSP_FEATURE_CRYPTO_HAS_RSA_WRAPPED                (0)

//I2S Features
#define BSP_FEATURE_I2S_FIFO_NUM_STAGES                   (4U)
#define BSP_FEATURE_I2S_VALID_CHANNEL_MASK                (1U)

/* LPM_B */
#define BSP_FEATURE_LPM_CHANGE_MSTP_ARRAY                    (0)  // Feature not available on this MCU
#define BSP_FEATURE_LPM_CHANGE_MSTP_REQUIRED                 (0U)
#define BSP_FEATURE_LPM_DPSIEGR_MASK                         (0)  // Feature not available on this MCU
#define BSP_FEATURE_LPM_DPSIER_MASK                          (0)  // Feature not available on this MCU
#define BSP_FEATURE_LPM_HAS_DEEP_SLEEP                       (0U)
#define BSP_FEATURE_LPM_HAS_DEEP_STANDBY                     (0U)
#define BSP_FEATURE_LPM_HAS_DPSBYCR_DEEPCUT                  (0U)
#define BSP_FEATURE_LPM_HAS_DPSBYCR_DPSBY                    (0U)
#define BSP_FEATURE_LPM_HAS_DPSBYCR_SRKEEP                   (0U)
#define BSP_FEATURE_LPM_HAS_LDO_CONTROL                      (0U)
#define BSP_FEATURE_LPM_HAS_LPSCR                            (0U)
#define BSP_FEATURE_LPM_HAS_PDRAMSCR                         (0U)
#define BSP_FEATURE_LPM_HAS_SBYCR_OPE                        (0U)
#define BSP_FEATURE_LPM_HAS_SBYCR_SSBY                       (0U)
#define BSP_FEATURE_LPM_HAS_SNOOZE                           (0U)
#define BSP_FEATURE_LPM_HAS_SNZEDCR1                         (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_HAS_SNZREQCR1                        (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_HAS_STCONR                           (0) // Feature not available on this MCU
#define BSP_FEATURE_LPM_SBYCR_WRITE1_B14                     (0)
#define BSP_FEATURE_LPM_SNZEDCR_MASK                         (0x0000009FU)
#define BSP_FEATURE_LPM_SNZREQCR_MASK                        (0x738200FFU)
#define BSP_FEATURE_LPM_STANDBY_MOCO_REQUIRED                (0U)

#define BSP_FEATURE_BSP_HAS_RTC_COUNT                        (1)

/* OSPI_W
 * RA6W1 has only one channel for OSPI_W
 */
#define BSP_FEATURE_OSPI_W_DEVICE_0_START_ADDRESS            (0x2A000000U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

// Crypto Features from fsp-da/da/fsp/src/bsp_w/mcu/da1469x/bsp_feature.h
 #define BSP_FEATURE_CRYPTO_HAS_AES                        (1)
 #define BSP_FEATURE_CRYPTO_HAS_AES_WRAPPED                (0)
 #define BSP_FEATURE_CRYPTO_HAS_ECC                        (1)
 #define BSP_FEATURE_CRYPTO_HAS_ECC_WRAPPED                (0)
 #define BSP_FEATURE_CRYPTO_HAS_HASH                       (1)
 #define BSP_FEATURE_CRYPTO_HAS_RSA                        (1)
 #define BSP_FEATURE_CRYPTO_HAS_RSA_WRAPPED                (0)
 #define BSP_FEATURE_CRYPTO_HAS_CC312                      (1)

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#define BSP_FEATURE_OSPI_DEVICE_0_START_ADDRESS           (0x2A000000)
#define BSP_FEATURE_OSPI_DEVICE_1_START_ADDRESS           (0x0U)  /* not supported */


#define BSP_FEATURE_FLASH_DATA_FLASH_START                (BSP_FEATURE_OSPI_DEVICE_0_START_ADDRESS|0x00300000U)
#define BSP_FEATURE_FLASH_LP_DF_BLOCK_SIZE                (0)
#define BSP_FEATURE_FLASH_HP_DF_BLOCK_SIZE                (0)
#define BSP_FEATURE_BLOCK_MEDIA_SPI_W_BLOCK_SIZE            (4096U)
#define BSP_FEATURE_FLASH_HP_VERSION                      (0)
#define BSP_FEATURE_FLASH_LP_VERSION                      (0)

#endif
