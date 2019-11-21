#include <Yolo/String.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace String
{
    constexpr uint64_t ConstHash(string target, uint64_t _prefer)
    {
        return prefer;
    }

    struct StringMeta
    {
        int  length;
        int  memref;
        long memtag;

        int  size;
        char data[];
    };

    static string   emptyString = "";
    static uint64_t memoryTag   = ConstHash("__string_memory_tag__", 0x9000000000000009);

    static StringMeta* CreateStringMeta(void* buffer, int bufferSize)
    {
        StringMeta* meta = (StringMeta*)buffer;

        meta->memref = -1;
        meta->memtag = memoryTag;
        meta->size   = bufferSize - sizeof(StringMeta);

        return meta;
    }

    static StringMeta* CreateStringMeta(int bufferSize)
    {
        StringMeta* meta = (StringMeta*)malloc(bufferSize + sizeof(StringMeta));

        meta->memref = 1;
        meta->memtag = memoryTag;
        meta->size   = bufferSize;

        return meta;
    }

    static StringMeta* GetStringMeta(string target)
    {
        return target && target != emptyString ? (StringMeta*)(target - sizeof(StringMeta)) : NULL;
    }

    string From(string source)
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
            return emptyString;
        }
        else
        {
            StringMeta* meta = CreateStringMeta(length + 1);
            meta->length = length;
            strcpy(meta->data, source);
            return meta->data;
        }
    }

    string From(void* buffer, string source)
    {
        StringMeta* meta = CreateStringMeta(buffer, length + 1);
        meta->length = Length(source);
        strcpy(meta->data, source);
        return meta->data;
    }

    string From(void* buffer, int bufferSize, string source)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = Length(source);
        strcpy(meta->data, source);
        return meta->data;
    }

    void   Free(string target)
    {
        if (IsManaged(target))
        {
            StringMeta* meta = GetStringMeta(target);
            if (meta->memref-- == 0)
            {
                free(meta);
            }
        }
    }

    bool   IsWeakRef(string target)
    {
        if (target == emptyString)
        {
            return true;
        }
        
        SringMeta* meta = GetStringMeta(target);
        if (meta)
        {
            return meta->memtag == memoryTag && meta->memref < 0;
        }

        return true;
    }

    bool   IsManaged(string target)
    {
        SringMeta* meta = GetStringMeta(target);
        if (meta)
        {
            return meta->memtag == memoryTag && meta->memref > 0;
        }

        return false;
    }

    
    int    Length(string target)
    {
        if (!target || target == emptyString)
        {
            return 0;
        }

        StringMeta* meta = (StringMeta*)(target - sizeof(StringMeta));
        if (meta->memtag == memoryTag)
        {
            return meta->length;
        }
        else
        {
            return (int)strlen(target);
        }
    }

    int    Compare(string str0, string str1)
    {
        return strcmp(str0, str1);
    }

    string Format(int bufferSize, string format, ...)
    {
        va_list argv;
        va_start(argv, format);
        string result = FormatArgv(bufferSize, format, argv);
        va_end(argv);
        return result;
    }

    string FormatArgv(int bufferSize, string format, va_list argv)
    {
        StringMeta* meta = CreateStringMeta(bufferSize);
        meta->length = (int)vsprintf(meta->data, format, argv);
        return meta->data;
    }

    string Format(void* buffer, int bufferSize, string format, ...)
    {
        va_list argv;
        va_start(argv, format);
        string result = FormatArgv(buffer, bufferSize, format, argv);
        va_end(argv);
        return result;
    }

    string FormatArgv(void* buffer, int bufferSize, string format, ...)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = (int)vsprintf(meta->data, format, argv);
        return meta->data;
    }
}