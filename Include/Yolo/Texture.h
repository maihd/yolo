#pragma once

#include <Yolo/Types.h>

enum struct PixelFormat
{
    Unknown = 0,

    RGB,
    RGBA,

    Red,
    Green,
    Blue,
    Alpha,
};

struct Texture
{
    Handle      handle;
    PixelFormat format;

    int         width;
    int         height;
};

namespace TextureOps
{
    Texture New(const void* pixels, int width, int height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);

    Texture Load(String path);
    void    Free(Texture* texture);
};
