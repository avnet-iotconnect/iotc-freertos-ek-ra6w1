/*
 * iotconnect.c
 *
 * /IOTCONNECT client for the EK-RA6W1 interactive demo. See iotconnect.h.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "FreeRTOS.h"
#include "task.h"
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

// iotc-c-lib
#include "iotcl.h"
#include "iotcl_c2d.h"
#include "iotcl_telemetry.h"
#include "iotcl_util.h"
#include "iotcl_dra_discovery.h"
#include "iotcl_dra_identity.h"

#include "iotc_https.h"
#include "iotc_demo_board.h"
#include "iotconnect.h"

#define IOTC_TOPIC_MAX_LEN          128

#ifndef IOTC_MQTT_CONNACK_TIMEOUT_MS
#define IOTC_MQTT_CONNACK_TIMEOUT_MS         (2000U)
#endif

#ifndef IOTC_MQTT_TRANSPORT_TIMEOUT_MS
#define IOTC_MQTT_TRANSPORT_TIMEOUT_MS       AWS_IOT_RX_NO_CONNECT_TIMEOUT
#endif

// Receive timeout of the TLS socket. Keep it short: it bounds how long one
// MQTT_ProcessLoop() call blocks and therefore how fast the demo loop reacts
// to button presses and pending command acks.
#ifndef IOTC_MQTT_RECV_TIMEOUT_MS
#define IOTC_MQTT_RECV_TIMEOUT_MS            (250U)
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

// Default AWS discovery endpoint for IoTConnect DRA.
#ifndef IOTC_DISCOVERY_SERVER
#define IOTC_DISCOVERY_SERVER        IOTCL_DRA_DEFAULT_DISCOVERY_HOST_AWS
#endif

// Firmware version reported in telemetry.
#ifndef IOTCONNECT_FW_VERSION
#define IOTCONNECT_FW_VERSION "2.0.0"
#endif

// 0 = publish forever.
#ifndef IOTCONNECT_SDK_PUBLISH_COUNT
#define IOTCONNECT_SDK_PUBLISH_COUNT 0U
#endif

#ifndef IOTCONNECT_SDK_PUBLISH_DELAY_MS
#define IOTCONNECT_SDK_PUBLISH_DELAY_MS 5000U
#endif

// Period of the automatic Wi-Fi access-point survey. 0 disables it; the
// "scan" command still works.
#ifndef IOTCONNECT_WIFI_SCAN_INTERVAL_MS
#define IOTCONNECT_WIFI_SCAN_INTERVAL_MS 60000U
#endif

#ifndef IOTCONNECT_MIN_PUBLISH_DELAY_MS
#define IOTCONNECT_MIN_PUBLISH_DELAY_MS 1000U
#endif

#ifndef IOTCONNECT_MAX_PUBLISH_DELAY_MS
#define IOTCONNECT_MAX_PUBLISH_DELAY_MS 3600000U
#endif

// How long each pass of the demo loop spends servicing MQTT.
#ifndef IOTCONNECT_PROCESS_LOOP_SLICE_MS
#define IOTCONNECT_PROCESS_LOOP_SLICE_MS 250U
#endif

// Reconnect backoff bounds.
#ifndef IOTCONNECT_RECONNECT_MIN_DELAY_MS
#define IOTCONNECT_RECONNECT_MIN_DELAY_MS 5000U
#endif

#ifndef IOTCONNECT_RECONNECT_MAX_DELAY_MS
#define IOTCONNECT_RECONNECT_MAX_DELAY_MS 60000U
#endif

#define IOTC_PENDING_ACK_COUNT  4
#define IOTC_ACK_MSG_LEN        96


struct NetworkContext {
    TlsTransportParams_t *pxParams;
};

typedef struct {
    bool in_use;
    bool is_ota;
    int status;
    char *ack_id;
    char message[IOTC_ACK_MSG_LEN];
} IotcPendingAck;

static MQTTContext_t xMqttContext;
static bool xAppConnectionEstablished = false;
static NetworkContext_t xNetworkContext;
static TlsTransportParams_t xTlsTransportParams;
static uint8_t ucSharedBuffer[NETWORK_BUFFER_SIZE];
static MQTTFixedBuffer_t xBuffer = { ucSharedBuffer, NETWORK_BUFFER_SIZE };

static uint32_t ulIotcGlobalEntryTimeMs;
static MQTTPubAckInfo_t xIotcOutgoingPublishRecords[IOTC_MQTT_OUTGOING_RECORD_COUNT];
static MQTTPubAckInfo_t xIotcIncomingPublishRecords[IOTC_MQTT_INCOMING_RECORD_COUNT];

// Demo state shared between the MQTT callback and the main loop. Acks are
// queued in the callback and flushed from the loop because publishing from
// inside MQTT_ProcessLoop() would re-enter coreMQTT.
static uint32_t s_telemetry_interval_ms = IOTCONNECT_SDK_PUBLISH_DELAY_MS;
static uint32_t s_scan_interval_ms = IOTCONNECT_WIFI_SCAN_INTERVAL_MS;
static volatile bool s_scan_requested = false;
static volatile bool s_reboot_requested = false;
static volatile bool s_publish_requested = false;
static volatile bool s_session_broken = false;
static IotcDemoApSurvey s_survey;
static bool s_have_survey = false;
static IotcPendingAck s_pending_acks[IOTC_PENDING_ACK_COUNT];

static void iotc_mqtt_send_cb(const char *topic, const char *json_str);

/* ---- MQTT session ------------------------------------------------------- */

static BaseType_t iotc_disconnect_mqtt_session(MQTTContext_t *pxMqttContext, NetworkContext_t *pxNetworkContext) {
    MQTTStatus_t xMqttStatus = MQTTSuccess;
    BaseType_t xReturnStatus = pdTRUE;

    configASSERT(pxMqttContext != NULL);
    configASSERT(pxNetworkContext != NULL);

    if (xAppConnectionEstablished == true) {
        xMqttStatus = MQTT_Disconnect(pxMqttContext);
        if (xMqttStatus != MQTTSuccess) {
            printf("Sending MQTT DISCONNECT failed with status=%s.\n", MQTT_Status_strerror(xMqttStatus));
            xReturnStatus = pdFAIL;
        }
    }

    TLS_FreeRTOS_Disconnect(pxNetworkContext);

    return xReturnStatus;
}

static void iotc_drop_pending_acks(void) {
    for (int i = 0; i < IOTC_PENDING_ACK_COUNT; i++) {
        if (s_pending_acks[i].in_use && (s_pending_acks[i].ack_id != NULL)) {
            iotcl_free(s_pending_acks[i].ack_id);
        }
        memset(&s_pending_acks[i], 0, sizeof(s_pending_acks[i]));
    }
}

static void iotc_sdk_deinit(void) {
    if (xAppConnectionEstablished == true) {
        iotc_disconnect_mqtt_session(&xMqttContext, &xNetworkContext);
        xAppConnectionEstablished = false;
    }

    iotc_drop_pending_acks();
    iotcl_deinit();
}

static uint32_t iotc_get_time_ms(void) {
    uint32_t ulTimeMs = pdTICKS_TO_MS(xTaskGetTickCount());

    return (uint32_t) (ulTimeMs - ulIotcGlobalEntryTimeMs);
}

/* ---- Cloud-to-device commands ------------------------------------------- */

static void iotc_queue_ack(bool is_ota, const char *ack_id, int status, const char *message) {
    if (ack_id == NULL) {
        // Command configured without "receipt required": nothing to send.
        printf("[c2d] %s (no ack requested)\n", message ? message : "");
        return;
    }

    for (int i = 0; i < IOTC_PENDING_ACK_COUNT; i++) {
        if (!s_pending_acks[i].in_use) {
            s_pending_acks[i].ack_id = iotcl_strdup(ack_id);
            if (s_pending_acks[i].ack_id == NULL) {
                printf("[c2d] out of memory, ack dropped\n");
                return;
            }
            s_pending_acks[i].is_ota = is_ota;
            s_pending_acks[i].status = status;
            snprintf(s_pending_acks[i].message, sizeof(s_pending_acks[i].message), "%s", message ? message : "");
            s_pending_acks[i].in_use = true;
            return;
        }
    }
    printf("[c2d] ack queue full, ack dropped\n");
}

static BaseType_t iotc_flush_pending_acks(void) {
    for (int i = 0; i < IOTC_PENDING_ACK_COUNT; i++) {
        IotcPendingAck *a = &s_pending_acks[i];
        if (!a->in_use) {
            continue;
        }
        int rc;
        if (a->is_ota) {
            rc = iotcl_mqtt_send_ota_ack(a->ack_id, a->status, a->message);
        } else {
            rc = iotcl_mqtt_send_cmd_ack(a->ack_id, a->status, a->message);
        }
        iotcl_free(a->ack_id);
        memset(a, 0, sizeof(*a));
        if (rc != 0) {
            // Configuration problem (e.g. no ack topic in the identity
            // response). Not a connection failure: keep the session.
            printf("[c2d] sending ack failed: %d\n", rc);
        }
        if (s_session_broken) {
            return pdFAIL;
        }
    }
    return pdPASS;
}

// Splits "verb arg" in place. Returns the verb; *arg points at the first
// argument or an empty string.
static char *iotc_split_command(char *cmd, char **arg) {
    while (*cmd && isspace((unsigned char) *cmd)) {
        cmd++;
    }
    char *p = cmd;
    while (*p && !isspace((unsigned char) *p)) {
        *p = (char) tolower((unsigned char) *p);
        p++;
    }
    if (*p) {
        *p++ = '\0';
        while (*p && isspace((unsigned char) *p)) {
            p++;
        }
    }
    // trim trailing whitespace of the argument
    char *end = p + strlen(p);
    while ((end > p) && isspace((unsigned char) end[-1])) {
        *--end = '\0';
    }
    *arg = p;
    return cmd;
}

static void iotc_on_command(IotclC2dEventData data) {
    const char *command = iotcl_c2d_get_command(data);
    const char *ack_id = iotcl_c2d_get_ack_id(data);
    char buf[128];
    char reply[IOTC_ACK_MSG_LEN];
    int status = IOTCL_C2D_EVT_CMD_FAILED;

    if (command == NULL) {
        printf("[c2d] command event without a command string\n");
        iotc_queue_ack(false, ack_id, status, "missing command");
        return;
    }

    printf("[c2d] command: \"%s\"\n", command);
    snprintf(buf, sizeof(buf), "%s", command);

    char *arg;
    char *verb = iotc_split_command(buf, &arg);

    if (strcmp(verb, "led") == 0) {
        if ((strcmp(arg, "on") == 0) || (strcmp(arg, "1") == 0) || (strcmp(arg, "true") == 0)) {
            iotc_demo_led_set(true);
        } else if ((strcmp(arg, "off") == 0) || (strcmp(arg, "0") == 0) || (strcmp(arg, "false") == 0)) {
            iotc_demo_led_set(false);
        } else if ((strcmp(arg, "toggle") == 0) || (arg[0] == '\0')) {
            iotc_demo_led_toggle();
        } else {
            snprintf(reply, sizeof(reply), "usage: led on|off|toggle");
            goto done;
        }
        status = IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK;
        snprintf(reply, sizeof(reply), "LED0 is %s", iotc_demo_led_get() ? "on" : "off");
        s_publish_requested = true; // report the new LED state right away
    } else if (strcmp(verb, "set-interval") == 0) {
        char *endp = NULL;
        unsigned long ms = strtoul(arg, &endp, 10);
        if ((arg[0] == '\0') || (endp == NULL) || (*endp != '\0')
            || (ms < IOTCONNECT_MIN_PUBLISH_DELAY_MS) || (ms > IOTCONNECT_MAX_PUBLISH_DELAY_MS)) {
            snprintf(reply, sizeof(reply), "usage: set-interval <ms> (%u..%u)",
                     (unsigned) IOTCONNECT_MIN_PUBLISH_DELAY_MS, (unsigned) IOTCONNECT_MAX_PUBLISH_DELAY_MS);
            goto done;
        }
        s_telemetry_interval_ms = (uint32_t) ms;
        status = IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK;
        snprintf(reply, sizeof(reply), "telemetry interval set to %lu ms", ms);
        s_publish_requested = true;
    } else if (strcmp(verb, "scan") == 0) {
        s_scan_requested = true;
        status = IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK;
        snprintf(reply, sizeof(reply), "Wi-Fi survey scheduled");
    } else if (strcmp(verb, "reboot") == 0) {
        s_reboot_requested = true;
        status = IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK;
        snprintf(reply, sizeof(reply), "rebooting");
    } else {
        snprintf(reply, sizeof(reply), "unknown command \"%s\"", verb);
    }

done:
    printf("[c2d] %s: %s\n", (status == IOTCL_C2D_EVT_CMD_SUCCESS_WITH_ACK) ? "ok" : "failed", reply);
    iotc_queue_ack(false, ack_id, status, reply);
}

static void iotc_on_ota(IotclC2dEventData data) {
    const char *ack_id = iotcl_c2d_get_ack_id(data);
    const char *version = iotcl_c2d_get_ota_sw_version(data);

    printf("[c2d] OTA request for version %s ignored: this demo does not implement OTA\n",
           version ? version : "(unknown)");
    iotc_queue_ack(true, ack_id, IOTCL_C2D_EVT_OTA_FAILED, "OTA not supported by this firmware");
}

/* ---- MQTT event routing -------------------------------------------------- */

static void iotc_mqtt_event_cb(MQTTContext_t *pxMqttContext,
                               MQTTPacketInfo_t *pxPacketInfo,
                               MQTTDeserializedInfo_t *pxDeserializedInfo) {
    (void) pxMqttContext;

    configASSERT(pxPacketInfo != NULL);
    configASSERT(pxDeserializedInfo != NULL);

    if ((pxPacketInfo->type & 0xF0U) != MQTT_PACKET_TYPE_PUBLISH) {
        // SUBACK / PUBACK / etc. The helper frees the outgoing publish slot
        // on PUBACK, which is required for the next publish to succeed.
        vHandleOtherIncomingPacket(pxPacketInfo, pxDeserializedInfo->packetIdentifier);
        return;
    }

    const MQTTPublishInfo_t *pub = pxDeserializedInfo->pPublishInfo;
    configASSERT(pub != NULL);

    char topic[IOTC_TOPIC_MAX_LEN];
    size_t tlen = pub->topicNameLength;
    if (tlen >= sizeof(topic)) {
        tlen = sizeof(topic) - 1;
    }
    memcpy(topic, pub->pTopicName, tlen);
    topic[tlen] = '\0';

    printf("[c2d] <- %s : %.*s\n", topic, (int) pub->payloadLength, (const char *) pub->pPayload);

    int rc = iotcl_mqtt_receive_with_length(topic, (const uint8_t *) pub->pPayload, pub->payloadLength);
    if (rc == IOTCL_ERR_IGNORED) {
        printf("[c2d] message on unexpected topic ignored\n");
    } else if (rc != 0) {
        printf("[c2d] failed to process message: %d\n", rc);
    }
}

/* ---- Library configuration --------------------------------------------- */

static int iotc_init_with_config(const char *pcDuid, const char *pcCpid) {
    IotclClientConfig cfg;

    if ((pcDuid == NULL) || (pcCpid == NULL)) {
        return IOTCL_ERR_MISSING_VALUE;
    }

    iotcl_init_client_config(&cfg);

    cfg.device.duid = pcDuid;
    cfg.device.cpid = pcCpid;
    cfg.device.instance_type = IOTCL_DCT_CUSTOM;
    cfg.mqtt_send_cb = iotc_mqtt_send_cb;
    cfg.events.cmd_cb = iotc_on_command;
    cfg.events.ota_cb = iotc_on_ota;

    int rc = iotcl_init(&cfg);
    if (rc != 0) {
        printf("iotcl_init failed: %d\n", rc);
        return rc;
    }

    return IOTCL_SUCCESS;
}

static BaseType_t iotc_establish_mqtt_session(
    MQTTContext_t *pxMqttContext,
    NetworkContext_t *pxNetworkContext,
    MQTTFixedBuffer_t *pxNetworkBuffer,
    MQTTEventCallback_t eventCallback
) {
    BaseType_t xReturnStatus = pdTRUE;
    TlsTransportStatus_t xNetworkStatus;
    MQTTStatus_t xMQTTStatus;
    MQTTConnectInfo_t xConnectInfo;
    TransportInterface_t xTransport;
    NetworkCredentials_t xNetworkCredentials = { 0 };
    IotclMqttConfig *pxIotcMqttConfig = iotcl_mqtt_get_config();
    const char *pcBrokerHost = "";
    const char *pcClientId = "";
    bool xSessionPresent = false;
    UINT16 usKeepAlive = IOTC_MQTT_KEEP_ALIVE_SECONDS;

#if defined(democonfigCLIENT_USERNAME)
    static const char *ppcAlpnProtocols[] = { "mqtt", NULL };
#else
    static const char *ppcAlpnProtocols[] = { "x-amzn-mqtt-ca", NULL };
#endif

    configASSERT(pxMqttContext != NULL);
    configASSERT(pxNetworkContext != NULL);
    configASSERT(pxNetworkBuffer != NULL);

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->host != NULL)) {
        pcBrokerHost = pxIotcMqttConfig->host;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->client_id != NULL)) {
        pcClientId = pxIotcMqttConfig->client_id;
    }

    memset(pxMqttContext, 0, sizeof(*pxMqttContext));
    memset(xIotcOutgoingPublishRecords, 0, sizeof(xIotcOutgoingPublishRecords));
    memset(xIotcIncomingPublishRecords, 0, sizeof(xIotcIncomingPublishRecords));

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
        pcBrokerHost, (unsigned long) IOTC_MQTT_RUNTIME_BROKER_PORT);

    xNetworkStatus = TLS_FreeRTOS_Connect(pxNetworkContext,
                                          pcBrokerHost,
                                          (uint16_t)IOTC_MQTT_RUNTIME_BROKER_PORT,
                                          &xNetworkCredentials,
                                          IOTC_MQTT_RECV_TIMEOUT_MS,
                                          IOTC_MQTT_TRANSPORT_TIMEOUT_MS);
    if (xNetworkStatus != TLS_TRANSPORT_SUCCESS) {
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
                            iotc_get_time_ms,
                            eventCallback,
                            pxNetworkBuffer);
    if (xMQTTStatus != MQTTSuccess) {
        printf("MQTT_Init failed with status %s.\n", MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        return pdFAIL;
    }

    xMQTTStatus = MQTT_InitStatefulQoS(pxMqttContext,
                                       xIotcOutgoingPublishRecords,
                                       IOTC_MQTT_OUTGOING_RECORD_COUNT,
                                       xIotcIncomingPublishRecords,
                                       IOTC_MQTT_INCOMING_RECORD_COUNT);
    if (xMQTTStatus != MQTTSuccess) {
        printf("MQTT_InitStatefulQos failed with status %s.\n", MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        return pdFAIL;
    }

    memset(&xConnectInfo, 0, sizeof(xConnectInfo));
#if (USE_PERSISTENT_SESSION == 1)
    xConnectInfo.cleanSession = false;
#else
    xConnectInfo.cleanSession = true;
#endif
    xConnectInfo.pClientIdentifier = pcClientId;
    xConnectInfo.clientIdentifierLength = (uint16_t)strlen(pcClientId);

    usKeepAlive = IOTC_MQTT_RUNTIME_KEEP_ALIVE_SECONDS;
    if (usKeepAlive > IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS) {
        xConnectInfo.keepAliveSeconds = IOTC_MQTT_KEEP_ALIVE_MAX_SECONDS;
    } else if (usKeepAlive < IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS) {
        xConnectInfo.keepAliveSeconds = IOTC_MQTT_KEEP_ALIVE_MIN_SECONDS;
    } else {
        xConnectInfo.keepAliveSeconds = usKeepAlive;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->username != NULL)) {
        xConnectInfo.pUserName = pxIotcMqttConfig->username;
        xConnectInfo.userNameLength = (uint16_t) strlen(pxIotcMqttConfig->username);
    } else {
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
    if (xMQTTStatus != MQTTSuccess) {
        printf("Connection with MQTT broker failed with status %s.\n",
            MQTT_Status_strerror(xMQTTStatus));
        TLS_FreeRTOS_Disconnect(pxNetworkContext);
        xReturnStatus = pdFAIL;
    } else {
        printf("MQTT connection successfully established with broker. clientID=%s\n",
            xConnectInfo.pClientIdentifier);
    }

    return xReturnStatus;
}

static void iotc_mqtt_send_cb(const char *topic, const char *json_str) {
    BaseType_t xStatus = xPublishToTopic(&xMqttContext,
                                         topic,
                                         (int32_t)strlen(topic),
                                         json_str,
                                         (size_t)strlen(json_str));

    if (xStatus == pdPASS) {
        printf("[publishJson] OK -> %s : %s\r\n", topic, json_str);
    } else {
        printf("[publishJson] FAIL -> %s\r\n", topic);
        // A failed publish means the connection is unusable. Let the loop reconnect.
        s_session_broken = true;
    }
}

int run_http_identity(const char *duid, const char *cpid, const char *env) {
    IotclDraUrlContext discovery_url = { 0 };
    IotclDraUrlContext identity_url = { 0 };
    char *response = NULL;
    size_t response_len = 0;
    int status = 0;

    if ((duid == NULL) || (cpid == NULL) || (env == NULL)) {
        printf("run_http_identity received a NULL argument.\n");
        return -1;
    }

    status = iotcl_dra_discovery_init_url_with_host(&discovery_url,
                                                    IOTC_DISCOVERY_SERVER,
                                                    cpid,
                                                    env);
    if (status != 0) {
        printf("Failed to build discovery URL.\n");
        goto cleanup;
    }

    status = http_client_request(iotcl_dra_url_get_url(&discovery_url),
                                 HTTP_CLIENT_OPCODE_GET,
                                 NULL,
                                 0,
                                 &response,
                                 &response_len);
    if ((status != FSP_SUCCESS) || (response == NULL)) {
        printf("IOTC discovery request error.\n");
        if (status == FSP_SUCCESS) {
            status = -1;
        }
        goto cleanup;
    }

    printf("IOTC discovery response is %s\n", response);

    status = iotcl_dra_discovery_parse(&identity_url, 0, response);
    if (status != 0) {
        printf("Error while parsing discovery response from %s\n",
            iotcl_dra_url_get_url(&discovery_url));
        goto cleanup;
    }

    http_client_response_free(response);
    response = NULL;
    response_len = 0;

    status = iotcl_dra_identity_build_url(&identity_url, duid);
    if (status != 0) {
        goto cleanup;
    }

    status = http_client_request(iotcl_dra_url_get_url(&identity_url),
                                 HTTP_CLIENT_OPCODE_GET,
                                 NULL,
                                 0,
                                 &response,
                                 &response_len);
    if ((status != FSP_SUCCESS) || (response == NULL)) {
        printf("IOTC identity request error.\n");
        if (status == FSP_SUCCESS) {
            status = -1;
        }
        goto cleanup;
    }

    printf("IOTC identity response is %s\n", response);

    status = iotcl_dra_identity_configure_library_mqtt(response);
    if (status != 0) {
        printf("Error while parsing identity response from %s\n",
            iotcl_dra_url_get_url(&identity_url));
        goto cleanup;
    }

    IotclMqttConfig *mc = iotcl_mqtt_get_config();
    if (mc != NULL) {
        printf("IOTC mqtt host is %s\n", (mc->host != NULL) ? mc->host : "(null)");
        printf("IOTC pub topic is %s\n", (mc->pub_rpt != NULL) ? mc->pub_rpt : "(null)");
        printf("IOTC c2d topic is %s\n", (mc->sub_c2d != NULL) ? mc->sub_c2d : "(null)");

        if (mc->username != NULL) {
            iotcl_free(mc->username);
            mc->username = NULL;
        }
    }

cleanup:
    if (response != NULL) {
        http_client_response_free(response);
    }
    iotcl_dra_url_deinit(&discovery_url);
    iotcl_dra_url_deinit(&identity_url);
    return status;
}

/* ---- Telemetry ----------------------------------------------------------- */

static BaseType_t iotc_publish_telemetry_ex(bool button_pressed) {
    if (!xAppConnectionEstablished) {
        printf("publish_telemetry: not connected\n");
        return pdFAIL;
    }

    IotclMessageHandle msg = iotcl_telemetry_create();
    if (msg == NULL) {
        printf("iotcl_telemetry_create failed\n");
        return pdFAIL;
    }

    char text[32];
    int8_t rssi = 0;

    iotcl_telemetry_set_string(msg, "version", IOTCONNECT_FW_VERSION);
    iotcl_telemetry_set_number(msg, "uptime_s", (double) iotc_demo_uptime_s());
    iotcl_telemetry_set_number(msg, "free_heap", (double) iotc_demo_free_heap());
    iotcl_telemetry_set_number(msg, "min_free_heap", (double) iotc_demo_min_free_heap());
    iotcl_telemetry_set_number(msg, "report_interval_ms", (double) s_telemetry_interval_ms);

    if (iotc_demo_wifi_rssi(&rssi) == 0) {
        iotcl_telemetry_set_number(msg, "wifi_rssi", (double) rssi);
    } else {
        iotcl_telemetry_set_null(msg, "wifi_rssi");
    }
    if (iotc_demo_wifi_ip(text, sizeof(text)) == 0) {
        iotcl_telemetry_set_string(msg, "ip", text);
    }
    if (iotc_demo_wifi_mac(text, sizeof(text)) == 0) {
        iotcl_telemetry_set_string(msg, "mac", text);
    }

    iotcl_telemetry_set_bool(msg, "led", iotc_demo_led_get());
    iotcl_telemetry_set_number(msg, "button_count", (double) iotc_demo_button_count());
    iotcl_telemetry_set_bool(msg, "button_pressed", button_pressed);

    if (s_have_survey) {
        iotcl_telemetry_set_number(msg, "wifi_ap_count", (double) s_survey.count);
        for (int i = 0; (i < IOTC_DEMO_SURVEY_TOP_N) && (i < s_survey.count); i++) {
            char path[24];
            snprintf(path, sizeof(path), "ap%d.ssid", i + 1);
            iotcl_telemetry_set_string(msg, path, s_survey.aps[i].ssid);
            snprintf(path, sizeof(path), "ap%d.rssi", i + 1);
            iotcl_telemetry_set_number(msg, path, (double) s_survey.aps[i].rssi);
            snprintf(path, sizeof(path), "ap%d.channel", i + 1);
            iotcl_telemetry_set_number(msg, path, (double) s_survey.aps[i].channel);
        }
    }

    // Triggers iotc_mqtt_send_cb() -> xPublishToTopic().
    int rc = iotcl_mqtt_send_telemetry(msg, false);
    iotcl_telemetry_destroy(msg);

    return ((rc == 0) && !s_session_broken) ? pdPASS : pdFAIL;
}

BaseType_t iotc_publish_telemetry(void) {
    return iotc_publish_telemetry_ex(false);
}

static void iotc_run_survey(void) {
    if (iotc_demo_wifi_survey(&s_survey) == 0) {
        s_have_survey = true;
    }
}

/* ---- Connection ---------------------------------------------------------- */

BaseType_t iotc_mqtt_connect(void) {
    BaseType_t xStatus = pdFAIL;
    IotclMqttConfig *pxIotcMqttConfig = iotcl_mqtt_get_config();
    const char *pcClientId = "";
    const char *pcBrokerHost = "";

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->client_id != NULL)) {
        pcClientId = pxIotcMqttConfig->client_id;
    }

    if ((pxIotcMqttConfig != NULL) && (pxIotcMqttConfig->host != NULL)) {
        pcBrokerHost = pxIotcMqttConfig->host;
    }

    printf("Attempting connection with Thing name: %s\n", pcClientId);
    printf("Endpoint: %s\n", pcBrokerHost);

    if (xAppConnectionEstablished == true) {
        printf("Disconnecting MQTT session for reconnection...\n");
        iotc_disconnect_mqtt_session(&xMqttContext, &xNetworkContext);
        xAppConnectionEstablished = false;
    }

    // Set the network context transport parameters before connecting.
    xNetworkContext.pxParams = &xTlsTransportParams;

    xStatus = iotc_establish_mqtt_session(&xMqttContext, &xNetworkContext, &xBuffer, iotc_mqtt_event_cb);

    if (xStatus != pdTRUE) {
        printf("Failed to establish MQTT session with provisioned credentials.\n");
        TLS_FreeRTOS_Disconnect(&xNetworkContext);
        xAppConnectionEstablished = false;
        return pdFAIL;
    }

    printf("Successfully established connection with provisioned credentials.\n");
    xAppConnectionEstablished = true;
    s_session_broken = false;

    return pdPASS;
}

static BaseType_t iotc_subscribe_c2d(void) {
    IotclMqttConfig *mc = iotcl_mqtt_get_config();

    if ((mc == NULL) || (mc->sub_c2d == NULL)) {
        printf("No c2d topic in the identity response; commands will not work.\n");
        return pdPASS;
    }

    if (xSubscribeToTopic(&xMqttContext, mc->sub_c2d, (uint16_t) strlen(mc->sub_c2d)) != pdPASS) {
        printf("Failed to subscribe to %s\n", mc->sub_c2d);
        return pdFAIL;
    }

    printf("Subscribed to commands on %s\n", mc->sub_c2d);
    return pdPASS;
}

// One connected session. Returns pdPASS when the configured message count
// was reached, pdFAIL when the connection has to be rebuilt.
static BaseType_t iotc_session_loop(uint32_t max_messages, uint32_t *published) {
    TickType_t last_publish = xTaskGetTickCount();
    TickType_t last_scan = xTaskGetTickCount();

    // Boot report, with the survey taken before connecting.
    if (iotc_publish_telemetry_ex(false) != pdPASS) {
        return pdFAIL;
    }
    (*published)++;

    while (xAppConnectionEstablished && !s_session_broken) {
        if ((max_messages != 0U) && (*published >= max_messages)) {
            return pdPASS;
        }

        // Service MQTT: receives commands, keep-alive, acks.
        if (xProcessLoop(&xMqttContext, IOTCONNECT_PROCESS_LOOP_SLICE_MS) != pdPASS) {
            printf("MQTT process loop failed; reconnecting.\n");
            return pdFAIL;
        }

        if (iotc_flush_pending_acks() != pdPASS) {
            return pdFAIL;
        }

        if (s_reboot_requested) {
            vTaskDelay(pdMS_TO_TICKS(500)); // let the ack leave the socket
            iotc_demo_reboot();
        }

        TickType_t now = xTaskGetTickCount();
        bool publish_now = false;
        bool button = false;

        if (iotc_demo_button_take_event()) {
            printf("[board] button pressed (%lu total)\n", (unsigned long) iotc_demo_button_count());
            button = true;
            publish_now = true;
        }

        if (s_scan_requested
            || ((s_scan_interval_ms != 0U) && ((now - last_scan) >= pdMS_TO_TICKS(s_scan_interval_ms)))) {
            s_scan_requested = false;
            last_scan = now;
            iotc_run_survey();
            publish_now = true;
        }

        if (s_publish_requested) {
            s_publish_requested = false;
            publish_now = true;
        }

        if ((now - last_publish) >= pdMS_TO_TICKS(s_telemetry_interval_ms)) {
            publish_now = true;
        }

        if (publish_now) {
            if (iotc_publish_telemetry_ex(button) != pdPASS) {
                printf("publish_telemetry failed; reconnecting.\n");
                return pdFAIL;
            }
            (*published)++;
            last_publish = xTaskGetTickCount();
        }
    }

    return pdFAIL;
}

int iotconnect_sdk(IotConnectClientConfig *c) {
    if ((c == NULL) || (c->env == NULL) || (c->cpid == NULL) || (c->duid == NULL)) {
        printf("Error: Device configuration is invalid. Configuration values for env, cpid and duid are required!\n");
        iotc_sdk_deinit();
        return IOTCL_ERR_MISSING_VALUE;
    }

    if ((c->connection_type != IOTC_CT_AWS) && (c->connection_type != IOTC_CT_AZURE)) {
        printf("Error: Device configuration is invalid. Must specify connection type!\n");
        iotc_sdk_deinit();
        return IOTCL_ERR_MISSING_VALUE;
    }

    if ((c->env[0] == '\0') || (c->cpid[0] == '\0') || (c->duid[0] == '\0')) {
        printf("Error: IOTCONNECT_ENV, IOTCONNECT_CPID and IOTCONNECT_DUID must be set in app_config.h!\n");
        return IOTCL_ERR_MISSING_VALUE;
    }

    uint32_t max_messages = IOTCONNECT_SDK_PUBLISH_COUNT;
    if (c->telemetry_count != 0U) {
        max_messages = c->telemetry_count;
    }
    if (c->telemetry_delay_ms != 0U) {
        s_telemetry_interval_ms = c->telemetry_delay_ms;
    }
    if (c->wifi_scan_interval_ms != 0U) {
        s_scan_interval_ms = c->wifi_scan_interval_ms;
    }

    iotc_demo_board_init();

    uint32_t published = 0;
    uint32_t backoff_ms = IOTCONNECT_RECONNECT_MIN_DELAY_MS;

    for (;;) {
        iotc_sdk_deinit();

        int status = iotc_init_with_config(c->duid, c->cpid);
        if (status != IOTCL_SUCCESS) {
            iotc_sdk_deinit();
            return status;
        }

        status = run_http_identity(c->duid, c->cpid, c->env);
        if (status != IOTCL_SUCCESS) {
            printf("Discovery/identity failed (%d); retrying in %lu ms\n", status, (unsigned long) backoff_ms);
            goto retry;
        }

        if (iotcl_mqtt_get_config() == NULL) {
            printf("MQTT config failed\n");
            goto retry;
        }

        // Survey before connecting so the first report already carries it,
        // and so the scan does not stall a fresh MQTT session.
        if (s_scan_interval_ms != 0U) {
            iotc_run_survey();
        }

        if (iotc_mqtt_connect() != pdPASS) {
            printf("MQTT connect failed; retrying in %lu ms\n", (unsigned long) backoff_ms);
            goto retry;
        }

        if (iotc_subscribe_c2d() != pdPASS) {
            goto retry;
        }

        printf("Connected. LED0, button, Wi-Fi survey and commands are live.\n");
        backoff_ms = IOTCONNECT_RECONNECT_MIN_DELAY_MS;

        if (iotc_session_loop(max_messages, &published) == pdPASS) {
            printf("Published %lu messages; done.\n", (unsigned long) published);
            iotc_sdk_deinit();
            return IOTCL_SUCCESS;
        }

retry:
        iotc_sdk_deinit();
        vTaskDelay(pdMS_TO_TICKS(backoff_ms));
        backoff_ms *= 2U;
        if (backoff_ms > IOTCONNECT_RECONNECT_MAX_DELAY_MS) {
            backoff_ms = IOTCONNECT_RECONNECT_MAX_DELAY_MS;
        }
    }
}
