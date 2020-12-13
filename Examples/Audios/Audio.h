#pragma once

#include <Yolo/Core.h>

struct Wave
{
    U32     SampleCount;
    U32     SampleRate;
    U32     SampleSize;
    Buffer  Buffer;
};

struct AudioStream
{
    U32     SampleCount;
    U32     SampleSize;
    U32     Channels;
    U32     Handle;
};

struct Sound
{
    U32         SampleCount;
    AudioStream Stream;
};

struct Music
{
    U32         ContextType;
    void*       ContextData;

    bool        Looping;
    U32         SampleCount;

    AudioStream Stream;
};

bool OpenAudioDevice(void);
void CloseAudioDevice(void);
bool IsAudioDeviceReady(void);
void SetAudioDeviceVolume(float volume);

Wave    LoadWave(StringView path);
void    FreeWave(Wave* wave);

Sound   LoadSound(StringView path);
Sound   MakeSound(Wave wave);
void    FreeSound(Sound* sound);
void    UpdateSoundData(Sound sound, Buffer data, int sampleCount);

void    PlaySound(Sound sound);
void    StopSound(Sound sound);
void    PauseSound(Sound sound);
void    ResumeSound(Sound sound);

void    PlaySoundMulti(Sound sound);
void    StopSoundMulti(Sound sound);

int     NumSoundsPlaying();
bool    IsSoundPlaying(Sound sound);