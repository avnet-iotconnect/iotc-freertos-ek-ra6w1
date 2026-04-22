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

#ifndef RM_WIFI_EVENT_H
#define RM_WIFI_EVENT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rm_wifi_api.h"
#include "rm_wifi.h"
#include "osal.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef void (*rm_wifi_event_handler_t)(void *);

/**********************************************************************************************************************
 * Function declarations
 **********************************************************************************************************************/
fsp_err_t rm_wifi_event_register(int event_type, rm_wifi_event_handler_t handler);
void rm_wifi_event_handler_init(void);
void rm_wifi_event_handler_deinit(void);
void rm_wifi_event_signal_connected(u8 *ssid, u8 ssid_len, u8 *bssid, int security, u8 chan_info);
void rm_wifi_event_signal_disconnected(u16 reason);
void rm_wifi_event_signal_ap_sta_disconnected(u16 reason, u8 *ucMac);
void rm_wifi_event_signal_twt(WIFI_TWTEvent_t twt_event);

#endif // RM_WIFI_EVENT_H
