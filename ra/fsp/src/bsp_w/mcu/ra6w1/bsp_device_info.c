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
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

#ifdef UNIT_TESTING
 #include "fake_regs.h"
#endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define ASCII_3095    (('3' << 24) | ('0' << 16) | ('9' << 8) | '5') /* 0x33303935 = '3' '0' '9' '5' */

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

TESTABLE_STATIC uint32_t bsp_prv_chip_revision;

/*******************************************************************************************************************//**
 * Check whether we are running on RA6W1 revision A.
 *
 * @retval true if we are running on RA6W1 revision A, false otherwise.
 **********************************************************************************************************************/
bool bsp_is_chip_revision_a (void)
{
    return bsp_prv_chip_revision == 0;
}

/*******************************************************************************************************************//**
 * Initialize chip revision and check whether we are running on RA6W1 chip.
 *
 * @retval true if we are running on RA6W1 chip, false otherwise.
 **********************************************************************************************************************/
bool bsp_device_info_init (void)
{
    union
    {
        uint8_t  arr[4];
        uint32_t value;
    } device_chip_id;

    /* Initial chip revision is 'A'. Subtract 'A' to get numeric value. */
    bsp_prv_chip_revision = CHIP_VERSION->CHIP_REVISION_REG_b.CHIP_REVISION - 'A';

    device_chip_id.arr[3] = CHIP_VERSION->CHIP_ID1_REG_b.CHIP_ID1;
    device_chip_id.arr[2] = CHIP_VERSION->CHIP_ID2_REG_b.CHIP_ID2;
    device_chip_id.arr[1] = CHIP_VERSION->CHIP_ID3_REG_b.CHIP_ID3;
    device_chip_id.arr[0] = CHIP_VERSION->CHIP_ID4_REG_b.CHIP_ID4;

    return device_chip_id.value == ASCII_3095;
}

/** @} (end addtogroup BSP_MCU_PRV) */
