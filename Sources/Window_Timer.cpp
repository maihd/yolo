#include <Yolo/Window.h>

#include "./Internal.h"

static struct
{
    I32 TotalFrames = 0;
    float Framerate = 0.0f;
    float DeltaTime = 0.0f;
    float TotalTime = 0.0f;
    float TimeScale = 1.0f;

    float UpdateFramerateTimer = 0.0f;
    float UpdateFramerateInterval = 1.0f;

    U64 PrevCounter = 0;
} Timer;

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
    return Timer.Framerate;
}

I32 GetTotalFrames(void)
{
    return Timer.TotalFrames;
}

float GetTotalTime(void)
{
    return Timer.TotalTime;
}

float GetTimeScale(void)
{
    return Timer.TimeScale;
}

void SetTimeScale(float timeScale)
{
    Timer.TimeScale = timeScale;
}

float GetDeltaTime(void)
{
    return Timer.DeltaTime * Timer.TimeScale;
}

float GetUnscaledDeltaTime(void)
{
    return Timer.DeltaTime;
}

void UpdateTimer(void)
{
    U64 counter = GetCpuCounter();
    U64 ticks = counter - Timer.PrevCounter;

    Timer.DeltaTime = (float)((double)ticks / (double)GetCpuFrequency());
    Timer.TotalTime = Timer.TotalTime + Timer.DeltaTime;

    Timer.UpdateFramerateTimer += Timer.DeltaTime;
    if (Timer.UpdateFramerateTimer >= Timer.UpdateFramerateInterval)
    {
        Timer.UpdateFramerateTimer -= Timer.UpdateFramerateInterval;
        Timer.Framerate = 1.0f / Timer.DeltaTime;
    }

    Timer.TotalFrames++;

    Timer.PrevCounter = counter;
}

bool UpdateTimerAndSleep(float limitFramerate)
{
    U64 frequency  = GetCpuFrequency();
    U64 limitTicks = (U64)(frequency / (double)(limitFramerate < 1.0f ? 1.0f : limitFramerate));

    U64 counter = GetCpuCounter();
    U64 ticks = Timer.PrevCounter > 0 ? counter - Timer.PrevCounter : limitTicks;

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

    Timer.DeltaTime = (float)((double)ticks / (double)frequency);
    Timer.TotalTime = Timer.TotalTime + Timer.DeltaTime;

    Timer.UpdateFramerateTimer += Timer.DeltaTime;
    if (Timer.UpdateFramerateTimer >= Timer.UpdateFramerateInterval)
    {
        Timer.UpdateFramerateTimer -= Timer.UpdateFramerateInterval;
        Timer.Framerate = 1.0f / Timer.DeltaTime;
    }

    Timer.TotalFrames++;
    Timer.PrevCounter = counter;
    return isSleep;
}
