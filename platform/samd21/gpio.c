#include <platform/gpio.h>
#include <platform.h>
#include <board.h>
#include "gclk.h"

static exti_func_t exti_functions[EIC_EXTINT_NUM];
static int exti_initialized = 0;

static void exti_init(void) {
	int i;
	for(i = 0; i < EIC_EXTINT_NUM; i++) {
		exti_functions[i] = NULL;
	}

	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_EIC) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_EIC) |
			GCLK_CLKCTRL_CLKEN);

	PM->APBAMASK.reg |= PM_APBAMASK_EIC;

	EIC->CTRL.reg = EIC_CTRL_SWRST;
	while(EIC->STATUS.bit.SYNCBUSY);

	EIC->CTRL.reg = EIC_CTRL_ENABLE;
	while(EIC->STATUS.bit.SYNCBUSY);

	EIC->INTFLAG.reg = 0xFFFFFFFF;
	NVIC_EnableIRQ(EIC_IRQn);
	exti_initialized = 1;
}

static void exti_attach(exti_t *exti) {
	int config_num, config_pos;
	int flag;

	if(exti_initialized == 0) {
		exti_init();
	}

	flag = (1 << exti->num);
	config_num = (exti->num / 8);
	config_pos = ((exti->num % 8) * 4);

	exti_functions[exti->num] = exti->function;
	EIC->CONFIG[config_num].reg &= ~(0xF << config_pos);
	EIC->CONFIG[config_num].reg |= ((exti->sense | (exti->filter << 3)) << config_pos);
	EIC->INTENSET.reg |= flag;
	EIC->WAKEUP.reg |= flag;
}

void EIC_Handler(void) {
	uint32_t flag;
	int i;

	flag = EIC->INTFLAG.reg;
	EIC->INTFLAG.reg = 0xFFFFFFFF;
	for(i = 0; i < EIC_EXTINT_NUM; i++) {
		if(((flag >> i) & 1) && exti_functions[i] != NULL) {
			exti_functions[i]();
		}
	}
}

void gpio_setup(gpio_t *gpio) {
	gpio->port = (gpio->num / 32);
	gpio->pin = (gpio->num % 32);
	gpio->mask = (1 << gpio->pin);

	if(gpio->config.pmux == PMUX_ENABLE) {
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.PMUXEN = 1;

		if(gpio->pin & 1) {
			PORT->Group[gpio->port].PMUX[(gpio->pin / 2)].bit.PMUXO = gpio->config.pmux_function;
		}else{
			PORT->Group[gpio->port].PMUX[(gpio->pin / 2)].bit.PMUXE = gpio->config.pmux_function;
		}
	}else{
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.PMUXEN = 0;
	}

	if(gpio->config.direction == DIR_OUT) {
		PORT->Group[gpio->port].DIR.reg |= gpio->mask;
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.INEN = 0;
	}else{
		PORT->Group[gpio->port].DIR.reg &= ~gpio->mask;
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.INEN = 1;
	}

	if(gpio->config.pull == PULL_ENABLE) {
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.PULLEN = 1;
	}else{
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.PULLEN = 0;
	}

	if(gpio->config.drive == DRIVE_HIGH) {
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.DRVSTR = 1;
	}else{
		PORT->Group[gpio->port].PINCFG[gpio->pin].bit.DRVSTR = 0;
	}

	if(gpio->interrupt.function != NULL) {
		exti_attach(&gpio->interrupt);
	}
}

int gpio_read(gpio_t *gpio) {
	if(gpio->config.direction == DIR_IN) {
		return (PORT->Group[gpio->port].IN.reg & gpio->mask) ? 1 : 0;
	}else{
		return (PORT->Group[gpio->port].OUT.reg & gpio->mask) ? 1 : 0;
	}
}

void gpio_write(gpio_t *gpio, int state) {
	if(state == 0) {
		PORT->Group[gpio->port].OUT.reg &= ~gpio->mask;
	}else{
		PORT->Group[gpio->port].OUT.reg |= gpio->mask;
	}
}

void gpio_toggle(gpio_t *gpio) {
	PORT->Group[gpio->port].OUTTGL.reg |= gpio->mask;
}
