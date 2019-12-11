#pragma once

#include <Yolo/Types.h>

namespace ShaderOps
{
    Shader Load(String vertexFile, String pixelsFile);
    Shader Compile(String vertexSource, String pixelsSource);
}

