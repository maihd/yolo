#include "./DrawBuffer.h"

#include <Yolo/Math.h>
#include <Yolo/Macros.h>

#include "./OpenGL.h"

namespace DrawBufferOps
{
    DrawBuffer New(VertexShape* vertices, uint16* indices)
    {
        VertexArray vertexArray = VertexArray::New();
        VertexArray::DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexShape), offsetof(VertexShape, position));

        DrawBuffer drawBuffer = {
            false,
            vertexArray,
            vertices,
            indices
        };

        return drawBuffer;
    }

    void Free(DrawBuffer* drawBuffer)
    {
        assert(drawBuffer);

        VertexArray::Free(&drawBuffer->vertexArray);

        Array::Free(&drawBuffer->vertices);
        Array::Free(&drawBuffer->indices);
    }

    void AddTriangle(DrawBuffer* drawBuffer, VertexShape v0, VertexShape v1, VertexShape v2)
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

    void AddTriangle(DrawBuffer* drawBuffer, VertexShape* vertices)
    {
        assert(drawBuffer);
        assert(Array::IsArray(vertices));

        AddTriangle(drawBuffer, vertices, Array::Length(vertices));
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

        Array::Clear(&drawBuffer->vertices);
        Array::Clear(&drawBuffer->indices);
    }

    void UpdateBuffers(DrawBuffer* drawBuffer)
    {
        assert(drawBuffer);

        if (drawBuffer->shouldUpdate)
        {
            drawBuffer->shouldUpdate = false;

            VertexArray::SetIndexData(drawBuffer->vertexArray, drawBuffer->indices, Array::SizeOf(drawBuffer->indices), BufferUsage::StreamDraw);
            VertexArray::SetVertexData(drawBuffer->vertexArray, drawBuffer->vertices, Array::SizeOf(drawBuffer->vertices), BufferUsage::StreamDraw);
        }
    }

    void AddCircle(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments)
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

    void AddCircleLines(DrawBuffer* drawBuffer, vec2 position, float radius, vec4 color, int segments)
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

            const uint16 index = (uint16)Array::Length(drawBuffer->vertices);
            Array::Push(&drawBuffer->indices, index);
            Array::Push(&drawBuffer->vertices, v);
        }
    }

    void AddRectangle(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color)
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

    void AddRectangleLines(DrawBuffer* drawBuffer, vec2 position, vec2 size, vec4 color)
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
}
