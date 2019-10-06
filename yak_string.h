#if !defined(YAK_STRING)

#include <yak_defines.h>
#include <yak_memory.h>
#include <yak_char.h>
EXTERN_C_START

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#endif // YAK_ENABLE_SHORTHAND_MACROS

/**
* Dependencies
* In: Memory
* Out: String
* */

typedef struct string
{
    size_t Length;
    union
    {
        wchar_t* Wide;
        char* Char;
        void* Raw;
        // TODO: Chunks
    };
} string;

// TODO: Convert to String
inline bool
YakStr_Concat(char* Source, char* Dest, size_t MaxLength)
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