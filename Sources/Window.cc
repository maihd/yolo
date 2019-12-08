#include <Yolo/Window.h>
#include <Yolo/String.h>

#include "./Runtime.h"

#pragma comment(lib, "OpenGL32.lib")

namespace Window
{
    constexpr TCHAR* WINDOW_CLASS_NAME = TEXT("YOLO_WINDOW_CLASS");

    static LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    bool RegisterWindowClass(void)
    {
        static bool isRegistered = false;
        if (!isRegistered)
        {
            WNDCLASS wndclass       = {};
            wndclass.hInstance      = GetModuleHandle(NULL);
            wndclass.lpszClassName  = WINDOW_CLASS_NAME;
            wndclass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            wndclass.lpfnWndProc    = WindowProc;

            if (!::RegisterClass(&wndclass))
            {
                return false;
            }

            isRegistered = true;
        }

        return true;
    }

    bool Init(String title, int width, int height)
    {
        if (!RegisterWindowClass())
        {
            return false;
        }

#if UNICODE
        TCHAR unicodeTitle[1024];
        ::MultiByteToWideChar(CP_UTF8, 0, title, -1, unicodeTitle, sizeof(unicodeTitle));
#else
        TCHAR* unicodeTitle = title;
#endif

        DWORD winFlags = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE;
        HWND window = CreateWindow(
            WINDOW_CLASS_NAME, 
            unicodeTitle, 
            winFlags,
            0, 0, 
            width, height,
            NULL, NULL, GetModuleHandle(NULL), NULL
        );
        if (!window)
        {
            return false;
        }

        HDC hdc = GetDC(window);
        if (!hdc)
        {
            return false;
        }

        RECT windowRect;
        RECT clientRect;
        GetWindowRect(window, &windowRect);
        GetClientRect(window, &clientRect);

        int screenWidth     = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight    = GetSystemMetrics(SM_CYSCREEN);

        int borderWidth     = (int)((windowRect.right - windowRect.left) - (clientRect.right - clientRect.left));
        int borderHeight    = (int)((windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top));

        int resultWidth     = width + borderWidth;
        int resultHeight    = height + borderHeight;

        int centerX         = (screenWidth - resultWidth) >> 1;
        int centerY         = (screenHeight - resultHeight) >> 1;

        SetWindowPos(window, HWND_TOP, centerX, centerY, resultWidth, resultHeight, SWP_NOZORDER);

        Runtime::mainWindowContext = hdc;
        Runtime::mainWindow = window;
        Runtime::shouldQuit = false;
        return true;
    }

    void Quit(void)
    {
        DestroyWindow(Runtime::mainWindow);
    }

    bool PollEvents(void)
    {
        if (Runtime::shouldQuit)
        {
            return true;
        }

        MSG msg = {};
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

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
        ::SwapBuffers(Runtime::mainWindowContext);
    }

    LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            if (hwnd == Runtime::mainWindow)
            {
                Runtime::mainWindowContext = NULL;
                Runtime::mainWindow = NULL;

                PostQuitMessage(0);
            }

            return 0;
        }

        return DefWindowProc(hwnd, msg, wparam, lparam);
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

    Handle GetHandle(void)
    {
        return (Handle)(uint64)Runtime::mainWindow;
    }
}