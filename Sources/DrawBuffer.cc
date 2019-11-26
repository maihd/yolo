#include "./DrawBuffer.h"

#include <Yolo/Math.h>
#include <math.h>

DrawBuffer DrawBuffer::New(Vertex* vertices, uint16* indices)
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

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex v0, Vertex v1, Vertex v2)
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

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices)
{
    assert(drawBuffer);
    assert(Array::IsArray(vertices));
    
    DrawBuffer::AddTriangle(drawBuffer, vertices, Array::Length(vertices));
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices, int count)
{
    assert(drawBuffer);

    for (int i = 0; i < count; i += 3)
    {
        Vertex v0 = vertices[i + 0];
        Vertex v1 = vertices[i + 1];
        Vertex v2 = vertices[i + 2];

        AddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBuffer::Clear(DrawBuffer* drawBuffer)
{
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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));
        }

        glBindBuffer(GL_ARRAY_BUFFER, drawBuffer->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, Array::Length(drawBuffer->vertices) * sizeof(Vertex), drawBuffer->vertices, GL_DYNAMIC_DRAW);
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

        const Vertex v0 = {
            vec3::From(position.x + cosf(angle0) * radius, position.y + sinf(angle0) * radius),
            vec2::From(0, 0),
            color
        };

        const Vertex v1 = {
            vec3::From(position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius),
            vec2::From(0, 0),
            color
        };

        const Vertex v2 = {
            vec3::From(position.x, position.y),
            vec2::From(0, 0),
            color
        };

        AddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBuffer::AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color)
{
    const Vertex v0 = {
        vec3::From(position.x, position.y),
        vec2::From(0, 0),
        color
    };

    const Vertex v1 = {
        vec3::From(position.x, position.y + size.y),
        vec2::From(0, 0),
        color
    };

    const Vertex v2 = {
        vec3::From(position.x + size.x, position.y + size.y),
        vec2::From(0, 0),
        color
    };

    const Vertex v3 = {
        vec3::From(position.x + size.x, position.y),
        vec2::From(0, 0),
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