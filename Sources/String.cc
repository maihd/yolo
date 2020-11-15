#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Yolo/Math.h>
#include <Yolo/String.h>
#include <Yolo/Random.h>
#include <Yolo/HashTable.h>

namespace StringOps
{   
    static HashTable<String> internedStrings = HashTableOps::New<String>(64);

    String SaveString(String source)
    {
        if (source.isStatic)
        {
            return source;
        }
        else
        {
            return String(_strdup(source.buffer), source.length, true);
        }
    }

    String Intern(String source)
    {
        U64 hash = CalcHash64(source);

        return Intern(hash, source);
    }

    String Intern(U64 hash, String source)
    {
        if (Length(source) == 0)
        {
            return "";
        }

        String result;
        if (HashTableOps::TryGetValue(internedStrings, hash, &result))
        {
            return result;
        }
        else
        {
            String duplicatedString = SaveString(source);
            HashTableOps::SetValue(&internedStrings, hash, duplicatedString);
            return duplicatedString;
        }
    }

    String InternNoAllocation(U64 hash, String source)
    {
        if (Length(source) == 0)
        {
            return "";
        }

        String result;
        if (HashTableOps::TryGetValue(internedStrings, hash, &result))
        {
            return result;
        }
        else
        {
            HashTableOps::SetValue(&internedStrings, hash, source);
            return source;
        }
    }

    I32 Length(String target)
    {
        return target.length;
    }

    bool IsEmpty(String target)
    {
        return target.length == 0;
    }

    I32 Compare(String str0, String str1)
    {
        return strncmp(str0.buffer, str1.buffer, min(str0.length, str1.length));
    }

    String Format(I32 bufferSize, String format, ...)
    {
        ArgList argv;
        ArgListBegin(argv, format);
        String result = FormatArgv(bufferSize, format, argv);
        ArgListEnd(argv);
        return result;
    }

    String FormatArgv(I32 bufferSize, String format, ArgList argv)
    {
        void* buffer = malloc(bufferSize);
        return FormatArgv(buffer, bufferSize, format, argv);
    }

    String Format(void* buffer, I32 bufferSize, String format, ...)
    {
        ArgList argv;
        ArgListBegin(argv, format);
        String result = FormatArgv(buffer, bufferSize, format, argv);
        ArgListEnd(argv);
        return result;
    }

    String FormatArgv(void* buffer, I32 bufferSize, String format, ArgList argv)
    {
        vsnprintf((char*)buffer, bufferSize, format.buffer, argv);
        return String((char*)buffer, strlen((char*)buffer), false);
    }

    char CharAt(String target, I32 index)
    {
        return target.buffer[index];
    }

    I32 CharCodeAt(String target, I32 index)
    {
        return target.buffer[index];
    }

    I32 IndexOf(String target, I32 charCode)
    {
        I32 length = StringOps::Length(target);
        for (I32 i = 0; i < length; i++)
        {
            if (StringOps::CharCodeAt(target, i) == charCode)
            {
                return i;
            }
        }

        return -1;
    }

    I32 IndexOf(String target, String substring)
    {
        I32 substringLength = substring.length;
        for (I32 i = 0, n = target.length - substringLength; i < n; i++)
        {
            if (strncmp(target.buffer + i, substring.buffer, (size_t)substringLength) == 0)
            {
                return i;
            }
        }

        return -1;
    }

    I32 LastIndexOf(String target, I32 charCode)
    {
        I32 length = StringOps::Length(target);
        for (I32 i = length - 1; i > -1; i--)
        {
            if (StringOps::CharCodeAt(target, i) == charCode)
            {
                return i;
            }
        }

        return -1;
    }

    I32 LastIndexOf(String target, String substring)
    {
        I32 substringLength = substring.length;
        for (I32 i = StringOps::Length(target) - substringLength - 1; i > -1; i--)
        {
            if (strncmp(target.buffer + i, substring.buffer, (size_t)substringLength) == 0)
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
            return String(source.buffer + start, source.length - start, source.isOwned, source.isStatic);
        }
        else
        {
            I32 substringLength = end - start;
            if (substringLength <= 0)
            {
                return String();
            }

            char* content = (char*)malloc(substringLength + 1);

            strncpy(content, source.buffer, substringLength);
            content[substringLength] = 0;

            return String(content, substringLength, true);
        }
    }
}