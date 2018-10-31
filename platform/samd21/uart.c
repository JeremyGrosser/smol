#include <platform/uart.h>
#include <platform/gpio.h>
#include <platform.h>
#include "gclk.h"

uint16_t uart_baud(uint32_t baud) {
	float result;
	result = 65536.0 * (1.0 - ((8.0 * (float)baud) / (float)SystemCoreClock));
	return (uint16_t)result;
}

int uart_setup(uart_t *uart) {
	Sercom *sercom;

	sercom = (Sercom *)(((void *)SERCOM0) + (uart->num * 0x400UL));
	uart->sercom = &sercom->USART;

	gpio_setup(uart->txd);
	gpio_setup(uart->rxd);

	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val + uart->num) |
			GCLK_CLKCTRL_GEN(GCLK_GEN_UART) |
			GCLK_CLKCTRL_CLKEN);

	// Enable the sercom clock
	PM->APBCMASK.reg |= (1 << (uart->num + 2));

	uart->sercom->INTENCLR.reg = 0xFF;
	uart->sercom->CTRLA.bit.SWRST = 1;
	while(uart->sercom->SYNCBUSY.reg);

	// This is 115200 baud, trust me
	uart->sercom->BAUD.reg = uart_baud(uart->baud); // 5138

	uart->sercom->CTRLA.reg = (
			SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
			SERCOM_USART_CTRLA_RXPO(uart->rx_pad) |
			SERCOM_USART_CTRLA_TXPO(uart->tx_pad) |
			SERCOM_USART_CTRLA_DORD |
			SERCOM_USART_CTRLA_SAMPR(2)
			);

	if(uart->tx_pad == 0) {
		uart->sercom->CTRLA.bit.TXPO = 0;
	}else if(uart->tx_pad == 2) {
		uart->sercom->CTRLA.bit.TXPO = 1;
	}else{
		return -1;
	}

	uart->sercom->DBGCTRL.reg = 0;

	uart->sercom->CTRLB.reg = (
			SERCOM_USART_CTRLB_CHSIZE(8) |
			SERCOM_USART_CTRLB_SBMODE |
			SERCOM_USART_CTRLB_TXEN |
			SERCOM_USART_CTRLB_RXEN
			);
	while(uart->sercom->SYNCBUSY.reg);

	uart->sercom->CTRLA.bit.ENABLE = 1;
	while(uart->sercom->SYNCBUSY.reg);

	uart->sercom->INTENSET.bit.RXC = 1;
	uart->sercom->INTENSET.bit.TXC = 1;

	//NVIC_EnableIRQ(SERCOM0_IRQn + uart->num);

	return 0;
}

void uart_putc(uart_t *uart, uint8_t c) {
	uart->sercom->DATA.reg = c;
	while(!uart->sercom->INTFLAG.bit.TXC);
	uart->sercom->INTFLAG.reg = 0;
}

uint8_t uart_getc(uart_t *uart) {
	if(!uart->sercom->INTFLAG.bit.RXC) {
		return 0;
	}else{
		uart->sercom->INTFLAG.reg = 0;
		return uart->sercom->DATA.reg;
	}
}

void uart_write(uart_t *uart, uint8_t *msg, size_t len) {
	size_t i;
	for(i = 0; i < len; i++) {
		uart_putc(uart, msg[i]);
	}
}

int uart_read(uart_t *uart, uint8_t *msg, size_t len) {
	uint8_t c;
	size_t i;

	for(i = 0; i < len; i++) {
		c = uart_getc(uart);
		if(c <= 0) {
			break;
		}
		msg[i] = c;
	}

	return i;
}
