#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include <Yolo/Types.h>
#include <Yolo/Input.h>

namespace Runtime
{
    extern bool shouldQuit;
    extern HWND mainWindow;
    extern HDC  mainWindowContext;
}

namespace Input
{
    void Setup(void);

    void NewFrame(void);
    void EndFrame(void);

    void UpdateCharInput(I32 character);
    void UpdateCharInput(String string);

    void UpdateKey(KeyCode key, bool down);

    void UpdateMouse(MouseButton button, bool down);
    void UpdateMouseMove(F32 x, F32 y);
    void UpdateMouseWheel(F32 h, F32 v);

    void UpdateGamepadAxis(I32 gamepadID, GamepadAxis axis, F32 value);
    void UpdateGamepadButton(I32 gamepadID, GamepadButton button, bool down);
}