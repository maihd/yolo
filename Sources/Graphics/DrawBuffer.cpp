#include "./DrawBuffer.h"

#include <Math/Math.h>
#include <Container/Array.h>

#include "./OpenGL.h"

DrawBuffer DrawBufferNew()
{
    VertexArray vertexArray = MakeVertexArray();
    DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexShapeColor), offsetof(VertexShapeColor, position));
    DefineAttribute(vertexArray, 1, DataType::Vector4, sizeof(VertexShapeColor), offsetof(VertexShapeColor, color));

    DrawBuffer drawBuffer = {
        false,
        vertexArray,
        MakeArray<VertexShapeColor>(),
        MakeArray<U16>()
    };

    return drawBuffer;
}

void DrawBufferFree(DrawBuffer* drawBuffer)
{
    assert(drawBuffer);

    FreeVertexArray(&drawBuffer->VertexArray);

    FreeArray(&drawBuffer->Vertices);
    FreeArray(&drawBuffer->Indices);
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShapeColor v0, VertexShapeColor v1, VertexShapeColor v2)
{
    assert(drawBuffer);

    U16 startIndex = (U16)drawBuffer->Vertices.Count;
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 0u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 1u));
    ArrayPush(&drawBuffer->Indices, (U16)(startIndex + 2u));

    ArrayPush(&drawBuffer->Vertices, v0);
    ArrayPush(&drawBuffer->Vertices, v1);
    ArrayPush(&drawBuffer->Vertices, v2);

    drawBuffer->ShouldUpdate = true;
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, Array<VertexShapeColor> vertices)
{
    assert(drawBuffer);

    DrawBufferAddTriangle(drawBuffer, vertices.Items, vertices.Count);
}

void DrawBufferAddTriangle(DrawBuffer* drawBuffer, VertexShapeColor* vertices, I32 count)
{
    assert(drawBuffer);

    for (I32 i = 0; i < count; i += 3)
    {
        VertexShapeColor v0 = vertices[i + 0];
        VertexShapeColor v1 = vertices[i + 1];
        VertexShapeColor v2 = vertices[i + 2];

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

        SetIndexData(drawBuffer->VertexArray, drawBuffer->Indices.Items, ArraySizeInBytes(drawBuffer->Indices), BufferUsage::StreamDraw);
        SetVertexData(drawBuffer->VertexArray, drawBuffer->Vertices.Items, ArraySizeInBytes(drawBuffer->Vertices), BufferUsage::StreamDraw);
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

        const VertexShapeColor v0 = {
            { position.x + cosf(angle0) * radius, position.y + sinf(angle0) * radius },
            color
        };

        const VertexShapeColor v1 = {
            { position.x + cosf(angle1) * radius, position.y + sinf(angle1) * radius },
            color
        };

        const VertexShapeColor v2 = {
            { position.x, position.y },
            color
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

        const VertexShapeColor v = {
            { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius },
            color
        };

        const U16 index = (U16)drawBuffer->Vertices.Count;
        ArrayPush(&drawBuffer->Indices, index);
        ArrayPush(&drawBuffer->Vertices, v);
    }
}

void DrawBufferAddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color)
{
    const VertexShapeColor v0 = {
        { position.x, position.y },
        color
    };

    const VertexShapeColor v1 = {
        { position.x, position.y + size.y },
        color
    };

    const VertexShapeColor v2 = {
        { position.x + size.x, position.y + size.y },
        color
    };

    const VertexShapeColor v3 = {
        { position.x + size.x, position.y },
        color
    };

    const U16 startIndex = (U16)drawBuffer->Vertices.Count;
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
    const VertexShapeColor v0 = {
        { position.x, position.y },
        color
    };

    const VertexShapeColor v1 = {
        { position.x, position.y + size.y },
        color
    };

    const VertexShapeColor v2 = {
        { position.x + size.x, position.y + size.y },
        color
    };

    const VertexShapeColor v3 = {
        { position.x + size.x, position.y },
        color
    };

    const U16 startIndex = (U16)drawBuffer->Vertices.Count;
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
