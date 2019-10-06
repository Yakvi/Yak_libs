#if !defined(YAK_DEFINES)

#ifdef __cplusplus
#define EXTERN_C_START \
    extern "C"         \
    {
#define EXTERN_C_END }
#else
#define EXTERN_C_START
#define EXTERN_C_END
#endif

EXTERN_C_START
//
// NOTE: Basic assertions
//
#ifdef DEBUG
#define Assert(Expression) \
    if (!(Expression)) { *(int*)0 = 0; } // Point to sector 0, immediately crash
#else
#define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase \
    default:               \
    {                      \
        InvalidCodePath;   \
    }                      \
    break

//
// NOTE: Compilers
//
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif // !COMPILER_MSVC

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif // !COMPILER_LLVM

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
// TODO: MORE compilers!
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

//
// NOTE: Types
//
#if !defined(internal)
#define internal static
#endif
#define local_persist static
#define global_variable static

// #include <stddef.h>
// #include <limits.h>
// #include <float.h>

#ifdef COMPILER_MSVC
typedef __int8 s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;
typedef s32 b32;
typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef size_t memory_index;
typedef float f32;
typedef double f64;
#else
// TODO: LLVM and other compilers
#error something
#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uintptr;
typedef size_t memory_index;
typedef float f32;
typedef double f64;
#endif // COMPILER_MSVC

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

//
// NOTE: Array Utilities
//
#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f

#define ArrayWithSize(Array) Array, sizeof(Array)
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define ArrayWithCount(Input) Input, ArrayCount(Input)
#define InArray(Count, Array) ((Count) < ArrayCount(Array))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
// TODO: swap, min, max ... macros?
// #if COMPILER_MSVC
// #include <intrin.h>
// #elif COMPILER_LLVM
// #include <x86intrin.h>
// #else
// #error SEE/NEON optimizations are not available for this compiler yet!!!!
// #endif

EXTERN_C_END

#define YAK_DEFINES 1
#endif