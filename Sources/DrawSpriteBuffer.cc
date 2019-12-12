#include "./DrawSpriteBuffer.h"

#include <GL/glew.h>

#include <Yolo/Math.h>
#include <Yolo/Array.h>
#include <Yolo/VertexArray.h>

namespace DrawSpriteBufferOps
{
    static inline bool ShouldAppendCommand(DrawSpriteBuffer::Command command, Handle texture)
    {
        return command.textureHandle == texture;
    }

    DrawSpriteBuffer New(void)
    {
        VertexArray vertexArray = VertexArrayOps::New();
        VertexArrayOps::DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexColor), offsetof(VertexColor, position));
        VertexArrayOps::DefineAttribute(vertexArray, 1, DataType::Vector2, sizeof(VertexColor), offsetof(VertexColor, uv));
        VertexArrayOps::DefineAttribute(vertexArray, 2, DataType::Vector4, sizeof(VertexColor), offsetof(VertexColor, color));

        return DrawSpriteBuffer{
            false,
            vertexArray,

            ArrayOps::New<VertexColor>(),
            ArrayOps::New<uint16>(),
            ArrayOps::New<DrawSpriteBuffer::Command>(),
        };
    }

    void Free(DrawSpriteBuffer* drawSpriteBuffer)
    {
        drawSpriteBuffer->shouldUpdate = false;
        
        VertexArrayOps::Free(&drawSpriteBuffer->vertexArray);

        ArrayOps::Free(&drawSpriteBuffer->commands);
        ArrayOps::Free(&drawSpriteBuffer->indices);
        ArrayOps::Free(&drawSpriteBuffer->vertices);
    }

    void Clear(DrawSpriteBuffer* drawSpriteBuffer)
    {
        drawSpriteBuffer->shouldUpdate = true;

        ArrayOps::Clear(&drawSpriteBuffer->commands);
        ArrayOps::Clear(&drawSpriteBuffer->indices);
        ArrayOps::Clear(&drawSpriteBuffer->vertices);
    }

    void AddTriangle(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, Handle texture)
    {
        drawSpriteBuffer->shouldUpdate = true;

        uint16 startIndex = (uint16)drawSpriteBuffer->vertices.length;
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 0));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 1));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 2));

        ArrayOps::Push(&drawSpriteBuffer->vertices, v0);
        ArrayOps::Push(&drawSpriteBuffer->vertices, v1);
        ArrayOps::Push(&drawSpriteBuffer->vertices, v2);

        DrawSpriteBuffer::Command command = {
            3,
            startIndex,
            texture
        };

        ArrayOps::Push(&drawSpriteBuffer->commands, command);
    }

    void AddQuad(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, VertexColor v3, Handle texture)
    {
        drawSpriteBuffer->shouldUpdate = true;

        uint16 startIndex = (uint16)drawSpriteBuffer->vertices.length;
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 0));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 1));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 2));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 0));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 2));
        ArrayOps::Push(&drawSpriteBuffer->indices, (uint16)(startIndex + 3));

        ArrayOps::Push(&drawSpriteBuffer->vertices, v0);
        ArrayOps::Push(&drawSpriteBuffer->vertices, v1);
        ArrayOps::Push(&drawSpriteBuffer->vertices, v2);
        ArrayOps::Push(&drawSpriteBuffer->vertices, v3);

        bool addNewCommand = true;
        if (drawSpriteBuffer->commands.length > 0)
        {
            DrawSpriteBuffer::Command* command = &drawSpriteBuffer->commands.elements[drawSpriteBuffer->commands.length - 1];
            if (ShouldAppendCommand(*command, texture))
            {
                command->indexCount += 6;
                addNewCommand = false;
            }
        }

        if (addNewCommand)
        {
            DrawSpriteBuffer::Command command = {
            6,
            startIndex,
            texture
            };

            ArrayOps::Push(&drawSpriteBuffer->commands, command);
        }
    }

    void AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, vec2 position, float rotation, vec2 scale, vec4 color);

    void AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, vec2 position, float rotation, vec2 scale, vec4 color);
    void AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, vec2 position, float rotation, vec2 scale, vec4 color)
    {
        mat4 transform = Math::Transform(position, rotation, scale);

        vec3 pos0 = mul(transform, vec3{ 0, 0 });
        vec3 pos1 = mul(transform, vec3{ 0, (float)texture.height });
        vec3 pos2 = mul(transform, vec3{ (float)texture.width, (float)texture.height });
        vec3 pos3 = mul(transform, vec3{ (float)texture.width, 0 });

        VertexColor v0 = {
            pos0,
            vec2{ 0.0f, 1.0f },
            color,
        };

        VertexColor v1 = {
            pos1,
            vec2{ 0.0f, 0.0f },
            color,
        };

        VertexColor v2 = {
            pos2,
            vec2{ 1.0f, 0.0f },
            color,
        };

        VertexColor v3 = {
            pos3,
            vec2{ 1.0f, 1.0f },
            color,
        };

        AddQuad(drawSpriteBuffer, v0, v1, v2, v3, texture.handle);
    }

    void UpdateBuffers(DrawSpriteBuffer* drawSpriteBuffer)
    {
        assert(drawSpriteBuffer);

        if (drawSpriteBuffer->shouldUpdate)
        {
            drawSpriteBuffer->shouldUpdate = false;

            VertexArrayOps::SetIndexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->indices.elements, ArrayOps::SizeOf(drawSpriteBuffer->indices), BufferUsage::StreamDraw);
            VertexArrayOps::SetVertexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->vertices.elements, ArrayOps::SizeOf(drawSpriteBuffer->vertices), BufferUsage::StreamDraw);
        }
    }

    void Draw(DrawSpriteBuffer* drawSpriteBuffer, Shader shader, mat4 projection)
    {
        UpdateBuffers(drawSpriteBuffer);

        mat4 model = Math::Translation(0, 0, 0);

        glUseProgram(shader.handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawSpriteBuffer->vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.indexBuffer);

        for (int i = 0, n = drawSpriteBuffer->commands.length; i < n; i++)
        {
            DrawSpriteBuffer::Command command = drawSpriteBuffer->commands.elements[i];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, command.textureHandle);

            glDrawElements(GL_TRIANGLES, command.indexCount, GL_UNSIGNED_SHORT, (const void*)(GLintptr)(command.indexOffset * sizeof(uint16)));

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }
}