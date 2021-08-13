#pragma once

#include <GL/glew.h>
#include <System/Core.h>
#include <Graphics/Graphics.h>

struct DrawBuffer
{
    bool                    ShouldUpdate;
    VertexArray             VertexArray;

    Array<VertexShapeColor> Vertices;
    Array<U16>              Indices;
};

DrawBuffer DrawBufferNew(void);
void       DrawBufferFree(DrawBuffer* drawBuffer);

void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShapeColor v0, VertexShapeColor v1, VertexShapeColor v2);
void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, Array<VertexShapeColor> vertices);
void       DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShapeColor* vertices, I32 count);

void       DrawBufferAddCircle(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segments = 30);
void       DrawBufferAddCircleLines(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segmentDrawBuffers = 30);

void       DrawBufferAddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 DrawBuffersize, Vector4 color);
void       DrawBufferAddRectangleLines(DrawBuffer* drawBuffer, Vector2 position, Vector2 DrawBuffersize, Vector4 color);

void       DrawBufferClear(DrawBuffer* drawBuffer);
void       DrawBufferUpdateBuffers(DrawBuffer* drawBuffer);
