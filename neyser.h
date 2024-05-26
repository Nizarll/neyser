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

#include "ansi.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>

#ifdef MSC_VER
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __ORDER_LITTLE_ENDIAN__ 4321
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#else
#define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
#endif
#endif

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

#define u64_half_sec UINT64_C(u64_half_first) << 32

typedef uint16_t i8;
typedef uint16_t i16;
typedef uint32_t i32;
typedef uint64_t i64;

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

i16 deserialize_u16(i8* buff, size_t size);
i32 deserialize_u32(i8* buff, size_t size);
i64 deserialize_u64(i8* buff, size_t size);
u16 deserialize_u16(i8* buff, size_t size);
u32 deserialize_u32(i8* buff, size_t size);
u64 deserialize_u64(i8* buff, size_t size);
float serialize_float(float var, size_t size);
char* derialize_struct(i8* buff, size_t size); // type-punning to return the base struct 

typedef struct {
	u8 sign;    // 1 bit float sign
	u8 exp;    // 8 bit float exponent
	u32 mant; // 24 bit float mantissa
}f_pack_t;

// IEE-754 floating single-precision floating number
// serialize float using its IEE-754 format
//
//

u16 deserialize_u16(i8* buff, size_t size) {
	if (size < 2) LOG_ERR("error! could not deserialize u16!");
	return (u16) *buff | *(buff + 1);
}

u32 deserialize_u32(i8* buff, size_t size) {
	if (size < 4) LOG_ERR("error! could not deserialize u16!");
	return (u32) (deserialize_u16(buff, size) | ((u32) deserialize_buff(buff + 2, size - 2)) << 2 * byte);
}

u64 deserialize_u64(i8* buff, size_t size)  {
	if (size < 8) LOG_ERR("error! could not deserialize u16!");
	return (u64) (deserialize_u32(buff, size) | ((u32) deserialize_buff(buff + 4, size - 4)) << 4 * byte);
}

float deserialize_float(i8* buff, size_t size) {
	if (size < 6) LOG_ERR("error! could not deserialize u16!");
	return (ldexpf((float)(deserialize_u32(buff + 2, size - 2) / powf(2, 24)), *(buff + 1));
}

void serialize_float(float var, i8* buff, size_t size) {
	f_pack_t pack = {.sign = var > .0f ? 0 : 1};
	pack.mant = (u32)(frexpf(var, &pack.exp) * pow(2, 24));
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	serialize_u32(pack.mant, buff, size);
	*(buff + 4) = pack.sign;
	*(buff + 5) = pack.exp;
#else
	*buff = pack.sign;
	*(buff + 1) = pack.exp;
	serialize_u32(pack.mant, buff + 2, size - 2);
#endif
}

void serialize_u16(u16 var, i8* buff, size_t size) {
	if (size < 2) 
		LOG_ERR("could not serialize u16 !");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	*buff = (var & u16_half_sec) >> 8;
	*(buff + 1) = var & u16_half_first;
#else
	*buff = var & u16_half_first;
	*(buff + 1) = (var & u16_half_sec) >> 8;
#endif
}

void serialize_u32(u32 var, i8* buff, size_t size) {
	if (size < 4)
		LOG_ERR("could not serialize u32 !");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	serialize_u16((u16)((var & u32_half_sec) >> (2 * byte)), buff, size);
	serialize_u16((u16)(var & u32_half_first), buff + 2, size - 2);
#else
	serialize_u16((u16)(n_var & u32_half_first), buff, size);
	serialize_u16((u16)((n_var & u32_half_sec) >> 16), buff + 2, size - 2);
#endif
}

void serialize_u64(u64 var, i8* buff, size_t size) {
	if (size < 8)
		LOG_ERR("could not serialize u64 !");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	serialize_u32((u32)((var & u64_half_sec) >> (4 * byte)), buff, size);
	serialize_u32((u32)(var & u64_half_first), buff + 2, size - 2);
#else
	serialize_u32((u32)(var & u64_half_first), buff, size);
	serialize_u32((u32)(var & u32_half_sec) >> 32), buff + 2, size - 2);
#endif 
}

#endif // NEYSER_H
