#include <yak_defines.h>
#include <yak_test.h>
#include <yak_memory.h>
int
main()
{
    YakTest_Assert(true, "Hello world");
    
    YakTest_ShowResults();
    return (0);
}