#pragma once

#include <stdlib.h>

struct CrtMalloc
{
    inline void* Alloc(int size)
    {
        return malloc((size_t)size);
    }

    inline void* Realloc(void* ptr, int size)
    {
        return realloc(ptr, (size_t)size);
    }

    inline void Free(void* ptr)
    {
        free(ptr);
    }
};
