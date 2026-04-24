/* generated HAL source file - do not edit */
#include "hal_data.h"
static spi_flash_erase_command_t g_ospi_aws_erase_command_list[] = {
#if ((0x20 > 0) && (4096 > 0))
		{ .command = 0x20, .size = 4096 },
#endif
#if ((0x52 > 0) && (32768 > 0))
		{ .command = 0x52, .size = 32768 },
#endif
#if (0xD8 > 0)
		{ .command = 0xD8, .size = 65536 },
#endif
		};

ospi_w_xspi_command_set_t g_ospi_aws_high_speed_command_set = { .protocol =
		SPI_FLASH_PROTOCOL_1S_4S_4S, .command_bytes = OSPI_W_COMMAND_BYTES_1,
		.read_command = 0xEB, .page_program_command = 0x32,
		.write_enable_command = 0x06, .status_command = 0x05,
		.read_dummy_cycles = 0x02, .program_dummy_cycles = 0,
		.status_dummy_cycles = 0,

		.p_erase_command_list = NULL, /* Use the default commands spi_flash_cfg_t */
		.erase_command_list_length = 0,

};

ospi_w_device_config_t g_ospi_aws_hw_cfg =
		{ .clk_div = OSPI_W_DEVICE_CLK_DIV_2, .bus_mode =
				OSPI_W_DEVICE_BUS_MODE_SINGLE, .access_mode =
				OSPI_W_DEVICE_ACCESS_MODE_AUTO, .clock_mode =
				OSPI_W_DEVICE_CLK_MODE_HIGH, .io2_dir =
				OSPI_W_DEVICE_IO_DIR_AUTO_SEL, .io2_value =
				OSPI_W_DEVICE_IO_VALUE_LOW, .io3_dir =
				OSPI_W_DEVICE_IO_DIR_AUTO_SEL, .io3_value =
				OSPI_W_DEVICE_IO_VALUE_LOW, .io4_7_dir =
				OSPI_W_DEVICE_IO_DIR_AUTO_SEL, .io4_7_value =
				OSPI_W_DEVICE_IO4_7_VALUE_0000, .hready_mode =
				OSPI_W_DEVICE_HREADY_MODE_WAIT, .sampling_edge =
				OSPI_W_DEVICE_SAMPLING_EDGE_NEG, .read_pipe =
				OSPI_W_DEVICE_READ_PIPE_ENABLE, .read_pipe_delay =
				OSPI_W_DEVICE_READ_PIPE_DELAY_7, .address_size =
				OSPI_W_DEVICE_ADDR_SIZE_24, .dummy_mode =
				OSPI_W_DEVICE_DUMMY_MODE_LAST_2_CLK, .slew_rate =
				OSPI_W_DEVICE_SLEW_RATE_0, .drive_current =
				OSPI_W_DEVICE_DRIVE_CURRENT_12,
				.manualmode_config.dir_change_mode =
						OSPI_W_DEVICE_DIR_CHANGE_MODE_DUMMY_ACCESS,
				.manualmode_config.mapped_addr_rd_acc_response =
						OSPI_W_DEVICE_MAPPED_ADDR_RD_ACC_RESPONSE_IGNORE,
				.automode_config.full_buffer_mode =
						OSPI_W_DEVICE_FULL_BUFFER_MODE_BLOCK,
				.automode_config.instruct_size =
						OSPI_W_DEVICE_INSTRUCT_SZ_1_BYTE,
				.automode_config.burst_len_limit =
						OSPI_W_DEVICE_BURST_LEN_LIMIT_UNSPECIFIED, };

ospi_w_device_read_instr_config_t g_ospi_aws_read_instr_config = {
#if (0xEB > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0xEB, .instr_extra_byte = 0xA0, .instr_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_SINGLE, .addr_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_QUAD, .extra_byte_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_QUAD, .dummy_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_QUAD, .data_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_QUAD, .extra_byte_cfg =
				OSPI_W_DEVICE_EXTRA_BYTE_ENABLE, .extra_byte_half_cfg =
				OSPI_W_DEVICE_EXTRA_BYTE_HALF_DISABLE, .dummy_bytes = 0x02,
		.instr_mode = OSPI_W_DEVICE_INSTR_MODE_SEND_ONCE, .idle_state_duration =
				OSPI_W_DEVICE_IDLE_STATE_DURATION_3, };

ospi_w_device_wrap_burst_instr_config_t g_ospi_aws_wrap_burst_instr_config = {
		.enable = 0, };

ospi_w_device_read_status_config_t g_ospi_aws_read_status_config = {
#if (0x05 > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0x05, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE,
		.receive_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, .dummy_bus_mode =
				OSPI_W_DEVICE_BUS_MODE_SINGLE, .busy_pos = 0, .busy_val =
				OSPI_W_DEVICE_BUSY_HIGH, .read_stat_del = 0,

		.read_stat_reg_cnt = OSPI_W_DEVICE_READ_STATUS_REG_CNT_RESSTS,
		.dummy_bytes = 0, .dummy_val = 0, };

ospi_w_device_erase_instr_config_t g_ospi_aws_erase_instr_config = {
#if (0x20 > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0x20, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE,
		.addr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, .hclk_cycles = 14,
		.cs_hi_cycles = 30, };

ospi_w_device_write_enable_instr_config_t g_ospi_aws_write_enable_instr_config =
		{
#if (0x06 > 0)
				.enable = 1,
#else
            .enable = 0,
            #endif
				.instr = 0x06, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, };

ospi_w_device_suspend_resume_instr_config_t g_ospi_aws_suspend_resume_instr_config =
		{
#if ((0x75 > 0) && (0x7A > 0))
				.enable = 1,
#else
            .enable = 0,
            #endif
				.suspend_instr = 0x75, .resume_instr = 0x7A, .suspend_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .resume_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .read_stat_del = 128, };

ospi_w_extended_cfg_t g_ospi_aws_exteneded_cfg = { .channel = 0,
		.p_timing_settings = NULL, .p_xspi_command_set_list =
				&g_ospi_aws_high_speed_command_set,
		.xspi_command_set_list_length = 1,
		.p_autocalibration_preamble_pattern_addr = NULL,
		.data_latch_delay_clocks = 0,
#if OSPI_W_CFG_DMAC_SUPPORT_ENABLE
            transfer_instance_t const * p_lower_lvl_transfer;
        #endif
		// automode erase support
		.p_ospi_w_device_cfg = &g_ospi_aws_hw_cfg, .p_read_instr_cfg =
				&g_ospi_aws_read_instr_config, .p_wrap_burst_instr_cfg =
				&g_ospi_aws_wrap_burst_instr_config, .p_read_status_instr_cfg =
				&g_ospi_aws_read_status_config, .p_write_enable_instr_cfg =
				&g_ospi_aws_write_enable_instr_config, .p_erase_instr_cfg =
				&g_ospi_aws_erase_instr_config, .p_suspend_resume_instr_cfg =
				&g_ospi_aws_suspend_resume_instr_config, };
spi_flash_cfg_t g_ospi_aws_cfg = { .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
		.read_mode = SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO, .address_bytes =
				SPI_FLASH_ADDRESS_BYTES_3, .dummy_clocks =
				SPI_FLASH_DUMMY_CLOCKS_2, /* RRQ61XXX EVK default  */
		.page_program_address_lines = SPI_FLASH_DATA_LINES_4, .page_size_bytes =
				256, .write_status_bit = 0, .write_enable_bit = 1,
		.page_program_command = 0x32, .write_enable_command = 0x06,
		.status_command = 0x05, .read_command = 0xEB,
#if OSPI_W_CFG_XIP_SUPPORT_ENABLE
                .xip_enter_command           = 0U,   /* RA6W1 always runs under xip */
                .xip_exit_command            = 0U,
            #else
		.xip_enter_command = 0U, .xip_exit_command = 0U,
#endif
		.erase_command_list_length = sizeof(g_ospi_aws_erase_command_list)
				/ sizeof(g_ospi_aws_erase_command_list[0]),
		.p_erase_command_list = &g_ospi_aws_erase_command_list[0], .p_extend =
				&g_ospi_aws_exteneded_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
ospi_w_instance_ctrl_t g_ospi_aws_ctrl;
spi_flash_instance_t g_ospi_aws = { .p_ctrl = &g_ospi_aws_ctrl, .p_cfg =
		&g_ospi_aws_cfg, .p_api = &g_ospi_w_on_spi_flash, };
rm_block_media_spi_w_instance_ctrl_t g_rm_block_media0_ctrl;

#define FSP_NOT_DEFINED 0xFFFFFFFF
static const rm_block_media_spi_w_extended_cfg_t g_rm_block_media0_extended_cfg =
		{
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
                .p_spi             = &FSP_NOT_DEFINED,
                .base_address      = 0,
            #elif (FSP_NOT_DEFINED != g_ospi_aws)
				.p_spi = &g_ospi_aws, .base_address = 0,
#else
                .p_spi             = &g_ospi_aws,
                .base_address      = 0,
            #endif
				.block_count_total = 8192, .block_size_bytes = 4096, };
#undef FSP_NOT_DEFINED

const rm_block_media_cfg_t g_rm_block_media0_cfg = { .p_callback = NULL,
		.p_context = NULL, .p_extend = &g_rm_block_media0_extended_cfg };
rm_block_media_instance_t g_rm_block_media0 = { .p_ctrl =
		&g_rm_block_media0_ctrl,

.p_cfg = &g_rm_block_media0_cfg, .p_api = &g_rm_block_media_on_spi };
const rtc_extended_cfg_t rtc_w_cfg_extend = { .reserved = 0, };

static const rtc_cfg_t g_rtc_w_test_cfg = { .p_extend = &rtc_w_cfg_extend, };

static rtc_w_instance_ctrl_t g_rtc_w_test_ctrl;

/* Instance structure to use this module. */
rtc_instance_t g_rtc_w_inst_obj = { .p_api = &g_rtc_on_rtc_w, .p_cfg =
		&g_rtc_w_test_cfg, .p_ctrl = &g_rtc_w_test_ctrl, };
/* Wi-Fi configuration. */
const wifi_cfg_t g_wifi_cfg = { .p_watchdog_service = &g_watchdog_service0,
		.p_vee_service = &g_vee0, .p_rtc_w_service = &g_rtc_w_inst_obj,
		.p_context = NULL, .p_extend = NULL, .coex_enabled = 0, .coex_cfg = {
				.coex_asc1_pin = (BSP_IO_PORT_00_PIN_07), .coex_asc2_pin =
						(BSP_IO_PORT_00_PIN_06), .coex_bt_act_pin =
						(BSP_IO_PORT_01_PIN_15), .coex_asc1_reset_val = 01,
				.coex_asc2_reset_val = 00, } };
rm_awsiot_w_app_instance_ctrl_t g_awsiot_w0_ctrl;

const rm_awsiot_w_app_cfg_t g_awsiot_w0_cfg = { .p_flash_instance =
		g_rm_block_media0_extended_cfg.p_spi, .p_context = NULL, .p_extend =
		NULL, };

const rm_awsiot_w_app_instance_t g_awsiot_w0 = { .p_ctrl = &g_awsiot_w0_ctrl,
		.p_cfg = &g_awsiot_w0_cfg, .p_api = &g_rm_awsiot_w };
uart_w_instance_ctrl_t g_uart0_ctrl;
/** UART_W extended configuration for UART HAL driver */
uart_w_baud_setting_t g_uart0_baud_setting = { .fra_baud = 26, .int_baud = 43 };

/** UART extended configuration for UART_W HAL driver */
const uart_w_extended_cfg_t g_uart0_cfg_extend =
		{ .fifo_enable = UART_W_FIFO_ENABLE, .rx_fifo_trigger =
				UART_W_RX_FIFO_TRIGGER_SEVEN_EIGHTHS, .tx_fifo_trigger =
				UART_W_TX_FIFO_TRIGGER_EIGHTH, .extended_data_bits_enable =
				UART_W_EXTENDED_DATA_BITS_DISABLE, .data_bits =
				UART_W_DATA_BITS_8, .p_baud_setting = &g_uart0_baud_setting,
				.flow_control = UART_W_AUTO_FLOW_CONTROL_DISABLED,
				.loop_back_enable = UART_W_LOOP_BACK_DISABLE, .rs485_enable =
						UART_W_RS485_DISABLE, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg = { .channel = 0, .data_bits = UART_DATA_BITS_8,
		.parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
				NULL, .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define FSP_NOT_DEFINED (1)
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
		.p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &FSP_NOT_DEFINED,
#endif
#if (FSP_NOT_DEFINED == FSP_NOT_DEFINED)
		.p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &FSP_NOT_DEFINED,
#endif
#undef FSP_NOT_DEFINED
		.rxi_ipl = (12),
#if defined(VECTOR_NUMBER_UART0_INT)
                .rxi_irq             = VECTOR_NUMBER_UART0_INT,
#else
		.rxi_irq = FSP_INVALID_VECTOR,
#endif

		};

/* Instance structure to use this module. */
const uart_instance_t g_uart0 = { .p_ctrl = &g_uart0_ctrl,
		.p_cfg = &g_uart0_cfg, .p_api = &g_uart_w };
/* UART Communication Device */

rm_comms_uart_w_instance_ctrl_t g_comms_stdio_w_ctrl;

#if BSP_CFG_RTOS == 1 // ThreadX

 #if !defined(g_comms_stdio_w_tx_mutex)
 rm_comms_mutex_t g_comms_stdio_w_tx_mutex =
 {
     .p_name = "g_comms_stdio_w tx mutex",
 };
 #endif

 #if !defined(g_comms_stdio_w_rx_mutex)
 rm_comms_mutex_t g_comms_stdio_w_rx_mutex =
 {
     .p_name = "g_comms_stdio_w rx mutex",
 };
 #endif

 #if !defined(g_comms_stdio_w_tx_semaphore)
 rm_comms_semaphore_t g_comms_stdio_w_tx_semaphore =
 {
     .p_name = "g_comms_stdio_w tx semaphore",
 };
 #endif

  #if !defined(g_comms_stdio_w_rx_semaphore)
 rm_comms_semaphore_t g_comms_stdio_w_rx_semaphore =
 {
     .p_name = "g_comms_stdio_w rx semaphore",
 };
 #endif

#elif BSP_CFG_RTOS == 2 // FreeRTOS

#if !defined(g_comms_stdio_w_tx_mutex)
rm_comms_mutex_t g_comms_stdio_w_tx_mutex;
#endif

#if !defined(g_comms_stdio_w_rx_mutex)
rm_comms_mutex_t g_comms_stdio_w_rx_mutex;
#endif
#if !defined(g_comms_stdio_w_tx_semaphore)
rm_comms_semaphore_t g_comms_stdio_w_tx_semaphore;
#endif

#if !defined(g_comms_stdio_w_rx_semaphore)
rm_comms_semaphore_t g_comms_stdio_w_rx_semaphore;
#endif

#else

#endif

rm_comms_uart_w_extended_cfg_t g_comms_stdio_w_extended_cfg = {
#if BSP_CFG_RTOS

#if !defined(g_comms_stdio_w_tx_mutex)
    .p_tx_mutex = &g_comms_stdio_w_tx_mutex,
#else
    .p_tx_mutex = NULL,
#endif

#if !defined(g_comms_stdio_w_rx_mutex)
    .p_rx_mutex = &g_comms_stdio_w_rx_mutex,
#else
    .p_rx_mutex = NULL,
#endif

#if !defined(g_comms_stdio_w_tx_semaphore)
    .p_tx_semaphore = &g_comms_stdio_w_tx_semaphore,
#else
    .p_tx_semaphore = NULL,
#endif

#if !defined(g_comms_stdio_w_rx_semaphore)
    .p_rx_semaphore = &g_comms_stdio_w_rx_semaphore,
#else
    .p_rx_semaphore = NULL,
#endif
    .mutex_timeout  = 0xFFFFFFFF,
#endif
		.buff_ovrw_prot = 0, .p_uart = &g_uart0, };

rm_comms_cfg_t g_comms_stdio_w_cfg = { .semaphore_timeout = 0xFFFFFFFF,
		.p_lower_level_cfg = NULL, .p_extend =
				(void*) &g_comms_stdio_w_extended_cfg, .p_callback = NULL, };

const rm_comms_instance_t g_comms_stdio_w = { .p_ctrl = &g_comms_stdio_w_ctrl,
		.p_cfg = &g_comms_stdio_w_cfg, .p_api = &g_comms_on_comms_uart_w, };
void g_hal_init(void) {
	g_common_init();
}
