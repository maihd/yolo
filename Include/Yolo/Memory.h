#pragma once

#include <stdint.h>
#include <string.h>
#include <Yolo/Types.h>

#if !defined(NDEBUG)
#define MemoryAlloc(size)              _MemoryAlloc(size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryRealloc(ptr, size)       _MemoryRealloc(ptr, size, __FUNCTION__, __FILE__, __LINE__)
#define MemoryFree(ptr)                _MemoryFree(ptr, __FUNCTION__, __FILE__, __LINE__)

void* _MemoryAlloc(size_t size, const char* func, const char* file, int line);
void* _MemoryRealloc(void* ptr, size_t size, const char* func, const char* file, int line);
void  _MemoryFree(void* ptr, const char* func, const char* file, int line);
#else
void* MemoryAlloc(size_t size);
void* MemoryRealloc(void* ptr, size_t size);
void  MemoryFree(void* ptr);
#endif

#define MemoryInit(dst, value, size)   memset(dst, value, size)
#define MemoryCopy(dst, src, size)     memcpy(dst, src, size)
#define MemoryMove(dst, src, size)     memmove(dst, src, size)

void MemoryDumpAllocs(void);

static inline U32 CalcHashPtr32(void* ptr, U32 seed = 0)
{
    const U32 magic = 2057;

    U32 value = (U32)(uintptr_t)ptr + (seed ^ magic);
    value = ~value + (value << 15);
    value =  value ^ (value >> 12);
    value =  value + (value << 2);
    value =  value ^ (value >> 4);
    value =  value * magic;
    value =  value ^ (value >> 16);
    return value;
}

static inline U64 CalcHashPtr64(void* ptr, U32 seed = 0)
{
    const U64 magic = 41142057ULL;

    U64 value = (U64)(uintptr_t)ptr + (seed ^ magic);
    value = ~value + (value << 30);
    value =  value ^ (value >> 24);
    value =  value + (value << 4);
    value =  value ^ (value >> 8);
    value =  value * magic;
    value =  value ^ (value >> 32);
    return value;
}

namespace Imgui
{
    // Open an debug window to view your memory allocations
    void DumpMemoryAllocs(void);
}
