#pragma once

#include <stdarg.h>
#include <stdint.h>

using string = const char*;

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

    uint64_t Hash(string target, uint64_t seed = 0);

    constexpr uint64_t ConstHash(string target, uint64_t seed = 0)
    {
        uint64_t h = seed;

        int length = 0;
        while (target[length])
        {
            length++;
        }

        const uint64_t m = 0xc6a4a7935bd1e995ULL;
        const uint64_t r = 47;
        const uint32_t l = length;
        const uint32_t n = (l >> 3) << 3;

        for (uint32_t i = 0; i < n; i += 8)
        {
            uint64_t b0 = target[i + 0];
            uint64_t b1 = target[i + 1];
            uint64_t b2 = target[i + 2];
            uint64_t b3 = target[i + 3];
            uint64_t b4 = target[i + 4];
            uint64_t b5 = target[i + 5];
            uint64_t b6 = target[i + 6];
            uint64_t b7 = target[i + 7];
#if CPU_LITTLE_ENDIAN
            uint64_t k = (b7 << 56) | (b6 << 48) | (b5 << 40) | (b4 << 32) | (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0);
#else
            uint64_t k = (b0 << 56) | (b1 << 48) | (b2 << 40) | (b3 << 32) | (b4 << 24) | (b5 << 16) | (b6 << 8) | (b7 << 0);
#endif

            k *= m;
            k ^= k >> r;
            k *= m;

            h ^= k;
            h *= m;
        }

        switch (l & 7)
        {
        case 7: h ^= uint64_t((target + n)[6]) << 48;            /* fall through */
        case 6: h ^= uint64_t((target + n)[5]) << 40;            /* fall through */
        case 5: h ^= uint64_t((target + n)[4]) << 32;            /* fall through */
        case 4: h ^= uint64_t((target + n)[3]) << 24;            /* fall through */
        case 3: h ^= uint64_t((target + n)[2]) << 16;            /* fall through */
        case 2: h ^= uint64_t((target + n)[1]) <<  8;            /* fall through */
        case 1: h ^= uint64_t((target + n)[0]) <<  0; h *= m;    /* fall through */
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }
}