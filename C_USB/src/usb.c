#include "usb.h"

#include "port_SAMD51G19A.h"
#include "registers.h"
#include "util.h"

typedef struct PACKED {
  usb_configuration_descriptor_t configuration_descriptor;
  usb_interface_descriptor_t interface_descriptor;
} usb_descriptor_collection_t;

usb_device_descriptor_t device_descriptor = {
    .bLength = sizeof(usb_device_descriptor_t),
    .bDescriptorType = USB_DEVICE_DESCRIPTOR,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0xFF,
    .bDeviceSubClass = 0xFF,
    .bDeviceProtocol = 0xFF,
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
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0b10000000,
    .bMaxPower = 125,
};

const usb_interface_descriptor_t interface_descriptor = {
    .bLength = sizeof(usb_interface_descriptor_t),
    .bDescriptorType = USB_INTERFACE_DESCRIPTOR,
    .bInterfaceNumber = 0,
    .bAlternateSetting = 0,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFE,
    .bInterfaceSubClass = 1,
    .bInterfaceProtocol = 1,
    .iInterface = 0,
};

usb_descriptor_collection_t descriptor_collection = {
    .configuration_descriptor = configuration_descriptor,
    .interface_descriptor = interface_descriptor,
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
    default:
      error();
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
          error();
          break;
      }
    }
  }
}
