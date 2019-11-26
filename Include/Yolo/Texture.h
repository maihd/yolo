#pragma once

#include <Yolo/Types.h>

enum struct PixelFormat
{
    Unknown = 0,

    RGB,
    RGBA,
};

struct Texture
{
    Handle      handle;
    PixelFormat format;

    int         width;
    int         height;

    static Texture Load(string path);
    static void    Free(Texture* texture);
};
