#pragma once

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <System/Core.h>
#include <System/Memory.h>

// ----------------------------------------------------------------------------
// API
// ----------------------------------------------------------------------------

constexpr int ARRAY_MIN_CAPACITY = 16;

template <typename T>
Array<T>    MakeArray(const I32 capacity = 0);

template <typename T>
Array<T>    MakeArray(const T* items, const I32 count);

template <typename T>
T*          MakeArray(const Array<T> array);

template <typename T>
Array<T>    MakeArray(int capacity, T value);

template <typename T>
void        FreeArray(Array<T>* array);

template <typename T>
int         ArraySizeInBytes(const Array<T> array);

template <typename T>
bool        ArrayIsEmpty(const Array<T> array);

template <typename T>
const T&    ArrayFirst(const Array<T>& array);

template <typename T>
const T&    ArrayLast(const Array<T>& array);

template <typename T>
bool        ArrayResize(Array<T>* array, int capacity);

template <typename T>
bool        ArrayEnsure(Array<T>* array, int capacity);

template <typename T>
bool        ArrayEnsure(const Array<T> array, int capacity);

#ifndef NDEBUG
#define     ArrayPush(array, ...) ArrayPushDebug(array, (__VA_ARGS__), __FUNCTION__, __FILE__, __LINE__)
template <typename T>
int         ArrayPushDebug(Array<T>* array, T element, const char* func, const char* file, int line);
#else
template <typename T>
int         ArrayPush(Array<T>* array, T element);
#endif // !NDEBUG

template <typename T>
T           ArrayPop(Array<T>* array);

template <typename T>
void        ArrayClear(Array<T>* array);

template <typename T>
int         ArrayIndexOf(Array<T> array, T value);

template <typename T>
int         ArrayLastIndexOf(Array<T> array, T value);

template <typename T>
bool        ArrayErase(Array<T>* array, int index);

template <typename T>
bool        ArrayErase(Array<T>* array, int start, int end);

template <typename T>
bool        ArrayEraseFast(Array<T>* array, int index);

template <typename T>
I32         ArrayRemove(Array<T>* array, T value);

template <typename T>
I32         ArrayRemoveLast(Array<T>* array, T value);

template <typename T>
I32         ArrayRemoveFast(Array<T>* array, T value);

template <typename T>
I32         ArrayRemoveLastFast(Array<T>* array, T value);

// ----------------------------------------------------------------------------
// Function definitions
// ----------------------------------------------------------------------------

template <typename T>
inline Array<T> MakeArray(const I32 capacity)
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
inline Array<T> MakeArray(const T* items, const I32 count)
{
    if (!buffer || count <= 0)
    {
        return {};
    }

    Array<T> result = {};
    if (ArrayResize(&result, count))
    {
        MemoryCopy(result, items, sizeof(T) * count);
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
const T& ArrayFirst(const Array<T>& array)
{
    DebugAssert(!ArrayIsEmpty(array), "array is empty");
    return array.Items[0];
}

template <typename T>
const T& ArrayLast(const Array<T>& array)
{
    DebugAssert(!ArrayIsEmpty(array), "array is empty");
    return array.Items[array.Count - 1];
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
inline bool ArrayResizeDebug(Array<T>* array, I32 capacity, const char* func, const char* file, int line)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    if (capacity <= array->Capacity)
    {
        return true;
    }

    int oldCapacity = array->Capacity;
    int newCapacity = capacity < ARRAY_MIN_CAPACITY ? ARRAY_MIN_CAPACITY : NextPOTwosI32(capacity);

    T* newItems = (T*)MemoryAllocDebug(newCapacity * sizeof(T), func, file, line);
    if (newItems)
    {
        if (array->Items)
        {
            MemoryCopy(newItems, array->Items, array->Count * sizeof(T));
            MemoryFree(array->Items);
        }

        array->Items    = newItems;
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
inline bool ArrayEnsureDebug(Array<T>* array, int capacity, const char* func, const char* file, int line)
{
    DebugAssert(array != nullptr, "The input array is nullptr");

    return (array->Capacity < capacity) ? ArrayResizeDebug(array, capacity, func, file, line) : true;
}

#ifndef NDEBUG
template <typename T>
inline int ArrayPushDebug(Array<T>* array, T item, const char* func, const char* file, int line)
#else
template <typename T>
inline int ArrayPush(Array<T>* array, T item)
#endif
{
    DebugAssert(array != nullptr, "The input array is nullptr");
#ifndef NDEBUG
    if (ArrayEnsureDebug(array, array->Count + 1, func, file, line))
#else
    if (ArrayEnsure(array, array->Count + 1))
#endif
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
inline bool ArrayEraseFast(Array<T>* array, int index)
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
inline I32 ArrayRemove(Array<T>* array, T value)
{
    I32 index = ArrayIndexOf(*array, value);
    ArrayErase(array, index);
    return index;
}

template <typename T>
inline I32 ArrayRemoveLast(Array<T>* array, T value)
{
    I32 index = ArrayLastIndexOf(*array, value);
    ArrayErase(array, index);
    return index;
}

template <typename T>
inline I32 ArrayRemoveFast(Array<T>* array, T value)
{
    I32 index = ArrayIndexOf(*array, value);
    ArrayEraseFast(array, index);
    return index;
}

template <typename T>
inline I32 ArrayRemoveLastFast(Array<T>* array, T value)
{
    I32 index = ArrayLastIndexOf(*array, value);
    ArrayEraseFast(array, index);
    return index;
}

// ----------------------------------------------------------------------------
// High-level statement helpers (foreach loop, ...)
// ----------------------------------------------------------------------------

template <typename T>
struct Iterable
{
    I32         Count;
    T* Items;

    /// DONOT CALL MANUALLY
    inline T* begin()
    {
        DebugAssert((Items == nullptr && Count == 0) || (Items != nullptr && Count >= 0), "Items and Count is not valid");
        return Items;
    }

    /// DONOT CALL MANUALLY
    inline T* end()
    {
        DebugAssert((Items == nullptr && Count == 0) || (Items != nullptr && Count >= 0), "Items and Count is not valid");
        return Items + Count;
    }
};

template <typename T>
struct ConstIterable
{
    I32         Count;
    T* Items;

    /// DONOT CALL MANUALLY
    inline const T* begin() const
    {
        DebugAssert((Items == nullptr && Count == 0) || (Items != nullptr && Count >= 0), "Items and Count is not valid");
        return Items;
    }

    /// DONOT CALL MANUALLY
    inline const T* end() const
    {
        DebugAssert((Items == nullptr && Count == 0) || (Items != nullptr && Count >= 0), "Items and Count is not valid");
        return Items + Count;
    }
};

template <typename T>
inline ArrayView<T> ViewArray(const T* items, const I32 count)
{
    return { count, items };
}

template <typename T>
inline ArrayView<T> ViewArray(const Array<T>& array)
{
    return { array.Count, array.Items };
}

template <typename T>
inline Iterable<T> IterateArray(Array<T>& array)
{
    return { array.Count, array.Items };
}

template <typename T>
inline Iterable<T> IterateArray(T* items, const I32 count)
{
    return { count, items };
}

template <typename T>
inline ConstIterable<T> IterateArray(ArrayView<T> array)
{
    return { array.Count, array.Items };
}

template <typename T>
inline ConstIterable<T> IterateArray(const Array<T>& array)
{
    return { array.Count, array.Items };
}

template <typename T>
inline ConstIterable<T> IterateArray(const T* items, const I32 count)
{
    return { count, items };
}
