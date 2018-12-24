#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>

#include <lib/hexdump.h>

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
		sx1276_dump_status(&RFM);
		platform_delay(5000);
	}

	return 0;
}
