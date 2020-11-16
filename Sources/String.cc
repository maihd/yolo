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
        if (source.IsStatic)
        {
            return source;
        }
        else
        {
            return String(_strdup(source.Buffer), source.Length, true);
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
        return target.Length;
    }

    bool IsEmpty(String target)
    {
        return target.Length == 0;
    }

    I32 Compare(String str0, String str1)
    {
        return strncmp(str0.Buffer, str1.Buffer, min(str0.Length, str1.Length));
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
        int length = vsnprintf((char*)buffer, bufferSize, format.Buffer, argv);
        if (length < -1)
        {
            return String();
        }

        return String((char*)buffer, length, false);
    }

    char CharAt(String target, I32 index)
    {
        return target.Buffer[index];
    }

    I32 CharCodeAt(String target, I32 index)
    {
        return target.Buffer[index];
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
        I32 substringLength = substring.Length;
        for (I32 i = 0, n = target.Length - substringLength; i < n; i++)
        {
            if (strncmp(target.Buffer + i, substring.Buffer, (size_t)substringLength) == 0)
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
        I32 substringLength = substring.Length;
        for (I32 i = StringOps::Length(target) - substringLength - 1; i > -1; i--)
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
            return String(source.Buffer + start, source.Length - start, source.IsOwned, source.IsStatic);
        }
        else
        {
            I32 substringLength = end - start;
            if (substringLength <= 0)
            {
                return String();
            }

            char* content = (char*)malloc(substringLength + 1);

            strncpy(content, source.Buffer, substringLength);
            content[substringLength] = 0;

            return String(content, substringLength, true);
        }
    }
}