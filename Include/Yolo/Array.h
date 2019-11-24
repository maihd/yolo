#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <Yolo/String.h>

namespace Array
{
    struct ArrayMeta
    {
        uint64_t memtag;

        int      length;
        int      capacity;
    };

    constexpr uint64_t MEMORY_TAG = String::ConstHash("ARRAY_MEMORY_TAG");

    template <typename T>
    inline bool IsEmpty(const T* array)
    {
        assert(IsArray(array));

        if (!array)
        {
            return true;
        }

        const ArrayMeta* meta = (const ArrayMeta*)((const uint8_t*)array - sizeof(ArrayMeta));
        return meta->length;
    }

    template <typename T>
    inline bool IsArray(const T* array)
    {
        if (!array)
        {
            return true;
        }

        const ArrayMeta* meta = (const ArrayMeta*)((const uint8_t*)array - sizeof(ArrayMeta));
        return meta->memtag == MEMORY_TAG;
    }

    template <typename T>
    inline T* New(void)
    {
        return 0;
    }

    template <typename T>
    inline void Free(T* array)
    {
        if (array && IsArray(array))
        {
            free((uint8_t*)array - sizeof(ArrayMeta));
        }
    }
}