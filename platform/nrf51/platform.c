#include <platform.h>

static uint32_t ticks = 0;

void platform_init(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
    // TODO: nrf51 has no systick, use RTC0

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

uint32_t platform_ticks(void) {
	return ticks;
}

void platform_delay(uint32_t ms) {
	uint32_t until = ticks + ms;
	while(ticks < until);
}

void HardFault_Handler(void) {
}
