#pragma once

#include "defines.h"

#define USB_SET_ADDRESS 5
#define USB_GET_DESCRIPTOR 6
#define USB_SET_CONFIGURATION 9

#define USB_CDC_SET_LINE_CODING 0x20
#define USB_CDC_SET_CONTROL_LINE_STATE 0x22

#define USB_DEVICE_DESCRIPTOR 1
#define USB_CONFIGURATION_DESCRIPTOR 2
#define USB_STRING_DESCRIPTOR 3
#define USB_INTERFACE_DESCRIPTOR 4
#define USB_ENDPOINT_DESCRIPTOR 5
#define USB_QUALIFIER_DESCRIPTOR 6
#define USB_DEBUG_DESCRIPTOR 0xA
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR 0xB
#define USB_CLASS_SPECIFIC_INTERFACE_DESCRIPTOR 0x24

#define USB_CDC_SUBTYPE_HEADER 0
#define USB_CDC_SUBTYPE_ACM 2
#define USB_CDC_SUBTYPE_UNION 6

typedef enum {
  CONTROL,
  INTERRUPT_IN,
  INTERRUPT_OUT,
} ep_type_t;

void usb_init();

void handle_setup_packet(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex,
                         u16 wLength);

typedef struct PACKED {
  u8 bLength;
  u8 bDescriptorType;
  u16 bcdUSB;
  u8 bDeviceClass;
  u8 bDeviceSubClass;
  u8 bDeviceProtocol;
  u8 bMaxPacketSize;
  u16 idVendor;
  u16 idProduct;
  u16 bcdDevice;
  u8 iManufacturer;
  u8 iProduct;
  u8 iSerialNumber;
  u8 bNumConfigurations;
} usb_device_descriptor_t;

typedef struct PACKED {
  u8 bLength;
  u8 bDescriptorType;
  u16 wTotalLength;
  u8 bNumInterfaces;
  u8 bConfigurationValue;
  u8 iConfiguration;
  u8 bmAttributes;
  u8 bMaxPower;
} usb_configuration_descriptor_t;

typedef struct PACKED {
  u8 bLength;
  u8 bDescriptorType;
  u8 bInterfaceNumber;
  u8 bAlternateSetting;
  u8 bNumEndpoints;
  u8 bInterfaceClass;
  u8 bInterfaceSubClass;
  u8 bInterfaceProtocol;
  u8 iInterface;
} usb_interface_descriptor_t;

typedef struct PACKED {
  u8 bLength;
  u8 bDescriptorType;
  u8 bEndpointAddress;
  u8 bmAttributes;
  u16 wMaxPacketSize;
  u8 bInterval;
} usb_endpoint_descriptor_t;

typedef struct PACKED {
  u8 bFunctionLength;
  u8 bDescriptorType;
  u8 bDescriptorSubtype;
  u16 bcdCDC;
} usb_cdc_acm_header_descriptor_t;

typedef struct PACKED {
  u8 bFunctionLength;
  u8 bDescriptorType;
  u8 bDescriptorSubtype;
  u8 bmCapabilities;
} usb_cdc_acm_functional_descriptor_t;

typedef struct PACKED {
  u8 bFunctionLength;
  u8 bDescriptorType;
  u8 bDescriptorSubtype;
  u8 bControlInterface;
  u8 bSubordinateInterface0;
} usb_cdc_union_descriptor_t;

typedef struct PACKED {
  u8 bLength;
  u8 bDescriptorType;
  u8 bFirstInterface;
  u8 bInterfaceCount;
  u8 bFunctionClass;
  u8 bFunctionSubClass;
  u8 bFunctionProtocol;
  u8 iFunction;
} usb_interface_association_descriptor_t;

void LOG(const char*);
