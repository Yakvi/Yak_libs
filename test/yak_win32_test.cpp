#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_win32.h>

int
main(void)
{
    f32 ClockFreq = Yak__GetClockFrequency();

    LPSTR Prompt1 = "Hello world \n";
    
    HANDLE OutHandle = Yak__ConsoleInit();
    
    LARGE_INTEGER ClockStart = Yak__GetWallClock();
    Yak__Print(Prompt1, lstrlenA(Prompt1), OutHandle);
    f32 SecondsElapsed = Yak__GetSecondsElapsed(ClockStart, ClockFreq);
    
    ClockStart = Yak__GetWallClock();
    printf(Prompt1);
    SecondsElapsed = Yak__GetSecondsElapsed(ClockStart, ClockFreq);

    Yak__Print(Prompt1, lstrlenA(Prompt1), OutHandle, ConsoleColor_Yellow);

    test_output();
    return (0);
}