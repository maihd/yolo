#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

constexpr ma_format AUDIO_DEVICE_FORMAT             = ma_format_f32;
constexpr int       AUDIO_DEVICE_CHANNELS           = 2;
constexpr int       AUDIO_DEVICE_SAMPLE_RATE        = 44100;
constexpr int       MAX_AUDIO_BUFFER_POOL_CHANNELS  = 16;
constexpr int       DEFAULT_AUDIO_BUFFER_SIZE       = 4096;

static struct AudioSystem
{
    struct
    {
        ma_context  Context;
        ma_device   Device;
    } Backend;
} gAudioSystem;

bool OpenAudioDevice(void)
{
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
    deviceConfig.dataCallback = nullptr;
    deviceConfig.pUserData = &gAudioSystem;
    
    ma_result initDeviceResult = ma_device_init(&gAudioSystem.Backend.Context, &deviceConfig, &gAudioSystem.Backend.Device);
    if (initDeviceResult != MA_SUCCESS)
    {
        DebugPrint("AUDIO: Failed to initialize playback device");

        ma_context_uninit(&gAudioSystem.Backend.Context);
        return false;
    }
        
    return true;
}

void CloseAudioDevice(void)
{

}

bool IsAudioDeviceReady(void)
{
    return true;
}

void SetAudioDeviceVolume(float volume)
{

}
