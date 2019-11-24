#include <Yolo/Window.h>
#include <Yolo/String.h>

#include "./Runtime.h"

namespace Window
{
    constexpr string windowClassName = "className";

    static LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    bool RegisterWindowClass(void)
    {
        static bool isRegistered = false;
        if (!isRegistered)
        {
            WNDCLASSA wndclass      = {};
            wndclass.hInstance      = GetModuleHandleA(NULL);
            wndclass.lpszClassName  = windowClassName;
            wndclass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wndclass.lpfnWndProc    = WindowProc;

            if (!::RegisterClassA(&wndclass))
            {
                return false;
            }

            isRegistered = true;
        }

        return true;
    }

    bool Init(string title, int width, int height)
    {
        if (!RegisterWindowClass())
        {
            return false;
        }

        int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        int x = (screenWidth - width) >> 1;
        int y = (screenHeight - height) >> 1;

        HWND window = CreateWindowA(windowClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, x, y, width, height, NULL, NULL, GetModuleHandleA(NULL), NULL);
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
        DestroyWindow(Runtime::mainWindow);
        Runtime::mainWindow = NULL;
    }

    bool PollEvents(void)
    {
        if (Runtime::shouldQuit)
        {
            return true;
        }

        MSG msg = {};
        while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessageA(&msg);

            if (msg.message == WM_QUIT)
            {
                Runtime::shouldQuit = true;
                return true;
            }
        }

        return false;
    }

    void SwapBuffer(void)
    {
        HDC hdc = GetDC(Runtime::mainWindow);
        ::SwapBuffers(hdc);
    }

    LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        //case WM_QUIT:
        //    Runtime::shouldQuit = true;
        //    break;
        }

        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }

    bool ShouldQuit(void)
    {
        return Runtime::shouldQuit;
    }

    int GetWidth(void)
    {
        RECT rect;
        if (GetClientRect(Runtime::mainWindow, &rect))
        {
            return (int)rect.right - rect.left;
        }
        else
        {
            return 0;
        }
    }

    int GetHeight(void)
    {
        RECT rect;
        if (GetClientRect(Runtime::mainWindow, &rect))
        {
            return (int)rect.bottom - rect.top;
        }
        else
        {
            return 0;
        }
    }
}