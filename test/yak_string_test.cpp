#include <yak_test.h>

bool
CompareStrings(char* A, char* B)
{
    bool Result = true;

    char AtA = *A;
    char AtB = *B;
    while (AtA || AtB)
    {
        if ((!AtA || !AtB) || (AtA != AtB))
        {
            Result = false;
            break;
        }
        AtA = *++A;
        AtB = *++B;
    }

    return (Result);
}

char*
UintToChar(unsigned int Input)
{
    return "0";
}

int
main()
{
    Y_ASSERT(true, "Hello world");
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
    }
    Y_TEST_CATEGORY("u32 to char");
    {
    }

    YTestShowResults();
    return (0);
}