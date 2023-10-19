#include "port_SAMD51G19A.h"

#include "registers.h"
#include "util.h"

volatile u8 ALIGN(4) control_in[EP_0_PCKSIZE];
volatile u8 ALIGN(4) control_out[EP_0_PCKSIZE];

volatile ep_descriptor_t ALIGN(4) endpoints[] = {
    {
        .B0_ADDR = control_out,
        .B0_PCKSIZE = 3 << 28,
        .B0_EXTREG = 0,
        .B0_STATUS_BK = 0,
        .B1_ADDR = control_in,
        .B1_PCKSIZE = 3 << 28,
        .B1_STATUS_BK = 0,
    },
};

void setup_gpio() {
  // enable USB Data Minus PIN PA24
  // enable USB Data Plus PIN PA25
  PA_PMUX12 = 0x77;
  PA_PINCFG24 = 1;
  PA_PINCFG25 = 1;
}

void setup_clock() {
  // In closed-loop operation, the DFLL48M output frequency is continuously
  // regulated against a precise reference clock of relatively low frequency.
  // This will improve the accuracy and stability of the CLK_DFLL48M clock in
  // comparison to the open-loop (free-running) configuration.
  // In closed loop mode DFLL uses Peripheral Channel 0
  // RESET VALUES: CPU frequency, GENCTRL0 48 MHz DFLL48M open loop

  OSCCTRL_DFLLCTRLB = 0;
  OSCCTRL_DFLLCTRLA = 0x82;

  GCLK_GENCTRL0 = 0x106;

  // Set USB channel to GENCTRL0
  GCLK_PCHCTRL10 = 1 << 6;
}

void setup_usb() {
  // Enable USB in MCLK
  MCLK_AHBMASK |= 1 << 10;
  MCLK_APBBMASK |= 1;

  // Enable USB in Device Mode
  USB_CTRLA = 1 << 1;
}

void setup_interrupts() {
  NVIC_ISER2 |= 1 << 16;
  USB_INTENSET |= 1 << 3;
}

void usb_attach() { USB_CTRLB &= ~1; }

// Reset all endpoints and configure EP0 as control
// enable receive setup Interrupt for EP0

void usb_reset() {
  // Endpoints get reset automatically
  USB_DESCADD = (u32)&endpoints;
  // enable first, activate interrupt after because
  // INTENSET is always cleared when disabled
  USB_EPCFGn(0) = 0x11;
  USB_EPINTENSETn(0) |= 1 << 4;
}

void ISR_usb_general() {
  if (USB_INTFLAG & (1 << 3)) {  // USB reset interrupt
    usb_reset();
    USB_INTFLAG |= 1 << 3;
  }

  if (USB_EPINTSMRY) {
    if (USB_EPINTSMRY & 1) {  // EP0 interrupt
      if (USB_EPINTFLAGn(0) & (1 << 4)) {
        u8 bmRequestType = control_out[0];
        u8 bRequest = control_out[1];
        u16 wValue = control_out[3] << 8 | control_out[2];
        u16 wIndex = control_out[5] << 8 | control_out[4];
        u16 wLength = control_out[7] << 8 | control_out[6];
        USB_EPINTFLAGn(0) |= 1 << 4;
        // clear bank 0
        USB_EPSTATUSCLRn(0) |= 1 << 6;
        handle_setup_packet(bmRequestType, bRequest, wValue, wIndex, wLength);
      }
    }
  }
}

void wait_for_zlp_out() {
  if (USB_EPSTATUSn(0) |= (1 << 6)) {
    USB_EPSTATUSCLRn(0) |= 1 << 6;
  }

  while (!(USB_EPINTFLAGn(0) & 1)) {
  }
  // TODO check if packet really has zero length
  USB_EPINTFLAGn(0) |= 1;
  // clear bank 0
  USB_EPSTATUSCLRn(0) |= 1 << 6;
}

void usb_try_send(const void* data, u32 length) {
  while (USB_EPSTATUSSETn(0) & (1 << 7))
    ;
  u8* byteData = (u8*)data;
  endpoints[0].B1_PCKSIZE = (3 << 28) | length;
  for (int i = 0; i < length; i++) {
    control_in[i] = byteData[i];
  }
  USB_EPSTATUSSETn(0) |= 1 << 7;
  while (!(USB_EPINTFLAGn(0) & 2)) {
  }
  USB_EPINTFLAGn(0) |= 2;
}

void usb_try_send_zlp_in() {
  while (USB_EPSTATUSSETn(0) & (1 << 7))
    ;
  endpoints[0].B1_PCKSIZE = (3 << 28);
  USB_EPSTATUSSETn(0) |= 1 << 7;
  while (!(USB_EPINTFLAGn(0) & 2)) {
  }
  USB_EPINTFLAGn(0) |= 2;
}

void usb_set_address(u16 addr) { USB_DADD = (1 << 7) | (addr & 0x7F); }

void usb_stall_next_in() {
  USB_EPSTATUSSETn(0) |= 1 << 5;
  while (!(USB_EPINTFLAGn(0) & (1 << 6))) {
  }
  USB_EPINTFLAGn(0) |= 1 << 6;
  USB_EPSTATUSCLRn(0) |= 1 << 5;
}