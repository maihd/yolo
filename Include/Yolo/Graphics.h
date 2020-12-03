#pragma once

#include <Yolo/Core.h>

// -----------------------------------
// Types
// -----------------------------------

struct Vertex
{
    Vector3     position;
    Vector2     uv;
};

struct VertexColor
{
    Vector3     position;
    Vector2     uv;
    Vector4     color;
};

struct VertexColorU32
{
    Vector3     position;
    Vector2     uv;
    U32         color;
};

struct VertexShape
{
    Vector3     position;
};

struct VertexShapeColor
{
    Vector3     position;
    Vector4     color;
};

struct VertexShapeColorU32
{
    Vector3     position;
    U32         color;
};

// -----------------------------------
// Working with texture
// -----------------------------------

Texture MakeTexture(const void* pixels, I32 width, I32 height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);
Texture LoadTexture(StringView path);
void    FreeTexture(Texture* texture);

// -----------------------------------
// Working with font
// -----------------------------------

Font    LoadFont(StringView path, float fontSize);
void    FreeFont(Font* font);

// -----------------------------------
// Working with shader
// -----------------------------------

Shader  LoadShader(StringView vertexFile, StringView pixelsFile);
Shader  CompileShader(StringView vertexSource, StringView pixelsSource);
void    FreeShader(Shader* shader);

// -----------------------------------
// Drawing
// -----------------------------------

bool    BeginDrawing(void);
void    EndDrawing(void);

void    DrawCircle(DrawMode mode, Vector2 position, float radius, Vector4 color, int segments = 30);
void    DrawRectangle(DrawMode mode, Vector2 position, Vector2 size, Vector4 color);

void    DrawTexture(Texture texture, Vector2 position, float rotation = 0.0f, Vector2 scale = Vector2{ 1, 1 }, Vector4 color = Vector4{ 1, 1, 1, 1 }, Vector2 pivot = Vector2{ 0.5f, 0.5f });

void    DrawText(StringView text, Font font, Vector2 position);
void    DrawFramerate(Font font, Vector2 position);

// -----------------------------------
// Graphics state
// -----------------------------------

namespace Graphics
{
    void    Clear(void);

    Vector4 GetClearColor(void);
    void    SetClearColor(Vector3 color);
    void    SetClearColor(Vector4 color);
    void    SetClearColor(float r, float g, float b, float a = 1.0f);

    bool    IsWireframe(void);
    void    SetWireframe(bool enable);

    Matrix4 GetProjection(void);
    void    SetProjection(Matrix4 projection);

    float   GetLineWidth(void);
    void    SetLineWidth(float width);
}
