#pragma once

#include <Yolo/Types.h>

namespace TextureOps
{
    Texture New(const void* pixels, I32 width, I32 height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);

    Texture Load(String path);
    void    Free(Texture* texture);
};
