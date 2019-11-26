#pragma once

#include <Yolo/Types.h>

namespace Time
{
    uint64  GetCounter(void);
    uint64  GetFrequency(void);
    void    MicroSleep(uint64 microseconds);

    float   GetFramerate(void);
    int     GetTotalFrames(void);

    float   GetTotalTime(void);

    float   GetTimeScale(void);
    void    SetTimeScale(float timeScale);

    float   GetDeltaTime(void);
    float   GetUnscaledDeltaTime(void);

    void    Update(void);
    bool    UpdateAndSleep(float framerate = 0);
}

