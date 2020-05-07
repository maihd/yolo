#pragma once

// Primitive types

using I8        = signed char;
using I16       = signed short;
using I32       = signed int;
using I64       = signed long long;

using U8        = unsigned char;
using U16       = unsigned short;
using U32       = unsigned int;
using U64       = unsigned long long;

using F32       = float;
using F64       = double;

using String    = const char*;
using Handle    = unsigned int;
using NullPtr   = decltype(nullptr);

// Arguments

#include <stdarg.h>
using ArgList = va_list;
#define ArgListBegin    va_start
#define ArgListEnd      va_end
#define ArgListGet      va_arg
#define ArgListCopy     va_copy

// Math

constexpr const float PI = 3.141592653589f;

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix4;

struct Vector2
{
    F32 x, y;
};

struct Vector3
{
    F32 x, y, z;
};

struct Vector4
{
    F32 x, y, z, w;
};

struct Quaternion
{
    F32 x, y, z, w;
};

struct Matrix4
{
    F32 data[4][4];
};

// Container types

template <typename T>
struct Array
{
    I32 length;
    I32 capacity;
    T*  elements;
};

template <typename T>
struct HashTable
{
    I32         count;
    I32         capacity;

    I32*        hashs;
    I32         hashCount;

    I32*        nexts;
    U64*        keys;
    T*          values;
};

template <typename TKey, typename TValue>
struct OrderedTable
{
    I32         count;
    I32         capacity;

    TKey*       keys;
    TValue*     values;
};

// Function types

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

        this->context  = *(void**)&lambda;
        this->executor = &ExecuteLambda<T>;
    }

    inline R operator()(Args... args)
    {
        return this->executor(this->context, args...);
    }
};

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

// Runtime

struct Job
{
    Function<void(void)> executor;
};

// IO types

namespace FileModes
{
    enum Type
    {
        None,

        Sync        = 1 << 15,
        DataSync    = 1 << 14,
        NonBlock    = 1 << 13,

        NoLink      = 1 << 12,
        Append      = 1 << 11,
        Create      = 1 << 10,
        Direct      = 1 << 9,
        Existing    = 1 << 8,
        Truncate    = 1 << 7,

        Directory   = 1 << 6, 
        ShortLive   = 1 << 5,
        Temporary   = 1 << 4,

        Random      = 1 << 3,
        Sequence    = 1 << 2,

        Read        = 1 << 0,
        Write       = 1 << 1,
        ReadWrite   = Read | Write,
    };
};

using FileMode = FileModes::Type;
using File = Handle;

// Graphics

enum struct PixelFormat
{
    Unknown = 0,

    RGB,
    RGBA,

    Red,
    Green,
    Blue,
    Alpha,
};

enum struct DrawMode
{
    Fill = 0,
    Line = 1,
};

enum struct DataType
{
    Void,

    I8,
    I16,
    I32,

    U8,
    U16,
    U32,

    F32,
    F64,

    Vector2,
    Vector3,
    Vector4,

    Matrix4x4,
};

enum struct BufferUsage
{
    None,

    StaticDraw,
    StreamDraw,
    DynamicDraw,
};

struct VertexArray
{
    Handle handle;
    Handle indexBuffer;
    Handle vertexBuffer;
};

struct Texture
{
    Handle      handle;
    PixelFormat format;

    I32         width;
    I32         height;
};

struct Shader
{
    Handle  handle;
};

struct FontGlyph
{
    I32     value;

    F32     x0, y0;
    F32     x1, y1;

    F32     u0, v0;
    F32     u1, v1;

    F32     advance;
};

struct Font
{
    F32                 size;
    Array<FontGlyph>    glyphs;

    Texture             texture;
}; 

struct Sprite
{
    Texture texture;

    I32     x;
    I32     y;
    I32     width;
    I32     height;
};

// Audios

enum struct AudioFormat
{
    Mono8,
    Mono16,
    Stereo8,
    Stereo16,
};

struct AudioBuffer
{
    Handle handle;
};

struct AudioSource
{
    Handle handle;
};

// Utils work on types

#include <stddef.h>

#undef offsetof
#ifndef _CRT_USE_BUILTIN_OFFSETOF
#define offsetof(s, m) (int)(&((s*)0)->m)
#else
#define offsetof(s, m) (int)__builtin_offsetof(s,m)
#endif

template <typename T, I32 count>
constexpr I32 CountOf(const T(&_)[count])
{
    return count;
}

inline I32 NextPOT(I32 x)
{
    I32 result = x - 1;

    result = result | (result >> 1);
    result = result | (result >> 2);
    result = result | (result >> 4);
    result = result | (result >> 8);
    result = result | (result >> 16);

    return result + 1;
}

inline I64 NextPOT(I64 x)
{
    I64 result = x - 1;

    result = result | (result >> 1);
    result = result | (result >> 2);
    result = result | (result >> 4);
    result = result | (result >> 8);
    result = result | (result >> 16);
    result = result | (result >> 32);

    return result + 1;
}

U64 CalcHash64(const void* buffer, I32 length, U64 seed = 0);

template <U64 length>
constexpr U64 CalcHash64(const char (&buffer)[length], U64 seed = 0)
{
    U8* target = (U8*)buffer;

    U64 h = seed;

    const U32 l = length - 1;
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
    case 2: h ^= (U64)((target + n)[1]) <<  8;   /* fall through */
    case 1: h ^= (U64)((target + n)[0]) <<  0;   /* fall through */
    };

    h ^= (h << 12);
    h ^= (h >> 47);
    h ^= (h << 25);

    return h;
}

