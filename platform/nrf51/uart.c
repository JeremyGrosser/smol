#include <platform/uart.h>
#include <platform/gpio.h>
#include <platform.h>

int uart_setup(uart_t *uart) {
    if(uart->num != 0) {
        return -1;
    }

	gpio_setup(uart->rxd);
	gpio_setup(uart->txd);
    gpio_write(uart->txd, 1);

    uart->pdev = NRF_UART0;
    uart->pdev->POWER = (UART_POWER_POWER_Enabled << UART_POWER_POWER_Pos);
    uart->pdev->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);

    /* nRF51 RM v3.0, page 156, Table 287: BAUDRATE */
    switch(uart->baud) {
        case 1200:      uart->pdev->BAUDRATE = 0x0004F000; break;
        case 2400:      uart->pdev->BAUDRATE = 0x0009D000; break;
        case 4800:      uart->pdev->BAUDRATE = 0x0013B000; break;
        case 9600:      uart->pdev->BAUDRATE = 0x00275000; break;
        case 14400:     uart->pdev->BAUDRATE = 0x003B0000; break;
        case 19200:     uart->pdev->BAUDRATE = 0x004EA000; break;
        case 28800:     uart->pdev->BAUDRATE = 0x0075F000; break;
        case 38400:     uart->pdev->BAUDRATE = 0x009D5000; break;
        case 57600:     uart->pdev->BAUDRATE = 0x00EBF000; break;
        case 76800:     uart->pdev->BAUDRATE = 0x013A9000; break;
        case 115200:    uart->pdev->BAUDRATE = 0x01D7E000; break;
        case 230400:    uart->pdev->BAUDRATE = 0x03AFB000; break;
        case 250000:    uart->pdev->BAUDRATE = 0x04000000; break;
        case 460800:    uart->pdev->BAUDRATE = 0x075F7000; break;
        case 921600:    uart->pdev->BAUDRATE = 0x0EBEDFA4; break;
        case 1000000:   uart->pdev->BAUDRATE = 0x10000000; break;
        default:
            return -1;
            break;
    }

    uart->pdev->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos) | \
                         (UART_CONFIG_HWFC_Disabled << UART_CONFIG_HWFC_Pos);

    uart->pdev->INTENCLR = 0xFFFFFFFF;
    uart->pdev->INTENSET = (UART_INTENSET_RXDRDY_Enabled << UART_INTENSET_RXDRDY_Pos);
    uart->pdev->EVENTS_RXDRDY = 0;

    uart->pdev->PSELRXD = uart->rxd->num;
    uart->pdev->PSELTXD = uart->txd->num;

	gpio_setup(uart->rxd);
	gpio_setup(uart->txd);
    gpio_write(uart->txd, 1);

    NVIC_EnableIRQ(UART0_IRQn);

    uart->pdev->TASKS_STARTRX = 1;
	return 0;
}

void uart_putc(uart_t *uart, uint8_t c) {
    uart->pdev->TASKS_STARTTX = 1;
    uart->pdev->TXD = c;
    while(!uart->pdev->EVENTS_TXDRDY);
    uart->pdev->TASKS_STOPTX = 1;
    uart->pdev->EVENTS_TXDRDY = 0;
}

uint8_t uart_getc(uart_t *uart) {
    if(uart->pdev->EVENTS_RXDRDY != 1) {
        return 0;
    }else{
        uart->pdev->EVENTS_RXDRDY = 0;
        return uart->pdev->RXD;
    }
}

void uart_write(uart_t *uart, uint8_t *msg, size_t len) {
    size_t i;

    uart->pdev->TASKS_STARTTX = 1;
    for(i = 0; i < len; i++) {
        uart->pdev->TXD = msg[i];
        while(!uart->pdev->EVENTS_TXDRDY);
        uart->pdev->EVENTS_TXDRDY = 0;
    }
    uart->pdev->TASKS_STOPTX = 1;
    uart->pdev->EVENTS_TXDRDY = 0;
}

int uart_read(uart_t *uart, uint8_t *msg, size_t len) {
    return -1;
}


void UART0_IRQHandler(void) {
    uint8_t ch;
    if(NRF_UART0->EVENTS_RXDRDY) {
        ch = NRF_UART0->RXD;
        ch++;
        NRF_UART0->EVENTS_RXDRDY = 0;
    }
}
