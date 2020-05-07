#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <Yolo/Input.h>

namespace
{
    struct GamepadState
    {
        float   axis[8];

        int     buttonUpFrame[32];
        int     buttonDownFrame[32];
    };

    constexpr int   KEY_INPUT_COUNT = 1024;
    constexpr int   MOUSE_INPUT_COUNT = 8;

    static int      _currentFrame;

    static int      _inputTextLength;
    static char     _inputText[1024];

    static bool     _keyState[KEY_INPUT_COUNT];
    static int      _keyUpFrame[KEY_INPUT_COUNT];
    static int      _keyDownFrame[KEY_INPUT_COUNT];
                    
    static int      _mouseState;
    static int      _mouseUpFrame[MOUSE_INPUT_COUNT];
    static int      _mouseDownFrame[MOUSE_INPUT_COUNT];
    static float    _mouseX;
    static float    _mouseY;

    static float    _mouseWheelH;
    static float    _mouseWheelV;
    
    static GamepadState _gamepadStates[8];
}
    
namespace Input
{
    void Setup(void)
    {
        
    }

    void NewFrame(void)
    {
        _currentFrame++;
        _inputTextLength = 0;
    }

    void EndFrame(void)
    {
        _inputText[_inputTextLength] = 0;
    }

    void UpdateCharInput(int character)
    {
        _inputText[_inputTextLength++] = (char)character;
    }

    void UpdateCharInput(String string)
    {
        strcat(_inputText, string);
        _inputTextLength += strlen(string);
    }

    void UpdateKey(KeyCode key, bool down)
    {
        int index = (int)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            _keyState[index] = down;

            if (down)
            {
                _keyDownFrame[index] = _currentFrame;
            }
            else
            {
                _keyUpFrame[index] = _currentFrame;
            }
        }
    }

    void UpdateMouse(MouseButton button, bool down)
    {
        int index = (int)button;
        if (index > -1 && index < MOUSE_INPUT_COUNT)
        {
            if (down)
            {
                _mouseState |= button;
                _mouseDownFrame[index] = _currentFrame;
            }
            else
            {
                _mouseState &= ~button;
                _mouseUpFrame[index] = _currentFrame;
            }
        }
    }

    void UpdateMouseMove(float x, float y)
    {
        _mouseX = x;
        _mouseY = y;
    }

    void UpdateMouseWheel(float h, float v)
    {
        _mouseWheelV += v;
        _mouseWheelH += h;
    }

    void UpdateGamepadAxis(int gamepadID, GamepadAxis axis, float value)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];
            
            int index = (int)axis;
            if (index > -1 && index < CountOf(state.axis))
            {
                state.axis[index] = value;
            }
        }
    }

    void UpdateGamepadButton(int gamepadID, GamepadButton button, bool down)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];

            int index = (int)button;
            if (down)
            {
                if (index > -1 && index < CountOf(state.buttonDownFrame))
                {
                    state.buttonDownFrame[index] = _currentFrame;
                }
            }
            else
            {
                if (index > -1 && index < CountOf(state.buttonUpFrame))
                {
                    state.buttonUpFrame[index] = _currentFrame;
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

    int  GetMouseState(float* x, float* y)
    {
        if (x) *x = _mouseX;
        if (y) *y = _mouseY;

        return _mouseState;
    }
    
    Vector2 GetMousePosition(void)
    {
        return Vector2{ _mouseX, _mouseY };
    }

    bool GetMouseButton(MouseButton button)
    {
        int index = (int)button;
        if (index > -1 && index < CountOf(_mouseDownFrame) && index < CountOf(_mouseUpFrame))
        {
            return _mouseDownFrame[index] > _mouseUpFrame[index];
        }

        return false;
    }

    bool GetMouseButtonUp(MouseButton button)
    {
        int index = (int)button;
        if (index > -1 && index < CountOf(_mouseUpFrame))
        {
            return _mouseUpFrame[index] == _currentFrame;
        }

        return false;
    }

    bool GetMouseButtonDown(MouseButton button)
    {
        int index = (int)button;
        if (index > -1 && index < CountOf(_mouseDownFrame))
        {
            return _mouseDownFrame[index] == _currentFrame;
        }

        return false;
    }

    bool GetKey(KeyCode key)
    {
        int index = (int)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return _keyState[index];
        }

        return false;
    }

    bool GetKeyUp(KeyCode key)
    {
        int index = (int)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return _keyUpFrame[index] == _currentFrame;
        }

        return false;
    }

    bool GetKeyDown(KeyCode key)
    {
        int index = (int)key;
        if (index > -1 && index < KEY_INPUT_COUNT)
        {
            return _keyDownFrame[index] == _currentFrame;
        }

        return false;
    }

    float GetAxis(int gamepadID, GamepadAxis axis)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];

            int index = (int)axis;
            if (index > -1 && index < CountOf(state.axis))
            {
                return state.axis[index];
            }
        }

        return 0.0f;
    }

    bool  GetButton(int gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];

            int index = (int)button;
            if (index > -1 && index < CountOf(state.buttonDownFrame) && index < CountOf(state.buttonUpFrame))
            {
                return state.buttonDownFrame[index] > state.buttonUpFrame[index];
            }
        }

        return false;
    }

    bool  GetButtonUp(int gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];

            int index = (int)button;
            if (index > -1 && index < CountOf(state.buttonUpFrame))
            {
                return state.buttonUpFrame[index] == _currentFrame;
            }
        }

        return false;
    }

    bool  GetButtonDown(int gamepadID, GamepadButton button)
    {
        if (gamepadID > -1 && gamepadID < CountOf(_gamepadStates))
        {
            GamepadState& state = _gamepadStates[gamepadID];

            int index = (int)button;
            if (index > -1 && index < CountOf(state.buttonDownFrame))
            {
                return state.buttonDownFrame[index] == _currentFrame;
            }
        }

        return false;
    }

    const char* GetTextInput(void)
    {
        return _inputText;
    }
}