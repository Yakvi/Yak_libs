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


    YakTest_Assert(Test.Value == 0, "By default memory is zeroed out");

    YakTest_ShowResults();
    return (0);
}