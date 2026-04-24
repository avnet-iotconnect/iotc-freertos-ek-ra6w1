/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_ext_irq_w.h"
#include "r_external_irq_api.h"

#include "rm_pmgr_w_instance.h"
#include "r_ospi_w.h"
#include "r_spi_flash_api.h"
#include "rm_block_media_spi_w.h"
#include "rm_vee_flash_w.h"
#include "rm_vee_flash_w_cfg.h"
#include "rm_map_persistant_w.h"
#include "r_wdog_w.h"
#include "r_wdt_api.h"
#include "rm_watchdog_service_w.h"
#include "rm_watchdog_service_api.h"
#include "mbedtls/platform.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "rm_psa_crypto_w.h"
#include "rm_lwip_w.h"
#include "aws_dev_mode_key_provisioning.h"
#include "rm_littlefs_api.h"
#include "rm_littlefs_flash_w.h"
#include "lfs_util.h"
#include "lfs_util.h"
#include "rm_aws_lwip_sock_wrap_w_api.h"
#include "transport_mbedtls_pkcs11.h"
#include "core_mqtt.h"
#include "r_gpio_w.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/** External IRQ on EXT_IRQ Instance. */
extern const external_irq_instance_t g_external_irq_wakeup;

/** Access the EXT_IRQ instance using these structures when calling API functions directly (::p_api is not used). */
extern ext_irq_w_instance_ctrl_t g_external_irq_wakeup_ctrl;
extern const external_irq_cfg_t g_external_irq_wakeup_cfg;

#ifndef NULL
void NULL(external_irq_callback_args_t *p_args);
#endif

/** External IRQ on EXT_IRQ Instance. */
extern const external_irq_instance_t g_external_irq3;

/** Access the EXT_IRQ instance using these structures when calling API functions directly (::p_api is not used). */
extern ext_irq_w_instance_ctrl_t g_external_irq3_ctrl;
extern const external_irq_cfg_t g_external_irq3_cfg;

#ifndef provisioning_gpio_fr_handler
void provisioning_gpio_fr_handler(external_irq_callback_args_t *p_args);
#endif
extern const pmgr_instance_t g_pmgr_w_ins;
extern pmgr_instance_ctrl_t g_pmgr_w_ctrl;
extern const pmgr_cfg_t g_pmgr_w_cfg;
extern spi_flash_instance_t g_ospi;
extern ospi_w_instance_ctrl_t g_ospi_ctrl;
extern spi_flash_cfg_t g_ospi_cfg;
/* Block Media on SPI Instance */
extern rm_block_media_instance_t g_rm_block_media1;

/* Access the Block Media SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_block_media_spi_w_instance_ctrl_t g_rm_block_media1_ctrl;
extern const rm_block_media_cfg_t g_rm_block_media1_cfg;

#ifndef rm_vee_media_callback
void rm_vee_media_callback(rm_block_media_callback_args_t *p_args);
#endif
extern const rm_vee_instance_t g_vee0;
extern rm_vee_flash_w_instance_ctrl_t g_vee0_ctrl;
extern const rm_vee_cfg_t g_vee0_cfg;

/** Callback used by VEE Instance. */
#ifndef NULL
void NULL(rm_vee_callback_args_t *p_args);
#endif
extern const map_persistant_w_instance_t g_map_persistant_w_instance;
extern map_persistant_w_instance_ctrl_t g_map_persistant_w_ctrl;
extern const map_persistant_w_cfg_t g_map_persistant_w_cfg;
/** WDT on WDT Instance. */
extern const wdt_instance_t g_wdt0;

/** Access the WDT instance using these structures when calling API functions directly (::p_api is not used). */
extern wdog_w_instance_ctrl_t g_wdt0_ctrl;
extern const wdt_cfg_t g_wdt0_cfg;

#ifndef wdt_callback_default
void wdt_callback_default(wdt_callback_args_t *p_args);
#endif
/** Watchdog Service on Watchdog Service Instance. */
extern const watchdog_service_instance_t g_watchdog_service0;

/** Access the Watchdog Service instance using these structures when calling API functions directly (::p_api is not used). */
extern watchdog_service_w_instance_ctrl_t g_watchdog_service0_ctrl;
extern const watchdog_service_cfg_t g_watchdog_service0_cfg;
/** Access the lwIP instance using these structures when calling API functions directly (::p_api is not used). */
extern const lwip_w_cfg_t g_lwip0_cfg;
extern spi_flash_instance_t g_ospi_lfs;
extern ospi_w_instance_ctrl_t g_ospi_lfs_ctrl;
extern spi_flash_cfg_t g_ospi_lfs_cfg;
/* Block Media on SPI Instance */
extern rm_block_media_instance_t g_rm_block_media2;

/* Access the Block Media SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_block_media_spi_w_instance_ctrl_t g_rm_block_media2_ctrl;
extern const rm_block_media_cfg_t g_rm_block_media2_cfg;

#ifndef NULL
void NULL(rm_block_media_callback_args_t *p_args);
#endif
/** LittleFS on Flash Instance. */
extern const rm_littlefs_instance_t g_rm_littlefs0;
extern rm_littlefs_flash_w_instance_ctrl_t g_rm_littlefs0_ctrl;
extern const rm_littlefs_cfg_t g_rm_littlefs0_cfg;

extern struct lfs g_rm_littlefs0_lfs;
extern const struct lfs_config g_rm_littlefs0_lfs_cfg;

/** External IRQ on EXT_IRQ Instance. */
extern const external_irq_instance_t g_external_irq2;

/** Access the EXT_IRQ instance using these structures when calling API functions directly (::p_api is not used). */
extern ext_irq_w_instance_ctrl_t g_external_irq2_ctrl;
extern const external_irq_cfg_t g_external_irq2_cfg;

#ifndef NULL
void NULL(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on EXT_IRQ Instance. */
extern const external_irq_instance_t g_external_irq1;

/** Access the EXT_IRQ instance using these structures when calling API functions directly (::p_api is not used). */
extern ext_irq_w_instance_ctrl_t g_external_irq1_ctrl;
extern const external_irq_cfg_t g_external_irq1_cfg;

#ifndef srm_wifi_app_gpio_p0_fr_handler
void srm_wifi_app_gpio_p0_fr_handler(external_irq_callback_args_t *p_args);
#endif
/** External IRQ on EXT_IRQ Instance. */
extern const external_irq_instance_t g_external_irq0;

/** Access the EXT_IRQ instance using these structures when calling API functions directly (::p_api is not used). */
extern ext_irq_w_instance_ctrl_t g_external_irq0_ctrl;
extern const external_irq_cfg_t g_external_irq0_cfg;

#ifndef NULL
void NULL(external_irq_callback_args_t *p_args);
#endif
#define IOPORT_CFG_NAME g_bsp_pin_cfg
#define IOPORT_CFG_OPEN R_GPIO_W_Open
#define IOPORT_CFG_CTRL g_ioport_ctrl

/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern gpio_w_instance_ctrl_t g_ioport_ctrl;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
