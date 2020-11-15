#pragma once

#include <GL/glew.h>
#include <Yolo/Array.h>
#include <Yolo/Types.h>

struct VertexColor
{
    Vector3 position;
    Vector2 uv;
    Vector4 color;
};

struct SpriteBatch
{
    bool    shouldUpdate;

    Handle  vertexBuffer;
    Handle  indexBuffer;
    Handle  vertexArray;

    VertexColor*    vertices;
    U16*            indices;

    static SpriteBatch  New(VertexColor* vertices = 0, U16* indices = 0);
    static void         Free(SpriteBatch* drawBuffer);

    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor v0, VertexColor v1, VertexColor v2);
    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor* vertices);
    static void         AddTriangle(SpriteBatch* drawBuffer, VertexColor* vertices, I32 count);

    static void         AddCircle(SpriteBatch* drawBuffer, Vector2 position, float radius, Vector4 color, I32 segments = 30);
    static void         AddRectangle(SpriteBatch* drawBuffer, Vector2 position, Vector2 size, Vector4 color);

    static void         Clear(SpriteBatch* drawBuffer);
    static void         UpdateBuffers(SpriteBatch* drawBuffer);
};