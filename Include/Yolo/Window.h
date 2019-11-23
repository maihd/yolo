#pragma once

#include <Yolo/String.h>

namespace Window
{
    bool Init(string title, int width, int height);
    void Quit(void);

    void PollEvents(void);
    void SwapBuffer(void);

    bool ShouldQuit(void);

    int  GetWidth(void);
    int  GetHeight(void);
}