#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <Yolo/String.h>
#include <Yolo/Random.h>
#include <Yolo/HashTable.h>

namespace StringOps
{   
    static HashTable<String> internedStrings = HashTableOps::New<String>(64);

    String SaveString(String source)
    {
        return _strdup(source);
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
        if (!target || target == "")
        {
            return 0;
        }
        else
        {
            return (I32)strlen(target);
        }
    }

    bool IsEmpty(String target)
    {
        return StringOps::Length(target) == 0;
    }

    I32 Compare(String str0, String str1)
    {
        return strcmp(str0, str1);
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
        vsprintf((char*)buffer, format, argv);
        return (char*)buffer;
    }

    char CharAt(String target, I32 index)
    {
        return target[index];
    }

    I32 CharCodeAt(String target, I32 index)
    {
        return target[index];
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
        I32 substringLength = StringOps::Length(substring);
        for (I32 i = 0, n = StringOps::Length(target) - substringLength; i < n; i++)
        {
            if (strncmp(target + i, substring, (size_t)substringLength) == 0)
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
        I32 substringLength = StringOps::Length(substring);
        for (I32 i = StringOps::Length(target) - substringLength - 1; i > -1; i--)
        {
            if (strncmp(target + i, substring, (size_t)substringLength) == 0)
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
            return "";
        }

        if (end < 0)
        {
            return source + start;
        }
        else
        {
            I32 substringLength = end - start;
            if (substringLength <= 0)
            {
                return "";
            }

            char* content = (char*)malloc(substringLength + 1);

            strncpy(content, source, substringLength);
            content[substringLength] = 0;

            return content;
        }
    }
}