#include <Yolo/Texture.h>

#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Runtime.h"

Texture Texture::Load(string path)
{
    int width, height, channel;
    void* data = stbi_load(path, &width, &height, &channel, 0);
    if (!data)
    {
        return {};
    }

    GLuint handle;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    Texture texture = {
        handle, 
        channel == 4 ? PixelFormat::RGBA : PixelFormat::RGB,

        width, height
    };

    return texture;
}

void Texture::Free(Texture* texture)
{
    assert(texture);

    glDeleteTextures(1, &texture->handle);
    texture->width  = 0;
    texture->height = 0;
    texture->handle = 0;
}