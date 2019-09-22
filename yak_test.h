#if !defined(YAK_TEST)

///
/// API
///
// YakTest_ShowResults();            Call after you define your tests.
// YakTest_Cleanup();                Call if you wish to reset your tests with new data.
// YakTest_Category(Description);    Use this to split your tests into groups. Not nestable.
// YakTest_Assert(Test, Name);       Define your tests. These are executed right away, the results are stored for later use.
//
//

#include <yak_memory.h>
EXTERN_C_START
#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOFLOAT
#include <stb/stb_sprintf.h>
#include <stdio.h>  // TODO: Platform-specific implementations
#include <malloc.h> // TODO: Platform specific allocs
#define Yak_Alloc(item) (item*)malloc(sizeof(item))
#define Yak_FreeListItem(item, temp) \
    if (item)                        \
    {                                \
        temp = item->Next;           \
        free(item);                  \
        item = temp;                 \
    }

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

static void
YakTest_ShowResults(void)
{
    for (test* Test = YAK_GLOBAL__FirstTestSlot;
         Test;
         Test = Test->Next)
    {
        if (Test->ConditionCount > 0)
        {
            char Buffer[200];
            stbsp_sprintf(Buffer, "* %s\n", Test->Description ? Test->Description : "Base asserts");
            printf(Buffer);

            for (condition* Condition = Test->FirstCondition;
                 Condition->Next; // we stop if we don't see a next condition, this one is empty
                 Condition = Condition->Next)
            {
                // TODO: Colored text
                stbsp_sprintf(Buffer, "  - %-20s: %s\n",
                              Condition->Description,
                              Condition->Result ? "PASSED" : "\\\\FAILED//");
                //Condition->TestDescription
                printf(Buffer);
            }
        }
    }
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
EXTERN_C_END
#define YAK_TEST 1
#endif // !(YAK_TEST)