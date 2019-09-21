#if !defined(YAK_TEST)

///
/// API
///
// YTestShowResults();              // Call after you define your tests.
// YTestCleanup();                  // Call if you wish to reset your tests with new data.
// Y_TEST_CATEGORY(Description);    Use this to split your tests into groups. Not nestable.
// Y_ASSERT(Test, Name);            Define your tests. These are executed right away, the results are stored for later use.
//
//

#include <malloc.h>
#include <stdio.h> // TODO STB
// TODO: Platform specific allocs
#define ALLOC(item) (item*)malloc(sizeof(item))
#define FREELISTITEM(item, temp) \
    if (item)                    \
    {                            \
        temp = item->Next;       \
        free(item);              \
        item = temp;             \
    }

struct condition;
struct test;

inline void GetNextTestSlot(const char* Name);
inline void PopulateConditionAndAdvance(const char* TestText, size_t TestValue, const char* Name);
static condition* InitializeTestGlobals();

static test* GlobalFirstTestSlot;
static test* GlobalCurrentTestSlot;
static condition* GlobalCurrentCondition = InitializeTestGlobals();

#define Y_TEST_CATEGORY(Name) GetNextTestSlot(Name)
#define Y_ASSERT(Test, Name) PopulateConditionAndAdvance(#Test, (size_t)(Test), (Name))

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
GetCondition()
{
    condition* Result = ALLOC(condition);

    Result->Next = 0;
    Result->Description = 0;
    Result->TestDescription = 0;
    Result->Result = 0;

    return (Result);
}

inline test*
GetTestSlot(const char* Name)
{
    test* Result = ALLOC(test);

    Result->Description = Name;
    Result->ConditionCount = 0;
    Result->FirstCondition = 0;
    Result->Next = 0;

    return (Result);
}

static condition*
InitializeTestGlobals()
{
    GlobalFirstTestSlot = GlobalCurrentTestSlot = GetTestSlot(0);
    condition *FirstCondition = GlobalFirstTestSlot->FirstCondition = GetCondition();

    return (FirstCondition);
}

inline void
GetNextTestSlot(const char* Name)
{

    test *NextTest = GetTestSlot(Name);
    
    GlobalCurrentTestSlot->Next = NextTest;
    GlobalCurrentTestSlot = GlobalCurrentTestSlot->Next;
    
    GlobalCurrentCondition = NextTest->FirstCondition = GetCondition();
}

inline void
PopulateConditionAndAdvance(const char* TestText, size_t TestValue, const char* Name)
{
    GlobalCurrentTestSlot->ConditionCount++;
    
    GlobalCurrentCondition->Description = Name;
    GlobalCurrentCondition->TestDescription = TestText;
    GlobalCurrentCondition->Result = (unsigned)TestValue;
    GlobalCurrentCondition->Next = GetCondition();

    GlobalCurrentCondition = GlobalCurrentCondition->Next;
}

static void
YTestShowResults(void)
{
    for (test* Test = GlobalFirstTestSlot;
         Test;
         Test = Test->Next)
    {
        if (Test->ConditionCount > 0)
        {
            printf("* %s", Test->Description ? Test->Description : "Base asserts");
            printf("\n");

            for (condition* Condition = Test->FirstCondition;
                 Condition->Next; // we stop if we don't see a next condition, this one is empty
                 Condition = Condition->Next)
            {
                // TODO: Colored text
                printf("\t- %-30s: %s (test input: '%s')\n",
                       Condition->Description,
                       Condition->Result ? "PASSED" : "FAILED",
                       Condition->TestDescription);
            }
        }
    }
}

/** Free memory of each condition and each test */
static void
YTestCleanup(void)
{
    // Free tests linked list
    test* CurrentSlot = GlobalFirstTestSlot->Next;
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
                    FREELISTITEM(CurrentCondition, NextCondition);
                }
                else
                {
                    break;
                }
            }

            FREELISTITEM(CurrentSlot, NextSlot);
        }
        else
        {
            break;
        }
    }

    // Reinitialize the globals
    GlobalCurrentCondition = InitializeTestGlobals();
}

#define YAK_TEST 1
#endif // !(YAK_TEST)