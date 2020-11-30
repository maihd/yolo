#include <Yolo/Types.h>

#include <stdio.h>

U32 CalcHash32(const void* buffer, I32 length, U32 seed)
{
    // Murmur hash: multiply (mu) and rotate (r) x2
    constexpr U32 mul = 0xcc9e2d51;
    constexpr U32 rot = 17; // should be prime number

    const U8* target = (U8*)buffer;
    const U32 l = (U32)length;
    const U32 n = (l >> 2) << 2;

    U32 h = seed ^ (l | mul);
    for (U32 i = 0; i < n; i += 4)
    {
        U32 b0 = target[i + 0];
        U32 b1 = target[i + 1];
        U32 b2 = target[i + 2];
        U32 b3 = target[i + 3];
#if CPU_LITTLE_ENDIAN
        U32 k = (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0);
#else
        U32 k = (b0 << 24) | (b1 << 16) | (b2 << 8) | (b3 << 0);
#endif

        k *= mul;
        k ^= (k >> rot);
        k *= mul;

        h ^= k;
        h *= mul;
    }

    switch (l & 3)
    {
    case 3: h ^= (U32)((target + n)[2]) << 16;   /* fall through */
    case 2: h ^= (U32)((target + n)[1]) << 8;   /* fall through */
    case 1: h ^= (U32)((target + n)[0]) << 0;   /* fall through */
    };

    h *= mul;
    h ^= (h >> rot);
    h *= mul;
    h ^= (h >> rot);

    return h;
}

U64 CalcHash64(const void* buffer, I32 length, U64 seed)
{
    // Murmur hash: multiply (mu) and rotate (r) x2
    constexpr U64 mul = 0xc6a4a7935bd1e995ULL;
    constexpr U64 rot = 47; // should be prime number

    const U8* target = (U8*)buffer;
    const U32 l = (U32)length;
    const U32 n = (l >> 3) << 3;

    U64 h = seed ^ (l | mul);
    for (U32 i = 0; i < n; i += 8)
    {
        U64 b0 = target[i + 0];
        U64 b1 = target[i + 1];
        U64 b2 = target[i + 2];
        U64 b3 = target[i + 3];
        U64 b4 = target[i + 4];
        U64 b5 = target[i + 5];
        U64 b6 = target[i + 6];
        U64 b7 = target[i + 7];
#if CPU_LITTLE_ENDIAN
        U64 k = (b7 << 56) | (b6 << 48) | (b5 << 40) | (b4 << 32) | (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0 << 0);
#else
        U64 k = (b0 << 56) | (b1 << 48) | (b2 << 40) | (b3 << 32) | (b4 << 24) | (b5 << 16) | (b6 << 8) | (b7 << 0);
#endif

        k *= mul;
        k ^= (k >> rot);
        k *= mul;

        h ^= k;
        h *= mul;
    }

    switch (l & 7)
    {
    case 7: h ^= (U64)((target + n)[6]) << 48;   /* fall through */
    case 6: h ^= (U64)((target + n)[5]) << 40;   /* fall through */
    case 5: h ^= (U64)((target + n)[4]) << 32;   /* fall through */
    case 4: h ^= (U64)((target + n)[3]) << 24;   /* fall through */
    case 3: h ^= (U64)((target + n)[2]) << 16;   /* fall through */
    case 2: h ^= (U64)((target + n)[1]) <<  8;   /* fall through */
    case 1: h ^= (U64)((target + n)[0]) <<  0;   /* fall through */
    };

    h *= mul;
    h ^= (h >> rot);
    h *= mul;
    h ^= (h >> rot);

    return h;
}

void DebugPrintInternal(const char* func, const char* file, int line, const char* format, ...)
{
    printf("[%s:%d:%s] ", file, line, func);

    ArgList arglist;
    ArgListBegin(arglist, format);
    vprintf(format, arglist);
    ArgListEnd(arglist);

    printf("\n");
}

void DebugAssertReport(const char* test, const char* func, const char* file, int line, const char* format, ...)
{
    printf("Assertion failed: %s\n", test);
    printf("At %s:%d:%s\n\n", file, line, func);

    ArgList arglist;
    ArgListBegin(arglist, format);
    vprintf(format, arglist);
    ArgListEnd(arglist);

    printf("\n");
}
