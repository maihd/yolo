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
        MakeArray<Vertex>(), 
        MakeArray<U16>(),
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

    FreeArray(&drawBuffer->vertices);
    FreeArray(&drawBuffer->indices);
}

void DrawTextBuffer::Clear(DrawTextBuffer* drawBuffer)
{
    if (drawBuffer->vertexArray)
    {
        drawBuffer->shouldUpdate = true;
    }

    ArrayClear(&drawBuffer->vertices);
    ArrayClear(&drawBuffer->indices);
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
        glBufferData(GL_ARRAY_BUFFER, ArraySizeInBytes(drawBuffer->vertices), drawBuffer->vertices.Items, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ArraySizeInBytes(drawBuffer->indices), drawBuffer->indices.Items, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void DrawTextBuffer::AddText(DrawTextBuffer* drawTextBuffer, const char* text, Font font)
{
    I32 length = (size_t)strlen(text);
    if (length > 0)
    {
        float advanceX = 0;
        float advanceY = 0;

        for (I32 i = 0; i < length; i++)
        {
            drawTextBuffer->shouldUpdate = true;

            I32 c = text[i];
            if (c > 0 && c < font.Glyphs.Count)
            {
                FontGlyph glyph = font.Glyphs.Items[c];

                Vector2 pos0 = Vector2{ advanceX + glyph.X0, advanceY + glyph.Y0 };
                Vector2 pos1 = Vector2{ advanceX + glyph.X1, advanceY + glyph.Y1 };

                Vector2 uv0 = Vector2{ glyph.U0, glyph.V0 };
                Vector2 uv1 = Vector2{ glyph.U1, glyph.V1 };

                const U16 startIndex = (U16)drawTextBuffer->vertices.Count;
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 0));
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 1));
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 2));
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 0));
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 2));
                ArrayPush(&drawTextBuffer->indices, (U16)(startIndex + 3));

                ArrayPush(&drawTextBuffer->vertices, Vertex{ pos0.x, pos0.y, 0.0f, uv0.x, uv0.y });
                ArrayPush(&drawTextBuffer->vertices, Vertex{ pos0.x, pos1.y, 0.0f, uv0.x, uv1.y });
                ArrayPush(&drawTextBuffer->vertices, Vertex{ pos1.x, pos1.y, 0.0f, uv1.x, uv1.y });
                ArrayPush(&drawTextBuffer->vertices, Vertex{ pos1.x, pos0.y, 0.0f, uv1.x, uv0.y });

                advanceX += glyph.Advance;
                advanceY += 0;
            }
        }
    }
}
