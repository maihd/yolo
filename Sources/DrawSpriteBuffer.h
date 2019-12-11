#pragma once

#include <Yolo/Font.h>
#include <Yolo/Sprite.h>
#include <Yolo/Texture.h>

#include <Yolo/Vertex.h>
#include <Yolo/VertexArray.h>

struct DrawSpriteBuffer
{
    struct Command
    {
        int     indexCount;
        int     indexOffset;
        Handle  textureHandle;
    };

    bool        shouldUpdate;

    VertexArray vertexArray;
    
    Vertex*     vertices;
    uint16*     indices;
    Command*    commands;
};

namespace DrawSpriteBufferOps
{
    DrawSpriteBuffer New(void);
    void             Free(DrawSpriteBuffer* drawSpriteBuffer);

    void             AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, vec2 position, float rotation, vec2 scale, vec4 color);
    void             AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, vec2 position, float rotation, vec2 scale, vec4 color);
    void             AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, vec2 position, float rotation, vec2 scale, vec4 color);

    void             UpdateBuffers(DrawSpriteBuffer* drawSpriteBuffer);
}
