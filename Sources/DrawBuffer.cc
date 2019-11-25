#include "./DrawBuffer.h"

DrawBuffer DrawBuffer::New(Vertex* vertices, uint16* indices)
{
    return { vertices, indices };
}

void DrawBuffer::Free(DrawBuffer* drawBuffer)
{
    Array::Free(&drawBuffer->vertices);
    Array::Free(&drawBuffer->indices);
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex v0, Vertex v1, Vertex v2)
{
    uint16 startIndex = (uint16)Array::Length(drawBuffer->vertices);
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
    Array::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));

    Array::Push(&drawBuffer->vertices, v0);
    Array::Push(&drawBuffer->vertices, v1);
    Array::Push(&drawBuffer->vertices, v2);
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices)
{
    assert(Array::IsArray(vertices));
    
    DrawBuffer::AddTriangle(drawBuffer, vertices, Array::Length(vertices));
}

void DrawBuffer::AddTriangle(DrawBuffer* drawBuffer, Vertex* vertices, int count)
{
    for (int i = 0; i < count; i += 3)
    {
        Vertex v0 = vertices[i + 0];
        Vertex v1 = vertices[i + 1];
        Vertex v2 = vertices[i + 2];

        AddTriangle(drawBuffer, v0, v1, v2);
    }
}

GLuint DrawBuffer::CreateVertexBuffer(DrawBuffer drawBuffer)
{
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, Array::Length(drawBuffer.vertices) * sizeof(Vertex), drawBuffer.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vertexBuffer;
}

GLuint DrawBuffer::CreateIndexBuffer(DrawBuffer drawBuffer)
{
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Array::Length(drawBuffer.indices) * sizeof(uint16), drawBuffer.indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return indexBuffer;
}