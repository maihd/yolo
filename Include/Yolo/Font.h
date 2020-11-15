#pragma once

#include <Yolo/Types.h>

namespace FontOps
{
    Font Load(String path, F32 size);
    void Free(Font* font);
}
