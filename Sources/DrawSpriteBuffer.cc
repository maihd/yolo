#include "./DrawSpriteBuffer.h"

#include <GL/glew.h>

#include <Yolo/Math.h>
#include <Yolo/Array.h>

namespace DrawSpriteBufferOps
{
    static inline bool ShouldAppendCommand(DrawSpriteBuffer::Command command, Handle textureHandle)
    {
        return command.textureHandle == textureHandle;
    }

    DrawSpriteBuffer New(void)
    {
        VertexArray vertexArray = MakeVertexArray();
        DefineAttribute(vertexArray, 0, DataType::Vector3, sizeof(VertexColor), offsetof(VertexColor, position));
        DefineAttribute(vertexArray, 1, DataType::Vector2, sizeof(VertexColor), offsetof(VertexColor, uv));
        DefineAttribute(vertexArray, 2, DataType::Vector4, sizeof(VertexColor), offsetof(VertexColor, color));

        return DrawSpriteBuffer{
            false,
            vertexArray,

            MakeArray<VertexColor>(),
            MakeArray<U16>(),
            MakeArray<DrawSpriteBuffer::Command>(),
        };
    }

    void Free(DrawSpriteBuffer* drawSpriteBuffer)
    {
        drawSpriteBuffer->shouldUpdate = false;
        
        FreeVertexArray(&drawSpriteBuffer->vertexArray);

        FreeArray(&drawSpriteBuffer->commands);
        FreeArray(&drawSpriteBuffer->indices);
        FreeArray(&drawSpriteBuffer->vertices);
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

        U16 startIndex = (U16)drawSpriteBuffer->vertices.Count;
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

        U16 startIndex = (U16)drawSpriteBuffer->vertices.Count;
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
        if (drawSpriteBuffer->commands.Count > 0)
        {
            DrawSpriteBuffer::Command* command = &drawSpriteBuffer->commands.Items[drawSpriteBuffer->commands.Count - 1];
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

    void AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, Vector2 position, float rotation, Vector2 scale, Vector4 color)
    {

    }

    void AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, Vector2 position, float rotation, Vector2 scale, Vector4 color)
    {

    }

    void AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, Vector2 position, float rotation, Vector2 scale, Vector4 color, Vector2 pivot)
    {
        Matrix4 transform = Matrix4Transform2D(position, rotation, scale, pivot * Vector2{ (float)texture.Width, (float)texture.Height });

        Vector3 pos0 = mul(transform, Vector3{ 0, 0 });
        Vector3 pos1 = mul(transform, Vector3{ 0, (float)texture.Height });
        Vector3 pos2 = mul(transform, Vector3{ (float)texture.Width, (float)texture.Height });
        Vector3 pos3 = mul(transform, Vector3{ (float)texture.Width, 0 });

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

        AddQuad(drawSpriteBuffer, v0, v1, v2, v3, texture.Handle);
    }

    void UpdateBuffers(DrawSpriteBuffer* drawSpriteBuffer)
    {
        assert(drawSpriteBuffer);

        if (drawSpriteBuffer->shouldUpdate)
        {
            drawSpriteBuffer->shouldUpdate = false;

            SetIndexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->indices.Items, ArraySizeInBytes(drawSpriteBuffer->indices), BufferUsage::StreamDraw);
            SetVertexData(drawSpriteBuffer->vertexArray, drawSpriteBuffer->vertices.Items, ArraySizeInBytes(drawSpriteBuffer->vertices), BufferUsage::StreamDraw);
        }
    }

    void Draw(DrawSpriteBuffer* drawSpriteBuffer, Shader shader, Matrix4 projection)
    {
        UpdateBuffers(drawSpriteBuffer);

        Matrix4 model = Matrix4Translation(0, 0, 0);

        glUseProgram(shader.Handle);
        glUniformMatrix4fv(glGetUniformLocation(shader.Handle, "projection"), 1, false, (float*)&projection);
        glUniformMatrix4fv(glGetUniformLocation(shader.Handle, "model"), 1, false, (float*)&model);

        glBindVertexArray(drawSpriteBuffer->vertexArray.Handle);
        glBindBuffer(GL_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.VertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawSpriteBuffer->vertexArray.IndexBuffer);

        for (I32 i = 0, n = drawSpriteBuffer->commands.Count; i < n; i++)
        {
            DrawSpriteBuffer::Command command = drawSpriteBuffer->commands.Items[i];

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