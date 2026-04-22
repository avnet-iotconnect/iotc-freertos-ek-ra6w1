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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "rm_wifi.h"                  /* Socket and WiFi interface includes. */
#include "rm_awsiot_w.h"
#include "app_awsiot_w.h"
#include "iface_defs.h"
#include "common_def.h"
#include "net_common.h"

/***********************************************************************************************************************
 * Defines
 **********************************************************************************************************************/
#define AWSIOT_W_OPEN      (0X41575349ULL) //ascii: AWSI
#define AWSIOT_W_CLOSE     (0)

#define AWSIOT_W_TASK_NAME    "customer_awsiot_w"
#define AWSIOT_W_TASK_SIZE    512

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/


/***********************************************************************************************************************
 * Extern variables
 **********************************************************************************************************************/
extern unsigned int wait_supplicant_done(unsigned int timeout);

/***********************************************************************************************************************
 * Static Globals
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const awsiot_w_api_t g_rm_awsiot_w =
{
    .open              = RM_AWSIOT_W_Open,
    .close             = RM_AWSIOT_W_Close,
};
rm_awsiot_w_app_instance_ctrl_t const * gp_awsiot_w_app_instance;
/*******************************************************************************************************************//**
 *  Initialize the AWSIOT_W service.
 *
 * @param[in]  p_ctrl               Pointer to AWSIOT_W instance control structure.
 * @param[in]  p_cfg                Pointer to AWSIOT_W configuration structure.
 *
 * @retval FSP_SUCCESS              Function completed successfully.
 * @retval FSP_ERR_ALREADY_OPEN     AWSIOT_W instance is already opened.
 * @retval FSP_ERR_WIFI_FAILED      Error occurred with command to Wifi module.
 * @retval FSP_ERR_ASSERTION        The p_cfg instance is NULL.
 * @retval FSP_ERR_OUT_OF_MEMORY    Heap is too small or NULL to create a AWSIOT_W task.
 **********************************************************************************************************************/
fsp_err_t RM_AWSIOT_W_Open (awsiot_w_ctrl_t * const p_ctrl, rm_awsiot_w_app_cfg_t const * const p_cfg)
{
    fsp_err_t    err = FSP_SUCCESS;

    rm_awsiot_w_app_instance_ctrl_t * p_instance_ctrl = (rm_awsiot_w_app_instance_ctrl_t *) p_ctrl;
    gp_awsiot_w_app_instance = p_instance_ctrl;
    p_instance_ctrl->p_cfg        = p_cfg;

    if (0 == wait_supplicant_done(100))
    {
        err = FSP_ERR_WIFI_FAILED;
    }

    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    
    p_instance_ctrl->open = AWSIOT_W_OPEN;

    return err;
}

/*******************************************************************************************************************//**
 * Close the AWSIOT_W service.
 *
 * @param[in]  p_ctrl               Pointer to AWSIOT_W instance control structure.
 *
 * @retval FSP_SUCCESS              Function completed successfully.
 * @retval FSP_ERR_ASSERTION        The p_cfg instance is NULL.
 **********************************************************************************************************************/
fsp_err_t RM_AWSIOT_W_Close (awsiot_w_ctrl_t * const p_ctrl)
{
    rm_awsiot_w_app_instance_ctrl_t * p_instance_ctrl = (rm_awsiot_w_app_instance_ctrl_t *) p_ctrl;

    p_instance_ctrl->open = AWSIOT_W_CLOSE;

    return FSP_SUCCESS;
}
