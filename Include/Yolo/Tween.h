#pragma once

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

    inline vec2 Linear(vec2 s, vec2 e, float t)
    {
        vec2 r;
        r.x = Linear(s.x, e.x, t);
        r.y = Linear(s.y, e.y, t);
        return r;
    }

    inline vec2 Linear(vec2 s, vec2 e, float t, float d)
    {
        return Linear(s, e, t / d);
    }

    inline vec3 Linear(vec3 s, vec3 e, float t)
    {
        vec3 r;
        r.x = Linear(s.x, e.x, t);
        r.y = Linear(s.y, e.y, t);
        r.z = Linear(s.z, e.z, t);
        return r;
    }

    inline vec3 Linear(vec3 s, vec3 e, float t, float d)
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

    inline vec2 QuadIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuadIn(s.x, e.x, t),
            QuadIn(s.y, e.y, t),
        };
    }

    inline vec3 QuadIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuadIn(s.x, e.x, t),
            QuadIn(s.y, e.y, t),
            QuadIn(s.z, e.z, t),
        };
    }

    inline vec2 QuadOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuadOut(s.x, e.x, t),
            QuadOut(s.y, e.y, t),
        };
    }

    inline vec3 QuadOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuadOut(s.x, e.x, t),
            QuadOut(s.y, e.y, t),
            QuadOut(s.z, e.z, t),
        };
    }

    inline vec2 QuadInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuadInOut(s.x, e.x, t),
            QuadInOut(s.y, e.y, t),
        };
    }

    inline vec3 QuadInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuadInOut(s.x, e.x, t),
            QuadInOut(s.y, e.y, t),
            QuadInOut(s.z, e.z, t),
        };
    }

    inline vec2 QuadIn(vec2 s, vec2 e, float t, float d)
    {
        return QuadIn(s, e, t / d);
    }

    inline vec3 QuadIn(vec3 s, vec3 e, float t, float d)
    {
        return QuadIn(s, e, t / d);
    }

    inline vec2 QuadOut(vec2 s, vec2 e, float t, float d)
    {
        return QuadOut(s, e, t / d);
    }

    inline vec3 QuadOut(vec3 s, vec3 e, float t, float d)
    {
        return QuadOut(s, e, t / d);
    }

    inline vec2 QuadInOut(vec2 s, vec2 e, float t, float d)
    {
        return QuadInOut(s, e, t / d);
    }

    inline vec3 QuadInOut(vec3 s, vec3 e, float t, float d)
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
    
    inline vec2 CubicIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            CubicIn(s.x, e.x, t),
            CubicIn(s.y, e.y, t),
        };
    }

    inline vec3 CubicIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            CubicIn(s.x, e.x, t),
            CubicIn(s.y, e.y, t),
            CubicIn(s.z, e.z, t),
        };
    }

    inline vec2 CubicOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            CubicOut(s.x, e.x, t),
            CubicOut(s.y, e.y, t),
        };
    }

    inline vec3 CubicOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            CubicOut(s.x, e.x, t),
            CubicOut(s.y, e.y, t),
            CubicOut(s.z, e.z, t),
        };
    }

    inline vec2 CubicInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            CubicInOut(s.x, e.x, t),
            CubicInOut(s.y, e.y, t),
        };
    }

    inline vec3 CubicInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            CubicInOut(s.x, e.x, t),
            CubicInOut(s.y, e.y, t),
            CubicInOut(s.z, e.z, t),
        };
    }

    inline vec2 CubicIn(vec2 s, vec2 e, float t, float d)
    {
        return CubicIn(s, e, t / d);
    }

    inline vec3 CubicIn(vec3 s, vec3 e, float t, float d)
    {
        return CubicIn(s, e, t / d);
    }

    inline vec2 CubicOut(vec2 s, vec2 e, float t, float d)
    {
        return CubicOut(s, e, t / d);
    }

    inline vec3 CubicOut(vec3 s, vec3 e, float t, float d)
    {
        return CubicOut(s, e, t / d);
    }

    inline vec2 CubicInOut(vec2 s, vec2 e, float t, float d)
    {
        return CubicInOut(s, e, t / d);
    }

    inline vec3 CubicInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 QuartIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuartIn(s.x, e.x, t),
            QuartIn(s.y, e.y, t),
        };
    }

    inline vec3 QuartIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuartIn(s.x, e.x, t),
            QuartIn(s.y, e.y, t),
            QuartIn(s.z, e.z, t),
        };
    }

    inline vec2 QuartOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuartOut(s.x, e.x, t),
            QuartOut(s.y, e.y, t),
        };
    }

    inline vec3 QuartOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuartOut(s.x, e.x, t),
            QuartOut(s.y, e.y, t),
            QuartOut(s.z, e.z, t),
        };
    }

    inline vec2 QuartInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuartInOut(s.x, e.x, t),
            QuartInOut(s.y, e.y, t),
        };
    }

    inline vec3 QuartInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuartInOut(s.x, e.x, t),
            QuartInOut(s.y, e.y, t),
            QuartInOut(s.z, e.z, t),
        };
    }

    inline vec2 QuartIn(vec2 s, vec2 e, float t, float d)
    {
        return QuartIn(s, e, t / d);
    }

    inline vec3 QuartIn(vec3 s, vec3 e, float t, float d)
    {
        return QuartIn(s, e, t / d);
    }

    inline vec2 QuartOut(vec2 s, vec2 e, float t, float d)
    {
        return QuartOut(s, e, t / d);
    }

    inline vec3 QuartOut(vec3 s, vec3 e, float t, float d)
    {
        return QuartOut(s, e, t / d);
    }

    inline vec2 QuartInOut(vec2 s, vec2 e, float t, float d)
    {
        return QuartInOut(s, e, t / d);
    }

    inline vec3 QuartInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 QuintIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuintIn(s.x, e.x, t),
            QuintIn(s.y, e.y, t),
        };
    }

    inline vec3 QuintIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuintIn(s.x, e.x, t),
            QuintIn(s.y, e.y, t),
            QuintIn(s.z, e.z, t),
        };
    }

    inline vec2 QuintOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuintOut(s.x, e.x, t),
            QuintOut(s.y, e.y, t),
        };
    }

    inline vec3 QuintOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuintOut(s.x, e.x, t),
            QuintOut(s.y, e.y, t),
            QuintOut(s.z, e.z, t),
        };
    }

    inline vec2 QuintInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            QuintInOut(s.x, e.x, t),
            QuintInOut(s.y, e.y, t),
        };
    }

    inline vec3 QuintInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            QuintInOut(s.x, e.x, t),
            QuintInOut(s.y, e.y, t),
            QuintInOut(s.z, e.z, t),
        };
    }

    inline vec2 QuintIn(vec2 s, vec2 e, float t, float d)
    {
        return QuintIn(s, e, t / d);
    }

    inline vec3 QuintIn(vec3 s, vec3 e, float t, float d)
    {
        return QuintIn(s, e, t / d);
    }

    inline vec2 QuintOut(vec2 s, vec2 e, float t, float d)
    {
        return QuintOut(s, e, t / d);
    }

    inline vec3 QuintOut(vec3 s, vec3 e, float t, float d)
    {
        return QuintOut(s, e, t / d);
    }

    inline vec2 QuintInOut(vec2 s, vec2 e, float t, float d)
    {
        return QuintInOut(s, e, t / d);
    }

    inline vec3 QuintInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 SineIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            SineIn(s.x, e.x, t),
            SineIn(s.y, e.y, t),
        };
    }

    inline vec3 SineIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            SineIn(s.x, e.x, t),
            SineIn(s.y, e.y, t),
            SineIn(s.z, e.z, t),
        };
    }

    inline vec2 SineOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            SineOut(s.x, e.x, t),
            SineOut(s.y, e.y, t),
        };
    }

    inline vec3 SineOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            SineOut(s.x, e.x, t),
            SineOut(s.y, e.y, t),
            SineOut(s.z, e.z, t),
        };
    }

    inline vec2 SineInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            SineInOut(s.x, e.x, t),
            SineInOut(s.y, e.y, t),
        };
    }

    inline vec3 SineInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            SineInOut(s.x, e.x, t),
            SineInOut(s.y, e.y, t),
            SineInOut(s.z, e.z, t),
        };
    }

    inline vec2 SineIn(vec2 s, vec2 e, float t, float d)
    {
        return SineIn(s, e, t / d);
    }

    inline vec3 SineIn(vec3 s, vec3 e, float t, float d)
    {
        return SineIn(s, e, t / d);
    }

    inline vec2 SineOut(vec2 s, vec2 e, float t, float d)
    {
        return SineOut(s, e, t / d);
    }

    inline vec3 SineOut(vec3 s, vec3 e, float t, float d)
    {
        return SineOut(s, e, t / d);
    }

    inline vec2 SineInOut(vec2 s, vec2 e, float t, float d)
    {
        return SineInOut(s, e, t / d);
    }

    inline vec3 SineInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 ExpoIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            ExpoIn(s.x, e.x, t),
            ExpoIn(s.y, e.y, t),
        };
    }

    inline vec3 ExpoIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            ExpoIn(s.x, e.x, t),
            ExpoIn(s.y, e.y, t),
            ExpoIn(s.z, e.z, t),
        };
    }

    inline vec2 ExpoOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            ExpoOut(s.x, e.x, t),
            ExpoOut(s.y, e.y, t),
        };
    }

    inline vec3 ExpoOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            ExpoOut(s.x, e.x, t),
            ExpoOut(s.y, e.y, t),
            ExpoOut(s.z, e.z, t),
        };
    }

    inline vec2 ExpoInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            ExpoInOut(s.x, e.x, t),
            ExpoInOut(s.y, e.y, t),
        };
    }

    inline vec3 ExpoInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
             ExpoInOut(s.x, e.x, t),
             ExpoInOut(s.y, e.y, t),
             ExpoInOut(s.z, e.z, t),
        };
    }

    inline vec2 ExpoIn(vec2 s, vec2 e, float t, float d)
    {
        return ExpoIn(s, e, t / d);
    }

    inline vec3 ExpoIn(vec3 s, vec3 e, float t, float d)
    {
        return ExpoIn(s, e, t / d);
    }

    inline vec2 ExpoOut(vec2 s, vec2 e, float t, float d)
    {
        return ExpoOut(s, e, t / d);
    }

    inline vec3 ExpoOut(vec3 s, vec3 e, float t, float d)
    {
        return ExpoOut(s, e, t / d);
    }

    inline vec2 ExpoInOut(vec2 s, vec2 e, float t, float d)
    {
        return ExpoInOut(s, e, t / d);
    }

    inline vec3 ExpoInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 CircleIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            CircleIn(s.x, e.x, t),
            CircleIn(s.y, e.y, t),
        };
    }

    inline vec3 CircleIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            CircleIn(s.x, e.x, t),
            CircleIn(s.y, e.y, t),
            CircleIn(s.z, e.z, t),
        };
    }

    inline vec2 CircleOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            CircleOut(s.x, e.x, t),
            CircleOut(s.y, e.y, t),
        };
    }

    inline vec3 CircleOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            CircleOut(s.x, e.x, t),
            CircleOut(s.y, e.y, t),
            CircleOut(s.z, e.z, t),
        };
    }

    inline vec2 CircleInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            CircleInOut(s.x, e.x, t),
            CircleInOut(s.y, e.y, t),
        };
    }

    inline vec3 CircleInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            CircleInOut(s.x, e.x, t),
            CircleInOut(s.y, e.y, t),
            CircleInOut(s.z, e.z, t),
        };
    }

    inline vec2 CircleIn(vec2 s, vec2 e, float t, float d)
    {
        return CircleIn(s, e, t / d);
    }

    inline vec3 CircleIn(vec3 s, vec3 e, float t, float d)
    {
        return CircleIn(s, e, t / d);
    }

    inline vec2 CircleOut(vec2 s, vec2 e, float t, float d)
    {
        return CircleOut(s, e, t / d);
    }

    inline vec3 CircleOut(vec3 s, vec3 e, float t, float d)
    {
        return CircleOut(s, e, t / d);
    }

    inline vec2 CircleInOut(vec2 s, vec2 e, float t, float d)
    {
        return CircleInOut(s, e, t / d);
    }

    inline vec3 CircleInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 ElasticIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            ElasticIn(s.x, e.x, t),
            ElasticIn(s.y, e.y, t),
        };
    }

    inline vec3 ElasticIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            ElasticIn(s.x, e.x, t),
            ElasticIn(s.y, e.y, t),
            ElasticIn(s.z, e.z, t),
        };
    }

    inline vec2 ElasticOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            ElasticOut(s.x, e.x, t),
            ElasticOut(s.y, e.y, t),
        };
    }

    inline vec3 ElasticOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            ElasticOut(s.x, e.x, t),
            ElasticOut(s.y, e.y, t),
            ElasticOut(s.z, e.z, t),
        };
    }

    inline vec2 ElasticInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            ElasticInOut(s.x, e.x, t),
            ElasticInOut(s.y, e.y, t),
        };
    }

    inline vec3 ElasticInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            ElasticInOut(s.x, e.x, t),
            ElasticInOut(s.y, e.y, t),
            ElasticInOut(s.z, e.z, t),
        };
    }

    inline vec2 ElasticIn(vec2 s, vec2 e, float t, float d)
    {
        return ElasticIn(s, e, t / d);
    }

    inline vec3 ElasticIn(vec3 s, vec3 e, float t, float d)
    {
        return ElasticIn(s, e, t / d);
    }

    inline vec2 ElasticOut(vec2 s, vec2 e, float t, float d)
    {
        return ElasticOut(s, e, t / d);
    }

    inline vec3 ElasticOut(vec3 s, vec3 e, float t, float d)
    {
        return ElasticOut(s, e, t / d);
    }

    inline vec2 ElasticInOut(vec2 s, vec2 e, float t, float d)
    {
        return ElasticInOut(s, e, t / d);
    }

    inline vec3 ElasticInOut(vec3 s, vec3 e, float t, float d)
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

    inline vec2 BounceIn(vec2 s, vec2 e, float t)
    {
        return vec2{
            BounceIn(s.x, e.x, t),
            BounceIn(s.y, e.y, t),
        };
    }

    inline vec3 BounceIn(vec3 s, vec3 e, float t)
    {
        return vec3{
            BounceIn(s.x, e.x, t),
            BounceIn(s.y, e.y, t),
            BounceIn(s.z, e.z, t),
        };
    }

    inline vec2 BounceOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            BounceOut(s.x, e.x, t),
            BounceOut(s.y, e.y, t),
        };
    }

    inline vec3 BounceOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            BounceOut(s.x, e.x, t),
            BounceOut(s.y, e.y, t),
            BounceOut(s.z, e.z, t),
        };
    }

    inline vec2 BounceInOut(vec2 s, vec2 e, float t)
    {
        return vec2{
            BounceInOut(s.x, e.x, t),
            BounceInOut(s.y, e.y, t),
        };
    }

    inline vec3 BounceInOut(vec3 s, vec3 e, float t)
    {
        return vec3{
            BounceInOut(s.x, e.x, t),
            BounceInOut(s.y, e.y, t),
            BounceInOut(s.z, e.z, t),
        };
    }

    inline vec2 BounceIn(vec2 s, vec2 e, float t, float d)
    {
        return BounceIn(s, e, t / d);
    }

    inline vec3 BounceIn(vec3 s, vec3 e, float t, float d)
    {
        return BounceIn(s, e, t / d);
    }

    inline vec2 BounceOut(vec2 s, vec2 e, float t, float d)
    {
        return BounceOut(s, e, t / d);
    }

    inline vec3 BounceOut(vec3 s, vec3 e, float t, float d)
    {
        return BounceOut(s, e, t / d);
    }

    inline vec2 BounceInOut(vec2 s, vec2 e, float t, float d)
    {
        return BounceInOut(s, e, t / d);
    }

    inline vec3 BounceInOut(vec3 s, vec3 e, float t, float d)
    {
        return BounceInOut(s, e, t / d);
    }
}

