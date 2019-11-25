#pragma once

#include <Yolo/Types.h>

namespace String
{
    string From(string source);
    string From(void* buffer, string source);
    string From(void* buffer, int bufferSize, string source);

    string Format(int bufferSize, string format, ...);
    string FormatArgv(int bufferSize, string format, va_list argv);

    string Format(void* buffer, int bufferSize, string format, ...);
    string FormatArgv(void* buffer, int bufferSize, string format, va_list argv);

    void   Free(string target);
    bool   HasMeta(string target);
    bool   IsWeakRef(string target);
    bool   IsManaged(string target);

    int    Length(string target);

    int    Compare(string str0, string str1);

    uint64 Hash(string target, uint64 seed = 0);

    constexpr uint64 ConstHash(string target, uint64 seed = 0)
    {
        uint32 length = 0;
        while (target[length])
        {
            length++;
        }
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