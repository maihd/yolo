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

    VertexArrayOps::Free(&drawBuffer->VertexArray);

    ArrayFree(&drawBuffer->Vertices);
    ArrayFree(&drawBuffer->Indices);
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2)
{
    assert(drawBuffer);

    U16 startIndex = (U16)drawBuffer->Vertices.count;
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 2u));

    ArrayPush(&drawBuffer->Vertices, v0);
    ArrayPush(&drawBuffer->Vertices, v1);
    ArrayPush(&drawBuffer->Vertices, v2);

    drawBuffer->ShouldUpdate = true;
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
    drawBuffer->ShouldUpdate = true;

    ArrayClear(&drawBuffer->Vertices);
    ArrayClear(&drawBuffer->Indices);
}

void DrawBufferUpdateBuffers(DrawBuffer* drawBuffer)
{
    assert(drawBuffer);

    if (drawBuffer->ShouldUpdate)
    {
        drawBuffer->ShouldUpdate = false;

        VertexArrayOps::SetIndexData(drawBuffer->VertexArray, drawBuffer->Indices.elements, ArraySizeInBytes(drawBuffer->Indices), BufferUsage::StreamDraw);
        VertexArrayOps::SetVertexData(drawBuffer->VertexArray, drawBuffer->Vertices.elements, ArraySizeInBytes(drawBuffer->Vertices), BufferUsage::StreamDraw);
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

    drawBuffer->ShouldUpdate = true;

    segments = segments < 0 ? 30 : segments;

    float step = PI * 2.0f / segments;

    for (I32 i = 0; i <= segments; i++)
    {
        float angle = step * i;

        const VertexShape v = {
            { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius },
        };

        const U16 index = (U16)drawBuffer->Vertices.count;
        ArrayPush(&drawBuffer->Indices, index);
        ArrayPush(&drawBuffer->Vertices, v);
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

    const U16 startIndex = (U16)drawBuffer->Vertices.count;
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 3u));

    ArrayPush(&drawBuffer->Vertices, v0);
    ArrayPush(&drawBuffer->Vertices, v1);
    ArrayPush(&drawBuffer->Vertices, v2);
    ArrayPush(&drawBuffer->Vertices, v3);

    drawBuffer->ShouldUpdate = true;
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

    const U16 startIndex = (U16)drawBuffer->Vertices.count;
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 2u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 3u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));

    ArrayPush(&drawBuffer->Vertices, v0);
    ArrayPush(&drawBuffer->Vertices, v1);
    ArrayPush(&drawBuffer->Vertices, v2);
    ArrayPush(&drawBuffer->Vertices, v3);

    drawBuffer->ShouldUpdate = true;
}
