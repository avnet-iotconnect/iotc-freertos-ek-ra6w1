/*
 * iotconnect.h
 */

#ifndef IOTCONNECT_H
#define IOTCONNECT_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "core_mqtt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    IOTC_CT_AWS = 0,
    IOTC_CT_AZURE
} IotConnectConnectionType;

typedef struct {
    const char *env;
    const char *cpid;
    const char *duid;
    IotConnectConnectionType connection_type;
    MQTTEventCallback_t event_callback;
    uint32_t telemetry_count;
    uint32_t telemetry_delay_ms;
} IotConnectClientConfig;


// Run IoTConnect discovery and identity over HTTPS.
int run_http_identity(const char *duid, const char *cpid, const char *env);

// Build a telemetry payload and publish it via the iotc-c-lib.
BaseType_t iotc_publish_telemetry(void);

// Establish the MQTT session with retry logic.
BaseType_t iotc_mqtt_connect(MQTTEventCallback_t eventCallback);

// Run IoTConnect init, discovery/identity, MQTT connect, and telemetry publish.
int iotconnect_sdk(IotConnectClientConfig *c);

#ifdef __cplusplus
}
#endif

#endif /* IOTCONNECT_H */
