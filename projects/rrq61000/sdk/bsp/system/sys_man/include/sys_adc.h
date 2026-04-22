/**
 * \addtogroup MID_SYS_SERVICES
 * \{
 * \addtogroup SYS_ADC Analog Digital Converter Service
 *
 * \brief Functions for ADC Service
 *
 * \{
 */

/**
 ****************************************************************************************
 *
 * @file sys_adc.h
 *
 * @brief sys_adc header file.
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

#if ((DEVICE_FAMILY == DA1469X) || (DEVICE_FAMILY == DA1487X))
#if (dg_configUSE_SYS_ADC == 1)

#ifndef SYS_ADC_H_
#define SYS_ADC_H_

#include "ad_gpadc.h"
#include "ad_snc.h"
#include "snc_hw_sys.h"
///@cond INTERNAL
#include "../adapters/src/sys_platform_devices_internal.h"
///@endcond

/**
 * \brief Monitor parameters configuration
 *
 */
typedef struct {
        uint16_t        drift;          /**<  the drift to be monitored */
        uint16_t        pollIntervalms; /**<  time period for the GPADC uCode-Block execution */
} sys_adc_monitor_par_t;

/**
 * \brief Function that configures the GPADC device, measures the current value and
 *      calculates the initial upper and lower bounds
 *
 * \param [in] conf            the GPADC device controller to be configured
 * \param [in] _gpadc_cb        a callback function that should be called
 *                              whenever the GPADC uCode-Block notifies CM33
 * \param [in] monitor_par      pointer to monitor par struct
 */
void sys_adc_config(const ad_gpadc_controller_conf_t * conf, ad_snc_interrupt_cb _gpadc_cb, sys_adc_monitor_par_t* monitor_par);

/**
 * \brief Function that enables the GPADC uCode-Block
 *
 */
void sys_adc_enable();

/**
 * \brief Function that disables the GPADC uCode-Block
 *
 */
void sys_adc_disable();

/**
 * \brief Function that returns the GPADC measurement
 *
 * \return uint32_t     the GPADC raw value
 *
 */
uint32_t sys_adc_get_value();

/**
 * \brief Function that sets the upper, lower bounds
 *
 * \param [in] value    the value based on which the lower and upper bounds
 *                      are defined
 */
void sys_adc_set_bounds(uint16_t value);

/**
 * \brief uCode-Block that reads measurements from the on-chip radio temperature sensor and notifies M33
 *
 */
SNC_UCODE_BLOCK_DECL(ucode_gpadc_rf_temp_notify);


#endif /* SYS_ADC_H_ */

#endif /* (dg_configUSE_SYS_ADC == 1) */
#endif /* DEVICE_FAMILY */

/**
 \}
 \}
 */
