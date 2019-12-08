#pragma once

#include <Yolo/Types.h>

struct Shader
{
    Handle  handle;
};

namespace ShaderOps
{
    Shader Load(String vertexFile, String pixelsFile);
    Shader Compile(String vertexSource, String pixelsSource);
}

