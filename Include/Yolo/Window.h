#pragma once

#include <Yolo/Types.h>

namespace Window
{
    bool    Init(String title, int width, int height);
    void    Quit(void);

    bool    PollEvents(void);
    void    SwapBuffer(void);

    bool    ShouldQuit(void);

    Vector2    GetSize(void);
    int     GetWidth(void);
    int     GetHeight(void);

    Handle  GetHandle(void);
}