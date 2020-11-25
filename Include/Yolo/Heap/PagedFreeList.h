#pragma once

struct PagedFreeList
{
    struct Item
    {
        Item* Next;
    };

    struct Page
    {
        int   PageSize;
        int   ItemSize;

        Page* Next;
    };

    void*   FreeItem;
    int     ItemSize;

    void*   Alloc(int size);
    void    Free(void* ptr);
    int     GetSize(void* ptr) const;

    inline PagedFreeList()
        : FreeItem(nullptr)
        , ItemSize(0)
    {
    }
};
