#include "util.h"

#include "registers.h"

void reset_to_bootloader() {
  RESET_ADDR = RESET_VALUE;
  AIRCR = (0x5FA << 16) | (1 << 2) | (AIRCR & 0x700);
}

void delay(int n) {
  for (int i = 0; i < n; i++) {
    asm volatile("nop");
  }
}

void software_spi_to_led(u8 data) {
  for (int i = 7; i >= 0; i--) {
    if (data & 0x80) {
      PB_OUT |= 1 << 3;  // MOSI 1
    } else {
      PB_OUT &= ~(1 << 3);  // MOSI 0
    }
    data <<= 1;
    PB_OUT |= 1 << 2;
    PB_OUT &= ~(1 << 2);
  }
}

void led_set_color(u8 r, u8 g, u8 b) {
  // package start
  for (int i = 0; i < 4; i++) {
    software_spi_to_led(0);
  }
  // pixel start
  software_spi_to_led(0xFF);
  software_spi_to_led(b);
  software_spi_to_led(g);
  software_spi_to_led(r);
  // pixel end
  software_spi_to_led(0xFF);
}

void blink(int n){
  PA_OUT |= 1 << 22;
  delay(n);
  PA_OUT &= ~(1 << 22);
  delay(n); 
}

void error() {
  led_set_color(0x60, 0, 0);
  while (1)
    ;
}
