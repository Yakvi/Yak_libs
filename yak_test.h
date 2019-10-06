#if !defined(YAK_TEST)

///
/// API
///
// YakTest_Category(Description);    Use this to split your tests into groups. Not nestable.
// YakTest_Assert(Test, Name);       Define your tests. These are executed right away, the results are stored for later use.
// YakTest_ShowResults();            Call after you define your tests.
// YakTest_Cleanup();                Call if you wish to reset your tests with new data.
//
// Also see shorthand versions below, enabled by defining YAK_ENABLE_SHORTHAND_MACROS before including this

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef YAK_ENABLE_SHORTHAND_MACROS
#define test YakTest_Assert
#define test_cat YakTest_Category
#define test_clear YakTest_Cleanup
#define test_output YakTest_ShowResults
#define test_print YakTest_PrintWithNewLine
#endif // YAK_ENABLE_SHORTHAND_MACROS

#include <stdio.h> 
#include <malloc.h> // TODO: Too many mallocs
#define Yak_Alloc(item) (item*)malloc(sizeof(item))
#define Yak_FreeListItem(item, temp) \
    if (item)                        \
    {                                \
        temp = item->Next;           \
        free(item);                  \
        item = temp;                 \
    }
# define YakTest_Print(String, ...) printf(String, ##__VA_ARGS__)
# define YakTest_PrintWithNewLine(String, ...) YakTest_Print(String, ##__VA_ARGS__); YakTest_Print("\n")

struct condition;
struct test;

inline void Yak__GetNextTestSlot(const char* Name);
inline void Yak__PopulateConditionAndAdvance(const char* TestText, size_t TestValue, const char* Name);
static condition* Yak__InitializeTestGlobals();

static test* YAK_GLOBAL__FirstTestSlot;
static test* YAK_GLOBAL__CurrentTestSlot;
static condition* YAK_GLOBAL__CurrentCondition = Yak__InitializeTestGlobals();

#define YakTest_Category(Name) Yak__GetNextTestSlot(Name)
#define YakTest_Assert(Test, Name) Yak__PopulateConditionAndAdvance(#Test, (size_t)(Test), (Name))

//
// BOOKMARK: IMPLEMENTATION
//

struct condition
{
    const char* Description;
    const char* TestDescription;
    unsigned Result;

    condition* Next;
};

struct test
{
    const char* Description;
    condition* FirstCondition;
    unsigned ConditionCount;

    test* Next;
};

inline condition*
Yak__GetCondition()
{
    condition* Result = Yak_Alloc(condition);

    Result->Next = 0;
    Result->Description = 0;
    Result->TestDescription = 0;
    Result->Result = 0;

    return (Result);
}

inline test*
Yak__GetTestSlot(const char* Name)
{
    test* Result = Yak_Alloc(test);

    Result->Description = Name;
    Result->ConditionCount = 0;
    Result->FirstCondition = 0;
    Result->Next = 0;

    return (Result);
}

static condition*
Yak__InitializeTestGlobals()
{
    YAK_GLOBAL__FirstTestSlot = YAK_GLOBAL__CurrentTestSlot = Yak__GetTestSlot(0);
    condition* FirstCondition = YAK_GLOBAL__FirstTestSlot->FirstCondition = Yak__GetCondition();

    return (FirstCondition);
}

inline void
Yak__GetNextTestSlot(const char* Name)
{
    test* NextTest = Yak__GetTestSlot(Name);

    YAK_GLOBAL__CurrentTestSlot->Next = NextTest;
    YAK_GLOBAL__CurrentTestSlot = YAK_GLOBAL__CurrentTestSlot->Next;

    YAK_GLOBAL__CurrentCondition = NextTest->FirstCondition = Yak__GetCondition();
}

inline void
Yak__PopulateConditionAndAdvance(const char* TestText, size_t TestValue, const char* Name)
{
    YAK_GLOBAL__CurrentTestSlot->ConditionCount++;

    YAK_GLOBAL__CurrentCondition->Description = Name;
    YAK_GLOBAL__CurrentCondition->TestDescription = TestText;
    YAK_GLOBAL__CurrentCondition->Result = (unsigned)TestValue;
    YAK_GLOBAL__CurrentCondition->Next = Yak__GetCondition();

    YAK_GLOBAL__CurrentCondition = YAK_GLOBAL__CurrentCondition->Next;
}


/** Free memory of each condition and each test */
static void
YakTest_Cleanup(void)
{
    // Free tests linked list
    test* CurrentSlot = YAK_GLOBAL__FirstTestSlot->Next;
    test* NextSlot;
    for (;;)
    {
        if (CurrentSlot)
        {
            // Free conditions linked list for each test
            condition* CurrentCondition = CurrentSlot->FirstCondition;
            condition* NextCondition;
            for (;;)
            {
                if (CurrentCondition)
                {
                    Yak_FreeListItem(CurrentCondition, NextCondition);
                }
                else
                {
                    break;
                }
            }

            Yak_FreeListItem(CurrentSlot, NextSlot);
        }
        else
        {
            break;
        }
    }

    // Reinitialize the globals
    YAK_GLOBAL__CurrentCondition = Yak__InitializeTestGlobals();
}

static void
YakTest_ShowResults(void)
{
    YakTest_PrintWithNewLine("\n===|===|=== TEST RESULTS ===|===|===\n");
    for (test* Test = YAK_GLOBAL__FirstTestSlot;
         Test;
         Test = Test->Next)
    {
        if (Test->ConditionCount > 0)
        {
            YakTest_PrintWithNewLine(" * %s:", Test->Description ? Test->Description : "Base asserts");

            for (condition* Condition = Test->FirstCondition;
                 Condition->Next; // we stop if we don't see a next condition, this one is empty
                 Condition = Condition->Next)
            {
                YakTest_Print("[");
                // NOTE: Colored text
                if (Condition->Result)
                {
                    YakTest_Print("\033[0;32m");
                    YakTest_Print("PASS");
                }
                else
                {
                    YakTest_Print("\033[1;31m");
                    YakTest_Print("FAIL");
                }
                YakTest_Print("\033[0m");

                YakTest_PrintWithNewLine("]: %s", Condition->Description);
            }
        }
    }

    YakTest_PrintWithNewLine("\n===|===|===|===|===|===|===|===|===\n");

    YakTest_Cleanup(); // Free memory // TODO: Do I need to call it each time I call results?
}

}

#define YAK_TEST 1
#endif // !(YAK_TEST)