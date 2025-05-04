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
 
 #define ANSI_COLOR_RESET "\x1b[0m"
 #define ANSI_COLOR_RED "\x1b[31m"
 #define ANSI_COLOR_GREEN "\x1b[32m"
 #define ANSI_COLOR_YELLOW "\x1b[33m"
 
 #define LOG_OKAY(msg, ...) \
   printf(ANSI_COLOR_GREEN "[✓] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)
 
 #define LOG_ERR(msg, ...)                                                     \
   do                                                                          \
   {                                                                           \
     printf(ANSI_COLOR_RED "[x] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__); \
     exit(1);                                                                  \
   } while (0)
 
 #define LOG_WARN(msg, ...) \
   printf(ANSI_COLOR_YELLOW "[!] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)
 
 #define u16_half_first 0x000000ff
 #define u16_half_sec 0x0000ff00
 #define u32_half_first 0x0000ffff
 #define u32_half_sec 0xffff0000
 #define u32_half_first 0x0000ffff
 #define u32_half_sec 0xffff0000
 #define u64_half_first 0xffffffff
 
 #define u64_half_sec (UINT64_C(0xffffffff) << 32)
 
 #ifdef __cplusplus
 extern "C"
 {
 #endif
 
  typedef enum {
    SERIALIZATION_OK = 0,
    SERIALIZATION_ERROR_BUFFER_TOO_SMALL = -1,
    SERIALIZATION_ERROR_NULL_POINTER = -2
  } serialization_result_t;

   typedef int16_t i8;
   typedef int16_t i16;
   typedef int32_t i32;
   typedef int64_t i64;
 
   typedef uint16_t u8;
   typedef uint16_t u16;
   typedef uint32_t u32;
   typedef uint64_t u64;
 
   void serialize_i16(i16 var, i8 *buff, size_t size);
   void serialize_i32(i32 var, i8 *buff, size_t size);
   void serialize_i64(i64 var, i8 *buff, size_t size);
   void serialize_u16(u16 var, i8 *buff, size_t size);
   void serialize_u32(u32 var, i8 *buff, size_t size);
   void serialize_u64(u64 var, i8 *buff, size_t size);
   void serialize_float(float var, i8 *buff, size_t size);
   void serialize_double(double var, i8* buff, size_t size);
 
   i16 deserialize_i16(i8 *buff, size_t size);
   i32 deserialize_i32(i8 *buff, size_t size);
   i64 deserialize_i64(i8 *buff, size_t size);
   u16 deserialize_u16(i8 *buff, size_t size);
   u32 deserialize_u32(i8 *buff, size_t size);
   u64 deserialize_u64(i8 *buff, size_t size);
   float deserialize_float(i8 *buff, size_t size);
   double deserialize_double(i8 *buff, size_t size);
 
   struct _pack_t {
     u32 mant;
     u16 exp;
     u8 sign;
   };
 
   struct _d_pack_t {
     u64 mant;
     u32 exp;
     u8 sign;
   };
   
   typedef struct _pack_t f_pack_t;
   typedef struct _d_pack_t d_pack_t;
 
 #ifdef __cplusplus
 }
 #if defined(__cpp_concepts) && __cpp_concepts >= 201907L
 
 #include <concepts>
 
   template <typename T>
   concept Deserializable =
       std::same_as<T, i16> || std::same_as<T, i32> ||
       std::same_as<T, i64> || std::same_as<T, u16> ||
       std::same_as<T, u32> || std::same_as<T, u64> ||
       std::same_as<T, float> || std::same_as<T, double>;
 
   template <typename T>
   concept Serializable = Deserializable<T>;
 
   template <typename T> requires Deserializable<T>
   T deserialize(i8 * buff, size_t buff_size)
   {
     using CleanType = std::remove_cvref_t<T>;
 
     if constexpr (std::same_as<CleanType, float>) return deserialize_float(buff, buff_size);
     if constexpr (std::same_as<CleanType, double>) return deserialize_double(buff, buff_size);
     if constexpr (std::same_as<CleanType, i16>) return deserialize_i16(buff, buff_size);
     if constexpr (std::same_as<CleanType, i32>) return deserialize_i32(buff, buff_size);
     if constexpr (std::same_as<CleanType, i64>) return deserialize_i64(buff, buff_size);
     if constexpr (std::same_as<CleanType, u16>) return deserialize_u16(buff, buff_size);
     if constexpr (std::same_as<CleanType, u32>) return deserialize_u32(buff, buff_size);
     if constexpr (std::same_as<CleanType, u64>) return deserialize_u64(buff, buff_size);
   }
 
   template <typename T> requires Serializable<T>
   void serialize(T var, i8 * buff, size_t buff_size)
   {
     using CleanType = std::remove_cvref_t<T>;
     if constexpr (std::same_as<CleanType, float>) { serialize_float(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, double>) { serialize_double(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, i16>) { serialize_i16(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, i32>) { serialize_i32(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, i64>) { serialize_i64(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, u16>) { serialize_u16(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, u32>) { serialize_u32(var, buff, buff_size); return; }
     if constexpr (std::same_as<CleanType, u64>) { serialize_u64(var, buff, buff_size); return; }
   }
 
 #else
   template <typename T>
   T deserialize_helper(i8 *buff, size_t size);
   template <>
   i8 deserialize_helper<i8>(i8 *buff, size_t size) { return deserialize_u16((i8 *)buff, size); }
   template <>
   i16 deserialize_helper<i16>(i8 *buff, size_t size) { return deserialize_i16((i8 *)buff, size); }
   template <>
   i32 deserialize_helper<i32>(i8 *buff, size_t size) { return deserialize_i32((i8 *)buff, size); }
   template <>
   i64 deserialize_helper<i64>(i8 *buff, size_t size) { return deserialize_i64((i8 *)buff, size); }
   template <>
   u8 deserialize_helper<u8>(i8 *buff, size_t size) { return deserialize_u16((i8 *)buff, size); }
   template <>
   u16 deserialize_helper<u16>(i8 *buff, size_t size) { return deserialize_u16((i8 *)buff, size); }
   template <>
   u32 deserialize_helper<u32>(i8 *buff, size_t size) { return deserialize_u32((i8 *)buff, size); }
   template <>
   u64 deserialize_helper<u64>(i8 *buff, size_t size) { return deserialize_u64((i8 *)buff, size); }
   template <>
   float deserialize_helper<float>(i8 *buff, size_t size) { return deserialize_float((i8 *)buff, size); }
   template <>
   float deserialize_helper<float>(i8 *buff, size_t size) { return deserialize_float((i8 *)buff, size); }
   template <>
   double deserialize_helper<float>(i8 *buff, size_t size) { return deserialize_double((i8 *)buff, size); }
 
 
   template <typename T>
   void serialize_helper(T var, i8 *buff, size_t size);
   template <>
   void serialize_helper<i8>(i8 var, i8 *buff, size_t size) { return serialize_u16(i8 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<i16>(i16 var, i8 *buff, size_t size) { return serialize_i16(i16 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<i32>(i32 var, i8 *buff, size_t size) { return serialize_i32(i32 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<i64>(i64 var, i8 *buff, size_t size) { return serialize_i64(i64 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<u8>(u8 var, i8 *buff, size_t size) { return serialize_u16(u8 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<u16>(u16 var, i8 *buff, size_t size) { return serialize_u16(u16 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<u32>(u32 var, i8 *buff, size_t size) { return serialize_u32(u32 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<u64>(u64 var, i8 *buff, size_t size) { return serialize_u64(u64 var, (i8 *)buff, size); }
   template <>
   void serialize_helper<float>(float var, i8 *buff, size_t size) { return serialize_float(float var, (i8 *)buff, size); }
   template <>
   void serialize_helper<float>(double var, i8 *buff, size_t size) { return serialize_double(double var, (i8 *)buff, size); }
 
   template <typename T>
   void serialize(T var, i8 *buff, size_t size)
   {
     serialize_helper<T>(var, buff, size);
   }
 #endif
 #endif
 
 #endif // NEYSER_H