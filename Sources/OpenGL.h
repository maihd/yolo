#pragma once

#include <Yolo/Types.h>
#include <Yolo/Shader.h>

#include <GL/glew.h>

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

namespace OpenGL
{
    Handle  CreateVertexArray(Handle vertexBuffer = 0, Handle indexBuffer = 0);
    void    DeleteVertexArray(Handle* vertexArray);

    Handle  CreateIndexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer = 0);
    void    DeleteIndexBuffer(Handle* indexBuffer);

    Handle  CreateVertexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer = 0);
    void    DeleteVertexBuffer(Handle* vertexBuffer);
    
    void    DefineAttribute(Handle vertexArray, int location, DataType type, int stride, int offset);
    void    DefineAttribute(Handle vertexArray, Shader shader, string attrib, DataType type, int stride, int offset);

    void    AttachIndexBuffer(Handle vertexArray, Handle indexBuffer);
    void    AttachVertexBuffer(Handle vertexArray, Handle vertexBuffer);
}