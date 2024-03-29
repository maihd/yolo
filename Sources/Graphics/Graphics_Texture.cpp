
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GL/glew.h>

#include <System/FileSystem.h>
#include <Text/String.h>
#include <Graphics/Graphics.h>
#include <Container/HashTable.h>

static HashTable<Texture> LoadedTextures = MakeHashTable<Texture>(64);

static GLenum PixelFormatToGLenum(PixelFormat value)
{
    const GLenum enums[] = { GL_NONE, GL_RGB, GL_RGBA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };

    return enums[(I32)value];
}

Texture MakeTexture(const void* pixels, I32 width, I32 height, PixelFormat pixelsFormat, PixelFormat textureFormat)
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

        (float)width, (float)height
    };
}

Texture LoadTexture(StringView path)
{
    String fullPath = GetFullPath(path);
    if (IsStringEmpty(fullPath))
    {
        return {};
    }

    U64 textureHash = CalcHash64(fullPath.Buffer, fullPath.Length);

        Texture cachedTexture;
        if (HashTableTryGetValue(LoadedTextures, textureHash, &cachedTexture))
        {
            return cachedTexture;
        }

    I32 width, height, channel;
    void* pixels = stbi_load(fullPath.Buffer, &width, &height, &channel, 0);
    if (!pixels)
    {
        return {};
    }

    PixelFormat pixelsFormat = channel == 4 ? PixelFormat::RGBA : PixelFormat::RGB;
    PixelFormat textureFormat = PixelFormat::RGBA;

    Texture texture = MakeTexture(pixels, width, height, pixelsFormat, textureFormat);

    stbi_image_free(pixels);

    HashTableSetValue(&LoadedTextures, textureHash, texture);
    return texture;
}

void FreeTexture(Texture* texture)
{
    assert(texture);

    glDeleteTextures(1, &texture->Handle);

    texture->Width = 0;
    texture->Height = 0;
    texture->Handle = 0;
}
