#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>
#include <Shlwapi.h>

#include <Yolo/File.h>
#include <Yolo/Array.h>
#include <Yolo/String.h>

#undef  RemoveDirectory
#pragma comment(lib, "Shlwapi.lib")

namespace FileOps
{
    static Array<String> searchPaths;

    void AddSearchPath(String path)
    {
        if (ArrayOps::IndexOf(searchPaths, path) == -1)
        {
            ArrayOps::Push(&searchPaths, path);
        }
    }

    void RemoveSearchPath(String path)
    {
        ArrayOps::Remove(&searchPaths, path);
    }

    bool Exists(String path, bool useSearchPath)
    {
        if (!::PathFileExistsA(path))
        {
            if (useSearchPath)
            {
                char pathBuffer[2048];
                if (!Exists(path, false))
                {
                    String originPath = path;
                    for (I32 i = 0, n = searchPaths.length; i < n; i++)
                    {
                        String searchPath = searchPaths.elements[i];

                        ::sprintf(pathBuffer, "%s/%s", searchPath, originPath);
                        if (Exists(pathBuffer, false))
                        {
                            return true;
                        }
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

    String GetFullPath(String path)
    {
        thread_local char pathBuffer[2048];

        if (!PathFileExistsA(path))
        {
            if (!Exists(path, false))
            {
                String originPath = path;
                for (I32 i = 0, n = searchPaths.length; i < n; i++)
                {
                    String searchPath = searchPaths.elements[i];

                    sprintf(pathBuffer, "%s/%s", searchPath, originPath);
                    if (Exists(pathBuffer, false))
                    {
                        return pathBuffer;
                    }
                }
            }

            return "";
        }
        else
        {
            return path;
        }
    }

    File Open(String path, FileMode mode)
    {
        String fullPath = GetFullPath(path);
        if (fullPath == "")
        {
            return 0;
        }

        DWORD access        = 0;
        DWORD shared        = 0;
        DWORD disposition   = 0;
        DWORD attributes    = 0;

        switch (mode & (FileModes::ReadWrite))
        {
        case FileModes::Read:
            access |= FILE_GENERIC_READ;
            break;

        case FileModes::Write:
            access |= FILE_GENERIC_WRITE;
            break;

        default:
            return 0;
        }

        if (mode & FileModes::Append)
        {
            access &= ~FILE_WRITE_DATA;
            access |= FILE_APPEND_DATA;
        }

        shared = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

        switch (mode & (FileModes::Create | FileModes::Existing | FileModes::Truncate))
        {
        case 0:
        case FileModes::Existing:
            disposition = OPEN_EXISTING;
            break;
        
        case FileModes::Create:
            disposition = OPEN_ALWAYS;
            break;
        
        case FileModes::Create | FileModes::Existing:
        case FileModes::Create | FileModes::Truncate | FileModes::Existing:
            disposition = CREATE_NEW;
            break;
        
        case FileModes::Truncate:
        case FileModes::Truncate | FileModes::Existing:
            disposition = TRUNCATE_EXISTING;
            break;
        
        case FileModes::Create | FileModes::Truncate:
            disposition = CREATE_ALWAYS;
            break;
        
        default:
            return {};
        }

        attributes |= FILE_ATTRIBUTE_NORMAL;
        if (mode & FileModes::Create)
        {
            if (!(mode & FileModes::Write))
            {
                attributes |= FILE_ATTRIBUTE_READONLY;
            }
        }

        if (mode & FileModes::Temporary)
        {
            attributes |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;
            access |= DELETE;
        }

        if (mode & FileModes::ShortLive)
        {
            attributes |= FILE_ATTRIBUTE_TEMPORARY;
        }

        switch (mode & (FileModes::Sequence | FileModes::Random))
        {
        case 0:
            break;
        
        case FileModes::Sequence:
            attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;
        
        case FileModes::Random:
            attributes |= FILE_FLAG_RANDOM_ACCESS;
            break;
        
        default:
            return {};
        }

        if (mode & FileModes::Direct)
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

        switch (mode & (FileModes::DataSync | FileModes::Sync))
        {
        case 0:
            break;
        
        case FileModes::Sync:
        case FileModes::DataSync:
            attributes |= FILE_FLAG_WRITE_THROUGH;
            break;
        
        default:
            return {};
        }

        /* Setting this flag makes it possible to open a directory. */
        if (mode & FileModes::Directory)
        {
            attributes |= FILE_FLAG_BACKUP_SEMANTICS;
        }
        
        // Async read and write file
        if (mode & FileModes::NonBlock)
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
            if (error == ERROR_FILE_EXISTS && (mode & FileModes::Create) && !(mode & FileModes::Existing))
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
            return { (Handle)handle };
        }

        return 0;
    }

    void Close(File file)
    {
        CloseHandle((HANDLE)file);
    }

    I32 GetSize(File file)
    {
        return (I32)::GetFileSize((HANDLE)file, 0);
    }

    I32 GetSize(String path)
    {
        File file = Open(path, FileModes::Read);
        I32  size = GetSize(file);
        Close(file);

        return size;
    }

    I64 GetSize64(File file)
    {
        LARGE_INTEGER size;
        if (::GetFileSizeEx((HANDLE)file, &size))
        {
            return size.QuadPart;
        }
        else
        {
            return 0;
        }
    }

    I64 GetSize64(String path)
    {
        File  file = Open(path, FileModes::Read);
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
        if (::ReadFile((HANDLE)file, buffer, (DWORD)length, &readBytes, 0))
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
        if (::WriteFile((HANDLE)file, buffer, (DWORD)length, &writeBytes, 0))
        {
            return (I32)writeBytes;
        }
        else
        {
            return 0;
        }
    }
}
