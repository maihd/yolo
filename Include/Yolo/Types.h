#pragma once

#include <stdarg.h>

using int8    = signed char;
using int16   = signed short;
using int32   = signed int;
using int64   = signed long long;
using uint8   = unsigned char;
using uint16  = unsigned short;
using uint32  = unsigned int;
using uint64  = unsigned long long;

using string  = const char*;

struct Handle
{
    unsigned int id;
};

namespace Const
{
    constexpr int8      INT8_MIN     = -127 - 1;
    constexpr int16     INT16_MIN    = -32767 - 1;
    constexpr int32     INT32_MIN    = -2147483647 - 1;
    constexpr int64     INT64_MIN    = -9223372036854775807LL - 1LL;

    constexpr int8      INT8_MAX     = 127;
    constexpr int16     INT16_MAX    = 32767;
    constexpr int32     INT32_MAX    = 2147483647;
    constexpr int64     INT64_MAX    = 9223372036854775807LL;

    constexpr uint8     UINT8_MIN    = 0;
    constexpr uint16    UINT16_MIN   = 0;
    constexpr uint32    UINT32_MIN   = 0;
    constexpr uint64    UINT64_MIN   = 0;

    constexpr uint8     UINT8_MAX    = 0xffu;
    constexpr uint16    UINT16_MAX   = 0xffffu;
    constexpr uint32    UINT32_MAX   = 0xffffffffu;
    constexpr uint64    UINT64_MAX   = 0xffffffffffffffffull;

    constexpr Handle    EMPTY_HANDLE = { 0 };
    constexpr string    EMPTY_STRING = "";
}
