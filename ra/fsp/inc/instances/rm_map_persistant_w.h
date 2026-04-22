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

/******************************************************************************************************************//**
 * @addtogroup RM_MAP_PERSISTANT_W
 * @{
 *********************************************************************************************************************/

#ifndef RM_MAP_PERSISTANT_W_H
#define RM_MAP_PERSISTANT_W_H

#include "bsp_api.h"
#include "rm_map_persistant_w_api.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** "MAP" in ASCII, used to determine if the RM_MAP_PERSISTANT_W is open. */
#define MAP_PERSISTANT_W_OPEN (0x4D4150ULL)
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** RM_MAP_PERSISTANT_W private control block. DO NOT MODIFY. */
typedef struct st_map_persistant_w_instance_ctrl
{
    uint32_t map_persistant_w_open;                ///< Indicates whether the open() API has been successfully called.
    SemaphoreHandle_t map_persistant_w_mutex;      ///< Semaphore to handle synchronization.    
} map_persistant_w_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const map_persistant_w_api_t g_map_persistant_w_api;

/** @endcond */
map_persistant_w_instance_ctrl_t * RM_MAP_PERSISTANT_W_get_ctrl(void);
/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t RM_MAP_PERSISTANT_W_Open(map_persistant_w_ctrl_t * const p_ctrl);

fsp_err_t RM_MAP_PERSISTANT_W_Close(map_persistant_w_ctrl_t * const p_ctrl);

fsp_err_t RM_MAP_PERSISTANT_W_Read_UINT(map_persistant_w_ctrl_t * const p_ctrl,
                                      const char* group, const char *name, void *val,
                                      uint16_t  *data_length);

fsp_err_t RM_MAP_PERSISTANT_W_Write_UINT(map_persistant_w_ctrl_t * const p_ctrl,
                                       const char* group, const char *name, void *val,
                                       uint32_t len);

fsp_err_t RM_MAP_PERSISTANT_W_Read_INT(map_persistant_w_ctrl_t * const p_ctrl,
                                     const char* group, const char *name, int *val);

fsp_err_t RM_MAP_PERSISTANT_W_Write_INT(map_persistant_w_ctrl_t * const p_ctrl,
                                      const char* group, const char *name, int val);

fsp_err_t RM_MAP_PERSISTANT_W_Read_STRING(map_persistant_w_ctrl_t * const p_ctrl,
                                        const char* group, const char *name, char **val);

fsp_err_t RM_MAP_PERSISTANT_W_Write_STRING(map_persistant_w_ctrl_t * const p_ctrl,
                                         const char* group, const char *name, const char *val);

fsp_err_t RM_MAP_PERSISTANT_W_Write_Auto(map_persistant_w_ctrl_t * const p_ctrl,
                                         const char* group, const char *name, const char *val);

fsp_err_t RM_MAP_PERSISTANT_W_Erase(map_persistant_w_ctrl_t * const p_ctrl,
                                  const char* group, const char *name);

fsp_err_t RM_MAP_PERSISTANT_W_Erase_GROUP(map_persistant_w_ctrl_t * const p_ctrl,
                                        const char* group);

fsp_err_t RM_MAP_PERSISTANT_W_reflash(map_persistant_w_ctrl_t * const p_api_ctrl);

fsp_err_t RM_MAP_PERSISTANT_W_Display(map_persistant_w_ctrl_t * const p_api_ctrl,
                                    const char *group, int printall);

fsp_err_t RM_MAP_PERSISTANT_W_Read_Auto(map_persistant_w_ctrl_t * const p_api_ctrl,
                                 const char *group,
                                 const char *name,
                                 int8_t *data_length, uint8_t **data_ptr);

fsp_err_t RM_MAP_PERSISTANT_W_Read_BIN(map_persistant_w_ctrl_t * const p_api_ctrl,
                                         	 const char* group, uint32_t id_offset, uint8_t ** value);

fsp_err_t  RM_MAP_PERSISTANT_W_Write_BIN(map_persistant_w_ctrl_t * const p_api_ctrl,
                                         	   const char* group, uint32_t id_offset, uint8_t * value);                                 
/*******************************************************************************************************************//**
 * @} (end addtogroup RM_MAP_PERSISTANT_W)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 // RM_MAP_PERSISTANT_W_H
