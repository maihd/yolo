#include <Yolo/Window.h>
#include <Yolo/String.h>

#include "./Runtime.h"

#ifdef _WIN32
#   pragma comment(lib, "OpenGL32.lib")
#   pragma comment(lib, "SetupAPI.lib")
#   pragma comment(lib, "Version.lib")
#   pragma comment(lib, "Imm32.lib")
#   pragma comment(lib, "Winmm.lib")
#endif

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

bool OpenWindow(String title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        return false;
    }

    DWORD winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window = SDL_CreateWindow(title.buffer, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, winFlags);
    if (!window)
    {
        return false;
    }

    Runtime::mainWindow = window;
    Runtime::shouldQuit = false;
    return true;
}

void CloseWindow(void)
{
    SDL_DestroyWindow(Runtime::mainWindow);
    Runtime::mainWindow = nullptr;
}

bool WindowPollEvents(void)
{
    Input::NewFrame();

    if (Runtime::shouldQuit)
    {
        return true;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Runtime::shouldQuit = true;
            break;
        }
    }

    Input::EndFrame();
    return Runtime::shouldQuit;
}

void WindowSwapBuffer(void)
{
    SDL_GL_SwapWindow(Runtime::mainWindow);
}

bool ShouldWindowQuit(void)
{
    return Runtime::shouldQuit;
}

Vector2 WindowSize(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime::mainWindow, &width, &height);
    return { (float)width, (float)height };
}

int WindowWidth(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime::mainWindow, &width, &height);
    return width;
}

int WindowHeight(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime::mainWindow, &width, &height);
    return height;
}

Handle GetWindowHandle(void)
{
    return (Handle)(U64)Runtime::mainWindow;
}
