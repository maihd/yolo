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

    int Length(String target)
    {
        if (!target || target == "")
        {
            return 0;
        }
        else
        {
            return (int)strlen(target);
        }
    }

    bool IsEmpty(String target)
    {
        return StringOps::Length(target) == 0;
    }

    int Compare(String str0, String str1)
    {
        return strcmp(str0, str1);
    }

    String Format(int bufferSize, String format, ...)
    {
        ArgList argv;
        ArgListBegin(argv, format);
        String result = FormatArgv(bufferSize, format, argv);
        ArgListEnd(argv);
        return result;
    }

    String FormatArgv(int bufferSize, String format, ArgList argv)
    {
        void* buffer = malloc(bufferSize);
        return FormatArgv(buffer, bufferSize, format, argv);
    }

    String Format(void* buffer, int bufferSize, String format, ...)
    {
        ArgList argv;
        ArgListBegin(argv, format);
        String result = FormatArgv(buffer, bufferSize, format, argv);
        ArgListEnd(argv);
        return result;
    }

    String FormatArgv(void* buffer, int bufferSize, String format, ArgList argv)
    {
        vsprintf((char*)buffer, format, argv);
        return (char*)buffer;
    }

    char CharAt(String target, int index)
    {
        return target[index];
    }

    int CharCodeAt(String target, int index)
    {
        return target[index];
    }

    int IndexOf(String target, int charCode)
    {
        int length = StringOps::Length(target);
        for (int i = 0; i < length; i++)
        {
            if (StringOps::CharCodeAt(target, i) == charCode)
            {
                return i;
            }
        }

        return -1;
    }

    int IndexOf(String target, String substring)
    {
        int substringLength = StringOps::Length(substring);
        for (int i = 0, n = StringOps::Length(target) - substringLength; i < n; i++)
        {
            if (strncmp(target + i, substring, (size_t)substringLength) == 0)
            {
                return i;
            }
        }

        return -1;
    }

    int LastIndexOf(String target, int charCode)
    {
        int length = StringOps::Length(target);
        for (int i = length - 1; i > -1; i--)
        {
            if (StringOps::CharCodeAt(target, i) == charCode)
            {
                return i;
            }
        }

        return -1;
    }

    int LastIndexOf(String target, String substring)
    {
        int substringLength = StringOps::Length(substring);
        for (int i = StringOps::Length(target) - substringLength - 1; i > -1; i--)
        {
            if (strncmp(target + i, substring, (size_t)substringLength) == 0)
            {
                return i;
            }
        }

        return -1;
    }

    String SubString(String source, int start, int end)
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
            int substringLength = end - start;
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