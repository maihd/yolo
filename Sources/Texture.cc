#include <Yolo/Texture.h>

#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Runtime.h"

Texture Texture::Load(string path)
{
    int width, height, channel;
    void* pixels = stbi_load(path, &width, &height, &channel, 0);
    if (!pixels)
    {
        return {};
    }

    GLuint handle = 0;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(pixels);

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