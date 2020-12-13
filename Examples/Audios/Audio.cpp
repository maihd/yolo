#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

// -----------------------------------------------
// Interal types
// -----------------------------------------------

constexpr ma_format AUDIO_DEVICE_FORMAT             = ma_format_f32;
constexpr int       AUDIO_DEVICE_CHANNELS           = 2;
constexpr int       AUDIO_DEVICE_SAMPLE_RATE        = 44100;
constexpr int       MAX_AUDIO_BUFFER_POOL_CHANNELS  = 16;
constexpr int       DEFAULT_AUDIO_BUFFER_SIZE       = 4096;

enum struct MusicContextType
{
    WAV, OGG, FLAC, MP3, XM, MOD
};

enum struct AudioBufferUsage
{
    Static, Stream
};

struct Audio
{
    ma_data_converter   Converter;

    float               Volume;
    float               Pitch;

    bool                Playing;
    bool                Paused;
    bool                Looping;
    AudioBufferUsage    Usage;

    bool                IsSubBufferProcessed[2];
    U32                 SizeInFrames;
    U32                 FrameIndex;
    U32                 TotalFramesProcessed;

    U8*                 Data;

    Audio*              Next;
    Audio*              Prev;
};

struct AudioSystem
{
    struct
    {
        ma_context  Context;
        ma_device   Device;
        ma_mutex    Lock;
        bool        Ready;
    } Backend;

    struct
    {
        Audio*          First;
        Audio*          Last;
        I32             DefaultSize;
    } Buffer;

    struct
    {
        Audio*          Pool[MAX_AUDIO_BUFFER_POOL_CHANNELS];
        I32             PoolCounter;
        I32             Channels[MAX_AUDIO_BUFFER_POOL_CHANNELS];
    } MultiChannel;
};

// -----------------------------------------------
// Functions prototype
// -----------------------------------------------

static void OnSendAudioDataToDevice(ma_device* device, void* output, const void* input, U32 frameCount);

// -----------------------------------------------
// Device management
// -----------------------------------------------

static AudioSystem gAudioSystem;

bool OpenAudioDevice(void)
{
    if (IsAudioDeviceReady())
    {
        return true;
    }

    ma_context_config contextConfig = ma_context_config_init();
    contextConfig.logCallback = nullptr;

    ma_result initConfigResult = ma_context_init(nullptr, 0, &contextConfig, &gAudioSystem.Backend.Context);
    if (initConfigResult != MA_SUCCESS)
    {
        DebugPrint("AUDIO: Failed to initialize context");
        return false;
    }

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.pDeviceID = nullptr;
    deviceConfig.playback.format = AUDIO_DEVICE_FORMAT;
    deviceConfig.playback.channels = AUDIO_DEVICE_CHANNELS;
    deviceConfig.capture.pDeviceID = nullptr;
    deviceConfig.capture.format = ma_format_s16;
    deviceConfig.capture.channels = 1;
    deviceConfig.sampleRate = AUDIO_DEVICE_SAMPLE_RATE;
    deviceConfig.dataCallback = OnSendAudioDataToDevice;
    deviceConfig.pUserData = &gAudioSystem;
    
    ma_result initDeviceResult = ma_device_init(&gAudioSystem.Backend.Context, &deviceConfig, &gAudioSystem.Backend.Device);
    if (initDeviceResult != MA_SUCCESS)
    {
        DebugPrint("AUDIO: Failed to initialize playback device");

        ma_context_uninit(&gAudioSystem.Backend.Context);
        return false;
    }
    
    gAudioSystem.Backend.Ready = true;
    gAudioSystem.Buffer.DefaultSize = DEFAULT_AUDIO_BUFFER_SIZE;
    return true;
}

void CloseAudioDevice(void)
{
    if (IsAudioDeviceReady())
    {
        gAudioSystem.Backend.Ready = false;

        ma_mutex_uninit(&gAudioSystem.Backend.Lock);
        ma_device_uninit(&gAudioSystem.Backend.Device);
        ma_context_uninit(&gAudioSystem.Backend.Context);
    }
}

bool IsAudioDeviceReady(void)
{
    return gAudioSystem.Backend.Ready;
}

void SetAudioDeviceVolume(float volume)
{
    ma_device_set_master_volume(&gAudioSystem.Backend.Device, volume);
}

// ----------------------------------------------
// Streaming
// ----------------------------------------------

void OnSendAudioDataToDevice(ma_device* device, void* output, const void* input, U32 frameCount)
{

}