#if !defined(YAK_PLATFORM)

// Defined platform calls
struct file;

void* YakPlatform_GetMemory(size_t Size);
void YakPlatform_FreeMemory(void* Memory);
file YakPlatform_ReadFile(char* Filename);

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif