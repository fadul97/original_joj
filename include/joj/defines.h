#ifndef JDEFINES_H
#define JDEFINES_H

#include <stdint.h>

#define FALSE 0
#define TRUE 1
// #define TRUE !FALSE

// Unsigned types
typedef uint8_t u8;
typedef	uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed types
typedef int8_t i8;
typedef	int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point types
typedef float f32;
typedef double f64;

// Boolean types
typedef char b8;
typedef int b32;

// STATIC_ASSERT macro
#if defined(__GNUC__)
#define STATIC_ASSERT static_assert
#elif defined(_MSC_VER)
#define STATIC_ASSERT static_assert
#else
#define STATIC_ASSERT _Static_assert
#endif

// Ensure all types are of the correct size.

/** @brief Assert u8 to be 1 byte.*/
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

/** @brief Assert u16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");

/** @brief Assert u32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");

/** @brief Assert u64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

/** @brief Assert i8 to be 1 byte.*/
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

/** @brief Assert i16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

/** @brief Assert i32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");

/** @brief Assert i64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

/** @brief Assert f32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

/** @brief Assert f64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

/** @brief Assert b8 to be 1 bytes.*/
STATIC_ASSERT(sizeof(b8) == 1, "Expected b8 to be 1 byte.");

/** @brief Assert b32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(b32) == 4, "Expected f64 to be 4 bytes.");

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define JPLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define JPLATFORM_LINUX 1
#elif defined(__unix__)
// Catch anything not caught by the above.
#define JPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define JPLATFORM_POSIX 1
#else
#error "Unknown platform!"
#endif

// Inline functions
#if defined(__linux__) || defined(__gnu_linux__)
#define FINLINE static inline
#elif defined(_MSC_VER)
#define FINLINE __forceinline
#endif

// Dynamic libraries
#if defined(_MSC_VER)
#ifdef JOJ_ENGINE_IMPLEMENTATION
#define JAPI __declspec(dllexport)
#else
#define JAPI __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define JAPI __attribute__((visibility("default")))
#else
#define JAPI
#endif

#endif // JDEFINES_H