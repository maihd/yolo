#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace Runtime
{
    extern bool         shouldQuit;
    extern SDL_Window*  mainWindow;
}