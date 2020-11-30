#pragma once

#include <Yolo/Core.h>

namespace VertexArrayOps
{
    VertexArray New(void);
    void        Free(VertexArray* vertexArray);

    void        SetIndexData(VertexArray vertexArray, const void* data, I32 size, BufferUsage usage);
    void        SetVertexData(VertexArray vertexArray, const void* data, I32 size, BufferUsage usage);

    void        DefineAttribute(VertexArray vertexArray, I32 location, DataType type, I32 stride, I32 offset);
}
