/*
 * iotconnect.c
 *
 * IoTConnect (iotc-c-lib) integration helpers extracted from
 * app_dpm_sample.c.
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
#include "iotcl_dra_discovery.h"
#include "iotcl_dra_identity.h"

#include "iotc_https.h"
#include "iotconnect.h"

#define IOTC_TOPIC_MAX_LEN          128

#ifndef IOTC_MQTT_CONNACK_TIMEOUT_MS
#define IOTC_MQTT_CONNACK_TIMEOUT_MS         (2000U)
#endif

#ifndef IOTC_MQTT_TRANSPORT_TIMEOUT_MS
#define IOTC_MQTT_TRANSPORT_TIMEOUT_MS       AWS_IOT_RX_NO_CONNECT_TIMEOUT
#endif

#ifndef IOTC_MQTT_KEEP_ALIVE_SECONDS
#define IOTC_MQTT_KEEP_ALIVE_SECONDS         (60U)
#endif

#ifndef IOTC_MQTT_BROKER_PORT
#define IOTC_MQTT_BROKER_PORT                AWS_IOT_MQTT_PORT
#endif

#ifndef IOTC_MQTT_RUNTIME_BROKER_PORT
#define IOTC_MQTT_RUNTIME_BROKER_PORT        IOTC_MQTT_BROKER_PORT
#endif

#ifndef IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS
#define IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS     (60U)
#endif

#ifndef IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS
#define IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS     (1200U)
#endif

#ifndef IOTC_MQTT_RUNTIME_KEEP_ALIVE_SECONDS
#define IOTC_MQTT_RUNTIME_KEEP_ALIVE_SECONDS IOTC_MQTT_KEEP_ALIVE_SECONDS
#endif

#ifndef IOTC_MQTT_OUTGOING_RECORD_COUNT
#define IOTC_MQTT_OUTGOING_RECORD_COUNT      (15U)
#endif

#ifndef IOTC_MQTT_INCOMING_RECORD_COUNT
#define IOTC_MQTT_INCOMING_RECORD_COUNT      (15U)
#endif

#ifndef NETWORK_BUFFER_SIZE
#define NETWORK_BUFFER_SIZE                  (4096U)
#endif

/* Default AWS discovery endpoint for IoTConnect DRA. */
#ifndef IOTC_DISCOVERY_SERVER
#define IOTC_DISCOVERY_SERVER        IOTCL_DRA_DEFAULT_DISCOVERY_HOST_AWS
#endif

/* Number of consecutive connection failures before forcing sleep. */
#ifndef IOTC_MAX_RETRY_CNT_TO_SLEEP
#define IOTC_MAX_RETRY_CNT_TO_SLEEP 3
#endif

#ifndef IOTCONNECT_DEFAULT_CPID
#define IOTCONNECT_DEFAULT_CPID "97FF86E8728645E9B89F7B07977E4B15"
#endif

#ifndef IOTCONNECT_SDK_PUBLISH_COUNT
#define IOTCONNECT_SDK_PUBLISH_COUNT 100U
#endif

#ifndef IOTCONNECT_SDK_PUBLISH_DELAY_MS
#define IOTCONNECT_SDK_PUBLISH_DELAY_MS 2000U
#endif

/* Same NetworkContext layout as in app_dpm_sample.c, required so the
 * MQTTContext_t pointer can be used across translation units. */
struct NetworkContext
{
    TlsTransportParams_t *pxParams;
};

static MQTTContext_t        xMqttContext;
static bool                 xAppConnectionEstablished = false;
static NetworkContext_t     xNetworkContext;
static TlsTransportParams_t xTlsTransportParams;
static uint8_t              ucSharedBuffer[NETWORK_BUFFER_SIZE];
static MQTTFixedBuffer_t    xBuffer = { ucSharedBuffer, NETWORK_BUFFER_SIZE };

static uint32_t       ulIotcGlobalEntryTimeMs;
static MQTTPubAckInfo_t xIotcOutgoingPublishRecords[IOTC_MQTT_OUTGOING_RECORD_COUNT];
static MQTTPubAckInfo_t xIotcIncomingPublishRecords[IOTC_MQTT_INCOMING_RECORD_COUNT];

static void iotc_mqtt_send_cb(const char *topic, const char *json_str);

static int prvCopyMqttString(char *pcDestination,
                             size_t xDestinationSize,
                             const char *pcSource,
                             const char *pcFieldName)
{
    int lWritten;

    (void) pcFieldName;

    if ((pcDestination == NULL) || (pcSource == NULL) || (xDestinationSize == 0U))
    {
        printf("Invalid MQTT output buffer for %s.\n", pcFieldName);
        return -1;
    }

    lWritten = snprintf(pcDestination, xDestinationSize, "%s", pcSource);
    if ((lWritten < 0) || ((size_t) lWritten >= xDestinationSize))
    {
        printf("MQTT %s buffer is too small.\n", pcFieldName);
        return -1;
    }

    return IOTCL_SUCCESS;
}

static void prvIotconnectSdkDeinit(void)
{
    if (xAppConnectionEstablished == true)
    {
        xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        xAppConnectionEstablished = false;
    }

    iotcl_deinit();
}

static int prvIotcInitWithConfig(const char *pcDuid, const char *pcCpid)
{
    IotclClientConfig cfg;

    if ((pcDuid == NULL) || (pcCpid == NULL))
    {
        return IOTCL_ERR_MISSING_VALUE;
    }

    iotcl_init_client_config(&cfg);

    cfg.device.duid          = pcDuid;
    cfg.device.cpid          = pcCpid;
    cfg.device.instance_type = IOTCL_DCT_CUSTOM;
    cfg.mqtt_send_cb         = iotc_mqtt_send_cb;

    int rc = iotcl_init(&cfg);
    if (rc != 0)
    {
        printf("iotcl_init failed: %d\n", rc);
        return rc;
    }

    return IOTCL_SUCCESS;
}

static uint32_t prvIotcGetTimeMs(void)
{
    uint32_t ulTimeMs = pdTICKS_TO_MS(xTaskGetTickCount());

    return (uint32_t)(ulTimeMs - ulIotcGlobalEntryTimeMs);
}

static BaseType_t prvEstablishMqttSession(MQTTContext_t *pxMqttContext,
                                          NetworkContext_t *pxNetworkContext,
                                          MQTTFixedBuffer_t *pxNetworkBuffer,
                                          MQTTEventCallback_t eventCallback)
{
    BaseType_t           xReturnStatus = pdTRUE;
    TlsTransportStatus_t xNetworkStatus;
    MQTTStatus_t         xMQTTStatus;
    MQTTConnectInfo_t    xConnectInfo;
    TransportInterface_t xTransport;
    NetworkCredentials_t xNetworkCredentials = { 0 };
    IotclMqttConfig     *pxIotcMqttConfig = iotcl_mqtt_get_config();
    const char          *pcBrokerHost = "";
    const char          *pcClientId = "";
    bool                 xSessionPresent = false;
    UINT16               usKeepAlive = IOTC_MQTT_KEEP_ALIVE_SECONDS;

#if defined(democonfigCLIENT_USERNAME)
    static const char *ppcAlpnProtocols[] = { "mqtt", NULL };
#else
    static const char *ppcAlpnProtocols[] = { "x-amzn-mqtt-ca", NULL };
#endif

    configASSERT(pxMqttContext != NULL);
    configASSERT(pxNetworkContext != NULL);
    configASSERT(pxNetworkBuffer != NULL);

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->host != NULL))
    {
        pcBrokerHost = pxIotcMqttConfig->host;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->client_id != NULL))
    {
        pcClientId = pxIotcMqttConfig->client_id;
    }

    (void)memset(pxMqttContext, 0, sizeof(*pxMqttContext));
    (void)memset(xIotcOutgoingPublishRecords, 0, sizeof(xIotcOutgoingPublishRecords));
    (void)memset(xIotcIncomingPublishRecords, 0, sizeof(xIotcIncomingPublishRecords));

#if IOTC_MQTT_BROKER_PORT == 443U
    xNetworkCredentials.pAlpnProtos = ppcAlpnProtocols;
#else
    xNetworkCredentials.pAlpnProtos = NULL;
#endif
    xNetworkCredentials.disableSni = pdFALSE;
    xNetworkCredentials.pRootCa = (const unsigned char *)IOTCONNECT_ROOTCA;
    xNetworkCredentials.rootCaSize = strlen(IOTCONNECT_ROOTCA) + 1U;
    xNetworkCredentials.pClientCert = (const unsigned char *)IOTCONNECT_DEVICE_CERT;
    xNetworkCredentials.clientCertSize = strlen(IOTCONNECT_DEVICE_CERT) + 1U;
    xNetworkCredentials.pPrivateKey = (const unsigned char *)IOTCONNECT_DEVICE_KEY;
    xNetworkCredentials.privateKeySize = strlen(IOTCONNECT_DEVICE_KEY) + 1U;

        printf("Create a TCP connection to %s:%lu.\n",
            pcBrokerHost,
            (unsigned long)IOTC_MQTT_RUNTIME_BROKER_PORT);

    xNetworkStatus = TLS_FreeRTOS_Connect(pxNetworkContext,
                                          pcBrokerHost,
                                          (uint16_t)IOTC_MQTT_RUNTIME_BROKER_PORT,
                                          &xNetworkCredentials,
                                          IOTC_MQTT_TRANSPORT_TIMEOUT_MS,
                                          IOTC_MQTT_TRANSPORT_TIMEOUT_MS);
    if (xNetworkStatus != TLS_TRANSPORT_SUCCESS)
    {
        printf("Failed to connect to MQTT broker %s.\n", pcBrokerHost);
        return pdFAIL;
    }

    xTransport.pNetworkContext = pxNetworkContext;
    xTransport.send = TLS_FreeRTOS_send;
    xTransport.recv = TLS_FreeRTOS_recv;
    xTransport.writev = NULL;

    ulIotcGlobalEntryTimeMs = pdTICKS_TO_MS(xTaskGetTickCount());

    xMQTTStatus = MQTT_Init(pxMqttContext,
                            &xTransport,
                            prvIotcGetTimeMs,
                            eventCallback,
                            pxNetworkBuffer);
    if (xMQTTStatus != MQTTSuccess)
    {
        printf("MQTT_Init failed with status %s.\n", MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        return pdFAIL;
    }

    xMQTTStatus = MQTT_InitStatefulQoS(pxMqttContext,
                                       xIotcOutgoingPublishRecords,
                                       IOTC_MQTT_OUTGOING_RECORD_COUNT,
                                       xIotcIncomingPublishRecords,
                                       IOTC_MQTT_INCOMING_RECORD_COUNT);
    if (xMQTTStatus != MQTTSuccess)
    {
        printf("MQTT_InitStatefulQos failed with status %s.\n", MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        return pdFAIL;
    }

    (void)memset(&xConnectInfo, 0, sizeof(xConnectInfo));
#if (USE_PERSISTENT_SESSION == 1)
    xConnectInfo.cleanSession = false;
#else
    xConnectInfo.cleanSession = true;
#endif
    xConnectInfo.pClientIdentifier = pcClientId;
    xConnectInfo.clientIdentifierLength = (uint16_t)strlen(pcClientId);

    usKeepAlive = IOTC_MQTT_RUNTIME_KEEP_ALIVE_SECONDS;
    if (usKeepAlive > IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS)
    {
        xConnectInfo.keepAliveSeconds = IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS;
    }
    else if (usKeepAlive < IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS)
    {
        xConnectInfo.keepAliveSeconds = IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS;
    }
    else
    {
        xConnectInfo.keepAliveSeconds = usKeepAlive;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->username != NULL))
    {
        xConnectInfo.pUserName = pxIotcMqttConfig->username;
        xConnectInfo.userNameLength = (uint16_t)strlen(pxIotcMqttConfig->username);
    }
    else
    {
        xConnectInfo.pUserName = NULL;
        xConnectInfo.userNameLength = 0U;
    }
    xConnectInfo.pPassword = NULL;
    xConnectInfo.passwordLength = 0U;

    xMQTTStatus = MQTT_Connect(pxMqttContext,
                               &xConnectInfo,
                               NULL,
                               IOTC_MQTT_CONNACK_TIMEOUT_MS,
                               &xSessionPresent);
    if (xMQTTStatus != MQTTSuccess)
    {
         printf("Connection with MQTT broker failed with status %s.\n",
             MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        xReturnStatus = pdFAIL;
    }
    else
    {
         printf("MQTT connection successfully established with broker. clientID=%s\n",
             xConnectInfo.pClientIdentifier);
    }

    return xReturnStatus;
}

static void iotc_mqtt_send_cb(const char *topic, const char *json_str)
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

int run_http_identity(const char *duid, const char *cpid, const char *env)
{
    IotclDraUrlContext discovery_url = { 0 };
    IotclDraUrlContext identity_url  = { 0 };
    char              *response      = NULL;
    size_t             response_len  = 0;
    int                status        = 0;

    if ((duid == NULL) || (cpid == NULL) || (env == NULL))
    {
        printf("run_http_identity received a NULL argument.\n");
        return -1;
    }

    status = iotcl_dra_discovery_init_url_with_host(&discovery_url,
                                                    IOTC_DISCOVERY_SERVER,
                                                    cpid,
                                                    env);
    if (status != 0)
    {
        printf("Failed to build discovery URL.\n");
        goto cleanup;
    }

    status = http_client_request(iotcl_dra_url_get_url(&discovery_url),
                                 HTTP_CLIENT_OPCODE_GET,
                                 NULL,
                                 0,
                                 &response,
                                 &response_len);
    if ((status != FSP_SUCCESS) || (response == NULL))
    {
        printf("IOTC discovery request error.\n");
        if (status == FSP_SUCCESS)
        {
            status = -1;
        }
        goto cleanup;
    }

    printf("IOTC discovery response is %s\n", response);

    status = iotcl_dra_discovery_parse(&identity_url, 0, response);
    if (status != 0)
    {
         printf("Error while parsing discovery response from %s\n",
             iotcl_dra_url_get_url(&discovery_url));
        goto cleanup;
    }

    http_client_response_free(response);
    response = NULL;
    response_len = 0;

    status = iotcl_dra_identity_build_url(&identity_url, duid);
    if (status != 0)
    {
        goto cleanup;
    }

    status = http_client_request(iotcl_dra_url_get_url(&identity_url),
                                 HTTP_CLIENT_OPCODE_GET,
                                 NULL,
                                 0,
                                 &response,
                                 &response_len);
    if ((status != FSP_SUCCESS) || (response == NULL))
    {
        printf("IOTC identity request error.\n");
        if (status == FSP_SUCCESS)
        {
            status = -1;
        }
        goto cleanup;
    }

    printf("IOTC identity response is %s\n", response);

    status = iotcl_dra_identity_configure_library_mqtt(response);
    if (status != 0)
    {
         printf("Error while parsing identity response from %s\n",
             iotcl_dra_url_get_url(&identity_url));
        goto cleanup;
    }

    IotclMqttConfig *mc = iotcl_mqtt_get_config();
    if (mc != NULL)
    {
        printf("IOTC mqtt host is %s\n", (mc->host != NULL) ? mc->host : "(null)");
        printf("IOTC pub topic is %s\n", (mc->pub_rpt != NULL) ? mc->pub_rpt : "(null)");

        if (mc->username != NULL)
        {
            iotcl_free(mc->username);
            mc->username = NULL;
        }
    }

cleanup:
    if (response != NULL)
    {
        http_client_response_free(response);
    }
    iotcl_dra_url_deinit(&discovery_url);
    iotcl_dra_url_deinit(&identity_url);
    return status;
}

int iotc_init(void)
{
    return prvIotcInitWithConfig(IOTCONNECT_DUID, IOTCONNECT_DEFAULT_CPID);
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
    IotclMqttConfig *pxIotcMqttConfig = iotcl_mqtt_get_config();
    const char      *pcClientId = "";
    const char      *pcBrokerHost = "";

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->client_id != NULL))
    {
        pcClientId = pxIotcMqttConfig->client_id;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->host != NULL))
    {
        pcBrokerHost = pxIotcMqttConfig->host;
    }

    //printf("Establishing MQTT session with provisioned certificate...\n");
    printf("Attempting connection with Thing name: %s\n", pcClientId);
    printf("Endpoint: %s\n", pcBrokerHost);

RETRY_CONNECTION:
    if (xAppConnectionEstablished == true)
    {
        printf("Disconnecting MQTT session for reconnection...\n");
        xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        xAppConnectionEstablished = false;
    }

    /* Set the pParams member of the network context with desired transport. */
    xNetworkContext.pxParams = &xTlsTransportParams;

    xStatus = prvEstablishMqttSession(&xMqttContext, &xNetworkContext, &xBuffer, eventCallback);

    if (xStatus != pdTRUE)
    {
        printf("Failed to establish MQTT session with provisioned credentials.\n");

        xDisconnectMqttSession(&xMqttContext, &xNetworkContext);
        xAppConnectionEstablished = false;
        fail_count++;
        goto RETRY_CONNECTION;
    }

    printf("Sucessfully established connection with provisioned credentials.\n");
    printf("MQTT connection succeeded.\n");
    xAppConnectionEstablished = true;
    fail_count = 0;

    return pdPASS;
}

int iotconnect_sdk(IotConnectClientConfig *c)
{
    uint32_t         ulIndex;
    uint32_t         ulPublishCount = IOTCONNECT_SDK_PUBLISH_COUNT;
    uint32_t         ulPublishDelayMs = IOTCONNECT_SDK_PUBLISH_DELAY_MS;
    int              status;
    IotclMqttConfig *mc;

    if ((c == NULL) || (c->env == NULL) || (c->cpid == NULL) || (c->duid == NULL))
    {
        printf("Error: Device configuration is invalid. Configuration values for env, cpid and duid are required!\n");
        prvIotconnectSdkDeinit();
        return IOTCL_ERR_MISSING_VALUE;
    }

    if ((c->connection_type != IOTC_CT_AWS) && (c->connection_type != IOTC_CT_AZURE))
    {
        printf("Error: Device configuration is invalid. Must specify connection type!\n");
        prvIotconnectSdkDeinit();
        return IOTCL_ERR_MISSING_VALUE;
    }

    prvIotconnectSdkDeinit();

    status = prvIotcInitWithConfig(c->duid, c->cpid);
    if (status != IOTCL_SUCCESS)
    {
        prvIotconnectSdkDeinit();
        return status;
    }

    status = run_http_identity(c->duid, c->cpid, c->env);
    if (status != IOTCL_SUCCESS)
    {
        prvIotconnectSdkDeinit();
        return status;
    }

    mc = iotcl_mqtt_get_config();
    if (mc == NULL)
    {
        printf("MQTT config failed\n");
        prvIotconnectSdkDeinit();
        return -1;
    }

    if (iotc_mqtt_connect(c->event_callback) != pdPASS)
    {
        printf("MQTT Connect Failure\n");
        prvIotconnectSdkDeinit();
        return -1;
    }

    printf("MQTT Connect successful\n");

    if (c->telemetry_count != 0U)
    {
        ulPublishCount = c->telemetry_count;
    }

    if (c->telemetry_delay_ms != 0U)
    {
        ulPublishDelayMs = c->telemetry_delay_ms;
    }

    for (ulIndex = 0; ulIndex < ulPublishCount; ulIndex++)
    {
        if (iotc_publish_telemetry() != pdPASS)
        {
            printf("publish_telemetry failed at iteration %lu\n", (unsigned long) ulIndex);
            prvIotconnectSdkDeinit();
            return -1;
        }

        if ((ulIndex + 1U) < ulPublishCount)
        {
            vTaskDelay(pdMS_TO_TICKS(ulPublishDelayMs));
        }
    }

    return IOTCL_SUCCESS;
}
