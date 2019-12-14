/**************************************************************
 * HotDylib - Hot reload dynamic library from memory and file *
 *                                                            *
 **************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#ifndef HOTDYLIB_PDB_UNLOCK
#define HOTDYLIB_PDB_UNLOCK 1
#endif

#define HOTDYLIB_MAX_PATH   1024
#define HotDylib_CountOf(x) (sizeof(x) / sizeof((x)[0]))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <setjmp.h>
#include <signal.h>

#include <Yolo/HotDylib.h>

#if defined(__MINGW32__) || (defined(_WIN32) && defined(__clang__))
#   define HOTDYLIB_TRY(lib)      if (SEHBegin(lib))
#   define HOTDYLIB_EXCEPT(lib)   else
#   define HOTDYLIB_FINALLY(lib)  SEHEnd(lib); if (1)
#elif (__unix__) || defined(__linux__) || defined(__APPLE__)
#   define HOTDYLIB_TRY(lib)      if (SEHBegin(lib) && sigsetjmp((lib)->jumpPoint) == 0)
#   define HOTDYLIB_EXCEPT(lib)   else
#   define HOTDYLIB_FINALLY(lib)  SEHEnd(lib); if (1)
#endif

struct HotDylibData
{
    void* library;

    long  libtime;
    char  libRealPath[HOTDYLIB_MAX_PATH];
    char  libTempPath[HOTDYLIB_MAX_PATH];

#if defined(_MSC_VER) && HOTDYLIB_PDB_UNLOCK
    long  pdbtime;

    char  pdbRealPath[HOTDYLIB_MAX_PATH];
    char  pdbTempPath[HOTDYLIB_MAX_PATH];
#endif
};

/* Define dynamic library loading API */
#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>

#   define Dylib_Load(path)         (void*)LoadLibraryA(path)
#   define Dylib_Free(lib)          FreeLibrary((HMODULE)lib)
#   define Dylib_GetSymbol(l, n)    (void*)GetProcAddress((HMODULE)l, n)

static String Dylib_GetError(void)
{
    static int  error;
    static char buffer[256];

    int last_error = GetLastError();
    if (last_error != error)
    {
        error = last_error;
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, last_error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buffer, sizeof(buffer), NULL);
    }
    return buffer;
}
#elif (__unix__)
#  include <dlfcn.h>
#  define Dylib_Load(path)          dlopen(path, RTLD_LAZY)
#  define Dylib_Free(lib)           dlclose(lib)
#  define Dylib_GetSymbol(l, n)     dlsym(l, n)
#  define Dylib_GetError()          dlerror()
#else
#  error "Unsupported platform"
#endif

namespace HotDylibOps
{
    /* Undocumented, should not call by hand */
    static bool SEHBegin(HotDylib* lib);

    /* Undocumented, should not call by hand */
    static void SEHEnd(HotDylib* lib);


/** Custom helper functions **/
#if defined(_WIN32)

    static long GetLastModifyTime(String path)
    {
        WIN32_FILE_ATTRIBUTE_DATA fad;
        if (!GetFileAttributesExA(path, GetFileExInfoStandard, &fad))
        {
            return 0;
        }

        LARGE_INTEGER time;
        time.LowPart = fad.ftLastWriteTime.dwLowDateTime;
        time.HighPart = fad.ftLastWriteTime.dwHighDateTime;

        return (long)(time.QuadPart / 10000000L - 11644473600L);
    }

    #undef CopyFile
    static bool CopyFile(String from, String to)
    {
        if (CopyFileA(from, to, false))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    #undef RemoveFile
    static bool RemoveFile(String path);

#if defined(_MSC_VER)

#  if defined(HOTDYLIB_PDB_UNLOCK) || defined(HOTDYLIB_PDB_DELETE)
#    include <winternl.h>
#    include <RestartManager.h> 
#    pragma comment(lib, "ntdll.lib")
#    pragma comment(lib, "rstrtmgr.lib")

#    define NTSTATUS_SUCCESS              ((NTSTATUS)0x00000000L)
#    define NTSTATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xc0000004L)

    typedef struct
    {
        UNICODE_STRING Name;
    } OBJECT_INFORMATION;

    static void UnlockFileFromProcess(ULONG pid, const WCHAR* file)
    {
        const OBJECT_INFORMATION_CLASS ObjectNameInformation = (OBJECT_INFORMATION_CLASS)1;
        const OBJECT_INFORMATION_CLASS ObjectTypeInformation = (OBJECT_INFORMATION_CLASS)2;

        // Make sure the process is valid
        HANDLE hCurProcess = GetCurrentProcess();
        HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!hProcess)
        {
            return;
        }

        WCHAR volumeName[8];
        GetVolumePathNameW(file, volumeName, sizeof(volumeName));

        volumeName[2] = 0;  /* Remove '\\' */
        WCHAR prefix[1024];
        QueryDosDeviceW(volumeName, prefix, sizeof(prefix));
        wcscat(prefix, L"\\");
        size_t prefixLength = wcslen(prefix);

        DWORD handleIter, handleCount;
        GetProcessHandleCount(hProcess, &handleCount);
        for (handleIter = 0, handleCount *= 16; handleIter <= handleCount; handleIter += 4)
        {
            HANDLE handle = (HANDLE)handleIter;

            HANDLE hCopy; // Duplicate the handle in the current process
            if (!DuplicateHandle(hProcess, handle, hCurProcess, &hCopy, 0, FALSE, DUPLICATE_SAME_ACCESS))
            {
                continue;
            }

            const char ObjectBuffer[sizeof(OBJECT_INFORMATION) + 512] = {};
            OBJECT_INFORMATION* pobj = (OBJECT_INFORMATION*)ObjectBuffer;

            if (NtQueryObject(hCopy, ObjectNameInformation, pobj, sizeof(ObjectBuffer), NULL) != NTSTATUS_SUCCESS)
            {
                CloseHandle(hCopy);
                continue;
            }

            if (!pobj->Name.Buffer)
            {
                CloseHandle(hCopy);
                continue;
            }

            if (wcsncmp(pobj->Name.Buffer, prefix, prefixLength) == 0)
            {
                WCHAR path0[HOTDYLIB_MAX_PATH];
                WCHAR path1[HOTDYLIB_MAX_PATH];

                swscanf(pobj->Name.Buffer + prefixLength, L"%s", path0);

                wsprintfW(path1, L"%s\\%s", volumeName, path0);

                if (wcscmp(path1, file) == 0)
                {
                    HANDLE hForClose;
                    DuplicateHandle(hProcess, handle, hCurProcess, &hForClose, MAXIMUM_ALLOWED, false, DUPLICATE_CLOSE_SOURCE);
                    CloseHandle(hForClose);
                    CloseHandle(hCopy);
                    break;
                }
            }

            CloseHandle(hCopy);
        }

        CloseHandle(hProcess);
    }

    static void UnlockPdbFile(HotDylibData* lib, String file)
    {
        WCHAR           szFile[HOTDYLIB_MAX_PATH + 1];
        UINT            i;
        UINT            nProcInfoNeeded;
        UINT            nProcInfo = 10;
        DWORD           dwError;
        DWORD           dwReason;
        DWORD           dwSession;
        RM_PROCESS_INFO rgpi[10];
        WCHAR           szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };

        MultiByteToWideChar(CP_UTF8, 0, file, -1, szFile, HOTDYLIB_MAX_PATH);

        dwError = RmStartSession(&dwSession, 0, szSessionKey);
        if (dwError == ERROR_SUCCESS)
        {
            const WCHAR* szFiles = szFile;

            dwError = RmRegisterResources(dwSession, 1, &szFiles, 0, NULL, 0, NULL);
            if (dwError == ERROR_SUCCESS)
            {
                dwError = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rgpi, &dwReason);
                if (dwError == ERROR_SUCCESS)
                {
                    for (i = 0; i < nProcInfo; i++)
                    {
                        UnlockFileFromProcess(rgpi[i].Process.dwProcessId, szFile);
                    }
                }
            }
            RmEndSession(dwSession);
        }
    }

    static int GetPdbPath(String libpath, char* buf, int len)
    {
        char drv[8];
        char dir[HOTDYLIB_MAX_PATH];
        char name[HOTDYLIB_MAX_PATH];

        GetFullPathNameA(libpath, len, buf, NULL);
        _splitpath(buf, drv, dir, name, NULL);

        return snprintf(buf, len, "%s%s%s.pdb", drv, dir, name);
    }
#  endif /* HOTDYLIB_PDB_UNLOCK */

#if HOTDYLIB_USE_SEH
    static int SEHFilter(HotDylib* lib, int excode)
    {
        int errcode = HotDylibError::None;

        switch (excode)
        {
        case EXCEPTION_FLT_OVERFLOW:
        case EXCEPTION_FLT_UNDERFLOW:
        case EXCEPTION_FLT_STACK_CHECK:
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_FLT_INEXACT_RESULT:
        case EXCEPTION_FLT_DENORMAL_OPERAND:
        case EXCEPTION_FLT_INVALID_OPERATION:
            errcode = HOTDYLIB_ERROR_FLOAT;
            break;

        case EXCEPTION_ACCESS_VIOLATION:
            errcode = HOTDYLIB_ERROR_SEGFAULT;
            break;

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            errcode = HOTDYLIB_ERROR_ILLCODE;
            break;

        case EXCEPTION_DATATYPE_MISALIGNMENT:
            errcode = HOTDYLIB_ERROR_MISALIGN;
            break;

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            errcode = HOTDYLIB_ERROR_OUTBOUNDS;
            break;

        case EXCEPTION_STACK_OVERFLOW:
            errcode = HOTDYLIB_ERROR_STACKOVERFLOW;
            break;

        default:
            break;
        }

        if (lib) lib->errcode = errcode;
        int rc = errcode != HotDylibError::None;
        return rc;
    }
#else
    typedef struct SehFilter
    {
        int                             ref;
        HotDylib*                       lib;
        LPTOP_LEVEL_EXCEPTION_FILTER    oldHandler;
    } SehFilter;

    static SehFilter    s_filterStack[128];
    static int          s_filterStackPointer = -1;

    /* Undocumented, should not call by hand */
    static HotDylibError SEHFilter(HotDylib* lib, int exception)
    {
        HotDylibError error = HotDylibError::None;

        switch (exception)
        {
        case EXCEPTION_FLT_OVERFLOW:
        case EXCEPTION_FLT_UNDERFLOW:
        case EXCEPTION_FLT_STACK_CHECK:
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        case EXCEPTION_FLT_INEXACT_RESULT:
        case EXCEPTION_FLT_DENORMAL_OPERAND:
        case EXCEPTION_FLT_INVALID_OPERATION:
            error = HotDylibError::Float;
            break;

        case EXCEPTION_ACCESS_VIOLATION:
            error = HotDylibError::Segfault;
            break;

        case EXCEPTION_ILLEGAL_INSTRUCTION:
            error = HotDylibError::Illcode;
            break;

        case EXCEPTION_DATATYPE_MISALIGNMENT:
            error = HotDylibError::Misalign;
            break;

        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            error = HotDylibError::OutOfBound;
            break;

        case EXCEPTION_STACK_OVERFLOW:
            error = HotDylibError::StackOverflow;
            break;

        default:
            break;
        }

        if (lib) lib->error = error;
        return error;
    }

    static LONG WINAPI SignalHandler(EXCEPTION_POINTERS* info)
    {
        assert(s_filterStackPointer > -1);

        int exception = (int)info->ExceptionRecord->ExceptionCode;
        HotDylibError error = SEHFilter(NULL, exception);

        return error != HotDylibError::None;
    }

    bool SEHBegin(HotDylib* lib)
    {
        assert(lib);

        if (s_filterStack[s_filterStackPointer].lib == lib)
        {
            s_filterStack[s_filterStackPointer].ref++;
        }
        else
        {
            assert(s_filterStackPointer < (int)HotDylib_CountOf(s_filterStack));

            SehFilter* filter = &s_filterStack[++s_filterStackPointer];
            filter->ref = 0;
            filter->lib = lib;
            filter->oldHandler = SetUnhandledExceptionFilter(SignalHandler);
        }

        return true;
    }

    void SEHEnd(HotDylib* lib)
    {
        assert(lib);
        assert(s_filterStackPointer > -1 && s_filterStack[s_filterStackPointer].lib == lib);

        SehFilter* filter = &s_filterStack[s_filterStackPointer];
        if (--filter->ref <= 0)
        {
            s_filterStackPointer--;
            SetUnhandledExceptionFilter(filter->oldHandler);
        }
    }
#endif

# endif /* _MSC_VER */

#elif defined(__unix__)
#   include <sys/stat.h>
#   include <sys/types.h>

    const int HotDylib_Signals[] = { SIGBUS, SIGSYS, SIGILL, SIGSEGV, SIGABRT };

    static long GetLastModifyTime(String path)
    {
        struct stat st;
        if (stat(path, &st) != 0)
        {
            return 0;
        }

        return (long)st.st_mtime;
    }

    static int CopyFile(String from_path, String to_path)
    {
        char scmd[3 * PATH_MAX]; /* 2 path and command */
        sprintf(scmd, "cp -fR %s %s", from_path, to_path);
        if (system(scmd) != 0)
        {
            /* Has an error occur */
            return 0;
        }
        else
        {
            return 1;
        }
    }

    static void SignalHandler(int code, siginfo_t* info, void* context)
    {
        assert(s_filterStackPointer > -1);

        int errcode;

        (void)info;
        (void)context;

        switch (code)
        {
        case SIGILL:
            errcode = HOTDYLIB_ERROR_ILLCODE;
            break;

        case SIGBUS:
            errcode = HOTDYLIB_ERROR_MISALIGN;
            break;

        case SIGSYS:
            errcode = HOTDYLIB_ERROR_SYSCALL;
            break;

        case SIGABRT:
            errcode = HOTDYLIB_ERROR_ABORT;
            break;

        case SIGSEGV:
            errcode = HOTDYLIB_ERROR_SEGFAULT;
            break;

        default:
            errcode = HotDylibError::None;
            break;
        }

        siglongjmp(s_filterStack[s_filterStackPointer].lib->jumpPoint, errcode);
    }


    bool SEHBegin(HotDylib* lib)
    {
        if (s_filterStackPointer < 0)
        {
            struct sigaction sa;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_NODEFER;
            sa.sa_handler = NULL;
            sa.sa_sigaction = SignalHandler;

            int idx;
            for (idx = 0; idx < HotDylib_CountOf(HotDylib_Signals); idx++)
            {
                if (sigaction(HotDylib_Signals[idx], &sa, NULL) != 0)
                {
                    return false;
                }
            }

            SehFilter* filter = &s_filterStack[++s_filterStackPointer];
            filter->lib = lib;
        }
        return true;
    }

    void SEHEnd(HotDylib* lib)
    {
        int idx;
        for (idx = 0; idx < HotDylib_CountOf(HotDylib_Signals); idx++)
        {
            if (signal(HotDylib_Signals[idx], SIG_DFL) != 0)
            {
                assert(false);
            }
        }
    }
#else
#error "Unsupported platform"
#endif

    static bool RemoveFile(String path)
    {
        return DeleteFileA(path);
    }

    #undef GetTempPath
    static int GetTempPath(String path, char* buffer, int length)
    {
        int res;

        if (buffer)
        {
            int version = 0;
            while (1)
            {
                res = snprintf(buffer, length, "%s.%d", path, version++);
                FILE* file = fopen(buffer, "r");
                if (file)
                {
                    fclose(file);
                }
                else
                {
                    break;
                }
            }
        }

        return res;
    }

    static bool CheckChanged(HotDylib* lib)
    {
        HotDylibData* data = (HotDylibData*)(lib + 1);

        long src = data->libtime;
        long cur = GetLastModifyTime(data->libRealPath);
        bool res = cur > src;
#if defined(_MSC_VER) && HOTDYLIB_PDB_UNLOCK
        if (res)
        {
            src = data->pdbtime;
            cur = GetLastModifyTime(data->pdbRealPath);
            res = (cur == src && cur == 0) || cur > src;
        }
#endif
        return res;
    }

    static bool CallEntryFunction(HotDylib* lib, void* library, HotDylibState state)
    {
        typedef void* (*HotDylibEntryFn)(void* userdata, HotDylibState newState, HotDylibState oldState);
        HotDylibEntryFn func = (HotDylibEntryFn)Dylib_GetSymbol(library, lib->entryName);

        bool res = true;
        if (func)
        {
#if defined(_MSC_VER) || (defined(__clang__) && defined(_WIN32))
            __try
            {
                lib->userdata = func(lib->userdata, state, lib->state);
            }
            __except (SEHFilter(lib, GetExceptionCode()) != HotDylibError::None)
            {
                res = false;
            }
#else
            HOTDYLIB_TRY(lib)
            {
                lib->userdata = func(lib->userdata, lib->state, state);
            }
            HOTDYLIB_EXCEPT(lib)
            {
                res = false;
            }
            HOTDYLIB_FINALLY(lib)
            {
                /*null statement*/
            }
#endif
        }

        return res;
    }

    HotDylib* Open(String path, String entryName)
    {
        HotDylib* lib = (HotDylib*)(malloc(sizeof(HotDylib) + sizeof(HotDylibData)));
        if (!lib)
        {
            return NULL;
        }

        lib->state      = HotDylibState::None;
        lib->error      = HotDylibError::None;
        lib->userdata   = NULL;
        lib->entryName  = entryName;

        HotDylibData* data = (HotDylibData*)(lib + 1);
        data->libtime = 0;
        data->library = NULL;
        GetTempPath(path, data->libTempPath, HOTDYLIB_MAX_PATH);

        strncpy(data->libRealPath, path, HOTDYLIB_MAX_PATH);

#if defined(_MSC_VER) && HOTDYLIB_PDB_UNLOCK
        data->pdbtime = 0;
        GetPdbPath(path, data->pdbRealPath, HOTDYLIB_MAX_PATH);
        GetTempPath(data->pdbRealPath, data->pdbTempPath, HOTDYLIB_MAX_PATH);
#endif

        return lib;
    }

    void Close(HotDylib* lib)
    {
        if (lib)
        {
            HotDylibData* data = (HotDylibData*)(lib + 1);
            /* Raise quit event */
            if (data->library)
            {
                lib->state = HotDylibState::Quit;
                CallEntryFunction(lib, data->library, lib->state);
                Dylib_Free(data->library);

                /* Remove temp library */
                RemoveFile(data->libTempPath); /* Ignore error code */
            }

            /* Clean up */
            free(lib);
        }
    }

    HotDylibState Update(HotDylib* lib)
    {
        HotDylibData* data = (HotDylibData*)(lib + 1);

        if (CheckChanged(lib))
        {
            void* library;

            /* Unload old version */
            library = data->library;
            if (library)
            {
                /* Raise unload event */
                lib->state = HotDylibState::Unload;
                CallEntryFunction(lib, library, lib->state);

                /* Collect garbage */
                Dylib_Free(library);
                data->library = NULL;

                if (lib->error != HotDylibError::None)
                {
                    lib->state = HotDylibState::Failed;
                    return lib->state;
                }
                else
                {
                    return lib->state;
                }
            }

            /* Create and load new temp version */
            RemoveFile(data->libTempPath); /* Remove temp library */
            if (CopyFile(data->libRealPath, data->libTempPath))
            {
                library = Dylib_Load(data->libTempPath);
                if (library)
                {
                    HotDylibState state = lib->state == HotDylibState::None ? HotDylibState::Init : HotDylibState::Reload;
                    CallEntryFunction(lib, library, state);

                    data->library = library;
                    data->libtime = GetLastModifyTime(data->libRealPath);

                    if (lib->error != HotDylibError::None)
                    {
                        Dylib_Free(data->library);

                        data->library = NULL;
                        lib->state = HotDylibState::Failed;
                    }
                    else
                    {
                        lib->state = state;

#if defined(_MSC_VER) && HOTDYLIB_PDB_UNLOCK
                        UnlockPdbFile(data, data->pdbRealPath);
                        data->pdbtime = GetLastModifyTime(data->pdbRealPath);
#endif
                    }
                }
            }

            return lib->state;
        }
        else
        {
            if (lib->state != HotDylibState::Failed)
            {
                lib->state = HotDylibState::None;
            }

            return HotDylibState::None;
        }
    }

    void* GetSymbol(const HotDylib* lib, String symbolName)
    {
        HotDylibData* data = (HotDylibData*)(lib + 1);
        return Dylib_GetSymbol(data->library, symbolName);
    }

    String GetError(const HotDylib* lib)
    {
        (void)lib;
        return Dylib_GetError();
    }
}
