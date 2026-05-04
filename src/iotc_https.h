/*
 * iotc_https.h
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
// Perform a single blocking HTTP or HTTPS request.
int http_client_request(
    const char *url,
    https_client_opcode_t op_code,
    const void *body,
    size_t body_len,
    char **resp_out,
    size_t *resp_len_out
);

// Free a response buffer returned by http_client_request().
void http_client_response_free(char *resp);

#ifdef __cplusplus
}
#endif

#endif /* IOTC_HTTPS_H */
