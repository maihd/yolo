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
        alGenBuffers(1, &buffer.Handle);
        alBufferData(buffer.Handle, ConvertAudioFormat(format), data, size, frequency);
        return buffer;
    }

    void Free(AudioBuffer* source)
    {
        alDeleteBuffers(1, &source->Handle);
        source->Handle = 0;
    }
}

namespace AudioSourceOps
{
    AudioSource New(void)
    {
        AudioSource source;
        alGenSources(1, &source.Handle);

        alSourcef(source.Handle, AL_GAIN, 1.0f);
        alSourcef(source.Handle, AL_PITCH, 1.0f);
        alSourcei(source.Handle, AL_LOOPING, false);
        alSource3f(source.Handle, AL_POSITION, 0, 0, 0);
        alSource3f(source.Handle, AL_VELOCITY, 0, 0, 0);

        return source;
    }

    void Free(AudioSource* source)
    {
        alDeleteSources(1, &source->Handle);

        source->Handle = 0;
    }

    void SetGain(AudioSource source, float value)
    {
        alSourcef(source.Handle, AL_GAIN, value);
    }

    void SetPitch(AudioSource source, float value)
    {
        alSourcef(source.Handle, AL_PITCH, value);
    }

    void SetLooping(AudioSource source, bool value)
    {
        alSourcei(source.Handle, AL_LOOPING, value);
    }

    void SetPosition(AudioSource source, float x, float y, float z)
    {
        alSource3f(source.Handle, AL_POSITION, x, y, z);
    }

    void SetVelocity(AudioSource source, float x, float y, float z)
    {
        alSource3f(source.Handle, AL_VELOCITY, x, y, z);
    }

    void SetBuffer(AudioSource source, AudioBuffer buffer)
    {
        alSourcei(source.Handle, AL_BUFFER, buffer.Handle);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            //printf("AudioSource::SetBuffer: %s\n", alGetString(error));
        }
    }

    void Play(AudioSource source)
    {
        alSourcePlay(source.Handle);
    }

    void Stop(AudioSource source)
    {
        alSourceStop(source.Handle);
    }

    void Pause(AudioSource source)
    {
        alSourcePause(source.Handle);
    }

    void Resume(AudioSource source)
    {
        alSourcePlay(source.Handle);
    }
}

namespace AudioListener
{
    void SetPosition(float x, float y, float z)
    {
        alListener3f(AL_POSITION, x, y, z);
    }

    void SetVelocity(float x, float y, float z)
    {
        alListener3f(AL_VELOCITY, x, y, z);
    }

    void SetOrientation(float x0, float y0, float x1, float y1)
    {
        ALfloat listenerOri[] = { x0, y0, 0.0f, x1, y1, 0.0f };
        alListenerfv(AL_ORIENTATION, listenerOri);
    }

    void SetOrientation(float x0, float y0, float z0, float x1, float y1, float z1)
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

#if 0
bool OpenAudioDevice(void)
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

void CloseAudioDevice(void)
{
    alcDestroyContext(_context);
    _context = 0;
    _device = 0;
}
#endif
