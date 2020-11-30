#pragma once

#include <Yolo/Core.h>

// ------------------------------
// File system
// ------------------------------

void        AddFileSearchPath(const char* path);
void        RemoveFileSearchPath(const char* path);

bool        FileExists(const char* path, bool useSearchPath = true);
const char* GetFullPath(const char* expectPath);

// ------------------------------
// Bassic on files
// ------------------------------

File        OpenFile(const char* path, FileMode mode);
void        CloseFile(File file);

I32         GetFileSize(File file);
I32         GetFileSize(const char* path);

I64         GetFileSize64(File file);
I64         GetFileSize64(const char* path);

I32         FileRead(File file, void* buffer, I32 length);
I32         FileWrite(File file, const void* buffer, I32 length);

I64         FileRead64(File file, void* buffer, I64 length);
I64         FileWrite64(File file, const void* buffer, I64 length);

// ------------------------------
// Extensions & helpers
// ------------------------------

Buffer      LoadFileData(File file);
Buffer      LoadFileData(const char* path);

Buffer64    LoadFileData64(File file);
Buffer64    LoadFileData64(const char* path);
