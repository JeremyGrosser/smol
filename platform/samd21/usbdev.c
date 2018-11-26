#include <platform/usbdev.h>
#include <platform.h>
#include "gclk.h"

#include <stdint.h>
#include <string.h>


int usb_setup(usbdev_t *dev) {
	PM->APBBMASK.reg |= PM_APBBMASK_USB;

	gpio_setup(dev->dm);
	gpio_setup(dev->dp);

	GCLK->CLKCTRL.reg = (
			GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_USB) |
			GCLK_CLKCTRL_ID(GCLK_GEN_USB) |
			GCLK_CLKCTRL_CLKEN);

	while(GCLK->STATUS.bit.SYNCBUSY);

	USB->DEVICE.CTRLA.bit.SWRST = 1;

	while(USB->DEVICE.SYNCBUSY.bit.SWRST);

	// TODO: PADCAL
	
	USB->DEVICE.CTRLA.reg = (
			USB_CTRLA_MODE_DEVICE |
			USB_CTRLA_RUNSTDBY);

	USB->DEVICE.DESCADD.reg = (uint32_t)(&dev->endpoints[0]);

	USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;
	USB->DEVICE.CTRLB.bit.DETACH = 0;

	memset(&dev->endpoints, 0, sizeof(dev->endpoints) * sizeof(UsbDeviceDescriptor));

	//USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;
	
	return 0;
}
