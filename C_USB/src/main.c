#include "registers.h"
#include "usb.h"
#include "util.h"

extern void ISR_usb_general();

extern volatile u32 data_start_flash;
extern volatile u32 data_start_ram;
extern volatile u32 data_end_ram;

extern volatile u32 bss_start_ram;
extern volatile u32 bss_end_ram;

void main() {
  // Copy data to ram
  for (volatile u32 *writer = &data_start_ram, *reader = &data_start_flash;
       writer != &data_end_ram; writer++, reader++) {
    *writer = *reader;
  }

  // Clear bss
  for (volatile u32* writer = &bss_start_ram; writer < &bss_end_ram; writer++) {
    *writer = 0;
  }

  PB_DIR |= 1 << 2;
  PB_DIR |= 1 << 3;

  led_set_color(0, 0, 0);

  usb_init();

  while (1) {
    asm volatile("wfi");
  }
}

void undefined_interrupt() {
  while (1)
    ;
}

__attribute__((section(".vector_table"))) void* vector_table[] = {
    (void*)0x20030000,
    main,
    [2 ... 95] = undefined_interrupt,
    ISR_usb_general,
    undefined_interrupt,
    ISR_usb_general,
    ISR_usb_general,
};
