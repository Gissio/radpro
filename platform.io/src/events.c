/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "adc.h"
#include "buzzer.h"
#include "cmath.h"
#include "comm.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "game.h"
#include "keyboard.h"
#include "led.h"
#include "measurements.h"
#include "rng.h"
#include "settings.h"
#include "tube.h"
#include "vibrator.h"

#if !defined(SPEAKER)
#define PULSE_CLICK_QUIET_TICKS ((uint32_t)(0.001 * SYSTICK_FREQUENCY))
#else
#define PULSE_CLICK_QUIET_TICKS ((uint32_t)(0.004 * SYSTICK_FREQUENCY))
#endif
#define PULSE_CLICK_LOUD_TICKS ((uint32_t)(0.015 * SYSTICK_FREQUENCY))
#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_FLASH_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_VIBRATION_WEAK_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_VIBRATION_STRONG_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

#define ALARM_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))
#define ALARM_FLASH_TICKS ((uint32_t)(0.200 * SYSTICK_FREQUENCY))

#define POWERON_TEST_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

volatile uint32_t eventsCurrentTick;

static struct
{
    bool pulseThresholding;

    bool lastPulseTimeCountInitialized;
    uint32_t lastPulseTimeCount;
    uint32_t deadTimeCount;

    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;

    int32_t requestedDisplayBacklightTimer;
    bool requestedDisplayBacklightReset;
    volatile int32_t displayBacklightTimer;

#if defined(PULSE_LED)
    volatile int32_t pulseLEDTimer;
#endif

#if defined(ALERT_LED)
    volatile int32_t alertLEDTimer;
#endif

#if defined(VIBRATOR)
    volatile int32_t vibratorTimer;
#endif

    int32_t keyboardTimer;

    int32_t periodTimer;
    volatile uint32_t periodUpdate;
    uint32_t lastPeriodUpdate;
} events;

static int32_t displayTimerValues[] = {
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

static uint32_t pulseClickTicks[] = {
    0,
    PULSE_CLICK_QUIET_TICKS,
    PULSE_CLICK_LOUD_TICKS,
};

#if defined(VIBRATOR)
static uint32_t pulseVibrationTicks[] = {
    0,
    PULSE_VIBRATION_WEAK_TICKS,
    PULSE_VIBRATION_STRONG_TICKS,
};
#endif

void resetEvents(void)
{
    events.deadTimeCount = PULSE_MEASUREMENT_FREQUENCY;
    events.periodTimer = SYSTICK_FREQUENCY;
    events.keyboardTimer = KEY_TICKS;
}

static TimerState updateTimer(volatile int32_t *timer)
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

void onTick(void)
{
    // Measurement
    uint32_t pulseCount = 0;
    uint32_t pulseTimeCount;

    while (getTubePulse(&pulseTimeCount))
    {
        onRNGPulse(pulseTimeCount);

        if (events.lastPulseTimeCountInitialized)
        {
            uint32_t deltaTimeCount = pulseTimeCount - events.lastPulseTimeCount;

            if (deltaTimeCount < events.deadTimeCount)
                events.deadTimeCount = deltaTimeCount;
        }
        else
            events.lastPulseTimeCountInitialized = true;
        events.lastPulseTimeCount = pulseTimeCount;

        pulseCount++;
    }

    onMeasurementTick(pulseCount);

    if (updateTimer(&events.periodTimer) == TIMER_ELAPSED)
    {
        events.periodTimer = SYSTICK_FREQUENCY;
        events.periodUpdate++;

        onMeasurementPeriod();
    }

    // Keyboard
    if (updateTimer(&events.keyboardTimer) == TIMER_ELAPSED)
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Buzzer
#if defined(SIMULATOR)
    updateBuzzer();
#endif

    switch (updateTimer(&events.buzzerTimer))
    {
    case TIMER_ELAPSED:
        setBuzzer(false);

        break;

    case TIMER_RUNNING:
        switch (updateTimer(&events.buzzerNoiseTimer))
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

    // Display
    if (updateTimer(&events.displayBacklightTimer) == TIMER_ELAPSED)
        setDisplayBacklight(false);

#if defined(PULSE_LED)
    // Pulse LED
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
#endif

#if defined(ALERT_LED)
    // Alert LED
    if (updateTimer(&events.alertLEDTimer) == TIMER_ELAPSED)
        setAlertLED(false);
#endif

#if defined(VIBRATOR)
    // Vibrator
    if (updateTimer(&events.vibratorTimer) == TIMER_ELAPSED)
        setVibrator(false);
#endif
}

uint32_t getTick(void)
{
    return eventsCurrentTick;
}

// Events

void syncTimerThread(void)
{
    sleep(1);
}

void dispatchEvents(void)
{
    sleep(0);

    dispatchViewEvents();
    dispatchCommEvents();
    updateDatalog();

    uint32_t periodUpdate = events.periodUpdate;
    if (events.lastPeriodUpdate != periodUpdate)
    {
        events.lastPeriodUpdate = periodUpdate;

        updateMeasurements();
        updateADC();
        updateViewPeriod();
    }
}

// Dead time

float getTubeDeadTime(void)
{
    return events.deadTimeCount * timerCountToSeconds;
}

// Timers

static void setBuzzerTimer(int32_t value, int32_t noiseValue)
{
    if (value > events.buzzerTimer)
    {
        events.buzzerTimer = value;
        events.buzzerNoiseTimer = noiseValue;
    }
}

static void setDisplayBacklightTimer(int32_t value)
{
    if ((value == -1) ||
        ((events.displayBacklightTimer != -1) &&
         (value > events.displayBacklightTimer)))
    {
        if (value != 1)
            setDisplayBacklight(true);

        events.displayBacklightTimer = value;
    }
}

#if defined(PULSE_LED)
static void setPulseLEDTimer(int32_t value)
{
    if (value > events.pulseLEDTimer)
    {
        setPulseLED(true);

        events.pulseLEDTimer = value;
    }
}
#endif

#if defined(ALERT_LED)
static void setAlertLEDTimer(int32_t value)
{
    if (value > events.alertLEDTimer)
    {
        setAlertLED(true);

        events.alertLEDTimer = value;
    }
}
#endif

#if defined(VIBRATOR)
static void setVibratorTimer(int32_t value)
{
    if (value > events.vibratorTimer)
    {
        setVibrator(true);

        events.vibratorTimer = value;
    }
}
#endif

void requestDisplayBacklightTrigger(void)
{
    events.requestedDisplayBacklightTimer = displayTimerValues[settings.displaySleep];
    events.requestedDisplayBacklightReset = true;
}

static void requestDisplayBacklightAlarm(void)
{
    if ((events.requestedDisplayBacklightTimer >= 0) &&
        events.requestedDisplayBacklightTimer < ALARM_FLASH_TICKS)
        events.requestedDisplayBacklightTimer = ALARM_FLASH_TICKS;
}

bool isDisplayBacklightTriggerRequested(void)
{
    return events.requestedDisplayBacklightTimer != 0;
}

void triggerDisplayBacklight(void)
{
    if (events.requestedDisplayBacklightReset)
        events.displayBacklightTimer = 0;

    setDisplayBacklightTimer(events.requestedDisplayBacklightTimer);

    events.requestedDisplayBacklightTimer = 0;
    events.requestedDisplayBacklightReset = false;
}

void cancelDisplayBacklight(void)
{
    events.displayBacklightTimer = 1;
}

bool isDisplayBacklightActive(void)
{
    return events.displayBacklightTimer != 0;
}

void enablePulseThresholding(bool value)
{
    events.pulseThresholding = value;
}

bool isPulseThresholdingEnabled(void)
{
    return events.pulseThresholding;
}

void triggerPulse(void)
{
    if (!events.pulseThresholding)
    {
        if (settings.pulseClicks)
            setBuzzerTimer(pulseClickTicks[settings.pulseClicks] + 1,
                           events.buzzerTimer + 1);

#if defined(PULSE_LED)
        if (settings.pulseLED)
            setPulseLEDTimer(PULSE_LED_TICKS);
#endif

        if (settings.pulseFlashes && isDisplayEnabled())
            setDisplayBacklightTimer(PULSE_FLASH_TICKS);

#if defined(VIBRATOR)
        if (settings.pulseVibrations)
            setVibratorTimer(pulseVibrationTicks[settings.pulseVibrations]);
#endif
    }
}

void triggerAlarm(void)
{
    syncTimerThread();

    setBuzzerTimer(ALARM_TICKS, 1);

    requestDisplayBacklightAlarm();

#if defined(ALERT_LED)
    setAlertLEDTimer(ALARM_TICKS);
#elif defined(PULSE_LED)
    setPulseLEDTimer(ALARM_TICKS);
#endif

#if defined(VIBRATOR)
    setVibratorTimer(ALARM_TICKS);
#endif
}

void triggerPowerOnTest(void)
{
    syncTimerThread();

    setVibratorTimer(POWERON_TEST_TICKS, 1);
}
