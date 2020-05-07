#include <Yolo/Time.h>

#include "Runtime.h"

namespace Time
{
    int totalFrames = 0;
    float framerate = 0.0f;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    float timeScale = 1.0f;

    float updateFramerateTimer    = 0.0f;
    float updateFramerateInterval = 1.0f;

    U64 prevCounter = 0;

    U64 GetCounter(void)
    {
        LARGE_INTEGER value;
        return QueryPerformanceCounter(&value) ? (U64)value.QuadPart : 0;
    }

    U64 GetFrequency(void)
    {
        LARGE_INTEGER value;
        return QueryPerformanceFrequency(&value) ? (U64)value.QuadPart : 0;
    }


    void MicroSleep(U64 microseconds)
    {
       /* 'NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);' */
       /* 'typedef LONG NTSTATUS;' =)) */
       /* '#define NTAPI __stdcall' =)) */
       typedef LONG(__stdcall * NtDelayExecutionFN)(BOOL, PLARGE_INTEGER);
       
       static int done_finding;
       static NtDelayExecutionFN NtDelayExecution;
       
       if (!NtDelayExecution && !done_finding)
       {
           done_finding = 1;
           HMODULE module = GetModuleHandle(TEXT("ntdll.dll"));
           const char* func = "NtDelayExecution";
           NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, func);
       }
       
       if (NtDelayExecution)
       {
           LARGE_INTEGER times;
           times.QuadPart = -(I64)microseconds * 10;
           NtDelayExecution(FALSE, &times);
       }
       else
       {
           ::Sleep((U32)(microseconds / 1000U));
       }
    }

    float GetFramerate(void)
    {
        return framerate;
    }

    int GetTotalFrames(void)
    {
        return totalFrames;
    }

    float GetTotalTime(void)
    {
        return totalTime;
    }

    float GetTimeScale(void)
    {
        return timeScale;
    }

    void SetTimeScale(float timeScale)
    {
        Time::timeScale = timeScale;
    }

    float GetDeltaTime(void)
    {
        return deltaTime * timeScale;
    }

    float GetUnscaledDeltaTime(void)
    {
        return deltaTime;
    }

    void Update(void)
    {
        U64 counter = GetCounter();
        U64 ticks = counter - prevCounter;

        deltaTime = (float)((double)ticks / (double)GetFrequency());
        totalTime = totalTime + deltaTime;

        updateFramerateTimer += deltaTime;
        if (updateFramerateTimer >= updateFramerateInterval)
        {
            updateFramerateTimer -= updateFramerateInterval;
            framerate = 1.0f / deltaTime;
        }

        totalFrames++;

        prevCounter = counter;
    }

    bool UpdateAndSleep(float targetFramerate)
    {
        U64 frequency = GetFrequency();
        U64 limitTicks = (U64)(frequency / (double)(targetFramerate < 1.0f ? 1.0f : targetFramerate));

        U64 counter = GetCounter();
        U64 ticks = prevCounter > 0 ? counter - prevCounter : limitTicks;

        bool isSleep = false;
        if (ticks < limitTicks)
        {
            isSleep = true;

            U64 remainTicks = limitTicks - ticks;
            double remainSeconds = ((double)remainTicks) / frequency;
            U64 remainMicroSeconds = (U64)(remainSeconds * 1000000ULL);
            MicroSleep(remainMicroSeconds);

            ticks = limitTicks;
        }

        deltaTime = (float)((double)ticks / (double)frequency);
        totalTime = totalTime + deltaTime;

        updateFramerateTimer += deltaTime;
        if (updateFramerateTimer >= updateFramerateInterval)
        {
            updateFramerateTimer -= updateFramerateInterval;
            framerate = 1.0f / deltaTime;
        }

        totalFrames++;
        prevCounter = counter;
        return isSleep;
    }
}
