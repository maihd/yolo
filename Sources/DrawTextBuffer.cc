#include "./DrawTextBuffer.h"

#include <Yolo/Math.h>
#include <Yolo/String.h>

#include <math.h>
#include <GL/glew.h>
#include <stb_truetype.h>

DrawTextBuffer DrawTextBuffer::New(Vertex* vertices, uint16* indices)
{
    return { 
        false,
        Const::EMPTY_HANDLE, 
        Const::EMPTY_HANDLE, 
        Const::EMPTY_HANDLE, 
        vertices, 
        indices 
    };
}

void DrawTextBuffer::Free(DrawTextBuffer* drawBuffer)
{
    assert(drawBuffer);

    glDeleteBuffers(1, &drawBuffer->vertexBuffer);
    glDeleteBuffers(1, &drawBuffer->indexBuffer);
    glDeleteVertexArrays(1, &drawBuffer->vertexArray);

    drawBuffer->vertexBuffer = Const::EMPTY_HANDLE;
    drawBuffer->indexBuffer = Const::EMPTY_HANDLE;
    drawBuffer->vertexArray = Const::EMPTY_HANDLE;

    Array::Free(&drawBuffer->vertices);
    Array::Free(&drawBuffer->indices);
}

void DrawTextBuffer::Clear(DrawTextBuffer* drawBuffer)
{
    if (drawBuffer->vertexArray)
    {
        drawBuffer->shouldUpdate = true;
    }

    Array::Clear(&drawBuffer->vertices);
    Array::Clear(&drawBuffer->indices);
}

void DrawTextBuffer::UpdateBuffers(DrawTextBuffer* drawBuffer)
{
    assert(drawBuffer);

    if (drawBuffer->shouldUpdate)
    {
        drawBuffer->shouldUpdate = false;

        if (drawBuffer->vertexBuffer == Const::EMPTY_HANDLE)
        {
            glGenBuffers(1, &drawBuffer->vertexBuffer);
        }

        if (drawBuffer->indexBuffer == Const::EMPTY_HANDLE)
        {
            glGenBuffers(1, &drawBuffer->indexBuffer);
        }

        if (drawBuffer->vertexArray == Const::EMPTY_HANDLE)
        {
            glGenVertexArrays(1, &drawBuffer->vertexArray);
            glBindVertexArray(drawBuffer->vertexArray);
            glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
        }

        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, Array::Length(drawBuffer->vertices) * sizeof(Vertex), drawBuffer->vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Array::Length(drawBuffer->indices) * sizeof(uint16), drawBuffer->indices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void DrawTextBuffer::AddText(DrawTextBuffer* drawTextBuffer, string text, Font font)
{
    int length = String::Length(text);
    if (length > 0)
    {
        float advanceX = 0;
        float advanceY = 0;

        for (int i = 0; i < length; i++)
        {
            drawTextBuffer->shouldUpdate = true;

            int c = text[i];
            FontGlyph glyph = font.glyphs[c];

            vec2 pos0 = vec2{ advanceX + glyph.x0, advanceY + glyph.y0 };
            vec2 pos1 = vec2{ advanceX + glyph.x1, advanceY + glyph.y1 };

            vec2 uv0 = vec2{ glyph.u0, glyph.v0 };
            vec2 uv1 = vec2{ glyph.u1, glyph.v1 };

            const uint16 startIndex = (uint16)(Array::Length(drawTextBuffer->vertices));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 0));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 1));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 2));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 0));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 2));
            Array::Push(&drawTextBuffer->indices, (uint16)(startIndex + 3));

            Array::Push(&drawTextBuffer->vertices, Vertex{ pos0.x, pos0.y, 0.0f, uv0.x, uv0.y });
            Array::Push(&drawTextBuffer->vertices, Vertex{ pos0.x, pos1.y, 0.0f, uv0.x, uv1.y });
            Array::Push(&drawTextBuffer->vertices, Vertex{ pos1.x, pos1.y, 0.0f, uv1.x, uv1.y });
            Array::Push(&drawTextBuffer->vertices, Vertex{ pos1.x, pos0.y, 0.0f, uv1.x, uv0.y });

            advanceX += glyph.advance;
            advanceY += 0;
        }
    }
}
