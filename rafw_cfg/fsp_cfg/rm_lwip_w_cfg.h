/* generated configuration header file - do not edit */
#ifndef RM_LWIP_W_CFG_H_
#define RM_LWIP_W_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define LWIP_W_CFG_WATCHDOG_SERVICE_ENABLE (1)
#define LWIP_W_CFG_GARP_TIME 60

#ifndef LWIP_DNS
#define LWIP_DNS (0)
#endif
#ifndef LWIP_DHCP
#define LWIP_DHCP (0)
#endif
#ifndef MEMP_MEM_MALLOC
#define MEMP_MEM_MALLOC (1)
#endif
#ifndef __SUPPORT_IPV4__
#define __SUPPORT_IPV4__ (1)
#endif
#ifndef __SUPPORT_IPV6__
#define __SUPPORT_IPV6__ (1)
#endif
#ifndef OS_FREERTOS
#define OS_FREERTOS
#endif
#define FREERTOS_DIRTY (0)
#define __DISABLE_DPM_MOD_IN_SDK__ 
#define LWIP_TESTCASE (1)
#define MBEDTLS_SSL_MAX_CONTENT_LEN 4096
#define HTTPC_REQ_DATA_MAX_SIZE (1024 * 4)
#define RM_LWIP_W
#define RRQ61XX_CUSTOM_FIXES_MANDATORY
#define RM_LWIP_W_CLEANED                     (1)
#define HTTPD_ENABLE_HTTPS                  (1)

#ifdef __cplusplus
}
#endif
#endif /* RM_LWIP_W_CFG_H_ */
