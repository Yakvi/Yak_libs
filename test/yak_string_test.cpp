#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_test.h>
#include <yak_string.h>

internal void
CompareStrings()
{
    test_cat("Compare Strings");
    test(Yak_StrCmp("A", "A"), "Equal strings return true");
    test(!Yak_StrCmp("A", "B"), "Different strings return false");
    test(Yak_StrCmp("", ""), "Empty strings return true");
    test(!Yak_StrCmp("", "B"), "One Empty string returns false");
    test(Yak_StrCmp("AB", "AB"), "Longer strings are compared");
    test(!Yak_StrCmp("AA", "AB"), "Longer strings are compared");
    test(!Yak_StrCmp("Hello", "Worlds"), "Unequal strings are compared");
    test(Yak_StrCmp("Hello", "Hello"), "Unequal strings are compared");
    test(Yak_StrCmp("0", "0"), "Zero is accepted");
    test(Yak_StrCmp("0Hello", "0Hello"), "Zero doesn't mean early exit");
    test(!Yak_StrCmp("0Hello", "0Hey"), "Zero doesn't mean early exit");
}

internal void
NumbersToString()
{
    test_cat("uint to char");
    char Out[33] = {};
    test(Yak_StrCmp(Yak_UintToChar(0, Out), "0"), "Pass 0 return 0");
    test(Yak_StrCmp(Yak_UintToChar(1, Out), "1"), "Pass 1 return 1");
    test(Yak_StrCmp(Yak_UintToChar(2, Out), "2"), "Pass 2 return 2");
    test(Yak_StrCmp(Yak_UintToChar(3, Out), "3"), "Pass 3 return 3");
    test(Yak_StrCmp(Yak_UintToChar(4, Out), "4"), "Pass 4 return 4");
    test(Yak_StrCmp(Yak_UintToChar(5, Out), "5"), "Pass 5 return 5");
    test(Yak_StrCmp(Yak_UintToChar(6, Out), "6"), "Pass 6 return 6");
    test(Yak_StrCmp(Yak_UintToChar(7, Out), "7"), "Pass 7 return 7");
    test(Yak_StrCmp(Yak_UintToChar(8, Out), "8"), "Pass 8 return 8");
    test(Yak_StrCmp(Yak_UintToChar(9, Out), "9"), "Pass 9 return 9");
    test(Yak_StrCmp(Yak_UintToChar(10, Out), "10"), "Pass 10 return 10");
    test(Yak_StrCmp(Yak_UintToChar(11, Out), "11"), "Pass 11 return 11");
    test(Yak_StrCmp(Yak_UintToChar(12, Out), "12"), "Pass 12 return 12");
    test(Yak_StrCmp(Yak_UintToChar(13, Out), "13"), "Pass 13 return 13");
    test(Yak_StrCmp(Yak_UintToChar(14, Out), "14"), "Pass 14 return 14");
    test(Yak_StrCmp(Yak_UintToChar(15, Out), "15"), "Pass 15 return 15");
    test(Yak_StrCmp(Yak_UintToChar(16, Out), "16"), "Pass 16 return 16");
    test(Yak_StrCmp(Yak_UintToChar(17, Out), "17"), "Pass 17 return 17");
    test(Yak_StrCmp(Yak_UintToChar(18, Out), "18"), "Pass 18 return 18");
    test(Yak_StrCmp(Yak_UintToChar(19, Out), "19"), "Pass 19 return 19");
    test(Yak_StrCmp(Yak_UintToChar(100, Out), "100"), "Pass 100 return 100");
    test(Yak_StrCmp(Yak_UintToChar(20, Out), "20"), "Pass 20 return 20");
    test(Yak_StrCmp(Yak_UintToChar(35, Out), "35"), "Pass 35 return 35");
    test(Yak_StrCmp(Yak_UintToChar(10506, Out), "10506"), "Pass 10506 return 10506");

    test_cat("Int to char");
    test(Yak_StrCmp(Yak_IntToChar(1, Out), "1"), "Pass 1 return 1");
    test(Yak_StrCmp(Yak_IntToChar(-1, Out), "-1"), "Pass -1 return -1");
    test(Yak_StrCmp(Yak_IntToChar(-10, Out), "-10"), "Pass -10 return -10");
    test(Yak_StrCmp(Yak_IntToChar(0, Out), "0"), "Pass 0 return 0");
}

internal void
StringComposition()
{
    test_cat("String composition");
    string Output = {};
    
    test(Output.Length == 0, "Initialized string's length is 0");
}

int
main()
{
    // CompareStrings();
    // NumbersToString();
    StringComposition();

    YakTest_ShowResults();
    return (0);
}