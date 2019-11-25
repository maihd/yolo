#include <Yolo/Random.h>

namespace Random
{
    int32 NextI32(int32 seed)
    {
        int32 result = seed;

        result ^= (result >> 12);
        result ^= (result << 17);
        result ^= (result >> 5);

        return result;
    }

    int64 NextI64(int64 seed)
    {
        int64 result = seed;

        result ^= (result >> 12);
        result ^= (result << 17);
        result ^= (result >> 5);
        result ^= (result >> 24);
        result ^= (result << 34);
        result ^= (result >> 10);

        return result;
    }

    uint32 NextU32(uint32 seed)
    {
        uint32 result = seed;

        result ^= (result >> 12);
        result ^= (result << 17);
        result ^= (result >> 5);

        return result;
    }

    uint64 NextU64(uint64 seed)
    {
        uint64 result = seed;

        result ^= (result >> 12);
        result ^= (result << 17);
        result ^= (result >> 5);
        result ^= (result >> 24);
        result ^= (result << 34);
        result ^= (result >> 10);

        return result;
    }
}