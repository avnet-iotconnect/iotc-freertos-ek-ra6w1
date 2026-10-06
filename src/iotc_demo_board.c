/* SPDX-License-Identifier: MIT
 * Copyright (C) 2025 Avnet
 *
 * Board-level helpers for the /IOTCONNECT interactive demo on the EK-RA6W1.
 * See iotc_demo_board.h for the overview.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "portable.h"

#include "bsp_api.h"
#include "r_gpio_w.h"
#include "rm_wifi_api.h"
#include "rm_wifi.h"
#include "rm_wifi_helper.h"
#include "common_data.h"
#include "bsp_pin_cfg.h"

#include "iotc_demo_board.h"

/* LED0 is P1_14 on the EVK (symbolic name LED0 in the FSP pin configuration). */
#ifndef IOTC_DEMO_LED_PIN
#define IOTC_DEMO_LED_PIN           LED0
#endif

/* User button. The default is BTN2 on P0_12, which the Wi-Fi SDK also watches
 * as its WPS button through EXT_INTR2 (see custom_config_sdk.h). The SDK only
 * reacts to a hold of one second or more, so the demo samples the pin itself
 * instead of taking over that interrupt: a short tap is a demo event, a long
 * hold still starts WPS. Any other GPIO can be used by overriding this macro;
 * it is then configured as an input with pull-up. */
#ifndef IOTC_DEMO_BUTTON_PIN
#define IOTC_DEMO_BUTTON_PIN        EXT_INTR2_PIN
#endif

/* Sampling period of the button poll and the number of consecutive "pressed"
 * samples needed to register a press. 2 x 20 ms rejects contact bounce. */
#ifndef IOTC_DEMO_BUTTON_POLL_MS
#define IOTC_DEMO_BUTTON_POLL_MS    20U
#endif
#ifndef IOTC_DEMO_BUTTON_DEBOUNCE_SAMPLES
#define IOTC_DEMO_BUTTON_DEBOUNCE_SAMPLES 2U
#endif

static volatile uint32_t  s_button_count = 0;
static volatile bool      s_button_event = false;
static bool               s_button_pressed = false;
static uint32_t           s_button_low_samples = 0;
static TimerHandle_t      s_button_timer = NULL;
static bool               s_led_on = false;
static bool               s_initialized = false;

/* ---- Button ------------------------------------------------------------- */

static void iotc_demo_button_poll(TimerHandle_t xTimer) {
    FSP_PARAMETER_NOT_USED(xTimer);

    bsp_io_level_t level = BSP_IO_LEVEL_HIGH;
    if (g_ioport.p_api->pinRead(g_ioport.p_ctrl, IOTC_DEMO_BUTTON_PIN, &level) != FSP_SUCCESS) {
        return;
    }

    /* Active low: the pin has a pull-up and the button shorts it to ground. */
    if (level == BSP_IO_LEVEL_LOW) {
        if (!s_button_pressed) {
            s_button_low_samples++;
            if (s_button_low_samples >= IOTC_DEMO_BUTTON_DEBOUNCE_SAMPLES) {
                s_button_pressed = true;
                s_button_count++;
                s_button_event = true;
            }
        }
    } else {
        s_button_pressed = false;
        s_button_low_samples = 0;
    }
}

static int iotc_demo_button_init(void) {
    if (IOTC_DEMO_BUTTON_PIN != EXT_INTR2_PIN) {
        /* A pin of our own: input with pull-up. EXT_INTR2 is left exactly as
         * the Wi-Fi SDK configured it. */
        g_ioport.p_api->pinCfg(g_ioport.p_ctrl,
                               IOTC_DEMO_BUTTON_PIN,
                               (uint32_t) (IOPORT_CFG_PORT_DIRECTION_INPUT | IOPORT_CFG_PULLUP_ENABLE));
    }

    if (s_button_timer == NULL) {
        s_button_timer = xTimerCreate("iotcBtn",
                                      pdMS_TO_TICKS(IOTC_DEMO_BUTTON_POLL_MS),
                                      pdTRUE,
                                      NULL,
                                      iotc_demo_button_poll);
        if (s_button_timer == NULL) {
            printf("[board] button timer create failed\n");
            return -1;
        }
    }

    if (xTimerStart(s_button_timer, pdMS_TO_TICKS(100)) != pdPASS) {
        printf("[board] button timer start failed\n");
        return -1;
    }

    return 0;
}

uint32_t iotc_demo_button_count(void) {
    return s_button_count;
}

bool iotc_demo_button_take_event(void) {
    if (!s_button_event) {
        return false;
    }
    s_button_event = false;
    return true;
}

/* ---- LED ---------------------------------------------------------------- */

void iotc_demo_led_set(bool on) {
    s_led_on = on;
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl, IOTC_DEMO_LED_PIN, on ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
}

void iotc_demo_led_toggle(void) {
    iotc_demo_led_set(!s_led_on);
}

bool iotc_demo_led_get(void) {
    return s_led_on;
}

/* ---- Init --------------------------------------------------------------- */

int iotc_demo_board_init(void) {
    if (s_initialized) {
        return 0;
    }

    g_ioport.p_api->pinCfg(g_ioport.p_ctrl,
                           IOTC_DEMO_LED_PIN,
                           (uint32_t) (IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_LOW));
    iotc_demo_led_set(false);

    int rc = iotc_demo_button_init();

    s_initialized = true;
    printf("[board] LED0 on pin 0x%03x, button on pin 0x%03x (%s)\n",
           (unsigned) IOTC_DEMO_LED_PIN, (unsigned) IOTC_DEMO_BUTTON_PIN, (rc == 0) ? "polling" : "disabled");
    return rc;
}

/* ---- Wi-Fi -------------------------------------------------------------- */

int iotc_demo_wifi_rssi(int8_t *rssi_dbm) {
    if (rssi_dbm == NULL) {
        return -1;
    }
    *rssi_dbm = 0;
    return (WIFI_GetRSSI(rssi_dbm) == eWiFiSuccess) ? 0 : -1;
}

int iotc_demo_wifi_mac(char *buf, size_t len) {
    uint8_t mac[wificonfigMAX_BSSID_LEN] = { 0 };

    if ((buf == NULL) || (len == 0)) {
        return -1;
    }
    buf[0] = '\0';
    if (WIFI_GetMAC(mac) != eWiFiSuccess) {
        return -1;
    }
    snprintf(buf, len, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return 0;
}

int iotc_demo_wifi_ip(char *buf, size_t len) {
    WIFIIPConfiguration_t ip;

    if ((buf == NULL) || (len == 0)) {
        return -1;
    }
    buf[0] = '\0';
    memset(&ip, 0, sizeof(ip));
    if (WIFI_GetIPInfo(&ip) != eWiFiSuccess) {
        return -1;
    }

    /* IPv4 address is stored in network byte order (see rm_wifi_api.h). */
    const uint8_t *b = (const uint8_t *) &ip.xIPAddress.ulAddress[0];
    snprintf(buf, len, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return 0;
}

static bool scan_entry_is_valid(const WIFIScanResult_t *r) {
    if (r->ucSSIDLength > 0) {
        return true;
    }
    for (size_t i = 0; i < wificonfigMAX_BSSID_LEN; i++) {
        if (r->ucBSSID[i] != 0) {
            return true;
        }
    }
    return false;
}

static void copy_ssid(char *dst, size_t dst_len, const WIFIScanResult_t *r) {
    size_t n = r->ucSSIDLength;
    if (n > wificonfigMAX_SSID_LEN) {
        n = wificonfigMAX_SSID_LEN;
    }
    if (n >= dst_len) {
        n = dst_len - 1;
    }
    if (n == 0) {
        snprintf(dst, dst_len, "<hidden>");
        return;
    }
    for (size_t i = 0; i < n; i++) {
        unsigned char c = r->ucSSID[i];
        dst[i] = (isprint(c) && (c != '"') && (c != '\\')) ? (char) c : '?';
    }
    dst[n] = '\0';
}

int iotc_demo_wifi_survey(IotcDemoApSurvey *out) {
    if (out == NULL) {
        return -1;
    }
    memset(out, 0, sizeof(*out));

    WIFIScanResult_t *results = pvPortMalloc(sizeof(WIFIScanResult_t) * IOTC_DEMO_SURVEY_SCAN_MAX);
    if (results == NULL) {
        printf("[board] survey: out of memory\n");
        return -1;
    }
    memset(results, 0, sizeof(WIFIScanResult_t) * IOTC_DEMO_SURVEY_SCAN_MAX);

    if (WIFI_Scan(results, IOTC_DEMO_SURVEY_SCAN_MAX) != eWiFiSuccess) {
        printf("[board] survey: scan failed\n");
        vPortFree(results);
        return -1;
    }

    bool used[IOTC_DEMO_SURVEY_SCAN_MAX] = { false };
    uint8_t count = 0;
    for (size_t i = 0; i < IOTC_DEMO_SURVEY_SCAN_MAX; i++) {
        if (scan_entry_is_valid(&results[i])) {
            count++;
        } else {
            used[i] = true; /* never select empty entries */
        }
    }
    out->count = count;

    /* Results are documented as sorted by RSSI, but select the strongest
     * explicitly so the report does not depend on that. */
    uint8_t kept = 0;
    while ((kept < IOTC_DEMO_SURVEY_TOP_N) && (kept < count)) {
        int best = -1;
        for (size_t i = 0; i < IOTC_DEMO_SURVEY_SCAN_MAX; i++) {
            if (used[i]) {
                continue;
            }
            if ((best < 0) || (results[i].cRSSI > results[best].cRSSI)) {
                best = (int) i;
            }
        }
        if (best < 0) {
            break;
        }
        used[best] = true;
        copy_ssid(out->aps[kept].ssid, sizeof(out->aps[kept].ssid), &results[best]);
        out->aps[kept].rssi = results[best].cRSSI;
        out->aps[kept].channel = results[best].ucChannel;
        kept++;
    }

    vPortFree(results);

    printf("[board] survey: %u access points", (unsigned) out->count);
    for (uint8_t i = 0; i < kept; i++) {
        printf("%s %s (%d dBm, ch %u)", (i == 0) ? ":" : ",", out->aps[i].ssid, out->aps[i].rssi, out->aps[i].channel);
    }
    printf("\n");
    return 0;
}

/* ---- System ------------------------------------------------------------- */

uint32_t iotc_demo_uptime_s(void) {
    return (uint32_t) ((uint64_t) xTaskGetTickCount() / configTICK_RATE_HZ);
}

size_t iotc_demo_free_heap(void) {
    return xPortGetFreeHeapSize();
}

size_t iotc_demo_min_free_heap(void) {
    return xPortGetMinimumEverFreeHeapSize();
}

void iotc_demo_reboot(void) {
    printf("[board] rebooting on cloud request\n");
    vTaskDelay(pdMS_TO_TICKS(200));
    reset();
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
