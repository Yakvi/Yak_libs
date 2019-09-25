#if !defined(YAK_MEMORY)

#include <yak_defines.h>
#include <yak_platform.h>
EXTERN_C_START

struct memory
{
    memory_index Size;
    u8* Base;
    memory_index Used;
};

memory*
Yak_AllocatePlatformMemory(size_t Size)
{
    memory* Result = (memory*)YakPlatform_GetMemory(Size + sizeof(memory));
    if (Result)
    {
        Result->Size = Size; // Can write from 0 to Size - 1
        Result->Base = (u8*)Result + sizeof(memory);
        Result->Used = 0;
    }
    return (Result);
}


/** TODO: 
 * ClearMemory
 * AlignMemory
 * GetMemory
 * QuickGetMemory (no clear)
 * ReturnMemory
 **/

//
// NOTE: Memory Utilities
//
#define AlignPow2(Value, Alignment) \
    ((Value + ((Alignment)-1)) & ~((Alignment)-1))
#define Align4(Value) (((Value) + 3) & ~3)
#define Align8(Value) (((Value) + 7) & ~7)
#define Align16(Value) (((Value) + 15) & ~15)

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

EXTERN_C_END
#define YAK_MEMORY
#endif