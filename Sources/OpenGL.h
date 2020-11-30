#pragma once

#include <Yolo/Core.h>
#include <Yolo/Shader.h>

#include <GL/glew.h>

namespace OpenGL
{
    Handle  CreateVertexArray(Handle vertexBuffer = 0, Handle indexBuffer = 0);
    void    DeleteVertexArray(Handle* vertexArray);

    Handle  CreateIndexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer = 0);
    void    DeleteIndexBuffer(Handle* indexBuffer);

    Handle  CreateVertexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer = 0);
    void    DeleteVertexBuffer(Handle* vertexBuffer);

    void    AttachIndexBuffer(Handle vertexArray, Handle indexBuffer);
    void    AttachVertexBuffer(Handle vertexArray, Handle vertexBuffer);
}