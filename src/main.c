#include <platform/gpio.h>
#include <platform/uart.h>
#include <platform/i2c.h>
#include <platform.h>
#include <board.h>

#include <stdio.h>

#define TMP102_ADDR	0x48

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		printf("board_init err=%d\r\n", err);
		return 1;
	}else{
		printf("ok!\r\n");
	}

	return 0;
}
