#pragma once

#include <stdarg.h>

// Primitive types

using byte    = unsigned char;
using uint    = unsigned int;

using int8    = signed char;
using int16   = signed short;
using int32   = signed int;
using int64   = signed long long;
using uint8   = unsigned char;
using uint16  = unsigned short;
using uint32  = unsigned int;
using uint64  = unsigned long long;

using String  = const char*;
using Handle  = unsigned int;
using NullPtr = decltype(nullptr);

// Math

#include <math.h>

constexpr const float PI = 3.141592653589f;

struct vec2;
struct vec3;
struct vec4;
struct mat4;

struct vec2
{
    float x, y;

    static vec2 From(vec3 value);
    static vec2 From(vec4 value);
};

struct vec3
{
    float x, y, z;

    static vec3 From(vec2 value, float z = 0.0f);
    static vec3 From(vec4 value);
};

struct vec4
{
    float x, y, z, w;

    static vec4 From(vec3 value, float w = 0.0f);
};

struct quat
{
    float x, y, z, w;

    static vec4 ToAxisAngle(quat quat);
};

struct mat4
{
    float data[4][4];

    static mat4 From(vec4 row0, vec4 row1, vec4 row2, vec4 row3);

    static mat4 Scalation(float s);
    static mat4 Scalation(vec2 v);
    static mat4 Scalation(vec3 v);
    static mat4 Scalation(float x, float y, float z = 1.0f);

    static mat4 Translation(vec2 v);
    static mat4 Translation(vec3 v);
    static mat4 Translation(float x, float y, float z = 0.0f);

    static mat4 Rotation(quat quaternion);
    static mat4 Rotation(vec3 axis, float angle);
    static mat4 Rotation(float x, float y, float z, float angle);

    static mat4 RotationX(float angle);
    static mat4 RotationY(float angle);
    static mat4 RotationZ(float angle);

    static mat4 Ortho(float l, float r, float b, float t, float n, float f);
    static mat4 Frustum(float l, float r, float b, float t, float n, float f);
    static mat4 Perspective(float fov, float aspect, float znear, float zfar);
};

static_assert(__is_pod(vec2), "vec2 must be plain old data type.");
static_assert(__is_pod(vec3), "vec3 must be plain old data type.");
static_assert(__is_pod(vec4), "vec4 must be plain old data type.");
static_assert(__is_pod(mat4), "mat4 must be plain old data type.");

inline vec2 vec2::From(vec3 value)
{
    return { value.x, value.y };
}

inline vec2 vec2::From(vec4 value)
{
    return { value.x, value.y };
}

inline vec3 vec3::From(vec2 value, float z)
{
    return { value.x, value.y, z };
}

inline vec3 vec3::From(vec4 value)
{
    return { value.x, value.y, value.z };
}

inline vec4 vec4::From(vec3 value, float w)
{
    return { value.x, value.y, value.z, w };
}

inline vec4 quat::ToAxisAngle(quat q)
{
    const float ilen = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    const vec4 c = q.w != 0.0f
        ? vec4{ q.x * ilen, q.y * ilen, q.z * ilen, q.w * ilen }
    : vec4{ q.x, q.y, q.z, q.w };

    const float den = sqrtf(1.0f - q.w * q.w);
    const vec3 axis = (den > 0.0001f)
        ? vec3{ c.x / den, c.y / den, c.z / den }
    : vec3{ 1, 0, 0 };

    float angle = 2.0f * cosf(c.w);
    return vec4::From(axis, angle);
}

inline mat4 mat4::From(vec4 row0, vec4 row1, vec4 row2, vec4 row3)
{
    return {
        row0.x, row0.y, row0.z, row0.w,
        row1.x, row1.y, row1.z, row1.w,
        row2.x, row2.y, row2.z, row2.w,
        row3.x, row3.y, row3.z, row3.w,
    };
}

inline mat4 mat4::Ortho(float l, float r, float b, float t, float n, float f)
{
    const float x = 1.0f / (r - l);
    const float y = 1.0f / (t - b);
    const float z = 1.0f / (f - n);

    return {
        2.0f * x, 0, 0, 0,
        0, 2.0f * y, 0, 0,
        0, 0, 2.0f * z, 0,
        -x * (l + r), -y * (b + t), -z * (n + f), 1.0f,
    };
}

inline mat4 mat4::Frustum(float l, float r, float b, float t, float n, float f)
{
    const float x = 1.0f / (r - l);
    const float y = 1.0f / (t - b);
    const float z = 1.0f / (f - n);

    return {
        2.0f * x, 0, 0, 0,
        0, 2.0f * y, 0, 0,
        x * (l + r), y * (b + t), z * (n + f), 1.0f,
        0, 0, 2.0f * z, 0
    };
}

inline mat4 mat4::Perspective(float fov, float aspect, float znear, float zfar)
{
    const float a = 1.0f / tanf(fov * 0.5f);
    const float b = zfar / (znear - zfar);

    return {
        a / aspect, 0, 0, 0,
        0, a, 0, 0,
        0, 0, b, -1,
        0, 0, znear * b, 0
    };
}

inline mat4 mat4::Scalation(float s)
{
    return Scalation(s, s, s);
}

inline mat4 mat4::Scalation(vec2 v)
{
    return Scalation(v.x, v.y);
}

inline mat4 mat4::Scalation(vec3 v)
{
    return Scalation(v.x, v.y, v.z);
}

inline mat4 mat4::Scalation(float x, float y, float z)
{
    return {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

inline mat4 mat4::Translation(vec2 v)
{
    return Translation(v.x, v.y);
}

inline mat4 mat4::Translation(vec3 v)
{
    return Translation(v.x, v.y, v.z);
}

inline mat4 mat4::Translation(float x, float y, float z)
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
}

inline mat4 mat4::Rotation(vec3 axis, float angle)
{
    return Rotation(axis.x, axis.y, axis.z, angle);
}

inline mat4 mat4::Rotation(float x, float y, float z, float angle)
{
    const float c = cosf(-angle);
    const float s = sinf(-angle);
    const float t = 1.0f - c;

    return {
        /* Row 1 */
        t * x * x + c,
        t * x * y - s * z,
        t * x * z + s * y,
        0.0f,

        /* Row 2 */
        t * x * y + s * z,
        t * y * y + c,
        t * y * z - s * x,
        0.0f,

        /* Row 3 */
        t * x * z - s * y,
        t * y * z + s * x,
        t * z * z + c,
        0.0f,

        /* Row 4 */
        0, 0, 0, 1.0f
    };
}

inline mat4 mat4::RotationX(float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return {
        1,  0, 0, 0,
        0,  c, s, 0,
        0, -s, c, 0,
        0,  0, 0, 1
    };
}

inline mat4 mat4::RotationY(float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return {
         c, 0, s, 0,
         0, 1, 0, 0,
        -s, 0, c, 0,
         0, 0, 0, 1
    };
}

inline mat4 mat4::RotationZ(float angle)
{
    const float s = sinf(angle);
    const float c = cosf(angle);

    return {
         c, s, 0, 0,
        -s, c, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
    };
}

inline mat4 mat4::Rotation(quat quaternion)
{
    vec4 axisangle = quat::ToAxisAngle(quaternion);
    return Rotation(axisangle.x, axisangle.y, axisangle.z, axisangle.w);
}

// Container types

template <typename T>
struct Array
{
    int length;
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
    uint64*     keys;
    T*          values;
};

template <typename TKey, typename TValue>
struct OrderedTable
{

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

        this->context = *(void**)&lambda;
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

// IO types

enum struct FileMode
{
    None,
    Read = 1 << 0,
    Write = 1 << 1,
    Binary = 1 << 2,
    Create = 1 << 3,
};

struct File
{
    Handle handle;
};

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

    Int8,
    Int16,
    Int32,

    Uint8,
    Uint16,
    Uint32,

    Float32,
    Float64,

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

    int         width;
    int         height;
};

struct Shader
{
    Handle  handle;
};

struct FontGlyph
{
    int     value;

    float   x0, y0;
    float   x1, y1;

    float   u0, v0;
    float   u1, v1;

    float   advance;
};

struct Font
{
    float               size;
    Array<FontGlyph>    glyphs;

    Texture             texture;
};

// Utils work on types

#include <stddef.h>

#undef offsetof
#ifndef _CRT_USE_BUILTIN_OFFSETOF
#define offsetof(s, m) (int)(&((s*)0)->m)
#else
#define offsetof(s, m) (int)__builtin_offsetof(s,m)
#endif

template <typename T, int count>
constexpr int CountOf(const T(&_)[count])
{
    return count;
}

inline int NextPOT(int x)
{
    int result = x - 1;

    result = result | (result >> 1);
    result = result | (result >> 2);
    result = result | (result >> 4);
    result = result | (result >> 8);
    result = result | (result >> 16);

    return result + 1;
}

uint64 CalcHash64(const void* buffer, int length, uint64 seed = 0);

template <typename T>
inline uint64 CalcHash64(T x, uint64 seed = 0)
{
    return CalcHash64(&x, sizeof(x), seed);
}

template <uint64 length>
constexpr uint64 CalcHash64(const char(&target)[length], uint64 seed = 0)
{
    uint64 h = seed;

    const uint32 l = length - 1;
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

