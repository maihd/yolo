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

    template <typename T>
    inline int IndexOf(Array<T> array, T value)
    {
        for (int i = 0, n = array.length; i < n; i++)
        {
            if (array.elements[i] == value)
            {
                return i;
            }
        }

        return -1;
    }

    template <typename T>
    inline int LastIndexOf(Array<T> array, T value)
    {
        int index = -1;
        for (int i = 0, n = array.length; i < n; i++)
        {
            if (elements[i] == value)
            {
                index = i;
            }
        }

        return index;
    }

    template <typename T>
    inline bool Erase(Array<T>* array, int index)
    {
        if (index < 0 || index >= array->length)
        {
            return false;
        }
        else
        {
            array->length--;
            if (index < array->length)
            {
                ::memcpy(&array->elements[index], &array->elements[index + 1], (array->length - index - 1) * sizeof(T));
            }

            return true;
        }
    }

    template <typename T>
    inline bool Erase(Array<T>* array, int start, int end)
    {
        start   = start > -1 ? start : 0;
        end     = end > array->length ? array->length : end;

        int eraseCount = (end - start);
        if (eraseCount <= 0)
        {
            return false;
        }
        else
        {
            if (array->length - end > 0)
            {
                ::memcpy(&array->elements[start], &array->elements[end - 1], (array->length - end) * sizeof(T));
            }
            array->length = array->length - eraseCount;

            return true;
        }
    }

    template <typename T>
    inline bool UnorderedErase(Array<T>* array, int index)
    {
        if (index < 0 || index >= array->length)
        {
            return false;
        }
        else
        {
            array->length--;
            if (index < array->length)
            {
                array->elements[index] = array->elements[array->length];
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