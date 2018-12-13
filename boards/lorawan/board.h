#ifndef BOARD_H
#define BOARD_H

#include <platform/uart.h>
#include <platform/i2c.h>
#include <platform/spi.h>
#include <platform/gpio.h>
#include <platform/usbdev.h>
#include <platform.h>

extern gpio_t D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D20, D21;
extern gpio_t A0, A1, A2, A3, A4, A5, AREF;

extern uart_t UART;
extern i2c_t I2C;
extern spi_t SPI;
extern usbdev_t USBDEV;

#define LED			A2
#define LED_ON		0
#define LED_OFF		1

#define UART_RX		D0
#define UART_TX		D1

#define SPI_MOSI	D11
#define SPI_MISO	D10
#define SPI_SCK		D13
#define SPI_NSS		D12

#define I2C_SDA		D20
#define I2C_SCL		D21

#define RF_IRQ		D8
#define RF_RESET	A0

int board_init();
void console_write(uint8_t *msg, size_t len);

void cdcserial_enumerate(usbdev_t *dev);

#endif
