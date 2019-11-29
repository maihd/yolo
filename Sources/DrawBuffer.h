#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

#include "./Vertex.h"

struct DrawBuffer
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    VertexColor* vertices;
    uint16* indices;

    static DrawBuffer New(VertexColor* vertices = 0, uint16* indices = 0);
    static void       Free(DrawBuffer* drawBuffer);

    static void       AddTriangle(DrawBuffer* drawBuffer, VertexColor v0, VertexColor v1, VertexColor v2);
    static void       AddTriangle(DrawBuffer* drawBuffer, VertexColor* vertices);
    static void       AddTriangle(DrawBuffer* drawBuffer, VertexColor* vertices, int count);

    static void       AddCircle(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);
    static void       AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color);

    static void       Clear(DrawBuffer* drawBuffer);
    static void       UpdateBuffers(DrawBuffer* drawBuffer);
};