#if !defined(YAK_PLATFORM)

// Defined platform calls
struct file;
void* Yak_GetMemory(size_t Size);
void Yak_FreeMemory(void* Memory);
file Yak_ReadFile(char* Filename);

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif