#ifndef NEYSER_H
#define NEYSER_H

#include "ansi.h"
#include <stdint.h>
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
    printf(ANSI_COLOR_RED "[✘] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__);  \
    exit(1);                                                                   \
  } while (0)

#define LOG_WARN(msg, ...)                                                     \
  printf(ANSI_COLOR_YELLOW "[!] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)

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

u64 htonu64(u64 val);
u64 ntohu64(u64 val);

void serialize_i16(u16 var, i8* buff, size_t size);
void serialize_i32(u32 var, i8* buff, size_t size);
void serialize_i64(u64 var, i8* buff, size_t size);
void serialize_u16(u16 var, i8* buff, size_t size);
void serialize_u32(u32 var, i8* buff, size_t size);
void serialize_u64(u64 var, i8* buff, size_t size);
void serialize_struct(void* data, size_t data_size, i8* buff, size_t buff_size);
// float
i16 deserialize_u16(i8* buff, size_t size);
i32 deserialize_u32(i8* buff, size_t size);
i64 deserialize_u64(i8* buff, size_t size);
u16 deserialize_u16(i8* buff, size_t size);
u32 deserialize_u32(i8* buff, size_t size);
u64 deserialize_u64(i8* buff, size_t size);

void serialize_u16(u16 var, i8* buff, size_t size) {
	if (size < 2) 
		LOG_ERR("could not serialize u16 !\n");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	*buff = (var & u16_half_sec) >> 8; // 2
	*(buff + 1) = var & u16_half_first; // 1
#else
	*buff = var & u16_half_first; // 1
	*(buff + 1) = (var & u16_half_sec) >> 8; // 2
#endif
}

void serialize_u32(u32 var, i8* buff, size_t size) {
	if (size < 4)
		LOG_ERR("could not serialize u32 !\n");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	serialize_u16((u16)((var & u32_half_sec) >> 16), buff, size);
	serialize_u16((u16)(var & u32_half_first), buff + 2, size - 2);
#else
	serialize_u16((u16)(n_var & u32_half_first), buff, size);
	serialize_u16((u16)((n_var & u32_half_sec) >> 16), buff + 2, size - 2);
#endif
}

void serialize_u64(u64 var, i8* buff, size_t size) {
	if (size < 8)
		LOG_ERR("could not serialize u64 !\n");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	serialize_u32((u32)((var & u64_half_sec) >> 32), buff, size);
	serialize_u32((u32)(var & u64_half_first), buff + 2, size - 2);
#else
	serialize_u32((u32)(var & u64_half_first), buff, size);
	serialize_u32((u32)(var & u32_half_sec) >> 32), buff + 2, size - 2);
#endif 
}

#endif // NEYSER_H
