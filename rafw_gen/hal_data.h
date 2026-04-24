/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_ospi_w.h"
#include "r_spi_flash_api.h"
#include "rm_block_media_spi_w.h"
#include "r_rtc_w.h"
#include "rm_wifi.h"
#include "rm_awsiot_w_api.h"
#include "rm_awsiot_w.h"
#include "r_uart_w.h"
#include "r_uart_api.h"
#include "../ra/fsp/src/rm_comms_lock/rm_comms_lock.h"
#include "rm_comms_uart_w.h"
#include "rm_comms_api.h"
FSP_HEADER
extern spi_flash_instance_t g_ospi_aws;
extern ospi_w_instance_ctrl_t g_ospi_aws_ctrl;
extern spi_flash_cfg_t g_ospi_aws_cfg;
/* Block Media on SPI Instance */
extern rm_block_media_instance_t g_rm_block_media0;

/* Access the Block Media SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_block_media_spi_w_instance_ctrl_t g_rm_block_media0_ctrl;
extern const rm_block_media_cfg_t g_rm_block_media0_cfg;

#ifndef NULL
void NULL(rm_block_media_callback_args_t *p_args);
#endif

/** Access the Watchdog Service instance using these structures when calling API functions directly (::p_api is not used). */
extern const wifi_cfg_t g_wifi_cfg;
extern const rm_awsiot_w_app_instance_t g_awsiot_w0;
extern rm_awsiot_w_app_instance_ctrl_t g_awsiot_w0_ctrl;
extern const rm_awsiot_w_app_cfg_t g_awsiot_w0_cfg;
/** UART_W Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern uart_w_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const uart_w_extended_cfg_t g_uart0_cfg_extend;

#ifndef NULL
void NULL(uart_callback_args_t *p_args);
#endif
/* UART Communication Device */
extern const rm_comms_instance_t g_comms_stdio_w;
extern rm_comms_uart_w_instance_ctrl_t g_comms_stdio_w_ctrl;
extern rm_comms_cfg_t g_comms_stdio_w_cfg;

#ifndef NULL
void NULL(rm_comms_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
