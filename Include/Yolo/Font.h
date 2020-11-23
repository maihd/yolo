#pragma once

#include <Yolo/Types.h>

namespace FontOps
{
    Font Load(const char* path, float fontSize);
    void Free(Font* font);
}
