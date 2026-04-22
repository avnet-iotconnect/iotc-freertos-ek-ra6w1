/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#ifdef BSP_MCU_GROUP_RA6W1
#include "r_gpio_w.h"
#else
#include "r_ioport.h"
#endif

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define ADC_WAKEUP_PIN (BSP_IO_PORT_00_PIN_06)
#define EXT_INTR3_PIN (BSP_IO_PORT_00_PIN_10)
#define EXT_INTR0_PIN (BSP_IO_PORT_00_PIN_11)
#define EXT_INTR2_PIN (BSP_IO_PORT_00_PIN_12)
#define EXT_INTR1_PIN (BSP_IO_PORT_00_PIN_13)
#define LED0 (BSP_IO_PORT_01_PIN_14)
extern const ioport_cfg_t g_bsp_pin_cfg; /* RRQ61xxx-EVB */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
