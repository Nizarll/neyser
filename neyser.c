#include "neyser.h"

u16 deserialize_u16(i8 *buff, size_t size)
{
  if (size < 2) LOG_ERR("error! could not deserialize u16!");
  return (u16)(u16)(*(buff + 1)) | (u16)(*buff << byte);
}

u32 deserialize_u32(i8 *buff, size_t size)
{
  if (size < 4) LOG_ERR("error! could not deserialize u16!");
  return (u32)(deserialize_u16(buff + 2, size - 2) | (((u32)deserialize_u16(buff, size)) << 2 * byte));
}

u64 deserialize_u64(i8 *buff, size_t size)
{
  if (size < 8) LOG_ERR("error! could not deserialize u32!");
  return (u64)(deserialize_u32(buff + 4, size - 4) | (((u32)deserialize_u32(buff, size)) << 4 * byte));
}

float deserialize_float(i8 *buff, size_t size)
{
  if (size < 6) LOG_ERR("error! could not deserialize float!");

  u32 mant = deserialize_u32(buff, size);
  i8 sign = (*(u8 *)(buff + 4)) ? -1 : 1;
  u8 exp = *(u8 *)(buff + 5);            

  return sign * ldexpf((float)mant / (1 << 24), exp);
}

double deserialize_double(i8 *buff, size_t size)
{
  if (size < 8) LOG_ERR("error! could not deserialize double!");
  u64 raw = deserialize_u64(buff, size);
  
  int sign = (raw >> 63) ? -1 : 1;
  int exponent = ((raw >> 52) & 0x7FF) - 1023;
  u64 mantissa = raw & 0xFFFFFFFFFFFFFull;

  double frac = (double)mantissa / (1ULL << 52) + 1.0;
  return sign * ldexp(frac, exponent);
}

void serialize_float(float var, i8 *buff, size_t size)
{
  f_pack_t pack = {.sign = var > .0f ? 0 : 1};
  pack.mant = (u32)(frexpf(var, &pack.exp) * pow(2, 24));
  printf("serialize_float:\n\tmant is %f, exp is %d, sign is %d\n", pack.mant, pack.exp, pack.sign);
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

void serialize_u16(u16 var, i8 *buff, size_t size)
{
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

void serialize_u32(u32 var, i8 *buff, size_t size)
{
  if (size < 4)
    LOG_ERR("could not serialize u32 !");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  serialize_u16((u16)((var & u32_half_sec) >> (2 * byte)), buff, size);
  serialize_u16((u16)(var & u32_half_first), buff + 2, size - 2);
#else
  serialize_u16((u16)(var & u32_half_first), buff, size);
  serialize_u16((u16)((var & u32_half_sec) >> 16), buff + 2, size - 2);
#endif
}

void serialize_u64(u64 var, i8 *buff, size_t size)
{
  if (size < 8)
    LOG_ERR("could not serialize u64 !");
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  serialize_u32((u32)((var & u64_half_sec) >> (4 * byte)), buff, size);
  serialize_u32((u32)(var & u64_half_first), buff + 2, size - 2);
#else
  serialize_u32((u32)(var & u64_half_first), buff, size);
  serialize_u32((u32)((var & u32_half_sec) >> 32), buff + 2, size - 2);
#endif
}
