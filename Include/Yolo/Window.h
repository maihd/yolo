#pragma once

#include <Yolo/Types.h>

namespace Window
{
    bool    Init(String title, I32 width, I32 height);
    void    Quit(void);

    bool    PollEvents(void);
    void    SwapBuffer(void);

    bool    ShouldQuit(void);

    Vector2 GetSize(void);
    I32     GetWidth(void);
    I32     GetHeight(void);

    Handle  GetHandle(void);
}
