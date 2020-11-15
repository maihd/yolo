#include "./DrawSpriteBuffer.h"

#include <GL/glew.h>

#include <Yolo/Math.h>
#include <Yolo/Array.h>
#include <Yolo/VertexArray.h>

namespace DrawSpriteBufferOps
{
    static inline bool ShouldAppendCommand(DrawSpriteBuffer::Command command, Handle textureHandle)
    {
        return command.textureHandle == textureHandle;
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

            ArrayNew<VertexColor>(),
            ArrayNew<U16>(),
            ArrayNew<DrawSpriteBuffer::Command>(),
        };
    }

    void Free(DrawSpriteBuffer* drawSpriteBuffer)
    {
        drawSpriteBuffer->shouldUpdate = false;
        
        VertexArrayOps::Free(&drawSpriteBuffer->vertexArray);

        ArrayFree(&drawSpriteBuffer->commands);
        ArrayFree(&drawSpriteBuffer->indices);
        ArrayFree(&drawSpriteBuffer->vertices);
    }

    void Clear(DrawSpriteBuffer* drawSpriteBuffer)
    {
        drawSpriteBuffer->shouldUpdate = true;

        ArrayClear(&drawSpriteBuffer->commands);
        ArrayClear(&drawSpriteBuffer->indices);
        ArrayClear(&drawSpriteBuffer->vertices);
    }

    void AddTriangle(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, Handle texture)
    {
        drawSpriteBuffer->shouldUpdate = true;

        U16 startIndex = (U16)drawSpriteBuffer->vertices.count;
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 0));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 1));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 2));

        ArrayPush(&drawSpriteBuffer->vertices, v0);
        ArrayPush(&drawSpriteBuffer->vertices, v1);
        ArrayPush(&drawSpriteBuffer->vertices, v2);

        DrawSpriteBuffer::Command command = {
            3,
            startIndex,
            texture
        };

        ArrayPush(&drawSpriteBuffer->commands, command);
    }

    void AddQuad(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, VertexColor v3, Handle texture)
    {
        drawSpriteBuffer->shouldUpdate = true;

        U16 startIndex = (U16)drawSpriteBuffer->vertices.count;
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 0));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 1));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 2));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 0));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 2));
        ArrayPush(&drawSpriteBuffer->indices, (U16)(startIndex + 3));

        ArrayPush(&drawSpriteBuffer->vertices, v0);
        ArrayPush(&drawSpriteBuffer->vertices, v1);
        ArrayPush(&drawSpriteBuffer->vertices, v2);
        ArrayPush(&drawSpriteBuffer->vertices, v3);

        bool addNewCommand = true;
        if (drawSpriteBuffer->commands.count > 0)
        {
            DrawSpriteBuffer::Command* command = &drawSpriteBuffer->commands.elements[drawSpriteBuffer->commands.count - 1];
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

            ArrayPush(&drawSpriteBuffer->commands, command);
        }
    }

    void AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, Vector2 position, F32 rotation, Vector2 scale, Vector4 color)
    {

    }

    void AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, Vector2 position, F32 rotation, Vector2 scale, Vector4 color)
    {

    }

    void AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, Vector2 position, F32 rotation, Vector2 scale, Vector4 color, Vector2 pivot)
    {
        Matrix4 transform = Matrix4Transform2D(position, rotation, scale, pivot * Vector2{ (F32)texture.width, (F32)texture.height });

        Vector3 pos0 = mul(transform, Vector3{ 0, 0 });
        Vector3 pos1 = mul(transform, Vector3{ 0, (F32)texture.height });
        Vector3 pos2 = mul(transform, Vector3{ (F32)texture.width, (F32)texture.height });
        Vector3 pos3 = mul(transform, Vector3{ (F32)texture.width, 0 });

        VertexColor v0 = {
            pos0,
            Vector2{ 0.0f, 1.0f },
            color,
        };

        VertexColor v1 = {
            pos1,
            Vector2{ 0.0f, 0.0f },
            color,
        };

        VertexColor v2 = {
            pos2,
            Vector2{ 1.0f, 0.0f },
            color,
        };

        VertexColor v3 = {
            pos3,
            Vector2{ 1.0f, 1.0f },
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

            VertexArrayOps::SetIndexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->indices.elements, ArraySizeInBytes(drawSpriteBuffer->indices), BufferUsage::StreamDraw);
            VertexArrayOps::SetVertexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->vertices.elements, ArraySizeInBytes(drawSpriteBuffer->vertices), BufferUsage::StreamDraw);
        }
    }

    void Draw(DrawSpriteBuffer* drawSpriteBuffer, Shader shader, Matrix4 projection)
    {
        UpdateBuffers(drawSpriteBuffer);

        Matrix4 model = Matrix4Translation(0, 0, 0);

        glUseProgram(shader.handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "projection"), 1, false, (F32*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.handle, "model"), 1, false, (F32*)&model);

        glBindVertexArray(drawSpriteBuffer->vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.indexBuffer);

        for (I32 i = 0, n = drawSpriteBuffer->commands.count; i < n; i++)
        {
            DrawSpriteBuffer::Command command = drawSpriteBuffer->commands.elements[i];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, command.textureHandle);

            glDrawElements(GL_TRIANGLES, command.indexCount, GL_UNSIGNED_SHORT, (const void*)(GLintptr)(command.indexOffset * sizeof(U16)));

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);
    }
}