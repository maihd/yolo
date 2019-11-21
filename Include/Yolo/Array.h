#pragma once

#include <stdlib.h>
#include <stdint.h>

namespace Array
{
    struct ArrayMeta
    {
        int memtag;
        int length;
        int capacity;
    };

    template <typename T>
    inline T* New(void)
    {
        return 0;
    }

    template <typename T>
    inline void Free(T* array)
    {
        if (array)
        {
            free((uint8_t*)array - sizeof(ArrayMeta));
        }
    }
}