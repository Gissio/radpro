/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "../devices/adc.h"
#include "../devices/buzzer.h"
#include "../devices/comm.h"
#include "../devices/display.h"
#include "../devices/keyboard.h"
#include "../devices/led.h"
#include "../devices/pulsesoundenable.h"
#include "../devices/tube.h"
#include "../devices/vibrator.h"
#include "../devices/voice.h"
#include "../extras/rng.h"
#include "../measurements/datalog.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/menu.h"

#define PULSE_DISPLAY_FLASH_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define PULSE_SOUND_CLICKS_TICKS ((uint32_t)(0.001 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_CHIRPS_TICKS ((uint32_t)(0.004 * SYSTICK_FREQUENCY))
#define PULSE_SOUND_BEEPS_TICKS ((uint32_t)(0.015 * SYSTICK_FREQUENCY))

#define PULSE_VIBRATION_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))

#define DISPLAY_FLASH_ALERT_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))
#define SOUND_WARNING_SHORT_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define SOUND_WARNING_LONG_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))
#define SOUND_ALARM_SHORT_TICKS ((uint32_t)(0.250 * SYSTICK_FREQUENCY))
#define SOUND_ALARM_LONG_TICKS ((uint32_t)(0.500 * SYSTICK_FREQUENCY))

#define POWERON_VIBRATION_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

#define LOCKMODE_BACKLIGHT_TICKS (10 * SYSTICK_FREQUENCY)

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

static struct
{
    volatile int32_t backlightTimer;
    int32_t requestedBacklightTimer;
    bool requestedBacklightReset;
    bool displayAwake;

#if defined(BUZZER)
    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;
#endif

#if defined(PULSE_LED)
    bool pulseLEDEnabled;
    volatile int32_t pulseLEDTimer;
#endif

#if defined(VIBRATOR)
    volatile int32_t vibrationTimer;
#endif

    int32_t keyboardTimer;

    int32_t heartbeatTimer;
    volatile uint32_t heartbeatCount;
    uint32_t lastHeartbeatCount;
} events;

volatile uint32_t currentTick;

static const int32_t displayTimerValues[] = {
#if defined(DISPLAY_MONOCHROME)
    1,
#endif
    10 * SYSTICK_FREQUENCY,
    30 * SYSTICK_FREQUENCY,
    60 * SYSTICK_FREQUENCY,
    120 * SYSTICK_FREQUENCY,
    300 * SYSTICK_FREQUENCY,
    -1,
};

#if defined(BUZZER)
static const uint32_t pulseSoundTicks[] = {
    PULSE_SOUND_CLICKS_TICKS,
    PULSE_SOUND_CHIRPS_TICKS,
    PULSE_SOUND_BEEPS_TICKS,
};
#endif

#if defined(BUZZER) || defined(VIBRATOR)
static const uint32_t alertSoundTicks[][2] = {
    {SOUND_WARNING_SHORT_TICKS, SOUND_WARNING_LONG_TICKS},
    {SOUND_ALARM_SHORT_TICKS, SOUND_ALARM_LONG_TICKS},
};
#endif

void syncTick(void);

void initEvents(void)
{
    events.keyboardTimer = KEY_TICKS;

    events.heartbeatTimer = SYSTICK_FREQUENCY;

    initEventsHardware();
}

void setupEvents(void)
{
    syncTick();

    events.heartbeatTimer = SYSTICK_FREQUENCY;
    events.lastHeartbeatCount = events.heartbeatCount;
}

// Timer

static TimerState tickTimer(volatile int32_t *timer)
{
    int32_t timerValue = *timer;

    if (timerValue == 0)
        return TIMER_OFF;
    else if (timerValue > 0)
    {
        timerValue--;

        *timer = timerValue;

        return (timerValue == 0) ? TIMER_ELAPSED : TIMER_RUNNING;
    }
    else
        return TIMER_RUNNING;
}

// Events

void onTick(void)
{
    // Measurement
#if defined(SIMULATOR)
    onTubeTick();
#endif

    // Pulses
    onPulseTick();

    // ADC
    onADCTick(events.heartbeatTimer);

    // Heartbeat
    if (tickTimer(&events.heartbeatTimer) == TIMER_ELAPSED)
    {
        events.heartbeatTimer = SYSTICK_FREQUENCY;
        events.heartbeatCount++;

        onPulseHeartbeat();
    }

    // Keyboard
    if (tickTimer(&events.keyboardTimer) == TIMER_ELAPSED)
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Display
    if (tickTimer(&events.backlightTimer) == TIMER_ELAPSED)
    {
        setBacklight(false);

        events.displayAwake = false;
    }

    // Buzzer
#if defined(BUZZER)
#if defined(SIMULATOR)
    onBuzzerTick();
#endif

    switch (tickTimer(&events.buzzerTimer))
    {
    case TIMER_ELAPSED:
        setBuzzer(false);

        break;

    case TIMER_RUNNING:
        switch (tickTimer(&events.buzzerNoiseTimer))
        {
        case TIMER_ELAPSED:
            setBuzzer(true);

            break;

        case TIMER_RUNNING:
            setBuzzer(getRandomBit());

            break;

        default:
            break;
        }

    default:
        break;
    }
#endif

    // Sound enable
#if defined(PULSESOUND_ENABLE)
    onPulseSoundEnableTick();
#endif

    // Voice
#if defined(VOICE)
    onVoiceTick();
#endif

    // Vibrator
#if defined(VIBRATOR)
    if (tickTimer(&events.vibrationTimer) == TIMER_ELAPSED)
        setVibrator(false);
#endif

    // Pulse LED
#if defined(PULSE_LED)
    if (tickTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
#endif
}

void syncTick(void)
{
#if !defined(SIMULATOR)
    sleep(1);
#endif
}

// Events

void updateEvents(void)
{
    updateComm();

    uint32_t heartbeatCount = events.heartbeatCount;
    if (events.lastHeartbeatCount != heartbeatCount)
    {
        events.lastHeartbeatCount = heartbeatCount;

        updateMeasurements();
        updateViewHeartbeat();
        updatePowerState();
    }

    updateDatalog();
    updateRNG();
    updateView();
}

// Backlight

static void setBacklightTimer(int32_t ticks)
{
    if ((ticks == -1) ||
        ((events.backlightTimer != -1) &&
         (ticks > events.backlightTimer)))
    {
        if (ticks != 1)
            setBacklight(true);

        events.backlightTimer = ticks;
    }
}

void requestBacklightTrigger(void)
{
    if (isInLockMode())
        events.requestedBacklightTimer = LOCKMODE_BACKLIGHT_TICKS;
    else
        events.requestedBacklightTimer = displayTimerValues[settings.displaySleep];
    events.requestedBacklightReset = true;
    events.displayAwake = true;
}

static void requestBacklightAlert(void)
{
    if ((events.requestedBacklightTimer >= 0) &&
        events.requestedBacklightTimer < DISPLAY_FLASH_ALERT_TICKS)
        events.requestedBacklightTimer = DISPLAY_FLASH_ALERT_TICKS;
}

bool isBacklightTriggerRequested(void)
{
    return events.requestedBacklightTimer != 0;
}

void triggerBacklight(void)
{
    syncTick();

    if (events.requestedBacklightReset)
        events.backlightTimer = 0;

    setBacklightTimer(events.requestedBacklightTimer);

    events.requestedBacklightTimer = 0;
    events.requestedBacklightReset = false;
}

void cancelBacklight(void)
{
    syncTick();

    events.backlightTimer = 0;
    events.displayAwake = false;

    setBacklight(false);
}

bool isBacklightActive(void)
{
    return events.backlightTimer != 0;
}

bool isDisplayAwake(void)
{
    return events.displayAwake;
}

// Buzzer

#if defined(BUZZER)
static void setBuzzerTimer(int32_t ticks, int32_t noiseTicks)
{
    if (ticks > events.buzzerTimer)
    {
        events.buzzerTimer = ticks;
        events.buzzerNoiseTimer = noiseTicks;
    }
}
#endif

// Vibration

#if defined(VIBRATOR)

static void setVibrationTimer(int32_t ticks)
{
    if (ticks > events.vibrationTimer)
    {
        setVibrator(true);

        events.vibrationTimer = ticks;
    }
}

#endif

void triggerVibration(void)
{
#if defined(VIBRATOR)
    syncTick();

    setVibrationTimer(POWERON_VIBRATION_TICKS);
#endif
}

// Pulse LED

#if defined(PULSE_LED)

void setPulseLEDIndication(bool value)
{
    syncTick();

    events.pulseLEDTimer = 0;
    events.pulseLEDEnabled = value;
}

static void setPulseLEDTimer(int32_t ticks)
{
    if (ticks > events.pulseLEDTimer)
    {
        setPulseLED(true);

        events.pulseLEDTimer = ticks;
    }
}

#endif

// Pulse and alert indication

void indicatePulse(void)
{
    // Called from interrupt thread

    if (settings.pulseDisplayFlash && isDisplayEnabled())
        setBacklightTimer(PULSE_DISPLAY_FLASH_TICKS);

#if defined(BUZZER)
    if (settings.pulseSound)
        setBuzzerTimer(pulseSoundTicks[settings.soundPulseType] + 1, events.buzzerTimer + 1);
#endif

#if defined(VIBRATOR)
    if (settings.pulseVibration)
        setVibrationTimer(PULSE_VIBRATION_TICKS);
#endif

#if defined(PULSE_LED)
    if (events.pulseLEDEnabled)
        setPulseLEDTimer(PULSE_LED_TICKS);
#endif
}

void triggerAlert(bool alarm)
{
    syncTick();

    if (settings.alertDisplayFlash)
        requestBacklightAlert();

#if defined(BUZZER) || defined(VIBRATOR)
    uint32_t alertTicks = alertSoundTicks[alarm][settings.soundAlertStyle];
#endif

#if defined(BUZZER)
    if (settings.alertSound)
        setBuzzerTimer(alertTicks, 1);
#endif

#if defined(VOICE)
    triggerVoiceAlert();
#endif

#if defined(VIBRATOR)
    if (settings.alertVibration)
        setVibrationTimer(alertTicks);
#endif
}
