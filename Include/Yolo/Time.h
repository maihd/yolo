#pragma once

#include <Yolo/Types.h>

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

void        TimeUpdate(void);
bool        TimeUpdateAndSleep(float framerate = 0);

