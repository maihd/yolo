#pragma once

#include <Yolo/Core.h>

struct SpriteMesh
{
    Handle  vertexArray;
    Handle  vertexBuffer;

    static SpriteMesh   New(void);
    static void         Free(SpriteMesh* spriteMesh);
};