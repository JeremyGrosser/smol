#ifndef BOARD_H
#define BOARD_H

#include <platform/gpio.h>
#include <platform/i2c.h>
#include <platform.h>

#include <stddef.h>

#define LED_ON	1
#define LED_OFF	0

#define DISPLAY_ADDR 0x70

gpio_t LED;
gpio_t BUTTON0, BUTTON1;
i2c_t DISPLAY_I2C;

int board_init();
void console_write(uint8_t *msg, size_t len);
int console_read(uint8_t *msg, size_t maxlen);

void button0_interrupt(void);
void button1_interrupt(void);

#endif
