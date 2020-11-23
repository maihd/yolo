#pragma once

// ----------------------
// System info
// ----------------------

// Runtime architecture
#if defined(__x86_64__)    \
 || defined(_M_X64)        \
 || defined(__aarch64__)   \
 || defined(__64BIT__)     \
 || defined(__mips64)      \
 || defined(__powerpc64__) \
 || defined(__ppc64__)     \
 || defined(__LP64__)
#   define ARCH_64BIT   1
#   define ARCH_32BIT   0
#   define ARCH_BITNESS 64
#else
#   define ARCH_64BIT   0
#   define ARCH_32BIT   1
#   define ARCH_BITNESS 32
#endif

// Dectect cpu endian
#if CPU_PPC
#	if _LITTLE_ENDIAN
#       define CPU_BIG_ENDIAN    0
#       define CPU_LITTLE_ENDIAN 1
#	else
#       define CPU_BIG_ENDIAN    1
#       define CPU_LITTLE_ENDIAN 0
#   endif
#else
#   define CPU_BIG_ENDIAN    0
#   define CPU_LITTLE_ENDIAN 1
#endif

// ----------------------
// Assertions
// ----------------------

#ifndef NDEBUG
#define DebugAssert(test, format, ...)                                                  \
    do {                                                                                \
        if (!(test)) {                                                                  \
            DebugPrint(#test, __FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
            __debugbreak();                                                             \
        }                                                                               \
    } while (0)

void DebugPrint(const char* test, const char* func, const char* file, int line, const char* format, ...);
#else
#define DebugAssert(test, format, ...) ((void)0)
#endif

// ----------------------
// Primitive types
// ----------------------

using I8        = signed char;
using I16       = signed short;
using I32       = signed int;
using I64       = signed long long;

using U8        = unsigned char;
using U16       = unsigned short;
using U32       = unsigned int;
using U64       = unsigned long long;

using Handle    = unsigned int;
using NullPtr   = decltype(nullptr);

// ----------------------
// Arguments
// ----------------------

#include <stdarg.h>
using ArgList = va_list;
#define ArgListBegin    va_start
#define ArgListEnd      va_end
#define ArgListGet      va_arg
#define ArgListCopy     va_copy

// ----------------------
// Math types
// ----------------------

#ifndef MATH_TYPES_DEFINED
#define MATH_TYPES_DEFINED
struct Vector2
{
    float x, y;
};

struct Vector3
{
    float x, y, z;
};

struct Vector4
{
    float x, y, z, w;
};

struct Quaternion
{
    float x, y, z, w;
};

struct Matrix4
{
    float data[4][4];
};
#endif

// ----------------------
// Common types
// ----------------------

struct Rectangle
{
    float x;
    float y;
    float width;
    float height;
};

// String containers
// Useful for storing string data in other structure
struct String
{
    const char* Buffer;
    int         Length;

    int         Alloced : 30;
    int         IsOwned : 1;
    int         IsStatic : 1;
};

// ----------------------
// Container types
// ----------------------

template <typename T>
struct Array
{
    int count;
    int capacity;
    T*  elements;
};

template <typename T>
struct HashTable
{
    int         count;
    int         capacity;

    int*        hashs;
    int         hashCount;

    int*        nexts;
    U64*        keys;
    T*          values;
};

template <typename TKey, typename TValue>
struct OrderedTable
{
    int         count;
    int         capacity;

    TKey*       keys;
    TValue*     values;
};

// ----------------------
// System
// ----------------------

struct Job
{
    void* Data;
    void (*Execute)(void* data);
};

// ----------------------
// IO types
// ----------------------

namespace _FileMode
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

using FileMode = _FileMode::Type;
using File = Handle;

// ----------------------
// Graphics Types
// ----------------------

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

    Float,
    Double,

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

    float         width;
    float         height;
};

struct Shader
{
    Handle  handle;
};

struct FontGlyph
{
    I32     value;

    float     x0, y0;
    float     x1, y1;

    float     u0, v0;
    float     u1, v1;

    float     advance;
};

struct Font
{
    float                 size;
    Array<FontGlyph>    glyphs;

    Texture             texture;
}; 

struct Sprite
{
    Texture     texture;
    float         x;
    float         y;
    float         width;
    float         height;
};

// ----------------------
// Audios types
// ----------------------

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

// ----------------------
// Constants
// ----------------------

constexpr const float PI = 3.141592653589f;

// ----------------------
// Utils work on types
// ----------------------

#include <stddef.h>

#undef offsetof
#ifndef _CRT_USE_BUILTIN_OFFSETOF
#define offsetof(s, m) (I32)(&((s*)0)->m)
#else
#define offsetof(s, m) (I32)__builtin_offsetof(s,m)
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

U32 CalcHash32(const void* buffer, I32 length, U32 seed = 0);
U64 CalcHash64(const void* buffer, I32 length, U64 seed = 0);

template <U32 length>
constexpr U32 ConstHash32(const char(&buffer)[length], U32 seed = 0)
{
    U8* target = (U8*)buffer;

    U64 h = seed;

    const U32 l = length - 1;
    const U32 n = (l >> 3) << 3;

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

        k ^= (k << 12);
        k ^= (k >> 24);
        k ^= (k << 15);

        h ^= k;
    }

    switch (l & 7)
    {
    case 3: h ^= (U32)((target + n)[2]) << 16;   /* fall through */
    case 2: h ^= (U32)((target + n)[1]) << 8;   /* fall through */
    case 1: h ^= (U32)((target + n)[0]) << 0;   /* fall through */
    };

    h ^= (h << 12);
    h ^= (h >> 24);
    h ^= (h << 15);

    return h;
}

template <U64 length>
constexpr U64 ConstHash64(const char (&buffer)[length], U64 seed = 0)
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
