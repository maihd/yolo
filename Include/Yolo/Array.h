#pragma once

#include <stdlib.h>
#include <assert.h>

#include <Yolo/Types.h>
#include <Yolo/String.h>

namespace Array
{
    struct ArrayMeta
    {
        uint64 memtag;

        int    length;
        int    capacity;
    };

    constexpr uint64 MEMORY_TAG = String::ConstHash("ARRAY_MEMORY_TAG");

    inline int NextPOT(int x)
    {
        int result = x - 1;

        result = result | (result >> 1);
        result = result | (result >> 2);
        result = result | (result >> 4);
        result = result | (result >> 8);
        result = result | (result >> 16);

        return result + 1;
    }

    template <typename T>
    inline T* Empty(void)
    {
        return 0;
    }

    template <typename T>
    inline void Free(T** array)
    {
        if (array && IsArray(*array))
        {
            free((ArrayMeta*)(*array) - 1);
            array = 0;
        }
    }

    template <typename T>
    inline bool IsArray(const T* array)
    {
        if (!array)
        {
            return true;
        }

        const ArrayMeta* meta = (const ArrayMeta*)array - 1;
        return meta->memtag == MEMORY_TAG;
    }

    template <typename T>
    inline int Length(const T* array)
    {
        assert(IsArray(array));

        return array ? ((const ArrayMeta*)array - 1)->length : 0;
    }

    template <typename T>
    inline int Capacity(const T* array)
    {
        assert(IsArray(array));

        return array ? ((const ArrayMeta*)array - 1)->capacity : 0;
    }

    template <typename T>
    inline bool IsEmpty(const T* array)
    {
        assert(IsArray(array));

        return Length(array) == 0;
    }

    template <typename T>
    inline bool Resize(T** array, int capacity)
    {
        assert(array != NULL);
        assert(IsArray(*array));

        if (capacity < Capacity(*array))
        {
            return true;
        }

        int length = Length(*array);

        int oldCapacity = Capacity(*array);
        int newCapacity = capacity < 8 ? 8 : NextPOT(capacity);

        ArrayMeta* oldMeta = *array ? (ArrayMeta*)(*array) - 1 : 0;
        ArrayMeta* newMeta = (ArrayMeta*)realloc(oldMeta, newCapacity * sizeof(T));
        if (newMeta)
        {
            newMeta->memtag     = MEMORY_TAG;
            newMeta->length     = length;
            newMeta->capacity   = newCapacity;

            *array = (T*)(newMeta + 1);

            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T>
    inline bool Ensure(T** array, int capacity)
    {
        assert(array != NULL);
        assert(IsArray(*array));

        return (Capacity(*array) < capacity) ? Resize(array, capacity) : true;
    }

    template <typename T>
    inline bool Ensure(const T* array, int capacity)
    {
        assert(IsArray(array));

        return (Capacity(array) >= capacity);
    }

    template <typename T>
    inline int Push(T** array, T element)
    {
        assert(array != NULL);
        assert(IsArray(*array));
        
        if (Ensure(array, Length(*array) + 1))
        {
            ArrayMeta* meta = (ArrayMeta*)(*array) - 1;

            int index = meta->length++;
            (*array)[index] = element;

            return index;
        }

        return -1;
    }

    template <typename T>
    inline T Pop(T** array)
    {
        assert(array != NULL);
        assert(IsArray(*array));
        assert(Length(*array) > 0);

        ArrayMeta* meta = (ArrayMeta*)(*array) - 1;
        meta->length--;

        return (*array)[meta->length];
    }
}