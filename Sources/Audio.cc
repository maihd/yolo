#include <Yolo/Audio.h>

#include <AL/al.h>
#include <AL/alc.h>

namespace
{
    static ALCdevice*  _device;
    static ALCcontext* _context;

    inline ALenum ConvertAudioFormat(AudioFormat format)
    {
        switch (format)
        {
        case AudioFormat::Mono8:
            return AL_FORMAT_MONO8;

        case AudioFormat::Mono16:
            return AL_FORMAT_MONO16;

        case AudioFormat::Stereo8:
            return AL_FORMAT_STEREO8;

        case AudioFormat::Stereo16:
            return AL_FORMAT_STEREO16;
        }

        return 0;
    }
}

namespace AudioBufferOps
{
    AudioBuffer New(const void* data, I32 size, I32 frequency, AudioFormat format)
    {
        AudioBuffer buffer;
        alGenBuffers(1, &buffer.handle);
        alBufferData(buffer.handle, ConvertAudioFormat(format), data, size, frequency);
        return buffer;
    }

    void Free(AudioBuffer* source)
    {
        alDeleteBuffers(1, &source->handle);
        source->handle = 0;
    }
}

namespace AudioSourceOps
{
    AudioSource New(void)
    {
        AudioSource source;
        alGenSources(1, &source.handle);

        alSourcef(source.handle, AL_GAIN, 1.0f);
        alSourcef(source.handle, AL_PITCH, 1.0f);
        alSourcei(source.handle, AL_LOOPING, false);
        alSource3f(source.handle, AL_POSITION, 0, 0, 0);
        alSource3f(source.handle, AL_VELOCITY, 0, 0, 0);

        return source;
    }

    void Free(AudioSource* source)
    {
        alDeleteSources(1, &source->handle);

        source->handle = 0;
    }

    void SetGain(AudioSource source, F32 value)
    {
        alSourcef(source.handle, AL_GAIN, value);
    }

    void SetPitch(AudioSource source, F32 value)
    {
        alSourcef(source.handle, AL_PITCH, value);
    }

    void SetLooping(AudioSource source, bool value)
    {
        alSourcei(source.handle, AL_LOOPING, value);
    }

    void SetPosition(AudioSource source, F32 x, F32 y, F32 z)
    {
        alSource3f(source.handle, AL_POSITION, x, y, z);
    }

    void SetVelocity(AudioSource source, F32 x, F32 y, F32 z)
    {
        alSource3f(source.handle, AL_VELOCITY, x, y, z);
    }

    void SetBuffer(AudioSource source, AudioBuffer buffer)
    {
        alSourcei(source.handle, AL_BUFFER, buffer.handle);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            //printf("AudioSource::SetBuffer: %s\n", alGetString(error));
        }
    }

    void Play(AudioSource source)
    {
        alSourcePlay(source.handle);
    }

    void Stop(AudioSource source)
    {
        alSourceStop(source.handle);
    }

    void Pause(AudioSource source)
    {
        alSourcePause(source.handle);
    }

    void Resume(AudioSource source)
    {
        alSourcePlay(source.handle);
    }
}

namespace AudioListener
{
    void SetPosition(F32 x, F32 y, F32 z)
    {
        alListener3f(AL_POSITION, x, y, z);
    }

    void SetVelocity(F32 x, F32 y, F32 z)
    {
        alListener3f(AL_VELOCITY, x, y, z);
    }

    void SetOrientation(F32 x0, F32 y0, F32 x1, F32 y1)
    {
        ALfloat listenerOri[] = { x0, y0, 0.0f, x1, y1, 0.0f };
        alListenerfv(AL_ORIENTATION, listenerOri);
    }

    void SetOrientation(F32 x0, F32 y0, F32 z0, F32 x1, F32 y1, F32 z1)
    {
        ALfloat listenerOri[] = { x0, y0, z1, x1, y1, z1 };
        alListenerfv(AL_ORIENTATION, listenerOri);
    }
};

namespace Audio
{
    bool Init(void)
    {
        _device = alcOpenDevice(0);
        if (!_device)
        {
            //fprintf(stderr, "audio::init(): Cannot open audio _device '%s'\n", alcGetString(NULL, alcGetError(NULL)));
            return false;
        }

        //fprintf(stderr, "audio::init(): %s\n", alcGetString(_device, ALC_DEVICE_SPECIFIER));

        _context = alcCreateContext(_device, 0);
        if (!_context)
        {
            //fprintf(stderr, "audio::init(): Cannot create audio _context '%s'", alcGetString(_device, alcGetError(_device)));
            return false;
        }

        if (!alcMakeContextCurrent(_context))
        {
            return false;
        }
        //printf("Audio version: %s\n", alGetString(AL_VERSION));
        //printf("Audio renderer: %s\n", alGetString(AL_RENDERER));

        alListener3f(AL_POSITION, 0, 0, 0);
        alListener3f(AL_VELOCITY, 0, 0, 0);

        ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
        alListenerfv(AL_ORIENTATION, listenerOri);

        return true;
    }

    void Quit(void)
    {
        alcDestroyContext(_context);
        _context = 0;
        _device = 0;
    }
}

