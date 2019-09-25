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
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
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

#ifdef DEBUG
#include <debugapi.h>
#define Yak_Log(Message) OutputDebugStringA(Message)
#else
#define Yak_Log(Message)
#endif // DEBUG

inline void*
YakPlatform_GetMemory(size_t Size)
{
    return VirtualAlloc(0, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

inline void
YakPlatform_FreeMemory(void* Memory)
{
    if (Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

// TODO: Return by pointer? 
file
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
            Result.Contents = YakPlatform_GetMemory(FileSize32);
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