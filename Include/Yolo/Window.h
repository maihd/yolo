#pragma once

#include <Yolo/Types.h>

bool    OpenWindow(String title, int width, int height);
void    CloseWindow(void);

bool    WindowPollEvents(void);
void    WindowSwapBuffer(void);

bool    ShouldWindowQuit(void);

Vector2 WindowSize(void);
int     WindowWidth(void);
int     WindowHeight(void);

Handle  GetWindowHandle(void);
