/*
 * FS2011 Pro
 * Event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#ifdef SDL_MODE
#include <SDL.h>
#endif

#include "buzzer.h"
#include "cmath.h"
#include "display.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "main.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
#include "ui.h"

struct Events
{
    volatile uint32_t sysTickValue;

    bool isEventsEnabled;

    uint32_t lastPulseCount;

    uint32_t keyTimer;

    volatile int backlightTimer;
    bool backlightPWMState;

    int buzzerTimer;

    int oneSecondTimer;
    volatile uint8_t oneSecondUpdate;
    volatile uint8_t lastOneSecondUpdate;
} events;

void initEvents(void)
{
#ifndef SDL_MODE
    // SysTick
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_clear();
    systick_set_frequency(SYS_TICK_FREQUENCY, rcc_ahb_frequency);

    systick_interrupt_enable();
    systick_counter_enable();

    // Watchdog
    // iwdg_set_period_ms(500);
    // iwdg_start();
#endif

    events.keyTimer = KEY_TICKS;
    events.oneSecondTimer = SYS_TICK_FREQUENCY;
}

void setEventsEnabled(bool value)
{
    events.isEventsEnabled = value;
}

static bool isTimerElapsed(int *timer)
{
    if ((*timer) <= 0)
        return false;

    (*timer)--;

    return (*timer) == 0;
}

void sys_tick_handler(void)
{
    events.sysTickValue++;

    // Backlight
    if (isTimerElapsed(&events.backlightTimer))
        setBacklight(false);
    else if (events.backlightTimer != 0)
    {
        events.backlightPWMState = !events.backlightPWMState;
        setBacklight(events.backlightPWMState);
    }

    // Events enabled
    if (!events.isEventsEnabled)
        return;

    // Keyboard
    if (isTimerElapsed(&events.keyTimer))
    {
        events.keyTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Buzzer
    if (isTimerElapsed(&events.buzzerTimer))
        setBuzzer(false);

    // Measurement
    uint32_t pulseCount = getPulseCount();
    uint32_t pulseCountDelta = pulseCount - events.lastPulseCount;
    events.lastPulseCount = pulseCount;

    onMeasurementTick(pulseCountDelta);

    // One second timer
    if (isTimerElapsed(&events.oneSecondTimer))
    {
        events.oneSecondTimer = SYS_TICK_FREQUENCY;

        onMeasurementOneSecond();

        events.oneSecondUpdate++;
    }
}

void waitSysTicks(uint32_t value)
{
#ifndef SDL_MODE
    uint32_t tickStart = events.sysTickValue;

    // iwdg_reset();
    while ((events.sysTickValue - tickStart) < value)
    {
        // asm("wfi")
        // iwdg_reset();
    }
#else
    uint32_t tickStart = SDL_GetTicks();
    uint32_t tickUpdate = tickStart;

    while (true)
    {
        uint32_t tickCurrent = SDL_GetTicks();

        while (tickUpdate < tickCurrent)
        {
            tickUpdate++;
            sys_tick_handler();
        }

        if ((tickCurrent - tickStart) >= value)
            break;

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                exit(0);
        }
    }
#endif
}

void updateBacklight(void)
{
    switch (settings.backlight)
    {
    case BACKLIGHT_OFF:
        events.backlightTimer = 1;
        break;

    case BACKLIGHT_10S:
    case BACKLIGHT_60S:
        events.backlightTimer = SYS_TICK_FREQUENCY * getBacklightTime(settings.backlight);
        break;

    case BACKLIGHT_ON:
        events.backlightTimer = -1;
        break;
    }
}

void disableBacklight(void)
{
    events.backlightTimer = 1;
}

void startBuzzerTimer(uint32_t value)
{
    if (events.buzzerTimer < value)
        events.buzzerTimer = value;

    setBuzzer(true);
}

void updateEvents(void)
{
    uint8_t oneSecondUpdate = events.oneSecondUpdate;
    if (events.lastOneSecondUpdate != oneSecondUpdate)
    {
        events.lastOneSecondUpdate = oneSecondUpdate;

        updateMeasurement();
        updateBattery();
        updateGameTimer();
        updateView();

        addClamped(&settings.lifeTimer, 1);
    }
}
