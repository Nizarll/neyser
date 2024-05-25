#ifndef NEYSER_H
#define NEYSER_H

#include "ansi.h"
#include <stdint.h>
#include <stdio.h>

#ifdef _MSC_VER
  #include <stdlib.h>
  #define bswap_16(x) _byteswap_ushort(x)
  #define bswap_32(x) _byteswap_ulong(x)
  #define bswap_64(x) _byteswap_uint64(x)
#else
  #include <byteswap.h>  // bswap_16 bswap_32 bswap_64
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
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

void serialize_u16(u16 var, i8* buff, size_t size);
void serialize_u32(u32 var, i8* buff, size_t size);
void serialize_u64(u64 var, i8* buff, size_t size);
void serialize_struct(void* data, size_t data_size, i8* buff, size_t buff_size);
// float
u16 deserialize_u16(i8* buff, size_t size);
u32 deserialize_u32(i8* buff, size_t size);
u64 deserialize_u64(i8* buff, size_t size);

u64 htonu64(uint64_t val)
{
    union{
        uint64_t retval;
        uint8_t bytes[8];
    };
    bytes[0] = (val & 0x00000000000000ff);
    bytes[1] = (val & 0x000000000000ff00) >> 8;
    bytes[2] = (val & 0x0000000000ff0000) >> 16;
    bytes[3] = (val & 0x00000000ff000000) >> 24;
    bytes[4] = (val & 0x000000ff00000000) >> 32;
    bytes[5] = (val & 0x0000ff0000000000) >> 40;
    bytes[6] = (val & 0x00ff000000000000) >> 48;
    bytes[7] = (val & 0xff00000000000000) >> 56;

    return retval;
}

u64 ntohu64(uint64_t val)
{
    union{
        uint64_t inval;
        uint8_t bytes[8];
    };
    inval = val;
    return bytes[0] |
        ((uint64_t)bytes[1]) <<  8 |
        ((uint64_t)bytes[2]) << 16 |
        ((uint64_t)bytes[3]) << 24 |
        ((uint64_t)bytes[4]) << 32 |
        ((uint64_t)bytes[5]) << 40 |
        ((uint64_t)bytes[6]) << 48 |
        ((uint64_t)bytes[7]) << 56;
}

void serialize_u16(u16 var, i8* buff, size_t size) {
	if (size < 2) 
		LOG_ERR("could not serialize u16 !\n");
	u16 n_var = (u16)ntohs(var);
	*buff = n_var & u16_half_first;
	*(buff + 1) = (n_var & u16_half_sec) >> 8;
}

void serialize_u32(u32 var, i8* buff, size_t size) {
	if (size < 4)
		LOG_ERR("could not serialize u32 !\n");
	u32 n_var = (u32)ntohl(var);
	serialize_u16((u16)(n_var & u32_half_first), buff, size);
	serialize_u16((u16)((n_var & u32_half_sec) >> 16), buff + 2, size - 2);
}

void serialize_u64(u64 var, i8* buff, size_t size) {
	u32 n_var_fr = (u32)ntohl((u32)(var & u64_half_first));
	u32 n_var_sec = (u32)ntohl((u32)((var & u64_half_sec) >> 32));
	serialize_u32(n_var_fr, buff, size);
	serialize_u32(n_var_sec, buff + 4, size - 4);
}

#endif // NEYSER_H
