#pragma once

#include <Yolo/Types.h>

/**
 * HotDylibState
 */
enum struct HotDylibState
{
    None,
    Init,
    Quit,
    Unload,
    Reload,
    Failed,
};

/**
 * HotDylib error code
 */
enum struct HotDylibError
{
    None,
    Abort,
    Float,
    Illcode,
    Syscall,
    Misalign,
    Segfault,
    OutOfBound,
    StackOverflow,
};

/** 
 * HotDylib
 */
struct HotDylib
{
    HotDylibState   state;
    HotDylibError   error;
    void*           userdata;
    String          entryName;
};

namespace HotDylibOps
{
    /**
     * Open an hot dynamic library, path can be not exists from open moment
     */
    HotDylib*       Open(String path, String entryName);

    /**
     * Close opened files by hot dynamic library
     */
    void            Close(HotDylib* lib);

    /**
     * Update lib, check for changed library and reload
     */
    HotDylibState   Update(HotDylib* lib);

    /**
     * Get an symbol address from library with symbol's name
     */
    void*           GetSymbol(const HotDylib* lib, String symbolName);

    /**
     * Get error message of hot dynamic library from last update
     */
    String          GetError(const HotDylib* lib);
}
