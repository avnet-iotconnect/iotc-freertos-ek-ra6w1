/**
 * \addtogroup MID_SYS_SERVICES
 * \{
 * \addtogroup SYS_BSR Busy Status Register Driver Service
 * \{
 * \brief Busy Status Register (BSR) driver
 */

/**
 ****************************************************************************************
 *
 * @file sys_bsr.h
 *
 * @brief Busy Status Register (BSR) driver file.
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

#ifndef SYS_BSR_H_
#define SYS_BSR_H_

#if ((DEVICE_FAMILY == DA1469X) || (DEVICE_FAMILY == DA1487X))

#include "hw_sys.h"

/**
 * \brief Initialize the BSR module.
 */
void sys_sw_bsr_init(void);

/**
 * \brief Acquire exclusive access to a specific peripheral when
 *        it is also used by other masters (SNC or CMAC). This function
 *        will block until access is granted.
 *
 * \param [in] sw_bsr_master_id The SW BSR ID of the relevant master
 * \param [in] periph_id The peripheral id for which exclusive access must be granted.
 *                       Valid range is (0 - 15). Check HW_SYS_BSR_PERIPH_ID.
 */
#if dg_configUSE_HW_SYS
void sys_sw_bsr_acquire(SW_BSR_MASTER_ID sw_bsr_master_id, uint32_t periph_id);

/**
 * \brief Checks if exclusive access to a specific peripheral has been acquired
 *        from a given master.
 *
 * \param [in] sw_bsr_master_id The SW BSR ID of the relevant master
 * \param [in] periph_id The peripheral id for which exclusive access will be checked.
 *                       Valid range is (0 - BSR_PERIPH_ID_MAX). Check HW_SYS_BSR_PERIPH_ID.
 * \return true if peripheral exclusive access has been acquired from the specific master, else false.
 */
bool sys_sw_bsr_acquired(SW_BSR_MASTER_ID sw_bsr_master_id, uint32_t periph_id);

/**
 * \brief Releases the exclusive access from a specific peripheral so it
 *        can be also used by other masters (SNC or CMAC).
 *
 * \param [in] sw_bsr_master_id The SW BSR ID of the relevant master
 * \param [in] periph_id The peripheral id for which exclusive access must be released.
 *                       Valid range is (0 - 15). Check HW_SYS_BSR_PERIPH_ID.
 */
void sys_sw_bsr_release(SW_BSR_MASTER_ID sw_bsr_master_id, uint32_t periph_id);
#endif
#endif /* DA1469X */

#endif /* SYS_BSR_H_ */

/**
 \}
 \}
 */
