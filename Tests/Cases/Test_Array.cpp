#include <Misc/Testing.h>
#include <Container/Array.h>

DEFINE_TEST_CASE("Array push")
{
    Array<int> intArray = MakeArray<int>();
    ArrayPush(&intArray, 10);

    Test(intArray.Items != nullptr && intArray.Count == 1 && ArrayLast(intArray) == 10);

    FreeArray(&intArray); 
}

DEFINE_TEST_CASE("Array pop")
{
    Array<int> intArray = MakeArray<int>();
    ArrayPush(&intArray, 10);
    
    Test(ArrayPop(&intArray) == 10 && intArray.Items != nullptr && intArray.Count == 0);

    FreeArray(&intArray);
}

DEFINE_TEST_CASE("Iterable of array")
{
    Array<int> intArray = MakeArray<int>();
    ArrayPush(&intArray, 10);

    for (int value : IterateArray(intArray))
    {
        TestEqual(value, 10);
    }

    FreeArray(&intArray);
}

// Đoạn comment này bằng tiếng việt và code vẫn có thể compile
DEFINE_TEST_CASE("Iterable of const array")
{
    Array<int> intArray = MakeArray<int>();
    ArrayPush(&intArray, 10);

    const Array<int> constIntArray = intArray;
    for (int value : IterateArray(constIntArray))
    {
        TestEqual(value, 10);
    }

    FreeArray(&intArray);
}
