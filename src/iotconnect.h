/*
 * iotconnect.h
 *
 * IoTConnect helper API extracted from app_dpm_sample.c.
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

typedef enum
{
	IOTC_CT_AWS = 0,
	IOTC_CT_AZURE
} IotConnectConnectionType;

typedef struct
{
	const char               *env;
	const char               *cpid;
	const char               *duid;
	IotConnectConnectionType  connection_type;
	MQTTEventCallback_t       event_callback;
	uint32_t                  telemetry_count;
	uint32_t                  telemetry_delay_ms;
} IotConnectClientConfig;


/**
 * @brief Initialize the iotc-c-lib client config and MQTT topics.
 *
 * @return 0 on success, non-zero on failure.
 */
int iotc_init(void);

/**
 * @brief Run IoTConnect discovery and identity over HTTPS.
 *
 * Uses the IoTConnect REST discovery flow to resolve the broker details,
 * then configures the library MQTT settings from the identity response.
 *
 * @param[in] duid Device unique identifier.
 * @param[in] cpid Company identifier.
 * @param[in] env  IoTConnect environment name.
 *
 * @return 0 on success, non-zero on failure.
 */
int run_http_identity(const char *duid, const char *cpid, const char *env);

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

/**
 * @brief Run IoTConnect init, discovery/identity, MQTT connect, and telemetry publish.
 *
 * This is a repo-native replacement for the sample SDK flow that used W6X helpers.
 * The optional @p iotc_mqtt_config output is filled from the resolved IoTConnect MQTT config.
 *
 * @param[in] c IoTConnect client configuration.
 * @param[in,out] iotc_mqtt_config Optional output buffers for broker host and client ID.
 *
 * @return IOTCL_SUCCESS on success, non-zero on failure.
 */
int iotconnect_sdk(IotConnectClientConfig *c);

#ifdef __cplusplus
}
#endif

#endif /* IOTCONNECT_H */
