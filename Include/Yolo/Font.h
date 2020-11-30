#pragma once

#include <Yolo/Types.h>

Font LoadFont(const char* path, float fontSize);
void FreeFont(Font* font);
