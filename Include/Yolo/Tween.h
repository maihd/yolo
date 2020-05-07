#pragma once

#include <math.h>
#include <Yolo/Types.h>

namespace Tween
{
    // Linear

    inline float Linear(float s, float e, float t)
    {
        return s + (e - s) * t;
    }

    inline float Linear(float s, float e, float t, float d)
    {
        return Linear(s, e, t / d);
    }

    inline Vector2 Linear(Vector2 s, Vector2 e, float t)
    {
        Vector2 r;
        r.x = Linear(s.x, e.x, t);
        r.y = Linear(s.y, e.y, t);
        return r;
    }

    inline Vector2 Linear(Vector2 s, Vector2 e, float t, float d)
    {
        return Linear(s, e, t / d);
    }

    inline Vector3 Linear(Vector3 s, Vector3 e, float t)
    {
        Vector3 r;
        r.x = Linear(s.x, e.x, t);
        r.y = Linear(s.y, e.y, t);
        r.z = Linear(s.z, e.z, t);
        return r;
    }

    inline Vector3 Linear(Vector3 s, Vector3 e, float t, float d)
    {
        return Linear(s, e, t / d);
    }

    // Quad

    inline float QuadIn(float s, float e, float t)
    {
        return s + (e - s) * t * t;
    }

    inline float QuadOut(float s, float e, float t)
    {
        return s + (s - e) * t * (t - 2.0f);
    }

    inline float QuadInOut(float s, float e, float t)
    {
        return (t *= 2.0f) < 1.0f
            ? s + (e - s) * 0.5f * t * t
            : s + (s - e) * 0.5f * ((t -= 1.0f) * (t - 2.0f) - 1.0f);
    }

    inline float QuadIn(float s, float e, float t, float d)
    {
        return QuadIn(s, e, t / d);
    }

    inline float QuadOut(float s, float e, float t, float d)
    {
        return QuadOut(s, e, t / d);
    }

    inline float QuadInOut(float s, float e, float t, float d)
    {
        return QuadInOut(s, e, t / d);
    }

    inline Vector2 QuadIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuadIn(s.x, e.x, t),
            QuadIn(s.y, e.y, t),
        };
    }

    inline Vector3 QuadIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuadIn(s.x, e.x, t),
            QuadIn(s.y, e.y, t),
            QuadIn(s.z, e.z, t),
        };
    }

    inline Vector2 QuadOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuadOut(s.x, e.x, t),
            QuadOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuadOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuadOut(s.x, e.x, t),
            QuadOut(s.y, e.y, t),
            QuadOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuadInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuadInOut(s.x, e.x, t),
            QuadInOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuadInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuadInOut(s.x, e.x, t),
            QuadInOut(s.y, e.y, t),
            QuadInOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuadIn(Vector2 s, Vector2 e, float t, float d)
    {
        return QuadIn(s, e, t / d);
    }

    inline Vector3 QuadIn(Vector3 s, Vector3 e, float t, float d)
    {
        return QuadIn(s, e, t / d);
    }

    inline Vector2 QuadOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuadOut(s, e, t / d);
    }

    inline Vector3 QuadOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuadOut(s, e, t / d);
    }

    inline Vector2 QuadInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuadInOut(s, e, t / d);
    }

    inline Vector3 QuadInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuadInOut(s, e, t / d);
    }

    // Cubic

    inline float CubicIn(float s, float e, float t)
    {
        return s + (e - s) * t * t * t;
    }

    inline float CubicOut(float s, float e, float t)
    {
        return s + (e - s) * ((t -= 1.0f) * t * t + 1.0f);
    }

    inline float CubicInOut(float s, float e, float t)
    {
        return (t *= 2.0f) < 1.0f
            ? s + (e - s) * 0.5f * t * t * t
            : s + (e - s) * 0.5f * ((t -= 2.0f) * t * t + 2.0f);
    }

    inline float CubicIn(float s, float e, float t, float d)
    {
        return CubicIn(s, e, t / d);
    }

    inline float CubicOut(float s, float e, float t, float d)
    {
        return CubicOut(s, e, t / d);
    }

    inline float CubicInOut(float s, float e, float t, float d)
    {
        return CubicInOut(s, e, t / d);
    }
    
    inline Vector2 CubicIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CubicIn(s.x, e.x, t),
            CubicIn(s.y, e.y, t),
        };
    }

    inline Vector3 CubicIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CubicIn(s.x, e.x, t),
            CubicIn(s.y, e.y, t),
            CubicIn(s.z, e.z, t),
        };
    }

    inline Vector2 CubicOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CubicOut(s.x, e.x, t),
            CubicOut(s.y, e.y, t),
        };
    }

    inline Vector3 CubicOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CubicOut(s.x, e.x, t),
            CubicOut(s.y, e.y, t),
            CubicOut(s.z, e.z, t),
        };
    }

    inline Vector2 CubicInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CubicInOut(s.x, e.x, t),
            CubicInOut(s.y, e.y, t),
        };
    }

    inline Vector3 CubicInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CubicInOut(s.x, e.x, t),
            CubicInOut(s.y, e.y, t),
            CubicInOut(s.z, e.z, t),
        };
    }

    inline Vector2 CubicIn(Vector2 s, Vector2 e, float t, float d)
    {
        return CubicIn(s, e, t / d);
    }

    inline Vector3 CubicIn(Vector3 s, Vector3 e, float t, float d)
    {
        return CubicIn(s, e, t / d);
    }

    inline Vector2 CubicOut(Vector2 s, Vector2 e, float t, float d)
    {
        return CubicOut(s, e, t / d);
    }

    inline Vector3 CubicOut(Vector3 s, Vector3 e, float t, float d)
    {
        return CubicOut(s, e, t / d);
    }

    inline Vector2 CubicInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return CubicInOut(s, e, t / d);
    }

    inline Vector3 CubicInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return CubicInOut(s, e, t / d);
    }

    // Quart

    inline float QuartIn(float s, float e, float t)
    {
        return s + (e - s) * t * t * t * t;
    }

    inline float QuartOut(float s, float e, float t)
    {
        return s + (s - e) * ((t -= 1.0f) * t * t * t - 1.0f);
    }

    inline float QuartInOut(float s, float e, float t)
    {
        return (t *= 2.0f) < 1.0f
            ? s + (e - s) * 0.5f * t * t * t * t
            : s + (s - e) * 0.5f * ((t -= 2.0f) * t * t * t - 2.0f);
    }

    inline float QuartIn(float s, float e, float t, float d)
    {
        return QuartIn(s, e, t / d);
    }

    inline float QuartOut(float s, float e, float t, float d)
    {
        return QuartOut(s, e, t / d);
    }

    inline float QuartInOut(float s, float e, float t, float d)
    {
        return QuartInOut(s, e, t / d);
    }

    inline Vector2 QuartIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuartIn(s.x, e.x, t),
            QuartIn(s.y, e.y, t),
        };
    }

    inline Vector3 QuartIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuartIn(s.x, e.x, t),
            QuartIn(s.y, e.y, t),
            QuartIn(s.z, e.z, t),
        };
    }

    inline Vector2 QuartOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuartOut(s.x, e.x, t),
            QuartOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuartOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuartOut(s.x, e.x, t),
            QuartOut(s.y, e.y, t),
            QuartOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuartInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuartInOut(s.x, e.x, t),
            QuartInOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuartInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuartInOut(s.x, e.x, t),
            QuartInOut(s.y, e.y, t),
            QuartInOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuartIn(Vector2 s, Vector2 e, float t, float d)
    {
        return QuartIn(s, e, t / d);
    }

    inline Vector3 QuartIn(Vector3 s, Vector3 e, float t, float d)
    {
        return QuartIn(s, e, t / d);
    }

    inline Vector2 QuartOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuartOut(s, e, t / d);
    }

    inline Vector3 QuartOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuartOut(s, e, t / d);
    }

    inline Vector2 QuartInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuartInOut(s, e, t / d);
    }

    inline Vector3 QuartInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuartInOut(s, e, t / d);
    }

    // @Quint

    inline float QuintIn(float s, float e, float t)
    {
        return s + (e - s) * t * t * t * t * t;
    }

    inline float QuintOut(float s, float e, float t)
    {
        return s + (e - s) * ((t -= 1.0f) * t * t * t * t + 1.0f);
    }

    inline float QuintInOut(float s, float e, float t)
    {
        return (t *= 2.0f) < 1.0f
            ? s + (e - s) * 0.5f * t * t * t * t * t
            : s + (e - s) * 0.5f * ((t -= 2.0f) * t * t * t * t + 2.0f);
    }

    inline float QuintIn(float s, float e, float t, float d)
    {
        return QuintIn(s, e, t / d);
    }

    inline float QuintOut(float s, float e, float t, float d)
    {
        return QuintOut(s, e, t / d);
    }

    inline float QuintInOut(float s, float e, float t, float d)
    {
        return QuintInOut(s, e, t / d);
    }

    inline Vector2 QuintIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuintIn(s.x, e.x, t),
            QuintIn(s.y, e.y, t),
        };
    }

    inline Vector3 QuintIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuintIn(s.x, e.x, t),
            QuintIn(s.y, e.y, t),
            QuintIn(s.z, e.z, t),
        };
    }

    inline Vector2 QuintOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuintOut(s.x, e.x, t),
            QuintOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuintOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuintOut(s.x, e.x, t),
            QuintOut(s.y, e.y, t),
            QuintOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuintInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            QuintInOut(s.x, e.x, t),
            QuintInOut(s.y, e.y, t),
        };
    }

    inline Vector3 QuintInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            QuintInOut(s.x, e.x, t),
            QuintInOut(s.y, e.y, t),
            QuintInOut(s.z, e.z, t),
        };
    }

    inline Vector2 QuintIn(Vector2 s, Vector2 e, float t, float d)
    {
        return QuintIn(s, e, t / d);
    }

    inline Vector3 QuintIn(Vector3 s, Vector3 e, float t, float d)
    {
        return QuintIn(s, e, t / d);
    }

    inline Vector2 QuintOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuintOut(s, e, t / d);
    }

    inline Vector3 QuintOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuintOut(s, e, t / d);
    }

    inline Vector2 QuintInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return QuintInOut(s, e, t / d);
    }

    inline Vector3 QuintInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return QuintInOut(s, e, t / d);
    }

    // @Sine

    inline float SineIn(float s, float e, float t)
    {
        return e + (s - e) * cosf(t * PI * 0.5f);
    }

    inline float SineOut(float s, float e, float t)
    {
        return s + (e - s) * sinf(t * PI * 0.5f);
    }

    inline float SineInOut(float s, float e, float t)
    {
        return s + (s - e) * 0.5f * (cosf(t * PI) - 1.0f);
    }

    inline float SineIn(float s, float e, float t, float d)
    {
        return SineIn(s, e, t / d);
    }

    inline float SineOut(float s, float e, float t, float d)
    {
        return SineOut(s, e, t / d);
    }

    inline float SineInOut(float s, float e, float t, float d)
    {
        return SineInOut(s, e, t / d);
    }

    inline Vector2 SineIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            SineIn(s.x, e.x, t),
            SineIn(s.y, e.y, t),
        };
    }

    inline Vector3 SineIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            SineIn(s.x, e.x, t),
            SineIn(s.y, e.y, t),
            SineIn(s.z, e.z, t),
        };
    }

    inline Vector2 SineOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            SineOut(s.x, e.x, t),
            SineOut(s.y, e.y, t),
        };
    }

    inline Vector3 SineOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            SineOut(s.x, e.x, t),
            SineOut(s.y, e.y, t),
            SineOut(s.z, e.z, t),
        };
    }

    inline Vector2 SineInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            SineInOut(s.x, e.x, t),
            SineInOut(s.y, e.y, t),
        };
    }

    inline Vector3 SineInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            SineInOut(s.x, e.x, t),
            SineInOut(s.y, e.y, t),
            SineInOut(s.z, e.z, t),
        };
    }

    inline Vector2 SineIn(Vector2 s, Vector2 e, float t, float d)
    {
        return SineIn(s, e, t / d);
    }

    inline Vector3 SineIn(Vector3 s, Vector3 e, float t, float d)
    {
        return SineIn(s, e, t / d);
    }

    inline Vector2 SineOut(Vector2 s, Vector2 e, float t, float d)
    {
        return SineOut(s, e, t / d);
    }

    inline Vector3 SineOut(Vector3 s, Vector3 e, float t, float d)
    {
        return SineOut(s, e, t / d);
    }

    inline Vector2 SineInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return SineInOut(s, e, t / d);
    }

    inline Vector3 SineInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return SineInOut(s, e, t / d);
    }

    // @Expo

    inline float ExpoIn(float s, float e, float t)
    {
        return t <= 0.0f ? s : s + (e - s) * powf(2.0f, 10.0f * (t - 1.0f));
    }

    inline float ExpoOut(float s, float e, float t)
    {
        return t >= 1.0f ? e : s + (e - s) * (1.0f - powf(2.0f, -10.0f * t));
    }

    inline float ExpoInOut(float s, float e, float t)
    {
        if (t <= 0.0f) return s;
        if (t >= 1.0f) return e;

        return t < 0.5f
            ? ExpoIn(s, s + (e - s) * 0.5f, t * 2.0f)
            : ExpoOut(s + (e - s) * 0.5f, e, (t - 0.5f) * 2.0f);
    }

    inline float ExpoIn(float s, float e, float t, float d)
    {
        return ExpoIn(s, e, t / d);
    }

    inline float ExpoOut(float s, float e, float t, float d)
    {
        return ExpoOut(s, e, t / d);
    }

    inline float ExpoInOut(float s, float e, float t, float d)
    {
        return ExpoInOut(s, e, t / d);
    }

    inline Vector2 ExpoIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ExpoIn(s.x, e.x, t),
            ExpoIn(s.y, e.y, t),
        };
    }

    inline Vector3 ExpoIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            ExpoIn(s.x, e.x, t),
            ExpoIn(s.y, e.y, t),
            ExpoIn(s.z, e.z, t),
        };
    }

    inline Vector2 ExpoOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ExpoOut(s.x, e.x, t),
            ExpoOut(s.y, e.y, t),
        };
    }

    inline Vector3 ExpoOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            ExpoOut(s.x, e.x, t),
            ExpoOut(s.y, e.y, t),
            ExpoOut(s.z, e.z, t),
        };
    }

    inline Vector2 ExpoInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ExpoInOut(s.x, e.x, t),
            ExpoInOut(s.y, e.y, t),
        };
    }

    inline Vector3 ExpoInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
             ExpoInOut(s.x, e.x, t),
             ExpoInOut(s.y, e.y, t),
             ExpoInOut(s.z, e.z, t),
        };
    }

    inline Vector2 ExpoIn(Vector2 s, Vector2 e, float t, float d)
    {
        return ExpoIn(s, e, t / d);
    }

    inline Vector3 ExpoIn(Vector3 s, Vector3 e, float t, float d)
    {
        return ExpoIn(s, e, t / d);
    }

    inline Vector2 ExpoOut(Vector2 s, Vector2 e, float t, float d)
    {
        return ExpoOut(s, e, t / d);
    }

    inline Vector3 ExpoOut(Vector3 s, Vector3 e, float t, float d)
    {
        return ExpoOut(s, e, t / d);
    }

    inline Vector2 ExpoInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return ExpoInOut(s, e, t / d);
    }

    inline Vector3 ExpoInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return ExpoInOut(s, e, t / d);
    }

    // @Circle

    inline float CircleIn(float s, float e, float t)
    {
        return s + (s - e) * (sqrtf(1.0f - t * t) - 1.0f);
    }

    inline float CircleOut(float s, float e, float t)
    {
        return s + (e - s) * (sqrtf(1.0f - (t - 1.0f) * (t - 1.0f)));
    }

    inline float CircleInOut(float s, float e, float t)
    {
        return (t *= 2.0f) < 1.0f
            ? s + (s - e) * 0.5f * (sqrtf(1.0f - t * t) - 1.0f)
            : s + (e - s) * 0.5f * (sqrtf(1.0f - (t - 2.0f) * (t - 2.0f)) + 1.0f);
    }

    inline float CircleIn(float s, float e, float t, float d)
    {
        return CircleIn(s, e, t / d);
    }

    inline float CircleOut(float s, float e, float t, float d)
    {
        return CircleOut(s, e, t / d);
    }

    inline float CircleInOut(float s, float e, float t, float d)
    {
        return CircleInOut(s, e, t / d);
    }

    inline Vector2 CircleIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CircleIn(s.x, e.x, t),
            CircleIn(s.y, e.y, t),
        };
    }

    inline Vector3 CircleIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CircleIn(s.x, e.x, t),
            CircleIn(s.y, e.y, t),
            CircleIn(s.z, e.z, t),
        };
    }

    inline Vector2 CircleOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CircleOut(s.x, e.x, t),
            CircleOut(s.y, e.y, t),
        };
    }

    inline Vector3 CircleOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CircleOut(s.x, e.x, t),
            CircleOut(s.y, e.y, t),
            CircleOut(s.z, e.z, t),
        };
    }

    inline Vector2 CircleInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            CircleInOut(s.x, e.x, t),
            CircleInOut(s.y, e.y, t),
        };
    }

    inline Vector3 CircleInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            CircleInOut(s.x, e.x, t),
            CircleInOut(s.y, e.y, t),
            CircleInOut(s.z, e.z, t),
        };
    }

    inline Vector2 CircleIn(Vector2 s, Vector2 e, float t, float d)
    {
        return CircleIn(s, e, t / d);
    }

    inline Vector3 CircleIn(Vector3 s, Vector3 e, float t, float d)
    {
        return CircleIn(s, e, t / d);
    }

    inline Vector2 CircleOut(Vector2 s, Vector2 e, float t, float d)
    {
        return CircleOut(s, e, t / d);
    }

    inline Vector3 CircleOut(Vector3 s, Vector3 e, float t, float d)
    {
        return CircleOut(s, e, t / d);
    }

    inline Vector2 CircleInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return CircleInOut(s, e, t / d);
    }

    inline Vector3 CircleInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return CircleInOut(s, e, t / d);
    }

    // @Elastic

    inline float ElasticIn(float s, float e, float t)
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

    inline float ElasticOut(float s, float e, float t)
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

    inline float ElasticInOut(float s, float e, float t)
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

    inline float ElasticIn(float s, float e, float t, float d)
    {
        return ElasticIn(s, e, t / d);
    }

    inline float ElasticOut(float s, float e, float t, float d)
    {
        return ElasticOut(s, e, t / d);
    }

    inline float ElasticInOut(float s, float e, float t, float d)
    {
        return ElasticInOut(s, e, t / d);
    }

    inline Vector2 ElasticIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ElasticIn(s.x, e.x, t),
            ElasticIn(s.y, e.y, t),
        };
    }

    inline Vector3 ElasticIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            ElasticIn(s.x, e.x, t),
            ElasticIn(s.y, e.y, t),
            ElasticIn(s.z, e.z, t),
        };
    }

    inline Vector2 ElasticOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ElasticOut(s.x, e.x, t),
            ElasticOut(s.y, e.y, t),
        };
    }

    inline Vector3 ElasticOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            ElasticOut(s.x, e.x, t),
            ElasticOut(s.y, e.y, t),
            ElasticOut(s.z, e.z, t),
        };
    }

    inline Vector2 ElasticInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            ElasticInOut(s.x, e.x, t),
            ElasticInOut(s.y, e.y, t),
        };
    }

    inline Vector3 ElasticInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            ElasticInOut(s.x, e.x, t),
            ElasticInOut(s.y, e.y, t),
            ElasticInOut(s.z, e.z, t),
        };
    }

    inline Vector2 ElasticIn(Vector2 s, Vector2 e, float t, float d)
    {
        return ElasticIn(s, e, t / d);
    }

    inline Vector3 ElasticIn(Vector3 s, Vector3 e, float t, float d)
    {
        return ElasticIn(s, e, t / d);
    }

    inline Vector2 ElasticOut(Vector2 s, Vector2 e, float t, float d)
    {
        return ElasticOut(s, e, t / d);
    }

    inline Vector3 ElasticOut(Vector3 s, Vector3 e, float t, float d)
    {
        return ElasticOut(s, e, t / d);
    }

    inline Vector2 ElasticInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return ElasticInOut(s, e, t / d);
    }

    inline Vector3 ElasticInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return ElasticInOut(s, e, t / d);
    }

    // @Bounce

    inline float BounceOut(float s, float e, float t);
    inline float BounceIn(float s, float e, float t)
    {
        return e - BounceOut(s, e, 1.0f - t);
    }

    inline float BounceOut(float s, float e, float t)
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

    inline float BounceInOut(float s, float e, float t)
    {
        return t < 0.5f
            ? BounceIn(s, s + (e - s) * 0.5f, t * 2.0f)
            : BounceOut(s + (e - s) * 0.5f, e, (t - 0.5f) * 2.0f);
    }

    inline float BounceIn(float s, float e, float t, float d)
    {
        return BounceIn(s, e, t / d);
    }

    inline float BounceOut(float s, float e, float t, float d)
    {
        return BounceOut(s, e, t / d);
    }

    inline float BounceInOut(float s, float e, float t, float d)
    {
        return BounceInOut(s, e, t / d);
    }

    inline Vector2 BounceIn(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            BounceIn(s.x, e.x, t),
            BounceIn(s.y, e.y, t),
        };
    }

    inline Vector3 BounceIn(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            BounceIn(s.x, e.x, t),
            BounceIn(s.y, e.y, t),
            BounceIn(s.z, e.z, t),
        };
    }

    inline Vector2 BounceOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            BounceOut(s.x, e.x, t),
            BounceOut(s.y, e.y, t),
        };
    }

    inline Vector3 BounceOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            BounceOut(s.x, e.x, t),
            BounceOut(s.y, e.y, t),
            BounceOut(s.z, e.z, t),
        };
    }

    inline Vector2 BounceInOut(Vector2 s, Vector2 e, float t)
    {
        return Vector2{
            BounceInOut(s.x, e.x, t),
            BounceInOut(s.y, e.y, t),
        };
    }

    inline Vector3 BounceInOut(Vector3 s, Vector3 e, float t)
    {
        return Vector3{
            BounceInOut(s.x, e.x, t),
            BounceInOut(s.y, e.y, t),
            BounceInOut(s.z, e.z, t),
        };
    }

    inline Vector2 BounceIn(Vector2 s, Vector2 e, float t, float d)
    {
        return BounceIn(s, e, t / d);
    }

    inline Vector3 BounceIn(Vector3 s, Vector3 e, float t, float d)
    {
        return BounceIn(s, e, t / d);
    }

    inline Vector2 BounceOut(Vector2 s, Vector2 e, float t, float d)
    {
        return BounceOut(s, e, t / d);
    }

    inline Vector3 BounceOut(Vector3 s, Vector3 e, float t, float d)
    {
        return BounceOut(s, e, t / d);
    }

    inline Vector2 BounceInOut(Vector2 s, Vector2 e, float t, float d)
    {
        return BounceInOut(s, e, t / d);
    }

    inline Vector3 BounceInOut(Vector3 s, Vector3 e, float t, float d)
    {
        return BounceInOut(s, e, t / d);
    }
}

