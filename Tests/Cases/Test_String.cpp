#include <Misc/Testing.h>

#include <Text/String.h>
#include <System/Memory.h>

DEFINE_TEST_CASE("Construct const string")
{
    String string = ConstString("Hello World");
    Test(string.IsConst && StringCompare(string, "Hello World") == 0);
}

DEFINE_TEST_CASE("Construct const StringView by ConstString functions")
{
    StringView string = ConstString("Hello World");
    Test(string.IsConst && StringCompare(string, "Hello World") == 0);
}

DEFINE_TEST_CASE("Construct const StringView with string literal")
{
    StringView string = "Hello World";
    Test(string.IsConst && StringCompare(string, "Hello World") == 0);
}

DEFINE_TEST_CASE("Construct const StringView with static char buffer")
{
    StringView stringView = "Hello World";
    Test(stringView.IsConst && StringCompare(stringView, "Hello World") == 0);

    static char buffer[1024] = "Hello World";
    StringView stringView2 = buffer;
    Test(!stringView2.IsConst && StringCompare(stringView2, "Hello World") == 0);
}

DEFINE_TEST_CASE("Test two strings are equal")
{
    Test(StringEquals("Hello World", "Hello World"));
}

DEFINE_TEST_CASE("Test two strings are not equal")
{
    Test(StringNotEquals("Hello World", "Hello Kitty"));
}

DEFINE_TEST_CASE("Test two strings are equal with operator")
{
    Test(ConstString("Hello World") == ConstString("Hello World"));
}

DEFINE_TEST_CASE("Test two strings are not equal with operator")
{
    Test(ConstString("Hello World") != ConstString("Hello Kitty"));
}
