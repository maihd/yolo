#pragma once

#include <Yolo/Types.h>

namespace StringOps
{
    String Format(I32 bufferSize, String format, ...);
    String FormatArgv(I32 bufferSize, String format, ArgList argv);

    String Format(void* buffer, I32 bufferSize, String format, ...);
    String FormatArgv(void* buffer, I32 bufferSize, String format, ArgList argv);

    I32    Length(String target);
    bool   IsEmpty(String target);

    I32    Compare(String str0, String str1);

    char   CharAt(String target, I32 index);
    I32    CharCodeAt(String target, I32 index);

    I32    IndexOf(String target, I32 charCode);
    I32    IndexOf(String target, String substring);

    I32    LastIndexOf(String target, I32 charCode);
    I32    LastIndexOf(String target, String substring);

    String SubString(String source, I32 start, I32 end = -1);

    String Intern(String source);
    String Intern(U64 hash, String source);
    String InternNoAllocation(U64 hash, String source);

    template <I32 length>
    inline String Static(const char(&source)[length])
    {
        if (length == 0)
        {
            return "";
        }

        U64 hash = ConstHash64<length>(source);
        return InternNoAllocation(hash, source);
    }
}

inline U32 CalcHash32(String string, U32 seed = 0)
{
    return CalcHash32(string.Buffer, string.Length, seed);
}

inline U64 CalcHash64(String string, U64 seed = 0)
{
    return CalcHash64(string.Buffer, string.Length, seed);
}

inline bool operator==(String a, String b)
{
    return StringOps::Compare(a, b) == 0;
}

inline bool operator!=(String a, String b)
{
    return StringOps::Compare(a, b) != 0;
}
