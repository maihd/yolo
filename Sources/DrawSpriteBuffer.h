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

    static DrawSpriteBuffer New(void);
    static void             Free(DrawSpriteBuffer* drawSpriteBuffer);

    static void             AddText(DrawSpriteBuffer* drawSpriteBuffer, String text, Font font, vec2 position, float rotation, vec2 scale, vec4 color);
    static void             AddSprite(DrawSpriteBuffer* drawSpriteBuffer, Sprite sprite, vec2 position, float rotation, vec2 scale, vec4 color);
    static void             AddTexture(DrawSpriteBuffer* drawSpriteBuffer, Texture texture, vec2 position, float rotation, vec2 scale, vec4 color);

    static void             UpdateBuffers(DrawSpriteBuffer* drawSpriteBuffer);
};