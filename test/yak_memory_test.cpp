#include <yak_defines.h>
#include <yak_test.h>
#include <yak_memory.h>
int
main()
{
    YakTest_Category("Platform memory allocation");

    int Size = 4096 - sizeof(memory); // ye standard page
    memory* MainMemory = Yak_AllocatePlatformMemory(Size);
    YakTest_Assert(sizeof(memory) == 24, "Memory struct is 24 bytes long");
    YakTest_Assert((MainMemory->Size == Size), "Available size equal to requested amount");
    YakTest_Assert((MainMemory->Base[0] = 'a'), "Can write to the beginning of requested memory");
    YakTest_Assert((MainMemory->Base[Size - 1] = 'z'), "Can write to the end of requested memory");
    // YakTest_Assert((MainMemory->Base[Size] = 'F'), "Crash here");

    YakTest_Category("Getting size from the bank");

    MainMemory->Size = 0; // Do not do it in real life!
    u32* TestNumber = (u32*)YakMem_GetSize(MainMemory, sizeof(u32));
    YakTest_Assert((u8*)TestNumber == MainMemory->Base, "Base address should be returned upon first allocation");
    YakTest_Assert(MainMemory->Used == sizeof(u32), "Memory should be deducted from the total");
    TestNumber = (u32*)YakMem_GetSize(MainMemory, sizeof(u32));
    YakTest_Assert((u8*)TestNumber == MainMemory->Base + sizeof(u32), "Correct address should be returned upon next allocation");
    YakTest_Assert(MainMemory->Used == 2 * sizeof(u32), "Memory should be deducted from the total");

    // struct test_struct{
    //     u32 Value;
    // };
    // test_struct Test = Yak_PushStruct(test_struct);
    // YakTest_Assert(Test.Value == 0, "By default memory is zeroed out");

    YakTest_ShowResults();
    return (0);
}