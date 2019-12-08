#pragma once

#include <Yolo/Types.h>

namespace Random
{
    int32  NextI32(int32 seed = 0);
    int64  NextI64(int64 seed = 0);
    uint32 NextU32(uint32 seed = 0);
    uint64 NextU64(uint64 seed = 0);

    constexpr uint32 AsCompileTimeU32(uint32 seed = 0)
    {
        String time = __TIME__;

        uint32 timeNumber0 = 0;
        timeNumber0 |= (uint32)time[0] << (uint32)(3 * 8);
        timeNumber0 |= (uint32)time[1] << (uint32)(2 * 8);
        timeNumber0 |= (uint32)time[2] << (uint32)(1 * 8);
        timeNumber0 |= (uint32)time[3] << (uint32)(0 * 8);

        uint32 timeNumber1 = 0;
        timeNumber1 |= (uint32)time[4] << (uint32)(3 * 8);
        timeNumber1 |= (uint32)time[5] << (uint32)(2 * 8);
        timeNumber1 |= (uint32)time[6] << (uint32)(1 * 8);
        timeNumber1 |= (uint32)time[7] << (uint32)(0 * 8);

        return seed ^ timeNumber0 ^ timeNumber1;
    }

    constexpr uint64 AsCompileTimeU64(uint64 seed = 0)
    {
        String time = __TIME__;

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
};

