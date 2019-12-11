#pragma once

#include <Yolo/Types.h>

struct Vertex
{
    vec3    position;
    vec2    uv;
};

struct VertexColor
{
    vec3    position;
    vec2    uv;
    vec4    color;
};

struct VertexColorU32
{
    vec3    position;
    vec2    uv;
    uint32  color;
};

struct VertexShape
{
    vec3    position;
};

struct VertexShapeColor
{
    vec3    position;
    vec4    color;
};

struct VertexShapeColorU32
{
    vec3    position;
    uint32  color;
};
