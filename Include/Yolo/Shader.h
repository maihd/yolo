#pragma once

#include <Yolo/Types.h>

struct Shader
{
    Handle  handle;

    static Shader Load(String vertexFile, String pixelsFile);
    static Shader Compile(String vertexSource, String pixelsSource);
};

