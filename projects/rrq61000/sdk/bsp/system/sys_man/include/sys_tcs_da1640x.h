/**
 * \addtogroup MID_SYS_SERVICES
 * \{
 * \addtogroup SYS_TCS_HANDLER TCS Handler
 * \brief TCS Handler
 * \{
 */

/**
 ****************************************************************************************
 *
 * @file sys_tcs_da1487x.h
 *
 * @brief TCS Handler header file.
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

#if (DEVICE_FAMILY == DA1640X)

#ifndef SYS_TCS_DA1487X_H_
#define SYS_TCS_DA1487X_H_

#include "sdk_defs.h"

//XXX 0x400E0000 value used in booter code
// end of register not include APU, CC312
#define MAX_REG_ADDR            0x5100155C

#define TCS_ATTRIBUTE_SIZE      20
#define TCS_DATA_SIZE           200   /**< max number of entries in words (4 bytes). */
#define GID_EMPTY               0x1FF

#ifdef TEST_TCS_OTP
#define OTP_CS_ADDRESS          0x000180*4 //180 test address instead of user area
#define OTP_CS_END              0x0001F0*4
#else
#define OTP_CS_ADDRESS          0x000040*4 //0x100 real
#define OTP_CS_END              0x0000C0*4
#endif

#define CS_START_CMD            0xA5A5A5A5
#define CS_BOOTER_VAL           0xE6000000

#define CS_SDK_VAL              0xE9000000
#define CS_STX_CONFIG           0xE8000000// 2bytes
#define CS_HW_CFG_VAL           0xEC000000// 1byte
#define CS_MIN_FW_VAL           0xEB000000
#define CS_DELAYCMD             0xED000000//1byte
#define CS_CRC_SKIP             0xEE000000//1byte
#define CS_STOP_CMD             0x00000000
#define CS_EMPTY_VAL            0xFFFFFFFF

#define CS_SDK_VAL_GID_MASK     0x000000FF
#define CS_SDK_VAL_LEN_MASK     0x0000FF00
#define CS_SDK_VAL_SETID_MASK   0x00FF0000

/**
 * \enum SYS_TCS_TYPE
 * \brief custom TCS value type.
 *
 */
typedef enum {
        SYS_TCS_TYPE_TRIM_VAL = 0,      /**< trimmed value */
        SYS_TCS_TYPE_REG_PAIR = 1,      /**< register pair value */
        SYS_TCS_TYPE_HPI_REG_PAIR =2,
        SYS_TCS_TYPE_RESERVED = 3,
} SYS_TCS_TYPE;

/**
 * \enum SYS_TCS_DOMAIN
 * \brief power domain of each TCS group.
 *
 */
typedef enum {
        SYS_TCS_DOMAIN_PD_SYS = 0,      /**< System power domain */
        SYS_TCS_DOMAIN_PD_COM,          /**< Communication power domain */
        SYS_TCS_DOMAIN_PD_MEM,          /**< Memory power domain */
        SYS_TCS_DOMAIN_PD_TMR,          /**< Timers power domain */
        SYS_TCS_DOMAIN_PD_PER,          /**< Peripherals power domain */
        SYS_TCS_DOMAIN_PD_RAD,          /**< Radio power domain */
        SYS_TCS_DOMAIN_PD_SYNTH,        /**< Synth power domain */
        SYS_TCS_DOMAIN_NA               /**< No power domain */
} SYS_TCS_DOMAIN;


/**
 * \brief the configured group ids.
 *
 */
typedef enum {
        //XXX based on D3095 Configuration Script content (12/06/22). Note: keep it in sync
        //XXX http://project/PWA/tin_d3095_master/Project%20Documents/131-%20OTP%20Configuration%20Script%20Specification/D2924%20Configuration%20Script%20content.xlsx

        SYS_TCS_GROUP_RESERVED                  = 0,
        SYS_TCS_GROUP_COMMON                    = 0x1,
        /* Power Domains Section */

        SYS_TCS_GROUP_PD_SYS_OLD                = 0x25,                    /**< PD_SYS group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_RF_HPI_OLD   = 0x27,                    /**< PD_MEM group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_TX_HPI_OLD   = 0x28,                    /**< PD_MEM group id */

        SYS_TCS_GROUP_PD_SYS                    = 0x20,                    /**< PD_SYS group id */
        SYS_TCS_GROUP_MAC_ADDR                  = 0x21,
        SYS_TCS_GROUP_PD_ADC                    = 0x22,                    /**< PD_VDD group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_RF_HPI       = 0x81,                    /**< PD_MEM group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_TX_HPI       = 0x82,                    /**< PD_MEM group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_DCORE        = 0x80,                    /**< PD_TMR group id */ //SYS_TCS_GROUP_PD_WIFI_TRIM_RF_FCI_old
        SYS_TCS_GROUP_PD_WIFI_TRIM_LUT          = 0x83,                    /**< PD_AUDIO group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_TXIQ         = 0x84,                    /**< PD_RAD group id */

        SYS_TCS_GROUP_PD_WIFI_TRIM_DCORE_OLD        = 0x19,                    /**< PD_TMR group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_LUT_OLD          = 0x1A,                    /**< PD_AUDIO group id */
        SYS_TCS_GROUP_PD_WIFI_TRIM_TXIQ_OLD         = 0x1B,                    /**< PD_RAD group id */

        /* System Section */
        SYS_TCS_GROUP_BD_ADDR =                 0x20,                   /**< BD_ADDR group id */

        /* Production Test Section */
        SYS_TCS_GROUP_CHIP_ID =                 0xC0,                   /**< CHIP_ID group id */
        SYS_TCS_GROUP_PROD_INFO =               0xC1,                   /**< PROD_INFO group id */
        SYS_TCS_GROUP_PROD_WAFER =              0xC2,                   /**< PROD_WAFER group id */
        SYS_TCS_GROUP_TESTPROGRAM_VERSION =     0xC3,                   /**< TESTPROGRAM_VERSION  group id */
        SYS_TCS_GROUP_PROD_SW_IP =              0xC4,                   /**< PROD_SW_IP group id */

        SYS_TCS_GROUP_MAX =                     0xE0                    /**< Maximum supported group id */
} SYS_TCS_GID;

/**
 * \brief Parsing order for the configuration script
 *
 */
typedef enum {
        TCS_OTP = 0,                    /* CS in OTP */
        TCS_OQSPI,                      /* CS in 4M Flash(0x3fe000) for devleopment, because otp is_empty*/
        TCS_OQSPI_2,                    /* CS in 8M Flash(0x6fe000) for devleopment, because otp is_empty**/
#if (TEST_CS_IN_CONST_TABLE == 1)
        TCS_IN_CONST_TABLE,             /* CS in RAM */
#endif
        TCS_NVMS,                       /* CS in NVMS */
} SYS_TCS_SOURCE;

/**
 * \struct sys_tcs_attr_t
 * \brief attributes per custom value group id
 *
 */
typedef struct {
        /*SYS_TCS_GID*/ uint32_t value_gid:8;
        /*SYS_TCS_TYPE*/ uint32_t value_type:2;        /**< TCS entry type */
        /*SYS_TCS_DOMAIN*/ uint32_t value_domain:4;
        /*SYS_TCS_RETAINED*/ uint32_t is_retained:2;
        uint32_t start:8;             /**< TCS entry start position  */
        uint32_t size:8;              /**< TCS entry type size in words */
} __attribute__((packed)) sys_tcs_attr_t ;

/**
 * \struct sys_tcs_shared_t
 * \brief This structure is used to hold the TCS shared-memory data in specific order
 * to prevent compiler optimization when building for both (SYSCPU and CMAC) CPUs.
 */
typedef struct {
        uint32_t *__tcs_data;               /*!< The CS data as read by the TCS parser */
        uint16_t tcs_data_size:8;                           /*!< Number of words in the __tcs_data */
        uint16_t tcs_attributes_size:8;
        sys_tcs_attr_t *tcs_attributes; /*!< The TCS attributes */
        uint16_t cs_otp_program_version;                  /*!< The version of the CS as described by the SYS_TCS_GROUP_TESTPROGRAM_VERSION attribute*/
        uint16_t cs_valid;
} sys_tcs_shared_t;

/*
 * Reset values of trimmed registers
 */
#define DEFAULT_CHARGER_TEST_CTRL_REG   0x00001F28

/**
 * \brief TCS custom trim values callback
 *
 * \param [in] values_group the TCS group id custom trim values belong to
 * \param [in] user_data user specific data
 * \param [in] values custom trim values
 * \param [in] size the number of the custom trim values
 *
 */
typedef void (*sys_tcs_custom_values_cb)( SYS_TCS_GID values_group , void *user_data, uint32_t *values, uint8_t size);

//#if defined(CORTEX_M33)
/**
 * \brief retrieve the TCS values from CS located in OTP or flash and then store
 * TCS register pair address, value  and/or custom value custom_trim_value in the Global TCS array
 *
 */
void sys_tcs_get_trim_values_from_cs(void);

/**
 * \brief Retrieves the TCS values from CS located in OTP.\n
 *
 */
void sys_tcs_get_trim_values_from_otp_cs(void);

/**
 * \brief check if the register addresses included in reg_address are configured in CS
 * \param [in] reg_address pointer to array containing the register addresses
 * \param [in] num number of register addresses
 * \param [out] trimmed_reg pointer to array containing information whether the corresponding
 * register is included in CS or not
 *
 * \return true if all register addresses are included in CS
 *
 * \warning trimmed_reg should be initialized to false otherwise will report false positive
 * that register address found.
 */
bool sys_tcs_reg_pairs_in_cs(const uint32_t* reg_address, uint8_t num, bool *trimmed_reg);

/**
 * \brief Gets the CS program version from OTP
 *
 * \returns The version number. The function returns 0 in case version does not exist in the CS\n
 *          <b>Example: 01000501</b> The program revision is 1.00.05.01
 *
 * \note The CS programe version is set when the CS is read from the OTP using the
 *       sys_tcs_get_trim_values_from_otp_cs function
 */
uint32_t sys_tcs_get_opt_program_version(void);
//#endif /* CORTEX_M33 */


/**
 * \brief get the number of entries stored in tcs_data
 *
 * \return the number of stored TCS entries. Each entry is 32bit
 */
uint32_t sys_tcs_get_tcs_data_size(void);

/**
 * \brief Gets the number of entries stored in tcs_attributes
 *
 * \return The number of stored TCS tcs_attributes entries.
 *         Each entry is 16bit \link sys_tcs_attr_t \endlink
 */
uint32_t sys_tcs_get_tcs_attributes_size(void);

/**
 * \brief get the number of entries stored in tcs_data
 *
 * \return the number of stored TCS entries. Each entry is 32bit
 */
uint32_t sys_tcs_get_tcs_data_size(void);

/**
 * \brief get the index of attributes from groud_id
 * \param [in] gid
 *
 * \return the index of attributes
 */
uint16_t _gid_to_idx(uint16_t gid);

/**
 * \brief get the groud_id of attributes from index
 * \param [in] idx
 *
 * \return the groud_id of attributes
 */
uint16_t _idx_to_gid(uint16_t idx);

/**
 * \brief set retention or shared based on group_id
 *
 * \param [in] gid
 *
 */
bool sys_tcs_set_tcs(SYS_TCS_GID gid, uint32 is_set);

/**
 * \brief get size of OTP TCS
 *
 * \param [out] size
 * \param [in] item
 * \param [out] item_reapeat_count
 *
 *  * \return true if success, other false
 */
uint32_t sys_tcs_get_otp_size(uint16_t *size, uint32_t item, uint16_t *item_reapeat_count);

/**
 * \brief get item of OTP TCS
 *
 * \param [in] item
 * \param [in] item
 * \param [out] size
 *
 *  * \return true if success, other false
 */
bool sys_tcs_get_item_in_otp_cs(const uint32_t item, uint32_t *dest, uint16_t *size);

/**
 * \brief display information of OTP TCS
 *
 * \param [in] item
 * \param [in] item
 * \param [out] size
 *
 *  * \return true if success, other false
 */
void tcs_info_printf(bool match);

uint16_t retained_gid_to_idx(uint16_t gid);
uint16_t reatained_idx_to_gid(uint16_t idx);
SYS_TCS_TYPE based_tcs_get_value_type(SYS_TCS_GID gid);
char *tcs_group(uint32_t group);
sys_tcs_attr_t* _sys_tcs_get_based_on_tcs_attributes_ptr(void);
uint16_t based_tcs_gid_to_idx(uint16_t gid);
uint16_t based_tcs_idx_to_gid(uint16_t idx);


#endif /* SYS_TCS_DA1487X_H_ */
#endif /* DEVICE_FAMILY */
/**
\}
\}
*/
