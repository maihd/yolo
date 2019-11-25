#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

struct Vertex
{
    vec3 position;
    vec2 uv;
    vec4 color;
};

struct DrawBuffer
{
    Vertex* vertices;
    uint16* indices;

    static DrawBuffer New(Vertex* vertices = 0, uint16* indices = 0);
    static void       Free(DrawBuffer* drawBuffer);

    static void       AddTriangle(DrawBuffer* drawBuffer, Vertex v0, Vertex v1, Vertex v2);
    static void       AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices);
    static void       AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices, int count);

    static GLuint     CreateVertexBuffer(DrawBuffer drawBuffer);
    static GLuint     CreateIndexBuffer(DrawBuffer drawBuffer);
};