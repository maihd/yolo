#pragma once

#include <Yolo/Font.h>
#include <Yolo/Texture.h>
#include <Yolo/MathTypes.h>

enum struct DrawMode
{
    Fill = 0,
    Line = 1,
};

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

    float   GetLineWidth(void);
    void    SetLineWidth(float width);

    void    DrawCircle(DrawMode mode, vec2 position, float radius, vec4 color, int segments = 30);
    void    DrawRectangle(DrawMode mode, vec2 position, vec2 size, vec4 color);

    void    DrawTexture(Texture texture, vec2 position, vec2 size);

    void    DrawText(string text, Font font, vec2 position);
    void    DrawFramerate(Font font, vec2 position);
}