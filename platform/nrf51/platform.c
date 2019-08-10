#include <platform.h>

#define TICK_HZ 1000
static uint32_t ticks = 0;

void platform_init(void) {
    SystemInit();
    SystemCoreClockUpdate();
    
    // enable 32KHz crystal
    NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Synth;
    NRF_CLOCK->TASKS_LFCLKSTART = 1;
    while(!NRF_CLOCK->EVENTS_LFCLKSTARTED);

    // nrf51 has no SYSTICK, use RTC0
    NRF_RTC0->POWER = (RTC_POWER_POWER_Enabled << RTC_POWER_POWER_Pos);
    NRF_RTC0->PRESCALER = (32768 / TICK_HZ) - 1;
    NRF_RTC0->INTENSET = (RTC_INTENSET_TICK_Enabled << RTC_INTENSET_TICK_Pos);
    NRF_RTC0->TASKS_START = 1;
    NVIC_EnableIRQ(RTC0_IRQn);

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // prefer lower power over constant latency
    NRF_POWER->TASKS_LOWPWR = 1;
}


uint32_t platform_ticks(void) {
    return ticks;
}

void platform_delay(uint32_t ms) {
    uint32_t until = ticks + ms;
    while(ticks < until) {
        __WFI();
    }
}

void RTC0_IRQHandler(void) {
    NRF_RTC0->EVENTS_TICK = 0;
    ticks++;
}

void HardFault_Handler(void) {
}
