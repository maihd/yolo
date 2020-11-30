#pragma once

#include <Yolo/Core.h>

namespace AudioBufferOps
{
    AudioBuffer New(const void* data, I32 size, I32 frequency, AudioFormat format);
    void        Free(AudioBuffer* source);
}

namespace AudioSourceOps
{
    AudioSource New(void);
    void        Free(AudioSource* source);

    void        SetGain(AudioSource source, float value);
    void        SetPitch(AudioSource source, float value);
    void        SetLooping(AudioSource source, bool value);

    void        SetPosition(AudioSource source, float x, float y, float z = 0.0f);
    void        SetVelocity(AudioSource source, float x, float y, float z = 0.0f);

    void        SetBuffer(AudioSource source, AudioBuffer buffer);

    void        Play(AudioSource source);
    void        Stop(AudioSource source);
    void        Pause(AudioSource source);
    void        Resume(AudioSource source);
}

namespace AudioListener
{
    void        SetPosition(float x, float y, float z = 0.0f);
    void        SetVelocity(float x, float y, float z = 0.0f);
    void        SetOrientation(float x0, float y0, float x1, float y1);
    void        SetOrientation(float x0, float y0, float z0, float x1, float y1, float z1);
};

namespace Audio
{
    bool        Init(void);
    void        Quit(void);

    void        SetVolume(float volume);
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

