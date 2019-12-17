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

    mat4    GetProjection(void);
    void    SetProjection(mat4 projection);

    float   GetLineWidth(void);
    void    SetLineWidth(float width);

    void    DrawCircle(DrawMode mode, vec2 position, float radius, vec4 color, int segments = 30);
    void    DrawRectangle(DrawMode mode, vec2 position, vec2 size, vec4 color);

    void    DrawTexture(Texture texture, vec2 position, float rotation = 0.0f, vec2 scale = vec2{ 1, 1 }, vec4 color = vec4{ 1, 1, 1, 1 }, vec2 pivot = vec2{ 0.5f, 0.5f });

    void    DrawText(String text, Font font, vec2 position);
    void    DrawFramerate(Font font, vec2 position);
}