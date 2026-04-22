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

#include <errno.h>
#include <stdatomic.h>
#include "rm_comms_api.h"

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

static atomic_flag g_init_flag = ATOMIC_FLAG_INIT;

/***********************************************************************************************************************
 * Externs
 **********************************************************************************************************************/

extern rm_comms_instance_t g_comms_stdio_w;

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
int _write (int fd, char *ptr, int len);
int _read (int fd, char *ptr, int len);


int _write (int fd, char *ptr, int len)
{
    (void)fd;

    fsp_err_t err;

    if (!atomic_flag_test_and_set(&g_init_flag))
    {
        err = g_comms_stdio_w.p_api->open(g_comms_stdio_w.p_ctrl, g_comms_stdio_w.p_cfg);
        if (err != FSP_SUCCESS)
        {
            errno = EIO;
            return -1;
        }
    }

    err = g_comms_stdio_w.p_api->write(g_comms_stdio_w.p_ctrl, (uint8_t *)ptr, (uint32_t) len);
    if (err != FSP_SUCCESS)
    {
        errno = EIO;
        return -1;
    }
    return len;
}

int _read (int fd, char *ptr, int len)
{
    (void)fd;
    (void)len;

    fsp_err_t err;

    if (!atomic_flag_test_and_set(&g_init_flag))
    {
        err = g_comms_stdio_w.p_api->open(g_comms_stdio_w.p_ctrl, g_comms_stdio_w.p_cfg);
        if (err != FSP_SUCCESS)
        {
            errno = EIO;
            return -1;
        }
    }

    err = g_comms_stdio_w.p_api->read(g_comms_stdio_w.p_ctrl, (uint8_t *)ptr, 1);
    if (err != FSP_SUCCESS)
    {
        errno = EIO;
        return -1;
    }
    return 1;
}

