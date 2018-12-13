#include <platform/gpio.h>
#include <board.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

	/*
	usb_open(&USBDEV);
	while(usb_isconfigured(&USBDEV) != 0);
	*/

	while(1) {
		gpio_write(&LED, 1);
		platform_delay(1000);
		gpio_write(&LED, 0);
		platform_delay(1000);
	}

	return 0;
}
