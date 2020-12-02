#pragma once

#include <Yolo/Core.h>
#include <Yolo/Graphics.h>

struct DrawTextBuffer
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    Array<Vertex>   vertices;
    Array<U16>      indices;

    static DrawTextBuffer   New();
    static void             Free(DrawTextBuffer* drawBuffer);

    static void             AddText(DrawTextBuffer* drawTextBuffer, const char* text, Font font);

    static void             Clear(DrawTextBuffer* drawBuffer);
    static void             UpdateBuffers(DrawTextBuffer* drawBuffer);
};
