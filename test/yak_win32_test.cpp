#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_win32.h>

int
main(void)
{
    platform PlatformData = YakPlatform_Init();

    char* Prompt1 = "Color tests \n";
    
    clock ClockStart = YakPlatform_GetClock();
    YakPlatform_OutputConsole(Prompt1, 14, PlatformData);
    f32 SecondsElapsed = YakPlatform_MeasureTime(ClockStart, PlatformData);
    
    ClockStart = YakPlatform_GetClock();
    printf(Prompt1);
    SecondsElapsed = YakPlatform_MeasureTime(ClockStart, PlatformData);

    YakPlatform_OutputConsole("Red ", 4, PlatformData, ConsoleColor_Red);
    YakPlatform_OutputConsole("Green ", 6, PlatformData, ConsoleColor_Green);
    YakPlatform_OutputConsole("Blue ", 5, PlatformData, ConsoleColor_Blue);
    YakPlatform_OutputConsole("Yellow ", 7, PlatformData, ConsoleColor_Yellow);
    YakPlatform_OutputConsole("Cyan ", 5, PlatformData, ConsoleColor_Cyan);
    YakPlatform_OutputConsole("Magenta ", 8, PlatformData, ConsoleColor_Magenta);
    YakPlatform_OutputConsole("White\n", 6, PlatformData, ConsoleColor_White);

    test(true, "This succeeds");
    test(false, "This fails");
    test_output();
    return (0);
}