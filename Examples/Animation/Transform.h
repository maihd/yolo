#pragma once

#include <Yolo/Core.h>
#include <Yolo/Math.h>

struct Transform
{
    Vector3     Scale;
    Vector3     Position;
    Quaternion  Rotation;
};

Transform CombineTransform(Transform a, Transform b)
{
    Transform result;

    result.Scale = a.Scale * b.Scale;
    result.Rotation = mul(b.Rotation, a.Rotation);
    result.Position = mul(a.Rotation, (a.Scale * b.Position));
}

Transform InverseTransform(Transform transform)
{
    Transform inv;

    inv.Rotation = inverse(transform.Rotation);

    inv.Scale.x = fabs(transform.Scale.x) < 0.01f ? 0.0f : 1.0f / transform.Scale.x;
    inv.Scale.y = fabs(transform.Scale.y) < 0.01f ? 0.0f : 1.0f / transform.Scale.y;
    inv.Scale.z = fabs(transform.Scale.z) < 0.01f ? 0.0f : 1.0f / transform.Scale.z;

    Vector3 invTranslate = transform.Position * -1.0f;
    inv.Position = mul(inv.Rotation, inv.Scale * invTranslate);

    return inv;
}