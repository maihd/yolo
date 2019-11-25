#pragma once

#include <Yolo/Types.h>

struct Shader
{
    Handle  handle;

    static Shader Load(string vertexFile, string pixelsFile);
    static Shader Compile(string vertexSource, string pixelsSource);
};

