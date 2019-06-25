#include <board.h>

#include <string.h>
#include <stdio.h>

gpio_t LED = {
	.num	= PIN_PB30,
	.config	= {
		.direction	= DIR_OUT,
		.drive		= DRIVE_LOW,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_DISABLE,
	},
};

gpio_t CONSOLE_TXD = {
	.num	= PIN_PA22,
	.config = {
		.direction	= DIR_OUT,
		.drive		= DRIVE_LOW,
		.pull		= PULL_DISABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PA22C_SERCOM3_PAD0,
	},
};

gpio_t CONSOLE_RXD = {
	.num	= PIN_PA23,
	.config = {
		.direction	= DIR_IN,
		.drive		= DRIVE_LOW,
		.pull		= PULL_DISABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PA23C_SERCOM3_PAD1,
	},
};

uart_t CONSOLE_UART = {
	.num	= 3,
	.txd	= &CONSOLE_TXD,
	.rxd	= &CONSOLE_RXD,
	.tx_pad = 0,
	.rx_pad = 1,
	.baud	= 115200,
};

gpio_t MAX7219_CS = {
    .num    = PIN_PB00,
    .config = {
        .direction = DIR_OUT,
        .drive = DRIVE_LOW,
        .pull = PULL_DISABLE,
        .pmux = PMUX_DISABLE,
    },
};

gpio_t MAX7219_CLK = {
    .num    = PIN_PB01,
    .config = {
        .direction = DIR_OUT,
        .drive = DRIVE_LOW,
        .pull = PULL_DISABLE,
        .pmux = PMUX_DISABLE,
    },
};

gpio_t MAX7219_DIN = {
    .num    = PIN_PB02,
    .config = {
        .direction = DIR_OUT,
        .drive = DRIVE_LOW,
        .pull = PULL_DISABLE,
        .pmux = PMUX_DISABLE,
    },
};

max7219_t MAX7219 = {
    .cs = &MAX7219_CS,
    .clk = &MAX7219_CLK,
    .din = &MAX7219_DIN,
};

int board_init() {
	platform_init();
	uart_setup(&CONSOLE_UART);

	// ANSI clear sequence
	printf("\033[2J\033[0;0H");
	printf("\r\nboard_init\r\n");

    max7219_setup(&MAX7219);

	return 0;
}

void console_write(uint8_t *msg, size_t len) {
	uart_write(&CONSOLE_UART, msg, len);
}
