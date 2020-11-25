#include <Yolo/Random.h>

namespace Random
{
    I32 NextI32(I32 seed)
    {
        I32 result = seed;

        result ^= (result >> 13);
        result ^= (result << 17);
        result ^= (result >> 5);

        return result;
    }

    I64 NextI64(I64 seed)
    {
        I64 result = seed;

        result ^= (result >> 13);
        result ^= (result << 17);
        result ^= (result >> 5);
        result ^= (result >> 23);
        result ^= (result << 37);
        result ^= (result >> 11);

        return result;
    }

    U32 NextU32(U32 seed)
    {
        U32 result = seed;

        result ^= (result >> 13);
        result ^= (result << 17);
        result ^= (result >> 5);

        return result;
    }

    U64 NextU64(U64 seed)
    {
        U64 result = seed;

        result ^= (result >> 13);
        result ^= (result << 17);
        result ^= (result >> 5);
        result ^= (result >> 23);
        result ^= (result << 37);
        result ^= (result >> 11);

        return result;
    }
}
