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

    static VertexArray New(void);
    static void        Free(VertexArray* vertexArray);

    static void        SetIndexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);
    static void        SetVertexData(VertexArray vertexArray, const void* data, int size, BufferUsage usage);

    static void        DefineAttribute(VertexArray vertexArray, int location, DataType type, int stride, int offset);
};