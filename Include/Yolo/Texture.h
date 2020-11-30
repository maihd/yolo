#pragma once

#include <Yolo/Core.h>

Texture MakeTexture(const void* pixels, I32 width, I32 height, PixelFormat pixelsFormat = PixelFormat::RGBA, PixelFormat textureFormat = PixelFormat::RGBA);
Texture LoadTexture(const char* path);
void    FreeTexture(Texture* texture);
