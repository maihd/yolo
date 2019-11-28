#pragma once

#include <Yolo/Types.h>

enum struct PixelFormat
{
    Unknown = 0,

    RGB,
    RGBA,
    Alpha,
};

struct Texture
{
    Handle      handle;
    PixelFormat format;

    int         width;
    int         height;

    static Texture New(const void* pixels, int width, int height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);

    static Texture Load(string path);
    static void    Free(Texture* texture);
};
