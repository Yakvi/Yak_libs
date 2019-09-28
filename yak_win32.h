#if !defined(YAK_WIN32)

// TODO: Printf (with colored text)
// TODO: Diagnostics (clock cycles)

#if defined _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
// #define NOUSER
#define NONLS
// #define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
// #define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#ifdef APIENTRY
#undef APIENTRY
#endif // APIENTRY

#include <windows.h>
// #include <winnt.h>
#include <fileapi.h>   // kernel32.lib
#include <handleapi.h> // kernel32.lib
#include <memoryapi.h> // kernel32.lib

struct file
{
    unsigned int Size;
    void* Contents;
};

struct platform
{
    bool IsInitialized;
    float ClockFrequency;
    HANDLE ConsoleOutHandle;
};

struct timer
{
    long long Snapshot;
};

#ifdef DEBUG
#include <debugapi.h>
#define Yak_Log(Message) OutputDebugStringA(Message)
#define Yak_TimerStart()
#define Yak_TimerEnd(ClockStart)
#else
#define Yak_Log(Message)
#define Yak_TimerStart()
#define Yak_TimerEnd(ClockStart)
#endif // DEBUG

//
// BOOKMARK: Memory
//

static void*
YakPlatform_GetMemory(size_t Size)
{
    return VirtualAlloc(0, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void
YakPlatform_FreeMemory(void* Memory)
{
    if (Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

//
// BOOKMARK: File i/o
//

// TODO: Return by pointer?
static file
YakPlatform_ReadFile(char* Filename)
{
    file Result = {};

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            WORD FileSize32 = (WORD)FileSize.QuadPart;
            Result.Contents = YakPlatform_GetMemory(FileSize32); // TODO: involve memory here
            if (Result.Contents)
            {
                DWORD BytesRead;
                if (ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
                    (FileSize32 == BytesRead))
                {
                    // NOTE: File read successfully
                    Result.Size = FileSize32;
                }
                else
                {
                    Yak_Log("File read failed.");
                    Yak_Log("File name:");
                    Yak_Log(Filename);
                    // Yak_Log("Error code: ");
                    // GetLastError(); // errhandlingapi.h
                    YakPlatform_FreeMemory(Result.Contents);
                    Result.Contents = 0;
                }
            }
            else
            {
                Yak_Log("Memory allocation failed.");
                // Yak_Log("Error code: ");
                // GetLastError(); // errhandlingapi.h
            }
        }
        else
        {
            Yak_Log("File size retrieval failed.");
            Yak_Log("File name:");
            Yak_Log(Filename);
            // Yak_Log("Error code: ");
            // GetLastError(); // errhandlingapi.h
        }

        CloseHandle(FileHandle);
    }
    else
    {
        Yak_Log("Unable to get file handle.");
        Yak_Log("Ensure you specified correct file name and path.");
        Yak_Log("File name:");
        Yak_Log(Filename);
    }

    return (Result);
}

//
// BOOKMARK: Console i/o
//

enum ConsoleColor
{
    ConsoleColor_Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    ConsoleColor_Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    ConsoleColor_Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
    ConsoleColor_Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    ConsoleColor_Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    ConsoleColor_Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    ConsoleColor_White = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
};

inline HANDLE
YakWin32__GetConsoleHandle(unsigned int HandleType)
{
    HANDLE Result = GetStdHandle(HandleType);
    if (Result == INVALID_HANDLE_VALUE)
    {
        Yak_Log("Error during console handle initialization");
    }
    return (Result);
}

inline void
YakPlatform_OutputConsole(char* String, u32 Length, platform Platform, WORD Color = ConsoleColor_White)
{
    if (SetConsoleTextAttribute(Platform.ConsoleOutHandle, Color))
    {
        if (!WriteConsoleA(Platform.ConsoleOutHandle, String, Length, 0, 0))
        {
            Yak_Log("Error while writing to console");
        }
    }
    else
    {
        Yak_Log("Error setting text color");
    }
}

// These two are only needed if the console mode has disabled line input mode.
// (OldMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT))
inline void
Yak__ScrollScreenBuffer(HANDLE OutHandle, CONSOLE_SCREEN_BUFFER_INFO* ScreenBuffer)
{
    SMALL_RECT srctScrollRect = {};

    srctScrollRect.Top = 1;
    srctScrollRect.Right = ScreenBuffer->dwSize.X - 1;
    srctScrollRect.Bottom = ScreenBuffer->dwSize.Y - 1;

    COORD Dest = {};
    CHAR_INFO Fill = {};

    ScrollConsoleScreenBuffer(OutHandle, &srctScrollRect, 0, Dest, &Fill);
}

inline void
Yak__NewLine(HANDLE OutHandle, CONSOLE_SCREEN_BUFFER_INFO* ScreenBuffer)
{
    ScreenBuffer->dwCursorPosition.X = 0;

    if (ScreenBuffer->dwCursorPosition.Y == (ScreenBuffer->dwSize.Y - 1))
    {
        Yak__ScrollScreenBuffer(OutHandle, ScreenBuffer);
    }
    else
    {
        ScreenBuffer->dwCursorPosition.Y += 1;
    }

    if (!SetConsoleCursorPosition(OutHandle, ScreenBuffer->dwCursorPosition))
    {
        Yak_Log("NewLine error: SetConsoleCursorPosition");
    }
}

//
// BOOKMARK: Clock
//

inline float
YakWin32__GetClockFrequency(void)
{
    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    float Result = (float)PerfCountFrequencyResult.QuadPart;
    return (Result);
}

inline timer
YakPlatform_GetTime(void)
{
    timer Result = {};

    LARGE_INTEGER Clock;
    QueryPerformanceCounter(&Clock);

    Result.Snapshot = Clock.QuadPart;
    return Result;
}

inline float
YakPlatform_StopTimer(timer Start, platform Platform)
{
    timer End = YakPlatform_GetTime();
    float Result = ((float)(End.Snapshot - Start.Snapshot) /
                    Platform.ClockFrequency);
    return (Result);
}

//
// Platform Initializer
//

static platform
YakPlatform_Init(void)
{
    platform Platform = {};
    Platform.ClockFrequency = YakWin32__GetClockFrequency();
    Platform.ConsoleOutHandle = YakWin32__GetConsoleHandle(STD_OUTPUT_HANDLE);
    Platform.IsInitialized = true;
    return (Platform);
}

#undef WIN32_LEAN_AND_MEAN
#undef NOGDICAPMASKS
#undef NOVIRTUALKEYCODES
#undef NOWINMESSAGES
#undef NOWINSTYLES
#undef NOSYSMETRICS
#undef NOMENUS
#undef NOICONS
#undef NOKEYSTATES
#undef NOSYSCOMMANDS
#undef NORASTEROPS
#undef NOSHOWWINDOW
#undef OEMRESOURCE
#undef NOATOM
#undef NOCLIPBOARD
#undef NOCOLOR
#undef NOCTLMGR
#undef NODRAWTEXT
#undef NOGDI
#undef NOKERNEL
#undef NOUSER
#undef NONLS
#undef NOMB
#undef NOMEMMGR
#undef NOMETAFILE
#undef NOMINMAX
#undef NOMSG
#undef NOOPENFILE
#undef NOSCROLL
#undef NOSERVICE
#undef NOSOUND
#undef NOTEXTMETRIC
#undef NOWH
#undef NOWINOFFSETS
#undef NOCOMM
#undef NOKANJI
#undef NOHELP
#undef NOPROFILER
#undef NODEFERWINDOWPOS
#undef NOMCX

#endif

#define YAK_WIN32 1
#endif