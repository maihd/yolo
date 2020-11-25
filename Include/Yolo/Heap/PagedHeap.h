#pragma once

struct PagedHeap
{
    void*   Alloc(int size);
    void*   Realloc(void* ptr, int size);
    void    Free(void* ptr);
    int     GetSize(void* ptr);
};
