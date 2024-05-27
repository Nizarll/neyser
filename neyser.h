/*
 * Neyser - OpenSource serialization library !
 *
 * Description: Neyser is a collection of useful functions for networking, sockets, and serialization.
 * It is released under the MIT License, which means you are free to use, modify, and distribute it.
 *
 * Author: Nizar yatim
 * Contact: forsubsofmy@gmail.com
 */

#ifndef NEYSER_H
#define NEYSER_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#ifndef __ORDER_LITTLE_ENDIAN__
  #define __ORDER_LITTLE_ENDIAN__ 1234
#endif

#ifndef __ORDER_BIG_ENDIAN__
  #define __ORDER_BIG_ENDIAN__ 4321
#endif

#if defined(MSC_VER)
	#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
	#else
    #define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
	#endif
#endif

#define ANSI_COLOR_RESET             "\x1b[0m"
#define ANSI_COLOR_BLACK             "\x1b[30m"
#define ANSI_COLOR_RED               "\x1b[31m"
#define ANSI_COLOR_GREEN             "\x1b[32m"
#define ANSI_COLOR_YELLOW            "\x1b[33m"
#define ANSI_COLOR_BLUE              "\x1b[34m"
#define ANSI_COLOR_MAGENTA           "\x1b[35m"
#define ANSI_COLOR_CYAN              "\x1b[36m"
#define ANSI_COLOR_WHITE             "\x1b[37m"
#define ANSI_COLOR_BRIGHT_BLACK      "\x1b[30;1m"
#define ANSI_COLOR_BRIGHT_RED        "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_GREEN      "\x1b[32;1m"
#define ANSI_COLOR_BRIGHT_YELLOW     "\x1b[33;1m"
#define ANSI_COLOR_BRIGHT_BLUE       "\x1b[34;1m"
#define ANSI_COLOR_BRIGHT_MAGENTA    "\x1b[35;1m"
#define ANSI_COLOR_BRIGHT_CYAN       "\x1b[36;1m"
#define ANSI_COLOR_BRIGHT_WHITE      "\x1b[37;1m"
#define ANSI_BG_COLOR_BLACK          "\x1b[40m"
#define ANSI_BG_COLOR_RED            "\x1b[41m"
#define ANSI_BG_COLOR_GREEN          "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW         "\x1b[43m"
#define ANSI_BG_COLOR_BLUE           "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA        "\x1b[45m"
#define ANSI_BG_COLOR_CYAN           "\x1b[46m"
#define ANSI_BG_COLOR_WHITE          "\x1b[47m"
#define ANSI_BG_COLOR_BRIGHT_BLACK   "\x1b[40;1m"
#define ANSI_BG_COLOR_BRIGHT_RED     "\x1b[41;1m"
#define ANSI_BG_COLOR_BRIGHT_GREEN   "\x1b[42;1m"
#define ANSI_BG_COLOR_BRIGHT_YELLOW  "\x1b[43;1m"
#define ANSI_BG_COLOR_BRIGHT_BLUE    "\x1b[44;1m"
#define ANSI_BG_COLOR_BRIGHT_MAGENTA "\x1b[45;1m"
#define ANSI_BG_COLOR_BRIGHT_CYAN    "\x1b[46;1m"
#define ANSI_BG_COLOR_BRIGHT_WHITE   "\x1b[47;1m"
#define ANSI_STYLE_BOLD              "\x1b[1m"
#define ANSI_STYLE_UNDERLINE         "\x1b[4m"
#define ANSI_STYLE_REVERSED          "\x1b[7m"

#define LOG_OKAY(msg, ...)                                                     \
  printf(ANSI_COLOR_GREEN "[✓] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)

#define LOG_ERR(msg, ...)                                                      \
  do {                                                                         \
    printf(ANSI_COLOR_RED "[x] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__);  \
    exit(1);                                                                   \
  } while (0)

#define LOG_WARN(msg, ...)                                                     \
  printf(ANSI_COLOR_YELLOW "[i] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)

#define byte 8

#define u16_half_first 0x000000ff
#define u16_half_sec   0x0000ff00
#define u32_half_first 0x0000ffff
#define u32_half_sec   0xffff0000
#define u32_half_first 0x0000ffff
#define u32_half_sec   0xffff0000
#define u64_half_first 0xffffffff

#define u64_half_sec (UINT64_C(0xffffffff) << 32)

typedef int16_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint16_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void serialize_i16(u16 var, i8* buff, size_t size);
void serialize_i32(u32 var, i8* buff, size_t size);
void serialize_i64(u64 var, i8* buff, size_t size);
void serialize_u16(u16 var, i8* buff, size_t size);
void serialize_u32(u32 var, i8* buff, size_t size);
void serialize_u64(u64 var, i8* buff, size_t size);
void serialize_float(float var, i8* buff, size_t size);
void serialize_struct(void* data, size_t data_size, i8* buff, size_t buff_size);

i16 deserialize_i16(i8* buff, size_t size);
i32 deserialize_i32(i8* buff, size_t size);
i64 deserialize_i64(i8* buff, size_t size);
u16 deserialize_u16(i8* buff, size_t size);
u32 deserialize_u32(i8* buff, size_t size);
u64 deserialize_u64(i8* buff, size_t size);
float deserialize_float(i8* buff, size_t size);
char* derialize_struct(i8* buff, size_t size); // type-punning to return the base struct

typedef struct {
        u8 sign;    // 1 bit float sign
        u8 exp;    // 8 bit float exponent
        u32 mant; // 24 bit float mantissa
}f_pack_t;

#endif // NEYSER_H
