#pragma once

#include <stdlib.h>
#include <string.h>

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

struct StrictSegHeapTraits
{
    // Return the bin that this size falls into
    static inline int GetSizeBin(int size)
    {
        int bin = 0;
        size--;
        while (size > 7)
        {
            size = size >> 1;
            bin++;
        }
        return bin;
    }
 
    // Return the size of allocations in this bin
    static inline int GetBinMaxSize(int bin)
    {
        return 1 << (bin + 3);
    }
};

template <int BinCount, typename Traits, typename LittleHeap, typename SuperHeap>
struct StrictSegHeap : public SuperHeap
{
    // The array of little heaps (bins), one for each allocation size
    LittleHeap LittleHeaps[BinCount];

    // Return the bin that this allocation falls into
    inline int InnerGetSizeBin(int size)
    {
        if (size > Traits::GetBinMaxSize(BinCount - 1))
        {
            return BinCount;
        }

        return Traits::GetSizeBin(size);
    }

    inline void* Alloc(int size)
    {
        int sizeBin = InnerGetSizeBin(size);
        if (sizeBin >= BinCount)
        {
            return SuperHeap::Alloc(size);
        }

        int allocSize = Traits::GetBinMaxSize(sizeBin);
        return LittleHeaps[sizeBin].Alloc(allocSize);
    }

    inline void* Realloc(void* ptr, int size)
    {
        if (ptr)
        {
            int allocedSize = SuperHeap::GetSize(ptr);
            int allocedSizeBin = InnerGetSizeBin(allocedSize);
            if (allocedSizeBin >= BinCount)
            {
                return SuperHeap::Realloc(ptr, size);
            }

            // Loose reallocation: only realloc if bigger or at least twice smaller
            if ((size > allocedSize) || (size < (allocedSize >> 1)))
            {
                void* newPtr = Alloc(size);
                if (newPtr)
                {
                    int copySize = allocedSize > size ? size : allocedSize;
                    memcpy(newPtr, ptr, copySize);
                }
                LittleHeaps[allocedSizeBin].Free(ptr);

                return newPtr;
            }

            return ptr;
        }

        return Alloc(size);
    }

    inline void Free(void* ptr)
    {
        if (ptr)
        {
            int size = SuperHeap::GetSize(ptr);
            int sizeBin = InnerGetSizeBin(size);
            if (sizeBin >= BinCount)
            {
                SuperHeap::Free(ptr);
            }
            else
            {
                LittleHeaps[sizeBin].Free(ptr);
            }
        }
    }
};

struct PagedHeap
{
    void*   Alloc(int size);
    void*   Realloc(void* ptr, int size);
    void    Free(void* ptr);
    int     GetSize(void* ptr);
};

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

struct CrtMalloc
{
    inline void* Alloc(int size)
    {
        return malloc((size_t)size);
    }

    inline void* Realloc(void* ptr, int size)
    {
        return realloc(ptr, (size_t)size);
    }

    inline void Free(void* ptr)
    {
        free(ptr);
    }
};
