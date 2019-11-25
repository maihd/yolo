#define _CRT_SECURE_NO_WARNINGS

#include <Yolo/String.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

namespace String
{
    constexpr uint64 ConstRandom(uint64 seed = 0)
    {
        string time = __TIME__;

        uint64 timeNumber = 0;
        timeNumber |= (uint64)time[0] << (uint64)(7 * 8);
        timeNumber |= (uint64)time[1] << (uint64)(6 * 8);
        timeNumber |= (uint64)time[2] << (uint64)(5 * 8);
        timeNumber |= (uint64)time[3] << (uint64)(4 * 8);
        timeNumber |= (uint64)time[4] << (uint64)(3 * 8);
        timeNumber |= (uint64)time[5] << (uint64)(2 * 8);
        timeNumber |= (uint64)time[6] << (uint64)(1 * 8);
        timeNumber |= (uint64)time[7] << (uint64)(0 * 8);

        return seed ^ timeNumber;
    }

    struct StringMeta
    {
        int       size;
        int       length;
        int       memref;
        uint64    memtag;
    };

    constexpr uint64 MEMORY_TAG   = ConstHash("STRING_MEMORY_TAG", ConstRandom(0xbf6929f592082ce9ULL));

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

    static StringMeta* GetStringMeta(string target)
    {
        return target && target != Const::EMPTY_STRING ? (StringMeta*)(target - sizeof(StringMeta)) : NULL;
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

    string From(void* buffer, string source)
    {
        int length = Length(source);
        StringMeta* meta = CreateStringMeta(buffer, length + 1);
        meta->length = length;
        strcpy((char*)meta + sizeof(StringMeta), source);
        return (char*)meta + sizeof(StringMeta);
    }

    string From(void* buffer, int bufferSize, string source)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = Length(source);
        strcpy((char*)meta + sizeof(StringMeta), source);
        return (char*)meta + sizeof(StringMeta);
    }

    void Free(string target)
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

    bool HasMeta(string target)
    {
        StringMeta* meta = GetStringMeta(target);
        return meta && meta->memtag == MEMORY_TAG;
    }

    bool IsWeakRef(string target)
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

    bool IsManaged(string target)
    {
        StringMeta* meta = GetStringMeta(target);
        if (meta)
        {
            return meta->memtag == MEMORY_TAG && meta->memref > 0;
        }

        return false;
    }

    
    int Length(string target)
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

    int Compare(string str0, string str1)
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
        meta->length = (int)vsprintf((char*)meta + sizeof(StringMeta), format, argv);
        return (char*)meta + sizeof(StringMeta);
    }

    string Format(void* buffer, int bufferSize, string format, ...)
    {
        va_list argv;
        va_start(argv, format);
        string result = FormatArgv(buffer, bufferSize, format, argv);
        va_end(argv);
        return result;
    }

    string FormatArgv(void* buffer, int bufferSize, string format, va_list argv)
    {
        StringMeta* meta = CreateStringMeta(buffer, bufferSize);
        meta->length = (int)vsprintf((char*)meta + sizeof(StringMeta), format, argv);
        return (char*)meta + sizeof(StringMeta);
    }

    uint64 Hash(string target, uint64 seed)
    {
        uint32 length = (uint32)Length(target);
        if (length == 0)
        {
            return seed;
        }

        uint64 h = seed;

        const uint32 l = length;
        const uint32 n = (l >> 3) << 3;

        for (uint32 i = 0; i < n; i += 8)
        {
            uint64 b0 = target[i + 0];
            uint64 b1 = target[i + 1];
            uint64 b2 = target[i + 2];
            uint64 b3 = target[i + 3];
            uint64 b4 = target[i + 4];
            uint64 b5 = target[i + 5];
            uint64 b6 = target[i + 6];
            uint64 b7 = target[i + 7];
#if CPU_LITTLE_ENDIAN
            uint64 k = (b7 << 56) | (b6 << 48) | (b5 << 40) | (b4 << 32) | (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0);
#else
            uint64 k = (b0 << 56) | (b1 << 48) | (b2 << 40) | (b3 << 32) | (b4 << 24) | (b5 << 16) | (b6 << 8) | (b7 << 0);
#endif

            k ^= (k << 12);
            k ^= (k >> 47);
            k ^= (k << 25);

            h ^= k;
        }

        switch (l & 7)
        {
        case 7: h ^= uint64((target + n)[6]) << 48;   /* fall through */
        case 6: h ^= uint64((target + n)[5]) << 40;   /* fall through */
        case 5: h ^= uint64((target + n)[4]) << 32;   /* fall through */
        case 4: h ^= uint64((target + n)[3]) << 24;   /* fall through */
        case 3: h ^= uint64((target + n)[2]) << 16;   /* fall through */
        case 2: h ^= uint64((target + n)[1]) <<  8;   /* fall through */
        case 1: h ^= uint64((target + n)[0]) <<  0;   /* fall through */
        };

        h ^= (h << 12);
        h ^= (h >> 47);
        h ^= (h << 25);

        return h;
    }
}