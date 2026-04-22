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
 * @addtogroup AWSIOT_W
 * @{
 **********************************************************************************************************************/

#ifndef RM_AWSIOT_W_H
#define RM_AWSIOT_W_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
 #include "rm_wifi.h"
 #include "r_spi_flash_api.h"
 #include "rm_awsiot_w_api.h"
#include "rm_awsiot_w_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** AWSIOT_W control structure. DO NOT INITIALIZE. */
typedef struct st_awsiot_w_app_instance_ctrl
{
    rm_awsiot_w_app_cfg_t const * p_cfg;    ///< Pointer to initial configurations.
    uint32_t     open;                    ///< Indicates whether the open() API has been successfully called.
    void const * p_context;               ///< Placeholder for user data.
} rm_awsiot_w_app_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const rm_awsiot_w_app_instance_ctrl_t * gp_awsiot_w_app_instance;

extern rm_awsiot_w_app_instance_ctrl_t g_rm_awsiot_w_app_ctrl;
extern rm_awsiot_w_app_cfg_t g_rm_awsiot_w_app_cfg;
extern const awsiot_w_api_t         g_rm_awsiot_w;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t RM_AWSIOT_W_Open(awsiot_w_ctrl_t * const p_ctrl, rm_awsiot_w_app_cfg_t const * const p_cfg);
fsp_err_t RM_AWSIOT_W_Close(awsiot_w_ctrl_t * const p_ctrl);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end addtogroup AWSIOT_W)
 **********************************************************************************************************************/
