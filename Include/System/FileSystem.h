#pragma once

#include <System/Core.h>

// ------------------------------
// File system
// ------------------------------

void        AddFileSearchPath(StringView path);
void        RemoveFileSearchPath(StringView path);

bool        FileExists(StringView path, bool useSearchPath = true);
String      GetFullPath(StringView expectPath);

// ------------------------------
// Bassic on files
// ------------------------------

File        OpenFile(StringView path, FileMode mode);
void        CloseFile(File file);

I32         GetFileSize(File file);
I32         GetFileSize(StringView path);

I64         GetFileSize64(File file);
I64         GetFileSize64(StringView path);

I32         FileRead(File file, void* buffer, I32 length);
I32         FileWrite(File file, const void* buffer, I32 length);

I64         FileRead64(File file, void* buffer, I64 length);
I64         FileWrite64(File file, const void* buffer, I64 length);

// ------------------------------
// Extensions & helpers
// ------------------------------

Buffer      LoadFileData(File file);
Buffer      LoadFileData(StringView path);

Buffer64    LoadFileData64(File file);
Buffer64    LoadFileData64(StringView path);
