#ifndef USBDEV_H
#define USBDEV_H

typedef struct usbdev_s usbdev_t;
typedef void (*usb_enumerate_handler)(usbdev_t *);

int usb_setup(usbdev_t *dev, usb_enumerate_handler enumerate_func);
int usb_open(usbdev_t *dev);
int usb_isconfigured(usbdev_t *dev);

#endif
