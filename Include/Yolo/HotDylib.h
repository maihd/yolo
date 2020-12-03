#pragma once

#include <Yolo/Core.h>

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

/**
* Open an hot dynamic library, path can be not exists from open moment
*/
HotDylib*       OpenHotDylib(const char* path, const char* entryName);

/**
 * Close opened files by hot dynamic library
 */
void            CloseHotDylib(HotDylib* lib);

/**
 * Update lib, check for changed library and reload
 */
HotDylibState   HotDylibUpdate(HotDylib* lib);

/**
 * Get an symbol address from library with symbol's name
 */
void*           HotDylibGetSymbol(const HotDylib* lib, const char* symbolName);

/**
 * Get error message of hot dynamic library from last update
 */
String          HotDylibGetError(const HotDylib* lib);
