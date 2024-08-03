#include <esp_log.h>
#include <lp_core_uart.h>
#include <lp_rvfplib.h>
#include <riscv/rv_utils.h>
#include <soc/clint_reg.h>
#include <ulp_lp_core.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <atomic>
#include <cmath>
#include <cstdio>

static constexpr char TAG[] = "test";

extern volatile float    lp_fp_counter asm("ulp_lp_fp_counter");
extern volatile uint32_t lp_started asm("ulp_lp_started");
extern const uint8_t     ulp_main_bin_start[] asm("_binary_lp_rvfplib_bin_start");
extern const uint8_t     ulp_main_bin_end[] asm("_binary_lp_rvfplib_bin_end");

static void load_ulp()
{
	static constexpr lp_core_uart_cfg_t lp_uart_cfg = {
	    .uart_pin_cfg =
	        {
	                       .tx_io_num  = GPIO_NUM_5,
	                       .rx_io_num  = GPIO_NUM_4,
	                       .rts_io_num = GPIO_NUM_NC,
	                       .cts_io_num = GPIO_NUM_NC,
	                       },
	    .uart_proto_cfg =
	        {
	                       .baud_rate           = 115200,
	                       .data_bits           = UART_DATA_8_BITS,
	                       .parity              = UART_PARITY_DISABLE,
	                       .stop_bits           = UART_STOP_BITS_1,
	                       .flow_ctrl           = UART_HW_FLOWCTRL_DISABLE,
	                       .rx_flow_ctrl_thresh = 0,
	                       },
	    .lp_uart_source_clk = LP_UART_SCLK_DEFAULT,
	};
	esp_err_t err = ESP_OK;
	ESP_ERROR_CHECK(lp_core_uart_init(&lp_uart_cfg));
	err = ulp_lp_core_load_binary(ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start));
	ESP_ERROR_CHECK(err);

	/* Start the program */
	ulp_lp_core_cfg_t cfg = {
	    .wakeup_source              = ULP_LP_CORE_WAKEUP_SOURCE_LP_TIMER,
	    .lp_timer_sleep_duration_us = 10000,
	};

	err = ulp_lp_core_run(&cfg);
	ESP_ERROR_CHECK(err);
}

extern "C" void app_main(void)
{
	load_ulp();

	while (!lp_started)
		;

	while (true) {
		ESP_LOGI(TAG, "fp counter: %f", lp_fp_counter);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
