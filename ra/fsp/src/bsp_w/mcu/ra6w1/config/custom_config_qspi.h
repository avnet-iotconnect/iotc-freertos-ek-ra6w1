/**
 ****************************************************************************************
 *
 * @file custom_config_qspi.h
 *
 * @brief Board Support Package. User Configuration file for cached QSPI mode.
 *
 * Copyright (c) 2023-2024 Renesas Electronics. All rights reserved.
 *
 * This software ("Software") is owned by Renesas Electronics.
 *
 * By using this Software you agree that Renesas Electronics retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Renesas Electronics is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Renesas Electronics products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * RENESAS ELECTRONICS BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */


#ifndef CUSTOM_CONFIG_QSPI_H_
#define CUSTOM_CONFIG_QSPI_H_

#ifndef __ASSEMBLER__
#include "bsp_api.h"
#endif
#include "bsp_definitions.h"

#if !DEVICE_FPGA
#define CONFIG_PLL
#endif

#if   defined(CONFIG_PLL)
#define dg_configXTAL_BASED_CPU40M                 ( 0 )
#define dg_configUSE_PM_STATISTICS                 ( 1 )
#else
#define dg_configXTAL_BASED_CPU40M                 ( 0 )
#define dg_configUSE_PM_STATISTICS                 ( 0 )
#endif

#define dg_configEXEC_MODE                         MODE_IS_CACHED
#define dg_configCODE_LOCATION                     NON_VOLATILE_IS_FLASH

#define dg_configUSE_ROMCRYPTO                     ( 1 )
#define dg_configUSE_ROMMAC                        ( 1 )
#define dg_configUSE_IPDREGION                     ( 1 )

#define dg_configUSE_HW_TIMER                      ( 1 )

#define dg_configFLASH_CONNECTED_TO                ( FLASH_CONNECTED_TO_1V8 )
#define dg_configFLASH_POWER_DOWN                  ( 0 )

#define dg_configUSE_USB                           ( 0 )
#define dg_configUSE_USB_CHARGER                   ( 0 )
#define dg_configALLOW_CHARGING_NOT_ENUM           ( 1 )

#define dg_configUSE_SW_CURSOR                     ( 1 )

#define dg_configSYSTEMVIEW                        ( 0 )

/*************************************************************************************************\
 * FreeRTOS specific config
 */
#ifndef OS_FREERTOS
#define OS_FREERTOS                                 /* Define this to use FreeRTOS */
#endif

/* FreeRTOS Total Heap Size */
#ifndef configTOTAL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE                      ( 300*1024 )
#endif

#ifndef configTOTAL_HEAP_SIZE_DYNAMIC_ALLOC
#define configTOTAL_HEAP_SIZE_DYNAMIC_ALLOC        ( 1 ) /* 0: Static, 1: Dynamic */
#endif

#if (dg_configSYSTEMVIEW == 1)
#define dg_configSYSTEMVIEW_MONITOR_QUEUE          ( 0 )
#define dg_configSYSTEMVIEW_MONITOR_MALLOC         ( 0 )
#define dg_configSYSTEMVIEW_MONITOR_EVENT_GROUP    ( 0 )
#define dg_configSYSTEMVIEW_STUCK_UNTIL_ATTACH     ( 0 )
#endif

/*************************************************************************************************\
 * Peripheral specific config
 */

/* i2c config */
#define dg_configUSE_HW_I2C                        ( 1 )
#define dg_configI2C_ADAPTER                       ( 1 )
#define dg_configI2C_ADAPTER_SLAVE_SUPPORT         ( 1 )

/* spi config */
#define dg_configUSE_HW_SPI                        ( 1 )

#if !DEVICE_FPGA
/* GPADC config */
#define dg_configUSE_HW_GPADC                      ( 1 )
#endif

#define dg_configRF_ENABLE_RECALIBRATION           ( 0 )

#define dg_configFLASH_ADAPTER                     ( 1 )
#define dg_configNVMS_ADAPTER                      ( 1 )
#define dg_configNVMS_VES                          ( 1 )
#define DIRECT_DRIVER_STRATEGY                     (DIRECT_DRIVER_DYNAMIC_SECTOR_BUF)

#define dg_configNVPARAM_ADAPTER                   ( 1 ) // 1: NVRAM, 0: NVRAM Emulator
#if dg_configNVPARAM_ADAPTER
    #define dg_configNVPARAM_ADAPTERv2             ( 1 )
#endif /* dg_configNVPARAM_ADAPTER */

#define dg_configNVPARAM_APP_AREA                  ( 2 )
#define dg_configADNVPARAM_PROJ_FILE               "ra6w1_platform_nvparam.h"
#define dg_configDISABLE_BACKGROUND_FLASH_OPS      ( 1 )

#define dg_configUSE_DCDC                          ( 0 )

#define dg_configUSE_HW_OTPC                       ( 1 )

#define dg_configUSE_NVIC                          ( 0 ) // Using NVIC(Nested Vectored Interrupt Controller)
#define dg_configUSE_TRACE_FOR_DEBUG               ( 0 ) // trace function information

#define dg_configUSE_SLEEP_MGMT_FUNCTION           ( 1 ) // Sleep Management function (with DPM)
#define dg_configUSE_RETENTION_MEM_INFO            ( 1 ) // Print retention memory information

#define dg_configUSE_IDLE_TASK_TRIGGER             ( 0 ) // Activating sleep_dameon by IDLE task
#if (dg_configUSE_IDLE_TASK_TRIGGER == 1)
#define dg_configUSE_TIMER_TRIGGER                 ( 0 )
#else
#define dg_configUSE_TIMER_TRIGGER                 ( 1 ) // Activating sleep_dameon by timer
#endif

#define dg_configDPM_TIMER_ADAPTER                 ( 1 )

#define dg_configSYS_TCS                           ( 1 )
#define dg_configUSE_SYS_TCS                       ( 0 )
#define dg_configBOOT_SYS_TCS                      ( 0 )

/*************************************************************************************************\
 * PTIM specific config
 */
/// Enable DPM MODE to boot PTIM image when in sleep mode 3
#define dg_configPTIM                              ( 1 )

#if dg_configPTIM
#define dg_configPTIMG_LOAD                        ( 1 )
#define dg_configPTIM_UAGC_CODE                    ( 0 )
#define dg_configPTIM_RXIPD                        ( 0 )
#endif

/* Include SFLASH map table */
#include "bsp_sflash_map_ra6w1.h"

/* Include bsp default values */
#include "bsp_defaults.h"

/* Include middleware default values */
#include "middleware_defaults.h"

#endif /* CUSTOM_CONFIG_QSPI_H_ */

/* EOF */
