#pragma once

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
}