#pragma once

#include <math.h>

struct vec2;
struct vec3;
struct vec4;
struct mat4;

struct vec2
{
    float x, y;

    static vec2 From(float x, float y);
    static vec2 From(float value[2]);
    static vec2 From(vec2 value);
    static vec2 From(vec3 value);
    static vec2 From(vec4 value);
};

struct vec3
{
    float x, y, z;

    static vec3 From(float x, float y, float z = 0.0f);
    static vec3 From(vec2 value, float z = 0.0f);

    static vec3 From(float value[3]);
    static vec3 From(vec3 value);
    static vec3 From(vec4 value);
};

struct vec4
{
    float x, y, z, w;

    static vec4 From(float x, float y, float z, float w);

    static vec4 From(float value[4]);
    static vec4 From(vec3 value, float w = 0.0f);
    static vec4 From(vec4 value);
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

inline vec2 vec2::From(float x, float y)
{
    return { x, y };
}

inline vec2 vec2::From(float value[2])
{
    return { value[0], value[1] };
}

inline vec2 vec2::From(vec2 value)
{
    return { value.x, value.y };
}

inline vec2 vec2::From(vec3 value)
{
    return { value.x, value.y };
}

inline vec2 vec2::From(vec4 value)
{
    return { value.x, value.y };
}

inline vec3 vec3::From(float x, float y, float z)
{
    return { x, y, z };
}

inline vec3 vec3::From(vec2 value, float z)
{
    return { value.x, value.y, z };
}

inline vec3 vec3::From(float value[3])
{
    return { value[0], value[1], value[2] };
}

inline vec3 vec3::From(vec3 value)
{
    return { value.x, value.y, value.z };
}

inline vec3 vec3::From(vec4 value)
{
    return { value.x, value.y, value.z };
}

inline vec4 vec4::From(float x, float y, float z, float w)
{
    return { x, y, z, w };
}

inline vec4 vec4::From(float value[4])
{
    return { value[0], value[1], value[2], value[3] };
}

inline vec4 vec4::From(vec3 value, float w)
{
    return { value.x, value.y, value.z, w };
}

inline vec4 vec4::From(vec4 value)
{
    return { value.x, value.y, value.z, value.w };
}

inline vec4 quat::ToAxisAngle(quat q)
{
    const float ilen = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    const vec4 c = q.w != 0.0f
        ? vec4::From(q.x * ilen, q.y * ilen, q.z * ilen, q.w * ilen)
        : vec4::From(q.x, q.y, q.z, q.w);

    const float den = sqrtf(1.0f - q.w * q.w);
    const vec3 axis = (den > 0.0001f)
        ? vec3::From(c.x / den, c.y / den, c.z / den)
        : vec3::From(1, 0, 0);

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

