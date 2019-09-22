#if !defined(YAK_DEFINES)
// clang-format off

#ifdef __cplusplus
extern "C"
{
#endif
//
// NOTE: Assertions
//
#ifdef DEBUG
#define Assert(Expression) if (!(Expression)) { *(int *)0 = 0; } // Point to sector 0, immediately crash
#else
#define Assert(Expression)
#endif

#define InvalidCodePath Assert(!"InvalidCodePath")
#define InvalidDefaultCase default: { InvalidCodePath; } break

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

// #if COMPILER_MSVC
// #include <intrin.h>
// #elif COMPILER_LLVM
// #include <x86intrin.h>
// #else
// #error SEE/NEON optimizations are not available for this compiler yet!!!!
// #endif

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
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define InArray(Count, Array) ((Count) < ArrayCount(Array))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
// TODO: swap, min, max ... macros?

//
// NOTE: Memory Utilities
// 
#define AlignPow2(Value, Alignment) \
    ((Value + ((Alignment)-1)) & ~((Alignment)-1))
#define Align4(Value) (((Value) + 3) & ~3)
#define Align8(Value) (((Value) + 7) & ~7)
#define Align16(Value) (((Value) + 15) & ~15)

#define Pi32 3.14159265359f
#define Tau32 6.28318530717958647692f
#define Real32Maximum FLT_MAX
#define UIntMin 0
#define UInt32Max 0xFFFFFFFF
#define UInt16Max 65535
#define Int16Min -32768
#define Int16Max 32767

inline u32
SafeTruncateUInt64(u64 Value)
{
    Assert(Value <= UInt32Max);
    u32 Result = (u32)Value;
    return (Result);
}

inline u16
SafeTruncateToUInt16(u32 Value)
{
    Assert(Value <= UInt16Max);
    Assert(Value >= UIntMin);
    u16 Result = (u16)Value;
    return (Result);
}

inline s16
SafeTruncateToInt16(s32 Value)
{
    Assert(Value <= Int16Max);
    Assert(Value >= Int16Min);
    s16 Result = (s16)Value;
    return (Result);
}

#ifdef __cplusplus
}
#endif

// clang-format on
#define YAK_DEFINES 1
#endif
