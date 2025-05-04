#include "neyser.h"
#include <string.h>
#include <stdint.h>

/* Check if __BYTE_ORDER__ is defined, otherwise define manually */
#ifndef __BYTE_ORDER__
#if defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) ||          \
    defined(__MIPSEL__) || defined(__i386) || defined(__i386__) ||              \
    defined(__x86_64) || defined(__x86_64__) || defined(__amd64) ||             \
    defined(_M_IX86) || defined(_M_X64)
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) ||         \
    defined(__MIPSEB__)
#define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
#else
#error "Cannot determine endianness"
#endif
#endif

#ifndef __ORDER_LITTLE_ENDIAN__
#define __ORDER_LITTLE_ENDIAN__ 1234
#endif

#ifndef __ORDER_BIG_ENDIAN__
#define __ORDER_BIG_ENDIAN__ 4321
#endif

/* ============================ SERIALIZATION FUNCTIONS ============================ */

serialization_result_t serialize_u8(u8 value, uint8_t *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 1)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

    buffer[0] = value;

    return SERIALIZATION_OK;
}

serialization_result_t serialize_u16(u16 value, u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 2)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    buffer[0] = (uint8_t)(value & 0xFF);
    buffer[1] = (uint8_t)((value >> 8) & 0xFF);
#else
    buffer[0] = (uint8_t)((value >> 8) & 0xFF);
    buffer[1] = (uint8_t)(value & 0xFF);
#endif

    return SERIALIZATION_OK;
}

serialization_result_t serialize_u32(u32 value, u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 4)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    buffer[0] = (uint8_t)(value & 0xFF);
    buffer[1] = (uint8_t)((value >> 8) & 0xFF);
    buffer[2] = (uint8_t)((value >> 16) & 0xFF);
    buffer[3] = (uint8_t)((value >> 24) & 0xFF);
#else
    buffer[0] = (uint8_t)((value >> 24) & 0xFF);
    buffer[1] = (uint8_t)((value >> 16) & 0xFF);
    buffer[2] = (uint8_t)((value >> 8) & 0xFF);
    buffer[3] = (uint8_t)(value & 0xFF);
#endif

    return SERIALIZATION_OK;
}

serialization_result_t serialize_u64(u64 value, u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 8)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    buffer[0] = (uint8_t)(value & 0xFF);
    buffer[1] = (uint8_t)((value >> 8) & 0xFF);
    buffer[2] = (uint8_t)((value >> 16) & 0xFF);
    buffer[3] = (uint8_t)((value >> 24) & 0xFF);
    buffer[4] = (uint8_t)((value >> 32) & 0xFF);
    buffer[5] = (uint8_t)((value >> 40) & 0xFF);
    buffer[6] = (uint8_t)((value >> 48) & 0xFF);
    buffer[7] = (uint8_t)((value >> 56) & 0xFF);
#else
    buffer[0] = (uint8_t)((value >> 56) & 0xFF);
    buffer[1] = (uint8_t)((value >> 48) & 0xFF);
    buffer[2] = (uint8_t)((value >> 40) & 0xFF);
    buffer[3] = (uint8_t)((value >> 32) & 0xFF);
    buffer[4] = (uint8_t)((value >> 24) & 0xFF);
    buffer[5] = (uint8_t)((value >> 16) & 0xFF);
    buffer[6] = (uint8_t)((value >> 8) & 0xFF);
    buffer[7] = (uint8_t)(value & 0xFF);
#endif

    return SERIALIZATION_OK;
}

serialization_result_t serialize_i8(i8 value, u8 *buffer, size_t buffer_size)
{
    /*
     * For serializing signed integers, we use the same bit pattern as the
     * equivalent unsigned value. Two's complement representation ensures
     * negative numbers are correctly preserved.
     */
    uint8_t *value_ptr = (uint8_t *)&value;
    return serialize_u8(*value_ptr, buffer, buffer_size);
}

serialization_result_t serialize_i16(i16 value, u8 *buffer, size_t buffer_size)
{
    uint16_t unsigned_value;
    memcpy(&unsigned_value, &value, sizeof(value));
    return serialize_u16(unsigned_value, buffer, buffer_size);
}

serialization_result_t serialize_i32(i32 value, u8 *buffer, size_t buffer_size)
{
    /* Use memcpy to maintain bit representation when converting between signed/unsigned */
    uint32_t unsigned_value;
    memcpy(&unsigned_value, &value, sizeof(value));
    return serialize_u32(unsigned_value, buffer, buffer_size);
}

serialization_result_t serialize_i64(i64 value, u8 *buffer, size_t buffer_size)
{
    /* Use memcpy to maintain bit representation when converting between signed/unsigned */
    uint64_t unsigned_value;
    memcpy(&unsigned_value, &value, sizeof(value));
    return serialize_u64(unsigned_value, buffer, buffer_size);
}

serialization_result_t serialize_float(float value, u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 4)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return serialize_u32(bits, buffer, buffer_size);
}

serialization_result_t serialize_double(double value, u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL)
    {
        return SERIALIZATION_ERROR_NULL_POINTER;
    }

    if (buffer_size < 8)
    {
        return SERIALIZATION_ERROR_BUFFER_TOO_SMALL;
    }

    uint64_t bits;
    memcpy(&bits, &value, sizeof(bits));
    return serialize_u64(bits, buffer, buffer_size);
}

/* ============================ DESERIALIZATION FUNCTIONS ============================ */

uint8_t deserialize_u8(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 1)
    {
        return 0;
    }

    return buffer[0];
}

uint16_t deserialize_u16(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 2)
    {
        return 0;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);
#else
    return ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
#endif
}

uint32_t deserialize_u32(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 4)
    {
        return 0;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (uint32_t)buffer[0] |
           ((uint32_t)buffer[1] << 8) |
           ((uint32_t)buffer[2] << 16) |
           ((uint32_t)buffer[3] << 24);
#else
    return ((uint32_t)buffer[0] << 24) |
           ((uint32_t)buffer[1] << 16) |
           ((uint32_t)buffer[2] << 8) |
           (uint32_t)buffer[3];
#endif
}

uint64_t deserialize_u64(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 8)
    {
        return 0;
    }

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return (uint64_t)buffer[0] |
           ((uint64_t)buffer[1] << 8) |
           ((uint64_t)buffer[2] << 16) |
           ((uint64_t)buffer[3] << 24) |
           ((uint64_t)buffer[4] << 32) |
           ((uint64_t)buffer[5] << 40) |
           ((uint64_t)buffer[6] << 48) |
           ((uint64_t)buffer[7] << 56);
#else
    return ((uint64_t)buffer[0] << 56) |
           ((uint64_t)buffer[1] << 48) |
           ((uint64_t)buffer[2] << 40) |
           ((uint64_t)buffer[3] << 32) |
           ((uint64_t)buffer[4] << 24) |
           ((uint64_t)buffer[5] << 16) |
           ((uint64_t)buffer[6] << 8) |
           (uint64_t)buffer[7];
#endif
}

int8_t deserialize_i8(u8 *buffer, size_t buffer_size)
{
    uint8_t unsigned_value = deserialize_u8(buffer, buffer_size);
    int8_t signed_value;
    memcpy(&signed_value, &unsigned_value, sizeof(unsigned_value));
    return signed_value;
}

int16_t deserialize_i16(u8 *buffer, size_t buffer_size)
{
    uint16_t unsigned_value = deserialize_u16(buffer, buffer_size);
    int16_t signed_value;
    memcpy(&signed_value, &unsigned_value, sizeof(unsigned_value));
    return signed_value;
}

int32_t deserialize_i32(u8 *buffer, size_t buffer_size)
{
    uint32_t unsigned_value = deserialize_u32(buffer, buffer_size);
    int32_t signed_value;
    memcpy(&signed_value, &unsigned_value, sizeof(unsigned_value));
    return signed_value;
}

int64_t deserialize_i64(u8 *buffer, size_t buffer_size)
{
    uint64_t unsigned_value = deserialize_u64(buffer, buffer_size);
    int64_t signed_value;
    memcpy(&signed_value, &unsigned_value, sizeof(unsigned_value));
    return signed_value;
}

float deserialize_float(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 4)
    {
        return 0.0f;
    }

    uint32_t bits = deserialize_u32(buffer, buffer_size);
    float value;
    memcpy(&value, &bits, sizeof(bits));
    return value;
}

double deserialize_double(u8 *buffer, size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 8)
    {
        return 0.0;
    }

    uint64_t bits = deserialize_u64(buffer, buffer_size);
    double value;
    memcpy(&value, &bits, sizeof(bits));
    return value;
}