#pragma once

template <typename T>
using Comparer = int (*)(T a, T b);

template <typename T>
inline int DefaultComparer(T a, T b)
{
    return a - b;
}

template <typename T>
void InsertSort(T* items, int count, Comparer<T> comparer = DefaultComparer<T>);

template <typename T>
inline void InsertSort(T* items, int count, Comparer<T> comparer)
{
    T* lo = items;
    T* hi = items + count;
    while (hi > lo)
    {
        T* max = lo;
        for (T* p = lo + 1; p <= hi; p++)
        {
            if (comparer(*p, *max) > 0)
            {
                max = p;
            }
        }

        T tmp = *max;
        *max = *hi;
        *hi = tmp;
        hi--;
    }
}
