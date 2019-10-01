#if !defined(YAK_PLATFORM)

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define read_file YakPlatform_ReadFile
#define get_clock YakPlatform_GetClock
#define measure_time YakPlatform_MeasureTime
#define print_c YakPlatform_OutputConsole
#endif // YAK_ENABLE_SHORTHAND_MACROS

// PLATFORM API
struct file;
struct platform;
struct clock;

static platform YakPlatform_Init(void);

static void* YakPlatform_GetMemory(size_t);
static void YakPlatform_FreeMemory(void*);
static file YakPlatform_ReadFile(char*);

inline void YakPlatform_OutputConsole(char*, unsigned, platform*, unsigned short);
inline clock YakPlatform_GetClock(void);
inline float YakPlatform_MeasureTime(clock, platform*);

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif