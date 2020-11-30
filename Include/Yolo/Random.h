#pragma once

#include <Yolo/Core.h>

namespace Random
{
    I32 NextI32(I32 seed = 0);
    I64 NextI64(I64 seed = 0);
    U32 NextU32(U32 seed = 0);
    U64 NextU64(U64 seed = 0);

    constexpr U32 AsCompileTimeU32(U32 seed = 0)
    {
        const char* time = __TIME__;

        U32 timeNumber0 = 0;
        timeNumber0 |= (U32)time[0] << (U32)(3 * 8);
        timeNumber0 |= (U32)time[1] << (U32)(2 * 8);
        timeNumber0 |= (U32)time[2] << (U32)(1 * 8);
        timeNumber0 |= (U32)time[3] << (U32)(0 * 8);

        U32 timeNumber1 = 0;
        timeNumber1 |= (U32)time[4] << (U32)(3 * 8);
        timeNumber1 |= (U32)time[5] << (U32)(2 * 8);
        timeNumber1 |= (U32)time[6] << (U32)(1 * 8);
        timeNumber1 |= (U32)time[7] << (U32)(0 * 8);

        return seed ^ timeNumber0 ^ timeNumber1;
    }

    constexpr U64 AsCompileTimeU64(U64 seed = 0)
    {
        const char* time = __TIME__;

        U64 timeNumber = 0;
        timeNumber |= (U64)time[0] << (U64)(7 * 8);
        timeNumber |= (U64)time[1] << (U64)(6 * 8);
        timeNumber |= (U64)time[2] << (U64)(5 * 8);
        timeNumber |= (U64)time[3] << (U64)(4 * 8);
        timeNumber |= (U64)time[4] << (U64)(3 * 8);
        timeNumber |= (U64)time[5] << (U64)(2 * 8);
        timeNumber |= (U64)time[6] << (U64)(1 * 8);
        timeNumber |= (U64)time[7] << (U64)(0 * 8);

        return seed ^ timeNumber;
    }
};

