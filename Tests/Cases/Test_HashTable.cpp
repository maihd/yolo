#include <Misc/Testing.h>
#include <Container/HashTable.h>

DEFINE_TEST_CASE("Make new hash table")
{
    HashTable<int> intTable = MakeHashTable<int>();
    FreeHashTable(&intTable);
}
