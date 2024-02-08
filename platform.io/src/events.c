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

#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define PULSE_BACKLIGHT_TICKS ((uint32_t)(0.050 * SYSTICK_FREQUENCY))
#define ALARM_TICKS ((uint32_t)(0.100 * SYSTICK_FREQUENCY))

enum TimerState
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
};

static uint32_t backlightTimerValues[] = {
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

    volatile int32_t backlightTimer;

#if defined(PULSE_LED)
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
    initEventsHardware();

    events.deadTimeCount = UINT32_MAX;
    events.measurementPeriodTimer = SYSTICK_FREQUENCY;
    events.keyboardTimer = KEY_TICKS;
}

static enum TimerState updateTimer(volatile int32_t *timer)
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
    // Backlight

    if (updateTimer(&events.backlightTimer) == TIMER_ELAPSED)
    {
        setDisplayBacklight(false);
#if defined(DISPLAY_COLOR)
        setDisplay(false);
#endif
    }

    // Pulse LED

#if defined(PULSE_LED)
    if (updateTimer(&events.pulseLEDTimer) == TIMER_ELAPSED)
        setPulseLED(false);
#endif

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

    // Buzzer

#if defined(SDLSIM)
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

    // Alarm

    if (updateTimer(&events.alarmTimer) == TIMER_ELAPSED)
    {
#if defined(VIBRATOR)
        setVibrator(false);
#endif
    }

    // Keyboard

    if (updateTimer(&events.keyboardTimer) == TIMER_ELAPSED)
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardTick();
    }
}

uint32_t getTick(void)
{
    return eventsCurrentTick;
}

// Events

static void syncTimerThread(void)
{
    sleep(1);
}

void setEventHandling(bool value)
{
    syncTimerThread();

    if (!value)
    {
        events.backlightTimer = 1;
#if defined(PULSE_LED)
        events.pulseLEDTimer = 1;
#endif
        events.buzzerTimer = 1;
        events.alarmTimer = 1;
    }

    events.measurementsEnabled = value;
}

void dispatchEvents(void)
{
    sleep(0);

    uint32_t oneSecondUpdate = events.measurementPeriodUpdate;
    if (events.lastMeasurementPeriodUpdate != oneSecondUpdate)
    {
        events.lastMeasurementPeriodUpdate = oneSecondUpdate;

        onMeasurementsOneSecond();
        writeDatalogEntry(true);
        updateADC();
        onGameOneSecond();
        updateView();
    }

    dispatchViewEvents();
    dispatchCommEvents();
}

// Dead-time

float getDeadTime(void)
{
    return events.deadTimeCount * timerCountToSeconds;
}

// Timers

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

static bool isAlarmTimerActive(void);

static void setBacklightTimer(int32_t value)
{
    setDisplay(true);
    setDisplayBacklight(true);

    if ((value < 0) ||
        ((value > events.backlightTimer) &&
         (events.backlightTimer != -1)))
        events.backlightTimer = value;
}

static void setBuzzerTimer(int32_t value, int32_t noiseValue)
{
    if (value > events.buzzerTimer)
    {
        events.buzzerTimer = value;
        events.buzzerNoiseTimer = noiseValue;
    }
}

void triggerBacklight(void)
{
    syncTimerThread();

    events.backlightTimer = 0;

    setBacklightTimer(backlightTimerValues[settings.displayTimer]);
}

bool isBacklightTimerActive(void)
{
    return events.backlightTimer != 0;
}

void triggerPulse(void)
{
    if (isAlarmTimerActive())
        return;

#if defined(PULSE_LED)

    if (settings.pulseLED)
        setPulseLEDTimer(PULSE_LED_TICKS);

#endif

#if defined(DISPLAY_MONOCHROME)

    if (settings.displayTimer == DISPLAY_TIMER_PULSE_FLASHES)
        setBacklightTimer(PULSE_BACKLIGHT_TICKS);

#endif

    setBuzzerTimer(buzzerTimerValues[settings.pulseClicks] + 1,
                   events.buzzerTimer + 1);
}

void triggerAlarm(void)
{
    syncTimerThread();

#if defined(PULSE_LED)
    setPulseLEDTimer(ALARM_TICKS);
#endif
    setBuzzerTimer(ALARM_TICKS, 1);
    setBacklightTimer(ALARM_TICKS);
#if defined(VIBRATOR)
    setVibrator(true);
#endif

    events.alarmTimer = ALARM_TICKS;
}

static bool isAlarmTimerActive(void)
{
    return events.alarmTimer != 0;
}
