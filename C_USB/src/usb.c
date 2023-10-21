#include "usb.h"

#include "port_SAMD51G19A.h"
#include "registers.h"
#include "util.h"

typedef struct PACKED {
  usb_configuration_descriptor_t configuration_descriptor;
  usb_interface_association_descriptor_t associaton_descr;
  usb_interface_descriptor_t interface_descriptor_cdc;
  usb_cdc_acm_header_descriptor_t cdc_acm_header;
  usb_cdc_acm_functional_descriptor_t cdc_acm_descr;
  usb_cdc_union_descriptor_t cdc_union_descr;
  usb_endpoint_descriptor_t ep3_descriptor;
  usb_interface_descriptor_t interface_descriptor_cdc_data;
  usb_endpoint_descriptor_t ep1_descriptor;
  usb_endpoint_descriptor_t ep2_descriptor;
} usb_descriptor_collection_t;

usb_device_descriptor_t device_descriptor = {
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0xEF,
    .bDeviceSubClass = 0x2,
    .bDeviceProtocol = 0x1,
    .bMaxPacketSize = 64,
    .idVendor = 0x1209,
    .idProduct = 1,
    .bcdDevice = 0,
    .iManufacturer = 0,
    .iProduct = 0,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

const usb_configuration_descriptor_t configuration_descriptor = {
    .bLength = sizeof(usb_configuration_descriptor_t),
    .bDescriptorType = USB_CONFIGURATION_DESCRIPTOR,
    .wTotalLength = sizeof(usb_descriptor_collection_t),
    .bNumInterfaces = 2,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0b10000000,
    .bMaxPower = 125,
};

const usb_interface_association_descriptor_t association_descr = {
    .bLength = sizeof(usb_interface_association_descriptor_t),
    .bDescriptorType = USB_INTERFACE_ASSOCIATION_DESCRIPTOR,
    .bFirstInterface = 0,
    .bInterfaceCount = 2,
    .bFunctionClass = 2,
    .bFunctionSubClass = 2,
    .bFunctionProtocol = 1,
    .iFunction = 0,
};

const usb_interface_descriptor_t interface_descriptor_cdc = {
    .bLength = sizeof(usb_interface_descriptor_t),
    .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 1,
    .bInterfaceClass = 2,
    .bInterfaceSubClass = 2,
    .bInterfaceProtocol = 1,
    .iInterface = 0,
};

const usb_cdc_acm_header_descriptor_t acm_header = {
    .bFunctionLength = sizeof(usb_cdc_acm_header_descriptor_t),
    .bDescriptorType = USB_CLASS_SPECIFIC_INTERFACE_DESCRIPTOR,
    .bDescriptorSubtype = USB_CDC_SUBTYPE_HEADER,
    .bcdCDC = 0x0110,
};

const usb_cdc_acm_functional_descriptor_t acm_descr = {
    .bFunctionLength = sizeof(usb_cdc_acm_functional_descriptor_t),
    .bDescriptorType = USB_CLASS_SPECIFIC_INTERFACE_DESCRIPTOR,
    .bDescriptorSubtype = USB_CDC_SUBTYPE_ACM,
    .bmCapabilities = 6,
};

const usb_cdc_union_descriptor_t acm_union = {
    .bFunctionLength = sizeof(usb_cdc_union_descriptor_t),
    .bDescriptorType = USB_CLASS_SPECIFIC_INTERFACE_DESCRIPTOR,
    .bDescriptorSubtype = USB_CDC_SUBTYPE_UNION,
    .bControlInterface = 0,
    .bSubordinateInterface0 = 1,
};

const usb_endpoint_descriptor_t ep3_descriptor = {
    .bLength = sizeof(usb_endpoint_descriptor_t),
    .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
    .bEndpointAddress = (1 << 7) | 3,
    .bmAttributes = 3,
    .wMaxPacketSize = 8,
    .bInterval = 0xFF,
};

const usb_interface_descriptor_t interface_descriptor_cdc_data = {
    .bLength = sizeof(usb_interface_descriptor_t),
    .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
    .bInterfaceNumber = 1,
    .bAlternateSetting = 0,
    .bNumEndpoints = 2,
    .bInterfaceClass = 10,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
};

const usb_endpoint_descriptor_t ep1_descriptor = {
    .bLength = sizeof(usb_endpoint_descriptor_t),
    .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
    .bEndpointAddress = (1 << 7) | 1,
    .bmAttributes = 2,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

const usb_endpoint_descriptor_t ep2_descriptor = {
    .bLength = sizeof(usb_endpoint_descriptor_t),
    .bDescriptorType = USB_ENDPOINT_DESCRIPTOR,
    .bEndpointAddress = (0 << 7) | 2,
    .bmAttributes = 2,
    .wMaxPacketSize = 64,
    .bInterval = 0,
};

usb_descriptor_collection_t descriptor_collection = {
    .configuration_descriptor = configuration_descriptor,
    .associaton_descr = association_descr,
    .interface_descriptor_cdc = interface_descriptor_cdc,
    .cdc_acm_header = acm_header,
    .cdc_acm_descr = acm_descr,
    .cdc_union_descr = acm_union,
    .ep3_descriptor = ep3_descriptor,
    .interface_descriptor_cdc_data = interface_descriptor_cdc_data,
    .ep1_descriptor = ep1_descriptor,
    .ep2_descriptor = ep2_descriptor,
};

volatile int current_configuration = -1;

void usb_init() {
  // 1. Setup GPIO
  setup_gpio();

  // 2. Setup 48 MHz +- 0.25%
  setup_clock();

  // 3. Enable USB controller in host mode, full-speed
  setup_usb();

  // 4. Enable interrupts
  setup_interrupts();

  // 5. Attach
  usb_attach();
}

void send_usb_descriptor(u8 type, u8 index, u16 maxLength) {
  switch (type) {
    case USB_DEVICE_DESCRIPTOR:
      usb_try_send(&device_descriptor,
                   min(maxLength, sizeof(usb_device_descriptor_t)));
      wait_for_zlp_out();
      break;
    case USB_QUALIFIER_DESCRIPTOR:
      usb_stall_next_in();
      break;
    case USB_CONFIGURATION_DESCRIPTOR:
      usb_try_send(&descriptor_collection,
                   min(maxLength, sizeof(usb_descriptor_collection_t)));
      wait_for_zlp_out();
      break;
    case USB_DEBUG_DESCRIPTOR:
      usb_stall_next_in();
      break;
    default:
      reset_to_bootloader();
      break;
  }
}

void handle_setup_packet(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex,
                         u16 wLength) {
  u8 recipient = (bmRequestType & 0x1F);
  u8 type = (bmRequestType & 0x60) >> 5;

  if (recipient == 0) {  // Recipient: Device
    if (type == 0) {     // Type: Standard
      switch (bRequest) {
        case USB_GET_DESCRIPTOR: {
          u8 type = wValue >> 8;
          u8 index = wValue & 0xFF;
          send_usb_descriptor(type, index, wLength);
        } break;

        case USB_SET_ADDRESS:
          usb_try_send_zlp_in();
          usb_set_address(wValue);
          break;

        case USB_SET_CONFIGURATION:
          current_configuration = wValue;
          usb_try_send_zlp_in();
          break;

        default:
          reset_to_bootloader();
          break;
      }
    }
  } else if (recipient == 1) {
    if (type == 1) {
      switch (bRequest) {
        case USB_CDC_SET_LINE_CODING:
          // TODO handle line coding
          usb_try_send_zlp_in();
          break;

        case USB_CDC_SET_CONTROL_LINE_STATE:
          // TODO handle control line state
          usb_try_send_zlp_in();
          break;

        default:
          reset_to_bootloader();
          break;
      }
    } else {
      reset_to_bootloader();
    }
  } else {
    reset_to_bootloader();
  }
}
