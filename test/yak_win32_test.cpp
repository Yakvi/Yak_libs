#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_win32.h>

int
main(void)
{
    platform PlatformData = YakPlatform_Init();

    LPSTR Prompt1 = "Hello world \n";
    
    clock ClockStart = YakPlatform_GetTime();
    YakPlatform_OutputConsole(Prompt1, lstrlenA(Prompt1), PlatformData);
    f32 SecondsElapsed = YakPlatform_StopTimer(ClockStart, PlatformData);
    
    ClockStart = YakPlatform_GetTime();
    printf(Prompt1);
    SecondsElapsed = YakPlatform_StopTimer(ClockStart, PlatformData);

    YakPlatform_OutputConsole("Red ", lstrlenA("Red "), PlatformData, ConsoleColor_Red);
    YakPlatform_OutputConsole("Green ", lstrlenA("Green "), PlatformData, ConsoleColor_Green);
    YakPlatform_OutputConsole("Blue ", lstrlenA("Blue "), PlatformData, ConsoleColor_Blue);
    YakPlatform_OutputConsole("Yellow ", lstrlenA("Yellow "), PlatformData, ConsoleColor_Yellow);
    YakPlatform_OutputConsole("Cyan ", lstrlenA("Cyan "), PlatformData, ConsoleColor_Cyan);
    YakPlatform_OutputConsole("Magenta ", lstrlenA("Magenta "), PlatformData, ConsoleColor_Magenta);
    YakPlatform_OutputConsole("White\n", lstrlenA("White\n"), PlatformData, ConsoleColor_White);

    test(false, "This fails");
    test_output();
    return (0);
}