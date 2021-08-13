#include <System/Core.h>
#include <System/Heap.h>

#include <math.h>
#include <stdio.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__unix__)
#include <sys/mman.h>
#else
#error "The current system doesnot support paged allocations"
#endif

void* PagedHeap::Alloc(int size)
{
#if defined(_WIN32)
    return VirtualAlloc(nullptr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__unix__)
    return mmap(nullptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
#endif
}

void* PagedHeap::Realloc(void* ptr, int size)
{
#if defined(_WIN32)
    return VirtualAlloc(ptr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__unix__)
    return mmap(ptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
#endif
}

void PagedHeap::Free(void* ptr)
{
#if defined(_WIN32)
    VirtualFree(ptr, 0, MEM_DECOMMIT);
#elif defined(__unix__)
    munmap(ptr, 0);
#endif
}

int PagedHeap::GetSize(void* ptr)
{
    DebugAssert(false, "NOT IMPLEMENTED");
    return 0;
}

void* PagedFreeList::Alloc(int size)
{
    if (!FreeItem)
    {
        const int itemSize = size;
        const int allocSize = 64 * 1024;
        const int itemsPerBatch = (allocSize - sizeof(Page)) / itemSize;

    #if defined(_WIN32)
        Page* page = (Page*)VirtualAlloc(nullptr, (SIZE_T)allocSize, MEM_COMMIT, PAGE_READWRITE);
    #elif defined(__unix__)
        Page* page = (Page*)mmap(nullptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
    #else
        #error "The current system doesnot support paged allocations"
    #endif

        page->Next = AllocedPages;
        page->ItemSize = size;
        AllocedPages = page;

        void* allocBatch = page + 1;
        for (int i = 0; i < itemsPerBatch; i++)
        {
            Free((U8*)allocBatch + i * itemSize);
        }
    }

    Item* item = FreeItem;
    FreeItem = item->Next;
    return item;
}

void PagedFreeList::Free(void* ptr)
{
    Item* item = (Item*)ptr;
    item->Next = FreeItem;
    FreeItem = item;
}

int PagedFreeList::GetSize(void* ptr) const
{
    DebugAssert(false, "NOT IMPLEMENTED");
    return 0;
}

PagedFreeList::~PagedFreeList()
{
    Page* page = AllocedPages;
    while (page != nullptr)
    {
        Page* next = page->Next;
    #if defined(_WIN32)
        //VirtualFree(page, 0, MEM_DECOMMIT);
    #else

    #endif
        page = next;
    }

    FreeItem = nullptr;
    AllocedPages = nullptr;
}
