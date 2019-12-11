#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

#include <Yolo/Vertex.h>
#include <Yolo/VertexArray.h>

struct DrawBuffer
{
    bool                    shouldUpdate;
    VertexArray             vertexArray;

    Array<VertexShape>      vertices;
    Array<uint16>           indices;
};

namespace DrawBufferOps
{
    DrawBuffer New(void);
    void       Free(DrawBuffer* drawBuffer);

    void       AddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2);
    void       AddTriangle(DrawBuffer* drawBuffer, Array<VertexShape> vertices);
    void       AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, int count);

    void       AddCircle(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);
    void       AddCircleLines(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);

    void       AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color);
    void       AddRectangleLines(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color);

    void       Clear(DrawBuffer* drawBuffer);
    void       UpdateBuffers(DrawBuffer* drawBuffer);
}
