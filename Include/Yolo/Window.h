#pragma once

#include <Yolo/Types.h>

namespace Window
{
    bool    Init(string title, int width, int height);
    void    Quit(void);

    bool    PollEvents(void);
    void    SwapBuffer(void);

    bool    ShouldQuit(void);

    int     GetWidth(void);
    int     GetHeight(void);

    Handle  GetHandle(void);
}