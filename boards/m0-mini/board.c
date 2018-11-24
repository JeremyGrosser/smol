#include "board.h"


/* Arduino pin names */
gpio_t D0 = {.num = PIN_PA11};
gpio_t D1 = {.num = PIN_PA10};
gpio_t D2 = {.num = PIN_PA08};
gpio_t D3 = {.num = PIN_PA09};
gpio_t D4 = {.num = PIN_PA14};
gpio_t D5 = {.num = PIN_PA15};
gpio_t D6 = {.num = PIN_PA20};
gpio_t D7 = {.num = PIN_PA21};
gpio_t D8 = {.num = PIN_PA06};
gpio_t D9 = {.num = PIN_PA07};
gpio_t D10 = {.num = PIN_PA18};
gpio_t D11 = {.num = PIN_PA16};
gpio_t D12 = {.num = PIN_PA19};
gpio_t D13 = {.num = PIN_PA17};
gpio_t D20 = {.num = PIN_PA22};
gpio_t D21 = {.num = PIN_PA23};
gpio_t A0 = {.num = PIN_PA02};
gpio_t A1 = {.num = PIN_PB08};
gpio_t A2 = {.num = PIN_PB09};
gpio_t A3 = {.num = PIN_PA04};
gpio_t A4 = {.num = PIN_PA05};
gpio_t A5 = {.num = PIN_PB02};
gpio_t AREF = {.num = PIN_PA03};


int board_init() {
	platform_init();

	LED.config = (pincfg_t){
		.direction = DIR_OUT,
		.drive = DRIVE_LOW,
		.pull = PULL_DISABLE,
		.pmux = PMUX_DISABLE,
	};

	return 0;
}
