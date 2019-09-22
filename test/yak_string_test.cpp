#include <yak_test.h>

bool
CompareStrings(char* A, char* B)
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

char*
UintToCharRecursive(unsigned int Input, char* Output, int& Depth)
{
    int Value = Input;
    if (Input > 9)
    {
        UintToCharRecursive((unsigned int)(Input * 0.1f), Output, Depth);
        Depth++;
        Value = Input % 10;
    }

    Output[Depth] = (char)(Value + ENCODING_START);

    return (Output);
}

char*
UintToChar(unsigned int Input, char* Output)
{
    int Depth = 0;
    UintToCharRecursive(Input, Output, Depth);
    Output[Depth + 1] = 0;

    return (Output);
}

int
main()
{
    Y_TEST_CATEGORY("Compare Strings");
    {
        Y_ASSERT(CompareStrings("A", "A"), "Equal strings return true");
        Y_ASSERT(!CompareStrings("A", "B"), "Different strings return false");
        Y_ASSERT(CompareStrings("", ""), "Empty strings return true");
        Y_ASSERT(!CompareStrings("", "B"), "One Empty string returns false");
        Y_ASSERT(CompareStrings("AB", "AB"), "Longer strings are compared");
        Y_ASSERT(!CompareStrings("AA", "AB"), "Longer strings are compared");
        Y_ASSERT(!CompareStrings("Hello", "Worlds"), "Unequal strings are compared");
        Y_ASSERT(CompareStrings("Hello", "Hello"), "Unequal strings are compared");
        Y_ASSERT(CompareStrings("0", "0"), "Zero is accepted");
        Y_ASSERT(CompareStrings("0Hello", "0Hello"), "Zero doesn't mean early exit");
        Y_ASSERT(!CompareStrings("0Hello", "0Hey"), "Zero doesn't mean early exit");
    }
    Y_TEST_CATEGORY("uint to char");
    {
        char Out[33] = {};
        Y_ASSERT(CompareStrings(UintToChar(0, Out), "0"), "Pass 0 return 0");
        Y_ASSERT(CompareStrings(UintToChar(1, Out), "1"), "Pass 1 return 1");
        Y_ASSERT(CompareStrings(UintToChar(2, Out), "2"), "Pass 2 return 2");
        Y_ASSERT(CompareStrings(UintToChar(3, Out), "3"), "Pass 3 return 3");
        Y_ASSERT(CompareStrings(UintToChar(4, Out), "4"), "Pass 4 return 4");
        Y_ASSERT(CompareStrings(UintToChar(5, Out), "5"), "Pass 5 return 5");
        Y_ASSERT(CompareStrings(UintToChar(6, Out), "6"), "Pass 6 return 6");
        Y_ASSERT(CompareStrings(UintToChar(7, Out), "7"), "Pass 7 return 7");
        Y_ASSERT(CompareStrings(UintToChar(8, Out), "8"), "Pass 8 return 8");
        Y_ASSERT(CompareStrings(UintToChar(9, Out), "9"), "Pass 9 return 9");
        Y_ASSERT(CompareStrings(UintToChar(10, Out), "10"), "Pass 10 return 10");
        Y_ASSERT(CompareStrings(UintToChar(11, Out), "11"), "Pass 11 return 11");
        Y_ASSERT(CompareStrings(UintToChar(12, Out), "12"), "Pass 12 return 12");
        Y_ASSERT(CompareStrings(UintToChar(13, Out), "13"), "Pass 13 return 13");
        Y_ASSERT(CompareStrings(UintToChar(14, Out), "14"), "Pass 14 return 14");
        Y_ASSERT(CompareStrings(UintToChar(15, Out), "15"), "Pass 15 return 15");
        Y_ASSERT(CompareStrings(UintToChar(16, Out), "16"), "Pass 16 return 16");
        Y_ASSERT(CompareStrings(UintToChar(17, Out), "17"), "Pass 17 return 17");
        Y_ASSERT(CompareStrings(UintToChar(18, Out), "18"), "Pass 18 return 18");
        Y_ASSERT(CompareStrings(UintToChar(19, Out), "19"), "Pass 19 return 19");
        Y_ASSERT(CompareStrings(UintToChar(100, Out), "100"), "Pass 100 return 100");
        Y_ASSERT(CompareStrings(UintToChar(20, Out), "20"), "Pass 20 return 20");
        Y_ASSERT(CompareStrings(UintToChar(35, Out), "35"), "Pass 35 return 35");
        Y_ASSERT(CompareStrings(UintToChar(10506, Out), "10506"), "Pass 10506 return 10506");
        Y_ASSERT(CompareStrings(UintToChar(1, Out), "1"), "Pass 1 return 1");
    }

    YTestShowResults();
    return (0);
}