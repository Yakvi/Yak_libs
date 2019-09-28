#if !defined(YAK_PLATFORM)

// PLATFORM API
struct file;
struct platform;

static platform* YakPlatform_Init();

static void* YakPlatform_GetMemory(size_t);
static void YakPlatform_FreeMemory(void*);
static file YakPlatform_ReadFile(char*);

inline void YakPlatform_OutputConsole(char*, unsigned, platform*, unsigned short);
inline long long YakPlatform_StartTimer(void);
inline float YakPlatform_EndTimer(long long, platform*);

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif