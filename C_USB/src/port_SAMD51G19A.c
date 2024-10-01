#include "port_SAMD51G19A.h"

#include "registers.h"
#include "util.h"

volatile u8 ALIGN(4) control_in[EP_0_PCKSIZE];
volatile u8 ALIGN(4) control_out[EP_0_PCKSIZE];

volatile u8 ALIGN(4) cdc_control_in[EP_0_PCKSIZE];

volatile u8 ALIGN(4) cdc_in[64];
volatile u8 ALIGN(4) cdc_out[64];

volatile u32 log_size = 0;
volatile u8 log_buffer[LOG_SIZE] = {};

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
    {
        .B0_ADDR = 0,
        .B0_PCKSIZE = 0,
        .B0_EXTREG = 0,
        .B0_STATUS_BK = 0,
        .B1_ADDR = cdc_in,
        .B1_PCKSIZE = 3 << 28,
        .B1_STATUS_BK = 0,
    },
    {
        .B0_ADDR = cdc_out,
        .B0_PCKSIZE = 3 << 28,
        .B0_EXTREG = 0,
        .B0_STATUS_BK = 0,
        .B1_ADDR = 0,
        .B1_PCKSIZE = 0,
        .B1_STATUS_BK = 0,
    },
    {
        .B0_ADDR = 0,
        .B0_PCKSIZE = 0,
        .B0_EXTREG = 0,
        .B0_STATUS_BK = 0,
        .B1_ADDR = cdc_control_in,
        .B1_PCKSIZE = 0 << 28,
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
  NVIC_ISER2 |= 1 << 18;
  NVIC_ISER2 |= 1 << 19;
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

  USB_EPCFGn(1) = 0x40;
  USB_EPINTENSETn(1) |= 0b1111;

  USB_EPCFGn(2) = 0x4;
  USB_EPINTENSETn(2) |= 0b1111;

  USB_EPCFGn(3) = 0x40;
  USB_EPINTENSETn(3) |= 0b1111;
}

void ISR_usb_general() {

  if (USB_INTFLAG & (1 << 3)) {  // USB reset interrupt
    usb_reset();
    USB_INTFLAG |= 1 << 3;
  }

  if (USB_EPINTSMRY) {

    if (USB_EPINTSMRY & 1) {               // EP0 interrupt
      if (USB_EPINTFLAGn(0) & (1 << 4)) {  // RXSTP
        led_set_color(0,0,100);
        led_set_color(0,0,0);
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
    if (USB_EPINTSMRY & 2) {               // EP1 interrupt -> host wants data
      if (USB_EPINTFLAGn(1) & (1 << 3)) {  // TRFAIL1
        led_set_color(100,0,0);
        led_set_color(0,0,0);
        USB_EPINTFLAGn(1) |= (1 << 3);
        for (int i = 0; i < log_size; i++) {
          cdc_in[i] = log_buffer[i];
        }
        endpoints[1].B1_PCKSIZE = (3 << 28) | log_size;
        log_size = 0;
        USB_EPSTATUSSETn(1) |= 1 << 7;
        while (!(USB_EPINTFLAGn(1) & 2)) {
        }
        USB_EPINTFLAGn(1) |= 2;
      }
    }
    if (USB_EPINTSMRY & 4) {        // EP2 interrupt -> host sends data
      if (USB_EPINTFLAGn(2) & 1) {  // TRCPT0
        led_set_color(0,100,0);
        led_set_color(0,0,0);

        int receivedCount = endpoints[2].B0_PCKSIZE & 0x3FFF;

        if (cdc_out[0] == 'r') {
          reset_to_bootloader();
        }

        for (int i = 0; i < receivedCount; i++) {
          log_buffer[log_size] = cdc_out[i];
          log_size++;
        }

        // clear interrupt flag
        USB_EPINTFLAGn(2) |= 1;
        // clear bank 0
        USB_EPSTATUSCLRn(2) |= 1 << 6;
      }
    }
    if (USB_EPINTSMRY & 8) {               // EP3 interrupt
      if (USB_EPINTFLAGn(3) & (1 << 3)) {  // TRFAIL1
        USB_EPINTFLAGn(3) |= (1 << 3);
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
