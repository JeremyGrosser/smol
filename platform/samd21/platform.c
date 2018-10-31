#include <platform.h>

static uint32_t ticks = 0;

void platform_init(void) {
	SystemInit();
	SystemCoreClockUpdate();
	
	SysTick_Config(SystemCoreClock / 1000);

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PM->SLEEP.reg = (
			PM_SLEEP_IDLE_CPU |
			PM_SLEEP_IDLE_AHB |
			PM_SLEEP_IDLE_APB);
}

void SysTick_Handler(void) {
	ticks++;
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
