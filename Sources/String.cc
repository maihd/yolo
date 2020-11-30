#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Yolo/Math.h>
#include <Yolo/Memory.h>
#include <Yolo/String.h>
#include <Yolo/Random.h>
#include <Yolo/HashTable.h>

String MakeString(void* buffer, int bufferSize)
{
    DebugAssert(buffer != nullptr, "Invalid buffer");
    DebugAssert(bufferSize > 0, "Invalid bufferSize, which must be > 0");

    return { (char*)buffer, 0, bufferSize, true, false };
}

String MakeString(void* buffer, int bufferSize, const char* source)
{
    DebugAssert(buffer != nullptr, "Invalid buffer");
    DebugAssert(bufferSize > 0, "Invalid bufferSize, which must be > 0");

    char* content = (char*)buffer;
    if (source)
    {
        strcpy(content, source);
    }

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

String SaveString(const char* source)
{
    int length = (int)strlen(source);
    char* buffer = (char*)MemoryAlloc(length + 1);
    memcpy(buffer, source, length + 1);

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

bool IsStringEmpty(String target)
{
    return target.Length == 0;
}

I32 StringCompare(String str0, String str1)
{
    return strncmp(str0.Buffer, str1.Buffer, min(str0.Length, str1.Length));
}

String StringFormat(I32 bufferSize, const char* format, ...)
{
    ArgList argv;
    ArgListBegin(argv, format);
    String result = StringFormatArgv(bufferSize, format, argv);
    ArgListEnd(argv);
    return result;
}

String StringFormatArgv(I32 bufferSize, const char* format, ArgList argv)
{
    void* buffer = MemoryAlloc(bufferSize);
    return StringFormatArgv(buffer, bufferSize, format, argv);
}

String StringFormat(void* buffer, I32 bufferSize, const char* format, ...)
{
    ArgList argv;
    ArgListBegin(argv, format);
    String result = StringFormatArgv(buffer, bufferSize, format, argv);
    ArgListEnd(argv);
    return result;
}

String StringFormatArgv(void* buffer, I32 bufferSize, const char* format, ArgList argv)
{
    int length = vsnprintf((char*)buffer, bufferSize, format, argv);
    if (length < -1)
    {
        return { "", 0, 0, false, true };
    }

    return { (char*)buffer, length, bufferSize, false, false };
}

I32 StringIndexOf(String target, I32 charCode)
{
    I32 length = target.Length;
    for (I32 i = 0; i < length; i++)
    {
        if (target.Buffer[i] == charCode)
        {
            return i;
        }
    }

    return -1;
}

I32 StringIndexOf(String target, String substring)
{
    I32 substringLength = substring.Length;
    for (I32 i = 0, n = target.Length - substringLength; i < n; i++)
    {
        if (strncmp(&target.Buffer[i], substring.Buffer, (size_t)substringLength) == 0)
        {
            return i;
        }
    }

    return -1;
}

I32 StringLastIndexOf(String target, I32 charCode)
{
    I32 length = target.Length;
    for (I32 i = length - 1; i > -1; i--)
    {
        if (target.Buffer[i] == charCode)
        {
            return i;
        }
    }

    return -1;
}

I32 StringLastIndexOf(String target, String substring)
{
    I32 substringLength = substring.Length;
    for (I32 i = target.Length - substringLength - 1; i > -1; i--)
    {
        if (strncmp(target.Buffer + i, substring.Buffer, (size_t)substringLength) == 0)
        {
            return i;
        }
    }

    return -1;
}

String SubString(String source, I32 start, I32 end)
{
    if (start < 0)
    {
        return String();
    }

    if (end < 0)
    {
        return { source.Buffer + start, source.Length - start, source.IsOwned, source.Alloced, source.IsConst };
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
