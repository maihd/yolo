#pragma once

struct float2;
struct float3;
struct float4;
struct float4x4;

struct float2
{
    float x;
    float y;

    static float2 From(float x, float y);
    static float2 From(float value[2]);
    static float2 From(float2 value);
    static float2 From(float3 value);
    static float2 From(float4 value);
};

struct float3
{
    float x;
    float y;
    float z;

    static float3 From(float x, float y, float z = 0.0f);
    static float3 From(float2 value, float z = 0.0f);

    static float3 From(float value[3]);
    static float3 From(float3 value);
    static float3 From(float4 value);
};

struct float4
{
    float x;
    float y;
    float z;
    float w;

    static float4 From(float x, float y, float z, float w);

    static float4 From(float value[4]);
    static float4 From(float3 value, float w = 0.0f);
    static float4 From(float4 value);
};

struct float4x4
{
    float data[4][4];
};

static_assert(__is_pod(float2), "float2 must be plain old data type.");
static_assert(__is_pod(float3), "float3 must be plain old data type.");
static_assert(__is_pod(float4), "float4 must be plain old data type.");
static_assert(__is_pod(float4x4), "float4x4 must be plain old data type.");

inline float2 float2::From(float x, float y)
{
    return { x, y };
}

inline float2 float2::From(float value[2])
{
    return { value[0], value[1] };
}

inline float2 float2::From(float2 value)
{
    return { value.x, value.y };
}

inline float2 float2::From(float3 value)
{
    return { value.x, value.y };
}

inline float2 float2::From(float4 value)
{
    return { value.x, value.y };
}

inline float3 float3::From(float x, float y, float z)
{
    return { x, y, z };
}

inline float3 float3::From(float2 value, float z)
{
    return { value.x, value.y, z };
}

inline float3 float3::From(float value[3])
{
    return { value[0], value[1], value[2] };
}

inline float3 float3::From(float3 value)
{
    return { value.x, value.y, value.z };
}

inline float3 float3::From(float4 value)
{
    return { value.x, value.y, value.z };
}

inline float4 float4::From(float x, float y, float z, float w)
{
    return { x, y, z, w };
}

inline float4 float4::From(float value[4])
{
    return { value[0], value[1], value[2], value[3] };
}

inline float4 float4::From(float3 value, float w)
{
    return { value.x, value.y, value.z, w };
}

inline float4 float4::From(float4 value)
{
    return { value.x, value.y, value.z, value.w };
}
