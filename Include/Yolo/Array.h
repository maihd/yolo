#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Yolo/Types.h>
#include <Yolo/Memory.h>

// --------------------------------------
// API
// --------------------------------------

constexpr int ARRAY_MIN_CAPACITY = 16;

template <typename T>
Array<T> ArrayNew(int capacity = 0);

template <typename T>
Array<T> ArrayNew(const T* buffer, int count);

template <typename T>
T* ArrayNew(const Array<T> array);

template <typename T>
Array<T> ArrayFill(int capacity, T value);

template <typename T>
inline void ArrayFree(Array<T>* array);

template <typename T>
int ArraySizeInBytes(const Array<T> array);

template <typename T>
bool ArrayIsEmpty(const Array<T> array);

template <typename T>
bool ArrayResize(Array<T>* array, int capacity);

template <typename T>
bool ArrayEnsure(Array<T>* array, int capacity);

template <typename T>
bool ArrayEnsure(const Array<T> array, int capacity);

template <typename T>
int ArrayPush(Array<T>* array, T element);

template <typename T>
T ArrayPop(Array<T>* array);

template <typename T>
void ArrayClear(Array<T>* array);

template <typename T>
int ArrayIndexOf(Array<T> array, T value);

template <typename T>
int ArrayLastIndexOf(Array<T> array, T value);

template <typename T>
bool ArrayErase(Array<T>* array, int index);

template <typename T>
bool ArrayErase(Array<T>* array, int start, int end);

template <typename T>
bool ArrayUnorderedErase(Array<T>* array, int index);

template <typename T>
bool ArrayRemove(Array<T>* array, T value);

template <typename T>
bool ArrayRemoveLast(Array<T>* array, T value);

template <typename T>
bool ArrayUnorderedRemove(Array<T>* array, T value);

template <typename T>
bool ArrayUnorderedRemoveLast(Array<T>* array, T value);

// --------------------------------------
// Function definitions
// --------------------------------------

template <typename T>
inline Array<T> ArrayNew(int capacity)
{
    if (capacity <= 0)
    {
        return {};
    }

    Array<T> result = {};
    ArrayEnsure(&result, capacity);
    return result;
}

template <typename T>
inline Array<T> ArrayNew(const T* buffer, int count)
{
    if (!buffer || count <= 0)
    {
        return {};
    }

    Array<T> result = {};
    if (ArrayResize(&result, count))
    {
        MemoryCopy(result, buffer, sizeof(T) * count);
    }
    return result;
}

template <typename T>
inline T* ArrayNew(const Array<T> array)
{
    if (array.count == 0)
    {
        return {};
    }

    Array<T> result = {};
    if (ArrayResize(&result, array.count))
    {
        MemoryCopy(result, buffer, sizeof(T) * length);
    }
    return result;
}

template <typename T>
inline Array<T> ArrayFill(int capacity, T value)
{
    T* array = Empty();
    if (ArrayEnsure(&array, capacity))
    {
        for (int i = 0; i < capacity; i++)
        {
            ArrayPush(&array, value);
        }
    }

    return array;
}

template <typename T>
inline void ArrayFree(Array<T>* array)
{
    assert(array);

    MemoryFree(array->elements);

    array->count = 0;
    array->capacity = 0;
    array->elements = 0;
}

template <typename T>
inline int ArraySizeInBytes(const Array<T> array)
{
    return array.count * sizeof(T);
}

template <typename T>
inline bool ArrayIsEmpty(const Array<T> array)
{
    return array.count == 0;
}

template <typename T>
inline bool ArrayResize(Array<T>* array, int capacity)
{
    assert(array);

    if (capacity <= array->capacity)
    {
        return true;
    }

    int oldCapacity = array->capacity;
    int newCapacity = capacity < ARRAY_MIN_CAPACITY ? ARRAY_MIN_CAPACITY : NextPOT(capacity);

    T* elements = (T*)MemoryRealloc(array->elements, newCapacity * sizeof(T));
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
inline bool ArrayEnsure(Array<T>* array, int capacity)
{
    assert(array != NULL);

    return (array->capacity < capacity) ? ArrayResize(array, capacity) : true;
}

template <typename T>
inline bool ArrayEnsure(const Array<T> array, int capacity)
{
    return (array.capacity >= capacity);
}

template <typename T>
inline int ArrayPush(Array<T>* array, T element)
{
    assert(array != NULL);

    if (ArrayEnsure(array, array->count + 1))
    {
        int index = array->count++;
        array->elements[index] = element;

        return index;
    }

    return -1;
}

template <typename T>
inline T ArrayPop(Array<T>* array)
{
    assert(array != NULL);
    assert(array->count > 0);

    return array->elements[--array->count];
}

template <typename T>
inline void ArrayClear(Array<T>* array)
{
    assert(array);

    array->count = 0;
}

template <typename T>
inline int ArrayIndexOf(Array<T> array, T value)
{
    for (int i = 0, n = array.count; i < n; i++)
    {
        if (array.elements[i] == value)
        {
            return i;
        }
    }

    return -1;
}

template <typename T>
inline int ArrayLastIndexOf(Array<T> array, T value)
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
inline bool ArrayErase(Array<T>* array, int index)
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
            MemoryCopy(&array->elements[index], &array->elements[index + 1], (array->count - index - 1) * sizeof(T));
        }

        return true;
    }
}

template <typename T>
inline bool ArrayErase(Array<T>* array, int start, int end)
{
    start = start > -1 ? start : 0;
    end = end > array->count ? array->count : end;

    int eraseCount = (end - start);
    if (eraseCount <= 0)
    {
        return false;
    }
    else
    {
        if (array->count - end > 0)
        {
            MemoryCopy(&array->elements[start], &array->elements[end - 1], (array->count - end) * sizeof(T));
        }
        array->count = array->count - eraseCount;

        return true;
    }
}

template <typename T>
inline bool ArrayUnorderedErase(Array<T>* array, int index)
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
inline bool ArrayRemove(Array<T>* array, T value)
{
    return ArrayErase(array, ArrayIndexOf(*array, value));
}

template <typename T>
inline bool ArrayRemoveLast(Array<T>* array, T value)
{
    return ArrayErase(array, ArrayLastIndexOf(*array, value));
}

template <typename T>
inline bool ArrayUnorderedRemove(Array<T>* array, T value)
{
    return ArrayUnorderedErase(array, ArrayIndexOf(*array, value));
}

template <typename T>
inline bool ArrayUnorderedRemoveLast(Array<T>* array, T value)
{
    return ArrayUnorderedErase(array, ArrayLastIndexOf(*array, value));
}
