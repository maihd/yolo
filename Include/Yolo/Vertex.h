#pragma once

#include <Yolo/Core.h>

struct Vertex
{
    Vector3     position;
    Vector2     uv;
};

struct VertexColor
{
    Vector3     position;
    Vector2     uv;
    Vector4     color;
};

struct VertexColorU32
{
    Vector3     position;
    Vector2     uv;
    U32         color;
};

struct VertexShape
{
    Vector3     position;
};

struct VertexShapeColor
{
    Vector3     position;
    Vector4     color;
};

struct VertexShapeColorU32
{
    Vector3     position;
    U32         color;
};
