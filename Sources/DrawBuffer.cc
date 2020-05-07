#include "./DrawBuffer.h"

#include <Yolo/Math.h>
#include <Yolo/Array.h>
#include <Yolo/VertexArray.h>

#include "./OpenGL.h"

namespace DrawBufferOps
{
    DrawBuffer New()
    {
        VertexArray vertexArray = VertexArrayOps::New();
        VertexArrayOps::DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexShape), offsetof(VertexShape, position));

        DrawBuffer drawBuffer = {
            false,
            vertexArray,
            ArrayOps::New<VertexShape>(),
            ArrayOps::New<uint16>()
        };

        return drawBuffer;
    }

    void Free(DrawBuffer* drawBuffer)
    {
        assert(drawBuffer);

        VertexArrayOps::Free(&drawBuffer->vertexArray);

        ArrayOps::Free(&drawBuffer->vertices);
        ArrayOps::Free(&drawBuffer->indices);
    }

    void AddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2)
    {
        assert(drawBuffer);

        uint16 startIndex = (uint16)drawBuffer->vertices.length;
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));

        ArrayOps::Push(&drawBuffer->vertices, v0);
        ArrayOps::Push(&drawBuffer->vertices, v1);
        ArrayOps::Push(&drawBuffer->vertices, v2);

        drawBuffer->shouldUpdate = true;
    }

    void AddTriangle(DrawBuffer* drawBuffer, Array<VertexShape> vertices)
    {
        assert(drawBuffer);

        AddTriangle(drawBuffer, vertices.elements, vertices.length);
    }

    void AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices, int count)
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

    void Clear(DrawBuffer* drawBuffer)
    {
        drawBuffer->shouldUpdate = true;

        ArrayOps::Clear(&drawBuffer->vertices);
        ArrayOps::Clear(&drawBuffer->indices);
    }

    void UpdateBuffers(DrawBuffer* drawBuffer)
    {
        assert(drawBuffer);

        if (drawBuffer->shouldUpdate)
        {
            drawBuffer->shouldUpdate = false;

            VertexArrayOps::SetIndexData(drawBuffer->vertexArray, drawBuffer->indices.elements, ArrayOps::SizeOf(drawBuffer->indices), BufferUsage::StreamDraw);
            VertexArrayOps::SetVertexData(drawBuffer->vertexArray, drawBuffer->vertices.elements, ArrayOps::SizeOf(drawBuffer->vertices), BufferUsage::StreamDraw);
        }
    }

    void AddCircle(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, int segments)
    {
        assert(drawBuffer);

        segments = segments < 0 ? 30 : segments;

        float step = PI * 2.0f / segments;

        for (int i = 0; i < segments; i++)
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

            AddTriangle(drawBuffer, v0, v1, v2);
        }
    }

    void AddCircleLines(DrawBuffer* drawBuffer, Vector2 position, float radius, Vector4 color, int segments)
    {
        assert(drawBuffer != nullptr);

        drawBuffer->shouldUpdate = true;

        segments = segments < 0 ? 30 : segments;

        float step = PI * 2.0f / segments;

        for (int i = 0; i <= segments; i++)
        {
            float angle = step * i;

            const VertexShape v = {
                { position.x + cosf(angle) * radius, position.y + sinf(angle) * radius },
            };

            const uint16 index = (uint16)drawBuffer->vertices.length;
            ArrayOps::Push(&drawBuffer->indices, index);
            ArrayOps::Push(&drawBuffer->vertices, v);
        }
    }

    void AddRectangle(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color)
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

        const uint16 startIndex = (uint16)drawBuffer->vertices.length;
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 3u));

        ArrayOps::Push(&drawBuffer->vertices, v0);
        ArrayOps::Push(&drawBuffer->vertices, v1);
        ArrayOps::Push(&drawBuffer->vertices, v2);
        ArrayOps::Push(&drawBuffer->vertices, v3);

        drawBuffer->shouldUpdate = true;
    }

    void AddRectangleLines(DrawBuffer* drawBuffer, Vector2 position, Vector2 size, Vector4 color)
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

        const uint16 startIndex = (uint16)drawBuffer->vertices.length;
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 1u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 2u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 3u));
        ArrayOps::Push(&drawBuffer->indices, (uint16)(startIndex + 0u));

        ArrayOps::Push(&drawBuffer->vertices, v0);
        ArrayOps::Push(&drawBuffer->vertices, v1);
        ArrayOps::Push(&drawBuffer->vertices, v2);
        ArrayOps::Push(&drawBuffer->vertices, v3);

        drawBuffer->shouldUpdate = true;
    }
}
