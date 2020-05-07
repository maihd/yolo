#pragma once

#include <Yolo/Types.h>

namespace AudioBufferOps
{
    AudioBuffer New(const void* data, I32 size, I32 frequency, AudioFormat format);
    void        Free(AudioBuffer* source);
}

namespace AudioSourceOps
{
    AudioSource New(void);
    void        Free(AudioSource* source);

    void        SetGain(AudioSource source, F32 value);
    void        SetPitch(AudioSource source, F32 value);
    void        SetLooping(AudioSource source, bool value);

    void        SetPosition(AudioSource source, F32 x, F32 y, F32 z = 0.0f);
    void        SetVelocity(AudioSource source, F32 x, F32 y, F32 z = 0.0f);

    void        SetBuffer(AudioSource source, AudioBuffer buffer);

    void        Play(AudioSource source);
    void        Stop(AudioSource source);
    void        Pause(AudioSource source);
    void        Resume(AudioSource source);
}

namespace AudioListener
{
    void        SetPosition(F32 x, F32 y, F32 z = 0.0f);
    void        SetVelocity(F32 x, F32 y, F32 z = 0.0f);
    void        SetOrientation(F32 x0, F32 y0, F32 x1, F32 y1);
    void        SetOrientation(F32 x0, F32 y0, F32 z0, F32 x1, F32 y1, F32 z1);
};

namespace Audio
{
    bool        Init(void);
    void        Quit(void);

    void        SetVolume(F32 volume);
    float       GetVolume(void);

    void        PlayEffect(String path, bool loop = false);
    void        StopEffect(String path);

    void        PlayMusic(String path, bool loop = false);
    void        StopMusic(String path);
    void        PauseMusic(String path);
    void        ResumeMusic(String path);

    void        StopAllEffects(void);
    void        StopAllMusics(void);
}

