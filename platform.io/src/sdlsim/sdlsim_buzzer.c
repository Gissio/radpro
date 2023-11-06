/*
 * Rad Pro
 * SDLSim buzzer
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>

#include <SDL.h>

#include "../buzzer.h"
#include "../events.h"

#define AUDIO_SAMPLE_FREQUENCY 48000
#define AUDIO_BUFFER_SIZE 4096

#define SYS_TICKS_PER_BUFFER (SYS_TICK_FREQUENCY * AUDIO_BUFFER_SIZE / AUDIO_SAMPLE_FREQUENCY)

#define BUZZER_FREQUENCY 2700

#define PULSE_BUFFER_SIZE 512
#define PULSE_BUFFER_MASK (PULSE_BUFFER_SIZE - 1)

static struct
{
    bool value;

    uint32_t pulseTicksHead;
    uint32_t pulseTicksTail;
    bool pulseTicks[PULSE_BUFFER_SIZE];

    SDL_AudioDeviceID audioDeviceId;
    float audioPhase;
} buzzer;

static void onAudioBuffer(void *userdata, Uint8 *stream, int len);

void initBuzzerHardware(void)
{
    SDL_AudioSpec audioSpec;
    audioSpec.freq = AUDIO_SAMPLE_FREQUENCY;
    audioSpec.format = AUDIO_S16;
    audioSpec.channels = 1;
    audioSpec.samples = AUDIO_BUFFER_SIZE;
    audioSpec.callback = onAudioBuffer;
    audioSpec.userdata = NULL;

    buzzer.audioDeviceId = SDL_OpenAudio(&audioSpec, NULL);
    if (buzzer.audioDeviceId < 0)
    {
        printf("Could not open audio: %s\n", SDL_GetError());

        exit(1);
    }

    SDL_PauseAudio(0);
}

static void onAudioBuffer(void *userdata, Uint8 *stream, int len)
{
    uint32_t pulseTicksAvailable =
        (buzzer.pulseTicksHead - buzzer.pulseTicksTail) &
        PULSE_BUFFER_MASK;

    if (pulseTicksAvailable < SYS_TICKS_PER_BUFFER)
        memset(stream, 0, len);
    else
    {
        uint32_t pulseTicksSize;
        if (pulseTicksAvailable > (SYS_TICKS_PER_BUFFER * 3 / 2 + 2))
            pulseTicksSize = SYS_TICKS_PER_BUFFER + 2;
        else
            pulseTicksSize = SYS_TICKS_PER_BUFFER - 2;

        int16_t *samples = (int16_t *)stream;
        uint32_t sampleNum = len / sizeof(int16_t);

        float audioPhaseDelta = 2.0F * (float)M_PI *
                                BUZZER_FREQUENCY / AUDIO_SAMPLE_FREQUENCY;

        for (uint32_t i = 0; i < sampleNum; i++)
        {
            buzzer.audioPhase += audioPhaseDelta;
            if (buzzer.audioPhase > 2.0F * (float)M_PI)
                buzzer.audioPhase -= 2.0F * (float)M_PI;

            float amplitude = sinf(buzzer.audioPhase) +
                              sinf(3 * buzzer.audioPhase) / 3 +
                              sinf(5 * buzzer.audioPhase) / 5;

            uint32_t pulseTicksIndex =
                (buzzer.pulseTicksTail +
                 i * pulseTicksSize / AUDIO_BUFFER_SIZE) &
                PULSE_BUFFER_MASK;

            float gain = 0.25F * buzzer.pulseTicks[pulseTicksIndex];

            samples[i] = (int16_t)((INT16_MAX * gain) * amplitude);
        }

        buzzer.pulseTicksTail =
            (buzzer.pulseTicksTail + pulseTicksSize) & PULSE_BUFFER_MASK;
    }
}

void updateBuzzer(void)
{
    SDL_LockAudioDevice(buzzer.audioDeviceId);

    buzzer.pulseTicks[buzzer.pulseTicksHead] = buzzer.value;
    buzzer.pulseTicksHead = (buzzer.pulseTicksHead + 1) & PULSE_BUFFER_MASK;

    SDL_UnlockAudioDevice(buzzer.audioDeviceId);
}

void setBuzzer(bool value)
{
    buzzer.value = value;
}

bool getBuzzer(void)
{
    return buzzer.value;
}

#endif
