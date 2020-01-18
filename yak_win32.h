#if !defined(YAK_WIN32)

typedef struct file
{
    unsigned int Size;
    union
    {
        char* Text;
        void* Contents;
    };
} file;

typedef struct platform
{
    bool IsInitialized;
    float ClockFrequency;
    void* ConsoleOutHandle;
} platform;

typedef struct clock
{
    long long Time;
} clock;

#ifdef DEBUG
#define Yak_Log(Message) OutputDebugStringA(Message)
#else
#define Yak_Log(Message)
#endif // DEBUG

//
// BOOKMARK: Windows API
//
// #include <windows.h>
#ifndef _WINDOWS_
typedef union
{
    struct
    {
        unsigned long LowPart;
        long HighPart;
    } DUMMYSTRUCTNAME;
    struct
    {
        unsigned long LowPart;
        long HighPart;
    } u;
    long long QuadPart;
} LARGE_INTEGER;
typedef struct
{
    short X;
    short Y;
} COORD;
typedef struct
{
    short Left;
    short Top;
    short Right;
    short Bottom;
} SMALL_RECT;
typedef struct
{
    COORD dwSize;
    COORD dwCursorPosition;
    unsigned short wAttributes;
    SMALL_RECT srWindow;
    COORD dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO;
typedef struct
{
    union
    {
        wchar_t UnicodeChar;
        char AsciiChar;
    } Char;
    unsigned short Attributes;
} CHAR_INFO;

typedef struct
{
    unsigned long nLength;
    void* lpSecurityDescriptor;
    int bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct
{
    unsigned __int64 Internal;
    unsigned __int64 InternalHigh;
    union
    {
        struct
        {
            unsigned long Offset;
            unsigned long OffsetHigh;
        } DUMMYSTRUCTNAME;
        void* Pointer;
    } DUMMYUNIONNAME;

    void* hEvent;
} OVERLAPPED, *LPOVERLAPPED;

#define MEM_RESERVE 0x00002000
#define MEM_COMMIT 0x00001000
#define PAGE_READWRITE 0x04
#define MEM_RELEASE 0x00008000

#define GENERIC_READ (0x80000000L)
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3

#define FOREGROUND_BLUE 0x0001      // text color contains blue.
#define FOREGROUND_GREEN 0x0002     // text color contains green.
#define FOREGROUND_RED 0x0004       // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE 0x0010      // background color contains blue.
#define BACKGROUND_GREEN 0x0020     // background color contains green.
#define BACKGROUND_RED 0x0040       // background color contains red.
#define STD_OUTPUT_HANDLE ((unsigned long)-11)

#define INVALID_HANDLE_VALUE ((void*)(long long)-1)

extern "C" {
void* __stdcall VirtualAlloc(void* lpAddress, size_t Size, unsigned int AllocationType, unsigned int Protect);

int __stdcall VirtualFree(void* lpAddress, size_t dwSize, unsigned long dwFreeType);

void* __stdcall GetStdHandle(unsigned long nStdHandle);
int __stdcall SetConsoleTextAttribute(void* hConsoleOutput, unsigned short wAttributes);
int __stdcall WriteConsoleA(void* hConsoleOutput, const void* lpBuffer, unsigned long nNumberOfCharsToWrite, unsigned long* lpNumberOfCharsWritten, void* lpReserved);

int __stdcall ScrollConsoleScreenBufferA(void* hConsoleOutput, const SMALL_RECT* lpScrollRectangle, const SMALL_RECT* lpClipRectangle, COORD dwDestinationOrigin, const CHAR_INFO* lpFill);
int __stdcall SetConsoleCursorPosition(void* hConsoleOutput, COORD dwCursorPosition);

int __stdcall GetFileSizeEx(void* hFile, LARGE_INTEGER* lpFileSize);
unsigned long __stdcall GetLastError();
void* __stdcall CreateFileA(const char* lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile);
int __stdcall ReadFile(void* hFile, void* lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long* lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
int __stdcall CloseHandle(void* hObject);

void __stdcall OutputDebugStringA(char* string);

int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
int __stdcall QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
}
#endif
//
// BOOKMARK: Memory, kernel32.lib
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

    void* FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            unsigned short FileSize32 = (unsigned short)FileSize.QuadPart;
            Result.Contents = YakPlatform_GetMemory(FileSize32); // TODO: involve memory here
            if (Result.Contents)
            {
                unsigned long BytesRead;
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
                    // GetLastError();
                    YakPlatform_FreeMemory(Result.Contents); // TODO: involve memory module
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

inline void*
YakWin32__GetConsoleHandle(unsigned int HandleType)
{
    void* Result = GetStdHandle(HandleType);
    if (Result == INVALID_HANDLE_VALUE)
    {
        Yak_Log("Error during console handle initialization");
    }
    return (Result);
}

inline void
YakPlatform_OutputConsole(char* String, u32 Length, platform Platform, unsigned short Color = ConsoleColor_White)
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
Yak__ScrollScreenBuffer(void* OutHandle, CONSOLE_SCREEN_BUFFER_INFO* ScreenBuffer)
{
    SMALL_RECT srctScrollRect = {};

    srctScrollRect.Top = 1;
    srctScrollRect.Right = ScreenBuffer->dwSize.X - 1;
    srctScrollRect.Bottom = ScreenBuffer->dwSize.Y - 1;

    COORD Dest = {};
    CHAR_INFO Fill = {};

    ScrollConsoleScreenBufferA(OutHandle, &srctScrollRect, 0, Dest, &Fill);
}

inline void
Yak__NewLine(void* OutHandle, CONSOLE_SCREEN_BUFFER_INFO* ScreenBuffer)
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

inline clock
YakPlatform_GetClock(void)
{
    clock Result = {};

    LARGE_INTEGER Clock;
    QueryPerformanceCounter(&Clock);

    Result.Time = Clock.QuadPart;
    return Result;
}

// TODO: GetClockInSeconds

inline float
YakPlatform_MeasureTime(clock Start, platform Platform)
{
    clock End = YakPlatform_GetClock();
    float Result = ((float)(End.Time - Start.Time) /
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

#define YAK_WIN32 1
#endif