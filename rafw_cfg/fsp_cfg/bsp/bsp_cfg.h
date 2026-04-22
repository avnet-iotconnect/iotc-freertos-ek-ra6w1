/* generated configuration header file - do not edit */
#ifndef BSP_CFG_H_
#define BSP_CFG_H_
#ifdef __cplusplus
            extern "C" {
            #endif

#include "bsp_clock_cfg.h"
#include "bsp_mcu_family_cfg.h"
#include "board_cfg.h"
#define FSP_NOT_DEFINED 0
#ifndef BSP_CFG_RTOS
#if (FSP_NOT_DEFINED) != (1)
#define BSP_CFG_RTOS (2)
#elif (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)
              #define BSP_CFG_RTOS (1)
             #else
              #define BSP_CFG_RTOS (0)
             #endif
#endif
#ifndef BSP_CFG_RTC_USED
#define BSP_CFG_RTC_USED (1)
#endif
#undef FSP_NOT_DEFINED
#if defined(_RA_BOOT_IMAGE)
             #define BSP_CFG_BOOT_IMAGE (1)
            #endif
#define TIN_SKU_WIFI4_B24        (0x26)
#define TIN_SKU_WIFI6_B24        (0x22)
#define TIN_SKU_WIFI6_B24_5      (0x20)
#define TIN_SKU_WIFI6_B24_5_BLE  (0x28)
#if defined (BSP_MCU_R7SA6W1AEDZDD)   || defined (BSP_MCU_R7SA6W1AEDZNR) || defined (BSP_MCU_RRQ61051_208)
                #define TIN_SKU_BUILD_ID  TIN_SKU_WIFI4_B24
            #elif defined (BSP_MCU_R7SA6W1BEDZDD) || defined (BSP_MCU_R7SA6W1BEDZNR) || defined (BSP_MCU_RRQ61051_408)
                #define TIN_SKU_BUILD_ID  TIN_SKU_WIFI6_B24
            #elif defined (BSP_MCU_R7SA6W1CEDZDD) || defined (BSP_MCU_R7SA6W1CEDZNR) || defined (BSP_MCU_RRQ61001_008) || defined (BSP_MCU_RRQ61001_009) || defined (BSP_MCU_RRQ61001_010)
                #define TIN_SKU_BUILD_ID  TIN_SKU_WIFI6_B24_5
            #elif defined (BSP_MCU_R7JA6W2DEDZNR) || defined (BSP_MCU_RRQ61051_008)  || defined (BSP_MCU_RRQ61051_009) || defined (BSP_MCU_RRQ61051_010)
                #define TIN_SKU_BUILD_ID  TIN_SKU_WIFI6_B24_5_BLE
            #else
#define TIN_SKU_BUILD_ID  TIN_SKU_WIFI6_B24_5
#endif
#define BSP_CFG_STACK_MAIN_BYTES (0x1000)
#define BSP_CFG_HEAP_BYTES (0x2000)
#define BSP_CFG_PARAM_CHECKING_ENABLE (0)
#define BSP_CFG_ASSERT (0)
#define BSP_CFG_ERROR_LOG (0)

#define BSP_CFG_C_RUNTIME_INIT ((1))
#define BSP_CFG_EARLY_INIT     ((0))

#define BSP_CFG_STARTUP_CLOCK_REG_NOT_RESET ((0))

#ifndef BSP_CLOCK_CFG_MAIN_OSC_POPULATED
#define BSP_CLOCK_CFG_MAIN_OSC_POPULATED (1)
#endif

#ifndef BSP_CLOCK_CFG_MAIN_OSC_CLOCK_SOURCE
#define BSP_CLOCK_CFG_MAIN_OSC_CLOCK_SOURCE (0)
#endif
#ifndef BSP_CLOCK_CFG_XTAL32M_STABILIZATION_USEC
#define BSP_CLOCK_CFG_XTAL32M_STABILIZATION_USEC    0
#endif
#ifndef BSP_CLOCK_CFG_SUBCLOCK_POPULATED
#define BSP_CLOCK_CFG_SUBCLOCK_POPULATED (1)
#endif
#ifndef BSP_CLOCK_CFG_SUBCLOCK_STABILIZATION_MS
#define BSP_CLOCK_CFG_SUBCLOCK_STABILIZATION_MS 1000
#endif
#ifndef BSP_CFG_CMN_TIMING_DEBUG
/* TODO add a property for this */
#define BSP_CFG_CMN_TIMING_DEBUG                    (0U)
#endif

#define CFG_PMGR 1
#define CFG_PMGR_DPM_CLI 1

#define CFG_CLI (2)

#define CFG_MBEDTLS (1)

#define CFG_RTC_W (1)

#define SUPPORT_FSP_RM_OTA_W (FSP_NOT_DEFINED)

#define __PROGRAM_START
#if (1 == 1)
#define RM_MAP_PERSISTANT_W
#endif 

#if (1 == 1)
#define RM_STDIO_W
#endif

#ifdef __cplusplus
            }
            #endif
#endif /* BSP_CFG_H_ */
