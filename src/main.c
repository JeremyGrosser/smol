#include <platform/gpio.h>
#include <board.h>
#include <stdio.h>

#include <sx127x.h>
#include <lib/hexdump.h>

int main(void) {
	int err;

	err = board_init();
	if(err != 0) {
		return 1;
	}

	printf("sx127x: version 0x%02X\r\n", sx1276_version(&RFM));

	/*
	usb_open(&USBDEV);
	while(usb_isconfigured(&USBDEV) != 0);
	*/

	while(1) {
		sx1276_test_fifo(&RFM);
		platform_delay(1000);
	}

	return 0;
}
