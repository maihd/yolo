#pragma once

#include <Yolo/Types.h>

Texture NewTexture(const void* pixels, I32 width, I32 height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);
Texture LoadTexture(const char* path);
void    FreeTexture(Texture* texture);
