#include "./DrawBuffer.h"

#include <Yolo/Math.h>
#include <math.h>

DrawBuffer DrawBuffer::New(VertexShape* vertices, uint16* indices)
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

void DrawBuffer::Free(DrawBuffer* drawBuffer)
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

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2)
{
    assert(drawBuffer);

    uint16 startIndex = (uint16)Array::Length(drawBuffer->vertices);
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));

    Array::Push(&drawBuffer->vertices, v0);
    Array::Push(&drawBuffer->vertices, v1);
    Array::Push(&drawBuffer->vertices, v2);

    drawBuffer->shouldUpdate = true;
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices)
{
    assert(drawBuffer);
    assert(Array::IsArray(vertices));
    
    DrawBuffer::AddTriangle(drawBuffer, vertices, Array::Length(vertices));
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, int count)
{
    assert(drawBuffer);

    for (int i = 0; i < count; i += 3)
    {
        VertexShape v0 = vertices[i + 0];
        VertexShape v1 = vertices[i + 1];
        VertexShape v2 = vertices[i + 2];

        AddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBuffer::Clear(DrawBuffer* drawBuffer)
{
    if (drawBuffer->vertexArray)
    {
        drawBuffer->shouldUpdate = true;
    }

    Array::Clear(&drawBuffer->vertices);
    Array::Clear(&drawBuffer->indices);
}

void DrawBuffer::UpdateBuffers(DrawBuffer* drawBuffer)
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexShape), (const void*)offsetof(VertexShape, position));

            //glEnableVertexAttribArray(1);
            //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (const void*)offsetof(VertexColor, uv));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexShape), (const void*)offsetof(VertexShape, color));
        }

        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, Array::Length(drawBuffer->vertices) * sizeof(VertexColor), drawBuffer->vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawBuffer->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Array::Length(drawBuffer->indices) * sizeof(uint16), drawBuffer->indices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void DrawBuffer::AddCircle(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments)
{
    assert(drawBuffer);

    segments = segments < 0 ? 30 : segments;

    float step = Const::PI * 2.0f / segments;

    for (int i = 0; i < segments; i++)
    {
        float angle0 = step * i;
        float angle1 = step * (i + 1);

        const VertexShape v0 = {
            { position.x + cosf(angle0) * radius, position.y + sinf(angle0) * radius },
            color
        };

        const VertexShape v1 = {
            { position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius },
            color
        };

        const VertexShape v2 = {
            { position.x, position.y },
            color
        };

        AddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBuffer::AddCircleLines(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments)
{
    assert(drawBuffer != nullptr);

    drawBuffer->shouldUpdate = true;

    segments = segments < 0 ? 30 : segments;

    float step = Const::PI * 2.0f / segments;

    for (int i = 0; i <= segments; i++)
    {
        float angle = step * i;

        const VertexShape v = {
            { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius },
            color
        };

        const uint16 index = (uint16)Array::Length(drawBuffer->vertices);
        Array::Push(&drawBuffer->indices, index);
        Array::Push(&drawBuffer->vertices, v);
    }
}

void DrawBuffer::AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color)
{
    const VertexShape v0 = {
        { position.x, position.y },
        color
    };

    const VertexShape v1 = {
        { position.x, position.y + size.y },
        color
    };

    const VertexShape v2 = {
        { position.x + size.x, position.y + size.y },
        color
    };

    const VertexShape v3 = {
        { position.x + size.x, position.y },
        color
    };

    const uint16 startIndex = (uint16)Array::Length(drawBuffer->vertices);
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 3u));

    Array::Push(&drawBuffer->vertices, v0);
    Array::Push(&drawBuffer->vertices, v1);
    Array::Push(&drawBuffer->vertices, v2);
    Array::Push(&drawBuffer->vertices, v3);

    drawBuffer->shouldUpdate = true;
}

void DrawBuffer::AddRectangleLines(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color)
{
    const VertexShape v0 = {
        { position.x, position.y },
        color
    };

    const VertexShape v1 = {
        { position.x, position.y + size.y },
        color
    };

    const VertexShape v2 = {
        { position.x + size.x, position.y + size.y },
        color
    };

    const VertexShape v3 = {
        { position.x + size.x, position.y },
        color
    };

    const uint16 startIndex = (uint16)Array::Length(drawBuffer->vertices);
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 3u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));

    Array::Push(&drawBuffer->vertices, v0);
    Array::Push(&drawBuffer->vertices, v1);
    Array::Push(&drawBuffer->vertices, v2);
    Array::Push(&drawBuffer->vertices, v3);

    drawBuffer->shouldUpdate = true;
}