#pragma once

// --------------------------------------
// Main functions
// --------------------------------------

#if !defined(NDEBUG)
#define MemoryAlloc(size)           MemoryAllocDebug(size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryRealloc(ptr, size)    MemoryReallocDebug(ptr, size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFree(ptr)             MemoryFreeDebug(ptr, __FUNCTION__, __FILE__, __LINE__)

void* MemoryAllocDebug(int size, const char* func, const char* file, int line);
void* MemoryReallocDebug(void* ptr, int size, const char* func, const char* file, int line);
void  MemoryFreeDebug(void* ptr, const char* func, const char* file, int line);
#else
void* MemoryAlloc(int size);
void* MemoryRealloc(void* ptr, int size);
void  MemoryFree(void* ptr);
#endif

// --------------------------------------
// Report memory
// --------------------------------------

void MemoryDumpAllocs(void);

enum ImGuiDumpMemoryFlags
{
    ImGuiDumpMemoryFlags_None,
    ImGuiDumpMemoryFlags_OpenWindow = 1 << 0,
};

namespace ImGui
{
    // Open an debug window to view your memory allocations
    void DumpMemoryAllocs(ImGuiDumpMemoryFlags flags);
}

// ------------------------------------
// Memory system information functions
// ------------------------------------

int MemoryPageSize(void);
