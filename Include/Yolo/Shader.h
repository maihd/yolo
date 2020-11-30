#pragma once

#include <Yolo/Core.h>

namespace ShaderOps
{
    Shader Load(const char* vertexFile, const char* pixelsFile);
    Shader Compile(const char* vertexSource, const char* pixelsSource);
}

