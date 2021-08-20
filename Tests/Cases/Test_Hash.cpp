#include <Misc/Testing.h>

#include <System/Core.h>
#include <Text/String.h>

DEFINE_TEST_CASE("CalcHash32")
{
    
}

DEFINE_TEST_CASE("CalcHash64")
{
    
}

DEFINE_TEST_CASE("ConstHash32")
{
    TestEqual(ConstHash32("Hello world"), CalcHash32("Hello world"));
}

DEFINE_TEST_CASE("ConstHash64")
{
    TestEqual(ConstHash64("Hello world"), CalcHash64("Hello world"));
}
