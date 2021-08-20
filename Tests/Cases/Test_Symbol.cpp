#include <Misc/Testing.h>

#include <System/Core.h>
#include <Text/String.h>

DEFINE_TEST_CASE("Construct symbol as compile-time")
{
    Symbol symbol = ConstSymbol("Yolo");
#ifndef NDEBUG
    Test(StringCompare(symbol.Name, "Yolo") == 0);
#else

#endif
}
