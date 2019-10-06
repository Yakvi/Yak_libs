#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_test.h>
#include <yak_string.h>

internal void
CompareStrings()
{
    test_cat("Compare Strings");
    test(str_eq("A", "A"), "Equal strings return true");
    test(!str_eq("A", "B"), "Different strings return false");
    test(str_eq("", ""), "Empty strings return true");
    test(!str_eq("", "B"), "One Empty string returns false");
    test(str_eq("AB", "AB"), "Longer strings are compared");
    test(!str_eq("AA", "AB"), "Longer strings are compared");
    test(!str_eq("Hello", "Worlds"), "Unequal strings are compared");
    test(str_eq("Hello", "Hello"), "Unequal strings are compared");
    test(str_eq("0", "0"), "Zero is accepted");
    test(str_eq("0Hello", "0Hello"), "Zero doesn't mean early exit");
    test(!str_eq("0Hello", "0Hey"), "Zero doesn't mean early exit");
}

internal void
NumbersToString()
{
    test_cat("uint to char");
    char Out[33] = {};
    test(str_eq(u2char(0, Out), "0"), "Pass 0 return 0");
    test(str_eq(u2char(1, Out), "1"), "Pass 1 return 1");
    test(str_eq(u2char(2, Out), "2"), "Pass 2 return 2");
    test(str_eq(u2char(3, Out), "3"), "Pass 3 return 3");
    test(str_eq(u2char(4, Out), "4"), "Pass 4 return 4");
    test(str_eq(u2char(5, Out), "5"), "Pass 5 return 5");
    test(str_eq(u2char(6, Out), "6"), "Pass 6 return 6");
    test(str_eq(u2char(7, Out), "7"), "Pass 7 return 7");
    test(str_eq(u2char(8, Out), "8"), "Pass 8 return 8");
    test(str_eq(u2char(9, Out), "9"), "Pass 9 return 9");
    test(str_eq(u2char(10, Out), "10"), "Pass 10 return 10");
    test(str_eq(u2char(11, Out), "11"), "Pass 11 return 11");
    test(str_eq(u2char(12, Out), "12"), "Pass 12 return 12");
    test(str_eq(u2char(13, Out), "13"), "Pass 13 return 13");
    test(str_eq(u2char(14, Out), "14"), "Pass 14 return 14");
    test(str_eq(u2char(15, Out), "15"), "Pass 15 return 15");
    test(str_eq(u2char(16, Out), "16"), "Pass 16 return 16");
    test(str_eq(u2char(17, Out), "17"), "Pass 17 return 17");
    test(str_eq(u2char(18, Out), "18"), "Pass 18 return 18");
    test(str_eq(u2char(19, Out), "19"), "Pass 19 return 19");
    test(str_eq(u2char(100, Out), "100"), "Pass 100 return 100");
    test(str_eq(u2char(20, Out), "20"), "Pass 20 return 20");
    test(str_eq(u2char(35, Out), "35"), "Pass 35 return 35");
    test(str_eq(u2char(10506, Out), "10506"), "Pass 10506 return 10506");

    test_cat("Int to char");
    test(str_eq(s2char(1, Out), "1"), "Pass 1 return 1");
    test(str_eq(s2char(-1, Out), "-1"), "Pass -1 return -1");
    test(str_eq(s2char(-10, Out), "-10"), "Pass -10 return -10");
    test(str_eq(s2char(0, Out), "0"), "Pass 0 return 0");
}

internal void
StringComposition()
{
    test_cat("String composition");
    {
    string Output = {};    
    test(Output.Length == 0, "Initialized string's length is 0");
    test(Output.Char == 0, "Char component is accessible");
    test(Output.Raw == 0, "Raw component is accessible");
    test(Output.Wide == 0, "Wide component is accessible");}

    // test(String("Hello").Length == 5, "Correct string data is returned upon creation");
}

int
main()
{
    CompareStrings();
    NumbersToString();
    StringComposition();

    YakTest_ShowResults();
    return (0);
}