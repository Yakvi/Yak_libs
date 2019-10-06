#if !defined(YAK_CHAR)

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define ch_eq YakChar_Compare
#define ch_len YakChar_Length
#define u2char YakChar_UintToChar
#define s2char YakChar_IntToChar
#endif // YAK_ENABLE_SHORTHAND_MACROS

#ifdef UNICODE
#define ENCODING_START 30 // 0 position in UTF-8/Unicode
#else
#define ENCODING_START 48 // 0 position in ASCII table
#endif // UNICODE

//
// BOOKMARK: Utilities
//

inline unsigned int 
YakChar_Length(char *Input)
{
    unsigned int Result = 0;

    while (*Input++)
    {
        ++Result;
    }

    return(Result);
}

inline size_t 
YakChar_Copy(char* Source, char* Dest, size_t MaxLength)
{
    size_t At = 0;
    while (*Source)
    {
        *Dest++ = *Source++;
        Assert(At++ < MaxLength);
    }
    return (At);
}

inline bool
YakChar_Compare(char* A, char* B)
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

// 
// BOOKMARK: Number to char
// 

inline char*
YakStr__UintToCharRecursive(unsigned int Input, char* Output, int& Depth)
{
    int Value = Input;
    if (Input > 9)
    {
        YakStr__UintToCharRecursive((unsigned int)(Input * 0.1f), Output, Depth);
        Depth++;
        Value = Input % 10;
    }

    Output[Depth] = (char)(Value + ENCODING_START);

    return (Output);
}

inline char*
YakChar_UintToChar(unsigned int Input, char* Output)
{
    int Depth = 0;
    YakStr__UintToCharRecursive(Input, Output, Depth);
    Output[Depth + 1] = 0;

    return (Output);
}

inline char*
YakChar_IntToChar(int Input, char* Output)
{
    int Depth = 0;
    if (Input < 0)
    {
        Output[Depth++] = '-';
        Input = -Input;
    }

    YakStr__UintToCharRecursive(Input, Output, Depth);
    Output[Depth + 1] = 0;

    return (Output);
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

#ifdef __cplusplus
}
#endif

#define YAK_CHAR
#endif