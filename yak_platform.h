#if !defined(YAK_PLATFORM)

// PLATFORM API
struct file;
struct platform;
struct timer;

static platform YakPlatform_Init(void);

static void* YakPlatform_GetMemory(size_t);
static void YakPlatform_FreeMemory(void*);
static file YakPlatform_ReadFile(char*);

inline void YakPlatform_OutputConsole(char*, unsigned, platform*, unsigned short);
inline timer YakPlatform_GetTime(void);
inline float YakPlatform_StopTimer(timer, platform*);

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif