/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2023 Gissio
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
#include "rng.h"
#include "settings.h"
#include "tube.h"

#define PULSE_LED_TICKS ((uint32_t)(0.050 * SYS_TICK_FREQUENCY))
#define PULSE_BACKLIGHT_TICKS ((uint32_t)(0.050 * SYS_TICK_FREQUENCY))
#define ALARM_TICKS ((uint32_t)(0.100 * SYS_TICK_FREQUENCY))

enum TimerState
{
    TIMER_OFF = -1,
    TIMER_ELAPSED,
    TIMER_RUNNING,
};

static uint32_t displayTimerValues[] = {
#if defined(DISPLAY_MONO)
    1,
#endif
    30 * SYS_TICK_FREQUENCY,
    60 * SYS_TICK_FREQUENCY,
    2 * 60 * SYS_TICK_FREQUENCY,
    5 * 60 * SYS_TICK_FREQUENCY,
#if defined(DISPLAY_MONO)
    1,
#endif
    -1,
};

static uint32_t buzzerTimerValues[] = {
    0,
    ((uint32_t)(0.001 * SYS_TICK_FREQUENCY)),
    ((uint32_t)(0.015 * SYS_TICK_FREQUENCY)),
};

volatile uint32_t eventsCurrentTick;

static struct
{
    bool pulseTimeLastInitialized;
    uint32_t pulseTimeLast;
    uint32_t deadTime;

    volatile int32_t displayTimer;

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

    events.deadTime = SYS_FREQUENCY;
    events.measurementPeriodTimer = SYS_TICK_FREQUENCY;
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
    // Display

    switch (updateTimer(&events.displayTimer))
    {
    case TIMER_ELAPSED:
#if defined(DISPLAY_MONO)

        setBacklight(false);

#elif defined(DISPLAY_COLOR)

        setDisplay(false);

#endif
        break;

#if defined(DISPLAY_MONO)

    case TIMER_RUNNING:
        setBacklight(!getBacklight());
        break;

#endif
    default:
        break;
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
        uint32_t pulseTime;

        while (getTubePulse(&pulseTime))
        {
            onRNGPulse(pulseTime);

            if (events.pulseTimeLastInitialized)
            {
                uint32_t duration = pulseTime - events.pulseTimeLast;

                if (duration < events.deadTime)
                    events.deadTime = duration;
            }
            else
                events.pulseTimeLastInitialized = true;
            events.pulseTimeLast = pulseTime;

            pulseCount++;
        }

        onMeasurementTick(pulseCount);

        if (updateTimer(&events.measurementPeriodTimer) == TIMER_ELAPSED)
        {
            events.measurementPeriodTimer = SYS_TICK_FREQUENCY;
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

    updateTimer(&events.alarmTimer);

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

void startEvents(void)
{
    syncTimerThread();

    events.measurementsEnabled = true;
}

void stopEvents(void)
{
    syncTimerThread();

#if defined(PULSE_LED)

    events.pulseLEDTimer = 1;

#endif
    events.buzzerTimer = 1;
    events.displayTimer = 1;
    events.alarmTimer = 1;

    events.measurementsEnabled = false;
}

void updateEvents(void)
{
    sleep(0);

    uint32_t oneSecondUpdate = events.measurementPeriodUpdate;
    if (events.lastMeasurementPeriodUpdate != oneSecondUpdate)
    {
        events.lastMeasurementPeriodUpdate = oneSecondUpdate;

        updateMeasurements();
        writeDatalogEntry(true);
        updateADC();
        updateGameTimer();
        refreshView();
    }

    updateView();
    updateComm();
}

// Dead-time

float getDeadTime(void)
{
    return events.deadTime * (1.0F / SYS_FREQUENCY);
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

static void setDisplayTimer(int32_t value)
{
#if defined(DISPLAY_COLOR)

    setDisplay(true);

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

#if defined(DISPLAY_MONO)

    setDisplayTimer(displayTimerValues[settings.displayBacklight]);

#elif defined(DISPLAY_COLOR)

    setDisplayTimer(displayTimerValues[settings.displaySleep]);

#endif
}

bool isDisplayTimerActive(void)
{
    return events.displayTimer != 0;
}

void triggerPulse(void)
{
    if (isAlarmTimerActive())
        return;

#if defined(PULSE_LED)

    if (settings.pulseLED)
        setPulseLEDTimer(PULSE_LED_TICKS);

#endif

#if defined(DISPLAY_MONO)

    if (settings.displayBacklight == DISPLAY_BACKLIGHT_PULSE_FLASHES)
        setDisplayTimer(PULSE_BACKLIGHT_TICKS);

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
    setDisplayTimer(ALARM_TICKS);

    events.alarmTimer = ALARM_TICKS;
}

static bool isAlarmTimerActive(void)
{
    return events.alarmTimer != 0;
}
