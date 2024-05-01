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
#include "measurements.h"
#include "pulseled.h"
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

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

volatile uint32_t eventsCurrentTick;

static struct
{
    bool pulsesThresholdExceeded;

    bool lastPulseTimeCountInitialized;
    uint32_t lastPulseTimeCount;
    uint32_t deadTimeCount;

    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;

    volatile int32_t displayTimer;

#if defined(PULSE_LED)
    volatile int32_t pulseLEDTimer;
#endif

#if defined(VIBRATOR)
    volatile int32_t vibratorTimer;
#endif

    int32_t keyboardTimer;

    int32_t measurementPeriodTimer;
    volatile uint32_t measurementPeriodUpdate;
    uint32_t lastMeasurementPeriodUpdate;
    bool measurementsEnabled;
} events;

static uint32_t displayTimerValues[] = {
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

void initEvents(void)
{
    initEventsController();

    events.deadTimeCount = PULSE_MEASUREMENT_FREQUENCY;
    events.measurementPeriodTimer = SYSTICK_FREQUENCY;
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
    if (events.measurementsEnabled)
    {
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

        if (updateTimer(&events.measurementPeriodTimer) == TIMER_ELAPSED)
        {
            events.measurementPeriodTimer = SYSTICK_FREQUENCY;
            events.measurementPeriodUpdate++;

            onMeasurementPeriod();
        }
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
    if (updateTimer(&events.displayTimer) == TIMER_ELAPSED)
        setDisplayBacklightOn(false);

#if defined(PULSE_LED)
    // Pulse LED
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
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

void enableMeasurements(void)
{
    events.measurementsEnabled = true;
}

void disableMeasurements(void)
{
    syncTimerThread();

    events.buzzerTimer = 1;
    events.displayTimer = 1;
#if defined(PULSE_LED)
    events.pulseLEDTimer = 1;
#endif
#if defined(VIBRATOR)
    events.vibratorTimer = 1;
#endif

    events.measurementsEnabled = false;
}

void dispatchEvents(void)
{
    sleep(0);

    dispatchCommEvents();
    updateDatalog();

    uint32_t periodUpdate = events.measurementPeriodUpdate;
    if (events.lastMeasurementPeriodUpdate != periodUpdate)
    {
        events.lastMeasurementPeriodUpdate = periodUpdate;

        updateMeasurements();
        updateADC();
        updateGame();
        updateView();
    }

    dispatchViewEvents();
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

static void setDisplayTimer(int32_t value)
{
    if ((value < 0) ||
        ((value > events.displayTimer) &&
         (events.displayTimer != -1)))
    {
        if (value != 1)
            setDisplayBacklightOn(true);

        events.displayTimer = value;
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

void triggerDisplay(void)
{
    syncTimerThread();

    events.displayTimer = 0;

    setDisplayTimer(displayTimerValues[settings.displaySleep]);
}

bool isDisplayTimerActive(void)
{
    return events.displayTimer != 0;
}

void setPulsesThresholdExceeded(bool value)
{
    events.pulsesThresholdExceeded = value;
}

bool isPulsesThresholdExceeded(void)
{
    return events.pulsesThresholdExceeded;
}

void triggerPulse(void)
{
    if (!events.pulsesThresholdExceeded)
        return;

    if (settings.pulseClicks)
        setBuzzerTimer(pulseClickTicks[settings.pulseClicks] + 1,
                       events.buzzerTimer + 1);
#if defined(PULSE_LED)
    if (settings.pulseLED)
        setPulseLEDTimer(PULSE_LED_TICKS);
#endif
    if (settings.pulseFlashes)
        setDisplayTimer(PULSE_FLASH_TICKS);
#if defined(VIBRATOR)
    if (settings.pulseVibrations)
        setVibratorTimer(pulseVibrationTicks[settings.pulseVibrations]);
#endif
}

void triggerAlarm(void)
{
    syncTimerThread();

#if defined(PULSE_LED)
    setPulseLEDTimer(ALARM_TICKS);
#endif
    setBuzzerTimer(ALARM_TICKS, 1);
    setDisplayTimer(ALARM_FLASH_TICKS);
#if defined(VIBRATOR)
    setVibratorTimer(ALARM_TICKS);
#endif
}
