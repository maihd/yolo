#include <Yolo/Time.h>

#include "Runtime.h"

namespace Time
{
    int totalFrames = 0;
    float framerate = 0.0f;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    float timeScale = 1.0f;

    uint64 prevCounter = -1;

    uint64 GetCounter(void)
    {
        LARGE_INTEGER value;
        return QueryPerformanceCounter(&value) ? (uint64)value.QuadPart : 0;
    }

    uint64 GetFrequency(void)
    {
        LARGE_INTEGER value;
        return QueryPerformanceCounter(&value) ? (uint64)value.QuadPart : 0;
    }


    void MicroSleep(uint64 microseconds)
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
           times.QuadPart = -(int64)microseconds * 10;
           NtDelayExecution(FALSE, &times);
       }
       else
       {
           ::Sleep((uint32)(microseconds / 1000U));
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
        uint64 counter = GetCounter();
        uint64 ticks = counter - prevCounter;

        deltaTime = (float)((double)ticks / (double)GetFrequency());
        totalTime = totalTime + deltaTime;
        framerate = 1.0f / deltaTime;

        totalFrames++;

        prevCounter = counter;
    }

    bool UpdateAndSleep(float framerate)
    {
        uint64 frequency = GetFrequency();
        uint64 limitTicks = (uint64)(frequency / (double)(framerate < 1.0f ? 1.0f : framerate));

        uint64 counter = GetCounter();
        uint64 ticks = prevCounter > 0 ? counter - prevCounter : limitTicks;

        bool isSleep = false;
        if (ticks < limitTicks)
        {
            isSleep = true;

            uint64 remainTicks = limitTicks - ticks;
            double remainSeconds = ((double)remainTicks) / frequency;
            uint64 remainMicroSeconds = (uint64)(remainSeconds * 1000000ULL);
            MicroSleep(remainMicroSeconds);

            ticks = limitTicks;
        }

        deltaTime = (float)((double)ticks / (double)frequency);
        totalTime = totalTime + deltaTime;
        framerate = 1.0f / deltaTime;
        totalFrames++;

        prevCounter = counter;

        return isSleep;
    }
}
