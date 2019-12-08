#include <Yolo/String.h>
#include <Yolo/HashTable.h>

namespace StringOps
{
    static HashTable<String> internedStrings = HashTableOps::New<String>(64);

    String Intern(String source)
    {
        uint64 hash = CalcHash64(source);

        return Intern(hash, source);
    }

    String Intern(uint64 hash, String source)
    {
        String result;
        if (HashTableOps::TryGetValue(internedStrings, hash, &result))
        {
            return result;
        }
        else
        {
            String duplicatedString = StringOps::From(source);
            HashTableOps::SetValue(&internedStrings, hash, duplicatedString);
            return duplicatedString;
        }
    }
}
