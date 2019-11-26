#pragma once

#include <math.h>
#include <Yolo/MathTypes.h>

//
// @region: Operators
//

inline vec2 operator-(vec2 v)
{
    return vec2{ -v.x, -v.y };
}

inline vec2 operator+(vec2 v)
{
    return v;
}

inline vec2& operator--(vec2& v)
{
    --v.x;
    --v.y;
    return v;
}

inline vec2& operator++(vec2& v)
{
    ++v.x;
    ++v.y;
    return v;
}

inline vec2 operator--(vec2& v, int)
{
    const vec2 result = v;

    v.x--;
    v.y--;

    return result;
}

inline vec2 operator++(vec2& v, int)
{
    const vec2 result = v;

    v.x++;
    v.y++;

    return result;
}

inline vec2 operator+(vec2 a, vec2 b)
{
#if MATH_ENABLE_NEON   
    return vec2(vadd_f32(a, b));
#else
    return vec2{ a.x + b.x, a.y + b.y };
#endif
}

inline vec2 operator-(vec2 a, vec2 b)
{
#if MATH_ENABLE_NEON   
    return vec2(vsub_f32(a, b));
#else
    return vec2{ a.x - b.x, a.y - b.y };
#endif
}

inline vec2 operator*(vec2 a, vec2 b)
{
#if MATH_ENABLE_NEON   
    return vec2(vmul_f32(a, b));
#else
    return vec2{ a.x * b.x, a.y * b.y };
#endif
}

inline vec2 operator/(vec2 a, vec2 b)
{
#if MATH_ENABLE_NEON && 0 // experimental
    vec2 res;
    __asm volatile(
    "vcvt.f32.u32  q0, q0     \n\t"
        "vrecpe.f32    q0, q0     \n\t"
        "vmul.f32      q0, q0, q1 \n\t"
        "vcvt.u32.f32  q0, q0     \n\t"
        :
    : "r"(dst), "r"()
        :
        );
#else
    return vec2{ a.x / b.x, a.y / b.y };
#endif
}

inline vec2 operator+(vec2 a, float b)
{
    return vec2{ a.x + b, a.y + b };
}

inline vec2 operator-(vec2 a, float b)
{
    return vec2{ a.x - b, a.x - b };
}

inline vec2 operator*(vec2 a, float b)
{
    return vec2{ a.x * b, a.y * b };
}

inline vec2 operator/(vec2 a, float b)
{
    return vec2{ a.x / b, a.y / b };
}

inline vec2 operator+(float a, vec2 b)
{
    return vec2{ a + b.x, a + b.y };
}

inline vec2 operator-(float a, vec2 b)
{
    return vec2{ a - b.x, a - b.y };
}

inline vec2 operator*(float a, vec2 b)
{
    return vec2{ a * b.x, a * b.y };
}

inline vec2 operator/(float a, vec2 b)
{
    return vec2{ a / b.x, a / b.y };
}

inline vec2& operator+=(vec2& a, vec2 b)
{
    return (a = a + b);
}

inline vec2& operator+=(vec2& a, float b)
{
    return (a = a + b);
}

inline vec2& operator-=(vec2& a, vec2 b)
{
    return (a = a - b);
}

inline vec2& operator-=(vec2& a, float b)
{
    return (a = a - b);
}

inline vec2& operator*=(vec2& a, vec2 b)
{
    return (a = a * b);
}

inline vec2& operator*=(vec2& a, float b)
{
    return (a = a * b);
}

inline vec2& operator/=(vec2& a, vec2 b)
{
    return (a = a / b);
}

inline vec2& operator/=(vec2& a, float b)
{
    return (a = a + b);
}

inline bool operator==(vec2 a, vec2 b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(vec2 a, vec2 b)
{
    return a.x != b.x || a.y != b.y;
}

inline vec3 operator-(vec3 v)
{
    return vec3{ -v.x, -v.y, -v.z };
}

inline vec3 operator+(vec3 v)
{
    return v;
}

inline vec3& operator--(vec3& v)
{
    --v.x;
    --v.y;
    --v.z;
    return v;
}

inline vec3& operator++(vec3& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    return v;
}

inline vec3 operator--(vec3& v, int)
{
    const vec3 result = v;

    v.x--;
    v.y--;
    v.z--;

    return result;
}

inline vec3 operator++(vec3& v, int)
{
    const vec3 result = v;

    v.x++;
    v.y++;
    v.z++;

    return result;
}

inline vec3 operator+(vec3 a, vec3 b)
{
    return vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

inline vec3 operator-(vec3 a, vec3 b)
{
    return vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

inline vec3 operator*(vec3 a, vec3 b)
{
    return vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

inline vec3 operator/(vec3 a, vec3 b)
{
    return vec3{ a.x / b.x, a.y / b.y, a.z / b.z };
}

inline vec3 operator+(vec3 a, float b)
{
    return vec3{ a.x + b, a.y + b, a.z + b };
}

inline vec3 operator-(vec3 a, float b)
{
    return vec3{ a.x - b, a.y - b, a.z - b };
}

inline vec3 operator*(vec3 a, float b)
{
    return vec3{ a.x * b, a.y * b, a.z * b };
}

inline vec3 operator/(vec3 a, float b)
{
    return vec3{ a.x / b, a.y / b, a.z / b };
}

inline vec3 operator+(float a, vec3 b)
{
    return vec3{ a + b.x, a + b.y, a + b.z };
}

inline vec3 operator-(float a, vec3 b)
{
    return vec3{ a - b.x, a - b.y, a - b.z };
}

inline vec3 operator*(float a, vec3 b)
{
    return vec3{ a * b.x, a * b.y, a * b.z };
}

inline vec3 operator/(float a, vec3 b)
{
    return vec3{ a / b.x, a / b.y, a / b.z };
}

inline vec3& operator+=(vec3& a, vec3 b)
{
    return (a = a + b);
}

inline vec3& operator+=(vec3& a, float b)
{
    return (a = a + b);
}

inline vec3& operator-=(vec3& a, vec3 b)
{
    return (a = a - b);
}

inline vec3& operator-=(vec3& a, float b)
{
    return (a = a - b);
}

inline vec3& operator*=(vec3& a, vec3 b)
{
    return (a = a * b);
}

inline vec3& operator*=(vec3& a, float b)
{
    return (a = a * b);
}

inline vec3& operator/=(vec3& a, vec3 b)
{
    return (a = a / b);
}

inline vec3& operator/=(vec3& a, float b)
{
    return (a = a + b);
}

inline bool operator==(vec3 a, vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(vec3 a, vec3 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline vec4 operator-(vec4 v)
{
    return vec4{ -v.x, -v.y, -v.z, -v.w };
}

inline vec4 operator+(vec4 v)
{
    return v;
}

inline vec4& operator--(vec4& v)
{
    --v.x;
    --v.y;
    --v.z;
    --v.w;
    return v;
}

inline vec4& operator++(vec4& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    ++v.w;
    return v;
}

inline vec4 operator--(vec4& v, int)
{
    const vec4 result = v;

    v.x--;
    v.y--;
    v.z--;
    v.w--;

    return result;
}

inline vec4 operator++(vec4& v, int)
{
    const vec4 result = v;

    v.x++;
    v.y++;
    v.z++;
    v.w++;

    return result;
}

inline vec4 operator+(vec4 a, vec4 b)
{
    return vec4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline vec4 operator-(vec4 a, vec4 b)
{
    return vec4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline vec4 operator*(vec4 a, vec4 b)
{
    return vec4{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline vec4 operator/(vec4 a, vec4 b)
{
    return vec4{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

inline vec4 operator+(vec4 a, float b)
{
    return vec4{ a.x + b, a.y + b, a.z + b, a.w + b };
}

inline vec4 operator-(vec4 a, float b)
{
    return vec4{ a.x - b, a.y - b, a.z - b, a.w - b };
}

inline vec4 operator*(vec4 a, float b)
{
    return vec4{ a.x * b, a.y * b, a.z * b, a.w * b };
}

inline vec4 operator/(vec4 a, float b)
{
    return vec4{ a.x / b, a.y / b, a.z / b, a.w / b };
}

inline vec4 operator+(float a, vec4 b)
{
    return vec4{ a + b.x, a + b.y, a + b.z, a + b.w };
}

inline vec4 operator-(float a, vec4 b)
{
    return vec4{ a - b.x, a - b.y, a - b.z, a - b.w };
}

inline vec4 operator*(float a, vec4 b)
{
    return vec4{ a * b.x, a * b.y, a * b.z, a * b.w };
}

inline vec4 operator/(float a, vec4 b)
{
    return vec4{ a / b.x, a / b.y, a / b.z, a / b.w };
}

inline vec4& operator+=(vec4& a, vec4 b)
{
    return (a = a + b);
}

inline vec4& operator+=(vec4& a, float b)
{
    return (a = a + b);
}

inline vec4& operator-=(vec4& a, vec4 b)
{
    return (a = a - b);
}

inline vec4& operator-=(vec4& a, float b)
{
    return (a = a - b);
}

inline vec4& operator*=(vec4& a, vec4 b)
{
    return (a = a * b);
}

inline vec4& operator*=(vec4& a, float b)
{
    return (a = a * b);
}

inline vec4& operator/=(vec4& a, vec4 b)
{
    return (a = a / b);
}

inline vec4& operator/=(vec4& a, float b)
{
    return (a = a + b);
}

inline bool operator==(vec4 a, vec4 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(vec4 a, vec4 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}
inline quat operator-(quat v)
{
    return quat{ -v.x, -v.y, -v.z, -v.w };
}

inline quat operator+(quat v)
{
    return v;
}

inline quat& operator--(quat& v)
{
    --v.x;
    --v.y;
    --v.z;
    --v.w;
    return v;
}

inline quat& operator++(quat& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    ++v.w;
    return v;
}

inline quat operator--(quat& v, int)
{
    const quat result = v;

    v.x--;
    v.y--;
    v.z--;
    v.w--;

    return result;
}

inline quat operator++(quat& v, int)
{
    const quat result = v;

    v.x++;
    v.y++;
    v.z++;
    v.w++;

    return result;
}

inline quat operator+(quat a, quat b)
{
    return quat{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline quat operator-(quat a, quat b)
{
    return quat{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline quat operator*(quat a, quat b)
{
    return quat{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline quat operator/(quat a, quat b)
{
    return quat{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

inline quat operator+(quat a, float b)
{
    return quat{ a.x + b, a.y + b, a.z + b, a.w + b };
}

inline quat operator-(quat a, float b)
{
    return quat{ a.x - b, a.y - b, a.z - b, a.w - b };
}

inline quat operator*(quat a, float b)
{
    return quat{ a.x * b, a.y * b, a.z * b, a.w * b };
}

inline quat operator/(quat a, float b)
{
    return quat{ a.x / b, a.y / b, a.z / b, a.w / b };
}

inline quat operator+(float a, quat b)
{
    return quat{ a + b.x, a + b.y, a + b.z, a + b.w };
}

inline quat operator-(float a, quat b)
{
    return quat{ a - b.x, a - b.y, a - b.z, a - b.w };
}

inline quat operator*(float a, quat b)
{
    return quat{ a * b.x, a * b.y, a * b.z, a * b.w };
}

inline quat operator/(float a, quat b)
{
    return quat{ a / b.x, a / b.y, a / b.z, a / b.w };
}

inline quat& operator+=(quat& a, quat b)
{
    return (a = a + b);
}

inline quat& operator+=(quat& a, float b)
{
    return (a = a + b);
}

inline quat& operator-=(quat& a, quat b)
{
    return (a = a - b);
}

inline quat& operator-=(quat& a, float b)
{
    return (a = a - b);
}

inline quat& operator*=(quat& a, quat b)
{
    return (a = a * b);
}

inline quat& operator*=(quat& a, float b)
{
    return (a = a * b);
}

inline quat& operator/=(quat& a, quat b)
{
    return (a = a / b);
}

inline quat& operator/=(quat& a, float b)
{
    return (a = a + b);
}

inline bool operator==(quat a, quat b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(quat a, quat b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

inline bool operator==(mat4 a, mat4 b)
{
    return a.data[0][0] == b.data[0][0] && a.data[0][1] == b.data[0][1] && a.data[0][2] == b.data[0][2] && a.data[0][3] == b.data[0][3]
        && a.data[1][0] == b.data[1][0] && a.data[1][1] == b.data[1][1] && a.data[1][2] == b.data[1][2] && a.data[1][3] == b.data[0][3]
        && a.data[2][0] == b.data[2][0] && a.data[2][1] == b.data[2][1] && a.data[2][2] == b.data[2][2] && a.data[2][3] == b.data[0][3]
        && a.data[3][0] == b.data[3][0] && a.data[3][1] == b.data[3][1] && a.data[3][2] == b.data[3][2] && a.data[3][3] == b.data[0][3];
}

inline bool operator!=(mat4 a, mat4 b)
{
    return a.data[0][0] == b.data[0][0] || a.data[0][1] == b.data[0][1] || a.data[0][2] == b.data[0][2] || a.data[0][3] == b.data[0][3]
        || a.data[1][0] == b.data[1][0] || a.data[1][1] == b.data[1][1] || a.data[1][2] == b.data[1][2] || a.data[1][3] == b.data[0][3]
        || a.data[2][0] == b.data[2][0] || a.data[2][1] == b.data[2][1] || a.data[2][2] == b.data[2][2] || a.data[2][3] == b.data[0][3]
        || a.data[3][0] == b.data[3][0] || a.data[3][1] == b.data[3][1] || a.data[3][2] == b.data[3][2] || a.data[3][3] == b.data[0][3];
}

inline mat4 operator-(mat4 m)
{
    return mat4{
        -m.data[0][0], -m.data[0][1], -m.data[0][2], -m.data[0][3],
        -m.data[1][0], -m.data[1][1], -m.data[1][2], -m.data[1][3],
        -m.data[2][0], -m.data[2][1], -m.data[2][2], -m.data[2][3],
        -m.data[3][0], -m.data[3][1], -m.data[3][2], -m.data[3][3],
    };
}

inline mat4 operator+(mat4 m)
{
    return m;
}

inline mat4& operator--(mat4& m)
{
    --m.data[0][0]; --m.data[0][1]; --m.data[0][2]; --m.data[0][3];
    --m.data[1][0]; --m.data[1][1]; --m.data[1][2]; --m.data[1][3];
    --m.data[2][0]; --m.data[2][1]; --m.data[2][2]; --m.data[2][3];
    --m.data[3][0]; --m.data[3][1]; --m.data[3][2]; --m.data[3][3];
    return m;
}

inline mat4& operator++(mat4& m)
{
    ++m.data[0][0]; ++m.data[0][1]; ++m.data[0][2]; ++m.data[0][3];
    ++m.data[1][0]; ++m.data[1][1]; ++m.data[1][2]; ++m.data[1][3];
    ++m.data[2][0]; ++m.data[2][1]; ++m.data[2][2]; ++m.data[2][3];
    ++m.data[3][0]; ++m.data[3][1]; ++m.data[3][2]; ++m.data[3][3];
    return m;
}

inline mat4 operator--(mat4& m, int)
{
    const mat4 old = m;

    m.data[0][0]--; m.data[0][1]--; m.data[0][2]--; m.data[0][3]--;
    m.data[1][0]--; m.data[1][1]--; m.data[1][2]--; m.data[1][3]--;
    m.data[2][0]--; m.data[2][1]--; m.data[2][2]--; m.data[2][3]--;
    m.data[3][0]--; m.data[3][1]--; m.data[3][2]--; m.data[3][3]--;

    return old;
}

inline mat4 operator++(mat4& m, int)
{
    const mat4 old = m;

    m.data[0][0]++; m.data[0][1]++; m.data[0][2]++; m.data[0][3]++;
    m.data[1][0]++; m.data[1][1]++; m.data[1][2]++; m.data[1][3]++;
    m.data[2][0]++; m.data[2][1]++; m.data[2][2]++; m.data[2][3]++;
    m.data[3][0]++; m.data[3][1]++; m.data[3][2]++; m.data[3][3]++;

    return old;
}

inline mat4 operator+(mat4 a, mat4 b)
{
    return mat4{
        a.data[0][0] + b.data[0][0], a.data[0][1] + b.data[0][1], a.data[0][2] + b.data[0][2], a.data[0][3] + b.data[0][3],
        a.data[1][0] + b.data[1][0], a.data[1][1] + b.data[1][1], a.data[1][2] + b.data[1][2], a.data[1][3] + b.data[0][3],
        a.data[2][0] + b.data[2][0], a.data[2][1] + b.data[2][1], a.data[2][2] + b.data[2][2], a.data[2][3] + b.data[0][3],
        a.data[3][0] + b.data[3][0], a.data[3][1] + b.data[3][1], a.data[3][2] + b.data[3][2], a.data[3][3] + b.data[0][3],
    };
}

inline mat4 operator+(mat4 a, float b)
{
    return mat4{
        a.data[0][0] + b, a.data[0][1] + b, a.data[0][2] + b, a.data[0][3] + b,
        a.data[1][0] + b, a.data[1][1] + b, a.data[1][2] + b, a.data[1][3] + b,
        a.data[2][0] + b, a.data[2][1] + b, a.data[2][2] + b, a.data[2][3] + b,
        a.data[3][0] + b, a.data[3][1] + b, a.data[3][2] + b, a.data[3][3] + b,
    };
}

inline mat4 operator+(float a, mat4 b)
{
    return mat4{
        a + b.data[0][0], a + b.data[0][1], a + b.data[0][2], a + b.data[0][3],
        a + b.data[1][0], a + b.data[1][1], a + b.data[1][2], a + b.data[0][3],
        a + b.data[2][0], a + b.data[2][1], a + b.data[2][2], a + b.data[0][3],
        a + b.data[3][0], a + b.data[3][1], a + b.data[3][2], a + b.data[0][3],
    };
}

inline mat4 operator-(mat4 a, mat4 b)
{
    return mat4{
        a.data[0][0] - b.data[0][0], a.data[0][1] - b.data[0][1], a.data[0][2] - b.data[0][2], a.data[0][3] - b.data[0][3],
        a.data[1][0] - b.data[1][0], a.data[1][1] - b.data[1][1], a.data[1][2] - b.data[1][2], a.data[1][3] - b.data[0][3],
        a.data[2][0] - b.data[2][0], a.data[2][1] - b.data[2][1], a.data[2][2] - b.data[2][2], a.data[2][3] - b.data[0][3],
        a.data[3][0] - b.data[3][0], a.data[3][1] - b.data[3][1], a.data[3][2] - b.data[3][2], a.data[3][3] - b.data[0][3],
    };
}

inline mat4 operator-(mat4 a, float b)
{
    return mat4{
        a.data[0][0] - b, a.data[0][1] - b, a.data[0][2] - b, a.data[0][3] - b,
        a.data[1][0] - b, a.data[1][1] - b, a.data[1][2] - b, a.data[1][3] - b,
        a.data[2][0] - b, a.data[2][1] - b, a.data[2][2] - b, a.data[2][3] - b,
        a.data[3][0] - b, a.data[3][1] - b, a.data[3][2] - b, a.data[3][3] - b,
    };
}

inline mat4 operator-(float a, mat4 b)
{
    return mat4{
        a - b.data[0][0], a - b.data[0][1], a - b.data[0][2], a - b.data[0][3],
        a - b.data[1][0], a - b.data[1][1], a - b.data[1][2], a - b.data[0][3],
        a - b.data[2][0], a - b.data[2][1], a - b.data[2][2], a - b.data[0][3],
        a - b.data[3][0], a - b.data[3][1], a - b.data[3][2], a - b.data[0][3],
    };
}

inline mat4 operator*(mat4 a, mat4 b)
{
    return mat4{
        a.data[0][0] * b.data[0][0], a.data[0][1] * b.data[0][1], a.data[0][2] * b.data[0][2], a.data[0][3] * b.data[0][3],
        a.data[1][0] * b.data[1][0], a.data[1][1] * b.data[1][1], a.data[1][2] * b.data[1][2], a.data[1][3] * b.data[0][3],
        a.data[2][0] * b.data[2][0], a.data[2][1] * b.data[2][1], a.data[2][2] * b.data[2][2], a.data[2][3] * b.data[0][3],
        a.data[3][0] * b.data[3][0], a.data[3][1] * b.data[3][1], a.data[3][2] * b.data[3][2], a.data[3][3] * b.data[0][3],
    };
}

inline mat4 operator*(mat4 a, float b)
{
    return mat4{
        a.data[0][0] * b, a.data[0][1] * b, a.data[0][2] * b, a.data[0][3] * b,
        a.data[1][0] * b, a.data[1][1] * b, a.data[1][2] * b, a.data[1][3] * b,
        a.data[2][0] * b, a.data[2][1] * b, a.data[2][2] * b, a.data[2][3] * b,
        a.data[3][0] * b, a.data[3][1] * b, a.data[3][2] * b, a.data[3][3] * b,
    };
}

inline mat4 operator*(float a, mat4 b)
{
    return mat4{
        a * b.data[0][0], a * b.data[0][1], a * b.data[0][2], a * b.data[0][3],
        a * b.data[1][0], a * b.data[1][1], a * b.data[1][2], a * b.data[0][3],
        a * b.data[2][0], a * b.data[2][1], a * b.data[2][2], a * b.data[0][3],
        a * b.data[3][0], a * b.data[3][1], a * b.data[3][2], a * b.data[0][3],
    };
}

inline mat4 operator/(mat4 a, mat4 b)
{
    return mat4{
        a.data[0][0] / b.data[0][0], a.data[0][1] / b.data[0][1], a.data[0][2] / b.data[0][2], a.data[0][3] / b.data[0][3],
        a.data[1][0] / b.data[1][0], a.data[1][1] / b.data[1][1], a.data[1][2] / b.data[1][2], a.data[1][3] / b.data[0][3],
        a.data[2][0] / b.data[2][0], a.data[2][1] / b.data[2][1], a.data[2][2] / b.data[2][2], a.data[2][3] / b.data[0][3],
        a.data[3][0] / b.data[3][0], a.data[3][1] / b.data[3][1], a.data[3][2] / b.data[3][2], a.data[3][3] / b.data[0][3],
    };
}

inline mat4 operator/(mat4 a, float b)
{
    return mat4{
        a.data[0][0] / b, a.data[0][1] / b, a.data[0][2] / b, a.data[0][3] / b,
        a.data[1][0] / b, a.data[1][1] / b, a.data[1][2] / b, a.data[1][3] / b,
        a.data[2][0] / b, a.data[2][1] / b, a.data[2][2] / b, a.data[2][3] / b,
        a.data[3][0] / b, a.data[3][1] / b, a.data[3][2] / b, a.data[3][3] / b,
    };
}

inline mat4 operator/(float a, mat4 b)
{
    return mat4{
        a / b.data[0][0], a / b.data[0][1], a / b.data[0][2], a / b.data[0][3],
        a / b.data[1][0], a / b.data[1][1], a / b.data[1][2], a / b.data[0][3],
        a / b.data[2][0], a / b.data[2][1], a / b.data[2][2], a / b.data[0][3],
        a / b.data[3][0], a / b.data[3][1], a / b.data[3][2], a / b.data[0][3],
    };
}

inline mat4& operator+=(mat4& a, mat4 b)
{
    return (a = a + b);
}

inline mat4& operator+=(mat4& a, float b)
{
    return (a = a + b);
}

inline mat4& operator-=(mat4& a, mat4 b)
{
    return (a = a - b);
}

inline mat4& operator-=(mat4& a, float b)
{
    return (a = a - b);
}

inline mat4& operator*=(mat4& a, mat4 b)
{
    return (a = a * b);
}

inline mat4& operator*=(mat4& a, float b)
{
    return (a = a * b);
}

inline mat4& operator/=(mat4& a, mat4 b)
{
    return (a = a / b);
}

inline mat4& operator/=(mat4& a, float b)
{
    return (a = a + b);
}

inline int min(int x, int y)
{
    return x < y ? x : y;
}

inline int max(int x, int y)
{
    return x < y ? x : y;
}

inline int clamp(int x, int min, int max)
{
    return x < min ? min : (x > max ? max : x);
}

// Computes sign of 'x'
inline float signf(float x)
{
    return x < 0.0f ? -1.0f : x == 0.0f ? 0 : 1.0f;
}

/* Get the fractal part of floating point
*/
inline float fracf(float x)
{
    return modff(x, 0);
}

/* Get the smaller value
 */
inline float minf(float x, float y)
{
    return x < y ? x : y;
}

/* Get the larger value
 */
inline float maxf(float x, float y)
{
    return x > y ? x : y;
}

/* Clamps the 'x' value to the [min, max].
 */
inline float clampf(float x, float min, float max)
{
    return x < min ? min : (x > max ? max : x);
}

/* Clamps the specified value within the range of 0 to 1
 */
inline float saturatef(float x)
{
    return clampf(x, 0.0f, 1.0f);
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline float stepf(float y, float x)
{
    return x >= y;
}

/* Performs a linear interpolation.
 */
inline float lerpf(float x, float y, float s)
{
    return x + (y - x) * s;
}

/* Compute a smooth Hermite interpolation
 * @return: 0 if x <= min
 *          1 if x >= max
 *          (0, 1) otherwise
 */
inline float smoothstepf(float min, float max, float x)
{
    return (clampf(x, min, max) - min) / (max - min);
}

/* Computes inverse square root of 'x'.
 */
inline float rsqrtf(float x)
{
    return 1.0f / sqrtf(x);
}

/* Computes fast inverse square root of 'x'.
 */
inline float frsqrtf(float x)
{
    union
    {
        float f;
        int   i;
    } cvt;

    cvt.f = x;
    cvt.i = 0x5F3759DF - (cvt.i >> 1);
    cvt.f = cvt.f * (1.5f - (0.5f * x * cvt.f * cvt.f));
    return cvt.f;
}

/* Computes fast inverse square root of 'x'.
 */
inline float fsqrtf(float x)
{
    return x == 0.0f ? 0.0f : 1.0f / frsqrtf(x);
}

// Computes sign of 'x'
inline vec2 sign(vec2 v)
{
    return vec2{ signf(v.x), signf(v.y) };
}

/* Computes absolute value
 */
inline vec2 abs(vec2 v)
{
    return vec2{ fabsf(v.x), fabsf(v.y) };
}

/* Computes cosine
 */
inline vec2 cos(vec2 v)
{
    return vec2{ cosf(v.x), cosf(v.y) };
}

/* Computes sine
 */
inline vec2 sin(vec2 v)
{
    return vec2{ sinf(v.x), sinf(v.y) };
}

/* Computes tangent
 */
inline vec2 tan(vec2 v)
{
    return vec2{ tanf(v.x), tanf(v.y) };
}

/* Computes hyperbolic cosine
 */
inline vec2 cosh(vec2 v)
{
    return vec2{ coshf(v.x), coshf(v.y) };
}

/* Computes hyperbolic sine
 */
inline vec2 sinh(vec2 v)
{
    return vec2{ sinhf(v.x), sinhf(v.y) };
}

/* Computes hyperbolic tangent
 */
inline vec2 tanh(vec2 v)
{
    return vec2{ tanhf(v.x), tanhf(v.y) };
}

/* Computes inverse cosine
 */
inline vec2 acos(vec2 v)
{
    return vec2{ acosf(v.x), acosf(v.y) };
}

/* Computes inverse sine
 */
inline vec2 asin(vec2 v)
{
    return vec2{ asinf(v.x), asinf(v.y) };
}

/* Computes inverse tangent
 */
inline vec2 atan(vec2 v)
{
    return vec2{ atanf(v.x), atanf(v.y) };
}

/* Computes inverse tangent with 2 args
 */
inline vec2 atan2(vec2 a, vec2 b)
{
    return vec2{ atan2f(a.x, b.x), atan2f(a.y, b.y) };
}

/* Computes Euler number raised to the power 'x'
 */
inline vec2 exp(vec2 v)
{
    return vec2{ expf(v.x), expf(v.y) };
}

/* Computes 2 raised to the power 'x'
 */
inline vec2 exp2(vec2 v)
{
    return vec2{ exp2f(v.x), exp2f(v.y) };
}

/* Computes the base Euler number logarithm
 */
inline vec2 log(vec2 v)
{
    return vec2{ logf(v.x), logf(v.y) };
}

/* Computes the base 2 logarithm
 */
inline vec2 log2(vec2 v)
{
    return vec2{ log2f(v.x), log2f(v.y) };
}

/* Computes the base 10 logarithm
 */
inline vec2 log10(vec2 v)
{
    return vec2{ log10f(v.x), log10f(v.y) };
}

/* Computes the value of base raised to the power exponent
 */
inline vec2 pow(vec2 a, vec2 b)
{
    return vec2{ powf(a.x, b.x), powf(a.y, b.y) };
}

/* Get the fractal part of floating point
 */
inline vec2 frac(vec2 v)
{
    return vec2{ fracf(v.x), fracf(v.y) };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline vec2 fmod(vec2 a, vec2 b)
{
    return vec2{ fmodf(a.x, b.x), fmodf(a.y, b.y) };
}

/* Computes the smallest integer value not less than 'x'
 */
inline vec2 ceil(vec2 v)
{
    return vec2{ ceilf(v.x), ceilf(v.y) };
}

/* Computes the largest integer value not greater than 'x'
 */
inline vec2 floor(vec2 v)
{
    return vec2{ floorf(v.x), floorf(v.y) };
}

/* Computes the nearest integer value
 */
inline vec2 round(vec2 v)
{
    return vec2{ roundf(v.x), roundf(v.y) };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline vec2 trunc(vec2 v)
{
    return vec2{ truncf(v.x), truncf(v.y) };
}

/* Get the smaller value
 */
inline vec2 min(vec2 a, vec2 b)
{
    return vec2{ minf(a.x, b.x), minf(a.y, b.y) };
}

/* Get the larger value
 */
inline vec2 max(vec2 a, vec2 b)
{
    return vec2{ maxf(a.x, b.x), maxf(a.y, b.y) };
}

/* Clamps the 'x' value to the [min, max].
 */
inline vec2 clamp(vec2 v, vec2 min, vec2 max)
{
    return vec2{ clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y) };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline vec2 saturate(vec2 v)
{
    return vec2{ saturatef(v.x), saturatef(v.y) };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline vec2 step(vec2 a, vec2 b)
{
    return vec2{ stepf(a.x, b.x), stepf(a.y, b.y) };
}

/* Performs a linear interpolation.
 */
inline vec2 lerp(vec2 a, vec2 b, vec2 t)
{
    return vec2{ lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y) };
}

/* Performs a linear interpolation.
 */
inline vec2 lerp(vec2 a, vec2 b, float t)
{
    return vec2{ lerpf(a.x, b.x, t), lerpf(a.y, b.y, t) };
}

/* Compute a smooth Hermite interpolation
 */
inline vec2 smoothstep(vec2 a, vec2 b, vec2 t)
{
    return vec2{ smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y) };
}

/* Computes square root of 'x'.
 */
inline vec2 sqrt(vec2 v)
{
    return vec2{ sqrtf(v.x), sqrtf(v.y) };
}

/* Computes inverse square root of 'x'.
 */
inline vec2 rsqrt(vec2 v)
{
    return vec2{ rsqrtf(v.x), rsqrtf(v.y) };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec2 fsqrt(vec2 v)
{
    return vec2{ fsqrtf(v.x), fsqrtf(v.y) };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec2 frsqrt(vec2 v)
{
    return vec2{ frsqrtf(v.x), frsqrtf(v.y) };
}

//
// @region: Graphics funtions
//

/* Compute dot product of two vectors
 */
inline float dot(vec2 a, vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

/* Compute squared length of vector
 */
inline float lensqr(vec2 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(vec2 v)
{
    return sqrtf(lensqr(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(vec2 a, vec2 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distsqr(vec2 a, vec2 b)
{
    return lensqr(a - b);
}

/* Compute normalized vector
 */
inline vec2 normalize(vec2 v)
{
    const float lsqr = lensqr(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return vec2{ v.x * f, v.y * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline vec2 reflect(vec2 v, vec2 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline vec2 refract(vec2 v, vec2 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? vec2{ 0.0f, 0.0f }
        : eta * v - (eta * dot(v, n) + sqrtf(k)) * v;
}

/* Compute faceforward vector
 */
inline vec2 faceforward(vec2 n, vec2 i, vec2 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}
/* Computes sign of 'x'
*/
inline vec3 sign(vec3 v)
{
    return vec3{ signf(v.x), signf(v.y), signf(v.z) };
}

/* Computes absolute value
 */
inline vec3 abs(vec3 v)
{
    return vec3{ fabsf(v.x), fabsf(v.y), fabsf(v.z) };
}

/* Computes cosine
 */
inline vec3 cos(vec3 v)
{
    return vec3{ cosf(v.x), cosf(v.y), cosf(v.z) };
}

/* Computes sine
 */
inline vec3 sin(vec3 v)
{
    return vec3{ sinf(v.x), sinf(v.y), sinf(v.z) };
}

/* Computes tangent
 */
inline vec3 tan(vec3 v)
{
    return vec3{ tanf(v.x), tanf(v.y), tanf(v.z) };
}

/* Computes hyperbolic cosine
 */
inline vec3 cosh(vec3 v)
{
    return vec3{ coshf(v.x), coshf(v.y), coshf(v.z) };
}

/* Computes hyperbolic sine
 */
inline vec3 sinh(vec3 v)
{
    return vec3{ sinhf(v.x), sinhf(v.y), sinhf(v.z) };
}

/* Computes hyperbolic tangent
 */
inline vec3 tanh(vec3 v)
{
    return vec3{ tanhf(v.x), tanhf(v.y), tanhf(v.z) };
}

/* Computes inverse cosine
 */
inline vec3 acos(vec3 v)
{
    return vec3{ acosf(v.x), acosf(v.y), acosf(v.z) };
}

/* Computes inverse sine
 */
inline vec3 asin(vec3 v)
{
    return vec3{ asinf(v.x), asinf(v.y), asinf(v.z) };
}

/* Computes inverse tangent
 */
inline vec3 atan(vec3 v)
{
    return vec3{
        atanf(v.x),
        atanf(v.y),
        asinf(v.z)
    };
}

/* Computes inverse tangent with 2 args
 */
inline vec3 atan2(vec3 a, vec3 b)
{
    return vec3{
        atan2f(a.x, b.x),
        atan2f(a.y, b.y),
        atan2f(a.z, b.z)
    };
}

/* Computes Euler number raised to the power 'x'
 */
inline vec3 exp(vec3 v)
{
    return vec3{
        expf(v.x),
        expf(v.y),
        expf(v.z)
    };
}

/* Computes 2 raised to the power 'x'
 */
inline vec3 exp2(vec3 v)
{
    return vec3{
        exp2f(v.x),
        exp2f(v.y),
        exp2f(v.z)
    };
}

/* Computes the base Euler number logarithm
 */
inline vec3 log(vec3 v)
{
    return vec3{
        logf(v.x),
        logf(v.y),
        logf(v.z)
    };
}

/* Computes the base 2 logarithm
 */
inline vec3 log2(vec3 v)
{
    return vec3{
        log2f(v.x),
        log2f(v.y),
        log2f(v.z)
    };
}

/* Computes the base 10 logarithm
 */
inline vec3 log10(vec3 v)
{
    return vec3{
        log10f(v.x),
        log10f(v.y),
        log10f(v.z)
    };
}

/* Computes the value of base raised to the power exponent
 */
inline vec3 pow(vec3 a, vec3 b)
{
    return vec3{
        powf(a.x, b.x),
        powf(a.y, b.y),
        powf(a.z, b.z)
    };
}

/* Get the fractal part of floating point
 */
inline vec3 frac(vec3 v)
{
    return vec3{
        fracf(v.x),
        fracf(v.y),
        fracf(v.z)
    };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline vec3 fmod(vec3 a, vec3 b)
{
    return vec3{
        fmodf(a.x, b.x),
        fmodf(a.y, b.y),
        fmodf(a.z, b.z)
    };
}

/* Computes the smallest integer value not less than 'x'
 */
inline vec3 ceil(vec3 v)
{
    return vec3{
        ceilf(v.x),
        ceilf(v.y),
        ceilf(v.z)
    };
}

/* Computes the largest integer value not greater than 'x'
 */
inline vec3 floor(vec3 v)
{
    return vec3{
        floorf(v.x),
        floorf(v.y),
        floorf(v.z)
    };
}

/* Computes the nearest integer value
 */
inline vec3 round(vec3 v)
{
    return vec3{
        roundf(v.x),
        roundf(v.y),
        roundf(v.z)
    };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline vec3 trunc(vec3 v)
{
    return vec3{
        truncf(v.x),
        truncf(v.y),
        truncf(v.z)
    };
}

/* Get the smaller value
 */
inline vec3 min(vec3 a, vec3 b)
{
    return vec3{
        minf(a.x, b.x),
        minf(a.y, b.y),
        minf(a.z, b.z)
    };
}

/* Get the larger value
 */
inline vec3 max(vec3 a, vec3 b)
{
    return vec3{
        maxf(a.x, b.x),
        maxf(a.y, b.y),
        maxf(a.z, b.z)
    };
}

/* Clamps the 'x' value to the [min, max].
 */
inline vec3 clamp(vec3 v, vec3 min, vec3 max)
{
    return vec3{
        clampf(v.x, min.x, max.x),
        clampf(v.y, min.y, max.y),
        clampf(v.z, min.z, max.z)
    };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline vec3 saturate(vec3 v)
{
    return vec3{
        saturatef(v.x),
        saturatef(v.y),
        saturatef(v.z)
    };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline vec3 step(vec3 a, vec3 b)
{
    return vec3{
        stepf(a.x, b.x),
        stepf(a.y, b.y),
        stepf(a.z, b.z)
    };
}

/* Performs a linear interpolation.
 */
inline vec3 lerp(vec3 a, vec3 b, vec3 t)
{
    return vec3{
        lerpf(a.x, b.x, t.x),
        lerpf(a.y, b.y, t.y),
        lerpf(a.z, b.z, t.z)
    };
}

/* Performs a linear interpolation.
 */
inline vec3 lerp(vec3 a, vec3 b, float t)
{
    return vec3{
        lerpf(a.x, b.x, t),
        lerpf(a.y, b.y, t),
        lerpf(a.z, b.z, t)
    };
}

/* Compute a smooth Hermite interpolation
 */
inline vec3 smoothstep(vec3 a, vec3 b, vec3 t)
{
    return vec3{
        smoothstepf(a.x, b.x, t.x),
        smoothstepf(a.y, b.y, t.y),
        smoothstepf(a.z, b.z, t.z)
    };
}

/* Computes square root of 'x'.
 */
inline vec3 sqrt(vec3 v)
{
    return vec3{
        sqrtf(v.x),
        sqrtf(v.y),
        sqrtf(v.z)
    };
}

/* Computes inverse square root of 'x'.
 */
inline vec3 rsqrt(vec3 v)
{
    return vec3{
        rsqrtf(v.x),
        rsqrtf(v.y),
        rsqrtf(v.z)
    };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec3 fsqrt(vec3 v)
{
    return vec3{
        fsqrtf(v.x),
        fsqrtf(v.y),
        fsqrtf(v.z) 
    };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec3 frsqrt(vec3 v)
{
    return vec3{
        frsqrtf(v.x),
        frsqrtf(v.y),
        frsqrtf(v.z)
    };
}

//
// @region: Graphics functions
//

/* Compute cross product of two vectors
 */
inline vec3 cross(vec3 a, vec3 b)
{
    return vec3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

/* Compute dot product of two vectors
 */
inline float dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Compute squared length of vector
 */
inline float lensqr(vec3 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(vec3 v)
{
    return sqrtf(lensqr(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(vec3 a, vec3 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distsqr(vec3 a, vec3 b)
{
    return lensqr(a - b);
}

/* Compute normalized vector
 */
inline vec3 normalize(vec3 v)
{
    const float lsqr = lensqr(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return vec3{ v.x * f, v.y * f, v.z * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline vec3 reflect(vec3 v, vec3 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline vec3 refract(vec3 v, vec3 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? vec3{ 0.0f, 0.0f, 0.0f }
        : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/* Compute faceforward vector
 */
inline vec3 faceforward(vec3 n, vec3 i, vec3 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

/* Computes sign of 'x'
*/
inline vec4 sign(vec4 v)
{
    return vec4{
        signf(v.x),
        signf(v.y),
        signf(v.z),
        signf(v.w)
    };
}

/* Computes absolute value
 */
inline vec4 abs(vec4 v)
{
    return vec4{
        fabsf(v.x),
        fabsf(v.y),
        fabsf(v.z),
        fabsf(v.w)
    };
}

/* Computes cosine
 */
inline vec4 cos(vec4 v)
{
    return vec4{
        cosf(v.x),
        cosf(v.y),
        cosf(v.z),
        cosf(v.w) 
    };
}

/* Computes sine
 */
inline vec4 sin(vec4 v)
{
    return vec4{
        sinf(v.x),
        sinf(v.y),
        sinf(v.z),
        sinf(v.w)
    };
}

/* Computes tangent
 */
inline vec4 tan(vec4 v)
{
    return vec4{
        tanf(v.x),
        tanf(v.y),
        tanf(v.z),
        tanf(v.w)
    };
}

/* Computes hyperbolic cosine
 */
inline vec4 cosh(vec4 v)
{
    return vec4{
        coshf(v.x),
        coshf(v.y),
        coshf(v.z),
        coshf(v.w)
    };
}

/* Computes hyperbolic sine
 */
inline vec4 sinh(vec4 v)
{
    return vec4{
        sinhf(v.x),
        sinhf(v.y),
        sinhf(v.z),
        sinhf(v.w)
    };
}

/* Computes hyperbolic tangent
 */
inline vec4 tanh(vec4 v)
{
    return vec4{
        tanhf(v.x),
        tanhf(v.y),
        tanhf(v.z),
        tanhf(v.w) 
    };
}

/* Computes inverse cosine
 */
inline vec4 acos(vec4 v)
{
    return vec4{
        acosf(v.x),
        acosf(v.y),
        acosf(v.z),
        acosf(v.w)
    };
}

/* Computes inverse sine
 */
inline vec4 asin(vec4 v)
{
    return vec4{
        asinf(v.x),
        asinf(v.y),
        asinf(v.z),
        asinf(v.w)
    };
}

/* Computes inverse tangent
 */
inline vec4 atan(vec4 v)
{
    return vec4{
        atanf(v.x),
        atanf(v.y),
        atanf(v.z),
        atanf(v.w)
    };
}

/* Computes inverse tangent with 2 args
 */
inline vec4 atan2(vec4 a, vec4 b)
{
    return vec4{ atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w) };
}

/* Computes Euler number raised to the power 'x'
 */
inline vec4 exp(vec4 v)
{
    return vec4{ expf(v.x), expf(v.y), expf(v.z), expf(v.w) };
}

/* Computes 2 raised to the power 'x'
 */
inline vec4 exp2(vec4 v)
{
    return vec4{ exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w) };
}

/* Computes the base Euler number logarithm
 */
inline vec4 log(vec4 v)
{
    return vec4{ logf(v.x), logf(v.y), logf(v.z), logf(v.w) };
}

/* Computes the base 2 logarithm
 */
inline vec4 log2(vec4 v)
{
    return vec4{ log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w) };
}

/* Computes the base 10 logarithm
 */
inline vec4 log10(vec4 v)
{
    return vec4{ log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w) };
}

/* Computes the value of base raised to the power exponent
 */
inline vec4 pow(vec4 a, vec4 b)
{
    return vec4{
        powf(a.x, b.x),
        powf(a.y, b.y),
        powf(a.z, b.z),
        powf(a.w, b.w)
    };
}

/* Get the fractal part of floating point
 */
inline vec4 frac(vec4 v)
{
    return vec4{
        fracf(v.x),
        fracf(v.y),
        fracf(v.z),
        fracf(v.w)
    };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline vec4 fmod(vec4 a, vec4 b)
{
    return vec4{
        fmodf(a.x, b.x),
        fmodf(a.y, b.y),
        fmodf(a.z, b.z),
        fmodf(a.w, b.w)
    };
}

/* Computes the smallest integer value not less than 'x'
 */
inline vec4 ceil(vec4 v)
{
    return vec4{
        ceilf(v.x),
        ceilf(v.y),
        ceilf(v.z),
        ceilf(v.w)
    };
}

/* Computes the largest integer value not greater than 'x'
 */
inline vec4 floor(vec4 v)
{
    return vec4{
        floorf(v.x),
        floorf(v.y),
        floorf(v.z),
        floorf(v.w)
    };
}

/* Computes the nearest integer value
 */
inline vec4 round(vec4 v)
{
    return vec4{
        roundf(v.x),
        roundf(v.y),
        roundf(v.z),
        roundf(v.w)
    };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline vec4 trunc(vec4 v)
{
    return vec4{
        truncf(v.x),
        truncf(v.y),
        truncf(v.z),
        truncf(v.w)
    };
}

/* Get the smaller value
 */
inline vec4 min(vec4 a, vec4 b)
{
    return vec4{
        minf(a.x, b.x),
        minf(a.y, b.y),
        minf(a.z, b.z),
        minf(a.w, b.w)
    };
}

/* Get the larger value
 */
inline vec4 max(vec4 a, vec4 b)
{
    return vec4{
        maxf(a.x, b.x),
        maxf(a.y, b.y),
        maxf(a.z, b.z),
        maxf(a.w, b.w) 
    };
}

/* Clamps the 'x' value to the [min, max].
 */
inline vec4 clamp(vec4 v, vec4 min, vec4 max)
{
    return vec4{
        clampf(v.x, min.x, max.x),
        clampf(v.y, min.y, max.y),
        clampf(v.z, min.z, max.z),
        clampf(v.w, min.w, max.w)
    };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline vec4 saturate(vec4 v)
{
    return vec4{
        saturatef(v.x),
        saturatef(v.y),
        saturatef(v.z),
        saturatef(v.w)
    };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline vec4 step(vec4 a, vec4 b)
{
    return vec4{
        stepf(a.x, b.x),
        stepf(a.y, b.y),
        stepf(a.z, b.z),
        stepf(a.w, b.w)
    };
}

/* Performs a linear interpolation.
 */
inline vec4 lerp(vec4 a, vec4 b, vec4 t)
{
    return vec4{
        lerpf(a.x, b.x, t.x),
        lerpf(a.y, b.y, t.y),
        lerpf(a.z, b.z, t.z),
        lerpf(a.w, b.w, t.w)
    };
}

/* Performs a linear interpolation.
 */
inline vec4 lerp(vec4 a, vec4 b, float t)
{
    return vec4{
        lerpf(a.x, b.x, t),
        lerpf(a.y, b.y, t),
        lerpf(a.z, b.z, t),
        lerpf(a.w, b.w, t)
    };
}

/* Compute a smooth Hermite interpolation
 */
inline vec4 smoothstep(vec4 a, vec4 b, vec4 t)
{
    return vec4{
        smoothstepf(a.x, b.x, t.x),
        smoothstepf(a.y, b.y, t.y),
        smoothstepf(a.z, b.z, t.z),
        smoothstepf(a.w, b.w, t.w)
    };
}

/* Computes square root of 'x'.
 */
inline vec4 sqrt(vec4 v)
{
    return vec4{ sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w) };
}

/* Computes inverse square root of 'x'.
 */
inline vec4 rsqrt(vec4 v)
{
    return vec4{ rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w) };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec4 fsqrt(vec4 v)
{
    return vec4{ fsqrtf(v.x), fsqrtf(v.y), fsqrtf(v.z), fsqrtf(v.w) };
}

/* Computes fast inverse square root of 'x'.
 */
inline vec4 frsqrt(vec4 v)
{
    return vec4{ frsqrtf(v.x), frsqrtf(v.y), frsqrtf(v.z), frsqrtf(v.w) };
}

//
// @region: Graphics functions
//

/* Compute dot product of two vectors
 */
inline float dot(vec4 a, vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/* Compute squared length of vector
 */
inline float lensqr(vec4 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(vec4 v)
{
    return sqrtf(lensqr(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(vec4 a, vec4 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distsqr(vec4 a, vec4 b)
{
    return lensqr(a - b);
}

/* Compute normalized vector
 */
inline vec4 normalize(vec4 v)
{
    const float lsqr = lensqr(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return vec4{ v.x * f, v.y * f, v.z * f, v.w * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline vec4 reflect(vec4 v, vec4 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline vec4 refract(vec4 v, vec4 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? vec4{ 0.0f, 0.0f, 0.0f, 0.0f }
        : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/* Compute faceforward vector
 */
inline vec4 faceforward(vec4 n, vec4 i, vec4 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

//
// @region: Quaternion
//

/* Quaternion multiplication
 */
inline quat mul(quat a, quat b)
{
    const vec3 a3 = vec3{ a.x, a.y, a.z };
    const vec3 b3 = vec3{ b.x, b.y, b.z };

    vec3 v = a3 * b.w + b3 * a.w + cross(a3, b3);
    float  w = a.w * b.w - dot(a3, b3);
    return quat{ v.x, v.y, v.z, w };
}

inline quat inverse(quat q)
{
    return quat{ q.x, q.y, q.z, -q.w };
}

inline quat conj(quat q)
{
    return quat{ -q.x, -q.y, -q.z, q.w };
}

//
// @region: Quaternion
//

inline vec4 mul(mat4 a, vec4 b)
{
    const vec4 c0 = vec4{ a.data[0][0], a.data[1][0], a.data[2][0], a.data[3][0] };
    const vec4 c1 = vec4{ a.data[0][1], a.data[1][1], a.data[2][1], a.data[3][1] };
    const vec4 c2 = vec4{ a.data[0][2], a.data[1][2], a.data[2][2], a.data[3][2] };
    const vec4 c3 = vec4{ a.data[0][3], a.data[1][3], a.data[2][3], a.data[3][3] };

    return vec4{
        dot(c0, b),
        dot(c1, b),
        dot(c2, b),
        dot(c3, b)
    };
}

inline vec4 mul(vec4 a, mat4 b)
{
    const vec4 c0 = vec4{ b.data[0][0], b.data[0][1], b.data[0][2], b.data[0][3] };
    const vec4 c1 = vec4{ b.data[1][0], b.data[1][1], b.data[1][2], b.data[1][3] };
    const vec4 c2 = vec4{ b.data[2][0], b.data[2][1], b.data[2][2], b.data[2][3] };
    const vec4 c3 = vec4{ b.data[3][0], b.data[3][1], b.data[3][2], b.data[3][3] };

    return vec4{
        dot(a, c0),
        dot(a, c1),
        dot(a, c2),
        dot(a, c3)
    };
}

inline vec3 mul(mat4 a, vec3 b)
{
    const vec4 b0 = vec4{ b.x, b.y, b.z, 1.0f };
    const vec4 b1 = mul(a, b0);

    const float iw = 1.0f / b1.w;
    return vec3{ b1.x * iw, b1.y * iw, b1.z * iw };
}

inline vec3 mul(vec3 a, mat4 b)
{
    const vec4 a0 = vec4{ a.x, a.y, a.z, 1.0f };
    const vec4 a1 = mul(a0, b);

    const float iw = 1.0f / a1.w;
    return vec3{ a1.x * iw, a1.y * iw, a1.z * iw };
}

inline mat4 mul(mat4 a, mat4 b)
{
    const vec4 c0 = vec4{ b.data[0][0], b.data[0][1], b.data[0][2], b.data[0][3] };
    const vec4 c1 = vec4{ b.data[1][0], b.data[1][1], b.data[1][2], b.data[1][3] };
    const vec4 c2 = vec4{ b.data[2][0], b.data[2][1], b.data[2][2], b.data[2][3] };
    const vec4 c3 = vec4{ b.data[3][0], b.data[3][1], b.data[3][2], b.data[3][3] };

    const vec4 v0 = mul(a, c0);
    const vec4 v1 = mul(a, c1);
    const vec4 v2 = mul(a, c2);
    const vec4 v3 = mul(a, c3);

    return mat4{
        v0.x, v0.y, v0.z, v0.w,
        v1.x, v1.y, v1.z, v1.w,
        v2.x, v2.y, v2.z, v2.w,
        v3.x, v3.y, v3.z, v3.w,
    };
}
