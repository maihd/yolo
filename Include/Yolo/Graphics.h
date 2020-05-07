#pragma once

#include <Yolo/Types.h>

namespace Graphics
{
    bool    Init(void);
    void    Quit(void);

    void    Clear(void);
    void    ClearColor(float r, float g, float b, float a = 1.0f);

    void    Present(void);

    bool    IsVSync(void);
    void    SetVSync(bool enable);

    bool    IsWireframe(void);
    void    SetWireframe(bool enable);

    Matrix4    GetProjection(void);
    void    SetProjection(Matrix4 projection);

    float   GetLineWidth(void);
    void    SetLineWidth(float width);

    void    DrawCircle(DrawMode mode, Vector2 position, float radius, Vector4 color, int segments = 30);
    void    DrawRectangle(DrawMode mode, Vector2 position, Vector2 size, Vector4 color);

    void    DrawTexture(Texture texture, Vector2 position, float rotation = 0.0f, Vector2 scale = Vector2{ 1, 1 }, Vector4 color = Vector4{ 1, 1, 1, 1 }, Vector2 pivot = Vector2{ 0.5f, 0.5f });

    void    DrawText(String text, Font font, Vector2 position);
    void    DrawFramerate(Font font, Vector2 position);
}