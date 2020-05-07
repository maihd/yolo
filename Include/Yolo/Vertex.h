#pragma once

#include <Yolo/Types.h>

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
    uint32      color;
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
    uint32      color;
};
