#include <Yolo/Types.h>

uint64 CalcHash64(const void* buffer, int length, uint64 seed)
{
    uint8* target = (uint8*)buffer;

    uint64 h = seed;

    const uint32 l = (uint32)length;
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
    case 2: h ^= uint64((target + n)[1]) << 8;   /* fall through */
    case 1: h ^= uint64((target + n)[0]) << 0;   /* fall through */
    };

    h ^= (h << 12);
    h ^= (h >> 47);
    h ^= (h << 25);

    return h;
}

