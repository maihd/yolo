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
        ItemSize = size;

        const int itemSize = size;
        const int allocSize = 64 * 1024;
        const int itemsPerBatch = allocSize / itemSize;

    #if defined(_WIN32)
        void* allocBatch = VirtualAlloc(nullptr, (SIZE_T)allocSize, MEM_COMMIT, PAGE_READWRITE);
    #elif defined(__unix__)
        void* allocBatch = mmap(nullptr, (size_t)allocSize, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0, 0);
    #else
        #error "The current system doesnot support paged allocations"
    #endif

        for (int i = 0; i < itemsPerBatch; i++)
        {
            Free((U8*)allocBatch + i * itemSize);
        }
    }

    void* result = FreeItem;
    FreeItem = *((void**)result);
    return result;
}

void PagedFreeList::Free(void* ptr)
{
    *((void**)ptr) = FreeItem;
    FreeItem = ptr;
}
