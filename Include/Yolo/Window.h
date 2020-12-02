#pragma once

#include <Yolo/Core.h>

bool    OpenWindow(StringView title, int width, int height);
void    CloseWindow(void);

bool    HandleWindowEvents(void);
void    UpdateWindowGraphics(void);

bool    ShouldWindowClose(void);

Vector2 WindowSize(void);
int     WindowWidth(void);
int     WindowHeight(void);

Handle  GetWindowHandle(void);
