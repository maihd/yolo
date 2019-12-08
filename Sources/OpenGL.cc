#include "./OpenGL.h"

#include <assert.h>

namespace OpenGL
{

    Handle CreateVertexArray(Handle vertexBuffer, Handle indexBuffer)
    {
        GLuint vertexArray;
        glGenVertexArrays(1, &vertexArray);

        AttachIndexBuffer(vertexArray, indexBuffer);
        AttachVertexBuffer(vertexArray, vertexBuffer);

        return (Handle)vertexArray;
    }

    void DeleteVertexArray(Handle* vertexArray)
    {
        assert(vertexArray != nullptr);

        if (*vertexArray)
        {
            glDeleteVertexArrays(1, vertexArray);
            *vertexArray = 0;
        }
    }

    Handle CreateIndexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer)
    {
        GLuint indexBuffer = (GLuint)oldBuffer;
        if (!indexBuffer)
        {
            glGenBuffers(1, &indexBuffer);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return indexBuffer;
    }

    void DeleteIndexBuffer(Handle* indexBuffer)
    {
        assert(indexBuffer != nullptr);

        if (*indexBuffer)
        {
            glDeleteBuffers(1, indexBuffer);
            *indexBuffer = 0;
        }
    }

    Handle CreateVertexBuffer(const void* data, int size, GLenum usage, Handle oldBuffer)
    {
        GLuint vertexBuffer = (GLuint)oldBuffer;
        if (!vertexBuffer)
        {
            glGenBuffers(1, &vertexBuffer);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, usage);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vertexBuffer;
    }

    void DeleteVertexBuffer(Handle* vertexBuffer)
    {
        assert(vertexBuffer != nullptr);

        if (*vertexBuffer)
        {
            glDeleteBuffers(1, vertexBuffer);
            *vertexBuffer = 0;
        }
    }

    void AttachIndexBuffer(Handle vertexArray, Handle indexBuffer)
    {
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBindVertexArray(0);
    }

    void AttachVertexBuffer(Handle vertexArray, Handle vertexBuffer)
    {
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindVertexArray(0);
    }
}