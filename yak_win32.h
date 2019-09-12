#if !defined(YAK_WIN32)

#ifdef _WIN32

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

#include <windows.h>
#include <winnt.h>
#include <fileapi.h>   // kernel32.lib
#include <handleapi.h> // kernel32.lib
#include <memoryapi.h> // kernel32.lib

struct file
{
    unsigned int Size;
    void* Contents;
};

#ifdef _DEBUG
#include <debugapi.h>
#define Yak_Log(Message) OutputDebugStringA(Message)
#else
#define Yak_Log(Message)
#endif // _DEBUG

void*
Yak_GetMemory(size_t Size)
{
    return VirtualAlloc(0, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

void
Yak_FreeMemory(void* Memory)
{
    if (Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

file
Yak_ReadFile(char* Filename)
{
    file Result = {};

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            WORD FileSize32 = (WORD)FileSize.QuadPart;
            Result.Contents = Yak_GetMemory(FileSize32);
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
                    Yak_FreeMemory(Result.Contents);
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

#endif

#define YAK_WIN32 1
#endif