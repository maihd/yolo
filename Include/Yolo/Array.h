#pragma once

#include <stdlib.h>
#include <assert.h>

#include <Yolo/Types.h>

namespace ArrayOps
{
    constexpr int MIN_CAPACITY = 16;

    template <typename T>
    inline Array<T> New(int capacity = 0)
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
    inline Array<T> From(const T* buffer, int length)
    {
        if (!buffer || length <= 0)
        {
            return {};
        }

        Array<T> result = {};
        if (Resize(&result, length))
        {
            memcpy(result, buffer, sizeof(T) * length);
        }
        return result;
    }

    template <typename T>
    inline T* From(const Array<T> array)
    {
        int length = Length(array);
        if (length == 0)
        {
            return {};
        }

        Array<T> result = {};
        if (Resize(&result, length))
        {
            memcpy(result, buffer, sizeof(T) * length);
        }
        return result;
    }

    template <typename T>
    inline Array<T> Fill(int capacity, T value)
    {
        T* array = Empty();
        if (Ensure(&array, capacity))
        {
            for (int i = 0; i < capacity; i++)
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

        array->length   = 0;
        array->capacity = 0;
        array->elements = 0;
    }

    template <typename T>
    inline int SizeOf(const Array<T> array) 
    {
        return array.length * sizeof(T);
    }

    template <typename T>
    inline bool IsEmpty(const Array<T> array)
    {
        return array.length == 0;
    }

    template <typename T>
    inline bool Resize(Array<T>* array, int capacity)
    {
        assert(array);

        if (capacity <= array->capacity)
        {
            return true;
        }

        int oldCapacity = array->capacity;
        int newCapacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : NextPOT(capacity);

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
    inline bool Ensure(Array<T>* array, int capacity)
    {
        assert(array != NULL);

        return (array->capacity < capacity) ? Resize(array, capacity) : true;
    }

    template <typename T>
    inline bool Ensure(const Array<T> array, int capacity)
    {
        return (array.capacity >= capacity);
    }

    template <typename T>
    inline int Push(Array<T>* array, T element)
    {
        assert(array != NULL);
        
        if (Ensure(array, array->length + 1))
        {
            int index = array->length++;
            array->elements[index] = element;

            return index;
        }

        return -1;
    }

    template <typename T>
    inline T Pop(Array<T>* array)
    {
        assert(array != NULL);
        assert(array->length > 0);

        return array->elements[--array->length];
    }

    template <typename T>
    inline void Clear(Array<T>* array)
    {
        assert(array);
        
        array->length = 0;
    }
}