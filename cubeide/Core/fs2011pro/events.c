/*
 * FS2011 Pro
 * Event handler
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#include "backlight.h"
#include "cmath.h"
#include "events.h"
#include "game.h"
#include "keyboard.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
#include "buzzer.h"
#include "ui.h"

#define PULSE_SOUND_CLICK_TIME 0.0015F
#define PULSE_SOUND_BEEP_TIME 0.015F
#define ALARM_TIME 0.25F

#define PULSE_SOUND_QUIET_TICKS (int)(BUZZER_TICK_FREQUENCY * PULSE_SOUND_CLICK_TIME)
#define PULSE_SOUND_LOUD_TICKS (int)(BUZZER_TICK_FREQUENCY * PULSE_SOUND_BEEP_TIME)
#define ALARM_TICKS (int)(BUZZER_TICK_FREQUENCY * ALARM_TIME)

struct Events
{
    bool isInitialized;
    bool isWelcomed;

    unsigned int tick;

    unsigned int pulseCount;
    unsigned int lastPulseCount;

    unsigned int keyTimer;
    volatile unsigned char keyUpdatePush;
    volatile unsigned char keyUpdatePull;

    bool backlightTimerEnabled;
    unsigned int backlightTimer;

    unsigned int oneSecondTimer;
    volatile unsigned char oneSecondPush;
    volatile unsigned char oneSecondPull;
} events;

void initEvents()
{
    events.isInitialized = true;

    events.keyTimer = events.tick + KEY_TICKS;

    events.oneSecondTimer = TICK_FREQUENCY;

    setView(VIEW_WELCOME);

    triggerBacklight();
}

unsigned int getEventsTick()
{
    return events.tick;
}

bool isTimerElapsed(unsigned int tick)
{
    int deltaTime = events.tick - tick;

    return (deltaTime >= 0);
}

void triggerPulse()
{
    events.pulseCount++;

    switch (settings.pulseSound)
    {
    case PULSE_SOUND_QUIET:
        triggerBuzzer(PULSE_SOUND_QUIET_TICKS);
        break;

    case PULSE_SOUND_LOUD:
        triggerBuzzer(PULSE_SOUND_LOUD_TICKS);
        break;
    }
}

void triggerBacklight()
{
    if ((settings.backlight == BACKLIGHT_OFF) || (settings.backlight == BACKLIGHT_ON))
        events.backlightTimerEnabled = false;
    else
    {
        events.backlightTimerEnabled = true;
        events.backlightTimer = getEventsTick() + TICK_FREQUENCY * getBacklightTime(settings.backlight);
    }

    setBacklight(settings.backlight != BACKLIGHT_OFF);
}

void onEventsTick()
{
    if (!events.isInitialized)
        return;

    // Pulses
    unsigned int pulseCount = events.pulseCount;
    unsigned int newPulses = pulseCount - events.lastPulseCount;
    events.lastPulseCount = pulseCount;

    onMeasurementTick(newPulses);

    settings.lifeCounts += newPulses;

    // Keyboard
    if (isTimerElapsed(events.keyTimer))
    {
        events.keyTimer += KEY_TICKS;

        int key = getKeyboardKey();
        if (key >= 0)
        {
            events.keyUpdatePush = ((events.keyUpdatePush & ~0xf) + 0x10) | key;

            triggerBacklight();
        }
    }

    // Backlight
    if (events.backlightTimerEnabled && isTimerElapsed(events.backlightTimer))
    {
        events.backlightTimerEnabled = false;
        setBacklight(false);
    }

    // Measurements
    if (isTimerElapsed(events.oneSecondTimer))
    {
        events.oneSecondTimer += TICK_FREQUENCY;
        events.oneSecondPush++;

        if (isInstantaneousRateAlarm() || isDoseAlarm())
            triggerBuzzer(ALARM_TICKS);

        onMeasurementOneSecond();
    }

    events.tick++;
}

void updateEvents()
{
    unsigned char oneSecondPush = events.oneSecondPush;
    if (events.oneSecondPull != oneSecondPush)
    {
        events.oneSecondPull = oneSecondPush;

        updateMeasurements();

        updateBattery();

        updateGameTimer();

        if (!events.isWelcomed)
        {
            events.isWelcomed = true;

            setView(VIEW_INSTANTANEOUS_RATE);
        }

        updateView();

        addClamped(&settings.lifeTimer, 1);
    }
}

int getEventsKey()
{
    unsigned char keyUpdatePush = events.keyUpdatePush;
    if (events.keyUpdatePull != keyUpdatePush)
    {
        events.keyUpdatePull = keyUpdatePush;

        return (keyUpdatePush & 0xf);
    }

    return -1;
}
