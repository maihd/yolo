#include <Yolo/Window.h>
#include <Yolo/String.h>

#include "./Runtime.h"

#pragma comment(lib, "OpenGL32.lib")

namespace Window
{
    static KeyCode s_keyCodeMap[2048];
    static KeyCode ConvertKeyCode(int nativeKey)
    {
        static bool s_loaded;
        if (s_loaded)
        {
            return s_keyCodeMap[nativeKey];
        }
        else
        {
            s_loaded = true;

            s_keyCodeMap[VK_SPACE]      = KeyCode::Space;
            s_keyCodeMap[VK_BACK]       = KeyCode::Backspace;
            s_keyCodeMap[VK_RETURN]     = KeyCode::Return;
            s_keyCodeMap[VK_TAB]        = KeyCode::Tab;

            s_keyCodeMap[VK_UP]         = KeyCode::UpArrow;
            s_keyCodeMap[VK_DOWN]       = KeyCode::DownArrow;
            s_keyCodeMap[VK_LEFT]       = KeyCode::LeftArrow;
            s_keyCodeMap[VK_RIGHT]      = KeyCode::RightArrow;

            s_keyCodeMap[VK_HOME]       = KeyCode::Home;
            s_keyCodeMap[VK_END]        = KeyCode::End;
            s_keyCodeMap[VK_PRIOR]      = KeyCode::PageUp;
            s_keyCodeMap[VK_NEXT]       = KeyCode::PageDown;
            s_keyCodeMap[VK_INSERT]     = KeyCode::Insert;
            s_keyCodeMap[VK_DELETE]     = KeyCode::Delete;

            s_keyCodeMap[VK_LCONTROL]   = KeyCode::LeftControl;
            s_keyCodeMap[VK_RCONTROL]   = KeyCode::RightControl;

            s_keyCodeMap[VK_LSHIFT]     = KeyCode::LeftShift;
            s_keyCodeMap[VK_RSHIFT]     = KeyCode::RightShift;

            s_keyCodeMap[VK_MULTIPLY]   = KeyCode::NumpadMultiply;
            s_keyCodeMap[VK_DIVIDE]     = KeyCode::NumpadDivide;
            s_keyCodeMap[VK_ADD]        = KeyCode::NumpadAdd;
            s_keyCodeMap[VK_SUBTRACT]   = KeyCode::NumpadSubtract;
            s_keyCodeMap[VK_NUMPAD0]    = KeyCode::Numpad0;
            s_keyCodeMap[VK_NUMPAD1]    = KeyCode::Numpad1;
            s_keyCodeMap[VK_NUMPAD2]    = KeyCode::Numpad2;
            s_keyCodeMap[VK_NUMPAD3]    = KeyCode::Numpad3;
            s_keyCodeMap[VK_NUMPAD4]    = KeyCode::Numpad4;
            s_keyCodeMap[VK_NUMPAD5]    = KeyCode::Numpad5;
            s_keyCodeMap[VK_NUMPAD6]    = KeyCode::Numpad6;
            s_keyCodeMap[VK_NUMPAD7]    = KeyCode::Numpad7;
            s_keyCodeMap[VK_NUMPAD8]    = KeyCode::Numpad8;
            s_keyCodeMap[VK_NUMPAD9]    = KeyCode::Numpad9;

            s_keyCodeMap[VK_OEM_PERIOD] = KeyCode::Period;
            s_keyCodeMap[VK_OEM_PLUS]   = KeyCode::Plus;
            s_keyCodeMap[VK_OEM_MINUS]  = KeyCode::Minus;
            s_keyCodeMap[VK_OEM_COMMA]  = KeyCode::Comma;
            //s_keyCodeMap[vk_]

            s_keyCodeMap['0']           = KeyCode::Keypad0;
            s_keyCodeMap['1']           = KeyCode::Keypad1;
            s_keyCodeMap['2']           = KeyCode::Keypad2;
            s_keyCodeMap['3']           = KeyCode::Keypad3;
            s_keyCodeMap['4']           = KeyCode::Keypad4;
            s_keyCodeMap['5']           = KeyCode::Keypad5;
            s_keyCodeMap['6']           = KeyCode::Keypad6;
            s_keyCodeMap['7']           = KeyCode::Keypad7;
            s_keyCodeMap['8']           = KeyCode::Keypad8;
            s_keyCodeMap['9']           = KeyCode::Keypad9;

            //s_keyCodeMap['A']           = KeyCode::A;
            //s_keyCodeMap['B']           = KeyCode::B;
            //s_keyCodeMap['C']           = KeyCode::C;
            //s_keyCodeMap['D']           = KeyCode::D;
            //s_keyCodeMap['E']           = KeyCode::E;
            //s_keyCodeMap['G']           = KeyCode::G;
            //s_keyCodeMap['H']           = KeyCode::H;
            //s_keyCodeMap['I']           = KeyCode::I;
            //s_keyCodeMap['J']           = KeyCode::J;
            //s_keyCodeMap['K']           = KeyCode::K;
            //s_keyCodeMap['L']           = KeyCode::L;
            //s_keyCodeMap['M']           = KeyCode::M;
            //s_keyCodeMap['O']           = KeyCode::O;
            //s_keyCodeMap['P']           = KeyCode::P;
            //s_keyCodeMap['R']           = KeyCode::R;
            //s_keyCodeMap['S']           = KeyCode::S;
            //s_keyCodeMap['T']           = KeyCode::T;
            //s_keyCodeMap['U']           = KeyCode::U;
            //s_keyCodeMap['V']           = KeyCode::V;
            //s_keyCodeMap['W']           = KeyCode::W;
            //s_keyCodeMap['X']           = KeyCode::X;
            //s_keyCodeMap['Y']           = KeyCode::Y;
            //s_keyCodeMap['Z']           = KeyCode::Z;

            s_keyCodeMap['a']           = KeyCode::A;
            s_keyCodeMap['b']           = KeyCode::B;
            s_keyCodeMap['c']           = KeyCode::C;
            s_keyCodeMap['d']           = KeyCode::D;
            s_keyCodeMap['e']           = KeyCode::E;
            s_keyCodeMap['g']           = KeyCode::G;
            s_keyCodeMap['h']           = KeyCode::H;
            s_keyCodeMap['i']           = KeyCode::I;
            s_keyCodeMap['j']           = KeyCode::J;
            s_keyCodeMap['k']           = KeyCode::K;
            s_keyCodeMap['l']           = KeyCode::L;
            s_keyCodeMap['m']           = KeyCode::M;
            s_keyCodeMap['o']           = KeyCode::O;
            s_keyCodeMap['p']           = KeyCode::P;
            s_keyCodeMap['r']           = KeyCode::R;
            s_keyCodeMap['s']           = KeyCode::S;
            s_keyCodeMap['t']           = KeyCode::T;
            s_keyCodeMap['u']           = KeyCode::U;
            s_keyCodeMap['v']           = KeyCode::V;
            s_keyCodeMap['w']           = KeyCode::W;
            s_keyCodeMap['x']           = KeyCode::X;
            s_keyCodeMap['y']           = KeyCode::Y;
            s_keyCodeMap['z']           = KeyCode::Z;

            //s_keyCodeMap[','] = KeyCode::Comma;
            //s_keyCodeMap[':'] = KeyCode::Colon;
            s_keyCodeMap[';'] = KeyCode::SemiColon;
            //s_keyCodeMap['*'] = KeyCode::Aterisk;
            //s_keyCodeMap['&'] = KeyCode::Ampersand;
            s_keyCodeMap['`'] = KeyCode::BackQuote;
            //s_keyCodeMap['~'] = KeyCode::Tilde;
            //s_keyCodeMap['%'] = KeyCode::Percent;
            //s_keyCodeMap['$'] = KeyCode::Dolla;
            //s_keyCodeMap['#'] = KeyCode::Hash;
            //s_keyCodeMap['@'] = KeyCode::At;
            //s_keyCodeMap['!'] = KeyCode::Exclaim;
            //s_keyCodeMap['?'] = KeyCode::Question;
            //s_keyCodeMap['^'] = KeyCode::Caret;
            //s_keyCodeMap['<'] = KeyCode::Less;
            //s_keyCodeMap['='] = KeyCode::Equal;
            //s_keyCodeMap['>'] = KeyCode::Greater;

            //s_keyCodeMap['('] = KeyCode::LeftParen;
            //s_keyCodeMap[')'] = KeyCode::RightParen;
            s_keyCodeMap['['] = KeyCode::LeftBracket;
            s_keyCodeMap[']'] = KeyCode::RightBracket;
            //s_keyCodeMap['{'] = KeyCode::LeftCurlyBracket;
            //s_keyCodeMap['}'] = KeyCode::RightCurlyBracket;

            //s_keyCodeMap['\''] = KeyCode::Quote;
            //s_keyCodeMap['\"'] = KeyCode::DoubleQuote;

            //s_keyCodeMap['\/'] = KeyCode::Slash;
            s_keyCodeMap['\\'] = KeyCode::BackSlash;

            return s_keyCodeMap[nativeKey];
        }
    }

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
        Input::NewFrame();

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
                break;
            }
        }

        Input::EndFrame();
        return Runtime::shouldQuit;
    }

    void SwapBuffer(void)
    {
        ::SwapBuffers(Runtime::mainWindowContext);
    }

    LRESULT WINAPI WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_LBUTTONUP:
            Input::UpdateMouse(MouseButton::Left, false);
            return 0;

        case WM_LBUTTONDOWN:
            Input::UpdateMouse(MouseButton::Left, true);
            return 0;

        case WM_RBUTTONUP:
            Input::UpdateMouse(MouseButton::Right, false);
            return 0;

        case WM_RBUTTONDOWN:
            Input::UpdateMouse(MouseButton::Right, true);
            return 0;

        case WM_MBUTTONUP:
            Input::UpdateMouse(MouseButton::Middle, false);
            return 0;

        case WM_MBUTTONDOWN:
            Input::UpdateMouse(MouseButton::Middle, true);
            return 0;

        case WM_XBUTTONUP:
            Input::UpdateMouse(MouseButton::XButton1, false);
            return 0;

        case WM_XBUTTONDOWN:
            Input::UpdateMouse(MouseButton::XButton1, true);
            return 0;

        case WM_MOUSEMOVE:
            Input::UpdateMouseMove((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
            return 0;

        case WM_MOUSEWHEEL:
            Input::UpdateMouseWheel(0, (float)GET_WHEEL_DELTA_WPARAM(wparam));
            return 0;

        case WM_MOUSEHWHEEL:
            Input::UpdateMouseWheel((float)GET_WHEEL_DELTA_WPARAM(wparam), 0);
            return 0;

        case WM_KEYUP:
            Input::UpdateKey(ConvertKeyCode((int)wparam), false);
            return 0;

        case WM_KEYDOWN:
            Input::UpdateKey(ConvertKeyCode((int)wparam), true);
            return 0;

        case WM_CHAR:
        case WM_UNICHAR:
            Input::UpdateCharInput((int)wparam);
            return 0;

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

    vec2 GetSize(void)
    {
        RECT rect;
        if (GetClientRect(Runtime::mainWindow, &rect))
        {
            return { (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
        }
        else
        {
            return {};
        }
    }

    int GetWidth(void)
    {
        RECT rect;
        if (GetClientRect(Runtime::mainWindow, &rect))
        {
            return (int)(rect.right - rect.left);
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
            return (int)(rect.bottom - rect.top);
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