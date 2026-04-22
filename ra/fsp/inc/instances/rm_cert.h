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

#ifndef __RM_CERT_H__
#define __RM_CERT_H__

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "FreeRTOS.h"

#include "task.h"
#include "assert.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup RM_CERT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define CERT_MAX_LENGTH     (1024 * 4)
#define FLASH_WRITE_LENGTH  (1024 * 4)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Error codes for rm_cert */
typedef enum {
    RM_CERT_ERR_OK = 0,             ///< The operation succeeded.
    RM_CERT_ERR_NOK,                ///< The operation failed.
    RM_CERT_ERR_INVALID_MODULE,     ///< Invaild module.
    RM_CERT_ERR_INVALID_TYPE,       ///< Invaild type.
    RM_CERT_ERR_INVALID_FORMAT,     ///< Invaild format.
    RM_CERT_ERR_INVALID_LENGTH,     ///< Invaild length.
    RM_CERT_ERR_INVALID_FLASH_ADDR, ///< Failed to get flash memory.
    RM_CERT_ERR_INVALID_PARAMS,     ///< Invalid certificate.
    RM_CERT_ERR_FOPEN_FAILED,       ///< Failed to open.
    RM_CERT_ERR_MEM_FAILED,         ///< Failed to allocate memory.
    RM_CERT_ERR_EMPTY_CERTIFICATE   ///< No certificate.
} rm_cert_err_t;

/** Module ID */
typedef enum {
    RM_CERT_MODULE_NONE =          -1, ///< Invalid module.
    RM_CERT_MODULE_MQTT =           0, ///< MQTT.
    RM_CERT_MODULE_HTTPS_CLIENT =   1, ///< HTTPS client.
    RM_CERT_MODULE_WPA_ENTERPRISE = 2, ///< WPA enterprise.
    RM_CERT_MODULE_OTA =            3, ///< OTA.
    RM_CERT_MODULE_HTTPS_SERVER =   4, ///< HTTPS server.
    RM_CERT_MODULE_ATCMD =          5  ///< AT command.
} rm_cert_module_t;

/** Certificate type */
typedef enum {
    RM_CERT_TYPE_NONE = -1,       ///< Invalid type.
    RM_CERT_TYPE_CA_CERT = 0,     ///< CA certificate.
    RM_CERT_TYPE_CERT = 1,        ///< Certificate.
    RM_CERT_TYPE_PRIVATE_KEY = 2, ///< Private key.
    RM_CERT_TYPE_DH_PARAMS = 3    ///< DH params.
} rm_cert_type_t;

/** Certificate format */
typedef enum {
    RM_CERT_FORMAT_NONE = -1, ///< Invalid format.
    RM_CERT_FORMAT_DER = 0,   ///< DER format.
    RM_CERT_FORMAT_PEM = 1    ///< PEM format.
} rm_cert_format_t;

typedef enum {
    RM_CERT_MODE_NONE = -1,
    RM_CERT_MODE_STORE = 0,
    RM_CERT_MODE_DELETION = 1
} rm_cert_mode_t;

typedef struct {
    unsigned int module;
    unsigned int type;
    unsigned int format;
    unsigned int cert_len;
} rm_cert_info_t;

#define RM_CERT_MARGIN       (0)
#define RM_CERT_MAX_LENGTH   (FLASH_WRITE_LENGTH - sizeof(rm_cert_info_t) - RM_CERT_MARGIN)

typedef struct {
    rm_cert_info_t info;
    uint8_t cert[RM_CERT_MAX_LENGTH];
} rm_cert_t;

/* Compile assert */
#if defined(static_assert)
static_assert(FLASH_WRITE_LENGTH >= sizeof(rm_cert_t),
        "rm_cert_t is too large for the size of underlying FLASH_WRITE_LENGTH");
#endif

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 ****************************************************************************************
 * @brief Delete all the certificates stored in the flash memory.
 *
 * @retval RM_CERT_ERR_OK                 The operation succeeded.
 * @retval RM_CERT_ERR_NOK                Failed to erase data.
 * @retval RM_CERT_ERR_INVALID_MODULE     Invaild module.
 * @retval RM_CERT_ERR_INVALID_TYPE       Invaild type.
 * @retval RM_CERT_ERR_INVALID_FLASH_ADDR Failed to get flash memory.
 ****************************************************************************************
 */
int RM_CERT_DeleteAll(void);

/**
 ****************************************************************************************
 * @brief Write the certificate specified by module and type to the flash memory.
 * @param[in] module Module ID.
 * @param[in] type Certificate type.
 * @param[in] format Certificate format.
 * @param[in] in Pointer to write certificate.
 * @param[in] inlen Length of certificate.
 *
 * @retval RM_CERT_ERR_OK                 The operation succeeded.
 * @retval RM_CERT_ERR_NOK                The operation failed.
 * @retval RM_CERT_ERR_INVALID_MODULE     Invaild module.
 * @retval RM_CERT_ERR_INVALID_TYPE       Invaild type.
 * @retval RM_CERT_ERR_INVALID_FORMAT     Invaild format.
 * @retval RM_CERT_ERR_INVALID_LENGTH     Invaild length.
 * @retval RM_CERT_ERR_INVALID_FLASH_ADDR Failed to get flash memory.
 * @retval RM_CERT_ERR_INVALID_PARAMS     Invalid certificate.
 * @retval RM_CERT_ERR_MEM_FAILED         Failed to allocate memory.
 ****************************************************************************************
 */
rm_cert_err_t RM_CERT_Write(rm_cert_module_t module, rm_cert_type_t type, rm_cert_format_t format, uint8_t *in, size_t inlen);

/**
 ****************************************************************************************
 * @brief Read the certificate specified by module and type from the flash memory.
 * @param[in] module Module ID.
 * @param[in] type Certificate type.
 * @param[out] format Certificate format.
 * @param[out] out Pointer to read certificate.
 * @param[out] outlen Length of certificate.
 *
 * @retval RM_CERT_ERR_OK                 The operation succeeded.
 * @retval RM_CERT_ERR_INVALID_MODULE     Invaild module.
 * @retval RM_CERT_ERR_INVALID_TYPE       Invaild type.
 * @retval RM_CERT_ERR_INVALID_LENGTH     Not enough space.
 * @retval RM_CERT_ERR_INVALID_FLASH_ADDR Failed to get flash memory.
 * @retval RM_CERT_ERR_MEM_FAILED         Failed to allocate memory.
 * @retval RM_CERT_ERR_EMPTY_CERTIFICATE  No certificate.
 ****************************************************************************************
 */
rm_cert_err_t RM_CERT_Read(rm_cert_module_t module, rm_cert_type_t type, rm_cert_format_t *format, uint8_t *out, size_t *outlen);

/**
 ****************************************************************************************
 * @brief Delete the certificate specified by module and type from the flash memory.
 * @param[in] module Module ID.
 * @param[in] type Certificate type.
 *
 * @retval RM_CERT_ERR_OK                 The operation succeeded.
 * @retval RM_CERT_ERR_NOK                Failed to erase data.
 * @retval RM_CERT_ERR_INVALID_MODULE     Invaild module.
 * @retval RM_CERT_ERR_INVALID_TYPE       Invaild type.
 * @retval RM_CERT_ERR_INVALID_FLASH_ADDR Failed to get flash memory.
 ****************************************************************************************
 */
rm_cert_err_t RM_CERT_Delete(rm_cert_module_t module, rm_cert_type_t type);

/**
 ****************************************************************************************
 * @brief Check whether the certificate specified by module and type exists or not in the flash memory.
 * @param[in] module Module ID.
 * @param[in] type Certificate type.
 *
 * @retval true                           The certificate exists.
 * @retval false                          The certificate does not exist.
 ****************************************************************************************
 */
int RM_CERT_IsExistCert(rm_cert_module_t module, rm_cert_type_t type);

/**
 ****************************************************************************************
 * @brief Get module ID from specific flash memory address.
 * @param[in] flash_addr Specific flash memory address to get module ID.
 *
 * @return Module ID. See @ref rm_cert_module_t.
 ****************************************************************************************
 */
rm_cert_module_t RM_CERT_GetModule(uint32_t flash_addr);

/**
 ****************************************************************************************
 * @brief Get certificate type from specific flash memory address.
 * @param[in] flash_addr Specific flash memory address to get certificate type.
 *
 * @return Certificate type. See @ref rm_cert_type_t.
 ****************************************************************************************
 */
rm_cert_type_t RM_CERT_GetType(uint32_t flash_addr);

/**
 ****************************************************************************************
 * @brief Check whether the certificate is pem format or not.
 * @param[in] buf Pointer to the buffer of certificate.
 *
 * @retval true                           PEM format.
 * @retval false                          Other format.
 ****************************************************************************************
 */
int RM_CERT_IsPemFormat(const char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __RM_CERT_H__ */

/*******************************************************************************************************************//**
 * @} (end defgroup RM_CERT)
 **********************************************************************************************************************/
