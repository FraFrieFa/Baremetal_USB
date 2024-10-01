#include "registers.h"
#include "usb.h"
#include "util.h"
#include "sercom.h"

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

  // configure LED GPIO
  PB_DIR |= 1 << 2;
  PB_DIR |= 1 << 3;

  led_set_color(0, 0, 0);

  usb_init();

  sercom_init(SERCOM4);

  // configure PA22 (D13) as input
  PA_DIR &= ~ (1 << 22);
  u8 pa22_value = PA_IN & (1 << 22);

  while (1) {
	if(PA_IN & (1 << 22) != pa22_value){
		LOG("LEVEL CHANGE\n");
		pa22_value = PA_IN & (1 << 22);
	}
	if (pa22_value){
		LOG("HIGH\n");
	}else {
		LOG("LOW\n");
	}
    //asm volatile("wfi");
    delay(1000000);
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
