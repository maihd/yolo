#pragma once

#include <Yolo/Types.h>

enum struct DataType
{
    Void,

    Int8,
    Int16,
    Int32,

    Uint8,
    Uint16,
    Uint32,

    Float32,
    Float64,

    Vector2,
    Vector3,
    Vector4,

    Matrix4x4,
};

enum struct BufferUsage
{
    None,

    StaticDraw,
    StreamDraw,
    DynamicDraw,
};

struct VertexArray
{
    Handle handle;
    Handle indexBuffer;
    Handle vertexBuffer;
};

namespace VertexArrayOps
{
    VertexArray New(void);
    void        Free(VertexArray* vertexArray);

    void        SetIndexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);
    void        SetVertexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);

    void        DefineAttribute(VertexArray vertexArray, int location, DataType type, int stride, int offset);
}
