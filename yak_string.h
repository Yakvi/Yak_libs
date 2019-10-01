#if !defined(YAK_STRING)

#include <yak_defines.h>
#include <yak_memory.h>
EXTERN_C_START

/**
 * Requirements
 
* API
* In: Memory
* Out: n/a
* */

typedef struct string
{
    size_t Length;
    union
    {
        wchar_t* Wide;
        char* Char;
        void* Raw;
    };
} string;

inline bool
Yak_StrCmp(char* A, char* B)
{
    bool Result = false;

    if (A && B) // Pointers must be valid
    {
        Result = true;
        char AtA = *A;
        char AtB = *B;
        while (AtA || AtB)
        {
            if ((!AtA || !AtB) ||
                (AtA != AtB))
            {
                Result = false;
                break;
            }
            AtA = *++A;
            AtB = *++B;
        }
    }

    return (Result);
}

// void
// StringCopy(char* Source, char* Dest, int MaxLength)
// {
//     // TODO: cleanup this
//     int CharCount = 0;
//     while (*Source)
//     {
//         if (CharCount++ >= MaxLength)
//         {
//             // TODO: Error
//             break;
//         }
//         *Dest++ = *Source++;
//     }
// }

// void
// InvertString(char* Source)
// {
//     // TODO: cleanup this
//     if (*(Source + 1))
//     {
//         const int TempBufferLength = 200;
//         char Temp[TempBufferLength] = {};
//         StringCopy(Source, Temp + 1, TempBufferLength - 1);
//         char* At = Temp + TempBufferLength - 1;
//         while (!*At)
//         {
//             --At;
//         }
//         while (*At)
//         {
//             *Source++ = *At--;
//         }
//     }
// }

#define ENCODING_START 48
// #define BYTE_TOP_BIT_MASK 0b100
// #define BYTE_MASK 0xF

inline char*
Yak__UintToCharRecursive(unsigned int Input, char* Output, int& Depth)
{
    int Value = Input;
    if (Input > 9)
    {
        Yak__UintToCharRecursive((unsigned int)(Input * 0.1f), Output, Depth);
        Depth++;
        Value = Input % 10;
    }

    Output[Depth] = (char)(Value + ENCODING_START);

    return (Output);
}

inline char*
Yak_UintToChar(unsigned int Input, char* Output)
{
    int Depth = 0;
    Yak__UintToCharRecursive(Input, Output, Depth);
    Output[Depth + 1] = 0;

    return (Output);
}

inline char*
Yak_IntToChar(int Input, char* Output)
{
    int Depth = 0;
    if (Input < 0)
    {
        Output[Depth++] = '-';
    }

    Yak__UintToCharRecursive(Input, Output, Depth);
    Output[Depth + 1] = 0;

    return (Output);
}

inline bool
Yak_FillBuffer(char* Source, char* Dest, size_t MaxLength)
{
    bool Result = true;
    size_t BufferIndex = 0;
    char* At = Source;
    while (At)
    {
        Dest[BufferIndex++] = *At++;
        if (BufferIndex >= MaxLength)
        {
            Result = false;
            break;
        }
    }

    return (Result);
}

EXTERN_C_END

#define YAK_STRING
#endif