#pragma once

#include <Yolo/Types.h>

namespace FileOps
{
    void    AddSearchPath(String path);
    void    RemoveSearchPath(String path);

    bool    Exists(String path, bool useSearchPath = true);
    String  GetFullPath(String expectPath);

    File    Open(String path, FileMode mode);
    void    Close(File file);

    I32     GetSize(File file);
    I32     GetSize(String path);

    I64     GetSize64(File file);
    I64     GetSize64(String path);

    I32     Read(File file, void* buffer, I32 length);
    I32     Write(File file, const void* buffer, I32 length);

    I32     Read64(File file, void* buffer, I64 length);
    I32     Write64(File file, const void* buffer, I64 length);
}
