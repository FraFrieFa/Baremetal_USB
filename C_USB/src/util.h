#pragma once

#include "defines.h"

void reset_to_bootloader();
void delay(int n);

void error();

void led_set_color(u8 r, u8 g, u8 b);