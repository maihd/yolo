#define _CRT_SECURE_NO_WARNINGS

#include <Yolo/String.h>
#include <Yolo/Random.h>

#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <stdint.h>

namespace StringOps
{
    struct StringMeta
    {
        int       size;
        int       length;
        int       memref;
        uint64    memtag;
    };

    constexpr uint64 MEMORY_TAG = CalcHash64("STRING_MEMORY_TAG", Random::AsCompileTimeU64(0xbf6929f592082ce9ULL));

    static StringMeta* CreateStringMeta(void* buffer, int bufferSize)
    {
        StringMeta* meta = (StringMeta*)buffer;

        meta->memref = -1;
        meta->memtag = MEMORY_TAG;
        meta->size   = bufferSize - sizeof(StringMeta);

        return meta;
    }

    static StringMeta* CreateStringMeta(int bufferSize)
    {
        StringMeta* meta = (StringMeta*)malloc(bufferSize + sizeof(StringMeta));

        meta->memref = 1;
        meta->memtag = MEMORY_TAG;
        meta->size   = bufferSize;

        return meta;
    }

    static StringMeta* GetStringMeta(String target)
    {
        return target && target != Const::EMPTY_STRING ? (StringMeta*)(target - sizeof(StringMeta)) : NULL;
    }

    String From(String source)
    {
        if (IsWeakRef(source))
        {
            return source;
        }

        if (IsManaged(source))
        {
            StringMeta* meta = GetStringMeta(source);
            meta->memref++;
            return source;
        }
    
        int length = Length(source);
        if (length == 0)
        {
            return Const::EMPTY_STRING;
        }
        else
        {
            StringMeta* meta = CreateStringMeta(length + 1);
            meta->length = length;
            strcpy((char*)meta + sizeof(StringMeta), source);
            return (char*)meta + sizeof(StringMeta);
        }
    }

    String From(void* buffer, String source)
    {
        int length = Length(source);
        StringMeta* meta = CreateStringMeta(buffer, length + 1);
        meta->length = length;
        strcpy((char*)meta + sizeof(StringMeta), source);
        return (char*)meta + sizeof(StringMeta);
    }

    String From(void* buffer, int bufferSize, String source)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = Length(source);
        strcpy((char*)meta + sizeof(StringMeta), source);
        return (char*)meta + sizeof(StringMeta);
    }

    void Free(String target)
    {
        if (IsManaged(target))
        {
            StringMeta* meta = GetStringMeta(target);
            if (--meta->memref == 0)
            {
                free(meta);
            }
        }
    }

    bool HasMeta(String target)
    {
        StringMeta* meta = GetStringMeta(target);
        return meta && meta->memtag == MEMORY_TAG;
    }

    bool IsWeakRef(String target)
    {
        if (target == Const::EMPTY_STRING)
        {
            return true;
        }
        
        StringMeta* meta = GetStringMeta(target);
        if (meta)
        {
            return meta->memtag == MEMORY_TAG && meta->memref < 0;
        }

        return true;
    }

    bool IsManaged(String target)
    {
        StringMeta* meta = GetStringMeta(target);
        if (meta)
        {
            return meta->memtag == MEMORY_TAG && meta->memref > 0;
        }

        return false;
    }
    
    int Length(String target)
    {
        if (!target || target == Const::EMPTY_STRING)
        {
            return 0;
        }

        StringMeta* meta = (StringMeta*)(target - sizeof(StringMeta));
        if (meta->memtag == MEMORY_TAG)
        {
            return meta->length;
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
        va_list argv;
        va_start(argv, format);
        String result = FormatArgv(bufferSize, format, argv);
        va_end(argv);
        return result;
    }

    String FormatArgv(int bufferSize, String format, va_list argv)
    {
        StringMeta* meta = CreateStringMeta(bufferSize);
        meta->length = (int)vsprintf((char*)meta + sizeof(StringMeta), format, argv);
        return (char*)meta + sizeof(StringMeta);
    }

    String Format(void* buffer, int bufferSize, String format, ...)
    {
        va_list argv;
        va_start(argv, format);
        String result = FormatArgv(buffer, bufferSize, format, argv);
        va_end(argv);
        return result;
    }

    String FormatArgv(void* buffer, int bufferSize, String format, va_list argv)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = (int)vsprintf((char*)meta + sizeof(StringMeta), format, argv);
        return (char*)meta + sizeof(StringMeta);
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
            return Const::EMPTY_STRING;
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
                return Const::EMPTY_STRING;
            }

            StringMeta* meta = CreateStringMeta(substringLength + 1);
            char* content = (char*)(meta + 1);

            strncpy(content, source, substringLength);
            content[substringLength] = 0;

            return content;
        }
    }
}