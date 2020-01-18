#if !defined(YAK_PLATFORM)

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define read_file YakPlatform_ReadFile
#define get_clock YakPlatform_GetClock
#define measure_time YakPlatform_MeasureTime
#define print_c YakPlatform_OutputConsole
#endif // YAK_ENABLE_SHORTHAND_MACROS

// PLATFORM API
typedef struct file file;
typedef struct platform platform;
typedef struct clock clock;

static platform YakPlatform_Init();

static void* YakPlatform_GetMemory();
static void YakPlatform_FreeMemory();
static file YakPlatform_ReadFile();

inline void YakPlatform_OutputConsole();
inline clock YakPlatform_GetClock();
inline float YakPlatform_MeasureTime();

#ifdef _WIN32
#include <yak_win32.h>
#endif // _WIN32

#define YAK_PLATFORM
#endif