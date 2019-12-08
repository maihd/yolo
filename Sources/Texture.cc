#include <Yolo/Texture.h>

#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Runtime.h"

namespace TextureOps
{
    static GLenum PixelFormatToGLenum(PixelFormat value)
    {
        const GLenum enums[] = { GL_NONE, GL_RGB, GL_RGBA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };

        return enums[(int)value];
    }

    Texture New(const void* pixels, int width, int height, PixelFormat pixelsFormat, PixelFormat textureFormat)
    {
        GLuint handle = 0;
        glGenTextures(1, &handle);

        GLenum internalFormat = PixelFormatToGLenum(textureFormat);
        GLenum format = PixelFormatToGLenum(pixelsFormat);

        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return {
            handle,
            textureFormat,

            width, height
        };
    }

    Texture Load(string path)
    {
        int width, height, channel;
        void* pixels = stbi_load(path, &width, &height, &channel, 0);
        if (!pixels)
        {
            return {};
        }

        PixelFormat pixelsFormat = channel == 4 ? PixelFormat::RGBA : PixelFormat::RGB;
        PixelFormat textureFormat = PixelFormat::RGBA;

        Texture texture = New(pixels, width, height, pixelsFormat, textureFormat);

        stbi_image_free(pixels);

        return texture;
    }

    void Free(Texture* texture)
    {
        assert(texture);

        glDeleteTextures(1, &texture->handle);

        texture->width = 0;
        texture->height = 0;
        texture->handle = 0;
    }
}
