#include <board.h>
#include <platform.h>
#include <platform/gpio.h>
#include <platform/rtc.h>
#include <platform/i2c.h>
//#include <platform/uart.h>

#include <stdio.h>

gpio_t LED = {
	.num	= PIN_PA17,
	.config	= {
		.direction	= DIR_OUT,
		.drive		= DRIVE_HIGH,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_DISABLE,
	},
};

gpio_t BUTTON0 = {
	.num	= PIN_PA02,
	.config	= {
		.direction	= DIR_IN,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PA02A_EIC_EXTINT2,
	},
	.interrupt = {
		.num	= 2,
		.sense	= SENSE_RISE,
		.filter	= FILTER_ENABLE,
		.function = &button0_interrupt,
	},
};

gpio_t BUTTON1 = {
	.num	= PIN_PB08,
	.config	= {
		.direction	= DIR_IN,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PB08A_EIC_EXTINT8,
	},
	.interrupt = {
		.num	= 8,
		.sense	= SENSE_RISE,
		.filter	= FILTER_ENABLE,
		.function = &button1_interrupt,
	},
};

/*
gpio_t CONSOLE_TXD = {
	.num	= PIN_PA10,
	.config = {
		.direction		= DIR_OUT,
		.drive			= DRIVE_HIGH,
		.pull			= PULL_ENABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA10C_SERCOM0_PAD2,
	},
};

gpio_t CONSOLE_RXD = {
	.num	= PIN_PA11,
	.config = {
		.direction		= DIR_IN,
		.drive			= DRIVE_HIGH,
		.pull			= PULL_ENABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA11C_SERCOM0_PAD3,
	},
};

uart_t CONSOLE_UART = {
	.num	= 0,
	.sercom = &SERCOM0->USART,
	.txd	= &CONSOLE_TXD,
	.rxd	= &CONSOLE_RXD,
	.tx_pad = 2,
	.rx_pad	= 3,
};
*/

gpio_t I2C_SDA = {
	.num	= PIN_PA22,
	.config	= {
		.direction	= DIR_OUT,
		.drive		= DRIVE_LOW,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PA22C_SERCOM3_PAD0,
	},
};

gpio_t I2C_SCL = {
	.num	= PIN_PA23,
	.config	= {
		.direction	= DIR_OUT,
		.drive		= DRIVE_LOW,
		.pull		= PULL_ENABLE,
		.pmux		= PMUX_ENABLE,
		.pmux_function = MUX_PA23C_SERCOM3_PAD1,
	},
};

i2c_t DISPLAY_I2C = {
	.num = 3,
	.scl = &I2C_SCL,
	.sda = &I2C_SDA,
};

int board_init() {
	platform_init();

	gpio_setup(&LED);
	gpio_write(&LED, LED_ON);

	/*
	err = uart_setup(&CONSOLE_UART);
	if(err != 0) {
		return err;
	}
	*/
	printf("\033[2J\033[0;0H");
	printf("\r\n\r\nada3010 board_init\r\n");

	rtc_setup();
	i2c_setup(&DISPLAY_I2C);

	gpio_setup(&BUTTON0);
	gpio_setup(&BUTTON1);
	gpio_write(&BUTTON0, 0);
	gpio_write(&BUTTON1, 0);

	return 0;
}

void console_write(uint8_t *msg, size_t len) {
	//uart_write(&CONSOLE_UART, msg, len);
}

int console_read(uint8_t *msg, size_t maxlen) {
	//return uart_read(&CONSOLE_UART, msg, maxlen);
	return -1;
}

void button0_interrupt(void) {
	struct tm *now;

	now = rtc_read();
	now->tm_min = ((now->tm_min + 1) % 60);
	now->tm_sec = 0;
	rtc_write(now);
}

void button1_interrupt(void) {
	struct tm *now;

	now = rtc_read();
	now->tm_hour = ((now->tm_hour + 1) % 24);
	rtc_write(now);
	}
