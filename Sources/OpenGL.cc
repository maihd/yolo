#include "./OpenGL.h"

#include <assert.h>

namespace OpenGL
{
    GLenum DataTypeToGLenum(DataType type)
    {
        const GLenum enums[] = {
            0,                  // Void,

            GL_BYTE,            // Int8,
            GL_SHORT,           // Int16,
            GL_INT,             // Int32,

            GL_UNSIGNED_BYTE,   // Uint8,
            GL_UNSIGNED_SHORT,  // Uint16,
            GL_UNSIGNED_INT,    // Uint32,

            GL_FLOAT,           // Float32,
            GL_DOUBLE,          // Float64,

            GL_FLOAT,           // Vector2,
            GL_FLOAT,           // Vector3,
            GL_FLOAT,           // Vector4,

            GL_FLOAT,           // Matrix4x4,
        };

        return enums[(int)type];
    }

    GLint DataTypeComponents(DataType type) 
    {
        const GLint components[] = {
            0,  // Void,

            1,  // Int8,
            1,  // Int16,
            1,  // Int32,

            1,  // Uint8,
            1,  // Uint16,
            1,  // Uint32,

            1,  // Float32,
            1,  // Float64,

            2,  // Vector2,
            3,  // Vector3,
            4,  // Vector4,

            16, // Matrix4x4,
        };

        return components[(int)type];
    }

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

    void DefineAttribute(Handle vertexArray, int location, DataType type, int stride, int offset)
    {
        GLenum typeEnum         = DataTypeToGLenum(type);
        GLint  typeComponents   = DataTypeComponents(type);

        glBindVertexArray(vertexArray);
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, typeComponents, typeEnum, GL_FALSE, stride, (const void*)(GLintptr)offset);
        glBindVertexArray(0);
    }

    void DefineAttribute(Handle vertexArray, Shader shader, string attrib, DataType type, int stride, int offset)
    {
        int location = glGetAttribLocation(shader.handle, attrib);
        DefineAttribute(vertexArray, location, type, stride, offset);
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