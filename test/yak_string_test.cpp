#define YAK_ENABLE_SHORTHAND_MACROS
#include <yak_test.h>
#include <yak_string.h>

internal void
CompareCharStrings()
{
    test_cat("Compare Strings");
    test(ch_eq("A", "A"), "Equal strings return true");
    test(!ch_eq("A", "B"), "Different strings return false");
    test(ch_eq("", ""), "Empty strings return true");
    test(!ch_eq("", "B"), "One Empty string returns false");
    test(ch_eq("AB", "AB"), "Longer strings are compared");
    test(!ch_eq("AA", "AB"), "Longer strings are compared");
    test(!ch_eq("Hello", "Worlds"), "Unequal strings are compared");
    test(ch_eq("Hello", "Hello"), "Unequal strings are compared");
    test(ch_eq("0", "0"), "Zero is accepted");
    test(ch_eq("0Hello", "0Hello"), "Zero doesn't mean early exit");
    test(!ch_eq("0Hello", "0Hey"), "Zero doesn't mean early exit");
}

internal void
NumbersToChar()
{
    test_cat("uint to char");
    char Out[33] = {};
    test(ch_eq(u2char(0, Out), "0"), "Pass 0 return 0");
    test(ch_eq(u2char(1, Out), "1"), "Pass 1 return 1");
    test(ch_eq(u2char(2, Out), "2"), "Pass 2 return 2");
    test(ch_eq(u2char(3, Out), "3"), "Pass 3 return 3");
    test(ch_eq(u2char(4, Out), "4"), "Pass 4 return 4");
    test(ch_eq(u2char(5, Out), "5"), "Pass 5 return 5");
    test(ch_eq(u2char(6, Out), "6"), "Pass 6 return 6");
    test(ch_eq(u2char(7, Out), "7"), "Pass 7 return 7");
    test(ch_eq(u2char(8, Out), "8"), "Pass 8 return 8");
    test(ch_eq(u2char(9, Out), "9"), "Pass 9 return 9");
    test(ch_eq(u2char(10, Out), "10"), "Pass 10 return 10");
    test(ch_eq(u2char(11, Out), "11"), "Pass 11 return 11");
    test(ch_eq(u2char(12, Out), "12"), "Pass 12 return 12");
    test(ch_eq(u2char(13, Out), "13"), "Pass 13 return 13");
    test(ch_eq(u2char(14, Out), "14"), "Pass 14 return 14");
    test(ch_eq(u2char(15, Out), "15"), "Pass 15 return 15");
    test(ch_eq(u2char(16, Out), "16"), "Pass 16 return 16");
    test(ch_eq(u2char(17, Out), "17"), "Pass 17 return 17");
    test(ch_eq(u2char(18, Out), "18"), "Pass 18 return 18");
    test(ch_eq(u2char(19, Out), "19"), "Pass 19 return 19");
    test(ch_eq(u2char(100, Out), "100"), "Pass 100 return 100");
    test(ch_eq(u2char(20, Out), "20"), "Pass 20 return 20");
    test(ch_eq(u2char(35, Out), "35"), "Pass 35 return 35");
    test(ch_eq(u2char(10506, Out), "10506"), "Pass 10506 return 10506");

    test_cat("Int to char");
    test(ch_eq(s2char(1, Out), "1"), "Pass 1 return 1");
    test(ch_eq(s2char(-1, Out), "-1"), "Pass -1 return -1");
    test(ch_eq(s2char(-10, Out), "-10"), "Pass -10 return -10");
    test(ch_eq(s2char(0, Out), "0"), "Pass 0 return 0");
}

internal void
StringCreation()
{
    test_cat("String creation");
    {
        string Output = {};
        test(Output.Length == 0, "Initialized string's length is 0");
        test(Output.Char == 0, "Char component is accessible");
        test(Output.Raw == 0, "Raw component is accessible");
        test(Output.Wide == 0, "Wide component is accessible");
    }

    {
        test(ch_len("12345") == 5, "Char length is calculated");
        string Output = str_s("Hello");

        test(Output.Length == 5, "Length is correct");
        test(ch_eq(Output.Char, "Hello"), "Contents are correct");

        string Comparison = {};
        Comparison.Length = 5;
        Comparison.Char = "Hello";

        test(str_eq(Output, Comparison), "Equal strings are evaluated correctly");
        Comparison.Char = "World";
        test(!str_eq(Output, Comparison), "Not equal strings are evaluated correctly");
    }
}

internal void
StringConcatenation()
{
    test_cat("StringConcatenation");
    memory* Memory = mem_init(4096);
    string* Test = str(Memory, "Hello", "World");
    test(ch_eq(Test->Char, "Hello World"), "2 Strings composed");
    Test = str(Memory, Test->Char, "! ", "How are you today?");
    test(ch_eq(Test->Char, "Hello World! How are you today?"), "3 strings composed");
    Test = str(Memory, "Hello", ", ", "World", "!");
    test(ch_eq(Test->Char, "Hello, World!"), "4 strings composed");
}

int
main()
{
    // CompareCharStrings();
    // NumbersToChar();
    StringCreation();
    StringConcatenation();

    test_output();
    return 0;
}