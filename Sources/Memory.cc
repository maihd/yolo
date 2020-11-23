#include <Yolo/Types.h>
#include <Yolo/ImGui.h>
#include <Yolo/Memory.h>

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if !defined(NDEBUG)

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// ----------------------
// Internal types
// ----------------------

typedef struct SysFreeList
{
    int     ItemSize;
    void*   FreeItem;
} SysFreeList;

typedef struct AllocDesc
{
    void*               Ptr;
    int                 Size;

    const char*         Func;
    const char*         File;
    int                 Line;

    time_t              CreateTime;
    time_t              ModifiedTime;

    int                 ModifiedCount;
    int                 AddressChangedCount;

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
        const int itemSize = freeList->ItemSize;
        const int allocSize = 64 * 1024;
        const int itemsPerBatch = allocSize / itemSize;

#if defined(_WIN32)
        void* allocBatch = VirtualAlloc(nullptr, (SIZE_T)allocSize, MEM_COMMIT, PAGE_READWRITE);
#else
        void* allocBatch = malloc(allocSize);
#endif

        for (int i = 0; i < itemsPerBatch; i++)
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
    SysFreeList FreeAllocDescs = { sizeof(AllocDesc), nullptr };
    AllocDesc*  HashAllocDescs[ALLOC_DESC_COUNT];

    int         AllocSize = 0;
    int         Allocations = 0;
    int         AllocCalled = 0;
    int         ReallocCalled = 0;
    int         FreeCalled = 0;
} AllocStore;

static void AddAlloc(void* ptr, int size, const char* func, const char* file, int line)
{
    AllocDesc* allocDesc = (AllocDesc*)SysFreeListAcquire(&AllocStore.FreeAllocDescs);

    allocDesc->Ptr  = ptr;
    allocDesc->Size = size;
    allocDesc->Func = func;
    allocDesc->File = file;
    allocDesc->Line = line;

    allocDesc->CreateTime = time(nullptr);
    allocDesc->ModifiedTime = allocDesc->CreateTime;
    
    allocDesc->ModifiedCount = 0;
    allocDesc->AddressChangedCount = 0;

    U64 ptrHash = CalcHashPtr64(ptr) & (ALLOC_DESC_COUNT - 1);
    allocDesc->Next = AllocStore.HashAllocDescs[ptrHash];
    AllocStore.HashAllocDescs[ptrHash] = allocDesc;

    AllocStore.AllocSize += size;
    AllocStore.Allocations++;
}

static void UpdateAlloc(void* ptr, void* newPtr, int size, const char* func, const char* file, int line)
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
    allocDesc->Size = size;
    allocDesc->Func = func;
    allocDesc->File = file;
    allocDesc->Line = line;

    allocDesc->ModifiedTime = time(nullptr);
    allocDesc->ModifiedCount++;
    allocDesc->AddressChangedCount += (ptr != newPtr);

    AllocStore.AllocSize -= allocDesc->Size;
    AllocStore.AllocSize += size;

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

    AllocStore.AllocSize -= allocDesc->Size;
    AllocStore.Allocations--;
}

void* MemoryAllocDebug(int size, const char* func, const char* file, int line)
{
    DebugAssert(size > 0, "Request size must be greater than 0.");

    AllocStore.AllocCalled++;

    void* ptr = malloc((size_t)size);
    AddAlloc(ptr, size, func, file, line);
    return ptr;
}

void* MemoryReallocDebug(void* ptr, int size, const char* func, const char* file, int line)
{
    DebugAssert(size > 0, "Request size must be greater than 0.");

    AllocStore.ReallocCalled++;

    void* newPtr = realloc(ptr, (size_t)size);
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

void MemoryFreeDebug(void* ptr, const char* func, const char* file, int line)
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
        while (allocDesc != nullptr)
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
        ImGui::Text("AllocSize: %.2lfKB", AllocStore.AllocSize / 1024.0);
        ImGui::Text("Allocations: %d", AllocStore.Allocations);
        ImGui::Text("AllocCalled: %d", AllocStore.AllocCalled);
        ImGui::Text("ReallocCalled: %d", AllocStore.ReallocCalled);
        ImGui::Text("FreeCalled: %d", AllocStore.FreeCalled);

        ImGui::Columns(5);
        ImGui::SetColumnWidth(0, 96);
        ImGui::SetColumnWidth(1, 88);
        ImGui::SetColumnWidth(2, 120);
        ImGui::SetColumnWidth(3, 146);

        ImGui::Text("Address");
        ImGui::NextColumn();
        ImGui::Text("Size");
        ImGui::NextColumn();
        ImGui::Text("ModifiedCount");
        ImGui::NextColumn();
        ImGui::Text("AddressChangedCount");
        ImGui::NextColumn();
        ImGui::Text("Source");
        ImGui::NextColumn();

        ImGui::Columns(1);
        if (ImGui::BeginChild("Allocations List"))
        {
            ImGui::Columns(5);
            ImGui::SetColumnWidth(0, 88);
            ImGui::SetColumnWidth(1, 88);
            ImGui::SetColumnWidth(2, 120);
            ImGui::SetColumnWidth(3, 146);

            for (int i = 0; i < ALLOC_DESC_COUNT; i++)
            {
                AllocDesc* allocDesc = AllocStore.HashAllocDescs[i];
                while (allocDesc != nullptr)
                {
                    ImGui::Text("0x%p", allocDesc->Ptr);
                    ImGui::NextColumn();
                    ImGui::Text("%zu", allocDesc->Size);
                    ImGui::NextColumn();
                    ImGui::Text("%d", allocDesc->ModifiedCount);
                    ImGui::NextColumn();
                    ImGui::Text("%d", allocDesc->AddressChangedCount);
                    ImGui::NextColumn();
                    ImGui::Text("%s:%d:%s", allocDesc->File, allocDesc->Line, allocDesc->Func);
                    ImGui::NextColumn();

                    allocDesc = allocDesc->Next;
                }
            }
        }

        ImGui::EndChild();
    }

    if (openWindow)
    {
        ImGui::End();
    }
}
// END OF #if !defined(NDEBUG)
#else
void* MemoryAlloc(int size)
{
    return malloc((size_t)size);
}

void* MemoryRealloc(void* ptr, int size)
{
    return realloc(ptr, (size_t)size);
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

