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
#include "rm_vee_flash_w_cfg.h"
#ifdef RM_VEE_USE_CERT
/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_cert.h"
#include "custom_config_sdk.h" /* For __SUPPORT_ATCMD_TLS__ */
#include "r_ospi_w_cfg.h" /* For RRQ61X_OSPI_W_ENABLED */

#if defined (__SUPPORT_ATCMD_TLS__)
#include "rm_atcmd_w_core_socket_cert_mng.h"
#endif
#ifndef RRQ61X_OSPI_W_ENABLED
#include "ad_flash.h"
#else /* RRQ61X_OSPI_W_ENABLED */
#include "util_api.h"
#endif /* RRQ61X_OSPI_W_ENABLED */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#undef  RM_CERT_ENABLE_DBG_INFO
#undef  RM_CERT_ENABLE_DBG_ERR

#define RM_CERT_PRT  printf

#if defined (RM_CERT_ENABLE_DBG_INFO)
#define RM_CERT_INFO(fmt, ...)   \
        RM_CERT_PRT("[%s:%d]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define RM_CERT_INFO(...)    do {} while (0)
#endif  /* (RM_CERT_ENABLE_DBG_INFO) */

#if defined (RM_CERT_ENABLE_DBG_ERR)
#define RM_CERT_ERR(fmt, ...)    \
        RM_CERT_PRT("[%s:%d]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define RM_CERT_ERR(...) do {} while (0)
#endif /* (RM_CERT_ENABLE_DBG_ERR) */

/***********************************************************************************************************************
 * External functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

static void *rm_cert_internal_calloc(size_t n, size_t size);
static void rm_cert_internal_free(void *f);
static int rm_cert_is_valid_module(rm_cert_module_t module);
static int rm_cert_is_valid_type(rm_cert_type_t type);
static int rm_cert_is_valid_format(rm_cert_format_t format);
static int rm_cert_is_valid_length(size_t len);
static uint32_t rm_cert_get_flash_address(rm_cert_module_t module, rm_cert_type_t type);
static rm_cert_err_t rm_cert_write_by_addr(uint32_t addr, rm_cert_format_t format, uint8_t *in, size_t inlen);
static rm_cert_err_t rm_cert_read_by_addr(uint32_t addr, rm_cert_format_t *format, uint8_t *out, size_t *outlen);
static rm_cert_err_t rm_cert_delete_by_addr(uint32_t addr);

static void *rm_cert_internal_calloc(size_t n, size_t size)
{
    void *buf = NULL;
    size_t buflen = (n * size);

    buf = pvPortMalloc(buflen);
    if (buf) {
        memset(buf, 0x00, buflen);
    }

    return buf;
}

static void rm_cert_internal_free(void *f)
{
    if (f == NULL) {
        return ;
    }

    vPortFree(f);
    f = NULL;

    return ;
}

void *(*rm_cert_calloc)(size_t n, size_t size) = rm_cert_internal_calloc;
void (*rm_cert_free)(void *ptr) = rm_cert_internal_free;

static int rm_cert_is_valid_module(rm_cert_module_t module)
{
    RM_CERT_INFO("Module:%d\n", module);

    if ((module == RM_CERT_MODULE_MQTT           && CERT_MQTTS_CLI_USED == 1)  ||
        (module == RM_CERT_MODULE_HTTPS_CLIENT   && CERT_HTTPS_CLI_USED == 1)  ||
        (module == RM_CERT_MODULE_WPA_ENTERPRISE && CERT_WPA_ENT_USED   == 1)  ||
        (module == RM_CERT_MODULE_OTA            && CERT_OTA_USED       == 1)  ||
        (module == RM_CERT_MODULE_HTTPS_SERVER   && CERT_HTTPS_SVR_USED == 1)  ||
        (module == RM_CERT_MODULE_ATCMD          && CERT_ATCMD_USED     == 1)) {
        return pdTRUE;
    } else {
        return pdFALSE;
    }
}

static int rm_cert_is_valid_type(rm_cert_type_t type)
{
    RM_CERT_INFO("Type:%d\n", type);

    switch (type) {
    case RM_CERT_TYPE_CA_CERT:
    case RM_CERT_TYPE_CERT:
    case RM_CERT_TYPE_PRIVATE_KEY:
    case RM_CERT_TYPE_DH_PARAMS:
        return pdTRUE;
    default:
        return pdFALSE;
    }
}

static int rm_cert_is_valid_format(rm_cert_format_t format)
{
    RM_CERT_INFO("Format:%d\n", format);

    switch (format) {
    case RM_CERT_FORMAT_DER:
    case RM_CERT_FORMAT_PEM:
        return pdTRUE;
    default:
        return pdFALSE;
    }
}

static int rm_cert_is_valid_length(size_t len)
{
    RM_CERT_INFO("Length:%d\n", len);

    if (len > RM_CERT_MAX_LENGTH) {
        return pdFALSE;
    }

    return pdTRUE;
}

static uint32_t rm_cert_get_flash_address(rm_cert_module_t module, rm_cert_type_t type)
{
    RM_CERT_INFO("Module:%d, type:%d\n", module, type);

    if (module == RM_CERT_MODULE_MQTT) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_MQTT_CLI_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SF_TLS_CERT_MQTT_CLI_CERTIFICATE_ADDR;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SF_TLS_CERT_MQTT_CLI_PRIVATE_KEY_ADDR;
        } else if (type == RM_CERT_TYPE_DH_PARAMS) {
            return SF_TLS_CERT_MQTT_CLI_DH_PARAMETER_ADDR;
        }
    } else if (module == RM_CERT_MODULE_HTTPS_CLIENT) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_HTTPS_CLI_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SF_TLS_CERT_HTTPS_CLI_CERTIFICATE_ADDR;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SF_TLS_CERT_HTTPS_CLI_PRIVATE_KEY_ADDR;
        } else if (type == RM_CERT_TYPE_DH_PARAMS) {
            return SF_TLS_CERT_HTTPS_CLI_DH_PARAMETER_ADDR;
        }
    } else if (module == RM_CERT_MODULE_WPA_ENTERPRISE) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_WPA_ENT_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SF_TLS_CERT_WPA_ENT_CERTIFICATE_ADDR;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SF_TLS_CERT_WPA_ENT_PRIVATE_KEY_ADDR;
        } else if (type == RM_CERT_TYPE_DH_PARAMS) {
            return SF_TLS_CERT_WPA_ENT_DH_PARAMETER_ADDR;
        }
    } else if (module == RM_CERT_MODULE_OTA) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_OTA_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SF_TLS_CERT_OTA_CERTIFICATE_ADDR;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SF_TLS_CERT_OTA_PRIVATE_KEY_ADDR;
        } else if (type == RM_CERT_TYPE_DH_PARAMS) {
            return SF_TLS_CERT_OTA_DH_PARAMETER_ADDR;
        }
    } else if (module == RM_CERT_MODULE_HTTPS_SERVER) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_HTTPS_SVR_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SF_TLS_CERT_HTTPS_SVR_CERTIFICATE_ADDR;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SF_TLS_CERT_HTTPS_SVR_PRIVATE_KEY_ADDR;
        } else if (type == RM_CERT_TYPE_DH_PARAMS) {
            return SF_TLS_CERT_HTTPS_SVR_DH_PARAMETER_ADDR;
        }
    } else if (module == RM_CERT_MODULE_ATCMD) {
        if (type == RM_CERT_TYPE_CA_CERT) {
            return SF_TLS_CERT_ATCMD_CA_ADDR;
        } else if (type == RM_CERT_TYPE_CERT) {
            return SFLASH_ATCMD_TLS_CERT_01;
        } else if (type == RM_CERT_TYPE_PRIVATE_KEY) {
            return SFLASH_ATCMD_TLS_CERT_02;
        }
    }

    return 0;
}

rm_cert_module_t RM_CERT_GetModule(uint32_t flash_addr)
{
    RM_CERT_INFO("Addr:0x%x\n", (unsigned int)flash_addr);

    if (CERT_MQTTS_CLI_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_MQTT_CLI_CA_ADDR:
        case SF_TLS_CERT_MQTT_CLI_CERTIFICATE_ADDR:
        case SF_TLS_CERT_MQTT_CLI_PRIVATE_KEY_ADDR:
        case SF_TLS_CERT_MQTT_CLI_DH_PARAMETER_ADDR:
            return RM_CERT_MODULE_MQTT;
        default:
            break;
        }
    }
    if (CERT_HTTPS_CLI_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_HTTPS_CLI_CA_ADDR:
        case SF_TLS_CERT_HTTPS_CLI_CERTIFICATE_ADDR:
        case SF_TLS_CERT_HTTPS_CLI_PRIVATE_KEY_ADDR:
        case SF_TLS_CERT_HTTPS_CLI_DH_PARAMETER_ADDR:
            return RM_CERT_MODULE_HTTPS_CLIENT;
        default:
            break;
        }
    }
    if (CERT_WPA_ENT_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_WPA_ENT_CA_ADDR:
        case SF_TLS_CERT_WPA_ENT_CERTIFICATE_ADDR:
        case SF_TLS_CERT_WPA_ENT_PRIVATE_KEY_ADDR:
        case SF_TLS_CERT_WPA_ENT_DH_PARAMETER_ADDR:
            return RM_CERT_MODULE_WPA_ENTERPRISE;
        default:
            break;
        }
    }
    if (CERT_OTA_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_OTA_CA_ADDR:
        case SF_TLS_CERT_OTA_CERTIFICATE_ADDR:
        case SF_TLS_CERT_OTA_PRIVATE_KEY_ADDR:
        case SF_TLS_CERT_OTA_DH_PARAMETER_ADDR:
            return RM_CERT_MODULE_OTA;
        default:
            break;
        }
    }
    if (CERT_HTTPS_SVR_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_HTTPS_SVR_CA_ADDR:
        case SF_TLS_CERT_HTTPS_SVR_CERTIFICATE_ADDR:
        case SF_TLS_CERT_HTTPS_SVR_PRIVATE_KEY_ADDR:
        case SF_TLS_CERT_HTTPS_SVR_DH_PARAMETER_ADDR:
            return RM_CERT_MODULE_HTTPS_SERVER;
        default:
            break;
        }
    }
    if (CERT_ATCMD_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_ATCMD_CA_ADDR:
        case SFLASH_ATCMD_TLS_CERT_01:
        case SFLASH_ATCMD_TLS_CERT_02:
        case SFLASH_ATCMD_TLS_CERT_03:
        case SFLASH_ATCMD_TLS_CERT_04:
        case SFLASH_ATCMD_TLS_CERT_05:
        case SFLASH_ATCMD_TLS_CERT_06:
        case SFLASH_ATCMD_TLS_CERT_07:
        case SFLASH_ATCMD_TLS_CERT_08:
        case SFLASH_ATCMD_TLS_CERT_09:
        case SFLASH_ATCMD_TLS_CERT_10:
        case SFLASH_ATCMD_TLS_CERT_11:
        case SFLASH_ATCMD_TLS_CERT_12:
        case SFLASH_ATCMD_TLS_CERT_13:
        case SFLASH_ATCMD_TLS_CERT_14:
        case SFLASH_ATCMD_TLS_CERT_15:
        case SFLASH_ATCMD_TLS_CERT_16:
            return RM_CERT_MODULE_ATCMD;
        default:
            break;
        }
    }

    return RM_CERT_MODULE_NONE;
}

rm_cert_type_t RM_CERT_GetType(uint32_t flash_addr)
{
    RM_CERT_INFO("Addr:0x%x\n", (unsigned int)flash_addr);

    if (CERT_MQTTS_CLI_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_MQTT_CLI_CA_ADDR:
            return RM_CERT_TYPE_CA_CERT;
        case SF_TLS_CERT_MQTT_CLI_CERTIFICATE_ADDR:
            return RM_CERT_TYPE_CERT;
        case SF_TLS_CERT_MQTT_CLI_PRIVATE_KEY_ADDR:
            return RM_CERT_TYPE_PRIVATE_KEY;
        case SF_TLS_CERT_MQTT_CLI_DH_PARAMETER_ADDR:
            return RM_CERT_TYPE_DH_PARAMS;
        }
    }
    if (CERT_HTTPS_CLI_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_HTTPS_CLI_CA_ADDR:
            return RM_CERT_TYPE_CA_CERT;
        case SF_TLS_CERT_HTTPS_CLI_CERTIFICATE_ADDR:
            return RM_CERT_TYPE_CERT;
        case SF_TLS_CERT_HTTPS_CLI_PRIVATE_KEY_ADDR:
            return RM_CERT_TYPE_PRIVATE_KEY;
        case SF_TLS_CERT_HTTPS_CLI_DH_PARAMETER_ADDR:
            return RM_CERT_TYPE_DH_PARAMS;
        }
    }
    if (CERT_WPA_ENT_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_WPA_ENT_CA_ADDR:
            return RM_CERT_TYPE_CA_CERT;
        case SF_TLS_CERT_WPA_ENT_CERTIFICATE_ADDR:
            return RM_CERT_TYPE_CERT;
        case SF_TLS_CERT_WPA_ENT_PRIVATE_KEY_ADDR:
            return RM_CERT_TYPE_PRIVATE_KEY;
        case SF_TLS_CERT_WPA_ENT_DH_PARAMETER_ADDR:
            return RM_CERT_TYPE_DH_PARAMS;
        }
    }
    if (CERT_OTA_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_OTA_CA_ADDR:
            return RM_CERT_TYPE_CA_CERT;
        case SF_TLS_CERT_OTA_CERTIFICATE_ADDR:
            return RM_CERT_TYPE_CERT;
        case SF_TLS_CERT_OTA_PRIVATE_KEY_ADDR:
            return RM_CERT_TYPE_PRIVATE_KEY;
        case SF_TLS_CERT_OTA_DH_PARAMETER_ADDR:
            return RM_CERT_TYPE_DH_PARAMS;
        }
    }
    if (CERT_HTTPS_SVR_USED == 1) {
        switch (flash_addr) {
        case SF_TLS_CERT_HTTPS_SVR_CA_ADDR:
            return RM_CERT_TYPE_CA_CERT;
        case SF_TLS_CERT_HTTPS_SVR_CERTIFICATE_ADDR:
            return RM_CERT_TYPE_CERT;
        case SF_TLS_CERT_HTTPS_SVR_PRIVATE_KEY_ADDR:
            return RM_CERT_TYPE_PRIVATE_KEY;
        case SF_TLS_CERT_HTTPS_SVR_DH_PARAMETER_ADDR:
            return RM_CERT_TYPE_DH_PARAMS;
        }
    }

    return RM_CERT_TYPE_NONE;
}

int RM_CERT_IsPemFormat(const char *buf)
{
    /* RA6W1 only checks ----BEGIN string.
     * -----BEGIN CERTIFICATE-----
     * -----BEGIN RSA PRIVATE KEY-----
     * -----BEGIN EC PRIVATE KEY-----
     * -----BEGIN PRIVATE KEY-----
     * -----BEGIN ENCRYPTED PRIVATE KEY-----
     */
    const char *pem_prefix = "-----BEGIN";
    const size_t pem_prefix_len = strlen(pem_prefix);

    if ((strlen(buf) > pem_prefix_len)
        && strstr(buf, pem_prefix)) {
        return pdTRUE;
    }

    return pdFALSE;
}

static int cert_del_all = 0;
int RM_CERT_DeleteAll(void)
{
    int err = 0, i;
    int any_deleted = 0;

    uint32_t flash_addr[] = {
        SF_TLS_CERT_MQTT_CLI_CA_ADDR,               /* #1 */
        CERT_MQTTS_CLI_USED,
        SF_TLS_CERT_MQTT_CLI_CERTIFICATE_ADDR,
        CERT_MQTTS_CLI_USED,
        SF_TLS_CERT_MQTT_CLI_PRIVATE_KEY_ADDR,
        CERT_MQTTS_CLI_USED,        
        SF_TLS_CERT_MQTT_CLI_DH_PARAMETER_ADDR,
        CERT_MQTTS_CLI_USED,
        
        SF_TLS_CERT_HTTPS_CLI_CA_ADDR,              /* #2 */
        CERT_HTTPS_CLI_USED,
        SF_TLS_CERT_HTTPS_CLI_CERTIFICATE_ADDR,
        CERT_HTTPS_CLI_USED,
        SF_TLS_CERT_HTTPS_CLI_PRIVATE_KEY_ADDR,
        CERT_HTTPS_CLI_USED,
        SF_TLS_CERT_HTTPS_CLI_DH_PARAMETER_ADDR,
        CERT_HTTPS_CLI_USED,
        
        SF_TLS_CERT_WPA_ENT_CA_ADDR,                /* #3 */
        CERT_WPA_ENT_USED,
        SF_TLS_CERT_WPA_ENT_CERTIFICATE_ADDR,
        CERT_WPA_ENT_USED,
        SF_TLS_CERT_WPA_ENT_PRIVATE_KEY_ADDR,
        CERT_WPA_ENT_USED,
        SF_TLS_CERT_WPA_ENT_DH_PARAMETER_ADDR,
        CERT_WPA_ENT_USED,
        
        SF_TLS_CERT_OTA_CA_ADDR,                    /* #4 */
        CERT_OTA_USED,
        SF_TLS_CERT_OTA_CERTIFICATE_ADDR,
        CERT_OTA_USED,
        SF_TLS_CERT_OTA_PRIVATE_KEY_ADDR,
        CERT_OTA_USED,
        SF_TLS_CERT_OTA_DH_PARAMETER_ADDR,
        CERT_OTA_USED,
        
        SF_TLS_CERT_HTTPS_SVR_CA_ADDR,              /* #5 */
        CERT_HTTPS_SVR_USED,
        SF_TLS_CERT_HTTPS_SVR_CERTIFICATE_ADDR,
        CERT_HTTPS_SVR_USED,
        SF_TLS_CERT_HTTPS_SVR_PRIVATE_KEY_ADDR,
        CERT_HTTPS_SVR_USED,
        SF_TLS_CERT_HTTPS_SVR_DH_PARAMETER_ADDR,
        CERT_HTTPS_SVR_USED,
    };

    for (i = 0; i < (int) (sizeof(flash_addr)/sizeof(flash_addr[0])); i = i + 2) {
        
        rm_cert_module_t module = RM_CERT_GetModule(flash_addr[i]);
        rm_cert_type_t type = RM_CERT_GetType(flash_addr[i]);
    
        if (flash_addr[i+1] == 1 && RM_CERT_IsExistCert(module, type)) {
            cert_del_all = 1;
            err += RM_CERT_Delete(module, type);
            cert_del_all = 0;
            printf(".");
            vTaskDelay(portCONVERT_MS_2_TICKS(200));
            any_deleted++;
        }
    }
    if (any_deleted) {
        printf("\n");
    }

#if defined (__SUPPORT_ATCMD_TLS__)
    for (i = 0; i < ATCMD_CM_MAX_CERT_NUM; i++) {
        err += atcmd_cm_clear_cert_by_idx(i);
    }
#endif /* __SUPPORT_ATCMD_TLS__ */
    return err;
}

rm_cert_err_t RM_CERT_Write(rm_cert_module_t module, rm_cert_type_t type, rm_cert_format_t format, uint8_t *in, size_t inlen)
{
    uint32_t flash_addr = 0x00;

    RM_CERT_INFO("module(%d), type(%d), format(%d), inlen(%d)\n",
            module, type, format, inlen);

    /* Check module */
    if (!rm_cert_is_valid_module(module)) {
        RM_CERT_ERR("Invaild module(%d)\n", module);
        return RM_CERT_ERR_INVALID_MODULE;
    }

    /* Check type */
    if (!rm_cert_is_valid_type(type)) {
        RM_CERT_ERR("Invaild type(%d)\n", type);
        return RM_CERT_ERR_INVALID_TYPE;
    }

    /* Get flash memory address */
    flash_addr = rm_cert_get_flash_address(module, type);
    
    if (!flash_addr) {
        RM_CERT_ERR("Failed to get flash memory(%d,%d)\n", module, type);
        return RM_CERT_ERR_INVALID_FLASH_ADDR;
    }
    
    return rm_cert_write_by_addr(flash_addr, format, in, inlen);
}

static rm_cert_err_t rm_cert_write_by_addr(uint32_t addr, rm_cert_format_t format, uint8_t *in, size_t inlen)
{
    rm_cert_err_t err = RM_CERT_ERR_OK;
    rm_cert_t *rm_cert_ptr = NULL;
    uint8_t *buf = NULL;

#ifndef RRQ61X_OSPI_W_ENABLED
    int offset = 0;
    size_t w;
#endif //!RRQ61X_OSPI_W_ENABLED

    RM_CERT_INFO("flash memory address(0x%x)\n", (unsigned int)addr);

    /* Check format */
    if (!rm_cert_is_valid_format(format)) {
        RM_CERT_ERR("Invaild format(%d)\n", format);
        return RM_CERT_ERR_INVALID_FORMAT;
    }

    /* Check length */
    if (!rm_cert_is_valid_length(inlen)) {
        RM_CERT_ERR("Invaild length(%zu)\n", inlen);
        return RM_CERT_ERR_INVALID_LENGTH;
    }

    /* TODO: is required to check vaildation of certificate using parsing? */
    /* Check cert */
    if (!in) {
        RM_CERT_ERR("Invalid certificate\n");
        return RM_CERT_ERR_INVALID_PARAMS;
    }

    buf = rm_cert_calloc(FLASH_WRITE_LENGTH, sizeof(uint8_t));
    if (!buf) {
        RM_CERT_ERR("Failed to allocate memory(%zu)\n", FLASH_WRITE_LENGTH);
        return RM_CERT_ERR_MEM_FAILED;
    }

    /* Construct cert data */
    rm_cert_ptr = (rm_cert_t *)buf;
    rm_cert_ptr->info.format = (unsigned int) format;
    rm_cert_ptr->info.cert_len = inlen;
    memcpy(rm_cert_ptr->cert, in, inlen);

    if ((format == RM_CERT_FORMAT_PEM) && (strlen((const char *)in) == inlen)) {
        rm_cert_ptr->info.cert_len += 1;
    }

    rm_cert_ptr = NULL;

#ifdef RRQ61X_OSPI_W_ENABLED
    if (util_sflash_write((int) addr, (char *) buf, FLASH_WRITE_LENGTH) == pdTRUE) {
        err = RM_CERT_ERR_OK;
    } else {
        err = RM_CERT_ERR_NOK;
    }

#else ///////////////////////////////////////////////////////////////

    ad_flash_init();

    offset = ad_flash_update_possible(addr, (uint8_t*)buf, FLASH_WRITE_LENGTH);
    if (offset == FLASH_WRITE_LENGTH) {
        /* same content existing, no need to write to flash */ 
        err = RM_CERT_ERR_OK;
    } else {
        if (offset >= 0) {
            /* no need to erase flash */
        } else {
            /* offset < 0, erasing flash is needed */
            ad_flash_erase_region(addr, FLASH_WRITE_LENGTH);
            offset = 0;
        }

        w = offset + ad_flash_write(addr + offset, 
                                    buf + offset, FLASH_WRITE_LENGTH - offset);
            
        /* check that the intended content-size is actually written in flash */
        if (w == FLASH_WRITE_LENGTH) {
            err = RM_CERT_ERR_OK;
        } else {
            err = RM_CERT_ERR_NOK;
        }
    }
#endif /* RRQ61X_OSPI_W_ENABLED */

    /* Release cert data */
    if (buf) {
        rm_cert_free(buf);
        buf = NULL;
    }

    return err;
}

rm_cert_err_t RM_CERT_Read(rm_cert_module_t module, rm_cert_type_t type, rm_cert_format_t *format, uint8_t *out, size_t *outlen)
{
    uint32_t flash_addr = 0x00;

    RM_CERT_INFO("module(%d), type(%d), outlen(%d)\n", module, type, *outlen);

    if (format) {
        *format = RM_CERT_FORMAT_NONE;
    }

    /* Check module */
    if (!rm_cert_is_valid_module(module)) {
        RM_CERT_ERR("Invaild module(%d)\n", module);
        return RM_CERT_ERR_INVALID_MODULE;
    }

    /* Check type */
    if (!rm_cert_is_valid_type(type)) {
        RM_CERT_ERR("Invaild type(%d)\n", type);
        return RM_CERT_ERR_INVALID_TYPE;
    }

    /* Get flash memory address */
    flash_addr = rm_cert_get_flash_address(module, type);
    if (!flash_addr) {
        RM_CERT_ERR("Failed to get flash memory(%d,%d)\n", module, type);
        return RM_CERT_ERR_INVALID_FLASH_ADDR;
    }
    
    return rm_cert_read_by_addr(flash_addr, format, out, outlen);
}

static rm_cert_err_t rm_cert_read_by_addr(uint32_t addr, rm_cert_format_t *format, uint8_t *out, size_t *outlen)
{
    rm_cert_err_t err = RM_CERT_ERR_OK;
    rm_cert_t *rm_cert_ptr = NULL;
    uint8_t *buf = NULL;
    char *cert_ptr = NULL;
    size_t cert_len = 0;

    RM_CERT_INFO("flash memory address(0x%x)\n", (unsigned int)addr);

    if (format) {
        *format = RM_CERT_FORMAT_NONE;
    }
    
    buf = rm_cert_calloc(FLASH_WRITE_LENGTH, sizeof(uint8_t)); /* need to modify! */
    if (!buf) {
        RM_CERT_ERR("Failed to allocate memory(%zu)\n", FLASH_WRITE_LENGTH);
        return RM_CERT_ERR_MEM_FAILED;
    }

    /* Read certificate */
#ifdef RRQ61X_OSPI_W_ENABLED
    util_sflash_read((int) addr, (uint8_t *) buf, FLASH_WRITE_LENGTH);
#else
    ad_flash_init();

    ad_flash_read(addr, (uint8_t*)buf, FLASH_WRITE_LENGTH);
#endif /* RRQ61X_OSPI_W_ENABLED */
    rm_cert_ptr = (rm_cert_t *)buf;
    if (buf[0] == 0xFF) {
        /* No certificate */
        RM_CERT_INFO("Empty certificate\n");
        err = RM_CERT_ERR_EMPTY_CERTIFICATE;
    } else if (RM_CERT_IsPemFormat((const char *)rm_cert_ptr->cert)) {
        if (out) {
            if (*outlen < rm_cert_ptr->info.cert_len) {
                RM_CERT_ERR("Not enough space(%zu:%zu)\n", *outlen, rm_cert_ptr->info.cert_len);
                err = RM_CERT_ERR_INVALID_LENGTH;
            } else {
                cert_ptr = (char *)rm_cert_ptr->cert;
                cert_len = rm_cert_ptr->info.cert_len;
            }
        }

        if ((err == RM_CERT_ERR_OK) && format) {
            *format = RM_CERT_FORMAT_PEM;
        }
    } else {
        if (out) {
            if (*outlen < rm_cert_ptr->info.cert_len) {
                RM_CERT_ERR("Not enough space(%zu:%u)\n", *outlen, rm_cert_ptr->info.cert_len);
                err = RM_CERT_ERR_INVALID_LENGTH;
            } else {
                cert_ptr = (char *)rm_cert_ptr->cert;
                cert_len = rm_cert_ptr->info.cert_len;
            }
        }

        if ((err == RM_CERT_ERR_OK) && format) {
            *format = (int) rm_cert_ptr->info.format;
        }
    }

    if ((err == RM_CERT_ERR_OK) && out && cert_ptr && cert_len) {
        memcpy(out, cert_ptr, cert_len);
        *outlen = cert_len;
    }

    if (err) {
#if !defined(RM_CERT_ENABLE_DBG_INFO)
        if (err != RM_CERT_ERR_EMPTY_CERTIFICATE)
#endif /* ! RM_CERT_ENABLE_DBG_INFO */
        {
            RM_CERT_ERR("Failed to get certificate"
                    "(addr:0x%x, outlen:%zu, err:%d)\n",
                    (unsigned int)addr, *outlen, err);
        }
        goto end;
    }

end:

    cert_ptr = NULL;
    cert_len = 0;
    rm_cert_ptr = NULL;

    if (buf) {
        rm_cert_free(buf);
        buf = NULL;
    }

    return err;
}

rm_cert_err_t RM_CERT_Delete(rm_cert_module_t module, rm_cert_type_t type)
{
    uint32_t flash_addr = 0x00;

    RM_CERT_INFO("module(%d), type(%d)\n", module, type);

    /* Check module */
    if (!rm_cert_is_valid_module(module)) {
        RM_CERT_ERR("Invaild module(%d)\n", module);
        return RM_CERT_ERR_INVALID_MODULE;
    }

    /* Check type */
    if (!rm_cert_is_valid_type(type)) {
        RM_CERT_ERR("Invaild type(%d)\n", type);
        return RM_CERT_ERR_INVALID_TYPE;
    }

    /* Get flash memory address */
    flash_addr = rm_cert_get_flash_address(module, type);
    if (!flash_addr) {
        RM_CERT_ERR("Failed to get flash memory(%d,%d)\n", module, type);
        return RM_CERT_ERR_INVALID_FLASH_ADDR;
    }

    return rm_cert_delete_by_addr(flash_addr);
}

static rm_cert_err_t rm_cert_delete_by_addr(uint32_t addr)
{
    rm_cert_err_t err = RM_CERT_ERR_OK;
    RM_CERT_INFO("flash memory address(0x%x)\n", (unsigned int)addr);

#ifdef RRQ61X_OSPI_W_ENABLED
    err = util_sflash_erase((int) addr, FLASH_WRITE_LENGTH);
#else
    err = ad_flash_erase_region(addr, FLASH_WRITE_LENGTH);
#endif /* RRQ61X_OSPI_W_ENABLED */
    
    if (err != true) {
        RM_CERT_ERR("Failed to erase data(0x%x)\n", (unsigned int)addr);
        err = RM_CERT_ERR_NOK;
    } else {
        err = RM_CERT_ERR_OK;
    }

    return err;
}

int RM_CERT_IsExistCert(rm_cert_module_t module, rm_cert_type_t type)
{
    int err = RM_CERT_ERR_OK;

    RM_CERT_INFO("module(%d), type(%d)\n", module, type);

    err = RM_CERT_Read(module, type, NULL, NULL, 0);
    if (err == RM_CERT_ERR_OK) {
        return pdTRUE;
    }

    return pdFALSE;
}
#endif

/* EOF */
