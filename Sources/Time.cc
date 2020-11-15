#include <Yolo/Time.h>

#include "Runtime.h"

static I32 totalFrames = 0;
static float framerate = 0.0f;
static float deltaTime = 0.0f;
static float totalTime = 0.0f;
static float timeScale = 1.0f;

static float updateFramerateTimer    = 0.0f;
static float updateFramerateInterval = 1.0f;

static U64 prevCounter = 0;

U64 GetCpuCounter(void)
{
    LARGE_INTEGER value;
    return QueryPerformanceCounter(&value) ? (U64)value.QuadPart : 0;
}

U64 GetCpuFrequency(void)
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
       
    static bool haveLoadFunction;
    static NtDelayExecutionFN NtDelayExecution;
       
    if (!NtDelayExecution && !haveLoadFunction)
    {
        haveLoadFunction = true;

        HMODULE module = GetModuleHandle(TEXT("ntdll.dll"));
        NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, "NtDelayExecution");
    }
       
    if (NtDelayExecution)
    {
        LARGE_INTEGER times;
        times.QuadPart = -(LONGLONG)microseconds * 10;
        NtDelayExecution(FALSE, &times);
    }
    else
    {
        ::Sleep((DWORD)(microseconds / 1000U));
    }
}

float GetFramerate(void)
{
    return framerate;
}

I32 GetTotalFrames(void)
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
    ::timeScale = timeScale;
}

float GetDeltaTime(void)
{
    return deltaTime * timeScale;
}

float GetUnscaledDeltaTime(void)
{
    return deltaTime;
}

void TimeUpdate(void)
{
    U64 counter = GetCpuCounter();
    U64 ticks = counter - prevCounter;

    deltaTime = (float)((double)ticks / (double)GetCpuFrequency());
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

bool TimeUpdateAndSleep(float targetFramerate)
{
    U64 frequency  = GetCpuFrequency();
    U64 limitTicks = (U64)(frequency / (double)(targetFramerate < 1.0f ? 1.0f : targetFramerate));

    U64 counter = GetCpuCounter();
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
