#include <sdkconfig.h>

#include <riscv/rv_utils.h>
#include <stdbool.h>
#include <stdint.h>
#include <ulp_lp_core_print.h>
#include <ulp_lp_core_utils.h>

volatile float    lp_fp_counter = 0.0f;
volatile uint32_t lp_started    = 0;

int main(void)
{
	const float freq_hz = 16000000;
	lp_fp_counter       = 1.0f;

	while (1) {
		uint64_t prev = ((uint64_t)RV_READ_CSR(mcycleh) << 32) | ((uint64_t)RV_READ_CSR(mcycle));
		lp_fp_counter = prev / freq_hz + 1;
		lp_core_print_str("cpu tick: ");
		lp_core_print_hex((int)(prev >> 32));
		lp_core_print_hex((int)(prev));
		lp_core_print_char('\n');
		ulp_lp_core_delay_us(500000);
		lp_started = 1;
	}
	return 0;
}
