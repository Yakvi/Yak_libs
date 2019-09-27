#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_defines.h>
#include <yak_test.h>
#include <yak_win32.h>

int
main(void)
{
    LPSTR lpszPrompt1 = "Hello world \n";
    
    HANDLE OutHandle = Yak__ConsoleInit();
    Yak__Print(lpszPrompt1, lstrlenA(lpszPrompt1), OutHandle);
    Yak__Print(lpszPrompt1, lstrlenA(lpszPrompt1), OutHandle, ConsoleColor_Yellow);

    test_output();
    return (0);
}