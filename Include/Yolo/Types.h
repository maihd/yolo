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
using Handle  = unsigned int;
using NullPtr = decltype(nullptr);

template <typename T>
struct Function;

template <typename R, typename ...Args>
struct Function<R(Args...)>
{
    using Executor = R(*)(void*, Args...);

    void*       context;
    Executor    executor;

    static R ExecuteFunction(void* context, Args... args)
    {
        using Function = R(*)(Args...);
        return ((Function)context)(args...);
    };

    template <typename T>
    static R ExecuteLambda(void* context, Args... args)
    {
        return (*(T*)&context)(args...);
    };

    inline Function(R(*function)(Args...))
    {
        this->context = function;
        this->executor = &ExecuteFunction;
    }

    template <typename T>
    inline Function(T lambda)
    {
        static_assert(sizeof(lambda) <= sizeof(void*), "Lambda is too big, it must be <= sizeof(void*). We only should one variable binding, because closure is undefined behaviour.");

        this->context = *(void**)&lambda;
        this->executor = &ExecuteLambda<T>;
    }

    inline R operator()(Args... args)
    {
        return this->executor(this->context, args...);
    }
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

    constexpr Handle    EMPTY_HANDLE = 0;
    constexpr string    EMPTY_STRING = "";
}

template <typename R, typename ...Args>
inline bool operator==(Function<R(Args...)> a, Function<R(Args...)> b)
{
    return a.context == b.context;
}

template <typename R, typename ...Args>
inline bool operator!=(Function<R(Args...)> a, Function<R(Args...)> b)
{
    return a.context != b.context;
}

template <typename R, typename ...Args>
inline bool operator==(Function<R(Args...)> a, NullPtr)
{
    return a.executor == nullptr;
}

template <typename R, typename ...Args>
inline bool operator==(NullPtr, Function<R(Args...)> b)
{
    return b.executor == nullptr;
}

template <typename R, typename ...Args>
inline bool operator!=(Function<R(Args...)> a, NullPtr)
{
    return a.executor != nullptr;
}

template <typename R, typename ...Args>
inline bool operator!=(NullPtr, Function<R(Args...)> b)
{
    return b.executor != nullptr;
}
