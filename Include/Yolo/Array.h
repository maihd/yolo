#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Yolo/Types.h>

namespace ArrayOps
{
    constexpr I32 MIN_CAPACITY = 16;

    template <typename T>
    inline Array<T> New(I32 capacity = 0)
    {
        if (capacity <= 0)
        {
            return {};
        }

        Array<T> result = {};
        Ensure(&result, capacity);
        return result;
    }

    template <typename T>
    inline Array<T> From(const T* buffer, I32 count)
    {
        if (!buffer || count <= 0)
        {
            return {};
        }

        Array<T> result = {};
        if (Resize(&result, count))
        {
            memcpy(result, buffer, sizeof(T) * count);
        }
        return result;
    }

    template <typename T>
    inline T* From(const Array<T> array)
    {
        if (array.count == 0)
        {
            return {};
        }

        Array<T> result = {};
        if (Resize(&result, array.count))
        {
            memcpy(result, buffer, sizeof(T) * length);
        }
        return result;
    }

    template <typename T>
    inline Array<T> Fill(I32 capacity, T value)
    {
        T* array = Empty();
        if (Ensure(&array, capacity))
        {
            for (I32 i = 0; i < capacity; i++)
            {
                Push(&array, value);
            }
        }

        return array;
    }

    template <typename T>
    inline void Free(Array<T>* array)
    {
        assert(array);

        free(array->elements);

        array->count    = 0;
        array->capacity = 0;
        array->elements = 0;
    }

    template <typename T>
    inline I32 SizeOf(const Array<T> array)
    {
        return array.count * sizeof(T);
    }

    template <typename T>
    inline bool IsEmpty(const Array<T> array)
    {
        return array.count == 0;
    }

    template <typename T>
    inline bool Resize(Array<T>* array, I32 capacity)
    {
        assert(array);

        if (capacity <= array->capacity)
        {
            return true;
        }

        I32 oldCapacity = array->capacity;
        I32 newCapacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : NextPOT(capacity);

        T* elements = (T*)realloc(array->elements, newCapacity * sizeof(T));
        if (elements)
        {
            array->capacity = newCapacity;
            array->elements = elements;

            return true;
        }
        else
        {
            return false;
        }
    }

    template <typename T>
    inline bool Ensure(Array<T>* array, I32 capacity)
    {
        assert(array != NULL);

        return (array->capacity < capacity) ? Resize(array, capacity) : true;
    }

    template <typename T>
    inline bool Ensure(const Array<T> array, I32 capacity)
    {
        return (array.capacity >= capacity);
    }

    template <typename T>
    inline I32 Push(Array<T>* array, T element)
    {
        assert(array != NULL);

        if (Ensure(array, array->count + 1))
        {
            I32 index = array->count++;
            array->elements[index] = element;

            return index;
        }

        return -1;
    }

    template <typename T>
    inline T Pop(Array<T>* array)
    {
        assert(array != NULL);
        assert(array->count > 0);

        return array->elements[--array->count];
    }

    template <typename T>
    inline void Clear(Array<T>* array)
    {
        assert(array);

        array->count = 0;
    }

    template <typename T>
    inline I32 IndexOf(Array<T> array, T value)
    {
        for (I32 i = 0, n = array.count; i < n; i++)
        {
            if (array.elements[i] == value)
            {
                return i;
            }
        }

        return -1;
    }

    template <typename T>
    inline I32 LastIndexOf(Array<T> array, T value)
    {
        I32 index = -1;
        for (I32 i = 0, n = array.length; i < n; i++)
        {
            if (elements[i] == value)
            {
                index = i;
            }
        }

        return index;
    }

    template <typename T>
    inline bool Erase(Array<T>* array, I32 index)
    {
        if (index < 0 || index >= array->count)
        {
            return false;
        }
        else
        {
            array->count--;
            if (index < array->count)
            {
                memcpy(&array->elements[index], &array->elements[index + 1], (array->count - index - 1) * sizeof(T));
            }

            return true;
        }
    }

    template <typename T>
    inline bool Erase(Array<T>* array, I32 start, I32 end)
    {
        start = start > -1 ? start : 0;
        end = end > array->count ? array->count : end;

        I32 eraseCount = (end - start);
        if (eraseCount <= 0)
        {
            return false;
        }
        else
        {
            if (array->count - end > 0)
            {
                memcpy(&array->elements[start], &array->elements[end - 1], (array->count - end) * sizeof(T));
            }
            array->count = array->count - eraseCount;

            return true;
        }
    }

    template <typename T>
    inline bool UnorderedErase(Array<T>* array, I32 index)
    {
        if (index < 0 || index >= array->count)
        {
            return false;
        }
        else
        {
            array->count--;
            if (index < array->count)
            {
                array->elements[index] = array->elements[array->count];
            }

            return true;
        }
    }

    template <typename T>
    inline bool Remove(Array<T>* array, T value)
    {
        return Erase(array, IndexOf(*array, value));
    }

    template <typename T>
    inline bool RemoveLast(Array<T>* array, T value)
    {
        return Erase(array, LastIndexOf(*array, value));
    }

    template <typename T>
    inline bool UnorderedRemove(Array<T>* array, T value)
    {
        return UnorderedErase(array, IndexOf(*array, value));
    }

    template <typename T>
    inline bool UnorderedRemoveLast(Array<T>* array, T value)
    {
        return UnorderedErase(array, LastIndexOf(*array, value));
    }
}