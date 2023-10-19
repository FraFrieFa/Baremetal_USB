#pragma once

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define NOP asm volatile("nop")
#define PACKED __attribute__((__packed__))
#define ALIGN(n) __attribute__((aligned(n)))

#define min(x, y) ((x > y) ? y : x)