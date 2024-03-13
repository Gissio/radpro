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

#define PULSELED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_BACKLIGHT_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define ALARM_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

typedef enum
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
} TimerState;

static uint32_t displayTimerValues[] = {
#if defined(DISPLAY_MONOCHROME)
    1,
#endif
    30 * SYSTICK_FREQUENCY,
    60 * SYSTICK_FREQUENCY,
    2 * 60 * SYSTICK_FREQUENCY,
    5 * 60 * SYSTICK_FREQUENCY,
#if defined(DISPLAY_MONOCHROME)
    1,
#endif
    -1,
};

static uint32_t buzzerTimerValues[] = {
    0,
    ((uint32_t)(0.001 * SYSTICK_FREQUENCY)),
    ((uint32_t)(0.015 * SYSTICK_FREQUENCY)),
};

volatile uint32_t eventsCurrentTick;

static struct
{
    bool lastPulseTimeCountInitialized;
    uint32_t lastPulseTimeCount;
    uint32_t deadTimeCount;

    volatile int32_t displayTimer;

#if defined(PULSELED)
    volatile int32_t pulseLEDTimer;
#endif

    volatile int32_t buzzerTimer;
    volatile int32_t buzzerNoiseTimer;

    volatile int32_t alarmTimer;

    int32_t keyboardTimer;

    int32_t measurementPeriodTimer;
    volatile uint32_t measurementPeriodUpdate;
    uint32_t lastMeasurementPeriodUpdate;
    bool measurementsEnabled;
} events;

void initEvents(void)
{
    initEventsController();

    events.deadTimeCount = UINT32_MAX;
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
    {
#if defined(DISPLAY_MONOCHROME)
        setDisplayBacklight(false);
#endif
    }

    // Pulse LED
#if defined(PULSELED)
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
    {
        setPulseLED(false);
    }
#endif

    // Alarm
    if (updateTimer(&events.alarmTimer) == TIMER_ELAPSED)
    {
#if defined(VIBRATOR)
        setVibrator(false);
#endif
    }
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

    events.displayTimer = 1;
#if defined(PULSELED)
    events.pulseLEDTimer = 1;
#endif
    events.buzzerTimer = 1;
    events.alarmTimer = 1;

    events.measurementsEnabled = false;
}

void dispatchEvents(void)
{
    sleep(0);

    uint32_t oneSecondUpdate = events.measurementPeriodUpdate;
    if (events.lastMeasurementPeriodUpdate != oneSecondUpdate)
    {
        events.lastMeasurementPeriodUpdate = oneSecondUpdate;

        updateMeasurements();
        updateDatalog();
        updateADC();
        updateGame();
        updateView();
    }

    dispatchViewEvents();
    dispatchCommEvents();
}

// Dead-time

float getTubeDeadTime(void)
{
    return events.deadTimeCount * timerCountToSeconds;
}

// Timers

#if defined(PULSELED)

static void setPulseLEDTimer(int32_t value)
{
    if (value > events.pulseLEDTimer)
    {
        setPulseLED(true);

        events.pulseLEDTimer = value;
    }
}

#endif

static bool isAlarmTimerActive(void);

static void setDisplayTimer(int32_t value)
{
#if defined(DISPLAY_MONOCHROME)
    if (value != 1)
        setDisplayBacklight(true);
#endif

    if ((value < 0) ||
        ((value > events.displayTimer) &&
         (events.displayTimer != -1)))
        events.displayTimer = value;
}

static void setBuzzerTimer(int32_t value, int32_t noiseValue)
{
    if (value > events.buzzerTimer)
    {
        events.buzzerTimer = value;
        events.buzzerNoiseTimer = noiseValue;
    }
}

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

void triggerPulse(void)
{
    if (isAlarmTimerActive())
        return;

#if defined(PULSELED)
    if (settings.pulseLED)
        setPulseLEDTimer(PULSELED_TICKS);
#endif

#if defined(DISPLAY_MONOCHROME)
    if (settings.displaySleep == DISPLAY_SLEEP_PULSE_FLASHES)
        setDisplayTimer(PULSE_BACKLIGHT_TICKS);
#endif

    setBuzzerTimer(buzzerTimerValues[settings.pulseClicks] + 1,
                   events.buzzerTimer + 1);
}

void triggerAlarm(void)
{
    syncTimerThread();

#if defined(PULSELED)
    setPulseLEDTimer(ALARM_TICKS);
#endif
    setBuzzerTimer(ALARM_TICKS, 1);
    setDisplayTimer(ALARM_TICKS);
#if defined(VIBRATOR)
    setVibrator(true);
#endif

    events.alarmTimer = ALARM_TICKS;
}

static bool isAlarmTimerActive(void)
{
    return events.alarmTimer != 0;
}
