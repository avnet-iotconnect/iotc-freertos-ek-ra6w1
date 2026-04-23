/*
 * iotconnect.c
 *
 * IoTConnect (iotc-c-lib) integration helpers extracted from
 * app_dpm_sample.c. The MQTT context (xMqttContext) and the
 * application connection flag (xAppConnectionEstablished) remain
 * defined in app_dpm_sample.c and are accessed here via 'extern'
 * so that the same objects are shared.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "portable.h"
#include "projdefs.h"

#include "core_mqtt.h"
#include "transport_mbedtls_pkcs11.h"
#include "mqtt_pkcs11_demo_helpers.h"
#include "core_pkcs11_config.h"
#include "logging_levels.h"

#if defined(LIBRARY_LOG_LEVEL)
#undef LIBRARY_LOG_LEVEL
#endif
#if defined(LIBRARY_LOG_NAME)
#undef LIBRARY_LOG_NAME
#endif
#define LIBRARY_LOG_LEVEL  LOG_INFO
#define LIBRARY_LOG_NAME   "iotconnect"
#include "logging_stack.h"

#include "app_config.h"

/* iotc-c-lib */
#include "iotcl.h"
#include "iotcl_telemetry.h"

#include "iotconnect.h"

#define IOTC_TOPIC_MAX_LEN          128

/* Number of consecutive connection failures before forcing sleep. */
#ifndef IOTC_MAX_RETRY_CNT_TO_SLEEP
#define IOTC_MAX_RETRY_CNT_TO_SLEEP 3
#endif

/* Same NetworkContext layout as in app_dpm_sample.c, required so the
 * MQTTContext_t pointer can be used across translation units. */
struct NetworkContext
{
    TlsTransportParams_t *pxParams;
};

/* Shared state owned by app_dpm_sample.c */
extern MQTTContext_t        xMqttContext;
extern bool                 xAppConnectionEstablished;
extern NetworkContext_t     xNetworkContext;
extern TlsTransportParams_t xTlsTransportParams;
extern MQTTFixedBuffer_t    xBuffer;

/* Provided by app_sample_manager.c / app_dpm_thread.c */
extern uint8_t getFleetProvStatus(void);
extern void    goSleepAppOnException(uint32_t _retryCount);static void iotc_mqtt_send_cb(const char *topic, const char *json_str)
{
    BaseType_t xStatus = xPublishToTopic(&xMqttContext,
                                         topic,
                                         (int32_t)strlen(topic),
                                         json_str,
                                         (size_t)strlen(json_str));

    if (xStatus == pdPASS)
        printf("[publishJson] OK -> %s : %s\r\n", topic, json_str);
    else
        printf("[publishJson] FAIL -> %s\r\n", topic);
}

int iotc_init(void)
{
    IotclClientConfig cfg;
    iotcl_init_client_config(&cfg);

    cfg.device.duid          = IOTCONNECT_DUID;
    cfg.device.cpid          = "97FF86E8728645E9B89F7B07977E4B15";                  /* not used in CUSTOM */
    cfg.device.instance_type = IOTCL_DCT_CUSTOM;
    cfg.mqtt_send_cb         = iotc_mqtt_send_cb;

    int rc = iotcl_init(&cfg);
    if (rc != 0) {
        printf("iotcl_init failed: %d\n", rc);
        return rc;
    }

    /* Populate the global MqttConfig manually so iotcl_mqtt_send_telemetry()
     * has a topic to forward to our callback. */
    IotclMqttConfig *mc = iotcl_mqtt_get_config();
    if (mc != NULL) {
        /* These are owned by iotcl after assignment; we use static storage
         * via strdup-equivalent allocations from FreeRTOS heap. */
        //mc->host      = pvPortMalloc(strlen(IOTCONNECT_HOST) + 1);
        //mc->client_id = pvPortMalloc(strlen(IOTCONNECT_DUID) + 1);
        mc->pub_rpt   = pvPortMalloc(IOTC_TOPIC_MAX_LEN);
        mc->sub_c2d   = pvPortMalloc(IOTC_TOPIC_MAX_LEN);

        if (mc->pub_rpt && mc->sub_c2d) {
            //strcpy(mc->host,      IOTCONNECT_HOST);
            //strcpy(mc->client_id, IOTCONNECT_DUID);
            snprintf(mc->pub_rpt, IOTC_TOPIC_MAX_LEN,
                     "$aws/rules/msg_d2c_rpt/%s/2.1/0", IOTCONNECT_DUID);
            snprintf(mc->sub_c2d, IOTC_TOPIC_MAX_LEN,
                     "iot/%s/cmd", IOTCONNECT_DUID);

        } else {
            printf("MqttConfig allocation failed\n");
            return -1;
        }
    }
    return 0;
}

BaseType_t iotc_publish_telemetry(void)
{
    if (!xAppConnectionEstablished) {
        printf("publish_telemetry: not connected\n");
        return pdFAIL;
    }

    IotclMessageHandle msg = iotcl_telemetry_create();
    if (msg == NULL) {
        printf("iotcl_telemetry_create failed\n");
        return pdFAIL;
    }

    iotcl_telemetry_set_string(msg, "version", "1.0.0");
    iotcl_telemetry_set_number(msg, "random",  (double)(rand() % 100));

    /* Triggers iotc_mqtt_send_cb() -> xPublishToTopic() */
    int rc = iotcl_mqtt_send_telemetry(msg, false);
    iotcl_telemetry_destroy(msg);

    return (rc == 0) ? pdPASS : pdFAIL;
}

BaseType_t iotc_mqtt_connect(MQTTEventCallback_t eventCallback)
{
    BaseType_t      xStatus     = pdFAIL;
    static uint8_t  fail_count  = 0;

    LogInfo(("Establishing MQTT session with provisioned certificate..."));
    printf("Attempting connection with Thing name: %s\n", IOTCONNECT_DUID);
    printf("Endpoint: %s\n", IOTCONNECT_HOST);

RETRY_CONNECTION:
    if (xAppConnectionEstablished == true)
    {
        LogInfo(("Disconnecting MQTT session for reconnection..."));
        /* [[PCKS11 used or not */
        if (getFleetProvStatus())
        {
            xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        }
        else
        {
            xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        }
        /* ]] */
        xAppConnectionEstablished = false;
    }

    /* Set the pParams member of the network context with desired transport. */
    xNetworkContext.pxParams = &xTlsTransportParams;

    /* [[PCKS11 used or not */
    if (getFleetProvStatus())
    {
        xStatus = xEstablishMqttSession_P11(&xMqttContext, &xNetworkContext, &xBuffer, eventCallback,
                                            pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
                                            pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS);
    }
    else
    {
        xStatus = xEstablishMqttSession(&xMqttContext, &xNetworkContext, &xBuffer, eventCallback);
    }
    /* ]] */

    if (xStatus != pdTRUE)
    {
        LogError(("Failed to establish MQTT session with provisioned credentials. "));

        /* ToDo::exception process when infinite connection failure happened */
        if (getFleetProvStatus())
        {
            xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        }
        else
        {
            xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        }
        xAppConnectionEstablished = false;
        fail_count++;
        if (fail_count > IOTC_MAX_RETRY_CNT_TO_SLEEP)
        {
            goSleepAppOnException(fail_count);
        }
        goto RETRY_CONNECTION;
    }

    LogInfo(("Sucessfully established connection with provisioned credentials."));
    printf("MQTT connection succeeded.\n");
    xAppConnectionEstablished = true;
    fail_count = 0;

    return pdPASS;
}
