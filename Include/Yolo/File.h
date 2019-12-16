#pragma once

#include <Yolo/Types.h>

namespace FileOps
{
    void    AddSearchPath(String path);
    void    RemoveSearchPath(String path);

    bool    Exists(String path, bool useSearchPath = true);
    String  GetFullPath(String expectPath);

    File    Open(String path, int mode);
    void    Close(File file);

    int     GetSize(File file);
    int     GetSize(String path);

    int64   GetSize64(File file);
    int64   GetSize64(String path);

    int     Read(File file, void* buffer, int length);
    int     Write(File file, const void* buffer, int length);
}
