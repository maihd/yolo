#pragma once

#include <Yolo/Types.h>

namespace FileOps
{
    File Open(String path, FileMode mode);
    void Close(File file);

    int  GetSize(File file);
    int  GetSize(String path);

    uint64 GetSize64(File file);
    uint64 GetSize64(String path);

    int Read(File file, void* buffer, int length);
    uint64 Read64(File file, void* buffer, uint64 length);

    int Write(File file, const void* buffer, int length);
    uint64 Write(File file, const void* buffer, uint64 length);
}
