#pragma once

#include <GL/glew.h>
#include <Yolo/Types.h>
#include <Yolo/Vertex.h>

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

    void       AddCircle(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, int segments = 30);
    void       AddCircleLines(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, int segments = 30);

    void       AddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color);
    void       AddRectangleLines(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color);

    void       Clear(DrawBuffer* drawBuffer);
    void       UpdateBuffers(DrawBuffer* drawBuffer);
}
