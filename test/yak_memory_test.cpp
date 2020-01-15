#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_memory.h>

int
main()
{
    test_cat("Platform memory allocation");

    int Size = 4096 - sizeof(memory); // ye standard page
    memory* MainMemory = mem_init(Size);
    for (size_t i = 0; i < MainMemory->Size; i++)
    {
        MainMemory->Vault[i] = 'a';
    }
    test(sizeof(memory) == 24, "Memory struct is 24 bytes long");
    test((MainMemory->Size == Size), "Available size equal to requested amount");
    test((MainMemory->Vault[0] == 'a'), "Can write to the beginning of requested memory");
    test((MainMemory->Vault[Size - 1] == 'a'), "Can write to the end of requested memory");
    // test((MainMemory->Vault[Size] = 'F'), "Crash here");

    test_cat("Getting size from the bank");

    MainMemory->Used = 0; // IMPORTANT: Do not do it in real life!
    u32* TestNumber = (u32*)YakMem_GetSize(MainMemory, sizeof(u32));
    u32 ExpectedSize = sizeof(u32);

    test((u8*)TestNumber == MainMemory->Vault, "Base address should be returned upon first allocation");
    test(MainMemory->Used == ExpectedSize, "Memory should be deducted from the total");
    test(*TestNumber == 0, "Memory should be cleared by default");

    TestNumber = (u32*)YakMem_GetSize(MainMemory, sizeof(u32));
    ExpectedSize += sizeof(u32);
    test((u8*)TestNumber == MainMemory->Vault + sizeof(u32), "Correct address should be returned upon next allocation");
    test(MainMemory->Used == ExpectedSize, "Memory should be deducted from the total");

    char* TestChar = (char*)YakMem_GetSize(MainMemory, sizeof(char));
    ExpectedSize += 4;
    test((u64)&TestChar % 4 == 0, "Memory is always at least 4 bytes aligned");

    u64* TestU = (u64*)YakMem_GetSize(MainMemory, sizeof(u64), 8);
    ExpectedSize += 8;
    test((u64)&TestChar % 8 == 0, "Memory is 8 bytes aligned");

    test_cat("Getting space for Struct storage");
    struct test_struct
    {
        u32 Kung;
        u8 Fu;
    };
    test_struct* Test = YakMem_GetStruct(MainMemory, test_struct);
    test(Test->Kung == 0, "By default memory is zeroed out");

    ExpectedSize += 8; // u32 + u8 + 3 bytes alignment
    test(MainMemory->Used == ExpectedSize, "Struct memory is aligned");

    YakMem_Reset(MainMemory);
    test(MainMemory->Used == 0, "Memory has reset used memory to 0");

    MainMemory->Vault[0] = 'a';
    char* A = (char*)YakMem_GetSize(MainMemory, sizeof(char), 4, MemoryFlag_NoClear);
    test(*A == 'a', "No clear flags work properly");

    test_output();
    return (0);
}