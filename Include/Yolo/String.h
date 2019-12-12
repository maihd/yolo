#pragma once

#include <Yolo/Types.h>

namespace StringOps
{
    String Format(int bufferSize, String format, ...);
    String FormatArgv(int bufferSize, String format, va_list argv);

    String Format(void* buffer, int bufferSize, String format, ...);
    String FormatArgv(void* buffer, int bufferSize, String format, va_list argv);

    int    Length(String target);
    bool   IsEmpty(String target);

    int    Compare(String str0, String str1);

    char   CharAt(String target, int index);
    int    CharCodeAt(String target, int index);

    int    IndexOf(String target, int charCode);
    int    IndexOf(String target, String substring);

    int    LastIndexOf(String target, int charCode);
    int    LastIndexOf(String target, String substring);

    String SubString(String source, int start, int end = -1);

    String Intern(String source);
    String Intern(uint64 hash, String source);
    String InternNoAllocation(uint64 hash, String source);

    template <int length>
    inline String Static(const char(&source)[length])
    {
        uint64 hash = CalcHash64<length>(source);
        return InternNoAllocation(hash, source);
    }
}

template <>
inline uint64 CalcHash64(String x, uint64 seed)
{
    return CalcHash64(x, StringOps::Length(x), seed);
}

