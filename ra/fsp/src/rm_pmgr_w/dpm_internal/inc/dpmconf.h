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

#ifndef __DPMCONF_H__
#define __DPMCONF_H__

#if CFG_WIFI
/**
 * @file     dpmconf.h
 * @brief    DPM configuration
 * @details  This file defines the DPM configuration.
 *           This header file is used in RTOS & PTIM.
 */

/* Simulation */
/*#define TIM_SIMULATION*/ /* TIM_SIMULATION is defined in vcxproj of Visual Studio. */
#ifdef TIM_SIMULATION
#define DPM_RTC_CLK_SPEED_EN
#define DPM_RTC_CLK_SPEED 0
#endif

#undef  DPM_APTRK_CCA_MEASURE /* for SLR */
#define TIM_XTAL_CHANGE_EN /* TIM changes OSC into Xtal 32K */
#undef DPM_RTM_AUTO_CLEAR /* PTIM TEST FEATURE */

//#define NX_MDM_VER              20

//#define PTIM_SYS_CLK		80
//#define PTIM_MAC_CLK		40

/**
 * DEBUG FEATURE
 */
#undef DPM_TEST_ALWAYS_HI_CURRENT_CONTROL
#undef DPM_TEST_PBR_EVT_OFF
#undef DPM_TEST_FADC_V_CONTROL

/**
 * ##### RA6W1-20190113-LOW_CURRENT_MODE
 * 	DPM_TEST_ALWAYS_HI_CURRENT_CONTROL
 *	DPM_TEST_PBR_EVT_OFF
 *	DPM_TEST_FADC_V_CONTROL
 **/
#endif /*CFG_WIFI*/
#endif
