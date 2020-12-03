#pragma once

#include <Yolo/Core.h>

// ----------------------------------------
// Main functions
// ----------------------------------------

bool        OpenWindow(StringView title, int width, int height, int fps = 60);
void        CloseWindow(void);

bool        UpdateWindow(void);

// ----------------------------------------
// Screen & viewport
// ----------------------------------------

Vector2     WindowSize(void);
int         WindowWidth(void);
int         WindowHeight(void);

// ----------------------------------------
// Timer functions
// ----------------------------------------

float       GetFramerate(void);
int         GetTotalFrames(void);

float       GetTotalTime(void);

float       GetTimeScale(void);
void        SetTimeScale(float timeScale);

float       GetDeltaTime(void);
float       GetUnscaledDeltaTime(void);
