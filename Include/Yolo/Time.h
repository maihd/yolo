#pragma once

#include <Yolo/Types.h>

namespace Time
{
    U64     GetCounter(void);
    U64     GetFrequency(void);
    void    MicroSleep(U64 microseconds);

    F32     GetFramerate(void);
    int     GetTotalFrames(void);

    F32     GetTotalTime(void);

    F32     GetTimeScale(void);
    void    SetTimeScale(F32 timeScale);

    F32     GetDeltaTime(void);
    F32     GetUnscaledDeltaTime(void);

    void    Update(void);
    bool    UpdateAndSleep(F32 framerate = 0);
}

