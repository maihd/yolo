#pragma once

#include <Yolo/Types.h>

namespace FileOps
{
    void        AddSearchPath(const char* path);
    void        RemoveSearchPath(const char* path);

    bool        Exists(const char* path, bool useSearchPath = true);
    const char* GetFullPath(const char* expectPath);

    File        Open(const char* path, FileMode mode);
    void        Close(File file);

    I32         GetSize(File file);
    I32         GetSize(const char* path);

    I64         GetSize64(File file);
    I64         GetSize64(const char* path);

    I32         Read(File file, void* buffer, I32 length);
    I32         Write(File file, const void* buffer, I32 length);

    I32         Read64(File file, void* buffer, I64 length);
    I32         Write64(File file, const void* buffer, I64 length);
}
