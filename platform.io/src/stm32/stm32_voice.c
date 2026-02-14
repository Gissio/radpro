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
#define VOICE_TX_STOP_LENGTH 125
#define VOICE_TX_LENGTH (VOICE_TX_STOP_INDEX + VOICE_TX_STOP_LENGTH)

enum
{
    VOICE_VOLUME = 0xe0,
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
    VOICE_VOLUME12 = 0xed,
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

// Voices

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

#define VOICE_BSRR(value) (1 << (VOICE_TX_PIN + 0x10 * (1 - value)))

typedef struct
{
    uint8_t data[VOICE_SEQUENCE_MAX];
    uint32_t size;
} VoiceSequence;

static void setVoiceEnabled(void);
static bool isVoiceSending(void);
static bool isVoicePlaying(void);
static void sendVoiceData(uint8_t value);
static void disableVoice(void);
static void clearVoiceQueue(void);
static void pushVoiceQueue(uint8_t value);
static void sendVoiceQueue(void);

static const uint8_t voiceVolume[] = {VOICE_VOLUME3, VOICE_VOLUME7, VOICE_VOLUME11, VOICE_VOLUME15};

static struct
{
    volatile bool transmitting;

    uint32_t alertState;

    bool requestedSequenceUpdate;
    VoiceSequence requestedSequence;

    VoiceSequence sequence;
    uint32_t sequenceIndex;

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

    // Transmit buffer
    voice.buffer[VOICE_TX_START_INDEX] = VOICE_BSRR(0);
    for (uint32_t i = 0; i < 8; i++)
        voice.buffer[VOICE_TX_DATA_INDEX + i * VOICE_TX_BIT_LENGTH] = VOICE_BSRR(1);
    voice.buffer[VOICE_TX_STOP_INDEX] = VOICE_BSRR(1);
}

// static void wakeVoice(void)
// {
//     gpio_clear(VOICE_TX_PORT, VOICE_TX_PIN);
//     sleep(10);
//     gpio_set(VOICE_TX_PORT, VOICE_TX_PIN);
//     sleep(30);
// }

void onVoiceTick(void)
{
    if (voice.requestedSequenceUpdate)
    {
        // Update sequence
        voice.sequence = voice.requestedSequence;
        voice.sequenceIndex = 0;

        if (!voice.transmitting)
            setVoiceEnabled();

        voice.requestedSequenceUpdate = false;
    }

    if (voice.transmitting && !isVoiceSending())
    {
        // Send
        if (voice.sequenceIndex < voice.sequence.size)
        {
            if ((voice.sequenceIndex == 0) || !isVoicePlaying())
                sendVoiceData(voice.sequence.data[voice.sequenceIndex++]);
        }
        else
            disableVoice();
    }
}

static void setVoiceEnabled(void)
{
    // RCC
    rcc_enable_dma(VOICE_TX_DMA);
    rcc_enable_tim(VOICE_TX_TIMER);

    // Timer
    uint32_t period = VOICE_TX_TIMER_PERIOD;
    uint32_t onTime = 0;
    uint32_t prescalerFactor = prescalePWMParameters(&period, &onTime);

    tim_setup_dma(VOICE_TX_TIMER);
    tim_set_prescaler_factor(VOICE_TX_TIMER, prescalerFactor);
    tim_set_period(VOICE_TX_TIMER, period);
    tim_enable(VOICE_TX_TIMER);

    voice.transmitting = true;
}

static void disableVoice(void)
{
    // Timer
    tim_disable(VOICE_TX_TIMER);

    // DMA
    dma_disable(VOICE_TX_DMA_CHANNEL);

    // RCC
    rcc_disable_tim(VOICE_TX_TIMER);
    rcc_disable_dma(VOICE_TX_DMA);

    voice.transmitting = false;
}

static void sendVoiceData(uint8_t value)
{
    for (uint32_t i = 0; i < 8; i++)
    {
        uint32_t bufferIndex = VOICE_TX_DATA_INDEX + i * VOICE_TX_BIT_LENGTH;

        if (value & 1)
        {
            voice.buffer[bufferIndex + VOICE_TX_BIT_SHORT_LENGTH] = 0;
            voice.buffer[bufferIndex + VOICE_TX_BIT_LONG_LENGTH] = VOICE_BSRR(0);
        }
        else
        {
            voice.buffer[bufferIndex + VOICE_TX_BIT_SHORT_LENGTH] = VOICE_BSRR(0);
            voice.buffer[bufferIndex + VOICE_TX_BIT_LONG_LENGTH] = 0;
        }

        value >>= 1;
    }

    dma_setup_memory32_to_peripheral32(VOICE_TX_DMA_CHANNEL, (uint32_t *)&VOICE_TX_PORT->BSRR, voice.buffer, VOICE_TX_LENGTH);
    dma_enable(VOICE_TX_DMA_CHANNEL);
}

static bool isVoiceSending(void)
{
    return (VOICE_TX_DMA_CHANNEL->CNDTR != 0);
}

static bool isVoicePlaying(void)
{
    return !gpio_get(VOICE_BUSY_PORT, VOICE_BUSY_PIN);
}

// Sequence

static void pushVoiceQueue(uint8_t value);

static void clearVoiceQueue(void)
{
    while (voice.requestedSequenceUpdate)
        sleep(1);

    voice.requestedSequence.size = 0;
}

static void pushVoiceQueue(uint8_t value)
{
    voice.requestedSequence.data[voice.requestedSequence.size++] = value;
}

static void sendVoiceQueue(void)
{
    voice.requestedSequenceUpdate = true;
}

// Number processing

static void pushVoiceQueueThousandsGroup(uint32_t value)
{
    if (value >= 100)
    {
        uint32_t hundreds = value / 100;
        pushVoiceQueue((VOICE_ONE - 1) + hundreds);
        pushVoiceQueue(VOICE_HUNDRED);
        value %= 100;
    }
    if (value >= 20)
    {
        uint32_t tens = value / 10;
        pushVoiceQueue((VOICE_TWENTY - 2) + tens);
        value %= 10;
    }
    else if (value >= 10)
    {
        pushVoiceQueue((VOICE_TEN - 10) + value);

        return;
    }
    if (value > 0)
        pushVoiceQueue((VOICE_ONE - 1) + value);
}

static bool pushVoiceQueueNumber(float value)
{
    if ((value < 0.000001F) || (value >= 1E9F))
        return false;

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
        int32_t exponent = getDecimalDigits(value);
        int32_t mantissa = (exponent < 4) ? exponent : 4;
        uint32_t scaledValue = truncateDecimalMantissa(10.0F * value, mantissa) / 10;

        integerPart = scaledValue / 1000;
        fractionalPart = scaledValue % 1000;
    }

    if (integerPart == 0)
        pushVoiceQueue(VOICE_ZERO);

    if (integerPart >= 1000000)
    {
        pushVoiceQueueThousandsGroup(integerPart / 1000000);
        pushVoiceQueue(VOICE_MILLION);
        integerPart %= 1000000;
    }

    if (integerPart >= 1000)
    {
        pushVoiceQueueThousandsGroup(integerPart / 1000);
        pushVoiceQueue(VOICE_THOUSAND);
        integerPart %= 1000;
    }

    if (integerPart)
        pushVoiceQueueThousandsGroup(integerPart);

    if (fractionalPart)
    {
        pushVoiceQueue(VOICE_POINT);

        while (fractionalPart)
        {
            uint32_t digit = fractionalPart / 100;
            if (!digit)
                pushVoiceQueue(VOICE_ZERO);
            else
                pushVoiceQueue((VOICE_ONE - 1) + digit);

            fractionalPart = 10 * (fractionalPart % 100);
        }
    }

    return true;
}

// Commands

static const uint8_t voiceRateUnits[] = {VOICE_USVH, VOICE_MREMH, VOICE_CPM, VOICE_CPS};
static const uint8_t voiceDoseUnits[] = {VOICE_USV, VOICE_MREM, VOICE_NONE, VOICE_NONE};

static void pushVoiceQueueRate(float value)
{
    value *= getRateFactor();
    if (settings.doseUnits == DOSE_UNITS_SIEVERTS)
        value *= 1E6F;
    else if (settings.doseUnits == DOSE_UNITS_REM)
        value *= 1E3F;

    uint8_t voiceRateUnit = voiceRateUnits[settings.doseUnits];

    if (pushVoiceQueueNumber(value))
    {
        if (voiceRateUnit != VOICE_NONE)
            pushVoiceQueue(voiceRateUnit);
    }
}

static void pushVoiceQueueDose(float value)
{
    value *= getDoseFactor();
    if (settings.doseUnits == DOSE_UNITS_SIEVERTS)
        value *= 1E6F;
    else if (settings.doseUnits == DOSE_UNITS_REM)
        value *= 1E3F;

    uint8_t voiceRateUnit = voiceDoseUnits[settings.doseUnits];

    if (pushVoiceQueueNumber(value))
    {
        if (voiceRateUnit != VOICE_NONE)
            pushVoiceQueue(voiceRateUnit);
    }
}

void playVoiceInstantaneousRate(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
    pushVoiceQueueRate(getInstantaneousRate());
    sendVoiceQueue();
}

void playVoiceAverageRate(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
    pushVoiceQueueRate(getAverageRate());
    sendVoiceQueue();
}

void playVoiceCumulativeDose(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
    pushVoiceQueueDose(getCumulativeDosePulseCount());
    sendVoiceQueue();
}

void playAlertTest(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundAlertVolume]);
    pushVoiceQueue(VOICE_ALERT);
    if (settings.soundAlertStyle == SOUND_ALERTSTYLE_LONG)
        pushVoiceQueue(VOICE_ALERT);
    sendVoiceQueue();
}

void playVoiceTest(void)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
    pushVoiceQueue(VOICE_ONE);
    pushVoiceQueue(VOICE_TWO);
    pushVoiceQueue(VOICE_THREE);
    sendVoiceQueue();
}

void playNumber(uint32_t value)
{
    clearVoiceAlert();

    clearVoiceQueue();
    pushVoiceQueue(VOICE_STOP);
    pushVoiceQueue(voiceVolume[settings.soundVoiceVolume]);
    if (value == 0)
        pushVoiceQueue(VOICE_ZERO);
    else
        pushVoiceQueueNumber(value);
    sendVoiceQueue();
}

void triggerVoiceAlert(void)
{
    if (voice.transmitting || isVoicePlaying())
        return;

    bool playSound = settings.alertSound;
    bool playVoice = settings.alertVoice && (getInstantaneousRateAlertLevel() || getCumulativeDoseAlertLevel());

    if (!playSound && !playVoice)
        return;
    else if (playSound && !playVoice)
        voice.alertState = VOICE_ALERTSTATE_SOUND;
    else if (!playSound && playVoice)
        voice.alertState = VOICE_ALERTSTATE_VOICE;

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
        if (getInstantaneousRateAlertLevel())
            pushVoiceQueueRate(getInstantaneousRate());
        if (getCumulativeDoseAlertLevel())
            pushVoiceQueueDose(getCumulativeDosePulseCount());
    }

    sendVoiceQueue();

    voice.alertState = (voice.alertState + 1) % VOICE_ALERTSTATE_NUM;
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
