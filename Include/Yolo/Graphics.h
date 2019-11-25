#pragma once

#include <Yolo/Types.h>

struct Shader
{
    Handle  handle;
    int*    uniformLocations;

    Shader LoadShader();
    Shader CompileShader();
};

struct Texture
{
    Handle  handle;
    int     width;
    int     height;
};

namespace Graphics
{
    bool Init(void);
    void Quit(void);

    void Clear(void);
    void ClearColor(float r, float g, float b, float a = 1.0f);

    void Present(void);

    bool IsVSync(void);
    void SetVSync(bool enable);
}