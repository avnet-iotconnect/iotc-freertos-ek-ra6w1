/*
 * iotconnect.h
 *
 * /IOTCONNECT client for the EK-RA6W1 interactive demo.
 *
 * iotconnect_sdk() runs the whole device lifecycle: discovery and identity
 * over HTTPS, MQTT connect, subscription to the command topic, periodic
 * telemetry, button events, Wi-Fi access-point surveys and cloud-to-device
 * commands. It reconnects on failure and only returns when a telemetry
 * message limit is configured and reached.
 *
 * Commands understood (configure them in the device template):
 *   led <on|off|toggle>      drive LED0
 *   set-interval <ms>        change the telemetry period (1000..3600000)
 *   scan                     run a Wi-Fi survey now and report it
 *   reboot                   restart the board
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
    uint32_t telemetry_count;          /* 0 = run forever */
    uint32_t telemetry_delay_ms;       /* 0 = IOTCONNECT_SDK_PUBLISH_DELAY_MS */
    uint32_t wifi_scan_interval_ms;    /* 0 = IOTCONNECT_WIFI_SCAN_INTERVAL_MS */
} IotConnectClientConfig;


// Run IoTConnect discovery and identity over HTTPS.
int run_http_identity(const char *duid, const char *cpid, const char *env);

// Build a telemetry payload from the current board state and publish it.
BaseType_t iotc_publish_telemetry(void);

// Establish the MQTT session. Returns pdPASS or pdFAIL; the caller retries.
BaseType_t iotc_mqtt_connect(void);

// Run the demo: init, discovery/identity, MQTT connect, telemetry and command loop.
int iotconnect_sdk(IotConnectClientConfig *c);

#ifdef __cplusplus
}
#endif

#endif /* IOTCONNECT_H */
