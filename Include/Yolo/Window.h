#pragma once

#include <Yolo/Core.h>

// ----------------------------------------
// Main functions
// ----------------------------------------

bool        OpenWindow(StringView title, int width, int height);
void        CloseWindow(void);

bool        UpdateWindow(void);

Vector2     WindowSize(void);
int         WindowWidth(void);
int         WindowHeight(void);

Handle      GetWindowHandle(void);

// ----------------------------------------
// Timer functions
// ----------------------------------------

U64         GetCpuCounter(void);
U64         GetCpuFrequency(void);

void        MicroSleep(U64 microseconds);

float       GetFramerate(void);
int         GetTotalFrames(void);

float       GetTotalTime(void);

float       GetTimeScale(void);
void        SetTimeScale(float timeScale);

float       GetDeltaTime(void);
float       GetUnscaledDeltaTime(void);

void        UpdateTimer(void);
bool        UpdateTimerAndSleep(float limitFramerate = 0);
