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
#include <stdlib.h>
#include <stdint.h>

#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define PRNG_PAR_TYPE_32_BITS    (2U)

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Implementation of srand() function from standard C library.
 *
 * @param[in] seed    A seed value to use for a new sequence of pseudo-random integers to be returned by rand(). These
 *                    sequences are repeatable by calling srand() with the same seed value.
 *
 * @note RA6W1 PRNG is using polynomial calculation, which is similar to CRC. Providing the same seed to PRNG after POR
 *       boot (not SW reset), results in PRNG generating a deterministic sequence (the same pattern). However PRNG seed
 *       is designed to accumulate the current calculation without resetting the poly calculation. Hence to implement
 *       proper standard C library srand() semantics, need to use hardware CRC/SHA/AES instead of PRNG.
 **********************************************************************************************************************/
void srand (unsigned seed)
{
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;
    HW_ACC->PRNG_OP_EN_REG    = 1U;
    HW_ACC->PRNG_PAR_TYPE_REG = PRNG_PAR_TYPE_32_BITS;
    HW_ACC->PRNG_SEED_REG     = (uint32_t) seed & HW_ACC_PRNG_SEED_REG_PRNG_SEED_VAL_Msk;
    HW_ACC->PRNG_REQ_CLR_REG  = HW_ACC_PRNG_REQ_CLR_REG_PRNG_REQ_CLR_Msk;
    HW_ACC->PRNG_OP_EN_REG    = 0U;
    FSP_CRITICAL_SECTION_EXIT;
}

/*******************************************************************************************************************//**
 * Implementation of rand() function from standard C library.
 *
 * @retval a pseudo-random integer in the range 0 to RAND_MAX inclusive.
 * @note This function is not reentrant.
 **********************************************************************************************************************/
int rand (void)
{
    uint32_t old_value, new_value;
    FSP_CRITICAL_SECTION_DEFINE;

    FSP_CRITICAL_SECTION_ENTER;
    HW_ACC->PRNG_OP_EN_REG = 1U;
    old_value              = HW_ACC->PRNG_REG_CAL_VAL;

    do
    {
        HW_ACC->PRNG_REQ_CLR_REG = HW_ACC_PRNG_REQ_CLR_REG_PRNG_REQ_GET_Msk;
        new_value                = HW_ACC->PRNG_REG_CAL_VAL;
        if ((new_value == 0) && (old_value == 0))
        {
            /* Sometimes we hang indefinitely in this do-while loop (new_value is always zero, just as old_value).
             * It is reproducible without critical sections for a sequence like:
             *
             * srand(rand());
             * rand();
             *
             * It seems to be a HW design issue. This workaround solves it as far as I can test.
             * PRNG has not been revised, so it should behave in the same way for both AA and BA hardware revisions.
             * Note: adding busy-wait loops before/after rand()/srand() doesn't seem to help..
             * TODO:
             * - How can we confirm that previous PRNG operation is finished and we can issue next one?
             */
            HW_ACC->PRNG_REQ_CLR_REG  = HW_ACC_PRNG_REQ_CLR_REG_PRNG_REQ_CLR_Msk;
        }
    } while (new_value == old_value);

    HW_ACC->PRNG_OP_EN_REG = 0;
    FSP_CRITICAL_SECTION_EXIT;

    return (int) (new_value % (((uint32_t) RAND_MAX) + 1U));
}
