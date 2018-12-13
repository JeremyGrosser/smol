#include <platform/uart.h>
#include <platform/i2c.h>
#include <platform/spi.h>
#include <platform/usbdev.h>
#include <platform.h>
#include <board.h>

#include <eeprom.h>
#include <sx1231.h>

#include <stdio.h>
#include <string.h>


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
gpio_t USB_DM = {.num = PIN_PA24};
gpio_t USB_DP = {.num = PIN_PA25};


uart_t UART = {
	.num = 0,
	.txd = &UART_TX,
	.rxd = &UART_RX,
	.tx_pad = 2,
	.rx_pad = 3,
	.baud = 115200,
};


spi_t SPI = {
	.num	= 1,
	.mosi	= &SPI_MOSI,
	.miso	= &SPI_MISO,
	.sck	= &SPI_SCK,
	.nss	= &SPI_NSS,

	.mosi_pad = 0,
	.miso_pad = 2,
	.sck_pad = 1,
	.nss_pad = 3,
};


i2c_t I2C = {
	.num = 3,
	.sda = &I2C_SDA,
	.scl = &I2C_SCL,
};

eeprom_t EEPROM = {
	.i2c = &I2C,
	.address = 0xA0,
};

sx1231_t RFM = {
	.spi = &SPI,
	.reset = &RF_RESET,
};

usbdev_t USBDEV = {
	.dm = &D8,
	.dp = &D9,
	.hw = USB,
};


int board_init() {
	platform_init();

	LED.config = (pincfg_t){
		.direction = DIR_OUT,
		.drive = DRIVE_HIGH,
		.pull = PULL_DISABLE,
		.pmux = PMUX_DISABLE,
	};

	UART_RX.config = (pincfg_t){
		.direction = DIR_IN,
		.pmux = PMUX_ENABLE,
		.pmux_function = MUX_PA10C_SERCOM0_PAD2,
	};

	UART_TX.config = (pincfg_t){
		.direction = DIR_OUT,
		.pmux = PMUX_ENABLE,
		.pmux_function = MUX_PA11C_SERCOM0_PAD3,
	};

	USB_DM.config = (pincfg_t){
		.pmux = PMUX_ENABLE,
		.pmux_function = MUX_PA24G_USB_DM,
	};

	USB_DP.config = (pincfg_t){
		.pmux = PMUX_ENABLE,
		.pmux_function = MUX_PA25G_USB_DP,
	};
	
	I2C_SDA.config = (pincfg_t){
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA22C_SERCOM3_PAD0,
	};

	I2C_SCL.config = (pincfg_t){
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA23C_SERCOM3_PAD1,
	};

	SPI_SCK.config = (pincfg_t){
		.direction		= DIR_OUT,
		.drive			= DRIVE_LOW,
		.pull			= PULL_DISABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA17C_SERCOM1_PAD1,
	};

	SPI_NSS.config = (pincfg_t){
		.direction		= DIR_OUT,
		.drive			= DRIVE_LOW,
		.pull			= PULL_ENABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA19C_SERCOM1_PAD3,
	};

	SPI_MOSI.config = (pincfg_t){
		.direction		= DIR_OUT,
		.drive			= DRIVE_LOW,
		.pull			= PULL_DISABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA16C_SERCOM1_PAD0,
	};

	SPI_MISO.config = (pincfg_t){
		.direction		= DIR_IN,
		.pull			= PULL_DISABLE,
		.pmux			= PMUX_ENABLE,
		.pmux_function	= MUX_PA18C_SERCOM1_PAD2,
	};

	RF_IRQ.config = (pincfg_t){
		.direction		= DIR_IN,
		.pull			= PULL_DISABLE,
		.pmux			= PMUX_DISABLE,
	};

	RF_RESET.config = (pincfg_t){
		.direction		= DIR_OUT,
		.pull			= PULL_DISABLE,
		.pmux			= PMUX_DISABLE,
	};

	gpio_setup(&LED);
	gpio_write(&LED, LED_ON);

	uart_setup(&UART);
	eeprom_setup(&EEPROM);
	sx1231_setup(&RFM);

	//usb_setup(&USBDEV, cdcserial_enumerate);
	
	printf("\r\n\r\nlorawan board_init ok\r\n");

	gpio_write(&LED, LED_OFF);

	return 0;
}

void console_write(uint8_t *msg, size_t len) {
	uart_write(&UART, msg, len);
}
