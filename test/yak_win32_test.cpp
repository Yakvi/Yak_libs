#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_win32.h>

int
main(void)
{
    platform PlatformData = YakPlatform_Init();

    LPSTR Prompt1 = "Hello world \n";
    
    
    timer ClockStart = YakPlatform_GetTime();
    YakPlatform_OutputConsole(Prompt1, lstrlenA(Prompt1), PlatformData);
    f32 SecondsElapsed = YakPlatform_StopTimer(ClockStart, PlatformData);
    
    ClockStart = YakPlatform_GetTime();
    printf(Prompt1);
    SecondsElapsed = YakPlatform_StopTimer(ClockStart, PlatformData);

    YakPlatform_OutputConsole(Prompt1, lstrlenA(Prompt1), PlatformData, ConsoleColor_Yellow);

    test_output();
    return (0);
}