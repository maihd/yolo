#pragma once

#include <Yolo/Types.h>

namespace VertexArrayOps
{
    VertexArray New(void);
    void        Free(VertexArray* vertexArray);

    void        SetIndexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);
    void        SetVertexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);

    void        DefineAttribute(VertexArray vertexArray, int location, DataType type, int stride, int offset);
}
