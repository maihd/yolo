#pragma once

#include <GL/glew.h>
#include <Yolo/Types.h>
#include <Yolo/Vertex.h>

struct DrawBuffer
{
    bool                ShouldUpdate;
    VertexArray         VertexArray;

    Array<VertexShape>  Vertices;
    Array<U16>          Indices;
};

DrawBuffer DrawBufferNew(void);
void       DrawBufferFree(DrawBuffer* drawBuffer);

void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2);
void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, Array<VertexShape> vertices);
void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, I32 count);

void       DrawBufferAddCircle(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segments = 30);
void       DrawBufferAddCircleLines(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segmentDrawBuffers = 30);

void       DrawBufferAddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 DrawBuffersize, Vector4 color);
void       DrawBufferAddRectangleLines(DrawBuffer* drawBuffer, Vector2 position, Vector2 DrawBuffersize, Vector4 color);

void       DrawBufferClear(DrawBuffer* drawBuffer);
void       DrawBufferUpdateBuffers(DrawBuffer* drawBuffer);
