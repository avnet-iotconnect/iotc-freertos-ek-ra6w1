/*
 * iotconnect.h
 *
 * IoTConnect helper API extracted from app_dpm_sample.c.
 */
#ifndef IOTCONNECT_H
#define IOTCONNECT_H

#include "FreeRTOS.h"
#include "core_mqtt.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the iotc-c-lib client config and MQTT topics.
 *
 * @return 0 on success, non-zero on failure.
 */
int iotc_init(void);

/**
 * @brief Build a telemetry payload and publish it via the iotc-c-lib.
 *
 * @return pdPASS on success, pdFAIL otherwise.
 */
BaseType_t iotc_publish_telemetry(void);

/**
 * @brief Establish the MQTT session with retry logic.
 *
 * Disconnects any previous session, opens a new TLS/MQTT session
 * (using PKCS#11 credentials when fleet provisioning is active) and
 * keeps retrying until success. After MAX_RETRY_CNT_TO_SLEEP failures
 * the device is forced to sleep via goSleepAppOnException().
 *
 * Updates the shared `xAppConnectionEstablished` flag.
 *
 * @param[in] eventCallback MQTT event callback to register on the session.
 *
 * @return pdPASS once the connection is established.
 */
BaseType_t iotc_mqtt_connect(MQTTEventCallback_t eventCallback);

#ifdef __cplusplus
}
#endif

#endif /* IOTCONNECT_H */
