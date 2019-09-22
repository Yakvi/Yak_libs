#include <yak_test.h>
#include <yak_string.h>

int
main()
{
    Y_TEST_CATEGORY("Compare Strings");
    {
        Y_ASSERT(Yak_StrCmp("A", "A"), "Equal strings return true");
        Y_ASSERT(!Yak_StrCmp("A", "B"), "Different strings return false");
        Y_ASSERT(Yak_StrCmp("", ""), "Empty strings return true");
        Y_ASSERT(!Yak_StrCmp("", "B"), "One Empty string returns false");
        Y_ASSERT(Yak_StrCmp("AB", "AB"), "Longer strings are compared");
        Y_ASSERT(!Yak_StrCmp("AA", "AB"), "Longer strings are compared");
        Y_ASSERT(!Yak_StrCmp("Hello", "Worlds"), "Unequal strings are compared");
        Y_ASSERT(Yak_StrCmp("Hello", "Hello"), "Unequal strings are compared");
        Y_ASSERT(Yak_StrCmp("0", "0"), "Zero is accepted");
        Y_ASSERT(Yak_StrCmp("0Hello", "0Hello"), "Zero doesn't mean early exit");
        Y_ASSERT(!Yak_StrCmp("0Hello", "0Hey"), "Zero doesn't mean early exit");
    }
    Y_TEST_CATEGORY("uint to char");
    {
        char Out[33] = {};
        Y_ASSERT(Yak_StrCmp(UintToChar(0, Out), "0"), "Pass 0 return 0");
        Y_ASSERT(Yak_StrCmp(UintToChar(1, Out), "1"), "Pass 1 return 1");
        Y_ASSERT(Yak_StrCmp(UintToChar(2, Out), "2"), "Pass 2 return 2");
        Y_ASSERT(Yak_StrCmp(UintToChar(3, Out), "3"), "Pass 3 return 3");
        Y_ASSERT(Yak_StrCmp(UintToChar(4, Out), "4"), "Pass 4 return 4");
        Y_ASSERT(Yak_StrCmp(UintToChar(5, Out), "5"), "Pass 5 return 5");
        Y_ASSERT(Yak_StrCmp(UintToChar(6, Out), "6"), "Pass 6 return 6");
        Y_ASSERT(Yak_StrCmp(UintToChar(7, Out), "7"), "Pass 7 return 7");
        Y_ASSERT(Yak_StrCmp(UintToChar(8, Out), "8"), "Pass 8 return 8");
        Y_ASSERT(Yak_StrCmp(UintToChar(9, Out), "9"), "Pass 9 return 9");
        Y_ASSERT(Yak_StrCmp(UintToChar(10, Out), "10"), "Pass 10 return 10");
        Y_ASSERT(Yak_StrCmp(UintToChar(11, Out), "11"), "Pass 11 return 11");
        Y_ASSERT(Yak_StrCmp(UintToChar(12, Out), "12"), "Pass 12 return 12");
        Y_ASSERT(Yak_StrCmp(UintToChar(13, Out), "13"), "Pass 13 return 13");
        Y_ASSERT(Yak_StrCmp(UintToChar(14, Out), "14"), "Pass 14 return 14");
        Y_ASSERT(Yak_StrCmp(UintToChar(15, Out), "15"), "Pass 15 return 15");
        Y_ASSERT(Yak_StrCmp(UintToChar(16, Out), "16"), "Pass 16 return 16");
        Y_ASSERT(Yak_StrCmp(UintToChar(17, Out), "17"), "Pass 17 return 17");
        Y_ASSERT(Yak_StrCmp(UintToChar(18, Out), "18"), "Pass 18 return 18");
        Y_ASSERT(Yak_StrCmp(UintToChar(19, Out), "19"), "Pass 19 return 19");
        Y_ASSERT(Yak_StrCmp(UintToChar(100, Out), "100"), "Pass 100 return 100");
        Y_ASSERT(Yak_StrCmp(UintToChar(20, Out), "20"), "Pass 20 return 20");
        Y_ASSERT(Yak_StrCmp(UintToChar(35, Out), "35"), "Pass 35 return 35");
        Y_ASSERT(Yak_StrCmp(UintToChar(10506, Out), "10506"), "Pass 10506 return 10506");
        Y_ASSERT(Yak_StrCmp(UintToChar(1, Out), "1"), "Pass 1 return 1");
    }

    YTestShowResults();
    return (0);
}