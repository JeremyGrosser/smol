#include <platform/i2c.h>
#include <platform/uart.h>
#include <platform.h>
#include <board.h>
#include <stdio.h>

gpio_t D0 = {.num = PIN_PA11};
gpio_t D1 = {.num = PIN_PA10};

uart_t UART = {
	.num = 0,
	.txd = &D1,
	.rxd = &D0,
	.tx_pad = 2,
	.rx_pad = 3,
	.baud = 115200,
};

gpio_t D2 = {.num = PIN_PA14};
gpio_t D3 = {.num = PIN_PA09};
gpio_t D4 = {.num = PIN_PA08};
gpio_t D5 = {.num = PIN_PA15};
gpio_t D6 = {.num = PIN_PA20};
gpio_t D7 = {.num = PIN_PA21};
gpio_t D8 = {.num = PIN_PA06};
gpio_t D9 = {.num = PIN_PA07};
gpio_t D10 = {.num = PIN_PA18};
gpio_t D11 = {.num = PIN_PA16};
gpio_t D12 = {.num = PIN_PA19};
gpio_t D13 = {.num = PIN_PA17};

gpio_t D20 = {
	.num = PIN_PA22,
	.config = {
		.direction		= DIR_OUT,
		.drive			= DRIVE_LOW,
		.pull			= PULL_ENABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA22C_SERCOM3_PAD0,
	},
};

gpio_t D21 = {
	.num = PIN_PA23,
	.config = {
		.direction		= DIR_OUT,
		.drive			= DRIVE_LOW,
		.pull			= PULL_ENABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA23C_SERCOM3_PAD1,
	},
};

i2c_t I2C = {
	.num = 3,
	.sda = &D20,
	.scl = &D21,
};

gpio_t A0 = {.num = PIN_PA02};
gpio_t A1 = {.num = PIN_PB08};
gpio_t A2 = {.num = PIN_PB09};
gpio_t A3 = {.num = PIN_PA04};
gpio_t AREF = {.num = PIN_PA03};


int board_init() {
	platform_init();

	uart_setup(&UART);
	i2c_setup(&I2C);

	return 0;
}

void console_write(uint8_t *msg, size_t len) {
	uart_write(&UART, msg, len);
}
