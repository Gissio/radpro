/*
 * Rad Pro
 * STM32 voice chip
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#if defined(VOICE)

#include <assert.h>

#include "../measurements/average.h"
#include "../measurements/cumulative.h"
#include "../measurements/instantaneous.h"
#include "../measurements/pulses.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../stm32/device.h"

// KT148A definitions

#define VOICE_TX_START_INDEX 0
#define VOICE_TX_START_LENGTH 30
#define VOICE_TX_BIT_SHORT_LENGTH 1
#define VOICE_TX_BIT_LONG_LENGTH 3
#define VOICE_TX_BIT_LENGTH (VOICE_TX_BIT_SHORT_LENGTH + VOICE_TX_BIT_LONG_LENGTH)
#define VOICE_TX_DATA_INDEX (VOICE_TX_START_INDEX + VOICE_TX_START_LENGTH)
#define VOICE_TX_DATA_LENGTH (8 * VOICE_TX_BIT_LENGTH)
#define VOICE_TX_STOP_INDEX (VOICE_TX_DATA_INDEX + VOICE_TX_DATA_LENGTH)
#define VOICE_TX_LENGTH (VOICE_TX_STOP_INDEX + 1)

// Idle time after each byte (25 ms)
#define VOICE_TX_GAP_TICKS 25

static_assert(VOICE_TX_TIMER_PERIOD <= 0x10000,
              "VOICE_TX_TIMER_PERIOD must fit in 16 bits");

enum
{
    VOICE_VOLUME0 = 0xe0,
    VOICE_VOLUME1 = 0xe1,
    VOICE_VOLUME2 = 0xe2,
    VOICE_VOLUME3 = 0xe3,
    VOICE_VOLUME4 = 0xe4,
    VOICE_VOLUME5 = 0xe5,
    VOICE_VOLUME6 = 0xe6,
    VOICE_VOLUME7 = 0xe7,
    VOICE_VOLUME8 = 0xe8,
    VOICE_VOLUME9 = 0xe9,
    VOICE_VOLUME10 = 0xea,
    VOICE_VOLUME11 = 0xeb,
    VOICE_VOLUME12 = 0xec,
    VOICE_VOLUME13 = 0xed,
    VOICE_VOLUME14 = 0xee,
    VOICE_VOLUME15 = 0xef,
    VOICE_SLEEP = 0xf0,
    VOICE_LOOP = 0xf2,
    VOICE_SEQUENCE = 0xf3,
    VOICE_ADDRESSEXTENSION = 0xf4,
    VOICE_RESET = 0xf5,
    VOICE_STOP = 0xfe,
};

// Voices (1-19 are spoken as their own index)

enum
{
    VOICE_NONE = 0x00,
    VOICE_ONE = 0x01,
    VOICE_TWO = 0x02,
    VOICE_THREE = 0x03,
    VOICE_TEN = 0x0a,
    VOICE_TWENTY = 0x14,
    VOICE_ZERO = 0x1c,
    VOICE_HUNDRED = 0x1d,
    VOICE_THOUSAND = 0x1e,
    VOICE_MILLION = 0x1f,
    VOICE_POINT = 0x20,
    VOICE_USVH = 0x21,
    VOICE_MREMH = 0x22,
    VOICE_USV = 0x23,
    VOICE_MREM = 0x24,
    VOICE_CPM = 0x25,
    VOICE_ON = 0x26,
    VOICE_OFF = 0x27,
    VOICE_YES = 0x28,
    VOICE_NO = 0x29,
    VOICE_MANUAL = 0x2a,
    VOICE_DEVICEOFF = 0x2b,
    VOICE_DEVICEON = 0x2c,
    VOICE_ALERT = 0x2d,
    VOICE_CPS = 0x2e,
    VOICE_DEVICECHARGING = 0x2f,
};

enum
{
    VOICE_ALERTSTATE_SOUND = 0,
    VOICE_ALERTSTATE_VOICE = 4,
    VOICE_ALERTSTATE_NUM = 5,
};

#define VOICE_SEQUENCE_MAX 32

#define VOICE_BSRR(value) (1U << (VOICE_TX_PIN + 0x10 * (1 - (value))))

typedef struct
{
    uint8_t data[VOICE_SEQUENCE_MAX];
    uint32_t size;
} VoiceSequence;

static const uint8_t voiceVolume[] = {VOICE_VOLUME3, VOICE_VOLUME7, VOICE_VOLUME11, VOICE_VOLUME15};

static struct
{
    volatile bool transmitting;

    uint32_t alertState;

    // Shared with onVoiceTick() (tick interrupt)
    volatile bool requestedSequenceUpdate;
    VoiceSequence requestedSequence;

    VoiceSequence sequence;
    uint32_t sequenceIndex;
    uint32_t gapTicks;

    uint32_t buffer[VOICE_TX_LENGTH];
} voice;

void initVoice(void)
{
    // GPIO
    gpio_set(VOICE_TX_PORT, VOICE_TX_PIN);
    gpio_setup(VOICE_TX_PORT, VOICE_TX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);

    gpio_setup(VOICE_BUSY_PORT, VOICE_BUSY_PIN, GPIO_MODE_INPUT_PULLUP);

    gpio_clear(VOICE_PROG_PORT, VOICE_PROG_PIN);
    gpio_setup(VOICE_PROG_PORT, VOICE_PROG_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);

    // Timer (registers survive clock gating: runs whenever clocked)
    rcc_enable_tim(VOICE_TX_TIMER);
    tim_setup_dma(VOICE_TX_TIMER);
    tim_set_prescaler_factor(VOICE_TX_TIMER, 1);
    tim_set_period(VOICE_TX_TIMER, VOICE_TX_TIMER_PERIOD);
    tim_enable(VOICE_TX_TIMER);
    rcc_disable_tim(VOICE_TX_TIMER);
}

// Transmission

static void sendVoiceData(uint32_t value)
{
    voice.buffer[VOICE_TX_START_INDEX] = VOICE_BSRR(0);

    uint32_t *bit = &voice.buffer[VOICE_TX_DATA_INDEX];
    for (uint32_t i = 0; i < 8; i++, bit += VOICE_TX_BIT_LENGTH, value >>= 1)
    {
        // 1: high 3, low 1; 0: high 1, low 3
        uint32_t longLow = (value & 1) ? VOICE_BSRR(0) : 0;

        bit[0] = VOICE_BSRR(1);
        bit[VOICE_TX_BIT_SHORT_LENGTH] = longLow ^ VOICE_BSRR(0);
        bit[VOICE_TX_BIT_LONG_LENGTH] = longLow;
    }

    voice.buffer[VOICE_TX_STOP_INDEX] = VOICE_BSRR(1);

    // Writing CCR also disables the channel, so CNDTR can be reloaded
    dma_setup_memory32_to_peripheral32(VOICE_TX_DMA_CHANNEL, (uint32_t)&VOICE_TX_PORT->BSRR, (uint32_t)voice.buffer, VOICE_TX_LENGTH);
    dma_enable(VOICE_TX_DMA_CHANNEL);
}

static bool isVoicePlaying(void)
{
    return !gpio_get(VOICE_BUSY_PORT, VOICE_BUSY_PIN);
}

void onVoiceTick(void)
{
    if (voice.requestedSequenceUpdate)
    {
        // Update sequence (barrier: read flag before data)
        __DMB();

        voice.sequence = voice.requestedSequence;
        voice.sequenceIndex = 0;

        if (!voice.transmitting)
        {
            rcc_enable_dma(VOICE_TX_DMA);
            rcc_enable_tim(VOICE_TX_TIMER);

            voice.transmitting = true;
        }

        voice.requestedSequenceUpdate = false;
    }

    if (voice.transmitting && !dma_is_active(VOICE_TX_DMA_CHANNEL))
    {
        if (voice.gapTicks)
            voice.gapTicks--;
        else if (voice.sequenceIndex < voice.sequence.size)
        {
            if ((voice.sequenceIndex == 0) || !isVoicePlaying())
            {
                sendVoiceData(voice.sequence.data[voice.sequenceIndex++]);

                voice.gapTicks = VOICE_TX_GAP_TICKS;
            }
        }
        else
        {
            // Finished DMA channel stays enabled with CNDTR = 0: idle
            rcc_disable_tim(VOICE_TX_TIMER);
            rcc_disable_dma(VOICE_TX_DMA);

            voice.transmitting = false;
        }
    }
}

// Sequence

static void clearVoiceQueue(void)
{
    while (voice.requestedSequenceUpdate)
        syncTick();

    voice.requestedSequence.size = 0;
}

static void pushVoiceQueue(uint32_t value)
{
    voice.requestedSequence.data[voice.requestedSequence.size++] = value;
}

static void sendVoiceQueue(void)
{
    // Barrier: write data before flag
    __DMB();

    voice.requestedSequenceUpdate = true;
}

// Number processing

static void pushVoiceQueueThousandsGroup(uint32_t value)
{
    if (value >= 100)
    {
        pushVoiceQueue(value / 100);
        pushVoiceQueue(VOICE_HUNDRED);
        value %= 100;
    }

    if (value >= 20)
    {
        pushVoiceQueue((VOICE_TWENTY - 2) + value / 10);
        value %= 10;
    }

    if (value)
        pushVoiceQueue(value);
}

static void pushVoiceQueueNumber(float value, uint32_t unit)
{
    // Also rejects NaN
    if (!(value >= 0.0F) || (value >= 1E9F))
        return;

    uint32_t integerPart;
    uint32_t fractionalPart;

    if (value >= 10000.0F)
    {
        integerPart = truncateDecimalMantissa(value, 4);
        fractionalPart = 0;
    }
    else
    {
        value *= 1000.0F;
        // Values below 1 have no integer digits: keep only the rounding digit
        uint32_t integerValue = value;
        int32_t exponent = integerValue ? getDecimalDigits(integerValue) : 0;
        int32_t mantissa = (exponent < 4) ? exponent : 4;
        uint32_t scaledValue = truncateDecimalMantissa(10.0F * value, mantissa) / 10;

        integerPart = scaledValue / 1000;
        fractionalPart = scaledValue % 1000;
    }

    // Rounding may carry past the largest speakable value
    if (integerPart >= 1000000000)
        return;

    if (integerPart == 0)
        pushVoiceQueue(VOICE_ZERO);

    for (uint32_t divisor = 1000000, suffix = VOICE_MILLION;
         divisor;
         divisor /= 1000, suffix--)
    {
        uint32_t group = integerPart / divisor;
        integerPart %= divisor;

        if (group)
        {
            pushVoiceQueueThousandsGroup(group);
            if (divisor > 1)
                pushVoiceQueue(suffix);
        }
    }

    if (fractionalPart)
    {
        pushVoiceQueue(VOICE_POINT);

        do
        {
            uint32_t digit = fractionalPart / 100;
            pushVoiceQueue(digit ? digit : VOICE_ZERO);

            fractionalPart = 10 * (fractionalPart % 100);
        } while (fractionalPart);
    }

    if (unit != VOICE_NONE)
        pushVoiceQueue(unit);
}

// Commands

// Scales Sv and rem to the spoken µSv and mrem
static const float voiceUnitScales[DOSE_UNITS_NUM] = {1E6F, 1E3F, 1.0F, 1.0F};
static const uint8_t voiceUnits[2][DOSE_UNITS_NUM] = {
    {VOICE_USVH, VOICE_MREMH, VOICE_CPM, VOICE_CPS},
    {VOICE_USV, VOICE_MREM, VOICE_NONE, VOICE_NONE},
};

static void pushVoiceQueueMeasurement(float value, bool isDose)
{
    uint32_t doseUnits = settings.doseUnits;

    value *= (isDose ? getDoseFactor() : getRateFactor()) * voiceUnitScales[doseUnits];

    pushVoiceQueueNumber(value, voiceUnits[isDose][doseUnits]);
}

NOINLINE static void startVoicePlayback(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
}

static void playVoiceMeasurement(float value, bool isDose)
{
    startVoicePlayback();
    pushVoiceQueueMeasurement(value, isDose);
    sendVoiceQueue();
}

void playVoiceInstantaneousRate(void)
{
    playVoiceMeasurement(getInstantaneousRate(), false);
}

void playVoiceAverageRate(void)
{
    playVoiceMeasurement(getAverageRate(), false);
}

void playVoiceCumulativeDose(void)
{
    playVoiceMeasurement(getCumulativeDosePulseCount(), true);
}

void playVoiceTest(void)
{
    startVoicePlayback();
    pushVoiceQueue(VOICE_ONE);
    pushVoiceQueue(VOICE_TWO);
    pushVoiceQueue(VOICE_THREE);
    sendVoiceQueue();
}

void playNumber(uint32_t value)
{
    startVoicePlayback();
    pushVoiceQueueNumber(value, VOICE_NONE);
    sendVoiceQueue();
}

void triggerVoiceAlert(bool measurementAlert)
{
    if (voice.requestedSequenceUpdate || voice.transmitting || isVoicePlaying())
        return;

    bool rateAlert = getInstantaneousRateAlertLevel();
    bool doseAlert = getCumulativeDoseAlertLevel();
    bool playSound = settings.alertSound;
    bool playVoice = measurementAlert && settings.alertVoice && (rateAlert || doseAlert);

    if (!playSound)
    {
        if (!playVoice)
            return;

        voice.alertState = VOICE_ALERTSTATE_VOICE;
    }
    else if (!playVoice)
        voice.alertState = VOICE_ALERTSTATE_SOUND;

    clearVoiceQueue();
    if (voice.alertState < VOICE_ALERTSTATE_VOICE)
    {
        pushVoiceQueue(voiceVolume[settings.soundAlertVolume]);
        pushVoiceQueue(VOICE_ALERT);
        if (settings.soundAlertStyle == SOUND_ALERTSTYLE_LONG)
            pushVoiceQueue(VOICE_ALERT);
    }
    else
    {
        pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
        if (rateAlert)
            pushVoiceQueueMeasurement(getInstantaneousRate(), false);
        if (doseAlert)
            pushVoiceQueueMeasurement(getCumulativeDosePulseCount(), true);
    }
    sendVoiceQueue();

    if (++voice.alertState >= VOICE_ALERTSTATE_NUM)
        voice.alertState = 0;
}

void clearVoiceAlert(void)
{
    voice.alertState = 0;
}

void stopVoice(void)
{
    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    sendVoiceQueue();
}

#endif

#endif
