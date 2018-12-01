#include <platform/usbdev.h>
#include <platform.h>
#include "gclk.h"

#include <stdint.h>
#include <string.h>

enum usb_eptype {
	USB_ENDPOINT_DISABLE        = 0x00,
	USB_ENDPOINT_CONTROL        = 0x01,
	USB_ENDPOINT_ISOCHRONOUS    = 0x02,
	USB_ENDPOINT_BULK           = 0x03,
	USB_ENDPOINT_INTERRUPT      = 0x04,
	USB_ENDPOINT_DUALBANK       = 0x05,
};

enum usb_packet_size {
	USB_PACKET_SIZE_8			= 0x00,
	USB_PACKET_SIZE_16			= 0x01,
	USB_PACKET_SIZE_32			= 0x02,
	USB_PACKET_SIZE_64			= 0x03,
	/* The following values are only valid on isochronous endpoints */
	USB_PACKET_SIZE_128			= 0x04,
	USB_PACKET_SIZE_256			= 0x05,
	USB_PACKET_SIZE_512			= 0x06,
	USB_PACKET_SIZE_1023		= 0x07,
};

static void usb_calibrate(void) {
	uint32_t pad_transn, pad_transp, pad_trim;
	
	pad_transn = ((USB_FUSES_TRANSN_ADDR  & USB_FUSES_TRANSN_Msk) >> USB_FUSES_TRANSN_Pos);
	pad_transp = ((USB_FUSES_TRANSP_ADDR  & USB_FUSES_TRANSP_Msk) >> USB_FUSES_TRANSP_Pos);
	pad_trim = ((USB_FUSES_TRIM_ADDR  & USB_FUSES_TRIM_Msk) >> USB_FUSES_TRIM_Pos);

	USB->DEVICE.PADCAL.reg = (
			USB_PADCAL_TRANSN(pad_transn) |
			USB_PADCAL_TRANSP(pad_transp) |
			USB_PADCAL_TRIM(pad_trim));
}

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

	usb_calibrate();
	
	USB->DEVICE.CTRLA.reg = (
			USB_CTRLA_MODE_DEVICE |
			USB_CTRLA_RUNSTDBY);

	USB->DEVICE.DESCADD.reg = (uint32_t)(&dev->endpoints[0]);

	USB->DEVICE.CTRLB.bit.SPDCONF = USB_DEVICE_CTRLB_SPDCONF_FS_Val;
	USB->DEVICE.CTRLB.bit.DETACH = 0;

	memset(&dev->endpoints, 0, sizeof(dev->endpoints) * sizeof(UsbDeviceDescriptor));

	USB->DEVICE.INTENSET.bit.EORST = 1;

	return 0;
}

int usb_open(usbdev_t *dev) {
	USB->DEVICE.CTRLA.bit.ENABLE = 1;
	return 0;
}

void usb_isconfigured(usbdev_t *dev) {
	if(USB->DEVICE.INTFLAG.bit.EORST) {
		USB->DEVICE.INTFLAG.reg = USB_DEVICE_INTFLAG_EORST;
		USB->DEVICE.DADD.reg = (
				USB_DEVICE_DADD_ADDEN |
				USB_DEVICE_DADD_DADD(0));

		/* Configure Endpoint 0 as both Control IN and Control OUT */
		USB->DEVICE.DeviceEndpoint[0].EPCFG.reg = (
				USB_DEVICE_EPCFG_EPTYPE0(USB_ENDPOINT_CONTROL) |
				USB_DEVICE_EPCFG_EPTYPE1(USB_ENDPOINT_CONTROL));
		USB->DEVICE.DeviceEndpoint[0].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_BK0RDY;
		USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSSET_BK1RDY;

		dev->endpoints[0].DeviceDescBank[0].PCKSIZE.reg = (
				USB_DEVICE_PCKSIZE_SIZE(USB_PACKET_SIZE_64) |
				USB_DEVICE_PCKSIZE_MULTI_PACKET_SIZE(8) |
				USB_DEVICE_PCKSIZE_BYTE_COUNT(0)
				);
		dev->endpoints[0].DeviceDescBank[1].PCKSIZE.bit.SIZE = USB_PACKET_SIZE_64;

		dev->endpoints[0].DeviceDescBank[0].ADDR.reg = (uint32_t)&dev->udd_ep_out_cache_buffer[0];
		dev->endpoints[0].DeviceDescBank[1].ADDR.reg = (uint32_t)&dev->udd_ep_in_cache_buffer[0];

		dev->endpoints[0].DeviceDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = 8;
		dev->endpoints[0].DeviceDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;

		USB->DEVICE.DeviceEndpoint[0].EPSTATUSCLR.reg = USB_DEVICE_EPSTATUSCLR_BK0RDY;
	}else{
		if(USB->DEVICE.DeviceEndpoint[0].EPINTFLAG.bit.RXSTP) {
			// TODO Setup callback
		}
	}
}
