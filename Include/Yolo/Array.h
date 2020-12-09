#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Yolo/Core.h>
#include <Yolo/Memory.h>

// --------------------------------------
// API
// --------------------------------------

constexpr int ARRAY_MIN_CAPACITY = 16;

template <typename T>
Array<T> MakeArray(int capacity = 0);

template <typename T>
Array<T> MakeArray(const T* buffer, int count);

template <typename T>
T* MakeArray(const Array<T> array);

template <typename T>
Array<T> MakeArray(int capacity, T value);

template <typename T>
void FreeArray(Array<T>* array);

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
inline Array<T> MakeArray(int capacity)
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
inline Array<T> MakeArray(const T* buffer, int count)
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
inline T* MakeArray(const Array<T> array)
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
inline Array<T> MakeArray(int capacity, T value)
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
inline void FreeArray(Array<T>* array)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    MemoryFree(array->Items);

    array->Items    = nullptr;
    array->Count    = 0;
    array->Capacity = 0;
}

template <typename T>
inline int ArraySizeInBytes(const Array<T> array)
{
    return array.Count * sizeof(T);
}

template <typename T>
inline bool ArrayIsEmpty(const Array<T> array)
{
    return array.Count == 0;
}

template <typename T>
inline bool ArrayResize(Array<T>* array, int capacity)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    if (capacity <= array->Capacity)
    {
        return true;
    }

    int oldCapacity = array->Capacity;
    int newCapacity = capacity < ARRAY_MIN_CAPACITY ? ARRAY_MIN_CAPACITY : NextPOTwosI32(capacity);

    T* items = (T*)MemoryRealloc(array->Items, newCapacity * sizeof(T));
    if (items)
    {
        array->Items    = items;
        array->Capacity = newCapacity;

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
    DebugAssert(array != nullptr, "The input array is nullptr");

    return (array->Capacity < capacity) ? ArrayResize(array, capacity) : true;
}

template <typename T>
inline bool ArrayEnsure(const Array<T> array, int capacity)
{
    return (array.Capacity >= capacity);
}

template <typename T>
inline int ArrayPush(Array<T>* array, T item)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    if (ArrayEnsure(array, array->Count + 1))
    {
        int index = array->Count;
        array->Items[index] = item;
        array->Count++;
        return index;
    }

    return -1;
}

template <typename T>
inline T ArrayPop(Array<T>* array)
{
    DebugAssert(array != nullptr, "The input array is nullptr");
    DebugAssert(array->Count > 0, "Attempt to pop last item from empty array");

    return array->Items[--array->Count];
}

template <typename T>
inline void ArrayClear(Array<T>* array)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    array->Count = 0;
}

template <typename T>
inline int ArrayIndexOf(Array<T> array, T value)
{
    for (int i = 0, n = array.Count; i < n; i++)
    {
        if (array.Items[i] == value)
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
    for (int i = 0, n = array.Count; i < n; i++)
    {
        if (array.Items[i] == value)
        {
            index = i;
        }
    }

    return index;
}

template <typename T>
inline bool ArrayErase(Array<T>* array, int index)
{
    if (index < 0 || index >= array->Count)
    {
        return false;
    }
    else
    {
        if (index < array->Count - 1)
        {
            int rangeCount = array->Count - index - 1;
            int moveCount = rangeCount;

            memmove(&array->Items[index], &array->Items[index + 1], moveCount * sizeof(T));
        }
        array->Count--;
        return true;
    }
}

template <typename T>
inline bool ArrayErase(Array<T>* array, int start, int end)
{
    start = start > -1 ? start : 0;
    end = (end > array->Count) ? array->Count : end;

    int eraseCount = (end - start);
    if (eraseCount <= 0)
    {
        return false;
    }
    else
    {
        if ((array->Count - end) > 0)
        {
            memcpy(&array->Items[start], &array->Items[end - 1], (array->Count - end) * sizeof(T));
        }
        array->Count = array->Count - eraseCount;
        return true;
    }
}

template <typename T>
inline bool ArrayUnorderedErase(Array<T>* array, int index)
{
    if (index < 0 || index >= array->Count)
    {
        return false;
    }
    else
    {
        int lastIndex = array->Count - 1;
        if (index < lastIndex)
        {
            array->Items[index] = array->Items[lastIndex];
        }
        array->Count--;
        return true;
    }
}

template <typename T>
inline int ArrayRemove(Array<T>* array, T value)
{
    int index = ArrayIndexOf(*array, value);
    ArrayErase(array, index);
    return index;
}

template <typename T>
inline int ArrayRemoveLast(Array<T>* array, T value)
{
    int index = ArrayLastIndexOf(*array, value);
    ArrayErase(array, index);
    return index;
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
