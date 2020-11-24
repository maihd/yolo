#pragma once

template <typename SuperHeap>
struct SizeHeap : public SuperHeap
{
    inline void* Alloc(int size)
    {
        int* item = (int*)SuperHeap::Alloc(sizeof(int) + size);
        if (item)
        {
            *item = size;
        }
        return item + 1;
    }

    inline void* Realloc(void* ptr, int size)
    {
        int* newItem = (int*)SuperHeap::Realloc(ptr ? ((int*)ptr - 1) : nullptr, sizeof(int));
        if (newItem)
        {
            *newItem = size;
        }
        return newItem + 1;
    }

    inline void Free(void* ptr)
    {
        SuperHeap::Free((int*)ptr - 1);
    }

    inline int GetSize(void* ptr)
    {
        return *((int*)ptr - 1);
    }
};
