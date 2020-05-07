#pragma once

#include <Yolo/Types.h>
#include <Yolo/Vertex.h>

struct DrawSpriteBuffer
{
    struct Command
    {
        int     indexCount;
        int     indexOffset;
        Handle  textureHandle;
    };

    bool                    shouldUpdate;

    VertexArray             vertexArray;
    
    Array<VertexColor>      vertices;
    Array<uint16>           indices;
    Array<Command>          commands;
};

namespace DrawSpriteBufferOps
{
    DrawSpriteBuffer    New(void);
    void                Free(DrawSpriteBuffer* drawSpriteBuffer);

    void                Clear(DrawSpriteBuffer* drawSpriteBuffer);

    void                AddTriangle(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, Handle texture);
    void                AddQuad(DrawSpriteBuffer* drawSpriteBuffer, VertexColor v0, VertexColor v1, VertexColor v2, VertexColor v3, Handle texture);
    
    void                AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, Vector2 position, float rotation, Vector2 scale, Vector4 color);
    void                AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, Vector2 position, float rotation, Vector2 scale, Vector4 color);
    void                AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, Vector2 position, float rotation, Vector2 scale, Vector4 color, Vector2 pivot = Vector2{ 0.5f, 0.5f });

    void                UpdateBuffers(DrawSpriteBuffer* drawSpriteBuffer);
    void                Draw(DrawSpriteBuffer* drawSpriteBuffer, Shader shader, Matrix4 projection);
}
