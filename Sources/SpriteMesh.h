#pragma once

#include <Yolo/Types.h>

struct SpriteMesh
{
    Handle  vertexArray;
    Handle  vertexBuffer;

    static SpriteMesh   New(void);
    static void         Free(SpriteMesh* spriteMesh);
};