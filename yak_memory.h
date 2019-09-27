#if !defined(YAK_MEMORY)

#include <yak_defines.h>
#include <yak_platform.h>
EXTERN_C_START

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define mem_reserve Yak_AllocatePlatformMemory
#define mem_init YakMem_Init
#define mem_size YakMem_GetSize
#define mem_struct YakMem_GetStruct
#define mem_reset YakMem_Reset
#endif // YAK_ENABLE_SHORTHAND_MACROS

// Public API
struct memory;
internal memory* Yak_AllocatePlatformMemory(memory_index Size); // Call this as few times as possible, ideally only to define the main memory banks at the very beginning
#define YakMem_Init(Size) Yak_AllocatePlatformMemory(Size)
#define YakMem_GetSize(MemoryBank, Size, ...) Yak__PushMemory(Size, MemoryBank, ##__VA_ARGS__) // Memory is always 4 byte aligned
#define YakMem_GetStruct(MemoryBank, Storage, ...) (Storage*)YakMem_GetSize(MemoryBank, sizeof(Storage), ##__VA_ARGS__)
#define YakMem_Reset(MemoryBank, ...) Yak__ResetMemory(MemoryBank, ##__VA_ARGS__)

struct memory
{
    memory_index Size;
    u8* Vault;
    memory_index Used;
};

enum memory_flags
{
    MemoryFlag_Clear = 0b1,
    MemoryFlag_NoClear = 0b10,
};

inline void
Yak__ClearMemory(void* Memory, memory_index Size)
{
    // TODO: since we always align by 4 by default, can clear by u32s?
    for (memory_index Index = 0; Index < Size; ++Index)
    {
        ((u8*)Memory)[Index] = 0;
    }
}

inline void
Yak__ResetMemory(memory* Memory, memory_index Size = 0)
{
    Assert(Memory);
    if (Size)
    {
        Memory->Size = Size; // Can write from 0 to Size - 1
    }
    Memory->Used = 0;
}

inline memory_index
Yak__GetEffectiveSize(memory* Memory, memory_index DesiredSize, u32 Alignment)
{
    memory_index EffectiveSize = DesiredSize;

    Assert(Memory->Size >= Memory->Used + DesiredSize);

    memory_index CurrentBlock = (memory_index)Memory->Vault + Memory->Used + DesiredSize;
    memory_index AlignmentMask = Alignment - 1;

    memory_index Offset = 0;
    if (CurrentBlock & AlignmentMask)
    {
        Offset = Alignment - (CurrentBlock & AlignmentMask);
    }

    EffectiveSize += Offset;

    return (EffectiveSize);
}

inline void*
Yak__PushMemory(memory_index SizeInit, memory* Memory, u32 Flags = MemoryFlag_Clear, u32 Alignment = 4)
{
    void* Result = Memory->Vault + Memory->Used;

    memory_index Size = Yak__GetEffectiveSize(Memory, SizeInit, Alignment);

    Assert(Memory->Size >= Memory->Used + Size);
    Memory->Used += Size;
    Assert(Memory->Size >= Memory->Used);

    if (Flags & MemoryFlag_Clear)
    {
        Yak__ClearMemory(Result, Size);
    }

    return Result;
}

//
// Platform interaction
//

internal memory*
Yak_AllocatePlatformMemory(memory_index Size)
{
    memory* Result = (memory*)YakPlatform_GetMemory(Size + sizeof(memory));
    if (Result)
    {
        Result->Vault = (u8*)Result + sizeof(memory);
        YakMem_Reset(Result, Size);
    }

    return (Result);
}
/** TODO: 
 * FreePlatformMemory
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