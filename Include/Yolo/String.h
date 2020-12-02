#pragma once

#include <string.h>
#include <Yolo/Core.h>

// -----------------------------------
// Main functions
// -----------------------------------

String MakeString(void* buffer, int bufferSize);
String MakeString(void* buffer, int bufferSize, StringView source);

String SaveString(StringView source);

void   FreeString(String* source);

String StringFormat(I32 bufferSize, StringView format, ...);
String StringFormatArgv(I32 bufferSize, StringView format, ArgList argv);

String StringFormat(void* buffer, I32 bufferSize, StringView format, ...);
String StringFormatArgv(void* buffer, I32 bufferSize, StringView format, ArgList argv);

I32    StringCompare(StringView str0, StringView str1);

I32    StringIndexOf(StringView target, I32 charCode);
I32    StringIndexOf(StringView target, StringView substring);

I32    StringLastIndexOf(StringView target, I32 charCode);
I32    StringLastIndexOf(StringView target, StringView substring);

String SubString(StringView source, I32 start, I32 end = -1);

// -----------------------------------
// Constructor functions
// -----------------------------------

template <int LENGTH>
constexpr String ConstString(const char(&buffer)[LENGTH])
{
    String result;
    result.Buffer   = buffer;
    result.Length   = LENGTH;
    result.IsOwned  = false;
    result.IsConst  = true;
    result.Alloced  = 0;
    return result;
}

inline String RefString(StringView source)
{
    return { source.Buffer, source.Length, 0, false, false };
}

inline String RefString(const char* source, int length)
{
    return { source, length, 0, false, false };
}

// -----------------------------------
// Inline functions
// -----------------------------------

inline bool IsStringEmpty(String target)
{
    return target.Length == 0;
}

inline bool IsStringEmpty(StringView target)
{
    return target.Length == 0;
}

inline bool IsStringEmpty(const char* target)
{
    return target[0] == '\0';
}

// -----------------------------------
// Utils functions
// -----------------------------------

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
    return StringCompare(a, b) == 0;
}

inline bool operator!=(String a, String b)
{
    return StringCompare(a, b) != 0;
}

inline bool operator==(StringView a, StringView b)
{
    return StringCompare(a, b) == 0;
}

inline bool operator!=(StringView a, StringView b)
{
    return StringCompare(a, b) != 0;
}
