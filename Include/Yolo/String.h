#pragma once

#include <string.h>
#include <Yolo/Types.h>

// -------------------------------
// Main functions
// -------------------------------

String MakeString(void* buffer, int bufferSize);
String MakeString(void* buffer, int bufferSize, const char* source);

String SaveString(String source);
String SaveString(const char* source);

void   FreeString(String* source);

String StringFormat(I32 bufferSize, const char* format, ...);
String StringFormatArgv(I32 bufferSize, const char* format, ArgList argv);

String StringFormat(void* buffer, I32 bufferSize, const char* format, ...);
String StringFormatArgv(void* buffer, I32 bufferSize, const char* format, ArgList argv);

bool   IsStringEmpty(String target);

I32    StringCompare(String str0, String str1);

I32    StringIndexOf(String target, I32 charCode);
I32    StringIndexOf(String target, String substring);

I32    StringLastIndexOf(String target, I32 charCode);
I32    StringLastIndexOf(String target, String substring);

String SubString(String source, I32 start, I32 end = -1);

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
    result.IsStatic = true;
    result.Alloced  = 0;
    return result;
}

inline String RefString(const char* source)
{
    return { source, (int)strlen(source), 0, false, false };
}

inline String RefString(const char* source, int length)
{
    return { source, length, 0, false, false };
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
