#pragma once

#include "defines.h"

#define MCLK_AHBMASK *((volatile u32*)0x40000810)
#define MCLK_APBBMASK *((volatile u32*)0x40000818)

#define OSCCTRL_EVCTRL *((volatile u8*)0x40001000)
#define OSCCTRL_DFLLCTRLA *((volatile u8*)0x4000101C)
#define OSCCTRL_DFLLCTRLB *((volatile u8*)0x40001020)

#define GCLK_CTRLA *((volatile u8*)0x40001C00)
#define GCLK_GENCTRL0 *((volatile u32*)0x40001C20)
#define GCLK_GENCTRL1 *((volatile u32*)0x40001C24)
#define GCLK_PCHCTRL0 *((volatile u32*)0x40001C80)
#define GCLK_PCHCTRL10 *((volatile u32*)0x40001CA8)

#define USB_CTRLA *((volatile u8*)0x41000000)
#define USB_CTRLB *((volatile u16*)0x41000008)
#define USB_DADD *((volatile u8*)0x4100000A)

#define USB_INTENSET *((volatile u8*)0x41000018)
#define USB_INTFLAG *((volatile u8*)0x4100001C)
#define USB_DESCADD *((volatile u32*)0x41000024)
#define USB_EPINTSMRY *((volatile u16*)0x41000020)
#define USB_EPCFGn(n) *((volatile u8*)0x41000100 + n * 0x20)
#define USB_EPSTATUSCLRn(n) *((volatile u8*)0x41000104 + n * 0x20)
#define USB_EPSTATUSSETn(n) *((volatile u8*)0x41000105 + n * 0x20)
#define USB_EPSTATUSn(n) *((volatile u8*)0x41000106 + n * 0x20)
#define USB_EPINTFLAGn(n) *((volatile u8*)0x41000107 + n * 0x20)
#define USB_EPINTENSETn(n) *((volatile u8*)0x41000109 + n * 0x20)

// PORT base 0x41008000
#define PA_DIR *((volatile u32*)0x41008000)
#define PA_OUT *((volatile u32*)0x41008010)
#define PA_IN *((volatile u32*)0x41008020)
#define PA_PMUX0 *((volatile u8*)0x41008030)
#define PA_PMUX12 *((volatile u8*)0x4100803C)
#define PA_PINCFG0 *((volatile u8*)0x41008040)
#define PA_PINCFG24 *((volatile u8*)0x41008058)
#define PA_PINCFG25 *((volatile u8*)0x41008059)

#define PB_DIR *((volatile u32*)0x41008080)
#define PB_OUT *((volatile u32*)0x41008090)
#define PB_PMUX0 *((volatile u8*)0x410080B0)
#define PB_PMUX4 *((volatile u8*)0x410080B4)
#define PB_PINCFG0 *((volatile u8*)0x410080C0)
#define PB_PINCFG8 *((volatile u8*)0x410080C8)
#define PB_PINCFG9 *((volatile u8*)0x410080C9)


// Adafruit Isybitsy M4 Express uses uf2-samdx1 bootloader
// https://github.com/adafruit/uf2-samdx1
// To return to bootloader RESET_VALUE has to be written to RESET_ADDR
#define RESET_ADDR *((volatile u32*)(0x20030000 - 4))
#define RESET_VALUE 0xf01669ef

#define NVIC_ISER0 *((volatile u32*)0xE000E100)
#define NVIC_ISER1 *((volatile u32*)0xE000E104)
#define NVIC_ISER2 *((volatile u32*)0xE000E108)

#define AIRCR *((volatile u32*)0xE000ED0C)
