#pragma once

#include <Yolo/Types.h>
#include <Yolo/Hash.h>

namespace String
{
    string From(string source);
    string From(void* buffer, string source);
    string From(void* buffer, int bufferSize, string source);

    string Format(int bufferSize, string format, ...);
    string FormatArgv(int bufferSize, string format, va_list argv);

    string Format(void* buffer, int bufferSize, string format, ...);
    string FormatArgv(void* buffer, int bufferSize, string format, va_list argv);

    void   Free(string target);
    bool   HasMeta(string target);
    bool   IsWeakRef(string target);
    bool   IsManaged(string target);

    int    Length(string target);
    bool   IsEmpty(string target);

    int    Compare(string str0, string str1);

    char   CharAt(string target, int index);
    int    CharCodeAt(string target, int index);

    int    IndexOf(string target, int charCode);
    int    IndexOf(string target, string substring);

    int    LastIndexOf(string target, int charCode);
    int    LastIndexOf(string target, string substring);

    string SubString(string source, int start, int end = -1);
}

template <>
inline uint64 CalcHash64(string x, uint64 seed)
{
    return CalcHash64(&x, String::Length(x), seed);
}

