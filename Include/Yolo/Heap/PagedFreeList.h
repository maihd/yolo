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

    Item*   FreeItem;
    Page*   AllocedPages;

    void*   Alloc(int size);
    void    Free(void* ptr);
    int     GetSize(void* ptr) const;

            ~PagedFreeList();

    inline  PagedFreeList()
        : FreeItem(nullptr)
        , AllocedPages(nullptr)
    {
    }

};
