#if !defined(YAK_STRING)
/** In: Memory
  * Out: String  */

#include <yak_defines.h>
#include <yak_memory.h>
#include <yak_char.h>
EXTERN_C_START

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define str YakStr_Compose
#define str_s(Char) YakStr_CreateSimple(Char)
#define str_const YakStr_CreateSimple
#define str_eq YakStr_Compare
#endif // YAK_ENABLE_SHORTHAND_MACROS

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

inline bool
YakStr_Compare(string A, string B)
{
    bool Result = A.Length == B.Length;

    if (Result)
    {
        Result = YakChar_Compare(A.Char, B.Char);
    }

    return (Result);
}

inline string
YakStr_CreateSimple(char* Input)
{
    string Result = {};

    Result.Length = YakChar_Length(Input);
    Result.Char = Input;

    return (Result);
}

inline string*
YakStr_Compose(memory* Memory, char* A, char* B = 0, char* C = 0, char* D = 0)
{
    size_t Length = YakChar_Length(A);
    if (B) Length += YakChar_Length(B);
    if (C) Length += YakChar_Length(C);
    if (D) Length += YakChar_Length(D);

    string* Result = YakMem_GetStruct(Memory, string);
    Result->Length = Length;
    Result->Raw = YakMem_GetSize(Memory, Length);

    size_t Added = YakChar_Copy(A, Result->Char, Length);
    if (B) Added += YakChar_Copy(B, Result->Char + Added, Length);
    if (C) Added += YakChar_Copy(C, Result->Char + Added, Length);
    if (D) Added += YakChar_Copy(D, Result->Char + Added, Length);

    return (Result);
}

inline string*
YakStr_Create(memory* Memory, char* A, char* B = 0, char* C = 0, char* D = 0)
{
    if (Memory)
    {
        return YakStr_Compose(Memory, A, B, C, D);
    }
    else
    {
        string Result = YakStr_CreateSimple(A);
        return &Result;
    }
}

EXTERN_C_END

#define YAK_STRING
#endif