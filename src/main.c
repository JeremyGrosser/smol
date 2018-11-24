#include <platform/gpio.h>
#include <platform.h>
#include <board.h>

#include <stdio.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		printf("board_init err=%d\r\n", err);
		return 1;
	}else{
		printf("ok!\r\n");
	}

	gpio_setup(&LED);

	while(1) {
		gpio_write(&LED, 1);
		platform_delay(100);
		gpio_write(&LED, 0);
		platform_delay(100);
	}

	return 0;
}
