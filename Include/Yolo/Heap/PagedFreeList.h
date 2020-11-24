#pragma once

struct PagedFreeList
{
    void*   FreeItem;
    int     ItemSize;

    void*   Alloc(int size);
    void    Free(void* ptr);

    inline PagedFreeList()
        : FreeItem(nullptr)
        , ItemSize(0)
    {
    }

    inline int GetSize(void* ptr) const
    {
        return ItemSize;
    }
};
