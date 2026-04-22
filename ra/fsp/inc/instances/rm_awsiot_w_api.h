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
 * @ingroup RENESAS_NETWORKING_INTERFACES
 * @defgroup AWSIOT_W_API AWSIOT_W Application (rm_awsiot_w)
 * @brief Interface for AWSIOT_W Application APIs.
 *
 * @{
 **********************************************************************************************************************/

#ifndef RM_AWSIOT_W_API_H
#define RM_AWSIOT_W_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"
#include "rm_awsiot_w_cfg.h"
/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER
 * macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef void awsiot_w_ctrl_t;
/** AWSIOT_W configuration parameters */
typedef struct st_rm_awsiot_w_app_cfg
{
    spi_flash_instance_t const * p_flash_instance;          ///< Pointer to flash(ospi) instance.
    void const * p_context;                                 ///< User defined context passed into callback function.
    void const * p_extend;                                  ///< Pointer to extended configuration by instance of interface.
} rm_awsiot_w_app_cfg_t;

typedef struct st_awsiot_w_api
{
    /** Initialize AWSIOT_W Application.
     *
     * @param[in, out] p_ctrl Pointer to user-provided control data for the
     * AWSIOT_W.
     * @param[in]      p_cfg  Pointer to AWSIOT_W configuration structure.
     *                            user.
     */
    fsp_err_t (* open)(awsiot_w_ctrl_t * const p_ctrl, rm_awsiot_w_app_cfg_t const * const p_cfg);

    /** Close AWSIOT_W Application.
     *
     * @param[in]   p_ctrl     Pointer to the AWSIOT_W control block.
     */
    fsp_err_t (* close)(awsiot_w_ctrl_t * const p_ctrl);


} awsiot_w_api_t;

/** This structure encompasses everything that is needed to use an instance of
 * this interface. */
typedef struct st_rm_awsiot_w_app_instance
{
    awsiot_w_ctrl_t      * p_ctrl;       ///< Pointer to the control structure for this instance
    rm_awsiot_w_app_cfg_t const * p_cfg; ///< Pointer to the configuration structure for this instance
    awsiot_w_api_t const * p_api;        ///< Pointer to the API structure for this instance
} rm_awsiot_w_app_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER
 * macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************/ /**
 * @} (end defgroup AWSIOT_W_API)
 **********************************************************************************************************************/
