#pragma once

namespace Graphics
{
    bool Init(void);
    void Quit(void);

    void Clear(void);
    void ClearColor(float r, float g, float b, float a = 1.0f);
}