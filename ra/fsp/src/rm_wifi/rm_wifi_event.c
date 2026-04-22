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
#include <stdlib.h>
#include "rm_wifi_event.h"
#include "rm_wifi_api.h"
#include "rm_wifi.h"
#include "osal.h"

/***********************************************************************************************************************
 * Externs
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Enumerations
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Defines
 **********************************************************************************************************************/
#define WIFI_EVENT_THREAD_NAME          "wifi_evt_hdlr"
#define	WIFI_EVENT_THREAD_STACK_SIZE    1024 * OS_STACK_WORD_SIZE
#define WIFI_EVENT_THREAD_PRIORITY      OS_TASK_PRIORITY_NORMAL
#define	WIFI_EVENT_QUEUE_SIZE           64

#define WIFI_EVENT_HANDLER_MAX          eWiFiEventExtMax
#define WIFI_EVENT_BIT(event)           ((1) << (event))
#define WIFI_EVENT_MASK                 (WIFI_EVENT_BIT(eWiFiEventConnected) | \
                                         WIFI_EVENT_BIT(eWiFiEventDisconnected) | \
                                         WIFI_EVENT_BIT(eWiFiEventAPStationDisconnected) | \
                                         WIFI_EVENT_BIT(eWiFiEventExtTWT))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef union {
    WIFIEvent_t     evt;
    WIFIEventExt_t  evt_ext;
} rm_wifi_event_t;

/***********************************************************************************************************************
 * Static Globals
 **********************************************************************************************************************/
static rm_wifi_event_handler_t wifi_event_handlers[WIFI_EVENT_HANDLER_MAX] = { NULL };
static OS_QUEUE wifi_event_queue = NULL;

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
OS_TASK wifi_event_thread = NULL;

/***********************************************************************************************************************
 * Function Definitions
 **********************************************************************************************************************/
static void rm_wifi_event_handler_thread(void *params)
{
    FSP_PARAMETER_NOT_USED(params);
    OS_BASE_TYPE status;
    int event_type;
    rm_wifi_event_t *p_wifi_event = NULL;

    while (1)
    {
        status = OS_QUEUE_GET(wifi_event_queue, &p_wifi_event, OS_QUEUE_FOREVER);

        if (status != OS_QUEUE_OK)
        {
            break;
        }

        event_type = p_wifi_event->evt.xEventType;

        if (wifi_event_handlers[event_type])
        {
            wifi_event_handlers[event_type]((WIFIEvent_t *)p_wifi_event);
        }

        OS_FREE_FUNC(p_wifi_event);
    }
}

static rm_wifi_event_t *rm_wifi_event_alloc(int event_type)
{
    rm_wifi_event_t *p_wifi_event = NULL;

    p_wifi_event = OS_MALLOC(sizeof(*p_wifi_event));

    if (p_wifi_event)
    {
        memset(p_wifi_event, 0, sizeof(*p_wifi_event));
        p_wifi_event->evt.xEventType = event_type;
    }
    else
    {
        printf("%s:%d wifi event alloc fail! (event_type %d)\n", __func__, __LINE__, event_type);
    }

    return p_wifi_event;
}

static OS_BASE_TYPE rm_wifi_event_signal(rm_wifi_event_t *p_wifi_event)
{
    OS_BASE_TYPE status;

    OS_ASSERT(p_wifi_event != NULL);

    status = OS_QUEUE_PUT(wifi_event_queue, &p_wifi_event, OS_QUEUE_NO_WAIT);

    if (status != OS_QUEUE_OK)
    {
        printf("%s:%d Error: wifi_event_queue full!\n", __func__, __LINE__);
    }

    return status;
}

fsp_err_t rm_wifi_event_register(int event_type, rm_wifi_event_handler_t handler)
{
    if (event_type >= WIFI_EVENT_HANDLER_MAX)
    {
        return FSP_ERR_UNSUPPORTED;
    }

    if (!(WIFI_EVENT_BIT(event_type) & WIFI_EVENT_MASK))
    {
        return FSP_ERR_UNSUPPORTED;
    }

    wifi_event_handlers[event_type] = handler;

    return FSP_SUCCESS;
}

void rm_wifi_event_handler_init(void)
{
    OS_BASE_TYPE status;

    OS_QUEUE_CREATE(wifi_event_queue, sizeof(rm_wifi_event_t *), WIFI_EVENT_QUEUE_SIZE);
    OS_ASSERT(wifi_event_queue != NULL);

    status = OS_TASK_CREATE(WIFI_EVENT_THREAD_NAME,
                            rm_wifi_event_handler_thread,
                            (void *)NULL,
                            WIFI_EVENT_THREAD_STACK_SIZE,
                            WIFI_EVENT_THREAD_PRIORITY,
                            wifi_event_thread);
    OS_ASSERT(status == OS_TASK_CREATE_SUCCESS);
}

void rm_wifi_event_handler_deinit(void)
{
    if (wifi_event_thread)
    {
        OS_TASK_DELETE(wifi_event_thread);
    }

    if (wifi_event_queue)
    {
        OS_QUEUE_DELETE(wifi_event_queue);
    }
}

void rm_wifi_event_signal_connected(u8 *ssid, u8 ssid_len, u8 *bssid, int security, u8 chan_info)
{
    WIFIEvent_t *evt = (WIFIEvent_t *)rm_wifi_event_alloc(eWiFiEventConnected);
    int ssid_copy_len = MIN(ssid_len, wificonfigMAX_SSID_LEN);

    if (!evt)
    {
        return;
    }

    memcpy(evt->xInfo.xConnected.xConnectionInfo.ucSSID, ssid, ssid_copy_len);
    evt->xInfo.xConnected.xConnectionInfo.ucSSIDLength = ssid_len;
    memcpy(evt->xInfo.xConnected.xConnectionInfo.ucBSSID, bssid, wificonfigMAX_BSSID_LEN);
    evt->xInfo.xConnected.xConnectionInfo.xSecurity = security;
    evt->xInfo.xConnected.xConnectionInfo.ucChannel = chan_info;

    rm_wifi_event_signal((rm_wifi_event_t *)evt);
}

void rm_wifi_event_signal_disconnected(u16 reason)
{
    WIFIEvent_t *evt = (WIFIEvent_t *)rm_wifi_event_alloc(eWiFiEventDisconnected);

    if (!evt)
    {
        return;
    }

    evt->xInfo.xDisconnected.xReason = reason;

    rm_wifi_event_signal((rm_wifi_event_t *)evt);
}

void rm_wifi_event_signal_ap_sta_disconnected(u16 reason, u8 *ucMac)
{
    WIFIEvent_t *evt = (WIFIEvent_t *)rm_wifi_event_alloc(eWiFiEventAPStationDisconnected);

    if (!evt)
    {
        return;
    }

    evt->xInfo.xAPStationDisconnected.xReason = reason;
    memcpy(evt->xInfo.xAPStationDisconnected.ucMac, ucMac, wificonfigMAX_BSSID_LEN);
    rm_wifi_event_signal((rm_wifi_event_t *) evt);
}

void rm_wifi_event_signal_twt(WIFI_TWTEvent_t twt_event)
{
    WIFIEventExt_t *evt = (WIFIEventExt_t *)rm_wifi_event_alloc(eWiFiEventExtTWT);

    if (!evt)
    {
        return;
    }

    evt->xInfo.xTWT.xEvent = twt_event;

    rm_wifi_event_signal((rm_wifi_event_t *)evt);
}
