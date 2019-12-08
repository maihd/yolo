#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

#include <Yolo/Vertex.h>

struct DrawBuffer
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    VertexShape* vertices;
    uint16* indices;

    static DrawBuffer New(VertexShape* vertices = 0, uint16* indices = 0);
    static void       Free(DrawBuffer* drawBuffer);

    static void       AddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2);
    static void       AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices);
    static void       AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, int count);

    static void       AddCircle(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);
    static void       AddCircleLines(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);

    static void       AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color);
    static void       AddRectangleLines(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color);

    static void       Clear(DrawBuffer* drawBuffer);
    static void       UpdateBuffers(DrawBuffer* drawBuffer);
};