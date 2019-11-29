#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/MathTypes.h>

struct VertexColor
{
    vec3 position;
    vec2 uv;
    vec4 color;
};

struct SpriteBatch
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    VertexColor* vertices;
    uint16* indices;

    static SpriteBatch  New(VertexColor* vertices = 0, uint16* indices = 0);
    static void         Free(SpriteBatch* drawBuffer);

    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor v0, VertexColor v1, VertexColor v2);
    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor* vertices);
    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor* vertices, int count);

    static void         AddCircle(SpriteBatch* drawBuffer, vec2 position, float radius, vec4 color, int segments = 30);
    static void         AddRectangle(SpriteBatch* drawBuffer, vec2 position, vec2 size, vec4 color);

    static void         Clear(SpriteBatch* drawBuffer);
    static void         UpdateBuffers(SpriteBatch* drawBuffer);
};