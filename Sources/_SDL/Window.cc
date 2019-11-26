#include <Yolo/Window.h>
#include <Yolo/String.h>

#include "./Runtime.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "OpenGL32.lib")

namespace Window
{
    bool Init(string title, int width, int height)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) 
        {
            return false;
        }

        SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
        if (!window)
        {
            return false;
        }

        Runtime::mainWindow = window;
        Runtime::shouldQuit = false;
        return true;
    }

    void Quit(void)
    {
        SDL_DestroyWindow(Runtime::mainWindow);
        Runtime::mainWindow = NULL;
    }

    bool PollEvents(void)
    {
        if (Runtime::shouldQuit)
        {
            return true;
        }

        SDL_Event event = {};
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                Runtime::shouldQuit = true;
                return true;
            }
        }

        return false;
    }

    void SwapBuffer(void)
    {
        SDL_GL_SwapWindow(Runtime::mainWindow);
    }

    bool ShouldQuit(void)
    {
        return Runtime::shouldQuit;
    }

    int GetWidth(void)
    {
        int width = 0;
        SDL_GetWindowSize(Runtime::mainWindow, &width, 0);
        return width;
    }

    int GetHeight(void)
    {
        int height = 0;
        SDL_GetWindowSize(Runtime::mainWindow, &height, 0);
        return height;
    }

    Handle GetHandle(void)
    {
        return (Handle)(intptr_t)Runtime::mainWindow;
    }
}