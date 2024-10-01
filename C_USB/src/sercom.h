#pragma once

#include "defines.h"

typedef struct PACKED {
	u32 CTRLA;
	u32 CTRLB;
	u32 CTRLC;
	u16 BAUD;
	u8 RXPL;
	u32 reserved_0;
	u8 INTENCLR;
	u8 reserved_1;
	u8 INTENSET;
	u8 reserved_2;
	u8 INTFLAG;
	u8 reserved_3;
	u16 STATUS;
	u32 SYNCBUSY;
	u8 RXERRCNT;
	u8 reserved_4;
	u16 LENGTH;
	u32 reserved_5;
	u32 DATA;
	u32 reserved_6;
	u8 DBGCTRL;
} sercom_unit;


#define SERCOM4 ((sercom_unit*)0x43000000)

void sercom_init(sercom_unit*);
