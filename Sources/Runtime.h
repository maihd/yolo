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

    void UpdateCharInput(int character);
    void UpdateCharInput(String string);

    void UpdateKey(KeyCode key, bool down);

    void UpdateMouse(MouseButton button, bool down);
    void UpdateMouseMove(float x, float y);
    void UpdateMouseWheel(float h, float v);

    void UpdateGamepadAxis(int gamepadID, GamepadAxis axis, float value);
    void UpdateGamepadButton(int gamepadID, GamepadButton button, bool down);
}