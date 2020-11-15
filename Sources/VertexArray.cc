#include <Yolo/VertexArray.h>

#include <assert.h>
#include <GL/glew.h>

namespace VertexArrayOps
{
    static inline GLenum DataTypeToGLenum(DataType type)
    {
        const GLenum enums[] = {
            0,                  // Void,

            GL_BYTE,            // I8,
            GL_SHORT,           // I16,
            GL_INT,             // I32,

            GL_UNSIGNED_BYTE,   // U8,
            GL_UNSIGNED_SHORT,  // U16,
            GL_UNSIGNED_INT,    // U32,

            GL_FLOAT,           // float,
            GL_DOUBLE,          // double,

            GL_FLOAT,           // Vector2,
            GL_FLOAT,           // Vector3,
            GL_FLOAT,           // Vector4,

            GL_FLOAT,           // Matrix4x4,
        };

        return enums[(I32)type];
    }

    static inline GLint DataTypeComponents(DataType type)
    {
        const GLint components[] = {
            0,  // Void,

            1,  // I8,
            1,  // I16,
            1,  // I32,

            1,  // U8,
            1,  // U16,
            1,  // U32,

            1,  // float,
            1,  // double,

            2,  // Vector2,
            3,  // Vector3,
            4,  // Vector4,

            16, // Matrix4x4,
        };

        return components[(I32)type];
    }

    static inline GLenum BufferUsageToGLenum(BufferUsage usage)
    {
        const GLenum enums[] = {
            0,

            GL_STATIC_DRAW,
            GL_STREAM_DRAW,
            GL_DYNAMIC_DRAW,
        };

        return enums[(I32)usage];
    }

    VertexArray New(void)
    {
        GLuint handle;
        glGenVertexArrays(1, &handle);

        GLuint buffers[2];
        glGenBuffers(2, buffers);

        glBindVertexArray(handle);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return VertexArray{
            (Handle)handle,
            (Handle)buffers[1],
            (Handle)buffers[0],
        };
    }

    void Free(VertexArray* vertexArray)
    {
        assert(vertexArray != nullptr);

        glDeleteBuffers(2, &vertexArray->indexBuffer);
        glDeleteVertexArrays(1, &vertexArray->handle);

        vertexArray->handle = 0;
        vertexArray->indexBuffer = 0;
        vertexArray->vertexBuffer = 0;
    }

    void SetIndexData(VertexArray vertexArray, const void* data, I32 size, BufferUsage usage)
    {
        glBindVertexArray(vertexArray.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray.indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, BufferUsageToGLenum(usage));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void SetVertexData(VertexArray vertexArray, const void* data, I32 size, BufferUsage usage)
    {
        glBindVertexArray(vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, vertexArray.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, BufferUsageToGLenum(usage));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void DefineAttribute(VertexArray vertexArray, I32 location, DataType type, I32 stride, I32 offset)
    {
        if (location < 0)
        {
            return;
        }

        GLenum typeEnum = DataTypeToGLenum(type);
        GLint  typeComponents = DataTypeComponents(type);

        glBindVertexArray(vertexArray.handle);
        glBindBuffer(GL_ARRAY_BUFFER, vertexArray.vertexBuffer);

        glEnableVertexAttribArray((GLuint)location);
        glVertexAttribPointer((GLuint)location, typeComponents, typeEnum, GL_FALSE, stride, (const void*)(GLintptr)offset);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
