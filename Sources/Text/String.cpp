#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Math/Math.h>
#include <Text/String.h>
#include <System/Memory.h>
#include <Container/HashTable.h>

String MakeString(void* buffer, I32 bufferSize)
{
    DebugAssert(buffer != nullptr, "Invalid buffer");
    DebugAssert(bufferSize > 0, "Invalid bufferSize, which must be > 0");

    return { (char*)buffer, 0, bufferSize, true, false };
}

String MakeString(void* buffer, I32 bufferSize, I32 length)
{
    DebugAssert(buffer != nullptr, "Invalid buffer");
    DebugAssert(bufferSize > 0, "Invalid bufferSize, which must be > 0");

    return { (char*)buffer, length, bufferSize, true, false };
}

String MakeString(void* buffer, I32 bufferSize, StringView source)
{
    DebugAssert(buffer != nullptr, "Invalid buffer");
    DebugAssert(bufferSize > 0, "Invalid bufferSize, which must be > 0");
    DebugAssert(bufferSize >= source.Length, "Invalid bufferSize, which must be > source.Length");

    char* content = (char*)buffer;
    memcpy(content, source.Buffer, source.Length + 1);

    return { content, 0, bufferSize, true, false };
}

String SaveString(String source)
{
    if (source.IsConst)
    {
        return source;
    }
    else
    {
        char* buffer = (char*)MemoryAlloc(source.Alloced);
        memcpy(buffer, source.Buffer, source.Length + 1);

        return { buffer, source.Length, source.Alloced, true, false };
    }
}

String SaveString(StringView source)
{
    if (source.IsConst)
    {
        return { source.Buffer, source.Length, 0, false, true };
    }

    int length = source.Length >= 0 ? source.Length : (int)strlen(source.Buffer);
    char* buffer = (char*)MemoryAlloc(length + 1);
    memcpy(buffer, source.Buffer, length + 1);

    return { buffer, length, length + 1, true, false };
}

void FreeString(String* source)
{
    DebugAssert(source != nullptr, "Attempting free string data from nullptr.");

    if (source->IsOwned)
    {
        MemoryFree((void*)source->Buffer);
        *source = String();
    }
}

I32 StringCompare(StringView str0, StringView str1)
{
    return strncmp(str0.Buffer, str1.Buffer, min(str0.Length, str1.Length));
}

bool StringEquals(StringView str0, StringView str1)
{
    return str0.Length == str1.Length && memcmp(str0.Buffer, str1.Buffer, str0.Length) == 0;
}

bool StringNotEquals(StringView str0, StringView str1)
{
    return str0.Length != str1.Length || memcmp(str0.Buffer, str1.Buffer, str0.Length) != 0;
}

String StringFormat(I32 bufferSize, StringView format, ...)
{
    ArgList argv;
    ArgListBegin(argv, format);
    String result = StringFormatArgv(bufferSize, format, argv);
    ArgListEnd(argv);
    return result;
}

String StringFormatArgv(I32 bufferSize, StringView format, ArgList argv)
{
    void* buffer = MemoryAlloc(bufferSize);
    return StringFormatArgv(buffer, bufferSize, format, argv);
}

String StringFormat(void* buffer, I32 bufferSize, StringView format, ...)
{
    ArgList argv;
    ArgListBegin(argv, format);
    String result = StringFormatArgv(buffer, bufferSize, format, argv);
    ArgListEnd(argv);
    return result;
}

String StringFormatArgv(void* buffer, I32 bufferSize, StringView format, ArgList argv)
{
    int length = vsnprintf((char*)buffer, bufferSize, format.Buffer, argv);
    if (length < -1)
    {
        return { "", 0, 0, false, true };
    }

    return { (char*)buffer, length, bufferSize, false, false };
}

I32 StringIndexOf(StringView target, int charCode)
{
    for (int i = 0; i < target.Length; i++)
    {
        if (target.Buffer[i] == charCode)
        {
            return i;
        }
    }

    return -1;
}

int StringIndexOf(StringView target, StringView substring)
{
    for (int i = 0, n = target.Length - substring.Length; i < n; i++)
    {
        if (strncmp(&target.Buffer[i], substring.Buffer, (size_t)substring.Length) == 0)
        {
            return i;
        }
    }

    return -1;
}

int StringLastIndexOf(StringView target, int charCode)
{
    for (int i = target.Length - 1; i > -1; i--)
    {
        if (target.Buffer[i] == charCode)
        {
            return i;
        }
    }

    return -1;
}

int StringLastIndexOf(StringView target, StringView substring)
{
    for (int i = target.Length - substring.Length - 1; i > -1; i--)
    {
        if (strncmp(target.Buffer + i, substring.Buffer, (size_t)substring.Length) == 0)
        {
            return i;
        }
    }

    return -1;
}

String SubString(StringView source, int start, int end)
{
    if (start < 0 || start >= source.Length)
    {
        return String();
    }

    if (end < 0)
    {
        return { source.Buffer + start, source.Length - start, false, 0, source.IsConst };
    }
    else
    {
        I32 substringLength = end - start;
        if (substringLength <= 0)
        {
            return String();
        }

        char* content = (char*)MemoryAlloc(substringLength + 1);
        memcpy(content, source.Buffer, substringLength + 1);

        return { content, substringLength, substringLength + 1, true, false };
    }
}
