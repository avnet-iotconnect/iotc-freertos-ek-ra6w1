/* SPDX-License-Identifier: MIT
 * Copyright (C) 2025 Avnet
 *
 * Board-level helpers for the /IOTCONNECT interactive demo on the EK-RA6W1.
 *
 * Provides the physical bits the demo talks about from the cloud:
 *   - LED0 (P1_14) on/off/toggle, driven by the "led" command
 *   - a user button on P0_12 (BTN2), reported as an event
 *   - Wi-Fi link info (RSSI, IP, MAC) and a nearby access-point survey
 *   - uptime, heap statistics and a clean reboot
 *
 * Wiring: the EVK buttons are not hard-wired to MCU pins. Jumper BTN2 to P0_12
 * as described in the README. The pin is sampled by a software timer; a short
 * tap is a demo event, a hold of one second or more still starts the Wi-Fi
 * SDK's WPS flow on the same pin.
 */

#ifndef IOTC_DEMO_BOARD_H
#define IOTC_DEMO_BOARD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Number of strongest access points kept from a survey. */
#ifndef IOTC_DEMO_SURVEY_TOP_N
#define IOTC_DEMO_SURVEY_TOP_N      3
#endif

/* Maximum entries requested from the Wi-Fi scan. */
#ifndef IOTC_DEMO_SURVEY_SCAN_MAX
#define IOTC_DEMO_SURVEY_SCAN_MAX   20
#endif

#define IOTC_DEMO_SSID_BUF_LEN      33   /* 32 octets + NUL */

typedef struct {
    char    ssid[IOTC_DEMO_SSID_BUF_LEN];
    int8_t  rssi;
    uint8_t channel;
} IotcDemoAp;

typedef struct {
    uint8_t    count;                           /* total access points seen */
    IotcDemoAp aps[IOTC_DEMO_SURVEY_TOP_N];     /* strongest first */
} IotcDemoApSurvey;

/* Configure LED0 and start the button poll. Safe to call more than once. */
int iotc_demo_board_init(void);

/* LED0 control. */
void iotc_demo_led_set(bool on);
void iotc_demo_led_toggle(void);
bool iotc_demo_led_get(void);

/* Total presses since boot. */
uint32_t iotc_demo_button_count(void);

/* Returns true once per press. Clears the pending press flag. */
bool iotc_demo_button_take_event(void);

/* Wi-Fi link information. Return 0 on success. */
int iotc_demo_wifi_rssi(int8_t *rssi_dbm);
int iotc_demo_wifi_mac(char *buf, size_t len);
int iotc_demo_wifi_ip(char *buf, size_t len);

/* Blocking scan of nearby access points. Takes a few seconds. Return 0 on success. */
int iotc_demo_wifi_survey(IotcDemoApSurvey *out);

/* System statistics. */
uint32_t iotc_demo_uptime_s(void);
size_t   iotc_demo_free_heap(void);
size_t   iotc_demo_min_free_heap(void);

/* Stop services and reset the MCU. Does not return. */
void iotc_demo_reboot(void);

#ifdef __cplusplus
}
#endif

#endif /* IOTC_DEMO_BOARD_H */
