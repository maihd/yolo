#include "./DrawTextBuffer.h"

#include <GL/glew.h>

#include <Yolo/Math.h>
#include <Yolo/Array.h>
#include <Yolo/String.h>

DrawTextBuffer DrawTextBuffer::New()
{
    return DrawTextBuffer{
        false,
        0, 
        0, 
        0, 
        ArrayOps::New<Vertex>(), 
        ArrayOps::New<U16>(),
    };
}

void DrawTextBuffer::Free(DrawTextBuffer* drawBuffer)
{
    assert(drawBuffer);

    glDeleteBuffers(1, &drawBuffer->vertexBuffer);
    glDeleteBuffers(1, &drawBuffer->indexBuffer);
    glDeleteVertexArrays(1, &drawBuffer->vertexArray);

    drawBuffer->vertexBuffer = 0;
    drawBuffer->indexBuffer = 0;
    drawBuffer->vertexArray = 0;

    ArrayOps::Free(&drawBuffer->vertices);
    ArrayOps::Free(&drawBuffer->indices);
}

void DrawTextBuffer::Clear(DrawTextBuffer* drawBuffer)
{
    if (drawBuffer->vertexArray)
    {
        drawBuffer->shouldUpdate = true;
    }

    ArrayOps::Clear(&drawBuffer->vertices);
    ArrayOps::Clear(&drawBuffer->indices);
}

void DrawTextBuffer::UpdateBuffers(DrawTextBuffer* drawBuffer)
{
    assert(drawBuffer);

    if (drawBuffer->shouldUpdate)
    {
        drawBuffer->shouldUpdate = false;

        if (!drawBuffer->vertexBuffer)
        {
            glGenBuffers(1, &drawBuffer->vertexBuffer);
        }

        if (!drawBuffer->indexBuffer)
        {
            glGenBuffers(1, &drawBuffer->indexBuffer);
        }

        if (!drawBuffer->vertexArray)
        {
            glGenVertexArrays(1, &drawBuffer->vertexArray);
            glBindVertexArray(drawBuffer->vertexArray);
            glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, ArrayOps::SizeOf(drawBuffer->vertices), drawBuffer->vertices.elements, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ArrayOps::SizeOf(drawBuffer->indices), drawBuffer->indices.elements, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void DrawTextBuffer::AddText(DrawTextBuffer* drawTextBuffer, String text, Font font)
{
    I32 length = StringOps::Length(text);
    if (length > 0)
    {
        F32 advanceX = 0;
        F32 advanceY = 0;

        for (I32 i = 0; i < length; i++)
        {
            drawTextBuffer->shouldUpdate = true;

            I32 c = text[i];
            if (c > 0 && c < font.glyphs.count)
            {
                FontGlyph glyph = font.glyphs.elements[c];

                Vector2 pos0 = Vector2{ advanceX + glyph.x0, advanceY + glyph.y0 };
                Vector2 pos1 = Vector2{ advanceX + glyph.x1, advanceY + glyph.y1 };

                Vector2 uv0 = Vector2{ glyph.u0, glyph.v0 };
                Vector2 uv1 = Vector2{ glyph.u1, glyph.v1 };

                const U16 startIndex = (U16)drawTextBuffer->vertices.count;
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 0));
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 1));
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 2));
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 0));
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 2));
                ArrayOps::Push(&drawTextBuffer->indices, (U16)(startIndex + 3));

                ArrayOps::Push(&drawTextBuffer->vertices, Vertex{ pos0.x, pos0.y, 0.0f, uv0.x, uv0.y });
                ArrayOps::Push(&drawTextBuffer->vertices, Vertex{ pos0.x, pos1.y, 0.0f, uv0.x, uv1.y });
                ArrayOps::Push(&drawTextBuffer->vertices, Vertex{ pos1.x, pos1.y, 0.0f, uv1.x, uv1.y });
                ArrayOps::Push(&drawTextBuffer->vertices, Vertex{ pos1.x, pos0.y, 0.0f, uv1.x, uv0.y });

                advanceX += glyph.advance;
                advanceY += 0;
            }
        }
    }
}
