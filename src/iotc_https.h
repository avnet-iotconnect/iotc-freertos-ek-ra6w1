/*
 * iotc_https.h
 *
 * Thin wrappers around the Renesas FSP HTTPS client (rm_https_w) and
 * lwIP DNS, providing a small generic interface:
 *
 *   - get_host_address():  resolve a hostname to a dotted-quad IPv4 string.
 *   - http_client_request(): perform a one-shot HTTP/HTTPS request.
 */
#ifndef IOTC_HTTPS_H
#define IOTC_HTTPS_H

#include <stddef.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "rm_https_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Resolve a hostname (or dotted-quad address) to an IPv4 string.
 *
 * Internally calls lwIP's dns_A_Query() and waits up to @p timeout_ms.
 *
 * @param[in]  hostname   DNS name or IPv4 string to resolve.
 * @param[out] ipstr      Caller-provided buffer to receive the dotted-quad
 *                        result. Must be at least 16 bytes.
 * @param[in]  ipstr_len  Size of @p ipstr in bytes.
 * @param[in]  timeout_ms How long to wait for resolution, in milliseconds.
 *
 * @return pdPASS on success (ipstr populated), pdFAIL otherwise.
 */
BaseType_t get_host_address(const char *hostname,
                            char       *ipstr,
                            size_t      ipstr_len,
                            uint32_t    timeout_ms);

/**
 * @brief Perform a single blocking HTTP / HTTPS request.
 *
 * URL must begin with "http://" or "https://". For PUT/POST/PATCH a
 * non-NULL @p body is copied into the request payload.
 *
 * If @p resp_out is non-NULL, the response body is accumulated into a
 * heap-allocated buffer (via pvPortMalloc()) and a NUL-terminated pointer
 * is returned through @p resp_out. The caller owns the buffer and must
 * free it with http_client_response_free() (or vPortFree()) when done.
 * On failure, @p *resp_out is set to NULL and @p *resp_len_out (if
 * provided) is set to 0.
 *
 * Pass @p resp_out == NULL to discard the response body.
 *
 * @param[in]  url           Full request URL.
 * @param[in]  op_code       HTTP_CLIENT_OPCODE_GET / _POST / _PUT / etc.
 * @param[in]  body          Optional request body (may be NULL).
 * @param[in]  body_len      Length of @p body. Must be <= HTTPC_MAX_REQ_DATA.
 * @param[out] resp_out      Optional output: pointer to allocated response
 *                           buffer (caller frees). May be NULL to discard.
 * @param[out] resp_len_out  Optional output: number of response bytes
 *                           stored (excluding the NUL terminator).
 *
 * @return FSP_SUCCESS on success, an FSP error code otherwise.
 */
int http_client_request(const char           *url,
                        https_client_opcode_t op_code,
                        const void           *body,
                        size_t                body_len,
                        char                **resp_out,
                        size_t               *resp_len_out);

/**
 * @brief Free a response buffer returned by http_client_request().
 *
 * Equivalent to vPortFree(resp). Safe to call with NULL.
 */
void http_client_response_free(char *resp);

#ifdef __cplusplus
}
#endif

#endif /* IOTC_HTTPS_H */
