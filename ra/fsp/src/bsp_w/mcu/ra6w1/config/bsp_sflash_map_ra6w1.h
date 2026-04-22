/**
 ****************************************************************************************
 *
 * @file bsp_sflash_map_ra6w1.h
 *
 * @brief RA6W1_SFLASH Map
 *
 * Copyright (c) 2023 Renesas Electronics. All rights reserved.
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

#if !defined ( __BSP_SFLASH_MAP_RA6W1_H__ )
#define __BSP_SFLASH_MAP_RA6W1_H__


/*
 * USER configurable fields ...........................................................
 *  SFLASH type for RA6W1 SDK
 */
#define  RENESAS_AT25SL_8MB_OTA    // Default Renesas AT25SL 8MB w/ OTA
#undef   RENESAS_AT25SL_8MB        // Default Renesas AT25SL 8MB w/o OTA
#undef   NORMAL_4MB_OTA            // Normal 4MB SFLASH w/ OTA
#undef   NORMAL_4MB                // Normal 4MB SFLASH w/o OTA


//
// USER configurable fields ...........................................................
//   Need to config by USER
#define CERT_WPA_ENT_USED        ( 1 )        // Used = 1, Unused = 0
#define CERT_OTA_USED            ( 1 )        // Used = 1, Unused = 0
#define CERT_HTTPS_CLI_USED      ( 1 )        // Used = 1, Unused = 0
#define CERT_HTTPS_SVR_USED      ( 1 )        // Used = 1, Unused = 0
#define CERT_MQTTS_CLI_USED      ( 1 )        // Used = 1, Unused = 0
#define CERT_ATCMD_USED          ( 1 )        // Used = 1, Unused = 0
// ....................................................................................



////////////////////////// SFLASH_MAP //////////////////////////////////

#if defined ( RENESAS_AT25SL_8MB_OTA )    // ====================

/*
 * !!! Notice !!!
 *
 * Renese AT25SL Series 8MB SFLASH requires mutual exclusion operation
 *   in 1MB units for Write/Erase + Read operation.
 *
 */

/*
 * < RA6W1 Renesas AT25SL 8MB SFALSH Map - w/ OTA >
 *
 * ...............................................................................
 *         0x0000_0000 Product Header                      4 KB
 *         0x0000_1000 Product Header - Backup             4 KB
 *
 *         0x0000_2000 RTOS #0                         2,560 KB
 *         0x0028_2000 Reserved                          508 KB
 * .. 3MB ........................................................................
 *         0x0030_0000 NVRAM                              40 KB
 *         0x0030_A000 NVRAM - Backup (Multiplier 8)     280 KB
 *
 *         0x0035_0000 TLS_Cert_Base                     704 KB
 *         0x0035_2000 TLS Certificate WPA_Enterprise     16 KB or Unused
 *         0x0035_6000 TLS Certificate OTA_Update         16 KB or Unused
 *         0x0035_A000 TLS Certificate HTTPs Client       16 KB or Unused
 *         0x0035_E000 TLS Certificate HTTPs Server       16 KB or Unused
 *         0x0036_2000 TLS Certificate MQTTs Client       16 KB or Unused
 *         0x0036_6000 TLS Certificate CoAPs Client       16 KB or Unused
 *         0x0036_A000 TLS Certificate CoAPs Server       16 KB or Unused
 *         0x0036_E000 TLS Certificate AT-CMD #0 ~ #15    64 KB ( 4KB * 16 ) or Unused
 *         0x0038_F000 TLS Certificate Matter             16 KB or Unused
 *         0x003F_E000 Secure Asset Product Area           4 KB or Unused
 * .. 4MB ........................................................................
 *         0x0040_0000 RTOS #1                         2,560 KB
 *         0x0068_0000 Reserved                          512 KB
 * .. 7MB ........................................................................
 *         0x0070_0000 User Area #1                    1,020 KB
 *         0x007F_F000 Partition Table                     4 KB
 * .. 8MB ........................................................................
 *
 */

// Area size
#define SF_PRODUCT_HDR_SIZE      0x00001000         //     4 KB
#define SF_RTOS_SIZE             0x00280000         // 2,048 KB + 64 KB * 8

#define SF_NVRAM_SIZE            0x0000A000         //    40 KB
#define SF_NVRAM_AREA_SIZE       0x00050000         //   320 KB

#define SF_SYS_TLS_CERT_SIZE     0x00004000         //    16 KB
#define SF_ATCMD_TLS_CERT_SIZE   0x00010000         //    64 KB
#define SF_TLS_AREA_SIZE         0x000B0000         //   704 KB

#define SF_SECURE_ASSET_SIZE     0x00001000         //     4 KB
#define SF_USER_AREA_SIZE_ORI    0x000FF000         // 1,020 KB

#define SF_PARTITION_TBL_SIZE    0x00001000         //     4 KB

// Defined SFLASH address
#define SF_BASE_ADDR             0x00000000
#define SF_NVRAM_BASE_ADDR       0x00300000
#define SF_ASSET_PROD_BASE_ADDR  0x003FE000
#define SF_RTOS_1_BASE_ADDR      0x00400000
#if (TIN_SKU_BUILD_ID == TIN_SKU_WIFI6_B24_5_BLE || (defined (BSP_MCU_RRQ61051_208) || defined (BSP_MCU_RRQ61051_408)))
#define SF_USER_AREA_BASE_ADDR   0x00720000
#else
#define SF_USER_AREA_BASE_ADDR   0x00700000
#endif /* TIN_SKU_BUILD_ID == TIN_SKU_WIFI6_B24_5_BLE || (BSP_MCU_RRQ61051_208 || BSP_MCU_RRQ61051_408) */

#define SF_TLS_CERT_MATTER_DAC   0x0038F000

#define SF_PRODUCT_HDR           ( SF_BASE_ADDR )
#define SF_PRODUCT_HDR_BACKUP    ( SF_BASE_ADDR          + SF_PRODUCT_HDR_SIZE   )
#define SF_RTOS_0                ( SF_PRODUCT_HDR_BACKUP + SF_PRODUCT_HDR_SIZE   )

#define SF_NVRAM_AREA            ( SF_NVRAM_BASE_ADDR )
#define SF_NVRAM_BACKUP_AREA     ( SF_NVRAM_AREA         + SF_NVRAM_SIZE         )

#define SF_TLS_CERT_BASE_ADDR    ( SF_NVRAM_AREA         + SF_NVRAM_AREA_SIZE    )
#define SF_TLS_CERT_WPA_ENT      ( SF_TLS_CERT_BASE_ADDR )
#define SF_TLS_CERT_OTA          ( SF_TLS_CERT_WPA_ENT   + (SF_SYS_TLS_CERT_SIZE * CERT_WPA_ENT_USED)   )
#define SF_TLS_CERT_HTTPS_CLI    ( SF_TLS_CERT_OTA       + (SF_SYS_TLS_CERT_SIZE * CERT_OTA_USED)       )
#define SF_TLS_CERT_HTTPS_SVR    ( SF_TLS_CERT_HTTPS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_CLI_USED) )
#define SF_TLS_CERT_MQTTS_CLI    ( SF_TLS_CERT_HTTPS_SVR + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_SVR_USED) )
#define SF_TLS_CERT_ATCMD        ( SF_TLS_CERT_MQTTS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_MQTTS_CLI_USED) )

#define SF_SECURE_ASSET_PROD     ( SF_ASSET_PROD_BASE_ADDR )

#define SF_RTOS_1                ( SF_RTOS_1_BASE_ADDR )

#define SF_USER_AREA             ( SF_USER_AREA_BASE_ADDR )
#define SF_USER_AREA_SIZE        ( SF_USER_AREA_SIZE_ORI  )

#define SF_PARTITION_TBL           ( SF_USER_AREA     + SF_USER_AREA_SIZE   ) // Fixed - Mandatory
#define SF_END                     ( SF_PARTITION_TBL + SF_PARTITION_TBL_SIZE )


#elif defined ( RENESAS_AT25SL_8MB)  // ===========================================================

/*
 * !!! Notice !!!
 *
 * Renese AT25SL Series 8MB SFLASH requires mutual exclusion operation
 *   in 1MB units for Write/Erase + Read operation.
 *
 */

/*
 * < RA6W1 Renesas AT25SL 8MB SFALSH Map - w/o OTA >
 *
 * ...............................................................................
 *         0x0000_0000 Product Header                      4 KB
 *         0x0000_1000 Product Header - Backup             4 KB
 *
 *         0x0000_2000 RTOS #0                         2,560 KB
 *         0x0028_2000 Reserved                          508 KB
 * .. 3MB ........................................................................
 *         0x0030_0000 NVRAM                              40 KB
 *         0x0030_A000 NVRAM - Backup (Multiplier 8)     280 KB
 *         0x0035_0000 TLS_Cert_Base                     704 KB
 *         0x0035_2000 TLS Certificate WPA_Enterprise     16 KB or Unused
 *         0x0035_6000 TLS Certificate OTA_Update         16 KB or Unused
 *         0x0035_A000 TLS Certificate HTTPs Client       16 KB or Unused
 *         0x0035_E000 TLS Certificate HTTPs Server       16 KB or Unused
 *         0x0036_2000 TLS Certificate MQTTs Client       16 KB or Unused
 *         0x0036_6000 TLS Certificate CoAPs Client       16 KB or Unused
 *         0x0036_A000 TLS Certificate CoAPs Server       16 KB or Unused
 *         0x0036_E000 TLS Certificate AT-CMD #0 ~ #15    64 KB ( 4KB * 16 ) or Unused
  *        0x003F_E000 Secure Asset Product Area          128 B or Unused
 * .. 4MB ........................................................................
 *         0x0040_0000 User Area                       3,964 KB
 *         0x007F_F000 Partition Table                     4 KB
 * .. 8MB ........................................................................
 *
 */

// Area size
#define SF_PRODUCT_HDR_SIZE      0x00001000         //     4 KB
#define SF_RTOS_SIZE             0x00280000         // 2,048 KB + 64 KB * 8

#define SF_NVRAM_SIZE            0x0000A000         //    40 KB
#define SF_NVRAM_AREA_SIZE       0x00050000         //   320 KB

#define SF_SYS_TLS_CERT_SIZE     0x00004000         //    16 KB
#define SF_ATCMD_TLS_CERT_SIZE   0x00010000         //    64 KB
#define SF_TLS_AREA_SIZE         0x000B0000         //   704 KB

#define SF_SECURE_ASSET_SIZE     0x00001000         //     4 KB
#define SF_USER_AREA_SIZE_ORI    0x003FF000         // 4,092 KB

#define SF_PARTITION_TBL_SIZE    0x00001000         //     4 KB

// Defined SFLASH address
#define SF_BASE_ADDR             0x00000000
#define SF_NVRAM_BASE_ADDR       0x00300000
#define SF_ASSET_PROD_BASE_ADDR  0x003FE000
#define SF_USER_AREA_BASE_ADDR   0x00400000

#define SF_PRODUCT_HDR           ( SF_BASE_ADDR )
#define SF_PRODUCT_HDR_BACKUP    ( SF_BASE_ADDR          + SF_PRODUCT_HDR_SIZE   )
#define SF_RTOS_0                ( SF_PRODUCT_HDR_BACKUP + SF_PRODUCT_HDR_SIZE   )

#define SF_TLS_CERT_BASE_ADDR    ( SF_NVRAM_AREA         + SF_NVRAM_AREA_SIZE    )
#define SF_TLS_CERT_WPA_ENT      ( SF_TLS_CERT_BASE_ADDR )
#define SF_TLS_CERT_OTA          ( SF_TLS_CERT_WPA_ENT   + (SF_SYS_TLS_CERT_SIZE * CERT_WPA_ENT_USED)   )
#define SF_TLS_CERT_HTTPS_CLI    ( SF_TLS_CERT_OTA       + (SF_SYS_TLS_CERT_SIZE * CERT_OTA_USED)       )
#define SF_TLS_CERT_HTTPS_SVR    ( SF_TLS_CERT_HTTPS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_CLI_USED) )
#define SF_TLS_CERT_MQTTS_CLI    ( SF_TLS_CERT_HTTPS_SVR + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_SVR_USED) )
#define SF_TLS_CERT_ATCMD        ( SF_TLS_CERT_MQTTS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_MQTTS_CLI_USED) )

#define SF_NVRAM_AREA            ( SF_NVRAM_BASE_ADDR )
#define SF_NVRAM_BACKUP_AREA     ( SF_NVRAM_AREA         + SF_NVRAM_SIZE         )

#define SF_SECURE_ASSET_PROD     ( SF_ASSET_PROD_BASE_ADDR )

#define SF_USER_AREA             ( SF_USER_AREA_BASE_ADDR )
#define SF_USER_AREA_SIZE        ( SF_USER_AREA_SIZE_ORI  )

#define SF_PARTITION_TBL           ( SF_USER_AREA     + SF_USER_AREA_SIZE   ) // Fixed - Mandatory
#define SF_END                     ( SF_PARTITION_TBL + SF_PARTITION_TBL_SIZE )


#elif defined ( NORMAL_4MB_OTA ) // ===============================================================

/*
 * < RA6W1 Normal 4MB SFALSH Map - w/ OTA >
 *
 * ...............................................................................
 * 0x0000_0000 Product Header                          4 KB
 * 0x0000_1000 Product Header - Backup                 4 KB
 * 0x0000_2000 RTOS #0                             1,792 KB
 * 0x001C_2000 TLS_Cert_Base                         208 KB
 *   0x001C_2000 TLS Certificate WPA_Enterprise       16 KB or Unused
 *   0x001C_6000 TLS Certificate OTA_Update           16 KB or Unused
 *   0x001C_A000 TLS Certificate HTTPs Client         16 KB or Unused
 *   0x001C_E000 TLS Certificate HTTPs Server         16 KB or Unused
 *   0x001D_2000 TLS Certificate MQTTs Client         16 KB or Unused
 *   0x001D_6000 TLS Certificate CoAPs Client         16 KB or Unused
 *   0x001D_A000 TLS Certificate CoAPs Server         16 KB or Unused
 *   0x001D_E000 TLS Certificate AT-CMD #0 ~ #15      64 KB ( 4KB * 16 ) or Unused
 * 0x001F_6000 NVRAM                                  20 KB
 * 0x001F_B000 NVRAM - Backup                         20 KB
 * ...............................................................................
 * 0x0020_0000 RTOS #1                             1,792 KB
 * ...............................................................................
 * 0x003C_0000 User Area                             248 KB
 * 0x003F_E000 Secure Asset Product Area               4 KB
 * 0x003F_F000 Partition Table                         4 KB
 * ...............................................................................
 */

// Area size
#define SF_PRODUCT_HDR_SIZE      0x00001000         //     4 KB
#define SF_RTOS_SIZE             0x001C0000         // 1,792 KB
#define SF_SYS_TLS_CERT_SIZE     0x00004000         //    16 KB
#define SF_ATCMD_TLS_CERT_SIZE   0x00010000         //    64 KB
#define SF_TLS_AREA_SIZE         0x00034000         //   208 KB
#define SF_NVRAM_SIZE            0x00005000         //    20 KB
#define SF_SECURE_ASSET_SIZE     0x00001000         //     4 KB
#define SF_USER_AREA_SIZE_ORI    0x0003E000         //   248 KB
#define SF_PARTITION_TBL_SIZE    0x00001000         //     4 KB

// Defined SFLASH address
#define SF_BASE_ADDR             0x00000000
#define SF_ASSET_PROD_BASE_ADDR  0x003FE000

#define SF_PRODUCT_HDR           ( SF_BASE_ADDR )
#define SF_PRODUCT_HDR_BACKUP    ( SF_BASE_ADDR          + SF_PRODUCT_HDR_SIZE   )
#define SF_RTOS_0                ( SF_PRODUCT_HDR_BACKUP + SF_PRODUCT_HDR_SIZE   )

#define SF_TLS_CERT_BASE_ADDR    ( SF_RTOS_0             + SF_RTOS_SIZE          )
#define SF_TLS_CERT_WPA_ENT      ( SF_TLS_CERT_BASE_ADDR )
#define SF_TLS_CERT_OTA          ( SF_TLS_CERT_WPA_ENT   + (SF_SYS_TLS_CERT_SIZE * CERT_WPA_ENT_USED)   )
#define SF_TLS_CERT_HTTPS_CLI    ( SF_TLS_CERT_OTA       + (SF_SYS_TLS_CERT_SIZE * CERT_OTA_USED)       )
#define SF_TLS_CERT_HTTPS_SVR    ( SF_TLS_CERT_HTTPS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_CLI_USED) )
#define SF_TLS_CERT_MQTTS_CLI    ( SF_TLS_CERT_HTTPS_SVR + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_SVR_USED) )
#define SF_TLS_CERT_ATCMD        ( SF_TLS_CERT_MQTTS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_MQTTS_CLI_USED) )

#define SF_NVRAM_AREA            ( SF_TLS_CERT_BASE_ADDR + SF_TLS_AREA_SIZE      )
#define SF_NVRAM_BACKUP_AREA     ( SF_NVRAM_AREA         + SF_NVRAM_SIZE         )

#define SF_RTOS_1                ( SF_NVRAM_BACKUP_AREA  + SF_NVRAM_SIZE         )

#define SF_SECURE_ASSET_PROD     ( SF_ASSET_PROD_BASE_ADDR )

#define SF_USER_AREA_BASE        ( SF_RTOS_1             + SF_RTOS_SIZE          )

#define SF_USER_AREA             ( SF_USER_AREA_BASE )
#define SF_USER_AREA_SIZE        ( SF_USER_AREA_SIZE_ORI )

#define SF_PARTITION_TBL         ( SF_USER_AREA     + SF_USER_AREA_SIZE     )
#define SF_END                   ( SF_PARTITION_TBL + SF_PARTITION_TBL_SIZE )


#elif defined ( NORMAL_4MB )    // ================================================================

/*
 * < RA6W1 Normal 4MB SFALSH Map - w/o OTA >
 *
 * ...............................................................................
 * 0x0000_0000 Product Header                          4 KB
 * 0x0000_1000 Product Header - Backup                 4 KB
 * 0x0000_2000 RTOS #0                             1,792 KB
 * 0x001C_2000 TLS_Cert_Base                         208 KB
 *   0x001C_2000 TLS Certificate WPA_Enterprise       16 KB or Unused
 *   0x001C_6000 TLS Certificate OTA_Update           16 KB or Unused
 *   0x001C_A000 TLS Certificate HTTPs Client         16 KB or Unused
 *   0x001C_E000 TLS Certificate HTTPs Server         16 KB or Unused
 *   0x001D_2000 TLS Certificate MQTTs Client         16 KB or Unused
 *   0x001D_6000 TLS Certificate CoAPs Client         16 KB or Unused
 *   0x001D_A000 TLS Certificate CoAPs Server         16 KB or Unused
 *   0x001D_E000 TLS Certificate AT-CMD #0 ~ #15      64 KB ( 4KB * 16 ) or Unused
 * 0x001F_6000 NVRAM                                  20 KB
 * 0x001F_B000 NVRAM - Backup                         20 KB
 * ...............................................................................
 * 0x0020_0000 User Area                           2,040 KB 
 * 0x003F_E000 Secure Asset Product Area               4 KB
 * 0x003F_F000 Partition Table                         4 KB
 * ...............................................................................
 */

// Area size
#define SF_PRODUCT_HDR_SIZE      0x00001000         //     4 KB
#define SF_RTOS_SIZE             0x001C0000         // 1,792 KB
#define SF_SYS_TLS_CERT_SIZE     0x00004000         //    16 KB
#define SF_ATCMD_TLS_CERT_SIZE   0x00010000         //    64 KB
#define SF_TLS_AREA_SIZE         0x00034000         //   208 KB
#define SF_NVRAM_SIZE            0x00005000         //    20 KB
#define SF_SECURE_ASSET_SIZE     0x00001000         //     4 KB
#define SF_USER_AREA_SIZE_ORI    0x0023E000         // 2,044 KB
#define SF_PARTITION_TBL_SIZE    0x00001000         //     4 KB

// Defined SFLASH address
#define SF_BASE_ADDR             0x00000000
#define SF_ASSET_PROD_BASE_ADDR  0x003FE000

#define SF_PRODUCT_HDR           ( SF_BASE_ADDR )
#define SF_PRODUCT_HDR_BACKUP    ( SF_BASE_ADDR          + SF_PRODUCT_HDR_SIZE   )
#define SF_RTOS_0                ( SF_PRODUCT_HDR_BACKUP + SF_PRODUCT_HDR_SIZE   )

#define SF_TLS_CERT_BASE_ADDR    ( SF_RTOS_0             + SF_RTOS_SIZE          )
#define SF_TLS_CERT_WPA_ENT      ( SF_TLS_CERT_BASE_ADDR )
#define SF_TLS_CERT_OTA          ( SF_TLS_CERT_WPA_ENT   + (SF_SYS_TLS_CERT_SIZE * CERT_WPA_ENT_USED)   )
#define SF_TLS_CERT_HTTPS_CLI    ( SF_TLS_CERT_OTA       + (SF_SYS_TLS_CERT_SIZE * CERT_OTA_USED)       )
#define SF_TLS_CERT_HTTPS_SVR    ( SF_TLS_CERT_HTTPS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_CLI_USED) )
#define SF_TLS_CERT_MQTTS_CLI    ( SF_TLS_CERT_HTTPS_SVR + (SF_SYS_TLS_CERT_SIZE * CERT_HTTPS_SVR_USED) )
#define SF_TLS_CERT_ATCMD        ( SF_TLS_CERT_MQTTS_CLI + (SF_SYS_TLS_CERT_SIZE * CERT_MQTTS_CLI_USED) )

#define SF_NVRAM_AREA            ( SF_TLS_CERT_BASE_ADDR + SF_TLS_AREA_SIZE      )
#define SF_NVRAM_BACKUP_AREA     ( SF_NVRAM_AREA         + SF_NVRAM_SIZE         )

#define SF_SECURE_ASSET_PROD     ( SF_ASSET_PROD_BASE_ADDR )

#define SF_USER_AREA_BASE        ( SF_NVRAM_BACKUP_AREA  + SF_NVRAM_SIZE         )

#define SF_USER_AREA             ( SF_USER_AREA_BASE )
#define SF_USER_AREA_SIZE        ( SF_USER_AREA_SIZE_ORI )

#define SF_PARTITION_TBL         ( SF_USER_AREA      + SF_USER_AREA_SIZE   ) // Fixed - Mandatory
#define SF_END                   ( SF_PARTITION_TBL  + SF_PARTITION_TBL_SIZE )


#else // Undefind SFLASH type =====================================================================

#error "Not defined SFLASH type/map !!!"

#endif // __DEFAULT_4MB_SFLASH__ ==================================================================


/* TLS Certificate detailed addresses */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate WPA_ENT */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define SF_TLS_CERT_WPA_ENT_CA_ADDR              SF_TLS_CERT_WPA_ENT
#define SF_TLS_CERT_WPA_ENT_CERTIFICATE_ADDR     (SF_TLS_CERT_WPA_ENT_CA_ADDR + 0x1000)
#define SF_TLS_CERT_WPA_ENT_PRIVATE_KEY_ADDR     (SF_TLS_CERT_WPA_ENT_CA_ADDR + 0x2000)
#define SF_TLS_CERT_WPA_ENT_DH_PARAMETER_ADDR    (SF_TLS_CERT_WPA_ENT_CA_ADDR + 0x3000)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate OTA */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SF_TLS_CERT_OTA_CA_ADDR                  SF_TLS_CERT_OTA
#define SF_TLS_CERT_OTA_CERTIFICATE_ADDR         (SF_TLS_CERT_OTA_CA_ADDR + 0x1000)
#define SF_TLS_CERT_OTA_PRIVATE_KEY_ADDR         (SF_TLS_CERT_OTA_CA_ADDR + 0x2000)
#define SF_TLS_CERT_OTA_DH_PARAMETER_ADDR        (SF_TLS_CERT_OTA_CA_ADDR + 0x3000)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate Key HTTPS_CLI */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SF_TLS_CERT_HTTPS_CLI_CA_ADDR            SF_TLS_CERT_HTTPS_CLI
#define SF_TLS_CERT_HTTPS_CLI_CERTIFICATE_ADDR   (SF_TLS_CERT_HTTPS_CLI_CA_ADDR + 0x1000)
#define SF_TLS_CERT_HTTPS_CLI_PRIVATE_KEY_ADDR   (SF_TLS_CERT_HTTPS_CLI_CA_ADDR + 0x2000)
#define SF_TLS_CERT_HTTPS_CLI_DH_PARAMETER_ADDR  (SF_TLS_CERT_HTTPS_CLI_CA_ADDR + 0x3000)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate HTTPS_SVR */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SF_TLS_CERT_HTTPS_SVR_CA_ADDR            SF_TLS_CERT_HTTPS_SVR
#define SF_TLS_CERT_HTTPS_SVR_CERTIFICATE_ADDR   (SF_TLS_CERT_HTTPS_SVR_CA_ADDR + 0x1000)
#define SF_TLS_CERT_HTTPS_SVR_PRIVATE_KEY_ADDR   (SF_TLS_CERT_HTTPS_SVR_CA_ADDR + 0x2000)
#define SF_TLS_CERT_HTTPS_SVR_DH_PARAMETER_ADDR  (SF_TLS_CERT_HTTPS_SVR_CA_ADDR + 0x3000)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate Key MQTT_CLI */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SF_TLS_CERT_MQTT_CLI_CA_ADDR             SF_TLS_CERT_MQTTS_CLI
#define SF_TLS_CERT_MQTT_CLI_CERTIFICATE_ADDR    (SF_TLS_CERT_MQTT_CLI_CA_ADDR + 0x1000)
#define SF_TLS_CERT_MQTT_CLI_PRIVATE_KEY_ADDR    (SF_TLS_CERT_MQTT_CLI_CA_ADDR + 0x2000)
#define SF_TLS_CERT_MQTT_CLI_DH_PARAMETER_ADDR   (SF_TLS_CERT_MQTT_CLI_CA_ADDR + 0x3000)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* TLS Certificate ATCMD */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SF_TLS_CERT_ATCMD_CA_ADDR                SF_TLS_CERT_ATCMD
#define SFLASH_ATCMD_TLS_CERT_01                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x1000)
#define SFLASH_ATCMD_TLS_CERT_02                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x2000)
#define SFLASH_ATCMD_TLS_CERT_03                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x3000)
#define SFLASH_ATCMD_TLS_CERT_04                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x4000)
#define SFLASH_ATCMD_TLS_CERT_05                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x5000)
#define SFLASH_ATCMD_TLS_CERT_06                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x6000)
#define SFLASH_ATCMD_TLS_CERT_07                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x7000)
#define SFLASH_ATCMD_TLS_CERT_08                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x8000)
#define SFLASH_ATCMD_TLS_CERT_09                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x9000)
#define SFLASH_ATCMD_TLS_CERT_10                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xA000)
#define SFLASH_ATCMD_TLS_CERT_11                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xB000)
#define SFLASH_ATCMD_TLS_CERT_12                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xC000)
#define SFLASH_ATCMD_TLS_CERT_13                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xD000)
#define SFLASH_ATCMD_TLS_CERT_14                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xE000)
#define SFLASH_ATCMD_TLS_CERT_15                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0xF000)
#define SFLASH_ATCMD_TLS_CERT_16                 (SF_TLS_CERT_ATCMD_CA_ADDR + 0x10000)

#endif    /* __BSP_SFLASH_MAP_RA6W1_H__ */

/* EOF */
