#pragma once

#include <math.h>

// -----------------------------------
// Math types
// -----------------------------------

#ifndef MATH_TYPES_DEFINED
#define MATH_TYPES_DEFINED
struct Vector2
{
    float       x, y;
};

struct Vector3
{
    float       x, y, z;
};

struct Vector4
{
    float       x, y, z, w;
};

struct Quaternion
{
    float       x, y, z, w;
};

struct Matrix4
{
    float       data[4][4];
};
#endif

// -----------------------------------
// Operators
// -----------------------------------

inline Vector2 operator-(Vector2 v)
{
    return Vector2{ -v.x, -v.y };
}

inline Vector2 operator+(Vector2 v)
{
    return v;
}

inline Vector2& operator--(Vector2& v)
{
    --v.x;
    --v.y;
    return v;
}

inline Vector2& operator++(Vector2& v)
{
    ++v.x;
    ++v.y;
    return v;
}

inline Vector2 operator--(Vector2& v, int)
{
    const Vector2 result = v;

    v.x--;
    v.y--;

    return result;
}

inline Vector2 operator++(Vector2& v, int)
{
    const Vector2 result = v;

    v.x++;
    v.y++;

    return result;
}

inline Vector2 operator+(Vector2 a, Vector2 b)
{
#if MATH_ENABLE_NEON   
    return Vector2(vadd_f32(a, b));
#else
    return Vector2{ a.x + b.x, a.y + b.y };
#endif
}

inline Vector2 operator-(Vector2 a, Vector2 b)
{
#if MATH_ENABLE_NEON   
    return Vector2(vsub_f32(a, b));
#else
    return Vector2{ a.x - b.x, a.y - b.y };
#endif
}

inline Vector2 operator*(Vector2 a, Vector2 b)
{
#if MATH_ENABLE_NEON   
    return Vector2(vmul_f32(a, b));
#else
    return Vector2{ a.x * b.x, a.y * b.y };
#endif
}

inline Vector2 operator/(Vector2 a, Vector2 b)
{
#if MATH_ENABLE_NEON && 0 // experimental
    Vector2 res;
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
    return Vector2{ a.x / b.x, a.y / b.y };
#endif
}

inline Vector2 operator+(Vector2 a, float b)
{
    return Vector2{ a.x + b, a.y + b };
}

inline Vector2 operator-(Vector2 a, float b)
{
    return Vector2{ a.x - b, a.x - b };
}

inline Vector2 operator*(Vector2 a, float b)
{
    return Vector2{ a.x * b, a.y * b };
}

inline Vector2 operator/(Vector2 a, float b)
{
    return Vector2{ a.x / b, a.y / b };
}

inline Vector2 operator+(float a, Vector2 b)
{
    return Vector2{ a + b.x, a + b.y };
}

inline Vector2 operator-(float a, Vector2 b)
{
    return Vector2{ a - b.x, a - b.y };
}

inline Vector2 operator*(float a, Vector2 b)
{
    return Vector2{ a * b.x, a * b.y };
}

inline Vector2 operator/(float a, Vector2 b)
{
    return Vector2{ a / b.x, a / b.y };
}

inline Vector2& operator+=(Vector2& a, Vector2 b)
{
    return (a = a + b);
}

inline Vector2& operator+=(Vector2& a, float b)
{
    return (a = a + b);
}

inline Vector2& operator-=(Vector2& a, Vector2 b)
{
    return (a = a - b);
}

inline Vector2& operator-=(Vector2& a, float b)
{
    return (a = a - b);
}

inline Vector2& operator*=(Vector2& a, Vector2 b)
{
    return (a = a * b);
}

inline Vector2& operator*=(Vector2& a, float b)
{
    return (a = a * b);
}

inline Vector2& operator/=(Vector2& a, Vector2 b)
{
    return (a = a / b);
}

inline Vector2& operator/=(Vector2& a, float b)
{
    return (a = a + b);
}

inline bool operator==(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(Vector2 a, Vector2 b)
{
    return a.x != b.x || a.y != b.y;
}

inline Vector3 operator-(Vector3 v)
{
    return Vector3{ -v.x, -v.y, -v.z };
}

inline Vector3 operator+(Vector3 v)
{
    return v;
}

inline Vector3& operator--(Vector3& v)
{
    --v.x;
    --v.y;
    --v.z;
    return v;
}

inline Vector3& operator++(Vector3& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    return v;
}

inline Vector3 operator--(Vector3& v, int)
{
    const Vector3 result = v;

    v.x--;
    v.y--;
    v.z--;

    return result;
}

inline Vector3 operator++(Vector3& v, int)
{
    const Vector3 result = v;

    v.x++;
    v.y++;
    v.z++;

    return result;
}

inline Vector3 operator+(Vector3 a, Vector3 b)
{
    return Vector3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vector3 operator-(Vector3 a, Vector3 b)
{
    return Vector3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vector3 operator*(Vector3 a, Vector3 b)
{
    return Vector3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

inline Vector3 operator/(Vector3 a, Vector3 b)
{
    return Vector3{ a.x / b.x, a.y / b.y, a.z / b.z };
}

inline Vector3 operator+(Vector3 a, float b)
{
    return Vector3{ a.x + b, a.y + b, a.z + b };
}

inline Vector3 operator-(Vector3 a, float b)
{
    return Vector3{ a.x - b, a.y - b, a.z - b };
}

inline Vector3 operator*(Vector3 a, float b)
{
    return Vector3{ a.x * b, a.y * b, a.z * b };
}

inline Vector3 operator/(Vector3 a, float b)
{
    return Vector3{ a.x / b, a.y / b, a.z / b };
}

inline Vector3 operator+(float a, Vector3 b)
{
    return Vector3{ a + b.x, a + b.y, a + b.z };
}

inline Vector3 operator-(float a, Vector3 b)
{
    return Vector3{ a - b.x, a - b.y, a - b.z };
}

inline Vector3 operator*(float a, Vector3 b)
{
    return Vector3{ a * b.x, a * b.y, a * b.z };
}

inline Vector3 operator/(float a, Vector3 b)
{
    return Vector3{ a / b.x, a / b.y, a / b.z };
}

inline Vector3& operator+=(Vector3& a, Vector3 b)
{
    return (a = a + b);
}

inline Vector3& operator+=(Vector3& a, float b)
{
    return (a = a + b);
}

inline Vector3& operator-=(Vector3& a, Vector3 b)
{
    return (a = a - b);
}

inline Vector3& operator-=(Vector3& a, float b)
{
    return (a = a - b);
}

inline Vector3& operator*=(Vector3& a, Vector3 b)
{
    return (a = a * b);
}

inline Vector3& operator*=(Vector3& a, float b)
{
    return (a = a * b);
}

inline Vector3& operator/=(Vector3& a, Vector3 b)
{
    return (a = a / b);
}

inline Vector3& operator/=(Vector3& a, float b)
{
    return (a = a + b);
}

inline bool operator==(Vector3 a, Vector3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(Vector3 a, Vector3 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline Vector4 operator-(Vector4 v)
{
    return Vector4{ -v.x, -v.y, -v.z, -v.w };
}

inline Vector4 operator+(Vector4 v)
{
    return v;
}

inline Vector4& operator--(Vector4& v)
{
    --v.x;
    --v.y;
    --v.z;
    --v.w;
    return v;
}

inline Vector4& operator++(Vector4& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    ++v.w;
    return v;
}

inline Vector4 operator--(Vector4& v, int)
{
    const Vector4 result = v;

    v.x--;
    v.y--;
    v.z--;
    v.w--;

    return result;
}

inline Vector4 operator++(Vector4& v, int)
{
    const Vector4 result = v;

    v.x++;
    v.y++;
    v.z++;
    v.w++;

    return result;
}

inline Vector4 operator+(Vector4 a, Vector4 b)
{
    return Vector4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline Vector4 operator-(Vector4 a, Vector4 b)
{
    return Vector4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline Vector4 operator*(Vector4 a, Vector4 b)
{
    return Vector4{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline Vector4 operator/(Vector4 a, Vector4 b)
{
    return Vector4{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

inline Vector4 operator+(Vector4 a, float b)
{
    return Vector4{ a.x + b, a.y + b, a.z + b, a.w + b };
}

inline Vector4 operator-(Vector4 a, float b)
{
    return Vector4{ a.x - b, a.y - b, a.z - b, a.w - b };
}

inline Vector4 operator*(Vector4 a, float b)
{
    return Vector4{ a.x * b, a.y * b, a.z * b, a.w * b };
}

inline Vector4 operator/(Vector4 a, float b)
{
    return Vector4{ a.x / b, a.y / b, a.z / b, a.w / b };
}

inline Vector4 operator+(float a, Vector4 b)
{
    return Vector4{ a + b.x, a + b.y, a + b.z, a + b.w };
}

inline Vector4 operator-(float a, Vector4 b)
{
    return Vector4{ a - b.x, a - b.y, a - b.z, a - b.w };
}

inline Vector4 operator*(float a, Vector4 b)
{
    return Vector4{ a * b.x, a * b.y, a * b.z, a * b.w };
}

inline Vector4 operator/(float a, Vector4 b)
{
    return Vector4{ a / b.x, a / b.y, a / b.z, a / b.w };
}

inline Vector4& operator+=(Vector4& a, Vector4 b)
{
    return (a = a + b);
}

inline Vector4& operator+=(Vector4& a, float b)
{
    return (a = a + b);
}

inline Vector4& operator-=(Vector4& a, Vector4 b)
{
    return (a = a - b);
}

inline Vector4& operator-=(Vector4& a, float b)
{
    return (a = a - b);
}

inline Vector4& operator*=(Vector4& a, Vector4 b)
{
    return (a = a * b);
}

inline Vector4& operator*=(Vector4& a, float b)
{
    return (a = a * b);
}

inline Vector4& operator/=(Vector4& a, Vector4 b)
{
    return (a = a / b);
}

inline Vector4& operator/=(Vector4& a, float b)
{
    return (a = a + b);
}

inline bool operator==(Vector4 a, Vector4 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(Vector4 a, Vector4 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}
inline Quaternion operator-(Quaternion v)
{
    return Quaternion{ -v.x, -v.y, -v.z, -v.w };
}

inline Quaternion operator+(Quaternion v)
{
    return v;
}

inline Quaternion& operator--(Quaternion& v)
{
    --v.x;
    --v.y;
    --v.z;
    --v.w;
    return v;
}

inline Quaternion& operator++(Quaternion& v)
{
    ++v.x;
    ++v.y;
    ++v.z;
    ++v.w;
    return v;
}

inline Quaternion operator--(Quaternion& v, int)
{
    const Quaternion result = v;

    v.x--;
    v.y--;
    v.z--;
    v.w--;

    return result;
}

inline Quaternion operator++(Quaternion& v, int)
{
    const Quaternion result = v;

    v.x++;
    v.y++;
    v.z++;
    v.w++;

    return result;
}

inline Quaternion operator+(Quaternion a, Quaternion b)
{
    return Quaternion{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline Quaternion operator-(Quaternion a, Quaternion b)
{
    return Quaternion{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline Quaternion operator*(Quaternion a, Quaternion b)
{
    return Quaternion{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

inline Quaternion operator/(Quaternion a, Quaternion b)
{
    return Quaternion{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

inline Quaternion operator+(Quaternion a, float b)
{
    return Quaternion{ a.x + b, a.y + b, a.z + b, a.w + b };
}

inline Quaternion operator-(Quaternion a, float b)
{
    return Quaternion{ a.x - b, a.y - b, a.z - b, a.w - b };
}

inline Quaternion operator*(Quaternion a, float b)
{
    return Quaternion{ a.x * b, a.y * b, a.z * b, a.w * b };
}

inline Quaternion operator/(Quaternion a, float b)
{
    return Quaternion{ a.x / b, a.y / b, a.z / b, a.w / b };
}

inline Quaternion operator+(float a, Quaternion b)
{
    return Quaternion{ a + b.x, a + b.y, a + b.z, a + b.w };
}

inline Quaternion operator-(float a, Quaternion b)
{
    return Quaternion{ a - b.x, a - b.y, a - b.z, a - b.w };
}

inline Quaternion operator*(float a, Quaternion b)
{
    return Quaternion{ a * b.x, a * b.y, a * b.z, a * b.w };
}

inline Quaternion operator/(float a, Quaternion b)
{
    return Quaternion{ a / b.x, a / b.y, a / b.z, a / b.w };
}

inline Quaternion& operator+=(Quaternion& a, Quaternion b)
{
    return (a = a + b);
}

inline Quaternion& operator+=(Quaternion& a, float b)
{
    return (a = a + b);
}

inline Quaternion& operator-=(Quaternion& a, Quaternion b)
{
    return (a = a - b);
}

inline Quaternion& operator-=(Quaternion& a, float b)
{
    return (a = a - b);
}

inline Quaternion& operator*=(Quaternion& a, Quaternion b)
{
    return (a = a * b);
}

inline Quaternion& operator*=(Quaternion& a, float b)
{
    return (a = a * b);
}

inline Quaternion& operator/=(Quaternion& a, Quaternion b)
{
    return (a = a / b);
}

inline Quaternion& operator/=(Quaternion& a, float b)
{
    return (a = a + b);
}

inline bool operator==(Quaternion a, Quaternion b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(Quaternion a, Quaternion b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

inline bool operator==(Matrix4 a, Matrix4 b)
{
    return a.data[0][0] == b.data[0][0] && a.data[0][1] == b.data[0][1] && a.data[0][2] == b.data[0][2] && a.data[0][3] == b.data[0][3]
        && a.data[1][0] == b.data[1][0] && a.data[1][1] == b.data[1][1] && a.data[1][2] == b.data[1][2] && a.data[1][3] == b.data[0][3]
        && a.data[2][0] == b.data[2][0] && a.data[2][1] == b.data[2][1] && a.data[2][2] == b.data[2][2] && a.data[2][3] == b.data[0][3]
        && a.data[3][0] == b.data[3][0] && a.data[3][1] == b.data[3][1] && a.data[3][2] == b.data[3][2] && a.data[3][3] == b.data[0][3];
}

inline bool operator!=(Matrix4 a, Matrix4 b)
{
    return a.data[0][0] == b.data[0][0] || a.data[0][1] == b.data[0][1] || a.data[0][2] == b.data[0][2] || a.data[0][3] == b.data[0][3]
        || a.data[1][0] == b.data[1][0] || a.data[1][1] == b.data[1][1] || a.data[1][2] == b.data[1][2] || a.data[1][3] == b.data[0][3]
        || a.data[2][0] == b.data[2][0] || a.data[2][1] == b.data[2][1] || a.data[2][2] == b.data[2][2] || a.data[2][3] == b.data[0][3]
        || a.data[3][0] == b.data[3][0] || a.data[3][1] == b.data[3][1] || a.data[3][2] == b.data[3][2] || a.data[3][3] == b.data[0][3];
}

inline Matrix4 operator-(Matrix4 m)
{
    return Matrix4{
        -m.data[0][0], -m.data[0][1], -m.data[0][2], -m.data[0][3],
        -m.data[1][0], -m.data[1][1], -m.data[1][2], -m.data[1][3],
        -m.data[2][0], -m.data[2][1], -m.data[2][2], -m.data[2][3],
        -m.data[3][0], -m.data[3][1], -m.data[3][2], -m.data[3][3],
    };
}

inline Matrix4 operator+(Matrix4 m)
{
    return m;
}

inline Matrix4& operator--(Matrix4& m)
{
    --m.data[0][0]; --m.data[0][1]; --m.data[0][2]; --m.data[0][3];
    --m.data[1][0]; --m.data[1][1]; --m.data[1][2]; --m.data[1][3];
    --m.data[2][0]; --m.data[2][1]; --m.data[2][2]; --m.data[2][3];
    --m.data[3][0]; --m.data[3][1]; --m.data[3][2]; --m.data[3][3];
    return m;
}

inline Matrix4& operator++(Matrix4& m)
{
    ++m.data[0][0]; ++m.data[0][1]; ++m.data[0][2]; ++m.data[0][3];
    ++m.data[1][0]; ++m.data[1][1]; ++m.data[1][2]; ++m.data[1][3];
    ++m.data[2][0]; ++m.data[2][1]; ++m.data[2][2]; ++m.data[2][3];
    ++m.data[3][0]; ++m.data[3][1]; ++m.data[3][2]; ++m.data[3][3];
    return m;
}

inline Matrix4 operator--(Matrix4& m, int)
{
    const Matrix4 old = m;

    m.data[0][0]--; m.data[0][1]--; m.data[0][2]--; m.data[0][3]--;
    m.data[1][0]--; m.data[1][1]--; m.data[1][2]--; m.data[1][3]--;
    m.data[2][0]--; m.data[2][1]--; m.data[2][2]--; m.data[2][3]--;
    m.data[3][0]--; m.data[3][1]--; m.data[3][2]--; m.data[3][3]--;

    return old;
}

inline Matrix4 operator++(Matrix4& m, int)
{
    const Matrix4 old = m;

    m.data[0][0]++; m.data[0][1]++; m.data[0][2]++; m.data[0][3]++;
    m.data[1][0]++; m.data[1][1]++; m.data[1][2]++; m.data[1][3]++;
    m.data[2][0]++; m.data[2][1]++; m.data[2][2]++; m.data[2][3]++;
    m.data[3][0]++; m.data[3][1]++; m.data[3][2]++; m.data[3][3]++;

    return old;
}

inline Matrix4 operator+(Matrix4 a, Matrix4 b)
{
    return Matrix4{
        a.data[0][0] + b.data[0][0], a.data[0][1] + b.data[0][1], a.data[0][2] + b.data[0][2], a.data[0][3] + b.data[0][3],
        a.data[1][0] + b.data[1][0], a.data[1][1] + b.data[1][1], a.data[1][2] + b.data[1][2], a.data[1][3] + b.data[0][3],
        a.data[2][0] + b.data[2][0], a.data[2][1] + b.data[2][1], a.data[2][2] + b.data[2][2], a.data[2][3] + b.data[0][3],
        a.data[3][0] + b.data[3][0], a.data[3][1] + b.data[3][1], a.data[3][2] + b.data[3][2], a.data[3][3] + b.data[0][3],
    };
}

inline Matrix4 operator+(Matrix4 a, float b)
{
    return Matrix4{
        a.data[0][0] + b, a.data[0][1] + b, a.data[0][2] + b, a.data[0][3] + b,
        a.data[1][0] + b, a.data[1][1] + b, a.data[1][2] + b, a.data[1][3] + b,
        a.data[2][0] + b, a.data[2][1] + b, a.data[2][2] + b, a.data[2][3] + b,
        a.data[3][0] + b, a.data[3][1] + b, a.data[3][2] + b, a.data[3][3] + b,
    };
}

inline Matrix4 operator+(float a, Matrix4 b)
{
    return Matrix4{
        a + b.data[0][0], a + b.data[0][1], a + b.data[0][2], a + b.data[0][3],
        a + b.data[1][0], a + b.data[1][1], a + b.data[1][2], a + b.data[0][3],
        a + b.data[2][0], a + b.data[2][1], a + b.data[2][2], a + b.data[0][3],
        a + b.data[3][0], a + b.data[3][1], a + b.data[3][2], a + b.data[0][3],
    };
}

inline Matrix4 operator-(Matrix4 a, Matrix4 b)
{
    return Matrix4{
        a.data[0][0] - b.data[0][0], a.data[0][1] - b.data[0][1], a.data[0][2] - b.data[0][2], a.data[0][3] - b.data[0][3],
        a.data[1][0] - b.data[1][0], a.data[1][1] - b.data[1][1], a.data[1][2] - b.data[1][2], a.data[1][3] - b.data[0][3],
        a.data[2][0] - b.data[2][0], a.data[2][1] - b.data[2][1], a.data[2][2] - b.data[2][2], a.data[2][3] - b.data[0][3],
        a.data[3][0] - b.data[3][0], a.data[3][1] - b.data[3][1], a.data[3][2] - b.data[3][2], a.data[3][3] - b.data[0][3],
    };
}

inline Matrix4 operator-(Matrix4 a, float b)
{
    return Matrix4{
        a.data[0][0] - b, a.data[0][1] - b, a.data[0][2] - b, a.data[0][3] - b,
        a.data[1][0] - b, a.data[1][1] - b, a.data[1][2] - b, a.data[1][3] - b,
        a.data[2][0] - b, a.data[2][1] - b, a.data[2][2] - b, a.data[2][3] - b,
        a.data[3][0] - b, a.data[3][1] - b, a.data[3][2] - b, a.data[3][3] - b,
    };
}

inline Matrix4 operator-(float a, Matrix4 b)
{
    return Matrix4{
        a - b.data[0][0], a - b.data[0][1], a - b.data[0][2], a - b.data[0][3],
        a - b.data[1][0], a - b.data[1][1], a - b.data[1][2], a - b.data[0][3],
        a - b.data[2][0], a - b.data[2][1], a - b.data[2][2], a - b.data[0][3],
        a - b.data[3][0], a - b.data[3][1], a - b.data[3][2], a - b.data[0][3],
    };
}

inline Matrix4 operator*(Matrix4 a, Matrix4 b)
{
    return Matrix4{
        a.data[0][0] * b.data[0][0], a.data[0][1] * b.data[0][1], a.data[0][2] * b.data[0][2], a.data[0][3] * b.data[0][3],
        a.data[1][0] * b.data[1][0], a.data[1][1] * b.data[1][1], a.data[1][2] * b.data[1][2], a.data[1][3] * b.data[0][3],
        a.data[2][0] * b.data[2][0], a.data[2][1] * b.data[2][1], a.data[2][2] * b.data[2][2], a.data[2][3] * b.data[0][3],
        a.data[3][0] * b.data[3][0], a.data[3][1] * b.data[3][1], a.data[3][2] * b.data[3][2], a.data[3][3] * b.data[0][3],
    };
}

inline Matrix4 operator*(Matrix4 a, float b)
{
    return Matrix4{
        a.data[0][0] * b, a.data[0][1] * b, a.data[0][2] * b, a.data[0][3] * b,
        a.data[1][0] * b, a.data[1][1] * b, a.data[1][2] * b, a.data[1][3] * b,
        a.data[2][0] * b, a.data[2][1] * b, a.data[2][2] * b, a.data[2][3] * b,
        a.data[3][0] * b, a.data[3][1] * b, a.data[3][2] * b, a.data[3][3] * b,
    };
}

inline Matrix4 operator*(float a, Matrix4 b)
{
    return Matrix4{
        a * b.data[0][0], a * b.data[0][1], a * b.data[0][2], a * b.data[0][3],
        a * b.data[1][0], a * b.data[1][1], a * b.data[1][2], a * b.data[0][3],
        a * b.data[2][0], a * b.data[2][1], a * b.data[2][2], a * b.data[0][3],
        a * b.data[3][0], a * b.data[3][1], a * b.data[3][2], a * b.data[0][3],
    };
}

inline Matrix4 operator/(Matrix4 a, Matrix4 b)
{
    return Matrix4{
        a.data[0][0] / b.data[0][0], a.data[0][1] / b.data[0][1], a.data[0][2] / b.data[0][2], a.data[0][3] / b.data[0][3],
        a.data[1][0] / b.data[1][0], a.data[1][1] / b.data[1][1], a.data[1][2] / b.data[1][2], a.data[1][3] / b.data[0][3],
        a.data[2][0] / b.data[2][0], a.data[2][1] / b.data[2][1], a.data[2][2] / b.data[2][2], a.data[2][3] / b.data[0][3],
        a.data[3][0] / b.data[3][0], a.data[3][1] / b.data[3][1], a.data[3][2] / b.data[3][2], a.data[3][3] / b.data[0][3],
    };
}

inline Matrix4 operator/(Matrix4 a, float b)
{
    return Matrix4{
        a.data[0][0] / b, a.data[0][1] / b, a.data[0][2] / b, a.data[0][3] / b,
        a.data[1][0] / b, a.data[1][1] / b, a.data[1][2] / b, a.data[1][3] / b,
        a.data[2][0] / b, a.data[2][1] / b, a.data[2][2] / b, a.data[2][3] / b,
        a.data[3][0] / b, a.data[3][1] / b, a.data[3][2] / b, a.data[3][3] / b,
    };
}

inline Matrix4 operator/(float a, Matrix4 b)
{
    return Matrix4{
        a / b.data[0][0], a / b.data[0][1], a / b.data[0][2], a / b.data[0][3],
        a / b.data[1][0], a / b.data[1][1], a / b.data[1][2], a / b.data[0][3],
        a / b.data[2][0], a / b.data[2][1], a / b.data[2][2], a / b.data[0][3],
        a / b.data[3][0], a / b.data[3][1], a / b.data[3][2], a / b.data[0][3],
    };
}

inline Matrix4& operator+=(Matrix4& a, Matrix4 b)
{
    return (a = a + b);
}

inline Matrix4& operator+=(Matrix4& a, float b)
{
    return (a = a + b);
}

inline Matrix4& operator-=(Matrix4& a, Matrix4 b)
{
    return (a = a - b);
}

inline Matrix4& operator-=(Matrix4& a, float b)
{
    return (a = a - b);
}

inline Matrix4& operator*=(Matrix4& a, Matrix4 b)
{
    return (a = a * b);
}

inline Matrix4& operator*=(Matrix4& a, float b)
{
    return (a = a * b);
}

inline Matrix4& operator/=(Matrix4& a, Matrix4 b)
{
    return (a = a / b);
}

inline Matrix4& operator/=(Matrix4& a, float b)
{
    return (a = a + b);
}

// --------------------------------------------------------------------
// Functions
// @note: use single word lowercase name for standard math functions
// --------------------------------------------------------------------

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
    return x < 0.0f ? -1.0f : x == 0.0f ? 0.0f : 1.0f;
}

/* Get the fractal part of floating point
*/
inline float fracf(float x)
{
    return modff(x, (float*)0);
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
inline Vector2 sign(Vector2 v)
{
    return Vector2{ signf(v.x), signf(v.y) };
}

/* Computes absolute value
 */
inline Vector2 abs(Vector2 v)
{
    return Vector2{ fabsf(v.x), fabsf(v.y) };
}

/* Computes cosine
 */
inline Vector2 cos(Vector2 v)
{
    return Vector2{ cosf(v.x), cosf(v.y) };
}

/* Computes sine
 */
inline Vector2 sin(Vector2 v)
{
    return Vector2{ sinf(v.x), sinf(v.y) };
}

/* Computes tangent
 */
inline Vector2 tan(Vector2 v)
{
    return Vector2{ tanf(v.x), tanf(v.y) };
}

/* Computes hyperbolic cosine
 */
inline Vector2 cosh(Vector2 v)
{
    return Vector2{ coshf(v.x), coshf(v.y) };
}

/* Computes hyperbolic sine
 */
inline Vector2 sinh(Vector2 v)
{
    return Vector2{ sinhf(v.x), sinhf(v.y) };
}

/* Computes hyperbolic tangent
 */
inline Vector2 tanh(Vector2 v)
{
    return Vector2{ tanhf(v.x), tanhf(v.y) };
}

/* Computes inverse cosine
 */
inline Vector2 acos(Vector2 v)
{
    return Vector2{ acosf(v.x), acosf(v.y) };
}

/* Computes inverse sine
 */
inline Vector2 asin(Vector2 v)
{
    return Vector2{ asinf(v.x), asinf(v.y) };
}

/* Computes inverse tangent
 */
inline Vector2 atan(Vector2 v)
{
    return Vector2{ atanf(v.x), atanf(v.y) };
}

/* Computes inverse tangent with 2 args
 */
inline Vector2 atan2(Vector2 a, Vector2 b)
{
    return Vector2{ atan2f(a.x, b.x), atan2f(a.y, b.y) };
}

/* Computes Euler number raised to the power 'x'
 */
inline Vector2 exp(Vector2 v)
{
    return Vector2{ expf(v.x), expf(v.y) };
}

/* Computes 2 raised to the power 'x'
 */
inline Vector2 exp2(Vector2 v)
{
    return Vector2{ exp2f(v.x), exp2f(v.y) };
}

/* Computes the base Euler number logarithm
 */
inline Vector2 log(Vector2 v)
{
    return Vector2{ logf(v.x), logf(v.y) };
}

/* Computes the base 2 logarithm
 */
inline Vector2 log2(Vector2 v)
{
    return Vector2{ log2f(v.x), log2f(v.y) };
}

/* Computes the base 10 logarithm
 */
inline Vector2 log10(Vector2 v)
{
    return Vector2{ log10f(v.x), log10f(v.y) };
}

/* Computes the value of base raised to the power exponent
 */
inline Vector2 pow(Vector2 a, Vector2 b)
{
    return Vector2{ powf(a.x, b.x), powf(a.y, b.y) };
}

/* Get the fractal part of floating point
 */
inline Vector2 frac(Vector2 v)
{
    return Vector2{ fracf(v.x), fracf(v.y) };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline Vector2 fmod(Vector2 a, Vector2 b)
{
    return Vector2{ fmodf(a.x, b.x), fmodf(a.y, b.y) };
}

/* Computes the smallest integer value not less than 'x'
 */
inline Vector2 ceil(Vector2 v)
{
    return Vector2{ ceilf(v.x), ceilf(v.y) };
}

/* Computes the largest integer value not greater than 'x'
 */
inline Vector2 floor(Vector2 v)
{
    return Vector2{ floorf(v.x), floorf(v.y) };
}

/* Computes the nearest integer value
 */
inline Vector2 round(Vector2 v)
{
    return Vector2{ roundf(v.x), roundf(v.y) };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline Vector2 trunc(Vector2 v)
{
    return Vector2{ truncf(v.x), truncf(v.y) };
}

/* Get the smaller value
 */
inline Vector2 min(Vector2 a, Vector2 b)
{
    return Vector2{ minf(a.x, b.x), minf(a.y, b.y) };
}

/* Get the larger value
 */
inline Vector2 max(Vector2 a, Vector2 b)
{
    return Vector2{ maxf(a.x, b.x), maxf(a.y, b.y) };
}

/* Clamps the 'x' value to the [min, max].
 */
inline Vector2 clamp(Vector2 v, Vector2 min, Vector2 max)
{
    return Vector2{ clampf(v.x, min.x, max.x), clampf(v.y, min.y, max.y) };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline Vector2 saturate(Vector2 v)
{
    return Vector2{ saturatef(v.x), saturatef(v.y) };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline Vector2 step(Vector2 a, Vector2 b)
{
    return Vector2{ stepf(a.x, b.x), stepf(a.y, b.y) };
}

/* Performs a linear interpolation.
 */
inline Vector2 lerp(Vector2 a, Vector2 b, Vector2 t)
{
    return Vector2{ lerpf(a.x, b.x, t.x), lerpf(a.y, b.y, t.y) };
}

/* Performs a linear interpolation.
 */
inline Vector2 lerp(Vector2 a, Vector2 b, float t)
{
    return Vector2{ lerpf(a.x, b.x, t), lerpf(a.y, b.y, t) };
}

/* Compute a smooth Hermite interpolation
 */
inline Vector2 smoothstep(Vector2 a, Vector2 b, Vector2 t)
{
    return Vector2{ smoothstepf(a.x, b.x, t.x), smoothstepf(a.y, b.y, t.y) };
}

/* Computes square root of 'x'.
 */
inline Vector2 sqrt(Vector2 v)
{
    return Vector2{ sqrtf(v.x), sqrtf(v.y) };
}

/* Computes inverse square root of 'x'.
 */
inline Vector2 rsqrt(Vector2 v)
{
    return Vector2{ rsqrtf(v.x), rsqrtf(v.y) };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector2 fsqrt(Vector2 v)
{
    return Vector2{ fsqrtf(v.x), fsqrtf(v.y) };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector2 frsqrt(Vector2 v)
{
    return Vector2{ frsqrtf(v.x), frsqrtf(v.y) };
}

//
// @region: Graphics funtions
//

/* Compute dot product of two vectors
 */
inline float dot(Vector2 a, Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

/* Compute squared length of vector
 */
inline float lengthSquare(Vector2 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(Vector2 v)
{
    return sqrtf(lengthSquare(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(Vector2 a, Vector2 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distanceSquare(Vector2 a, Vector2 b)
{
    return lengthSquare(a - b);
}

/* Compute normalized vector
 */
inline Vector2 normalize(Vector2 v)
{
    const float lsqr = lengthSquare(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return Vector2{ v.x * f, v.y * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline Vector2 reflect(Vector2 v, Vector2 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline Vector2 refract(Vector2 v, Vector2 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? Vector2{ 0.0f, 0.0f }
    : eta * v - (eta * dot(v, n) + sqrtf(k)) * v;
}

/* Compute faceforward vector
 */
inline Vector2 faceforward(Vector2 n, Vector2 i, Vector2 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}
/* Computes sign of 'x'
*/
inline Vector3 sign(Vector3 v)
{
    return Vector3{ signf(v.x), signf(v.y), signf(v.z) };
}

/* Computes absolute value
 */
inline Vector3 abs(Vector3 v)
{
    return Vector3{ fabsf(v.x), fabsf(v.y), fabsf(v.z) };
}

/* Computes cosine
 */
inline Vector3 cos(Vector3 v)
{
    return Vector3{ cosf(v.x), cosf(v.y), cosf(v.z) };
}

/* Computes sine
 */
inline Vector3 sin(Vector3 v)
{
    return Vector3{ sinf(v.x), sinf(v.y), sinf(v.z) };
}

/* Computes tangent
 */
inline Vector3 tan(Vector3 v)
{
    return Vector3{ tanf(v.x), tanf(v.y), tanf(v.z) };
}

/* Computes hyperbolic cosine
 */
inline Vector3 cosh(Vector3 v)
{
    return Vector3{ coshf(v.x), coshf(v.y), coshf(v.z) };
}

/* Computes hyperbolic sine
 */
inline Vector3 sinh(Vector3 v)
{
    return Vector3{ sinhf(v.x), sinhf(v.y), sinhf(v.z) };
}

/* Computes hyperbolic tangent
 */
inline Vector3 tanh(Vector3 v)
{
    return Vector3{ tanhf(v.x), tanhf(v.y), tanhf(v.z) };
}

/* Computes inverse cosine
 */
inline Vector3 acos(Vector3 v)
{
    return Vector3{ acosf(v.x), acosf(v.y), acosf(v.z) };
}

/* Computes inverse sine
 */
inline Vector3 asin(Vector3 v)
{
    return Vector3{ asinf(v.x), asinf(v.y), asinf(v.z) };
}

/* Computes inverse tangent
 */
inline Vector3 atan(Vector3 v)
{
    return Vector3{
        atanf(v.x),
        atanf(v.y),
        asinf(v.z)
    };
}

/* Computes inverse tangent with 2 args
 */
inline Vector3 atan2(Vector3 a, Vector3 b)
{
    return Vector3{
        atan2f(a.x, b.x),
        atan2f(a.y, b.y),
        atan2f(a.z, b.z)
    };
}

/* Computes Euler number raised to the power 'x'
 */
inline Vector3 exp(Vector3 v)
{
    return Vector3{
        expf(v.x),
        expf(v.y),
        expf(v.z)
    };
}

/* Computes 2 raised to the power 'x'
 */
inline Vector3 exp2(Vector3 v)
{
    return Vector3{
        exp2f(v.x),
        exp2f(v.y),
        exp2f(v.z)
    };
}

/* Computes the base Euler number logarithm
 */
inline Vector3 log(Vector3 v)
{
    return Vector3{
        logf(v.x),
        logf(v.y),
        logf(v.z)
    };
}

/* Computes the base 2 logarithm
 */
inline Vector3 log2(Vector3 v)
{
    return Vector3{
        log2f(v.x),
        log2f(v.y),
        log2f(v.z)
    };
}

/* Computes the base 10 logarithm
 */
inline Vector3 log10(Vector3 v)
{
    return Vector3{
        log10f(v.x),
        log10f(v.y),
        log10f(v.z)
    };
}

/* Computes the value of base raised to the power exponent
 */
inline Vector3 pow(Vector3 a, Vector3 b)
{
    return Vector3{
        powf(a.x, b.x),
        powf(a.y, b.y),
        powf(a.z, b.z)
    };
}

/* Get the fractal part of floating point
 */
inline Vector3 frac(Vector3 v)
{
    return Vector3{
        fracf(v.x),
        fracf(v.y),
        fracf(v.z)
    };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline Vector3 fmod(Vector3 a, Vector3 b)
{
    return Vector3{
        fmodf(a.x, b.x),
        fmodf(a.y, b.y),
        fmodf(a.z, b.z)
    };
}

/* Computes the smallest integer value not less than 'x'
 */
inline Vector3 ceil(Vector3 v)
{
    return Vector3{
        ceilf(v.x),
        ceilf(v.y),
        ceilf(v.z)
    };
}

/* Computes the largest integer value not greater than 'x'
 */
inline Vector3 floor(Vector3 v)
{
    return Vector3{
        floorf(v.x),
        floorf(v.y),
        floorf(v.z)
    };
}

/* Computes the nearest integer value
 */
inline Vector3 round(Vector3 v)
{
    return Vector3{
        roundf(v.x),
        roundf(v.y),
        roundf(v.z)
    };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline Vector3 trunc(Vector3 v)
{
    return Vector3{
        truncf(v.x),
        truncf(v.y),
        truncf(v.z)
    };
}

/* Get the smaller value
 */
inline Vector3 min(Vector3 a, Vector3 b)
{
    return Vector3{
        minf(a.x, b.x),
        minf(a.y, b.y),
        minf(a.z, b.z)
    };
}

/* Get the larger value
 */
inline Vector3 max(Vector3 a, Vector3 b)
{
    return Vector3{
        maxf(a.x, b.x),
        maxf(a.y, b.y),
        maxf(a.z, b.z)
    };
}

/* Clamps the 'x' value to the [min, max].
 */
inline Vector3 clamp(Vector3 v, Vector3 min, Vector3 max)
{
    return Vector3{
        clampf(v.x, min.x, max.x),
        clampf(v.y, min.y, max.y),
        clampf(v.z, min.z, max.z)
    };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline Vector3 saturate(Vector3 v)
{
    return Vector3{
        saturatef(v.x),
        saturatef(v.y),
        saturatef(v.z)
    };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline Vector3 step(Vector3 a, Vector3 b)
{
    return Vector3{
        stepf(a.x, b.x),
        stepf(a.y, b.y),
        stepf(a.z, b.z)
    };
}

/* Performs a linear interpolation.
 */
inline Vector3 lerp(Vector3 a, Vector3 b, Vector3 t)
{
    return Vector3{
        lerpf(a.x, b.x, t.x),
        lerpf(a.y, b.y, t.y),
        lerpf(a.z, b.z, t.z)
    };
}

/* Performs a linear interpolation.
 */
inline Vector3 lerp(Vector3 a, Vector3 b, float t)
{
    return Vector3{
        lerpf(a.x, b.x, t),
        lerpf(a.y, b.y, t),
        lerpf(a.z, b.z, t)
    };
}

/* Compute a smooth Hermite interpolation
 */
inline Vector3 smoothstep(Vector3 a, Vector3 b, Vector3 t)
{
    return Vector3{
        smoothstepf(a.x, b.x, t.x),
        smoothstepf(a.y, b.y, t.y),
        smoothstepf(a.z, b.z, t.z)
    };
}

/* Computes square root of 'x'.
 */
inline Vector3 sqrt(Vector3 v)
{
    return Vector3{
        sqrtf(v.x),
        sqrtf(v.y),
        sqrtf(v.z)
    };
}

/* Computes inverse square root of 'x'.
 */
inline Vector3 rsqrt(Vector3 v)
{
    return Vector3{
        rsqrtf(v.x),
        rsqrtf(v.y),
        rsqrtf(v.z)
    };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector3 fsqrt(Vector3 v)
{
    return Vector3{
        fsqrtf(v.x),
        fsqrtf(v.y),
        fsqrtf(v.z)
    };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector3 frsqrt(Vector3 v)
{
    return Vector3{
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
inline Vector3 cross(Vector3 a, Vector3 b)
{
    return Vector3{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

/* Compute dot product of two vectors
 */
inline float dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* Compute squared length of vector
 */
inline float lengthSquare(Vector3 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(Vector3 v)
{
    return sqrtf(lengthSquare(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(Vector3 a, Vector3 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distanceSquare(Vector3 a, Vector3 b)
{
    return lengthSquare(a - b);
}

/* Compute normalized vector
 */
inline Vector3 normalize(Vector3 v)
{
    const float lsqr = lengthSquare(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return Vector3{ v.x * f, v.y * f, v.z * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline Vector3 reflect(Vector3 v, Vector3 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline Vector3 refract(Vector3 v, Vector3 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? Vector3{ 0.0f, 0.0f, 0.0f }
    : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/* Compute faceforward vector
 */
inline Vector3 faceforward(Vector3 n, Vector3 i, Vector3 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

/* Computes sign of 'x'
*/
inline Vector4 sign(Vector4 v)
{
    return Vector4{
        signf(v.x),
        signf(v.y),
        signf(v.z),
        signf(v.w)
    };
}

/* Computes absolute value
 */
inline Vector4 abs(Vector4 v)
{
    return Vector4{
        fabsf(v.x),
        fabsf(v.y),
        fabsf(v.z),
        fabsf(v.w)
    };
}

/* Computes cosine
 */
inline Vector4 cos(Vector4 v)
{
    return Vector4{
        cosf(v.x),
        cosf(v.y),
        cosf(v.z),
        cosf(v.w)
    };
}

/* Computes sine
 */
inline Vector4 sin(Vector4 v)
{
    return Vector4{
        sinf(v.x),
        sinf(v.y),
        sinf(v.z),
        sinf(v.w)
    };
}

/* Computes tangent
 */
inline Vector4 tan(Vector4 v)
{
    return Vector4{
        tanf(v.x),
        tanf(v.y),
        tanf(v.z),
        tanf(v.w)
    };
}

/* Computes hyperbolic cosine
 */
inline Vector4 cosh(Vector4 v)
{
    return Vector4{
        coshf(v.x),
        coshf(v.y),
        coshf(v.z),
        coshf(v.w)
    };
}

/* Computes hyperbolic sine
 */
inline Vector4 sinh(Vector4 v)
{
    return Vector4{
        sinhf(v.x),
        sinhf(v.y),
        sinhf(v.z),
        sinhf(v.w)
    };
}

/* Computes hyperbolic tangent
 */
inline Vector4 tanh(Vector4 v)
{
    return Vector4{
        tanhf(v.x),
        tanhf(v.y),
        tanhf(v.z),
        tanhf(v.w)
    };
}

/* Computes inverse cosine
 */
inline Vector4 acos(Vector4 v)
{
    return Vector4{
        acosf(v.x),
        acosf(v.y),
        acosf(v.z),
        acosf(v.w)
    };
}

/* Computes inverse sine
 */
inline Vector4 asin(Vector4 v)
{
    return Vector4{
        asinf(v.x),
        asinf(v.y),
        asinf(v.z),
        asinf(v.w)
    };
}

/* Computes inverse tangent
 */
inline Vector4 atan(Vector4 v)
{
    return Vector4{
        atanf(v.x),
        atanf(v.y),
        atanf(v.z),
        atanf(v.w)
    };
}

/* Computes inverse tangent with 2 args
 */
inline Vector4 atan2(Vector4 a, Vector4 b)
{
    return Vector4{ atan2f(a.x, b.x), atan2f(a.y, b.y), atan2f(a.z, b.z), atan2f(a.w, b.w) };
}

/* Computes Euler number raised to the power 'x'
 */
inline Vector4 exp(Vector4 v)
{
    return Vector4{ expf(v.x), expf(v.y), expf(v.z), expf(v.w) };
}

/* Computes 2 raised to the power 'x'
 */
inline Vector4 exp2(Vector4 v)
{
    return Vector4{ exp2f(v.x), exp2f(v.y), exp2f(v.z), exp2f(v.w) };
}

/* Computes the base Euler number logarithm
 */
inline Vector4 log(Vector4 v)
{
    return Vector4{ logf(v.x), logf(v.y), logf(v.z), logf(v.w) };
}

/* Computes the base 2 logarithm
 */
inline Vector4 log2(Vector4 v)
{
    return Vector4{ log2f(v.x), log2f(v.y), log2f(v.z), log2f(v.w) };
}

/* Computes the base 10 logarithm
 */
inline Vector4 log10(Vector4 v)
{
    return Vector4{ log10f(v.x), log10f(v.y), log10f(v.z), log10f(v.w) };
}

/* Computes the value of base raised to the power exponent
 */
inline Vector4 pow(Vector4 a, Vector4 b)
{
    return Vector4{
        powf(a.x, b.x),
        powf(a.y, b.y),
        powf(a.z, b.z),
        powf(a.w, b.w)
    };
}

/* Get the fractal part of floating point
 */
inline Vector4 frac(Vector4 v)
{
    return Vector4{
        fracf(v.x),
        fracf(v.y),
        fracf(v.z),
        fracf(v.w)
    };
}

/* Computes the floating-point remainder of the division operation x/y
 */
inline Vector4 fmod(Vector4 a, Vector4 b)
{
    return Vector4{
        fmodf(a.x, b.x),
        fmodf(a.y, b.y),
        fmodf(a.z, b.z),
        fmodf(a.w, b.w)
    };
}

/* Computes the smallest integer value not less than 'x'
 */
inline Vector4 ceil(Vector4 v)
{
    return Vector4{
        ceilf(v.x),
        ceilf(v.y),
        ceilf(v.z),
        ceilf(v.w)
    };
}

/* Computes the largest integer value not greater than 'x'
 */
inline Vector4 floor(Vector4 v)
{
    return Vector4{
        floorf(v.x),
        floorf(v.y),
        floorf(v.z),
        floorf(v.w)
    };
}

/* Computes the nearest integer value
 */
inline Vector4 round(Vector4 v)
{
    return Vector4{
        roundf(v.x),
        roundf(v.y),
        roundf(v.z),
        roundf(v.w)
    };
}

/* Computes the nearest integer not greater in magnitude than 'x'
 */
inline Vector4 trunc(Vector4 v)
{
    return Vector4{
        truncf(v.x),
        truncf(v.y),
        truncf(v.z),
        truncf(v.w)
    };
}

/* Get the smaller value
 */
inline Vector4 min(Vector4 a, Vector4 b)
{
    return Vector4{
        minf(a.x, b.x),
        minf(a.y, b.y),
        minf(a.z, b.z),
        minf(a.w, b.w)
    };
}

/* Get the larger value
 */
inline Vector4 max(Vector4 a, Vector4 b)
{
    return Vector4{
        maxf(a.x, b.x),
        maxf(a.y, b.y),
        maxf(a.z, b.z),
        maxf(a.w, b.w)
    };
}

/* Clamps the 'x' value to the [min, max].
 */
inline Vector4 clamp(Vector4 v, Vector4 min, Vector4 max)
{
    return Vector4{
        clampf(v.x, min.x, max.x),
        clampf(v.y, min.y, max.y),
        clampf(v.z, min.z, max.z),
        clampf(v.w, min.w, max.w)
    };
}

/* Clamps the specified value within the range of 0 to 1
 */
inline Vector4 saturate(Vector4 v)
{
    return Vector4{
        saturatef(v.x),
        saturatef(v.y),
        saturatef(v.z),
        saturatef(v.w)
    };
}

/* Compares two values, returning 0 or 1 based on which value is greater.
 */
inline Vector4 step(Vector4 a, Vector4 b)
{
    return Vector4{
        stepf(a.x, b.x),
        stepf(a.y, b.y),
        stepf(a.z, b.z),
        stepf(a.w, b.w)
    };
}

/* Performs a linear interpolation.
 */
inline Vector4 lerp(Vector4 a, Vector4 b, Vector4 t)
{
    return Vector4{
        lerpf(a.x, b.x, t.x),
        lerpf(a.y, b.y, t.y),
        lerpf(a.z, b.z, t.z),
        lerpf(a.w, b.w, t.w)
    };
}

/* Performs a linear interpolation.
 */
inline Vector4 lerp(Vector4 a, Vector4 b, float t)
{
    return Vector4{
        lerpf(a.x, b.x, t),
        lerpf(a.y, b.y, t),
        lerpf(a.z, b.z, t),
        lerpf(a.w, b.w, t)
    };
}

/* Compute a smooth Hermite interpolation
 */
inline Vector4 smoothstep(Vector4 a, Vector4 b, Vector4 t)
{
    return Vector4{
        smoothstepf(a.x, b.x, t.x),
        smoothstepf(a.y, b.y, t.y),
        smoothstepf(a.z, b.z, t.z),
        smoothstepf(a.w, b.w, t.w)
    };
}

/* Computes square root of 'x'.
 */
inline Vector4 sqrt(Vector4 v)
{
    return Vector4{ sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w) };
}

/* Computes inverse square root of 'x'.
 */
inline Vector4 rsqrt(Vector4 v)
{
    return Vector4{ rsqrtf(v.x), rsqrtf(v.y), rsqrtf(v.z), rsqrtf(v.w) };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector4 fsqrt(Vector4 v)
{
    return Vector4{ fsqrtf(v.x), fsqrtf(v.y), fsqrtf(v.z), fsqrtf(v.w) };
}

/* Computes fast inverse square root of 'x'.
 */
inline Vector4 frsqrt(Vector4 v)
{
    return Vector4{ frsqrtf(v.x), frsqrtf(v.y), frsqrtf(v.z), frsqrtf(v.w) };
}

//
// @region: Graphics functions
//

/* Compute dot product of two vectors
 */
inline float dot(Vector4 a, Vector4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/* Compute squared length of vector
 */
inline float lengthSquare(Vector4 v)
{
    return dot(v, v);
}

/* Compute length of vector
 */
inline float length(Vector4 v)
{
    return sqrtf(lengthSquare(v));
}

/* Compute distance from 'a' to b
 */
inline float distance(Vector4 a, Vector4 b)
{
    return length(a - b);
}

/* Compute squared distance from 'a' to b
 */
inline float distanceSquare(Vector4 a, Vector4 b)
{
    return lengthSquare(a - b);
}

/* Compute normalized vector
 */
inline Vector4 normalize(Vector4 v)
{
    const float lsqr = lengthSquare(v);
    if (lsqr > 0.0f)
    {
        const float f = rsqrtf(lsqr);
        return Vector4{ v.x * f, v.y * f, v.z * f, v.w * f };
    }
    else
    {
        return v;
    }
}

/* Compute reflection vector
 */
inline Vector4 reflect(Vector4 v, Vector4 n)
{
    return v - 2.0f * dot(v, n) * n;
}

/* Compute refraction vector
 */
inline Vector4 refract(Vector4 v, Vector4 n, float eta)
{
    const float k = 1.0f - eta * eta * (1.0f - dot(v, n) * dot(v, n));
    return k < 0.0f
        ? Vector4{ 0.0f, 0.0f, 0.0f, 0.0f }
    : eta * v - (eta * dot(v, n) + sqrtf(k)) * n;
}

/* Compute faceforward vector
 */
inline Vector4 faceforward(Vector4 n, Vector4 i, Vector4 nref)
{
    return dot(i, nref) < 0.0f ? n : -n;
}

//
// @region: Quaternion
//

/// Quaternion multiplication
inline Quaternion mul(Quaternion a, Quaternion b)
{
    const Vector3 a3 = Vector3{ a.x, a.y, a.z };
    const Vector3 b3 = Vector3{ b.x, b.y, b.z };

    Vector3 v = a3 * b.w + b3 * a.w + cross(a3, b3);
    float  w = a.w * b.w - dot(a3, b3);
    return Quaternion{ v.x, v.y, v.z, w };
}


inline Vector3 mul(Quaternion q, Vector3 v)
{
    const Vector3   quatVector = Vector3{ q.x, q.y, q.z };
    const float     quatScalar = q.w;

    return quatVector * 2.0f * dot(quatVector, v)
         + v * (quatScalar * quatScalar - lengthSquare(quatVector))
         + cross(quatVector, v) * 2.0f * quatScalar;
}

inline Quaternion inverse(Quaternion q)
{
    return Quaternion{ q.x, q.y, q.z, -q.w };
}

inline Quaternion conj(Quaternion q)
{
    return Quaternion{ -q.x, -q.y, -q.z, q.w };
}

//
// @region: Quaternion
//

inline Vector4 mul(Matrix4 a, Vector4 b)
{
    const Vector4 c0 = Vector4{ a.data[0][0], a.data[1][0], a.data[2][0], a.data[3][0] };
    const Vector4 c1 = Vector4{ a.data[0][1], a.data[1][1], a.data[2][1], a.data[3][1] };
    const Vector4 c2 = Vector4{ a.data[0][2], a.data[1][2], a.data[2][2], a.data[3][2] };
    const Vector4 c3 = Vector4{ a.data[0][3], a.data[1][3], a.data[2][3], a.data[3][3] };

    return Vector4{
        dot(c0, b),
        dot(c1, b),
        dot(c2, b),
        dot(c3, b)
    };
}

inline Vector4 mul(Vector4 a, Matrix4 b)
{
    const Vector4 c0 = Vector4{ b.data[0][0], b.data[0][1], b.data[0][2], b.data[0][3] };
    const Vector4 c1 = Vector4{ b.data[1][0], b.data[1][1], b.data[1][2], b.data[1][3] };
    const Vector4 c2 = Vector4{ b.data[2][0], b.data[2][1], b.data[2][2], b.data[2][3] };
    const Vector4 c3 = Vector4{ b.data[3][0], b.data[3][1], b.data[3][2], b.data[3][3] };

    return Vector4{
        dot(a, c0),
        dot(a, c1),
        dot(a, c2),
        dot(a, c3)
    };
}

inline Vector3 mul(Matrix4 a, Vector3 b)
{
    const Vector4 b0 = Vector4{ b.x, b.y, b.z, 1.0f };
    const Vector4 b1 = mul(a, b0);

    const float iw = 1.0f / b1.w;
    return Vector3{ b1.x * iw, b1.y * iw, b1.z * iw };
}

inline Vector3 mul(Vector3 a, Matrix4 b)
{
    const Vector4 a0 = Vector4{ a.x, a.y, a.z, 1.0f };
    const Vector4 a1 = mul(a0, b);

    const float iw = 1.0f / a1.w;
    return Vector3{ a1.x * iw, a1.y * iw, a1.z * iw };
}

inline Matrix4 mul(Matrix4 a, Matrix4 b)
{
    const Vector4 c0 = Vector4{ b.data[0][0], b.data[0][1], b.data[0][2], b.data[0][3] };
    const Vector4 c1 = Vector4{ b.data[1][0], b.data[1][1], b.data[1][2], b.data[1][3] };
    const Vector4 c2 = Vector4{ b.data[2][0], b.data[2][1], b.data[2][2], b.data[2][3] };
    const Vector4 c3 = Vector4{ b.data[3][0], b.data[3][1], b.data[3][2], b.data[3][3] };

    const Vector4 v0 = mul(a, c0);
    const Vector4 v1 = mul(a, c1);
    const Vector4 v2 = mul(a, c2);
    const Vector4 v3 = mul(a, c3);

    return Matrix4{
        v0.x, v0.y, v0.z, v0.w,
        v1.x, v1.y, v1.z, v1.w,
        v2.x, v2.y, v2.z, v2.w,
        v3.x, v3.y, v3.z, v3.w,
    };
}

// -----------------------------------------
// Math objects creation functions
// @note: use PascalCase for function name
// -----------------------------------------

inline Vector4 QuaternionToAxisAngle(Quaternion q)
{
    const float ilen = 1.0f / sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    const Vector4 c = q.w != 0.0f
        ? Vector4{ q.x * ilen, q.y * ilen, q.z * ilen, q.w * ilen }
        : Vector4{ q.x, q.y, q.z, q.w };

    const float den = sqrtf(1.0f - q.w * q.w);
    const Vector3 axis = (den > 0.0001f)
        ? Vector3{ c.x / den, c.y / den, c.z / den }
        : Vector3{ 1, 0, 0 };

    float angle = 2.0f * cosf(c.w);
    return Vector4{ axis.x, axis.y, axis.z, angle };
}

inline Quaternion QuaternionFromAxisAngle(Vector3 axis, float angle)
{
    return Quaternion{};
}

inline Matrix4 Matrix4Ortho(float l, float r, float b, float t, float n, float f)
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

inline Matrix4 Matrix4Frustum(float l, float r, float b, float t, float n, float f)
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

inline Matrix4 Matrix4Perspective(float fov, float aspect, float znear, float zfar)
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

inline Matrix4 Matrix4Scalation(float x, float y, float z = 1.0f)
{
    return {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
}

inline Matrix4 Matrix4Scalation(float s)
{
    return Matrix4Scalation(s, s, s);
}

inline Matrix4 Matrix4Scalation(Vector2 v)
{
    return Matrix4Scalation(v.x, v.y, 0);
}

inline Matrix4 Matrix4Scalation(Vector3 v)
{
    return Matrix4Scalation(v.x, v.y, v.z);
}

inline Matrix4 Matrix4Translation(float x, float y, float z = 0.0f)
{
    return {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
}

inline Matrix4 Matrix4Translation(Vector2 v)
{
    return Matrix4Translation(v.x, v.y, 0);
}

inline Matrix4 Matrix4Translation(Vector3 v)
{
    return Matrix4Translation(v.x, v.y, v.z);
}

inline Matrix4 Matrix4Rotation(float x, float y, float z, float angle)
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

inline Matrix4 Matrix4Rotation(Vector3 axis, float angle)
{
    return Matrix4Rotation(axis.x, axis.y, axis.z, angle);
}

inline Matrix4 Matrix4RotationX(float angle)
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

inline Matrix4 Matrix4RotationY(float angle)
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

inline Matrix4 Matrix4RotationZ(float angle)
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

inline Matrix4 Matrix4Rotation(Quaternion quaternion)
{
    Vector4 axisAngle = QuaternionToAxisAngle(quaternion);
    return Matrix4Rotation(axisAngle.x, axisAngle.y, axisAngle.z, axisAngle.w);
}

inline Matrix4 Matrix4Transform(Vector2 position, float rotation, Vector2 scale)
{
    return mul(mul(Matrix4Translation(position), Matrix4RotationZ(rotation)), Matrix4Scalation(scale));
}

inline Matrix4 Matrix4Transform(Vector3 position, Quaternion rotation, Vector3 scale)
{
    return mul(mul(Matrix4Translation(position), Matrix4Rotation(rotation)), Matrix4Scalation(scale));
}

inline Matrix4 Matrix4Transform2D(Vector2 position, float rotation, Vector2 scale, Vector2 pivot)
{
    Matrix4 translation = Matrix4Translation(position - pivot);
    Matrix4 rotationMat4 = mul(mul(Matrix4Translation(pivot.x, pivot.y), Matrix4RotationZ(rotation)), Matrix4Translation(-pivot.x, -pivot.y));
    Matrix4 scalation = Matrix4Scalation(scale);

    return mul(mul(translation, rotationMat4), scalation);
}
