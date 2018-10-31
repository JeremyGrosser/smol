#include <platform/rtc.h>
#include <platform/gpio.h>
#include <platform.h>
#include "gclk.h"

#include <sys/time.h>
#include <stdio.h>

static struct tm rtc_now = {
	.tm_year	= 70,
	.tm_mon		= 0,
	.tm_mday	= 1,
	.tm_hour	= 0,
	.tm_min		= 0,
	.tm_sec		= 0,
	.tm_isdst	= -1,
};

// Track the value of platform_ticks() as of the last RTC read and use that to
// calculate seconds
//static uint32_t rtc_ticks = 1000;

void rtc_init(void) {
	RtcMode2 *hw = &RTC->MODE2;

	/*
	gpio_t clkpin = {
		.num		= PIN_PA17,
		.config		= {
			.direction	= DIR_OUT,
			.drive		= DRIVE_LOW,
			.pull		= PULL_ENABLE,
			.pmux		= PMUX_ENABLE,
			.pmux_function = MUX_PA17H_GCLK_IO3,
		},
	};
	gpio_setup(&clkpin);
	*/

	SYSCTRL->INTENSET.reg |= SYSCTRL_INTENSET_XOSC32KRDY;
	SYSCTRL->XOSC32K.reg = (
			SYSCTRL_XOSC32K_ENABLE |
			SYSCTRL_XOSC32K_XTALEN |
			SYSCTRL_XOSC32K_EN32K |
			SYSCTRL_XOSC32K_RUNSTDBY |
			SYSCTRL_XOSC32K_AAMPEN);
	while(!SYSCTRL->PCLKSR.bit.XOSC32KRDY);

	GCLK->GENCTRL.reg = (
			GCLK_GENCTRL_ID(GCLK_GEN_RTC) |
			GCLK_GENCTRL_SRC_XOSC32K |
			GCLK_GENCTRL_GENEN);
	
	GCLK->GENDIV.reg = (
			GCLK_GENDIV_ID(GCLK_GEN_RTC) |
			GCLK_GENDIV_DIV(32));
	
	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID_RTC |
			GCLK_CLKCTRL_GEN(GCLK_GEN_RTC) |
			GCLK_CLKCTRL_CLKEN);
	while(GCLK->STATUS.bit.SYNCBUSY);

	PM->APBAMASK.reg |= PM_APBAMASK_RTC;

	// Disable
	hw->CTRL.reg = 0;
	while(hw->STATUS.bit.SYNCBUSY);

	// Reset
	/* If we're lucky, the RTC might already have the current time, so let's
	 * not reset it.
	hw->CTRL.reg = RTC_MODE2_CTRL_SWRST;
	while(hw->STATUS.bit.SYNCBUSY);
	*/

	// Keep running while debugging
	hw->DBGCTRL.reg = RTC_DBGCTRL_DBGRUN;

	// Enable ALARM0 interrupt
	hw->INTENSET.reg = RTC_MODE2_INTENSET_ALARM0;
	hw->INTFLAG.reg = 0xFF;
	NVIC_EnableIRQ(RTC_IRQn);

	// Configure
	hw->CTRL.reg = (
			RTC_MODE2_CTRL_ENABLE |
			RTC_MODE2_CTRL_MODE_CLOCK |
			RTC_MODE2_CTRL_PRESCALER_DIV1024);
	while(hw->STATUS.bit.SYNCBUSY);

	// ALARM0 interrupt fires when SECOND is 0
	hw->Mode2Alarm[0].ALARM.reg = 0;
	hw->Mode2Alarm[0].MASK.reg = RTC_MODE2_MASK_SEL_SS;
	while(hw->STATUS.bit.SYNCBUSY);

	// Trigger the interrupt manually once to force an initial read
	RTC_Handler();
}

struct tm *rtc_read(void) {
	RtcMode2 *hw = &RTC->MODE2;

	hw->READREQ.reg = RTC_READREQ_RREQ;
	while(hw->STATUS.bit.SYNCBUSY);

	rtc_now.tm_year = hw->CLOCK.bit.YEAR + 70;
	rtc_now.tm_mon = hw->CLOCK.bit.MONTH - 1;
	rtc_now.tm_mday = hw->CLOCK.bit.DAY;
	rtc_now.tm_hour = hw->CLOCK.bit.HOUR;
	rtc_now.tm_min = hw->CLOCK.bit.MINUTE;
	rtc_now.tm_sec = hw->CLOCK.bit.SECOND;
	rtc_now.tm_isdst = -1;

	if(rtc_now.tm_mon < 0) {
		return NULL;
	}

	//rtc_now.tm_sec = (platform_ticks() - rtc_ticks) / 1000;
	rtc_now.tm_sec = 0;
	if(rtc_now.tm_sec > 65) {
		// For some reason, the RTC interrupt hasn't fired in the last minute,
		// so don't return incorrect time data
		return NULL;
	}

	return &rtc_now;
}

void rtc_write(struct tm *now) {
	RtcMode2 *hw = &RTC->MODE2;

	hw->CLOCK.reg = (
			RTC_MODE2_CLOCK_YEAR(now->tm_year - 70) |
			RTC_MODE2_CLOCK_MONTH(now->tm_mon + 1) |
			RTC_MODE2_CLOCK_DAY(now->tm_mday) |
			RTC_MODE2_CLOCK_HOUR(now->tm_hour) |
			RTC_MODE2_CLOCK_MINUTE(now->tm_min) |
			RTC_MODE2_CLOCK_SECOND(now->tm_sec));
	while(hw->STATUS.bit.SYNCBUSY);
	//rtc_ticks = platform_ticks();
}

void RTC_Handler(void) {
	RtcMode2 *hw = &RTC->MODE2;

	/*
	hw->READREQ.reg = RTC_READREQ_RREQ;
	while(hw->STATUS.bit.SYNCBUSY);

	rtc_ticks = platform_ticks();

	rtc_now.tm_year = hw->CLOCK.bit.YEAR + 70;
	rtc_now.tm_mon = hw->CLOCK.bit.MONTH - 1;
	rtc_now.tm_mday = hw->CLOCK.bit.DAY;
	rtc_now.tm_hour = hw->CLOCK.bit.HOUR;
	rtc_now.tm_min = hw->CLOCK.bit.MINUTE;
	rtc_now.tm_sec = hw->CLOCK.bit.SECOND;
	rtc_now.tm_isdst = 0;
	*/

	hw->INTFLAG.reg = 0xFF;
}
