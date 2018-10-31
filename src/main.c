#include <platform/gpio.h>
#include <platform/rtc.h>
#include <platform.h>
#include <board.h>

#include <sys/time.h>
#include <stdio.h>

int main(void) {
	struct timeval tv;
	int err;

	err = board_init();
	if(err != 0) {
		printf("board_init err=%d\r\n", err);
		return 1;
	}else{
		printf("ok!\r\n");
	}

	gpio_setup(&LED);
	rtc_setup();

	while(1) {
		if(gettimeofday(&tv, NULL) == 0) {
			printf("%d.%d\r\n", (int)tv.tv_sec, (int)tv.tv_usec);
		}

		gpio_toggle(&LED);
		platform_delay(1000);
	}

	return 0;
}
