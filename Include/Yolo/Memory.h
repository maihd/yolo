#pragma once

#include <stdint.h>
#include <string.h>
#include <Yolo/Types.h>

// --------------------------------------
// Main functions
// --------------------------------------

#if !defined(NDEBUG)
#define MemoryAlloc(size)              MemoryAllocDebug(size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryRealloc(ptr, size)       MemoryReallocDebug(ptr, size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFree(ptr)                MemoryFreeDebug(ptr, __FUNCTION__, __FILE__, __LINE__)

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

// -----------------------------
// Small utils functions
// -----------------------------

inline U32 CalcHashPtr32(void* ptr, U32 seed = 0)
{
    const U32 magic = 2057;

    U32 value = (U32)(uintptr_t)ptr + (seed ^ magic);
    value = ~value + (value << 15);
    value = value ^ (value >> 12);
    value = value + (value << 2);
    value = value ^ (value >> 4);
    value = value * magic;
    value = value ^ (value >> 16);
    return value;
}

inline U64 CalcHashPtr64(void* ptr, U32 seed = 0)
{
    const U64 magic = 41142057ULL;

    U64 value = (U64)(uintptr_t)ptr + (seed ^ magic);
    value = ~value + (value << 30);
    value = value ^ (value >> 24);
    value = value + (value << 4);
    value = value ^ (value >> 8);
    value = value * magic;
    value = value ^ (value >> 32);
    return value;
}
