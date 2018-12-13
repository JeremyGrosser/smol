#ifndef DEV_USB_H
#define DEV_USB_H

#include <stdint.h>


struct usb_device_descriptor {
    /** Size of this descriptor (in bytes) */
    uint8_t  bLength;

    /* Descriptor type */
    uint8_t  bDescriptorType;

    /* USB specification release number in binary-coded decimal. A value of
     * 0x0200 indicates USB 2.0, 0x0110 indicates USB 1.1, etc. */
    uint16_t bcdUSB;

    /* USB-IF class code for the device. */
    uint8_t  bDeviceClass;

    /* USB-IF subclass code for the device, qualified by the bDeviceClass value */
    uint8_t  bDeviceSubClass;

    /* USB-IF protocol code for the device, qualified by the bDeviceClass and bDeviceSubClass values */
    uint8_t  bDeviceProtocol;

    /* Maximum packet size for endpoint 0 */
    uint8_t  bMaxPacketSize0;

    /* USB-IF vendor ID */
    uint16_t idVendor;

    /* USB-IF product ID */
    uint16_t idProduct;

    /* Device release number in binary-coded decimal */
    uint16_t bcdDevice;

    /* Index of string descriptor describing manufacturer */
    uint8_t  iManufacturer;

    /* Index of string descriptor describing product */
    uint8_t  iProduct;

    /* Index of string descriptor containing device serial number */
    uint8_t  iSerialNumber;

    /* Number of possible configurations */
    uint8_t  bNumConfigurations;
};


struct usb_endpoint_descriptor {
    /* Size of this descriptor (in bytes) */
    uint8_t  bLength;

    /* Descriptor type */
    uint8_t  bDescriptorType;

    /* The address of the endpoint described by this descriptor. Bits 0:3 are
     * the endpoint number. Bits 4:6 are reserved. Bit 7 indicates direction
     */
    uint8_t  bEndpointAddress;

    /* Attributes which apply to the endpoint when it is configured using
     * the bConfigurationValue. Bits 0:1 determine the transfer type and
     * correspond to \ref libusb_transfer_type. Bits 2:3 are only used for
     * isochronous endpoints and correspond to \ref libusb_iso_sync_type.
     * Bits 4:5 are also only used for isochronous endpoints and correspond to
     * \ref libusb_iso_usage_type. Bits 6:7 are reserved.
     */
    uint8_t  bmAttributes;

    /** Maximum packet size this endpoint is capable of sending/receiving. */
    uint16_t wMaxPacketSize;

    /** Interval for polling endpoint for data transfers. */
    uint8_t  bInterval;

    /** For audio devices only: the rate at which synchronization feedback
     * is provided. */
    uint8_t  bRefresh;

    /** For audio devices only: the address if the synch endpoint */
    uint8_t  bSynchAddress;

    /** Extra descriptors. If libusb encounters unknown endpoint descriptors,
     * it will store them here, should you wish to parse them. */
    const unsigned char *extra;

    /** Length of the extra descriptors, in bytes. */
    int extra_length;
};


struct usb_interface_descriptor {
    /** Size of this descriptor (in bytes) */
    uint8_t  bLength;

    /** Descriptor type. Will have value
     * \ref libusb_descriptor_type::LIBUSB_DT_INTERFACE LIBUSB_DT_INTERFACE                                                   
     * in this context. */
    uint8_t  bDescriptorType;

    /** Number of this interface */
    uint8_t  bInterfaceNumber;

    /** Value used to select this alternate setting for this interface */
    uint8_t  bAlternateSetting;

    /** Number of endpoints used by this interface (excluding the control
     * endpoint). */
    uint8_t  bNumEndpoints;

    /** USB-IF class code for this interface. See \ref libusb_class_code. */
    uint8_t  bInterfaceClass;

    /** USB-IF subclass code for this interface, qualified by the
     * bInterfaceClass value */
    uint8_t  bInterfaceSubClass;

    /** USB-IF protocol code for this interface, qualified by the
     * bInterfaceClass and bInterfaceSubClass values */
    uint8_t  bInterfaceProtocol;

    /** Index of string descriptor describing this interface */
    uint8_t  iInterface;

    /** Array of endpoint descriptors. This length of this array is determined
     * by the bNumEndpoints field. */
    const struct usb_endpoint_descriptor *endpoint;

    /** Extra descriptors. If libusb encounters unknown interface descriptors,
     * it will store them here, should you wish to parse them. */
    const unsigned char *extra;

    /** Length of the extra descriptors, in bytes. */
    int extra_length;
};


struct usb_interface {
    /** Array of interface descriptors. The length of this array is determined
     * by the num_altsetting field. */
    const struct usb_interface_descriptor *altsetting;

    /** The number of alternate settings that belong to this interface */
    int num_altsetting;
};


struct usb_config_descriptor {
    /** Size of this descriptor (in bytes) */
    uint8_t  bLength;

    /** Descriptor type. Will have value
     * \ref libusb_descriptor_type::LIBUSB_DT_CONFIG LIBUSB_DT_CONFIG
     * in this context. */
    uint8_t  bDescriptorType;

    /** Total length of data returned for this configuration */
    uint16_t wTotalLength;

    /** Number of interfaces supported by this configuration */
    uint8_t  bNumInterfaces;

    /** Identifier value for this configuration */
    uint8_t  bConfigurationValue;

    /** Index of string descriptor describing this configuration */
    uint8_t  iConfiguration;

    /** Configuration characteristics */
    uint8_t  bmAttributes;

    /** Maximum power consumption of the USB device from this bus in this
     * configuration when the device is fully operation. Expressed in units
     * of 2 mA when the device is operating in high-speed mode and in units
     * of 8 mA when the device is operating in super-speed mode. */
    uint8_t  MaxPower;

    /** Array of interfaces supported by this configuration. The length of
     * this array is determined by the bNumInterfaces field. */
    const struct usb_interface *interface;

    /** Extra descriptors. If libusb encounters unknown configuration
     * descriptors, it will store them here, should you wish to parse them. */
    const unsigned char *extra;

    /** Length of the extra descriptors, in bytes. */
    int extra_length;
};


struct usb_control_setup {
    /** Request type. Bits 0:4 determine recipient, see
     * \ref libusb_request_recipient. Bits 5:6 determine type, see
     * \ref libusb_request_type. Bit 7 determines data transfer direction, see
     * \ref libusb_endpoint_direction.
     */
    uint8_t  bmRequestType;

    /** Request. If the type bits of bmRequestType are equal to
     * \ref libusb_request_type::LIBUSB_REQUEST_TYPE_STANDARD
     * "LIBUSB_REQUEST_TYPE_STANDARD" then this field refers to
     * \ref libusb_standard_request. For other cases, use of this field is
     * application-specific. */
    uint8_t  bRequest;

    /** Value. Varies according to request */
    uint16_t wValue;

    /** Index. Varies according to request, typically used to pass an index
     * or offset */
    uint16_t wIndex;

    /** Number of bytes to transfer */
    uint16_t wLength;
};

#endif
