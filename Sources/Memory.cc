#include <Yolo/Memory.h>

#include <stdio.h>
#include <stdlib.h>

#include <Yolo/ImGui.h>

#if !defined(NDEBUG)

#if defined(__unix__)
#   include <unistd.h>
#elif defined(_WIN32)
#   include <windows.h>
#endif

// ----------------------
// Internal types
// ----------------------

typedef struct SysFreeList
{
    size_t  ItemSize;
    void*   FreeItem;
} SysFreeList;

typedef struct AllocDesc
{
    void*               Ptr;
    size_t              Size;

    const char*         Func;
    const char*         File;
    int                 Line;

    struct AllocDesc*   Next;
} AllocDesc;

static void SysFreeListCollect(SysFreeList* freeList, void* item)
{
    *(void**)item = freeList->FreeItem;
    freeList->FreeItem = item;
}

static void* SysFreeListAcquire(SysFreeList* freeList)
{
    if (!freeList->FreeItem)
    {
        const size_t itemSize = freeList->ItemSize;
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

    void* result = freeList->FreeItem;
    freeList->FreeItem = *((void**)result);
    return result;
}

// ----------------------------
// Tracking memory helpers
// ----------------------------

constexpr int ALLOC_DESC_COUNT = 64;
static struct
{
    SysFreeList FreeAllocDescs = { sizeof(AllocDesc), NULL };
    AllocDesc*  HashAllocDescs[ALLOC_DESC_COUNT];
    int         Allocations = 0;
    int         AllocCalled = 0;
    int         ReallocCalled = 0;
    int         FreeCalled = 0;
} AllocStore;

static void AddAlloc(void* ptr, size_t size, const char* func, const char* file, int line)
{
    AllocDesc* allocDesc = (AllocDesc*)SysFreeListAcquire(&AllocStore.FreeAllocDescs);

    allocDesc->Ptr = ptr;
    allocDesc->Size = size;
    allocDesc->Func = func;
    allocDesc->File = file;
    allocDesc->Line = line;

    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);
    allocDesc->Next = AllocStore.HashAllocDescs[ptrHash];
    AllocStore.HashAllocDescs[ptrHash] = allocDesc;

    AllocStore.Allocations++;
}

static void UpdateAlloc(void* ptr, void* newPtr, size_t size, const char* func, const char* file, int line)
{
    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = AllocStore.HashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->Ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->Next;
    }

    DebugAssert(allocDesc != nullptr, "This block is not allocated by our system, please check your memory source!");
    allocDesc->Ptr = newPtr;

    U64 newPtrHash = CalcHashPtr64(newPtr) & (ALLOC_DESC_COUNT - 1);
    if (newPtrHash != ptrHash)
    {
        if (prevAllocDesc)
        {
            prevAllocDesc->Next = allocDesc->Next;
        }
        else
        {
            AllocStore.HashAllocDescs[ptrHash] = allocDesc->Next;
        }

        allocDesc->Next = AllocStore.HashAllocDescs[newPtrHash];
        AllocStore.HashAllocDescs[newPtrHash] = allocDesc;
    }
}

static void RemoveAlloc(void* ptr, const char* func, const char* file, int line)
{
    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);

    AllocDesc* prevAllocDesc = nullptr;
    AllocDesc* allocDesc = AllocStore.HashAllocDescs[ptrHash];
    while (allocDesc != nullptr && allocDesc->Ptr != ptr)
    {
        prevAllocDesc = allocDesc;
        allocDesc = allocDesc->Next;
    }

    DebugAssert(allocDesc != nullptr, "This block is not allocated by our system! Are you attempt to double-free?");
    SysFreeListCollect(&AllocStore.FreeAllocDescs, allocDesc);

    if (prevAllocDesc)
    {
        prevAllocDesc->Next = allocDesc->Next;
    }
    else
    {
        AllocStore.HashAllocDescs[ptrHash] = allocDesc->Next;
    }

    AllocStore.Allocations--;
}

void* _MemoryAlloc(size_t size, const char* func, const char* file, int line)
{
    AllocStore.AllocCalled++;

    void* ptr = malloc(size);
    AddAlloc(ptr, size, func, file, line);
    return ptr;
}

void* _MemoryRealloc(void* ptr, size_t size, const char* func, const char* file, int line)
{
    AllocStore.ReallocCalled++;

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
    //DebugAssert(ptr != nullptr, "Attempt free nullptr at %s:%d:%s", func, file, line);
    AllocStore.FreeCalled++;

    if (ptr)
    {
        RemoveAlloc(ptr, func, file, line);
        free(ptr);
    }
}

void MemoryDumpAllocs(void)
{
    printf("Address,Size,Source\n");
    for (int i = 0; i < ALLOC_DESC_COUNT; i++)
    {
        AllocDesc* allocDesc = AllocStore.HashAllocDescs[i];
        while (allocDesc != NULL)
        {
            printf("0x%p,%zu,%s:%d:%s\n", allocDesc->Ptr, allocDesc->Size, allocDesc->File, allocDesc->Line, allocDesc->Func);
            allocDesc = allocDesc->Next;
        }
    }
}

// Open an debug window to view your memory allocations
void ImGui::DumpMemoryAllocs(ImGuiDumpMemoryFlags flags)
{
    bool render = true;

    bool openWindow = (flags & ImGuiDumpMemoryFlags_OpenWindow) != 0;
    if (openWindow)
    {
        render = ImGui::Begin("Memory Allocations");
    }

    if (render)
    {
        ImGui::Text("Allocations: %d", AllocStore.Allocations);
        ImGui::Text("AllocCalled: %d", AllocStore.AllocCalled);
        ImGui::Text("ReallocCalled: %d", AllocStore.ReallocCalled);
        ImGui::Text("FreeCalled: %d", AllocStore.FreeCalled);

        ImGui::Columns(3);
        ImGui::SetColumnWidth(0, 96);
        ImGui::SetColumnWidth(1, 88);

        ImGui::Text("Address");
        ImGui::NextColumn();
        ImGui::Text("Size");
        ImGui::NextColumn();
        ImGui::Text("Source");
        ImGui::NextColumn();

        ImGui::Columns(1);
        if (ImGui::BeginChild("Allocations List"))
        {
            ImGui::Columns(3);
            ImGui::SetColumnWidth(0, 88);
            ImGui::SetColumnWidth(1, 88);

            for (int i = 0; i < ALLOC_DESC_COUNT; i++)
            {
                AllocDesc* allocDesc = AllocStore.HashAllocDescs[i];
                while (allocDesc != NULL)
                {
                    ImGui::Text("0x%p", allocDesc->Ptr);
                    ImGui::NextColumn();
                    ImGui::Text("%zu", allocDesc->Size);
                    ImGui::NextColumn();
                    ImGui::Text("%s:%d:%s", allocDesc->File, allocDesc->Line, allocDesc->Func);
                    ImGui::NextColumn();

                    allocDesc = allocDesc->Next;
                }
            }

            ImGui::EndChild();
        }
    }

    if (openWindow)
    {
        ImGui::End();
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

// Open an debug window to view your memory allocations
void ImGui::DumpMemoryAllocs(void)
{
    if (ImGui::Begin("Memory Allocations"))
    {
        ImGui::Text("Cannot tracking memory allocations in Release build!");
        ImGui::End();
    }
}
#endif

