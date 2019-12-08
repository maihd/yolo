#pragma once

#include <Yolo/Font.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

#include <Yolo/Vertex.h>

struct DrawTextBuffer
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    Vertex* vertices;
    uint16* indices;

    static DrawTextBuffer   New(Vertex* vertices = 0, uint16* indices = 0);
    static void             Free(DrawTextBuffer* drawBuffer);

    static void             AddText(DrawTextBuffer* drawTextBuffer, String text, Font font);

    static void             Clear(DrawTextBuffer* drawBuffer);
    static void             UpdateBuffers(DrawTextBuffer* drawBuffer);
};