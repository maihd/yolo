#pragma once

#include <Yolo/Types.h>
#include <Yolo/Hash.h>

namespace StringOps
{
    String From(String source);
    String From(void* buffer, String source);
    String From(void* buffer, int bufferSize, String source);

    String Format(int bufferSize, String format, ...);
    String FormatArgv(int bufferSize, String format, va_list argv);

    String Format(void* buffer, int bufferSize, String format, ...);
    String FormatArgv(void* buffer, int bufferSize, String format, va_list argv);

    void   Free(String target);
    bool   HasMeta(String target);
    bool   IsWeakRef(String target);
    bool   IsManaged(String target);

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
}

template <>
inline uint64 CalcHash64(String x, uint64 seed)
{
    return CalcHash64(&x, StringOps::Length(x), seed);
}

