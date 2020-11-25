#define _CRT_SECURE_NO_WARNINGS

#include <Yolo/File.h>
#include <Yolo/Array.h>
#include <Yolo/String.h>

#include <stdio.h>
#include <Windows.h>
#include <Shlwapi.h>

#undef  RemoveDirectory
#pragma comment(lib, "Shlwapi.lib")

namespace FileOps
{
    static Array<String> searchPaths;

    void AddSearchPath(String path)
    {
        if (ArrayIndexOf(searchPaths, path) == -1)
        {
            ArrayPush(&searchPaths, path);
        }
    }

    void RemoveSearchPath(String path)
    {
        ArrayRemove(&searchPaths, path);
    }

    bool Exists(const char* path, bool useSearchPath)
    {
        if (!PathFileExistsA(path))
        {
            if (useSearchPath)
            {
                char pathBuffer[2048];
                for (I32 i = 0, n = searchPaths.Count; i < n; i++)
                {
                    String searchPath = searchPaths.Items[i];

                    ::sprintf(pathBuffer, "%s/%s", searchPath.Buffer, path);
                    if (!PathFileExistsA(pathBuffer))
                    {
                        return true;
                    }
                }
            }

            return false;
        }
        else
        {
            return true;
        }
    }

    const char* GetFullPath(const char* path)
    {
        thread_local char pathBuffer[2048];

        if (!Exists(path, false))
        {
            for (I32 i = 0, n = searchPaths.Count; i < n; i++)
            {
                String searchPath = searchPaths.Items[i];

                sprintf(pathBuffer, "%s/%s", searchPath.Buffer, path);
                if (Exists(pathBuffer, false))
                {
                    return pathBuffer;
                }
            }

            return "";
        }
        else
        {
            return path;
        }
    }

    File Open(const char* path, FileMode mode)
    {
        const char* fullPath = GetFullPath(path);
        if (fullPath == "")
        {
            return 0;
        }

        DWORD access        = 0;
        DWORD shared        = 0;
        DWORD disposition   = 0;
        DWORD attributes    = 0;

        switch (mode & (FileMode::ReadWrite))
        {
        case FileMode::Read:
            access |= FILE_GENERIC_READ;
            break;

        case FileMode::Write:
            access |= FILE_GENERIC_WRITE;
            break;

        default:
            return 0;
        }

        if (mode & FileMode::Append)
        {
            access &= ~FILE_WRITE_DATA;
            access |= FILE_APPEND_DATA;
        }

        shared = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

        switch (mode & (FileMode::Create | FileMode::Existing | FileMode::Truncate))
        {
        case 0:
        case FileMode::Existing:
            disposition = OPEN_EXISTING;
            break;
        
        case FileMode::Create:
            disposition = OPEN_ALWAYS;
            break;
        
        case FileMode::Create | FileMode::Existing:
        case FileMode::Create | FileMode::Truncate | FileMode::Existing:
            disposition = CREATE_NEW;
            break;
        
        case FileMode::Truncate:
        case FileMode::Truncate | FileMode::Existing:
            disposition = TRUNCATE_EXISTING;
            break;
        
        case FileMode::Create | FileMode::Truncate:
            disposition = CREATE_ALWAYS;
            break;
        
        default:
            return {};
        }

        attributes |= FILE_ATTRIBUTE_NORMAL;
        if (mode & FileMode::Create)
        {
            if (!(mode & FileMode::Write))
            {
                attributes |= FILE_ATTRIBUTE_READONLY;
            }
        }

        if (mode & FileMode::Temporary)
        {
            attributes |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;
            access |= DELETE;
        }

        if (mode & FileMode::ShortLive)
        {
            attributes |= FILE_ATTRIBUTE_TEMPORARY;
        }

        switch (mode & (FileMode::Sequence | FileMode::Random))
        {
        case 0:
            break;
        
        case FileMode::Sequence:
            attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;
        
        case FileMode::Random:
            attributes |= FILE_FLAG_RANDOM_ACCESS;
            break;
        
        default:
            return {};
        }

        if (mode & FileMode::Direct)
        {
            /*
             * FILE_APPEND_DATA and FILE_FLAG_NO_BUFFERING are mutually exclusive.
             * Windows returns 87, ERROR_INVALID_PARAMETER if these are combined.
             *
             * FILE_APPEND_DATA is included in FILE_GENERIC_WRITE:
             *
             * FILE_GENERIC_WRITE = STANDARD_RIGHTS_WRITE |
             *                      FILE_WRITE_DATA |
             *                      FILE_WRITE_ATTRIBUTES |
             *                      FILE_WRITE_EA |
             *                      FILE_APPEND_DATA |
             *                      SYNCHRONIZE
             *
             * Note: Appends are also permitted by FILE_WRITE_DATA.
             *
             * In order for direct writes and direct appends to succeed, we therefore
             * exclude FILE_APPEND_DATA if FILE_WRITE_DATA is specified, and otherwise
             * fail if the user's sole permission is a direct Append, since this
             * particular combination is invalid.
             */
            if (access & FILE_APPEND_DATA)
            {
                if (access & FILE_WRITE_DATA)
                {
                    access &= ~FILE_APPEND_DATA;
                }
                else
                {
                    return {};
                }
            }
            attributes |= FILE_FLAG_NO_BUFFERING;
        }

        switch (mode & (FileMode::DataSync | FileMode::Sync))
        {
        case 0:
            break;
        
        case FileMode::Sync:
        case FileMode::DataSync:
            attributes |= FILE_FLAG_WRITE_THROUGH;
            break;
        
        default:
            return {};
        }

        /* Setting this flag makes it possible to open a directory. */
        if (mode & FileMode::Directory)
        {
            attributes |= FILE_FLAG_BACKUP_SEMANTICS;
        }
        
        // Async read and write file
        if (mode & FileMode::NonBlock)
        {
            //attributes |= FILE_FLAG_OVERLAPPED;
        }

        HANDLE handle = CreateFileA(path,
            access,
            shared,
            NULL,
            disposition,
            attributes,
            NULL);

        if (handle == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();
            if (error == ERROR_FILE_EXISTS && (mode & FileMode::Create) && !(mode & FileMode::Existing))
            {
                /* Special case: when ERROR_FILE_EXISTS happens and UV_FS_O_CREAT was
                 * specified, it means the path referred to a directory. */
                 //SET_REQ_UV_ERROR(req, UV_EISDIR, error);
            }
            else
            {
                //SET_REQ_WIN32_ERROR(req, GetLastError());
            }
            return 0;
        }
        else
        {
            return (Handle)(intptr_t)handle;
        }

        return 0;
    }

    void Close(File file)
    {
        CloseHandle((HANDLE)(intptr_t)file);
    }

    I32 GetSize(File file)
    {
        return (I32)::GetFileSize((HANDLE)(intptr_t)file, 0);
    }

    I32 GetSize(const char* path)
    {
        File file = Open(path, FileMode::Read);
        I32  size = GetSize(file);
        Close(file);

        return size;
    }

    I64 GetSize64(File file)
    {
        LARGE_INTEGER size;
        if (::GetFileSizeEx((HANDLE)(intptr_t)file, &size))
        {
            return size.QuadPart;
        }
        else
        {
            return 0;
        }
    }

    I64 GetSize64(const char* path)
    {
        File  file = Open(path, FileMode::Read);
        I64 size = GetSize64(file);
        Close(file);

        return size;
    }

    I32 Read(File file, void* buffer, I32 length)
    {
        if (length <= 0)
        {
            return 0;
        }
        
        DWORD readBytes;
        if (::ReadFile((HANDLE)(intptr_t)file, buffer, (DWORD)length, &readBytes, 0))
        {
            return (I32)readBytes;
        }
        else
        {
            return 0;
        }
    }

    I32 Write(File file, const void* buffer, I32 length)
    {
        if (length <= 0)
        {
            return 0;
        }

        DWORD writeBytes;
        if (::WriteFile((HANDLE)(intptr_t)file, buffer, (DWORD)length, &writeBytes, 0))
        {
            return (I32)writeBytes;
        }
        else
        {
            return 0;
        }
    }
}
