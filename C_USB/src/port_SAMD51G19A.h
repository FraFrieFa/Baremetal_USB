#pragma once

#include "defines.h"
#include "usb.h"

#define EP_AMOUNT 8

#define EP_0_PCKSIZE 64

#define LOG_SIZE 512

extern volatile u32 log_size;
extern volatile u8 log_buffer[LOG_SIZE];

typedef struct PACKED {
  volatile u8* B0_ADDR;
  u32 B0_PCKSIZE;
  u16 B0_EXTREG;
  u8 B0_STATUS_BK;
  u8 _pad0;
  u32 _pad1;
  volatile u8* B1_ADDR;
  u32 B1_PCKSIZE;
  u16 _pad2;
  u8 B1_STATUS_BK;
  u8 _pad3;
  u32 _pad4;
} ep_descriptor_t;

void setup_gpio();

void setup_clock();

void setup_usb();

void setup_interrupts();

void usb_attach();

void ISR_usb_general();

void usb_try_send(const void* data, u32 length);
void usb_try_send_zlp_in();

void wait_for_zlp_out();

void usb_set_address(u16 addr);

void usb_stall_next_in();
