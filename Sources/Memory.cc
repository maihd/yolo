#include <Yolo/Memory.h>

#include <stdio.h>
#include <stdlib.h>

#if !defined(NDEBUG)

#if defined(__unix__)
#   include <unistd.h>
#elif defined(_WIN32)
#   include <windows.h>
#endif

typedef struct SysFreeList
{
    size_t  itemSize;
    void*   freeItem;
} SysFreeList;

typedef struct AllocDesc
{
    void*               ptr;
    size_t              size;

    const char*         func;
    const char*         file;
    int                 line;

    struct AllocDesc*   next;
} AllocDesc;

static void SysFreeListCollect(SysFreeList* freeList, void* item)
{
    *(void**)item = freeList->freeItem;
    freeList->freeItem = item;
}

static void* SysFreeListAcquire(SysFreeList* freeList)
{
    if (!freeList->freeItem)
    {
        const size_t itemSize = freeList->itemSize;
        const size_t allocSize = 64 * 1024;
        const size_t itemsPerBatch = allocSize / itemSize;

#if defined(__unix__)
        void* allocBatch = vmalloc(allocSize);
#elif defined(_WIN32)
        void* allocBatch = VirtualAlloc(NULL, (SIZE_T)allocSize, MEM_COMMIT, PAGE_READWRITE);
#else
        void* allocBatch = malloc(allocSize);
#endif

        for (size_t i = 0; i < itemsPerBatch; i++)
        {
            SysFreeListCollect(freeList, (uint8_t*)allocBatch + i * itemSize);
        }
    }

    void* result = freeList->freeItem;
    freeList->freeItem = *((void**)result);
    return result;
}

enum { ALLOC_DESC_COUNT = 64 };

static SysFreeList freeAllocDescs = { sizeof(AllocDesc), NULL };
static AllocDesc* hashAllocDescs[ALLOC_DESC_COUNT];

static void AddAlloc(void* ptr, size_t size, const char* func, const char* file, int line)
{
    AllocDesc* allocDesc = (AllocDesc*)SysFreeListAcquire(&freeAllocDescs);

    allocDesc->ptr = ptr;
    allocDesc->size = size;
    allocDesc->func = func;
    allocDesc->file = file;
    allocDesc->line = line;

    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);
    allocDesc->next = hashAllocDescs[ptrHash];
    hashAllocDescs[ptrHash] = allocDesc;
}

static void UpdateAlloc(void* ptr, void* newPtr, size_t size, const char* func, const char* file, int line)
{
    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = hashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->next;
    }

    DebugAssert(allocDesc != nullptr, "This block is not allocated by our system, please check your memory source!");
    allocDesc->ptr = newPtr;

    U64 newPtrHash = CalcHashPtr64(newPtr) & (ALLOC_DESC_COUNT - 1);
    if (newPtrHash != ptrHash)
    {
        if (prevAllocDesc)
        {
            prevAllocDesc->next = allocDesc->next;
        }
        else
        {
            hashAllocDescs[ptrHash] = allocDesc->next;
        }

        allocDesc->next = hashAllocDescs[newPtrHash];
        hashAllocDescs[newPtrHash] = allocDesc;
    }
}

static void RemoveAlloc(void* ptr, const char* func, const char* file, int line)
{
    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = hashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->next;
    }

    DebugAssert(allocDesc != nullptr, "This block is not allocated by our system! Are you attempt to double-free?");
    SysFreeListCollect(&freeAllocDescs, allocDesc);

    if (prevAllocDesc)
    {
        prevAllocDesc->next = allocDesc->next;
    }
    else
    {
        hashAllocDescs[ptrHash] = allocDesc->next;
    }
}

void* _MemoryAlloc(size_t size, const char* func, const char* file, int line)
{
    void* ptr = malloc(size);
    AddAlloc(ptr, size, func, file, line);
    return ptr;
}

void* _MemoryRealloc(void* ptr, size_t size, const char* func, const char* file, int line)
{
    void* newPtr = realloc(ptr, size);
    if (ptr == nullptr)
    {
        AddAlloc(newPtr, size, func, file, line);
    }
    else
    {
        UpdateAlloc(ptr, newPtr, size, func, file, line);
    }
    return newPtr;
}

void _MemoryFree(void* ptr, const char* func, const char* file, int line)
{
    DebugAssert(ptr != nullptr, "Attempt free nullptr at %s:%d:%s", func, file, line);

    RemoveAlloc(ptr, func, file, line);
    free(ptr);
}

void MemoryDumpAllocs(void)
{
    printf("Address,Size,Source\n");
    for (int i = 0; i < ALLOC_DESC_COUNT; i++)
    {
        AllocDesc* allocDesc = hashAllocDescs[i];
        while (allocDesc != NULL)
        {
            printf("0x%p,%zu,%s:%d:%s\n", allocDesc->ptr, allocDesc->size, allocDesc->file, allocDesc->line, allocDesc->func);
            allocDesc = allocDesc->next;
        }
    }
}
// END OF #if !defined(NDEBUG)
#else
void* MemoryAlloc(size_t size)
{
    return malloc(size);
}

void* MemoryRealloc(void* ptr, size_t size)
{
    return realloc(ptr, size);
}

void MemoryFree(void* ptr)
{
    free(ptr);
}

void MemoryDumpAllocs(void)
{
}
#endif

// Open an debug window to view your memory allocations
void Imgui::DumpMemoryAllocs(void)
{
    printf("Address,Size,Source\n");
    for (int i = 0; i < ALLOC_DESC_COUNT; i++)
    {
        AllocDesc* allocDesc = hashAllocDescs[i];
        while (allocDesc != NULL)
        {
            printf("0x%p,%zu,%s:%d:%s\n", allocDesc->ptr, allocDesc->size, allocDesc->file, allocDesc->line, allocDesc->func);
            allocDesc = allocDesc->next;
        }
    }
}
