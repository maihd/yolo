#pragma once

#include <Yolo/MathTypes.h>

namespace Graphics
{
    bool Init(void);
    void Quit(void);

    void Clear(void);
    void ClearColor(float r, float g, float b, float a = 1.0f);

    void Present(void);

    bool IsVSync(void);
    void SetVSync(bool enable);

    bool IsWireframe(void);
    void SetWireframe(bool enable);

    void DrawCircle(vec2 position, float radius, vec4 color, int segments = 30);
    void DrawRectangle(vec2 position, vec2 size, vec4 color);
}