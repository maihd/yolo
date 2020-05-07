#include <Yolo/Types.h>

U64 CalcHash64(const void* buffer, I32 length, U64 seed)
{
    U8* target = (U8*)buffer;

    U64 h = seed;

    const U32 l = (U32)length;
    const U32 n = (l >> 3) << 3;

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

        k ^= (k << 12);
        k ^= (k >> 47);
        k ^= (k << 25);

        h ^= k;
    }

    switch (l & 7)
    {
    case 7: h ^= (U64)((target + n)[6]) << 48;   /* fall through */
    case 6: h ^= (U64)((target + n)[5]) << 40;   /* fall through */
    case 5: h ^= (U64)((target + n)[4]) << 32;   /* fall through */
    case 4: h ^= (U64)((target + n)[3]) << 24;   /* fall through */
    case 3: h ^= (U64)((target + n)[2]) << 16;   /* fall through */
    case 2: h ^= (U64)((target + n)[1]) << 8;   /* fall through */
    case 1: h ^= (U64)((target + n)[0]) << 0;   /* fall through */
    };

    h ^= (h << 12);
    h ^= (h >> 47);
    h ^= (h << 25);

    return h;
}

