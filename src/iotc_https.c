/*
 * iotc_https.c
 *
 * Wrappers around the Renesas FSP HTTPS client (rm_https_w) and lwIP DNS.
 *
 * Provides:
 *   - get_host_address()    : DNS name -> IPv4 string (uses dns_A_Query()).
 *   - http_client_request() : one-shot HTTP/HTTPS request using the
 *                             g_https_w driver instance.
 *
 * The implementation opens its own https_w control block, performs the
 * request, waits (via task notification) for the asynchronous callback
 * to signal completion, and then closes the driver.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "projdefs.h"

#include "rm_https_api.h"
#include "rm_https_w.h"
#include "mbedtls/ssl.h"

#include "iotc_https.h"


/* Forward declared in the FSP; provides DNS name resolution. */
extern char *dns_A_Query(char *domain_name, unsigned long wait_option);

#ifndef IOTC_HTTPS_DEFAULT_IFACE
#define IOTC_HTTPS_DEFAULT_IFACE   (0)   /* WLAN0 */
#endif

#ifndef IOTC_HTTPS_REQUEST_TIMEOUT_MS
#define IOTC_HTTPS_REQUEST_TIMEOUT_MS  (30U * 1000U)
#endif

/* Initial / growth size for the response buffer (bytes). */
#ifndef IOTC_HTTPS_RESP_INITIAL_CAP
#define IOTC_HTTPS_RESP_INITIAL_CAP    (2U * 1024U)
#endif

/* Hard cap on the response buffer size to prevent unbounded growth. */
#ifndef IOTC_HTTPS_RESP_MAX_CAP
#define IOTC_HTTPS_RESP_MAX_CAP        (64U * 1024U)
#endif

/* HTTPS driver instance shared with this module. */
extern const https_api_t g_https_w;

/* Local control block / config used for one-shot requests. */
static https_w_instance_ctrl_t s_iotc_https_ctrl;
static TaskHandle_t            s_iotc_https_waiter = NULL;
static volatile int            s_iotc_https_result = 0;     /* +1=ok, -1=err */
static volatile uint32_t       s_iotc_https_httpc_result = 0; /* httpc_result_t */
static volatile int            s_iotc_https_err = 0;          /* lwIP err_t */

/* Response capture state, valid for the duration of one request. */
static char   *s_iotc_https_resp_buf = NULL;
static size_t  s_iotc_https_resp_len = 0U;
static size_t  s_iotc_https_resp_cap = 0U;
static bool    s_iotc_https_resp_overflow = false;

/* Append @p len bytes from @p data into the response buffer, growing it as
 * needed (up to IOTC_HTTPS_RESP_MAX_CAP). Sets the overflow flag if the
 * cap or an allocation failure prevents storing the data. */
static void iotc_https_resp_append(const void *data, size_t len)
{
    if (data == NULL || len == 0U || s_iotc_https_resp_overflow)
    {
        return;
    }

    /* Need room for new bytes plus a trailing NUL. */
    size_t needed = s_iotc_https_resp_len + len + 1U;
    if (needed > s_iotc_https_resp_cap)
    {
        size_t new_cap = (s_iotc_https_resp_cap == 0U) ? IOTC_HTTPS_RESP_INITIAL_CAP
                                                       : s_iotc_https_resp_cap;
        while (new_cap < needed)
        {
            new_cap *= 2U;
        }
        if (new_cap > IOTC_HTTPS_RESP_MAX_CAP)
        {
            new_cap = IOTC_HTTPS_RESP_MAX_CAP;
        }
        if (new_cap < needed)
        {
            s_iotc_https_resp_overflow = true;
            return;
        }

        char *new_buf = (char *)pvPortMalloc(new_cap);
        if (new_buf == NULL)
        {
            s_iotc_https_resp_overflow = true;
            return;
        }
        if (s_iotc_https_resp_buf != NULL)
        {
            memcpy(new_buf, s_iotc_https_resp_buf, s_iotc_https_resp_len);
            vPortFree(s_iotc_https_resp_buf);
        }
        s_iotc_https_resp_buf = new_buf;
        s_iotc_https_resp_cap = new_cap;
    }

    memcpy(s_iotc_https_resp_buf + s_iotc_https_resp_len, data, len);
    s_iotc_https_resp_len += len;
    s_iotc_https_resp_buf[s_iotc_https_resp_len] = '\0';
}

/*-----------------------------------------------------------*/

static void iotc_https_callback(https_callback_args_t *p_args)
{
    if (p_args == NULL)
    {
        return;
    }

    switch (p_args->event)
    {
        case HTTPS_EVENT_CLIENT_RECVED:           /* raw body chunk */
        case HTTPS_EVENT_CLIENT_RECVED_DECODED:   /* de-chunked body chunk */
        {
            iotc_https_resp_append(p_args->payload, (size_t)p_args->len);
            break;
        }

        case HTTPS_EVENT_CLIENT_RESULT:
        {
            int8_t err = (p_args->p_param != NULL) ? *(int8_t *)p_args->p_param : -1;
            uint32_t httpc_result = (p_args->payload != NULL)
                                  ? *(uint32_t *)p_args->payload : 0xFFFFFFFFU;
            s_iotc_https_httpc_result = httpc_result;
            s_iotc_https_err          = (int)err;
            s_iotc_https_result       = (err == 0 && httpc_result == 0) ? 1 : -1;
            if (s_iotc_https_waiter != NULL)
            {
                xTaskNotifyGive(s_iotc_https_waiter);
            }
            break;
        }

        case HTTPS_EVENT_CLIENT_GET_DONE:
        {
            if (s_iotc_https_result == 0)
            {
                s_iotc_https_result = 1;
            }
            if (s_iotc_https_waiter != NULL)
            {
                xTaskNotifyGive(s_iotc_https_waiter);
            }
            break;
        }

        default:
            break;
    }
}

static const https_cfg_t s_iotc_https_cfg =
{
    .p_callback = iotc_https_callback,
    .p_context  = NULL,
    .p_extend   = NULL,
};

/*-----------------------------------------------------------*/

BaseType_t get_host_address(const char *hostname,
                            char       *ipstr,
                            size_t      ipstr_len,
                            uint32_t    timeout_ms)
{
    if (hostname == NULL || ipstr == NULL || ipstr_len < 16U)
    {
        return pdFAIL;
    }

    /* dns_A_Query takes a non-const char *. */
    char *resolved = dns_A_Query((char *)hostname, (unsigned long)timeout_ms);
    if (resolved == NULL || resolved[0] == '\0')
    {
        printf("get_host_address: failed to resolve \"%s\"\n", hostname);
        return pdFAIL;
    }

    strncpy(ipstr, resolved, ipstr_len - 1U);
    ipstr[ipstr_len - 1U] = '\0';
    return pdPASS;
}

/*-----------------------------------------------------------*/

/* Parse "http(s)://host[:port]/path" into a pre-zeroed http_client_request_t.
 * Returns 0 on success, -1 on failure. */
static int iotc_https_parse_url(const char            *url,
                                http_client_request_t *req)
{
    if (url == NULL || req == NULL)
    {
        return -1;
    }

    const char *p = url;

    /* scheme */
    if (strncasecmp(p, "https://", 8) == 0)
    {
        req->insecure = pdTRUE;          /* "insecure" field == "is TLS" in this driver */
        req->port     = HTTPS_SERVER_PORT;
        p += 8;
    }
    else if (strncasecmp(p, "http://", 7) == 0)
    {
        req->insecure = pdFALSE;
        req->port     = HTTP_SERVER_PORT;
        p += 7;
    }
    else
    {
        printf("iotc_https: unsupported scheme in URL \"%s\"\n", url);
        return -1;
    }

    /* host[:port] */
    const char *host_start = p;
    while (*p && *p != ':' && *p != '/')
    {
        p++;
    }

    size_t host_len = (size_t)(p - host_start);
    if (host_len == 0U || host_len >= HTTPC_MAX_HOSTNAME_LEN)
    {
        printf("iotc_https: invalid hostname in URL\n");
        return -1;
    }
    memcpy(req->hostname, host_start, host_len);
    req->hostname[host_len] = '\0';

    /* optional :port */
    if (*p == ':')
    {
        p++;
        uint32_t port = 0U;
        while (isdigit((unsigned char)*p))
        {
            port = (port * 10U) + (uint32_t)(*p - '0');
            p++;
        }
        if (port > 0U && port < 65536U)
        {
            req->port = port;
        }
    }

    /* path */
    if (*p == '\0')
    {
        req->path[0] = '/';
        req->path[1] = '\0';
    }
    else
    {
        size_t path_len = strlen(p);
        if (path_len >= HTTPC_MAX_PATH_LEN)
        {
            printf("iotc_https: path too long\n");
            return -1;
        }
        memcpy(req->path, p, path_len);
        req->path[path_len] = '\0';
    }

    return 0;
}

/*-----------------------------------------------------------*/

void http_client_response_free(char *resp)
{
    if (resp != NULL)
    {
        vPortFree(resp);
    }
}

int http_client_request(const char           *url,
                        https_client_opcode_t op_code,
                        const void           *body,
                        size_t                body_len,
                        char                **resp_out,
                        size_t               *resp_len_out)
{
    fsp_err_t              err;
    http_client_request_t  request;

    if (resp_out != NULL)     { *resp_out = NULL; }
    if (resp_len_out != NULL) { *resp_len_out = 0U; }

    if (url == NULL)
    {
        return (int)FSP_ERR_INVALID_ARGUMENT;
    }

    /* Body sanity check for write methods. */
    if (op_code == HTTP_CLIENT_OPCODE_PUT  ||
        op_code == HTTP_CLIENT_OPCODE_POST ||
        op_code == HTTP_CLIENT_OPCODE_PATCH)
    {
        if (body == NULL || body_len == 0U)
        {
            return (int)FSP_ERR_INVALID_ARGUMENT;
        }
        if (body_len > HTTPC_MAX_REQ_DATA)
        {
            return (int)FSP_ERR_INVALID_ARGUMENT;
        }
    }

    memset(&request, 0, sizeof(request));
    request.op_code = op_code;
    request.iface   = IOTC_HTTPS_DEFAULT_IFACE;

    if (iotc_https_parse_url(url, &request) != 0)
    {
        return (int)FSP_ERR_INVALID_ARGUMENT;
    }

    /* For HTTPS, attach the built-in root CA and enable verification +
     * SNI. The driver memcpy()s request.https_conf into its internal
     * httpc_secure_connection_t, but it stores the pointer to the PEM
     * (not a copy of the bytes), so the PEM must remain valid for the
     * lifetime of the request -- s_iotc_https_root_ca_pem is static. */
    if (request.insecure == pdTRUE)
    {
        request.https_conf.ca          = (uint8_t *)s_iotc_https_root_ca_pem;
        request.https_conf.ca_len      = sizeof(s_iotc_https_root_ca_pem); /* includes NUL */
        request.https_conf.auth_mode   = MBEDTLS_SSL_VERIFY_REQUIRED;
        request.https_conf.sni         = request.hostname;
        request.https_conf.sni_len     = (int)strlen(request.hostname);
        request.https_conf.tls_ver_min = MBEDTLS_SSL_VERSION_TLS1_2;
        request.https_conf.tls_ver_max = MBEDTLS_SSL_VERSION_TLS1_2;
    }

    if (body != NULL && body_len > 0U)
    {
        memcpy(request.data, body, body_len);
    }

    /* Prepare for asynchronous completion notification. */
    s_iotc_https_waiter       = xTaskGetCurrentTaskHandle();
    s_iotc_https_result       = 0;
    s_iotc_https_httpc_result = 0;
    s_iotc_https_err          = 0;
    /* Drain any stale notification value. */
    (void)ulTaskNotifyTake(pdTRUE, 0);

    /* Reset response capture state. Only accumulate when caller wants it. */
    s_iotc_https_resp_buf      = NULL;
    s_iotc_https_resp_len      = 0U;
    s_iotc_https_resp_cap      = 0U;
    s_iotc_https_resp_overflow = (resp_out == NULL);  /* skip capture if not wanted */

    err = g_https_w.open(&s_iotc_https_ctrl, &s_iotc_https_cfg);
    if (err != FSP_SUCCESS)
    {
        printf("iotc_https: open failed (err=%d)\n", (int)err);
        s_iotc_https_waiter = NULL;
        return (int)err;
    }

    err = g_https_w.clientSendRequest(&s_iotc_https_ctrl, &request);
    if (err != FSP_SUCCESS)
    {
        printf("iotc_https: clientSendRequest failed (err=%d)\n", (int)err);
        (void)g_https_w.close(&s_iotc_https_ctrl);
        s_iotc_https_waiter = NULL;
        if (s_iotc_https_resp_buf != NULL)
        {
            vPortFree(s_iotc_https_resp_buf);
            s_iotc_https_resp_buf = NULL;
        }
        return (int)err;
    }

    /* Wait for the driver to invoke iotc_https_callback(). */
    bool timed_out = (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(IOTC_HTTPS_REQUEST_TIMEOUT_MS)) == 0U);

    (void)g_https_w.close(&s_iotc_https_ctrl);
    s_iotc_https_waiter = NULL;

    /* Detach the captured buffer from the static state so that subsequent
     * requests don't touch it. */
    char  *captured_buf = s_iotc_https_resp_buf;
    size_t captured_len = s_iotc_https_resp_len;
    bool   captured_ovf = s_iotc_https_resp_overflow;
    s_iotc_https_resp_buf = NULL;
    s_iotc_https_resp_len = 0U;
    s_iotc_https_resp_cap = 0U;

    if (timed_out)
    {
        printf("iotc_https: request timed out\n");
        if (captured_buf != NULL) { vPortFree(captured_buf); }
        return (int)FSP_ERR_TIMEOUT;
    }

    if (s_iotc_https_result < 0)
    {
        /* httpc_result_t: 0=OK 1=UNKNOWN 2=CONNECT 3=HOSTNAME 4=CLOSED
         *                 5=TIMEOUT 6=SVR_RESP 7=MEM 8=LOCAL_ABORT 9=CONTENT_LEN
         * err is the lwIP err_t (negative = lwIP error). */
        printf("iotc_https: request reported error (httpc_result=%lu, err=%d)\n",
               (unsigned long)s_iotc_https_httpc_result, s_iotc_https_err);
        if (captured_buf != NULL) { vPortFree(captured_buf); }
        return (int)FSP_ERR_INVALID_STATE;
    }

    /* Hand the response buffer back to the caller (or discard it). */
    if (resp_out != NULL)
    {
        if (captured_ovf && captured_buf == NULL)
        {
            printf("iotc_https: response too large or out of memory\n");
            return (int)FSP_ERR_OUT_OF_MEMORY;
        }
        *resp_out = captured_buf;
        if (resp_len_out != NULL)
        {
            *resp_len_out = captured_len;
        }
    }
    else if (captured_buf != NULL)
    {
        /* Defensive: caller didn't ask for response but we captured one. */
        vPortFree(captured_buf);
    }

    return (int)FSP_SUCCESS;
}
