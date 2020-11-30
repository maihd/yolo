#include <Yolo/Types.h>
#include <Yolo/Heap/PagedFreeList.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__unix__)
#include <sys/mman.h>
#endif

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
        VirtualFree(page, 0, MEM_DECOMMIT);
    #else

    #endif
        page = next;
    }

    FreeItem = nullptr;
    AllocedPages = nullptr;
}
