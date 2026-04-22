/**
 ****************************************************************************************
 *
 * @file sdk_crc16.h
 *
 * @brief CRC16 calculation library API
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

#ifndef SDK_CRC16_
#define SDK_CRC16_

#include <stddef.h>
#include <stdint.h>

/*
 * \brief Initialize CRC16
 *
 * \param [in] crc16 pointer to CRC16 value
 *
 */
void crc16_init(uint16_t *crc16);

/*
 * \brief Updates CRC16
 *
 * \param [in] crc16 pointer to CRC16 value
 * \param [in] buf data buffer
 * \param [in] len length of data
 *
 */
void crc16_update(uint16_t *crc16, const uint8_t *buf, size_t len);

/*
 * \brief Calculate CRC16
 *
 * \param [in] buf data buffer
 * \param [in] len length of data
 *
 * \return CRC16 value
 *
 */
uint16_t crc16_calculate(const uint8_t *buf, size_t len);

#endif /* SDK_CRC16_ */
