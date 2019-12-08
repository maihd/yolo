#pragma once

#include <Yolo/Types.h>
#include <Yolo/MathTypes.h>

struct Vertex
{
    vec3 position;
    vec2 uv;
};

struct VertexColor
{
    vec3 position;
    vec2 uv;
    vec4 color;
};

struct VertexShape
{
    vec3 position;
    vec4 color;
};
