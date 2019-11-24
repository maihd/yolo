#pragma once

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

struct mat4
{
    float data[4][4];
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
