#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <System/Input.h>

constexpr int KEY_INPUT_COUNT = 1024;
constexpr int MOUSE_INPUT_COUNT = 8;

constexpr int GAMEPAD_INPUT_COUNT = 8;
constexpr int GAMEPAD_AXIS_COUNT = 8;
constexpr int GAMEPAD_BUTTON_COUNT = 32;

struct GamepadState
{
    float   Axes[GAMEPAD_AXIS_COUNT];

    I32     ButtonUpFrames[GAMEPAD_BUTTON_COUNT];
    I32     ButtonDownFrames[GAMEPAD_BUTTON_COUNT];
};

static struct
{
    I32             CurrentFrame;

    I32             InputTextLength;
    char            InputText[1024];

    bool            KeyState[KEY_INPUT_COUNT];
    I32             KeyUpFrame[KEY_INPUT_COUNT];
    I32             KeyDownFrame[KEY_INPUT_COUNT];
                    
    I32             MouseState;
    I32             MouseUpFrame[MOUSE_INPUT_COUNT];
    I32             MouseDownFrame[MOUSE_INPUT_COUNT];
    float           MouseX;
    float           MouseY;

    float           MouseWheelH;
    float           MouseWheelV;
    
    GamepadState    GamepadStates[GAMEPAD_INPUT_COUNT];
} InputState;
    
namespace Input
{
    void Setup(void)
    {
        
    }

    void NewFrame(void)
    {
        InputState.CurrentFrame++;
        InputState.InputTextLength = 0;
    }

    void EndFrame(void)
    {
        InputState.InputText[InputState.InputTextLength] = 0;
    }

    void UpdateCharInput(I32 character)
    {
        InputState.InputText[InputState.InputTextLength++] = (char)character;
    }

    void UpdateCharInput(String string)
    {
        strcat(InputState.InputText, string.Buffer);
        InputState.InputTextLength += string.Length;
    }

    void UpdateKey(KeyCode key, bool down)
    {
        I32 index = (I32)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            InputState.KeyState[index] = down;

            if (down)
            {
                InputState.KeyDownFrame[index] = InputState.CurrentFrame;
            }
            else
            {
                InputState.KeyUpFrame[index] = InputState.CurrentFrame;
            }
        }
    }

    void UpdateMouse(MouseButton button, bool down)
    {
        I32 index = (I32)button;
        if (index > -1 && index < MOUSE_INPUT_COUNT)
        {
            if (down)
            {
                InputState.MouseState |= button;
                InputState.MouseDownFrame[index] = InputState.CurrentFrame;
            }
            else
            {
                InputState.MouseState &= ~button;
                InputState.MouseUpFrame[index] = InputState.CurrentFrame;
            }
        }
    }

    void UpdateMouseMove(float x, float y)
    {
        InputState.MouseX = x;
        InputState.MouseY = y;
    }

    void UpdateMouseWheel(float h, float v)
    {
        InputState.MouseWheelV += v;
        InputState.MouseWheelH += h;
    }

    void UpdateGamepadAxis(I32 gamepadID, GamepadAxis axis, float value)
    {
        if (gamepadID > -1 && gamepadID < GAMEPAD_INPUT_COUNT)
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];
            
            I32 index = (I32)axis;
            if (index > -1 && index < CountOf(state.Axes))
            {
                state.Axes[index] = value;
            }
        }
    }

    void UpdateGamepadButton(I32 gamepadID, GamepadButton button, bool down)
    {
        if (gamepadID > -1 && gamepadID < GAMEPAD_INPUT_COUNT)
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];

            I32 index = (I32)button;
            if (down)
            {
                if (index > -1 && index < CountOf(state.ButtonDownFrames))
                {
                    state.ButtonDownFrames[index] = InputState.CurrentFrame;
                }
            }
            else
            {
                if (index > -1 && index < CountOf(state.ButtonUpFrames))
                {
                    state.ButtonUpFrames[index] = InputState.CurrentFrame;
                }
            }
        }
    }

    void ShowMouseCursor(void)
    {
        SetMouseCursorVisible(true);
    }

    void HideMouseCursor(void)
    {
        SetMouseCursorVisible(false);
    }

    bool IsMouseCursorVisibled(void)
    {
        return false;
    }

    void SetMouseCursorVisible(bool visible)
    {
        
    }

    I32  GetMouseState(float* x, float* y)
    {
        if (x) *x = InputState.MouseX;
        if (y) *y = InputState.MouseY;

        return InputState.MouseState;
    }
    
    Vector2 GetMousePosition(void)
    {
        return Vector2 { InputState.MouseX, InputState.MouseY };
    }

    bool GetMouseButton(MouseButton button)
    {
        I32 index = (I32)button;
        if (index > -1 && index < CountOf(InputState.MouseDownFrame) && index < CountOf(InputState.MouseUpFrame))
        {
            return InputState.MouseDownFrame[index] > InputState.MouseUpFrame[index];
        }

        return false;
    }

    bool GetMouseButtonUp(MouseButton button)
    {
        I32 index = (I32)button;
        if (index > -1 && index < CountOf(InputState.MouseUpFrame))
        {
            return InputState.MouseUpFrame[index] == InputState.CurrentFrame;
        }

        return false;
    }

    bool GetMouseButtonDown(MouseButton button)
    {
        I32 index = (I32)button;
        if (index > -1 && index < CountOf(InputState.MouseDownFrame))
        {
            return InputState.MouseDownFrame[index] == InputState.CurrentFrame;
        }

        return false;
    }

    bool GetKey(KeyCode key)
    {
        I32 index = (I32)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return InputState.KeyState[index];
        }

        return false;
    }

    bool GetKeyUp(KeyCode key)
    {
        I32 index = (I32)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return InputState.KeyUpFrame[index] == InputState.CurrentFrame;
        }

        return false;
    }

    bool GetKeyDown(KeyCode key)
    {
        I32 index = (I32)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return InputState.KeyDownFrame[index] == InputState.CurrentFrame;
        }

        return false;
    }

    float GetAxis(I32 gamepadID, GamepadAxis axis)
    {
        if (gamepadID > -1 && gamepadID < CountOf(InputState.GamepadStates))
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];

            I32 index = (I32)axis;
            if (index > -1 && index < CountOf(state.Axes))
            {
                return state.Axes[index];
            }
        }

        return 0.0f;
    }

    bool  GetButton(I32 gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(InputState.GamepadStates))
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];

            I32 index = (I32)button;
            if (index > -1 && index < CountOf(state.ButtonDownFrames) && index < CountOf(state.ButtonUpFrames))
            {
                return state.ButtonDownFrames[index] > state.ButtonUpFrames[index];
            }
        }

        return false;
    }

    bool  GetButtonUp(I32 gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(InputState.GamepadStates))
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];

            I32 index = (I32)button;
            if (index > -1 && index < CountOf(state.ButtonUpFrames))
            {
                return state.ButtonUpFrames[index] == InputState.CurrentFrame;
            }
        }

        return false;
    }

    bool  GetButtonDown(I32 gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(InputState.GamepadStates))
        {
            GamepadState& state = InputState.GamepadStates[gamepadID];

            I32 index = (I32)button;
            if (index > -1 && index < CountOf(state.ButtonDownFrames))
            {
                return state.ButtonDownFrames[index] == InputState.CurrentFrame;
            }
        }

        return false;
    }

    const char* GetTextInput(void)
    {
        return InputState.InputText;
    }
}