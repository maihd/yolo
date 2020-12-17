#include <Yolo/Core.h>
#include <Yolo/Input.h>
#include <Yolo/Window.h>
#include <Yolo/String.h>

#ifdef _WIN32
#   define VC_EXTRALEAN
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h> // GetProcAddress, NtDelayExecution
#   pragma comment(lib, "OpenGL32.lib")
#   pragma comment(lib, "SetupAPI.lib")
#   pragma comment(lib, "Version.lib")
#   pragma comment(lib, "Imm32.lib")
#   pragma comment(lib, "Winmm.lib")
#elif defined(__unix__)
#   include <unistd.h>  // usleep
#endif

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "./Imgui/imgui_impl_sdl.h"
#include "./Imgui/imgui_impl_opengl3.h"

static struct
{
    bool            ShouldClose;
    bool            ShouldRender;

    SDL_Window*     MainWindow;
    SDL_GLContext   GraphicsContext;
} Runtime;

namespace Input
{
    void Setup(void);

    void NewFrame(void);
    void EndFrame(void);

    void UpdateCharInput(I32 character);
    void UpdateCharInput(String string);

    void UpdateKey(KeyCode key, bool down);

    void UpdateMouse(MouseButton button, bool down);
    void UpdateMouseMove(float x, float y);
    void UpdateMouseWheel(float h, float v);

    void UpdateGamepadAxis(I32 gamepadID, GamepadAxis axis, float value);
    void UpdateGamepadButton(I32 gamepadID, GamepadButton button, bool down);
}

// ------------------------
// Timer internal functions
// ------------------------

static void OpenTimer(int fps);
static void CloseTimer(void);

static void MicroSleep(U64 microseconds);
static void FakeMicroSleep(U64 microseconds);

static void UpdateTimer(void);
static bool UpdateTimerAndSleep(void);

// ------------------------------
// Input internal functions
// ------------------------------

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

        s_keyCodeMap[SDLK_SPACE]    = KeyCode::Space;
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

namespace Graphics
{
    void ApplyDefaultSettings(void);
    void CreateDefaultObjects(void);
}

bool OpenWindow(StringView title, int width, int height, int fps)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        return false;
    }

    DWORD winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* window = SDL_CreateWindow(title.Buffer, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, winFlags);
    if (!window)
    {
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GLContext graphicsContext = SDL_GL_CreateContext(window);
    if (!graphicsContext)
    {
        SDL_DestroyWindow(window);
        return false;
    }

    if (SDL_GL_MakeCurrent(window, graphicsContext) != 0)
    {
        SDL_GL_DeleteContext(graphicsContext);
        SDL_DestroyWindow(window);
        return false;
    }

    glewExperimental = false;
    GLenum glewState = glewInit();
    if (glewState != GLEW_OK)
    {
        SDL_GL_DeleteContext(graphicsContext);
        SDL_DestroyWindow(window);
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    if (!ImGui_ImplSDL2_InitForOpenGL(window, graphicsContext))
    {
        SDL_GL_DeleteContext(graphicsContext);
        SDL_DestroyWindow(window);
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
    {
        SDL_GL_DeleteContext(graphicsContext);
        SDL_DestroyWindow(window);
        return false;
    }

    Runtime.ShouldClose = false;
    Runtime.ShouldRender = false;
    
    Runtime.MainWindow = window;
    Runtime.GraphicsContext = graphicsContext;

    OpenTimer(fps);

    // Default settings
    Graphics::ApplyDefaultSettings();
    Graphics::CreateDefaultObjects();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    ImGui::GetIO().Fonts->AddFontFromFileTTF("../Assets/NotoMono-Regular.ttf", 14);

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    return true;
}

void CloseWindow(void)
{
    CloseTimer();

    SDL_GL_DeleteContext(Runtime.GraphicsContext);
    Runtime.GraphicsContext = nullptr;

    SDL_DestroyWindow(Runtime.MainWindow);
    Runtime.MainWindow = nullptr;

    Runtime.ShouldClose = false;
    Runtime.ShouldRender = false;
}

bool UpdateWindow(void)
{
    if (Runtime.MainWindow == nullptr || Runtime.ShouldClose)
    {
        return true;
    }

    UpdateTimerAndSleep();
    if (Runtime.ShouldRender)
    {
        ImGui::Render();
        ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(Runtime.MainWindow);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            Runtime.ShouldClose = true;
            break;
        }
    }

    if (!Runtime.ShouldClose)
    {
        Runtime.ShouldRender = true;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(Runtime.MainWindow);
        ImGui::NewFrame();
    }
    else
    {
        Runtime.ShouldRender = false;
    }

    return Runtime.ShouldClose;
}

bool IsVSync(void)
{
    return SDL_GL_GetSwapInterval();
}

void SetVSync(bool enable)
{
    SDL_GL_SetSwapInterval(enable);
}

Vector2 WindowSize(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime.MainWindow, &width, &height);
    return { (float)width, (float)height };
}

int WindowWidth(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime.MainWindow, &width, &height);
    return width;
}

int WindowHeight(void)
{
    int width, height;
    SDL_GetWindowSize(Runtime.MainWindow, &width, &height);
    return height;
}

// -----------------------------------------
// Timer system functions
// -----------------------------------------

/// NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);
/// typedef LONG NTSTATUS; =))
/// #define NTAPI __stdcall =))
typedef LONG(__stdcall * NtDelayExecutionFN)(BOOL, PLARGE_INTEGER);

static struct
{
    I32     TotalFrames;
    float   Framerate;
    float   DeltaTime;
    float   TotalTime;
    float   TimeScale;

    float   UpdateFramerateTimer;
    float   UpdateFramerateInterval;

    U64     PrevCounter;

    U64     CachedLimitTicks;
    U64     CachedCpuFrequency;

    void(*SleepFunction)(U64 microseconds);
    NtDelayExecutionFN NtDelayExecution;
} Timer;

void OpenTimer(int fps)
{
    Timer.TotalFrames = 0;
    Timer.Framerate = 0.0f;
    Timer.DeltaTime = 0.0f;
    Timer.TotalTime = 0.0f;
    Timer.TimeScale = 1.0f;

    Timer.UpdateFramerateTimer = 0.0f;
    Timer.UpdateFramerateInterval = 1.0f;

    Timer.PrevCounter = 0;

    Timer.CachedCpuFrequency = SDL_GetPerformanceFrequency();
    Timer.CachedLimitTicks = (U64)(Timer.CachedCpuFrequency / (double)(fps < 1 ? 1000 : fps));

#if defined(_WIN32)
    // Load sleep function
    HMODULE module = GetModuleHandle(TEXT("ntdll.dll"));
    Timer.NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, "NtDelayExecution");
    if (Timer.NtDelayExecution != nullptr)
    {
        Timer.SleepFunction = MicroSleep;
    }
    else
    {
        Timer.SleepFunction = FakeMicroSleep;
    }
#else
    Timer.SleepFunction = MicroSleep;
#endif
}

void CloseTimer(void)
{
    Timer.SleepFunction = nullptr;
    Timer.NtDelayExecution = nullptr;
}

void MicroSleep(U64 microseconds)
{
#if defined(_WIN32)
    LARGE_INTEGER times;
    times.QuadPart = -(LONGLONG)microseconds * 10;
    Timer.NtDelayExecution(FALSE, &times);
#elif defined(__unix__)
    usleep(microseconds);
#else
    U64 counter = GetCpuCounter();
    while (GetCpuCounter() - counter < microseconds)
    {
        // no-op
    }
#endif
}

void FakeMicroSleep(U64 microseconds)
{
    SDL_Delay((Uint32)(microseconds / 1000U));
}

// --------------------------------
// Manage timer state
// --------------------------------

float GetFramerate(void)
{
    return Timer.Framerate;
}

I32 GetTotalFrames(void)
{
    return Timer.TotalFrames;
}

float GetTotalTime(void)
{
    return Timer.TotalTime;
}

float GetTimeScale(void)
{
    return Timer.TimeScale;
}

void SetTimeScale(float timeScale)
{
    Timer.TimeScale = timeScale;
}

float GetDeltaTime(void)
{
    return Timer.DeltaTime * Timer.TimeScale;
}

float GetUnscaledDeltaTime(void)
{
    return Timer.DeltaTime;
}

void UpdateTimer(void)
{
    U64 counter = SDL_GetPerformanceCounter();
    if (Timer.PrevCounter > 0)
    {
        U64 ticks = counter - Timer.PrevCounter;

        Timer.DeltaTime = (float)((double)ticks / (double)Timer.CachedCpuFrequency);
        Timer.TotalTime = Timer.TotalTime + Timer.DeltaTime;

        Timer.UpdateFramerateTimer += Timer.DeltaTime;
        if (Timer.UpdateFramerateTimer >= Timer.UpdateFramerateInterval)
        {
            Timer.UpdateFramerateTimer -= Timer.UpdateFramerateInterval;
            Timer.Framerate = 1.0f / Timer.DeltaTime;
        }

    }

    Timer.TotalFrames++;
    Timer.PrevCounter = counter;
}

bool UpdateTimerAndSleep(void)
{
    bool isSleep = false;
    U64 counter = SDL_GetPerformanceCounter();

    if (Timer.PrevCounter > 0)
    {
        U64 frequency = Timer.CachedCpuFrequency;
        U64 limitTicks = Timer.CachedLimitTicks;

        U64 ticks = Timer.PrevCounter > 0 ? counter - Timer.PrevCounter : limitTicks;
        if (ticks < limitTicks)
        {
            isSleep = true;

            U64 remainTicks = limitTicks - ticks;
            double remainSeconds = ((double)remainTicks) / frequency;
            U64 remainMicroSeconds = (U64)(remainSeconds * 1000000ULL);
            Timer.SleepFunction(remainMicroSeconds);

            ticks = limitTicks;
        }

        Timer.DeltaTime = (float)((double)ticks / (double)frequency);
        Timer.TotalTime = Timer.TotalTime + Timer.DeltaTime;

        Timer.UpdateFramerateTimer += Timer.DeltaTime;
        if (Timer.UpdateFramerateTimer >= Timer.UpdateFramerateInterval)
        {
            Timer.UpdateFramerateTimer -= Timer.UpdateFramerateInterval;
            Timer.Framerate = 1.0f / Timer.DeltaTime;
        }
    }

    Timer.TotalFrames++;
    Timer.PrevCounter = counter;
    return isSleep;
}
