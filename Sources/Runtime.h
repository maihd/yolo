#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GL/glew.h>
#include <GL/wglew.h>

namespace Runtime
{
    extern bool shouldQuit;
    extern HWND mainWindow;
    extern HDC  mainWindowContext;
}