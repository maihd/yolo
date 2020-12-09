#include <Yolo/Array.h>

void TestArray()
{
    Array<int> intArray = MakeArray<int>();

    ArrayPush(&intArray, 10);

    FreeArray(&intArray); 
}