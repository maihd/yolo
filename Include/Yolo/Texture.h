#pragma once

#include <Yolo/Types.h>

namespace TextureOps
{
    Texture New(const void* pixels, int width, int height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);

    Texture Load(String path);
    void    Free(Texture* texture);
};
