/* generated common source file - do not edit */
#include "common_data.h"
ext_irq_w_extended_cfg_t g_external_irq_wakeup_ext_cfg = {
#ifdef EXT_INTR0_PIN
    .irq_pin = EXT_INTR0_PIN,
#endif
		};

ext_irq_w_instance_ctrl_t g_external_irq_wakeup_ctrl;
const external_irq_cfg_t g_external_irq_wakeup_cfg = { .channel = 0, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING, .p_callback = NULL, .p_context = NULL,
		.p_extend = &g_external_irq_wakeup_ext_cfg, .ipl = (12), .irq =
				FSP_INVALID_VECTOR, };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq_wakeup = { .p_ctrl =
		&g_external_irq_wakeup_ctrl, .p_cfg = &g_external_irq_wakeup_cfg,
		.p_api = &g_external_irq_on_ext_irq_w };

ext_irq_w_extended_cfg_t g_external_irq3_ext_cfg = {
#ifdef EXT_INTR3_PIN
    .irq_pin = EXT_INTR3_PIN,
#endif
		};

ext_irq_w_instance_ctrl_t g_external_irq3_ctrl;
const external_irq_cfg_t g_external_irq3_cfg = { .channel = 3, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING, .p_callback = provisioning_gpio_fr_handler,
		.p_context = NULL, .p_extend = &g_external_irq3_ext_cfg, .ipl = (12),
		.irq = FSP_INVALID_VECTOR, };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq3 = { .p_ctrl =
		&g_external_irq3_ctrl, .p_cfg = &g_external_irq3_cfg, .p_api =
		&g_external_irq_on_ext_irq_w };
const pmgr_cfg_t g_pmgr_w_cfg;
pmgr_instance_ctrl_t g_pmgr_w_ctrl;

const pmgr_instance_t g_pmgr_w_ins = { .p_ctrl = &g_pmgr_w_ctrl, .p_cfg =
		&g_pmgr_w_cfg, .p_api = &g_pmgr_w_api };
static spi_flash_erase_command_t g_ospi_erase_command_list[] = {
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

ospi_w_xspi_command_set_t g_ospi_high_speed_command_set = { .protocol =
		SPI_FLASH_PROTOCOL_1S_4S_4S, .command_bytes = OSPI_W_COMMAND_BYTES_1,
		.read_command = 0xEB, .page_program_command = 0x32,
		.write_enable_command = 0x06, .status_command = 0x05,
		.read_dummy_cycles = 0x02, .program_dummy_cycles = 0,
		.status_dummy_cycles = 0,

		.p_erase_command_list = NULL, /* Use the default commands spi_flash_cfg_t */
		.erase_command_list_length = 0,

};

ospi_w_device_config_t g_ospi_hw_cfg =
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

ospi_w_device_read_instr_config_t g_ospi_read_instr_config = {
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

ospi_w_device_wrap_burst_instr_config_t g_ospi_wrap_burst_instr_config = {
		.enable = 0, };

ospi_w_device_read_status_config_t g_ospi_read_status_config = {
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

ospi_w_device_erase_instr_config_t g_ospi_erase_instr_config = {
#if (0x20 > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0x20, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE,
		.addr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, .hclk_cycles = 14,
		.cs_hi_cycles = 30, };

ospi_w_device_write_enable_instr_config_t g_ospi_write_enable_instr_config = {
#if (0x06 > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0x06, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, };

ospi_w_device_suspend_resume_instr_config_t g_ospi_suspend_resume_instr_config =
		{
#if ((0x75 > 0) && (0x7A > 0))
				.enable = 1,
#else
            .enable = 0,
            #endif
				.suspend_instr = 0x75, .resume_instr = 0x7A, .suspend_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .resume_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .read_stat_del = 128, };

ospi_w_extended_cfg_t g_ospi_exteneded_cfg = { .channel = 0,
		.p_timing_settings = NULL, .p_xspi_command_set_list =
				&g_ospi_high_speed_command_set, .xspi_command_set_list_length =
				1, .p_autocalibration_preamble_pattern_addr = NULL,
		.data_latch_delay_clocks = 0,
#if OSPI_W_CFG_DMAC_SUPPORT_ENABLE
            transfer_instance_t const * p_lower_lvl_transfer;
        #endif
		// automode erase support
		.p_ospi_w_device_cfg = &g_ospi_hw_cfg, .p_read_instr_cfg =
				&g_ospi_read_instr_config, .p_wrap_burst_instr_cfg =
				&g_ospi_wrap_burst_instr_config, .p_read_status_instr_cfg =
				&g_ospi_read_status_config, .p_write_enable_instr_cfg =
				&g_ospi_write_enable_instr_config, .p_erase_instr_cfg =
				&g_ospi_erase_instr_config, .p_suspend_resume_instr_cfg =
				&g_ospi_suspend_resume_instr_config, };
spi_flash_cfg_t g_ospi_cfg =
		{ .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S, .read_mode =
				SPI_FLASH_READ_MODE_FAST_READ_QUAD_IO, .address_bytes =
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
				.erase_command_list_length = sizeof(g_ospi_erase_command_list)
						/ sizeof(g_ospi_erase_command_list[0]),
				.p_erase_command_list = &g_ospi_erase_command_list[0],
				.p_extend = &g_ospi_exteneded_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
ospi_w_instance_ctrl_t g_ospi_ctrl;
spi_flash_instance_t g_ospi = { .p_ctrl = &g_ospi_ctrl, .p_cfg = &g_ospi_cfg,
		.p_api = &g_ospi_w_on_spi_flash, };
rm_block_media_spi_w_instance_ctrl_t g_rm_block_media1_ctrl;

#define FSP_NOT_DEFINED 0xFFFFFFFF
static const rm_block_media_spi_w_extended_cfg_t g_rm_block_media1_extended_cfg =
		{
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
                .p_spi             = &FSP_NOT_DEFINED,
                .base_address      = BSP_FEATURE_FLASH_DATA_FLASH_START,
            #elif (FSP_NOT_DEFINED != g_ospi)
				.p_spi = &g_ospi, .base_address =
						BSP_FEATURE_FLASH_DATA_FLASH_START,
#else
                .p_spi             = &g_ospi,
                .base_address      = BSP_FEATURE_FLASH_DATA_FLASH_START,
            #endif
				.block_count_total = 512, .block_size_bytes = 4096, };
#undef FSP_NOT_DEFINED

const rm_block_media_cfg_t g_rm_block_media1_cfg = { .p_callback =
		rm_vee_media_callback, .p_context = &g_vee0_ctrl, .p_extend =
		&g_rm_block_media1_extended_cfg };
rm_block_media_instance_t g_rm_block_media1 = { .p_ctrl =
		&g_rm_block_media1_ctrl,

.p_cfg = &g_rm_block_media1_cfg, .p_api = &g_rm_block_media_on_spi };
rm_vee_flash_w_instance_ctrl_t g_vee0_ctrl;

#define FSP_NOT_DEFINED	(0xFFFFFFFF)
const rm_vee_flash_w_cfg_t g_vee0_cfg_ext = { .p_flash = NULL,
#if (FSP_NOT_DEFINED == g_rm_block_media1)
    .p_media = NULL,
#else
		.p_media = &g_rm_block_media1,
#endif
		};
#undef FSP_NOT_DEFINED  

static uint16_t g_vee0_record_offset[2048 + 1] = { 0 };

const rm_vee_cfg_t g_vee0_cfg = { .start_addr =
		BSP_FEATURE_FLASH_DATA_FLASH_START, .num_segments = 2, .total_size =
		BSP_DATA_FLASH_SIZE_BYTES / 2, .ref_data_size =
		RM_VEE_FLASH_W_REF_DATA_SIZE, .record_max_id = 2048, .rec_offset =
		&g_vee0_record_offset[0], .p_callback = NULL, .p_context = NULL,
		.p_extend = &g_vee0_cfg_ext };

/* Instance structure to use this module. */
const rm_vee_instance_t g_vee0 = { .p_ctrl = &g_vee0_ctrl, .p_cfg = &g_vee0_cfg,
		.p_api = &g_rm_vee_on_flash };
const map_persistant_w_cfg_t g_map_persistant_w_cfg;
map_persistant_w_instance_ctrl_t g_map_persistant_w_ctrl;

const map_persistant_w_instance_t g_map_persistant_w_instance = { .p_ctrl =
		&g_map_persistant_w_ctrl, .p_cfg = &g_map_persistant_w_cfg, .p_api =
		&g_map_persistant_w_api };
wdog_w_instance_ctrl_t g_wdt0_ctrl;

const wdt_cfg_t g_wdt0_cfg = { .timeout = 400, .reset_control =
		WDT_RESET_CONTROL_NMI, .p_callback = wdt_callback_default, };

/* Instance structure to use this module. */
const wdt_instance_t g_wdt0 = { .p_ctrl = &g_wdt0_ctrl, .p_cfg = &g_wdt0_cfg,
		.p_api = &g_wdt_on_wdog_w };
/* Watchdog Service control instance. */
watchdog_service_w_instance_ctrl_t g_watchdog_service0_ctrl;

/* Watchdog Service configuration. */
const watchdog_service_cfg_t g_watchdog_service0_cfg = { .p_wdt = &g_wdt0,
		.p_context = NULL, .p_extend = NULL, };

/* Instance structure to use Watchdog Service module. */
const watchdog_service_instance_t g_watchdog_service0 = { .p_ctrl =
		&g_watchdog_service0_ctrl, .p_cfg = &g_watchdog_service0_cfg, .p_api =
		&g_watchdog_service_on_watchdog_service_w, };

/* WDT default Callback. */
void wdt_callback_default(wdt_callback_args_t *p_args) {
	FSP_PARAMETER_NOT_USED(p_args);
}

/* lwIP configuration. */
const lwip_w_cfg_t g_lwip0_cfg = {
#if LWIP_W_CFG_WATCHDOG_SERVICE_ENABLE
    .p_watchdog_service = &g_watchdog_service0,
#endif
		.p_extend = NULL, };
lwip_w_cfg_t const *gp_lwip_w_cfg = &g_lwip0_cfg;
#ifndef vLoggingPrintf
#include <stdarg.h>

void vLoggingPrintf(const char *pcFormat, ...) {
	FSP_PARAMETER_NOT_USED(pcFormat);
}
#endif

#ifndef vLoggingPrint
void vLoggingPrint(const char *pcFormat);

void vLoggingPrint(const char *pcFormat) {
	FSP_PARAMETER_NOT_USED(pcFormat);
}
#endif
static spi_flash_erase_command_t g_ospi_lfs_erase_command_list[] = {
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

ospi_w_xspi_command_set_t g_ospi_lfs_high_speed_command_set = { .protocol =
		SPI_FLASH_PROTOCOL_1S_4S_4S, .command_bytes = OSPI_W_COMMAND_BYTES_1,
		.read_command = 0xEB, .page_program_command = 0x32,
		.write_enable_command = 0x06, .status_command = 0x05,
		.read_dummy_cycles = 0x02, .program_dummy_cycles = 0,
		.status_dummy_cycles = 0,

		.p_erase_command_list = NULL, /* Use the default commands spi_flash_cfg_t */
		.erase_command_list_length = 0,

};

ospi_w_device_config_t g_ospi_lfs_hw_cfg =
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

ospi_w_device_read_instr_config_t g_ospi_lfs_read_instr_config = {
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

ospi_w_device_wrap_burst_instr_config_t g_ospi_lfs_wrap_burst_instr_config = {
		.enable = 0, };

ospi_w_device_read_status_config_t g_ospi_lfs_read_status_config = {
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

ospi_w_device_erase_instr_config_t g_ospi_lfs_erase_instr_config = {
#if (0x20 > 0)
		.enable = 1,
#else
            .enable = 0,
            #endif
		.instr = 0x20, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE,
		.addr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, .hclk_cycles = 14,
		.cs_hi_cycles = 30, };

ospi_w_device_write_enable_instr_config_t g_ospi_lfs_write_enable_instr_config =
		{
#if (0x06 > 0)
				.enable = 1,
#else
            .enable = 0,
            #endif
				.instr = 0x06, .instr_bus_mode = OSPI_W_DEVICE_BUS_MODE_SINGLE, };

ospi_w_device_suspend_resume_instr_config_t g_ospi_lfs_suspend_resume_instr_config =
		{
#if ((0x75 > 0) && (0x7A > 0))
				.enable = 1,
#else
            .enable = 0,
            #endif
				.suspend_instr = 0x75, .resume_instr = 0x7A, .suspend_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .resume_bus_mode =
						OSPI_W_DEVICE_BUS_MODE_SINGLE, .read_stat_del = 128, };

ospi_w_extended_cfg_t g_ospi_lfs_exteneded_cfg = { .channel = 0,
		.p_timing_settings = NULL, .p_xspi_command_set_list =
				&g_ospi_lfs_high_speed_command_set,
		.xspi_command_set_list_length = 1,
		.p_autocalibration_preamble_pattern_addr = NULL,
		.data_latch_delay_clocks = 0,
#if OSPI_W_CFG_DMAC_SUPPORT_ENABLE
            transfer_instance_t const * p_lower_lvl_transfer;
        #endif
		// automode erase support
		.p_ospi_w_device_cfg = &g_ospi_lfs_hw_cfg, .p_read_instr_cfg =
				&g_ospi_lfs_read_instr_config, .p_wrap_burst_instr_cfg =
				&g_ospi_lfs_wrap_burst_instr_config, .p_read_status_instr_cfg =
				&g_ospi_lfs_read_status_config, .p_write_enable_instr_cfg =
				&g_ospi_lfs_write_enable_instr_config, .p_erase_instr_cfg =
				&g_ospi_lfs_erase_instr_config, .p_suspend_resume_instr_cfg =
				&g_ospi_lfs_suspend_resume_instr_config, };
spi_flash_cfg_t g_ospi_lfs_cfg = { .spi_protocol = SPI_FLASH_PROTOCOL_1S_1S_1S,
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
		.erase_command_list_length = sizeof(g_ospi_lfs_erase_command_list)
				/ sizeof(g_ospi_lfs_erase_command_list[0]),
		.p_erase_command_list = &g_ospi_lfs_erase_command_list[0], .p_extend =
				&g_ospi_lfs_exteneded_cfg, };
/** This structure encompasses everything that is needed to use an instance of this interface. */
ospi_w_instance_ctrl_t g_ospi_lfs_ctrl;
spi_flash_instance_t g_ospi_lfs = { .p_ctrl = &g_ospi_lfs_ctrl, .p_cfg =
		&g_ospi_lfs_cfg, .p_api = &g_ospi_w_on_spi_flash, };
rm_block_media_spi_w_instance_ctrl_t g_rm_block_media2_ctrl;

#define FSP_NOT_DEFINED 0xFFFFFFFF
static const rm_block_media_spi_w_extended_cfg_t g_rm_block_media2_extended_cfg =
		{
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
                .p_spi             = &FSP_NOT_DEFINED,
                .base_address      = 0x2A770000,
            #elif (FSP_NOT_DEFINED != g_ospi_lfs)
				.p_spi = &g_ospi_lfs, .base_address = 0x2A770000,
#else
                .p_spi             = &g_ospi_lfs,
                .base_address      = 0x2A770000,
            #endif
				.block_count_total = 128, .block_size_bytes = 4096, };
#undef FSP_NOT_DEFINED

const rm_block_media_cfg_t g_rm_block_media2_cfg = { .p_callback = NULL,
		.p_context = NULL, .p_extend = &g_rm_block_media2_extended_cfg };
rm_block_media_instance_t g_rm_block_media2 = { .p_ctrl =
		&g_rm_block_media2_ctrl,

.p_cfg = &g_rm_block_media2_cfg, .p_api = &g_rm_block_media_on_spi };
rm_littlefs_flash_w_instance_ctrl_t g_rm_littlefs0_ctrl;

#ifdef LFS_NO_MALLOC
static uint8_t g_rm_littlefs0_read[64];
static uint8_t g_rm_littlefs0_prog[64];
static uint8_t g_rm_littlefs0_lookahead[16];
#endif

struct lfs g_rm_littlefs0_lfs;

const struct lfs_config g_rm_littlefs0_lfs_cfg = { .context =
		&g_rm_littlefs0_ctrl, .read = &rm_littlefs_flash_w_read, .prog =
		&rm_littlefs_flash_write, .erase = &rm_littlefs_flash_w_erase, .sync =
		&rm_littlefs_flash_w_sync, .read_size = 1, .prog_size = 4, .block_size =
		4096, .block_count = (BSP_DATA_FLASH_SIZE_BYTES / 4096), .block_cycles =
		1024, .cache_size = 64, .lookahead_size = 16,
#ifdef LFS_NO_MALLOC
    .read_buffer = (void *) g_rm_littlefs0_read,
    .prog_buffer = (void *) g_rm_littlefs0_prog,
    .lookahead_buffer = (void *) g_rm_littlefs0_lookahead,
#endif
#if LFS_THREAD_SAFE
    .lock   = &rm_littlefs_flash_w_lock,
    .unlock = &rm_littlefs_flash_w_unlock,
#endif
		};

#define FSP_NOT_DEFINED	(0xFFFFFFFF)
rm_littlefs_flash_w_cfg_t g_rm_littlefs0_ext_cfg = { .p_flash = NULL,
#if (FSP_NOT_DEFINED == g_rm_block_media2)
    .p_media = NULL,
#else
		.p_media = &g_rm_block_media2,
#endif    
		};
#undef FSP_NOT_DEFINED 

const rm_littlefs_cfg_t g_rm_littlefs0_cfg = { .p_lfs_cfg =
		&g_rm_littlefs0_lfs_cfg, .p_extend = &g_rm_littlefs0_ext_cfg, };

/* Instance structure to use this module. */
const rm_littlefs_instance_t g_rm_littlefs0 = { .p_ctrl = &g_rm_littlefs0_ctrl,
		.p_cfg = &g_rm_littlefs0_cfg, .p_api = &g_rm_littlefs_on_flash, };

ext_irq_w_extended_cfg_t g_external_irq2_ext_cfg = {
#ifdef EXT_INTR2_PIN
    .irq_pin = EXT_INTR2_PIN,
#endif
		};

ext_irq_w_instance_ctrl_t g_external_irq2_ctrl;
const external_irq_cfg_t g_external_irq2_cfg = { .channel = 2, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING, .p_callback = NULL, .p_context = NULL,
		.p_extend = &g_external_irq2_ext_cfg, .ipl = (12), .irq =
				FSP_INVALID_VECTOR, };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq2 = { .p_ctrl =
		&g_external_irq2_ctrl, .p_cfg = &g_external_irq2_cfg, .p_api =
		&g_external_irq_on_ext_irq_w };
ext_irq_w_extended_cfg_t g_external_irq1_ext_cfg = {
#ifdef EXT_INTR1_PIN
    .irq_pin = EXT_INTR1_PIN,
#endif
		};

ext_irq_w_instance_ctrl_t g_external_irq1_ctrl;
const external_irq_cfg_t g_external_irq1_cfg = { .channel = 1, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING,
		.p_callback = srm_wifi_app_gpio_p0_fr_handler, .p_context = NULL,
		.p_extend = &g_external_irq1_ext_cfg, .ipl = (12), .irq =
				FSP_INVALID_VECTOR, };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq1 = { .p_ctrl =
		&g_external_irq1_ctrl, .p_cfg = &g_external_irq1_cfg, .p_api =
		&g_external_irq_on_ext_irq_w };
ext_irq_w_extended_cfg_t g_external_irq0_ext_cfg = {
#ifdef EXT_INTR0_PIN
    .irq_pin = EXT_INTR0_PIN,
#endif
		};

ext_irq_w_instance_ctrl_t g_external_irq0_ctrl;
const external_irq_cfg_t g_external_irq0_cfg = { .channel = 0, .trigger =
		EXTERNAL_IRQ_TRIG_FALLING, .p_callback = NULL, .p_context = NULL,
		.p_extend = &g_external_irq0_ext_cfg, .ipl = (12), .irq =
				FSP_INVALID_VECTOR, };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 = { .p_ctrl =
		&g_external_irq0_ctrl, .p_cfg = &g_external_irq0_cfg, .p_api =
		&g_external_irq_on_ext_irq_w };
gpio_w_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport = { .p_api = &g_gpio_w_on_ioport, .p_ctrl =
		&g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };

void g_common_init(void) {
}
