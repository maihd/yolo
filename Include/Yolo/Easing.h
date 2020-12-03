#pragma once

#include <math.h>
#include <Yolo/Core.h>

// Linear

inline float EaseLinear(float s, float e, float t)
{
    return s + (e - s) * t;
}

inline float EaseLinear(float s, float e, float t, float d)
{
    return EaseLinear(s, e, t / d);
}

inline Vector2 EaseLinear(Vector2 s, Vector2 e, float t)
{
    Vector2 r;
    r.x = EaseLinear(s.x, e.x, t);
    r.y = EaseLinear(s.y, e.y, t);
    return r;
}

inline Vector2 EaseLinear(Vector2 s, Vector2 e, float t, float d)
{
    return EaseLinear(s, e, t / d);
}

inline Vector3 EaseLinear(Vector3 s, Vector3 e, float t)
{
    Vector3 r;
    r.x = EaseLinear(s.x, e.x, t);
    r.y = EaseLinear(s.y, e.y, t);
    r.z = EaseLinear(s.z, e.z, t);
    return r;
}

inline Vector3 EaseLinear(Vector3 s, Vector3 e, float t, float d)
{
    return EaseLinear(s, e, t / d);
}

// Quad

inline float EaseQuadIn(float s, float e, float t)
{
    return s + (e - s) * t * t;
}

inline float EaseQuadOut(float s, float e, float t)
{
    return s + (s - e) * t * (t - 2.0f);
}

inline float EaseQuadInOut(float s, float e, float t)
{
    return (t *= 2.0f) < 1.0f
        ? s + (e - s) * 0.5f * t * t
        : s + (s - e) * 0.5f * ((t -= 1.0f) * (t - 2.0f) - 1.0f);
}

inline float EaseQuadIn(float s, float e, float t, float d)
{
    return EaseQuadIn(s, e, t / d);
}

inline float EaseQuadOut(float s, float e, float t, float d)
{
    return EaseQuadOut(s, e, t / d);
}

inline float EaseQuadInOut(float s, float e, float t, float d)
{
    return EaseQuadInOut(s, e, t / d);
}

inline Vector2 EaseQuadIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuadIn(s.x, e.x, t),
        EaseQuadIn(s.y, e.y, t),
    };
}

inline Vector3 EaseQuadIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuadIn(s.x, e.x, t),
        EaseQuadIn(s.y, e.y, t),
        EaseQuadIn(s.z, e.z, t),
    };
}

inline Vector2 EaseQuadOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuadOut(s.x, e.x, t),
        EaseQuadOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuadOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuadOut(s.x, e.x, t),
        EaseQuadOut(s.y, e.y, t),
        EaseQuadOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuadInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuadInOut(s.x, e.x, t),
        EaseQuadInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuadInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuadInOut(s.x, e.x, t),
        EaseQuadInOut(s.y, e.y, t),
        EaseQuadInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuadIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuadIn(s, e, t / d);
}

inline Vector3 EaseQuadIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuadIn(s, e, t / d);
}

inline Vector2 EaseQuadOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuadOut(s, e, t / d);
}

inline Vector3 EaseQuadOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuadOut(s, e, t / d);
}

inline Vector2 EaseQuadInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuadInOut(s, e, t / d);
}

inline Vector3 EaseQuadInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuadInOut(s, e, t / d);
}

// Cubic

inline float EaseCubicIn(float s, float e, float t)
{
    return s + (e - s) * t * t * t;
}

inline float EaseCubicOut(float s, float e, float t)
{
    return s + (e - s) * ((t -= 1.0f) * t * t + 1.0f);
}

inline float EaseCubicInOut(float s, float e, float t)
{
    return (t *= 2.0f) < 1.0f
        ? s + (e - s) * 0.5f * t * t * t
        : s + (e - s) * 0.5f * ((t -= 2.0f) * t * t + 2.0f);
}

inline float EaseCubicIn(float s, float e, float t, float d)
{
    return EaseCubicIn(s, e, t / d);
}

inline float EaseCubicOut(float s, float e, float t, float d)
{
    return EaseCubicOut(s, e, t / d);
}

inline float CubicInOut(float s, float e, float t, float d)
{
    return EaseCubicInOut(s, e, t / d);
}

inline Vector2 EaseCubicIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCubicIn(s.x, e.x, t),
        EaseCubicIn(s.y, e.y, t),
    };
}

inline Vector3 EaseCubicIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCubicIn(s.x, e.x, t),
        EaseCubicIn(s.y, e.y, t),
        EaseCubicIn(s.z, e.z, t),
    };
}

inline Vector2 EaseCubicOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCubicOut(s.x, e.x, t),
        EaseCubicOut(s.y, e.y, t),
    };
}

inline Vector3 EaseCubicOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCubicOut(s.x, e.x, t),
        EaseCubicOut(s.y, e.y, t),
        EaseCubicOut(s.z, e.z, t),
    };
}

inline Vector2 EaseCubicInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCubicInOut(s.x, e.x, t),
        EaseCubicInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseCubicInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCubicInOut(s.x, e.x, t),
        EaseCubicInOut(s.y, e.y, t),
        EaseCubicInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseCubicIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCubicIn(s, e, t / d);
}

inline Vector3 EaseCubicIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCubicIn(s, e, t / d);
}

inline Vector2 EaseCubicOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCubicOut(s, e, t / d);
}

inline Vector3 EaseCubicOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCubicOut(s, e, t / d);
}

inline Vector2 EaseCubicInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCubicInOut(s, e, t / d);
}

inline Vector3 EaseCubicInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCubicInOut(s, e, t / d);
}

// Quart

inline float EaseQuartIn(float s, float e, float t)
{
    return s + (e - s) * t * t * t * t;
}

inline float EaseQuartOut(float s, float e, float t)
{
    return s + (s - e) * ((t -= 1.0f) * t * t * t - 1.0f);
}

inline float EaseQuartInOut(float s, float e, float t)
{
    return (t *= 2.0f) < 1.0f
        ? s + (e - s) * 0.5f * t * t * t * t
        : s + (s - e) * 0.5f * ((t -= 2.0f) * t * t * t - 2.0f);
}

inline float EaseQuartIn(float s, float e, float t, float d)
{
    return EaseQuartIn(s, e, t / d);
}

inline float EaseQuartOut(float s, float e, float t, float d)
{
    return EaseQuartOut(s, e, t / d);
}

inline float EaseQuartInOut(float s, float e, float t, float d)
{
    return EaseQuartInOut(s, e, t / d);
}

inline Vector2 EaseQuartIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuartIn(s.x, e.x, t),
        EaseQuartIn(s.y, e.y, t),
    };
}

inline Vector3 EaseQuartIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuartIn(s.x, e.x, t),
        EaseQuartIn(s.y, e.y, t),
        EaseQuartIn(s.z, e.z, t),
    };
}

inline Vector2 EaseQuartOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuartOut(s.x, e.x, t),
        EaseQuartOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuartOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuartOut(s.x, e.x, t),
        EaseQuartOut(s.y, e.y, t),
        EaseQuartOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuartInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuartInOut(s.x, e.x, t),
        EaseQuartInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuartInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuartInOut(s.x, e.x, t),
        EaseQuartInOut(s.y, e.y, t),
        EaseQuartInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuartIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuartIn(s, e, t / d);
}

inline Vector3 EaseQuartIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuartIn(s, e, t / d);
}

inline Vector2 EaseQuartOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuartOut(s, e, t / d);
}

inline Vector3 EaseQuartOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuartOut(s, e, t / d);
}

inline Vector2 EaseQuartInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuartInOut(s, e, t / d);
}

inline Vector3 EaseQuartInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuartInOut(s, e, t / d);
}

// @Quint

inline float EaseQuintIn(float s, float e, float t)
{
    return s + (e - s) * t * t * t * t * t;
}

inline float EaseQuintOut(float s, float e, float t)
{
    return s + (e - s) * ((t -= 1.0f) * t * t * t * t + 1.0f);
}

inline float EaseQuintInOut(float s, float e, float t)
{
    return (t *= 2.0f) < 1.0f
        ? s + (e - s) * 0.5f * t * t * t * t * t
        : s + (e - s) * 0.5f * ((t -= 2.0f) * t * t * t * t + 2.0f);
}

inline float EaseQuintIn(float s, float e, float t, float d)
{
    return EaseQuintIn(s, e, t / d);
}

inline float EaseQuintOut(float s, float e, float t, float d)
{
    return EaseQuintOut(s, e, t / d);
}

inline float EaseQuintInOut(float s, float e, float t, float d)
{
    return EaseQuintInOut(s, e, t / d);
}

inline Vector2 EaseQuintIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuintIn(s.x, e.x, t),
        EaseQuintIn(s.y, e.y, t),
    };
}

inline Vector3 EaseQuintIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuintIn(s.x, e.x, t),
        EaseQuintIn(s.y, e.y, t),
        EaseQuintIn(s.z, e.z, t),
    };
}

inline Vector2 EaseQuintOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuintOut(s.x, e.x, t),
        EaseQuintOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuintOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuintOut(s.x, e.x, t),
        EaseQuintOut(s.y, e.y, t),
        EaseQuintOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuintInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseQuintInOut(s.x, e.x, t),
        EaseQuintInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseQuintInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseQuintInOut(s.x, e.x, t),
        EaseQuintInOut(s.y, e.y, t),
        EaseQuintInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseQuintIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuintIn(s, e, t / d);
}

inline Vector3 EaseQuintIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuintIn(s, e, t / d);
}

inline Vector2 EaseQuintOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuintOut(s, e, t / d);
}

inline Vector3 EaseQuintOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuintOut(s, e, t / d);
}

inline Vector2 EaseQuintInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseQuintInOut(s, e, t / d);
}

inline Vector3 EaseQuintInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseQuintInOut(s, e, t / d);
}

// @Sine

inline float EaseSineIn(float s, float e, float t)
{
    return e + (s - e) * cosf(t * PI * 0.5f);
}

inline float EaseSineOut(float s, float e, float t)
{
    return s + (e - s) * sinf(t * PI * 0.5f);
}

inline float EaseSineInOut(float s, float e, float t)
{
    return s + (s - e) * 0.5f * (cosf(t * PI) - 1.0f);
}

inline float EaseSineIn(float s, float e, float t, float d)
{
    return EaseSineIn(s, e, t / d);
}

inline float EaseSineOut(float s, float e, float t, float d)
{
    return EaseSineOut(s, e, t / d);
}

inline float EaseSineInOut(float s, float e, float t, float d)
{
    return EaseSineInOut(s, e, t / d);
}

inline Vector2 EaseSineIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseSineIn(s.x, e.x, t),
        EaseSineIn(s.y, e.y, t),
    };
}

inline Vector3 EaseSineIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseSineIn(s.x, e.x, t),
        EaseSineIn(s.y, e.y, t),
        EaseSineIn(s.z, e.z, t),
    };
}

inline Vector2 EaseSineOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseSineOut(s.x, e.x, t),
        EaseSineOut(s.y, e.y, t),
    };
}

inline Vector3 EaseSineOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseSineOut(s.x, e.x, t),
        EaseSineOut(s.y, e.y, t),
        EaseSineOut(s.z, e.z, t),
    };
}

inline Vector2 EaseSineInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseSineInOut(s.x, e.x, t),
        EaseSineInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseSineInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseSineInOut(s.x, e.x, t),
        EaseSineInOut(s.y, e.y, t),
        EaseSineInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseSineIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseSineIn(s, e, t / d);
}

inline Vector3 EaseSineIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseSineIn(s, e, t / d);
}

inline Vector2 EaseSineOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseSineOut(s, e, t / d);
}

inline Vector3 EaseSineOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseSineOut(s, e, t / d);
}

inline Vector2 EaseSineInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseSineInOut(s, e, t / d);
}

inline Vector3 EaseSineInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseSineInOut(s, e, t / d);
}

// @Expo

inline float EaseExpoIn(float s, float e, float t)
{
    return t <= 0.0f ? s : s + (e - s) * powf(2.0f, 10.0f * (t - 1.0f));
}

inline float EaseExpoOut(float s, float e, float t)
{
    return t >= 1.0f ? e : s + (e - s) * (1.0f - powf(2.0f, -10.0f * t));
}

inline float EaseExpoInOut(float s, float e, float t)
{
    if (t <= 0.0f) return s;
    if (t >= 1.0f) return e;

    return t < 0.5f
        ? EaseExpoIn(s, s + (e - s) * 0.5f, t * 2.0f)
        : EaseExpoOut(s + (e - s) * 0.5f, e, (t - 0.5f) * 2.0f);
}

inline float EaseExpoIn(float s, float e, float t, float d)
{
    return EaseExpoIn(s, e, t / d);
}

inline float EaseExpoOut(float s, float e, float t, float d)
{
    return EaseExpoOut(s, e, t / d);
}

inline float EaseExpoInOut(float s, float e, float t, float d)
{
    return EaseExpoInOut(s, e, t / d);
}

inline Vector2 EaseExpoIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseExpoIn(s.x, e.x, t),
        EaseExpoIn(s.y, e.y, t),
    };
}

inline Vector3 EaseExpoIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseExpoIn(s.x, e.x, t),
        EaseExpoIn(s.y, e.y, t),
        EaseExpoIn(s.z, e.z, t),
    };
}

inline Vector2 EaseExpoOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseExpoOut(s.x, e.x, t),
        EaseExpoOut(s.y, e.y, t),
    };
}

inline Vector3 EaseExpoOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseExpoOut(s.x, e.x, t),
        EaseExpoOut(s.y, e.y, t),
        EaseExpoOut(s.z, e.z, t),
    };
}

inline Vector2 EaseExpoInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseExpoInOut(s.x, e.x, t),
        EaseExpoInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseExpoInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
         EaseExpoInOut(s.x, e.x, t),
         EaseExpoInOut(s.y, e.y, t),
         EaseExpoInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseExpoIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseExpoIn(s, e, t / d);
}

inline Vector3 EaseExpoIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseExpoIn(s, e, t / d);
}

inline Vector2 EaseExpoOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseExpoOut(s, e, t / d);
}

inline Vector3 EaseExpoOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseExpoOut(s, e, t / d);
}

inline Vector2 EaseExpoInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseExpoInOut(s, e, t / d);
}

inline Vector3 EaseExpoInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseExpoInOut(s, e, t / d);
}

// @Circle

inline float EaseCircleIn(float s, float e, float t)
{
    return s + (s - e) * (sqrtf(1.0f - t * t) - 1.0f);
}

inline float EaseCircleOut(float s, float e, float t)
{
    return s + (e - s) * (sqrtf(1.0f - (t - 1.0f) * (t - 1.0f)));
}

inline float EaseCircleInOut(float s, float e, float t)
{
    return (t *= 2.0f) < 1.0f
        ? s + (s - e) * 0.5f * (sqrtf(1.0f - t * t) - 1.0f)
        : s + (e - s) * 0.5f * (sqrtf(1.0f - (t - 2.0f) * (t - 2.0f)) + 1.0f);
}

inline float EaseCircleIn(float s, float e, float t, float d)
{
    return EaseCircleIn(s, e, t / d);
}

inline float EaseCircleOut(float s, float e, float t, float d)
{
    return EaseCircleOut(s, e, t / d);
}

inline float EaseCircleInOut(float s, float e, float t, float d)
{
    return EaseCircleInOut(s, e, t / d);
}

inline Vector2 EaseCircleIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCircleIn(s.x, e.x, t),
        EaseCircleIn(s.y, e.y, t),
    };
}

inline Vector3 EaseCircleIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCircleIn(s.x, e.x, t),
        EaseCircleIn(s.y, e.y, t),
        EaseCircleIn(s.z, e.z, t),
    };
}

inline Vector2 EaseCircleOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCircleOut(s.x, e.x, t),
        EaseCircleOut(s.y, e.y, t),
    };
}

inline Vector3 EaseCircleOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCircleOut(s.x, e.x, t),
        EaseCircleOut(s.y, e.y, t),
        EaseCircleOut(s.z, e.z, t),
    };
}

inline Vector2 EaseCircleInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseCircleInOut(s.x, e.x, t),
        EaseCircleInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseCircleInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseCircleInOut(s.x, e.x, t),
        EaseCircleInOut(s.y, e.y, t),
        EaseCircleInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseCircleIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCircleIn(s, e, t / d);
}

inline Vector3 EaseCircleIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCircleIn(s, e, t / d);
}

inline Vector2 EaseCircleOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCircleOut(s, e, t / d);
}

inline Vector3 EaseCircleOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCircleOut(s, e, t / d);
}

inline Vector2 EaseCircleInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseCircleInOut(s, e, t / d);
}

inline Vector3 EaseCircleInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseCircleInOut(s, e, t / d);
}

// @Elastic

inline float EaseElasticIn(float s, float e, float t)
{
    if (t <= 0.0f) return s;
    if (t >= 1.0f) return e;

    const float a = (e - s);
    const float b = 0.3f;
    const float c = (a < 0) ? (b * 0.25f) : b;
    const float p = powf(2, 10.0f * (t -= 1.0f));
    const float q = sinf((t - c) * (2 * PI) / b);
    return s - (a * p * q);
}

inline float EaseElasticOut(float s, float e, float t)
{
    if (t <= 0.0f) return s;
    if (t >= 1.0f) return e;

    const float a = (e - s);
    const float b = 0.3f;
    const float c = (a < 0) ? (b * 0.25f) : b;
    const float p = powf(2, -10.0f * t);
    const float q = sinf((t - c) * (2.0f * PI) / b);
    return e + (a * p * q);
}

inline float EaseElasticInOut(float s, float e, float t)
{
    if (t <= 0.0f) return s;
    if (t >= 1.0f) return e;

    const float a = (e - s);
    const float b = 0.3f * 0.15f;
    const float c = (a < 0) ? (b * 0.25f) : b;
    if (t < 1.0f)
    {
        const float p = powf(2, 10.0f * (t -= 1.0f));
        const float q = sinf((t - c) * (2.0f * PI) / b);
        return s - 0.5f * (a * p * q);
    }
    else
    {
        const float p = powf(2, -10.0f * (t -= 1.0f));
        const float q = sinf((t - c) * (2.0f * PI) / b);
        return e + 0.5f * (a * p * q);
    }
}

inline float EaseElasticIn(float s, float e, float t, float d)
{
    return EaseElasticIn(s, e, t / d);
}

inline float EaseElasticOut(float s, float e, float t, float d)
{
    return EaseElasticOut(s, e, t / d);
}

inline float EaseElasticInOut(float s, float e, float t, float d)
{
    return EaseElasticInOut(s, e, t / d);
}

inline Vector2 EaseElasticIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseElasticIn(s.x, e.x, t),
        EaseElasticIn(s.y, e.y, t),
    };
}

inline Vector3 EaseElasticIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseElasticIn(s.x, e.x, t),
        EaseElasticIn(s.y, e.y, t),
        EaseElasticIn(s.z, e.z, t),
    };
}

inline Vector2 EaseElasticOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseElasticOut(s.x, e.x, t),
        EaseElasticOut(s.y, e.y, t),
    };
}

inline Vector3 EaseElasticOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseElasticOut(s.x, e.x, t),
        EaseElasticOut(s.y, e.y, t),
        EaseElasticOut(s.z, e.z, t),
    };
}

inline Vector2 EaseElasticInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseElasticInOut(s.x, e.x, t),
        EaseElasticInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseElasticInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseElasticInOut(s.x, e.x, t),
        EaseElasticInOut(s.y, e.y, t),
        EaseElasticInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseElasticIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseElasticIn(s, e, t / d);
}

inline Vector3 EaseElasticIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseElasticIn(s, e, t / d);
}

inline Vector2 EaseElasticOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseElasticOut(s, e, t / d);
}

inline Vector3 EaseElasticOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseElasticOut(s, e, t / d);
}

inline Vector2 EaseElasticInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseElasticInOut(s, e, t / d);
}

inline Vector3 EaseElasticInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseElasticInOut(s, e, t / d);
}

// @Bounce

inline float EaseBounceOut(float s, float e, float t);
inline float EaseBounceIn(float s, float e, float t)
{
    return e - EaseBounceOut(s, e, 1.0f - t);
}

inline float EaseBounceOut(float s, float e, float t)
{
    const float c = (e - s);
    if (t < 1.0f / 2.75f)
    {
        return c * (7.5625f * t * t) + s;
    }
    else if (t < 2.0f / 2.75f)
    {
        return c * (7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f) + s;
    }
    else if (t < 2.5f / 2.75f)
    {
        return c * (7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f) + s;
    }
    else
    {
        return c * (7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f) + s;
    }
}

inline float EaseBounceInOut(float s, float e, float t)
{
    return t < 0.5f
        ? EaseBounceIn(s, s + (e - s) * 0.5f, t * 2.0f)
        : EaseBounceOut(s + (e - s) * 0.5f, e, (t - 0.5f) * 2.0f);
}

inline float EaseBounceIn(float s, float e, float t, float d)
{
    return EaseBounceIn(s, e, t / d);
}

inline float EaseBounceOut(float s, float e, float t, float d)
{
    return EaseBounceOut(s, e, t / d);
}

inline float EaseBounceInOut(float s, float e, float t, float d)
{
    return EaseBounceInOut(s, e, t / d);
}

inline Vector2 EaseBounceIn(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseBounceIn(s.x, e.x, t),
        EaseBounceIn(s.y, e.y, t),
    };
}

inline Vector3 EaseBounceIn(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseBounceIn(s.x, e.x, t),
        EaseBounceIn(s.y, e.y, t),
        EaseBounceIn(s.z, e.z, t),
    };
}

inline Vector2 EaseBounceOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseBounceOut(s.x, e.x, t),
        EaseBounceOut(s.y, e.y, t),
    };
}

inline Vector3 EaseBounceOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseBounceOut(s.x, e.x, t),
        EaseBounceOut(s.y, e.y, t),
        EaseBounceOut(s.z, e.z, t),
    };
}

inline Vector2 EaseBounceInOut(Vector2 s, Vector2 e, float t)
{
    return Vector2{
        EaseBounceInOut(s.x, e.x, t),
        EaseBounceInOut(s.y, e.y, t),
    };
}

inline Vector3 EaseBounceInOut(Vector3 s, Vector3 e, float t)
{
    return Vector3{
        EaseBounceInOut(s.x, e.x, t),
        EaseBounceInOut(s.y, e.y, t),
        EaseBounceInOut(s.z, e.z, t),
    };
}

inline Vector2 EaseBounceIn(Vector2 s, Vector2 e, float t, float d)
{
    return EaseBounceIn(s, e, t / d);
}

inline Vector3 EaseBounceIn(Vector3 s, Vector3 e, float t, float d)
{
    return EaseBounceIn(s, e, t / d);
}

inline Vector2 EaseBounceOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseBounceOut(s, e, t / d);
}

inline Vector3 EaseBounceOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseBounceOut(s, e, t / d);
}

inline Vector2 EaseBounceInOut(Vector2 s, Vector2 e, float t, float d)
{
    return EaseBounceInOut(s, e, t / d);
}

inline Vector3 EaseBounceInOut(Vector3 s, Vector3 e, float t, float d)
{
    return EaseBounceInOut(s, e, t / d);
}

