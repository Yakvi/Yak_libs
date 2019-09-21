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

char*
UintToChar(unsigned int Input, char* Output)
{
    int TopMask = 0xF;
    int ByteTopBitMask = 0b1000;
    int ByteMidBitsMask = 0b110;

    int BotByte = Input & TopMask;
    int EncodingStart = 48;
    // Eliminate top value from A-F
    int Value = 0;
    if (BotByte & ByteTopBitMask) // Value is 8-15
    {

        if (!(BotByte & ByteMidBitsMask)) // Value is 11-15
        {
        }
        else // Value is 8-10
        {
            Input = Input >> 3;
            Value = (BotByte & 0b111) - 2;
        }
        *Output = (char)(Value + EncodingStart);
        UintToChar(Input, Output + 1);
    }
    else // Value is 0-9
    {
        Value = BotByte;
        *Output = (char)(Value + EncodingStart);
    }

    RevertString(Output);
    return (Output);
}

int
main()
{
#if 0
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
#endif
#if 1
    Y_TEST_CATEGORY("u32 to char");
    {
        char Out[10] = {};
        Y_ASSERT(CompareStrings(UintToChar(0, Out), "0"), "Pass 0 return 0");
        Y_ASSERT(CompareStrings(UintToChar(2, Out), "2"), "Pass 2 return 2");
        Y_ASSERT(CompareStrings(UintToChar(10, Out), "10"), "Pass 10 return 10");
        Y_ASSERT(CompareStrings(UintToChar(11, Out), "11"), "Pass 11 return 11");
    }
#endif

    YTestShowResults();
    return (0);
}