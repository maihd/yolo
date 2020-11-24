#include <Yolo/Heap/PagedHeap.h>

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
