/*
 * Rad Pro
 * Simulator buzzer
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#if defined(BUZZER)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "../peripherals/buzzer.h"
#include "../system/events.h"

#define AUDIO_SAMPLE_FREQUENCY 48000
#define AUDIO_BUFFER_SIZE 4096

#define SYS_TICKS_PER_BUFFER (SYSTICK_FREQUENCY * AUDIO_BUFFER_SIZE / AUDIO_SAMPLE_FREQUENCY)

#define BUZZER_FREQUENCY 2000

#define PULSE_BUFFER_SIZE 512
#define PULSE_BUFFER_MASK (PULSE_BUFFER_SIZE - 1)

#define VOLUME_GAIN_MAX 0.25F

static const float buzzerVolumeGains[] = {
    VOLUME_GAIN_MAX / 64.0F,
    VOLUME_GAIN_MAX / 16.0F,
    VOLUME_GAIN_MAX / 4.0F,
    VOLUME_GAIN_MAX / 1.0F,
};

// Ticks are queued by onBuzzerTick() and consumed by the audio thread
static struct
{
    bool audioOpen;

    bool value;
    float gain;

    uint32_t pulseTicksHead;
    uint32_t pulseTicksTail;
    bool pulseTicks[PULSE_BUFFER_SIZE];

    float audioPhase;
} buzzer;

static void onAudioBuffer(void *userdata, Uint8 *stream, int len)
{
    uint32_t pulseTicksAvailable = (buzzer.pulseTicksHead - buzzer.pulseTicksTail) & PULSE_BUFFER_MASK;

    if (pulseTicksAvailable < SYS_TICKS_PER_BUFFER)
    {
        memset(stream, 0, len);

        return;
    }

    // Consume slightly more or fewer ticks to track the simulation clock
    uint32_t pulseTicksSize = (pulseTicksAvailable > (SYS_TICKS_PER_BUFFER * 3 / 2 + 2))
                                  ? SYS_TICKS_PER_BUFFER + 2
                                  : SYS_TICKS_PER_BUFFER - 2;

    int16_t *samples = (int16_t *)stream;
    uint32_t sampleNum = len / sizeof(int16_t);

    const float audioPhaseDelta = 2.0F * (float)M_PI * BUZZER_FREQUENCY / AUDIO_SAMPLE_FREQUENCY;

    for (uint32_t i = 0; i < sampleNum; i++)
    {
        buzzer.audioPhase += audioPhaseDelta;
        if (buzzer.audioPhase > 2.0F * (float)M_PI)
            buzzer.audioPhase -= 2.0F * (float)M_PI;

        // Square-ish wave: first three odd harmonics
        float amplitude = sinf(buzzer.audioPhase) +
                          sinf(3 * buzzer.audioPhase) / 3 +
                          sinf(5 * buzzer.audioPhase) / 5;

        uint32_t pulseTicksIndex = (buzzer.pulseTicksTail + i * pulseTicksSize / sampleNum) & PULSE_BUFFER_MASK;
        float gain = buzzer.pulseTicks[pulseTicksIndex] ? buzzer.gain : 0.0F;

        samples[i] = (int16_t)(INT16_MAX * gain * amplitude);
    }

    buzzer.pulseTicksTail = (buzzer.pulseTicksTail + pulseTicksSize) & PULSE_BUFFER_MASK;
}

void initBuzzer(void)
{
    buzzer.gain = VOLUME_GAIN_MAX;

    SDL_AudioSpec audioSpec = {
        .freq = AUDIO_SAMPLE_FREQUENCY,
        .format = AUDIO_S16,
        .channels = 1,
        .samples = AUDIO_BUFFER_SIZE,
        .callback = onAudioBuffer,
    };

    // Legacy API: returns 0 on success, negative on error. Without an audio
    // device, the simulator runs silently.
    if (SDL_OpenAudio(&audioSpec, NULL) < 0)
    {
        printf("warning: could not open audio, buzzer disabled: %s\n", SDL_GetError());

        return;
    }

    buzzer.audioOpen = true;

    SDL_PauseAudio(0);
}

void onBuzzerTick(void)
{
    if (!buzzer.audioOpen)
        return;

    SDL_LockAudio();

    buzzer.pulseTicks[buzzer.pulseTicksHead] = buzzer.value;
    buzzer.pulseTicksHead = (buzzer.pulseTicksHead + 1) & PULSE_BUFFER_MASK;

    SDL_UnlockAudio();
}

void setBuzzerVolume(uint8_t volume)
{
    buzzer.gain = buzzerVolumeGains[volume];
}

void setBuzzer(bool value)
{
    buzzer.value = value;
}

#endif

#endif
