#include "./DrawBuffer.h"

#include <Yolo/Math.h>
#include <Yolo/Array.h>
#include <Yolo/VertexArray.h>

#include "./OpenGL.h"

DrawBuffer DrawBufferNew()
{
    VertexArray vertexArray = VertexArrayOps::New();
    VertexArrayOps::DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexShape), offsetof(VertexShape, position));

    DrawBuffer drawBuffer = {
        false,
        vertexArray,
        ArrayNew<VertexShape>(),
        ArrayNew<U16>()
    };

    return drawBuffer;
}

void DrawBufferFree(DrawBuffer* drawBuffer)
{
    assert(drawBuffer);

    VertexArrayOps::Free(&drawBuffer->vertexArray);

    ArrayFree(&drawBuffer->vertices);
    ArrayFree(&drawBuffer->indices);
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2)
{
    assert(drawBuffer);

    U16 startIndex = (U16)drawBuffer->vertices.count;
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 2u));

    ArrayPush(&drawBuffer->vertices, v0);
    ArrayPush(&drawBuffer->vertices, v1);
    ArrayPush(&drawBuffer->vertices, v2);

    drawBuffer->shouldUpdate = true;
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, Array<VertexShape> vertices)
{
    assert(drawBuffer);

    DrawBufferAddTriangle(drawBuffer, vertices.elements, vertices.count);
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, I32 count)
{
    assert(drawBuffer);

    for (I32 i = 0; i < count; i += 3)
    {
        VertexShape v0 = vertices[i + 0];
        VertexShape v1 = vertices[i + 1];
        VertexShape v2 = vertices[i + 2];

        DrawBufferAddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBufferClear(DrawBuffer* drawBuffer)
{
    drawBuffer->shouldUpdate = true;

    ArrayClear(&drawBuffer->vertices);
    ArrayClear(&drawBuffer->indices);
}

void DrawBufferUpdateBuffers(DrawBuffer* drawBuffer)
{
    assert(drawBuffer);

    if (drawBuffer->shouldUpdate)
    {
        drawBuffer->shouldUpdate = false;

        VertexArrayOps::SetIndexData(drawBuffer->vertexArray, drawBuffer->indices.elements, ArraySizeInBytes(drawBuffer->indices), BufferUsage::StreamDraw);
        VertexArrayOps::SetVertexData(drawBuffer->vertexArray, drawBuffer->vertices.elements, ArraySizeInBytes(drawBuffer->vertices), BufferUsage::StreamDraw);
    }
}

void DrawBufferAddCircle(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segments)
{
    assert(drawBuffer);

    segments = segments < 0 ? 30 : segments;

    float step = PI * 2.0f / segments;

    for (I32 i = 0; i < segments; i++)
    {
        float angle0 = step * i;
        float angle1 = step * (i + 1);

        const VertexShape v0 = {
            { position.x + cosf(angle0) * radius, position.y + sinf(angle0) * radius },
        };

        const VertexShape v1 = {
            { position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius },
        };

        const VertexShape v2 = {
            { position.x, position.y },
        };

        DrawBufferAddTriangle(drawBuffer, v0, v1, v2);
    }
}

void DrawBufferAddCircleLines(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segments)
{
    assert(drawBuffer != nullptr);

    drawBuffer->shouldUpdate = true;

    segments = segments < 0 ? 30 : segments;

    float step = PI * 2.0f / segments;

    for (I32 i = 0; i <= segments; i++)
    {
        float angle = step * i;

        const VertexShape v = {
            { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius },
        };

        const U16 index = (U16)drawBuffer->vertices.count;
        ArrayPush(&drawBuffer->indices, index);
        ArrayPush(&drawBuffer->vertices, v);
    }
}

void DrawBufferAddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color)
{
    const VertexShape v0 = {
        { position.x, position.y },
    };

    const VertexShape v1 = {
        { position.x, position.y + size.y },
    };

    const VertexShape v2 = {
        { position.x + size.x, position.y + size.y },
    };

    const VertexShape v3 = {
        { position.x + size.x, position.y },
    };

    const U16 startIndex = (U16)drawBuffer->vertices.count;
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 3u));

    ArrayPush(&drawBuffer->vertices, v0);
    ArrayPush(&drawBuffer->vertices, v1);
    ArrayPush(&drawBuffer->vertices, v2);
    ArrayPush(&drawBuffer->vertices, v3);

    drawBuffer->shouldUpdate = true;
}

void DrawBufferAddRectangleLines(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color)
{
    const VertexShape v0 = {
        { position.x, position.y },
    };

    const VertexShape v1 = {
        { position.x, position.y + size.y },
    };

    const VertexShape v2 = {
        { position.x + size.x, position.y + size.y },
    };

    const VertexShape v3 = {
        { position.x + size.x, position.y },
    };

    const U16 startIndex = (U16)drawBuffer->vertices.count;
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 3u));
    ArrayPush(&drawBuffer->indices, (U16)(startIndex + 0u));

    ArrayPush(&drawBuffer->vertices, v0);
    ArrayPush(&drawBuffer->vertices, v1);
    ArrayPush(&drawBuffer->vertices, v2);
    ArrayPush(&drawBuffer->vertices, v3);

    drawBuffer->shouldUpdate = true;
}
