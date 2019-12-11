#pragma once

#include <Yolo/Types.h>

namespace FontOps
{
    Font Load(String path, float size);
    void Free(Font* font);
}
