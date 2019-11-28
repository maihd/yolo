#pragma once

#include <Yolo/Types.h>

enum struct FileMode
{
    None,
    Read    = 1 << 0,
    Write   = 1 << 1,
    Binary  = 1 << 2,
    Create  = 1 << 3,
};

struct File
{
    Handle handle;
    
    File Open(string path, FileMode mode);
    void Close(File file);

    int  GetSize(File file);
    int  GetSize(string path);

    uint64 GetSize64(File file);
    uint64 GetSize64(string path);

    int Read(File file, void* buffer, int length);
    uint64 Read64(File file, void* buffer, uint64 length);

    int Write(File file, const void* buffer, int length);
    uint64 Write(File file, const void* buffer, uint64 length);
};