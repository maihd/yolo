#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <Yolo/Core.h>
#include <Yolo/Input.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

struct RuntimeState
{
    bool            ShouldClose;
    bool            ShouldRender;

    SDL_Window*     MainWindow;
    SDL_GLContext   GraphicsContext;
};

extern RuntimeState Runtime;

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