#ifndef __TESTING__
#define __TESTING__

struct TestCase
{
    using           TestFunc = void(*)();

    const char*     Name;
    TestFunc        Func;
    TestCase*       Next;

                    TestCase(const char* name, const TestFunc func);
    
    static bool     ShouldDebugBreak();
};

#ifndef DEBUG_BREAK
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() ((void)0)
#endif
#endif

#ifndef _CONCAT
#define _CONCAT(a, b)       _CONCAT_IMPL(a, b)
#define _CONCAT_IMPL(a, b)  a ## b
#endif

#ifndef _SYMBOL
#define _SYMBOL(name)       _CONCAT(name, __LINE__)
#endif

#define DEFINE_TEST_CASE(name)                                          \
    static void _SYMBOL(TestFunc)();                                    \
    static const TestCase _SYMBOL(TEST_CASE)(name, _SYMBOL(TestFunc));  \
    static void _SYMBOL(TestFunc)()

#define Test(condition)                                                 \
    do {                                                                \
        if (!(condition) && TestCase::ShouldDebugBreak())               \
        {                                                               \
            DEBUG_BREAK();                                              \
        }                                                               \
    } while (false)

#define TestEqual(expected, actual)                                     \
    do {                                                                \
        if ((expected) != (actual) && TestCase::ShouldDebugBreak())     \
        {                                                               \
            DEBUG_BREAK();                                              \
        }                                                               \
    } while (false)

#define TestNotEqual(expected, actual)                                  \
    do {                                                                \
        if ((expected) == (actual) && TestCase::ShouldDebugBreak())     \
        {                                                               \
            DEBUG_BREAK();                                              \
        }                                                               \
    } while (false)

#endif

#ifdef TEST_RUNNER

#include <Text/String.h>

#if defined(_MSC_VER)
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#define IS_DEBUGGING() IsDebuggerPresent()
#else
#define IS_DEBUGGING() false
#endif

static TestCase*    gTestCases          = nullptr;
static int          gTestCasesCount     = 0;
static bool         gEnableDebugBreak   = true;

TestCase::TestCase(const char* name, const TestFunc func)
    : Name(name)
    , Func(func)
    , Next(gTestCases)
{
    DebugAssert(name != nullptr && strlen(name), "Test case should have an name");
    DebugAssert(func != nullptr, "Test case must have an executor");

    gTestCases = this;
    gTestCasesCount++;
}

bool TestCase::ShouldDebugBreak()
{
    return IS_DEBUGGING() && gEnableDebugBreak;
}

static void EnableDebugBreak(bool enable)
{
    gEnableDebugBreak = enable;
}

static void RunAllTestCases()
{
    for (TestCase* test = gTestCases; test != nullptr; test = test->Next)
    {
        // Test should make sure there is no memory leaks in the system
        MemoryTracker memoryTracker;

        // Run test function
        test->Func();
    }
}

#endif
