/*
 * Rad Pro
 * Event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#ifndef SDL_MODE

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/iwdg.h>

#else

#include <SDL.h>

#endif

#include "battery.h"
#include "buzzer.h"
#include "cmath.h"
#include "display.h"
#include "events.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "settings.h"
#include "ui.h"

#define HOUR (60 * 60)

struct
{
    volatile uint32_t tick;

    volatile int32_t backlightTimer;
    volatile int32_t buzzerTimer;
    int32_t keyTimer;

    int32_t oneSecondTimer;
    volatile uint8_t oneSecondUpdate;
    uint8_t lastOneSecondUpdate;

    int32_t oneHourTimer;

    bool measurementsEnabled;
} events;

void initEvents(void)
{
#ifndef SDL_MODE
    // SysTick
    systick_set_frequency(SYS_TICK_FREQUENCY, rcc_ahb_frequency);
    systick_clear();

    systick_interrupt_enable();
    systick_counter_enable();

    nvic_set_priority(NVIC_SYSTICK_IRQ, 255);

    // Watchdog
    iwdg_set_period_ms(500 * 40000 / 32768);
    iwdg_start();
#endif

    events.keyTimer = KEY_TICKS;
    events.oneSecondTimer = SYS_TICK_FREQUENCY;
    events.oneHourTimer = HOUR;
}

void setMeasurements(bool value)
{
    events.measurementsEnabled = value;
}

static bool isTimerElapsed(volatile int32_t *timer)
{
    int32_t timerValue = *timer;

    if (timerValue <= 0)
        return false;

    timerValue--;
    *timer = timerValue;

    return (timerValue == 0);
}

void sys_tick_handler(void)
{
    events.tick++;

    // Backlight
    if (isTimerElapsed(&events.backlightTimer))
        setBacklight(false);
    else if (events.backlightTimer != 0)
        setBacklight(!getBacklight());

    // Buzzer
    if (isTimerElapsed(&events.buzzerTimer))
        setBuzzer(false);

    // Keyboard
    if (isTimerElapsed(&events.keyTimer))
    {
        events.keyTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Measurement
    uint32_t pulseNum = 0;
    uint32_t pulseTime;
    while (getGMPulse(&pulseTime))
    {
        onRNGPulse(pulseTime);

        pulseNum++;
    }

    if (events.measurementsEnabled)
    {
        onMeasurementTick(pulseNum);

        // One second timer
        if (isTimerElapsed(&events.oneSecondTimer))
        {
            if (events.measurementsEnabled)
                onMeasurementOneSecond();

            events.oneSecondTimer = SYS_TICK_FREQUENCY;
            events.oneSecondUpdate++;
        }
    }
}

void sleep(uint32_t value)
{
#ifndef SDL_MODE
    uint32_t tickStart = events.tick;

    iwdg_reset();
    while ((events.tick - tickStart) < value)
    {
        asm("wfi");
        iwdg_reset();
    }
#else
    if (!value)
        return;

    uint32_t tickStart = SDL_GetTicks();
    uint32_t tickUpdated = tickStart;

    while (true)
    {
        uint32_t tickCurrent = SDL_GetTicks();

        while ((tickCurrent - tickUpdated) > 0)
        {
            tickUpdated++;
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

void syncThreads(void)
{
    sleep(1);
}

void setBacklightTimer(int32_t value)
{
    if ((value > 0) && (value < events.backlightTimer))
        return;

    events.backlightTimer = value;
}

void triggerBacklight(void)
{
    syncThreads();

    switch (settings.backlight)
    {
    case BACKLIGHT_OFF:
    case BACKLIGHT_PULSE_FLASHES:
        events.backlightTimer = 1;
        break;

    case BACKLIGHT_10S:
        events.backlightTimer = 10 * SYS_TICK_FREQUENCY;
        break;

    case BACKLIGHT_60S:
        events.backlightTimer = 60 * SYS_TICK_FREQUENCY;
        break;

    case BACKLIGHT_ON:
        events.backlightTimer = -1;
        break;
    }
}

void stopBacklightTimer(void)
{
    syncThreads();

    events.backlightTimer = 1;
}

void setBuzzerTimer(int32_t value)
{
    if ((value > 0) && (value < events.buzzerTimer))
        return;

    events.buzzerTimer = value;

    setBuzzer(true);
}

void stopBuzzerTimer(void)
{
    syncThreads();

    events.buzzerTimer = 1;
}

void updateEvents(void)
{
    uint8_t oneSecondUpdate = events.oneSecondUpdate;
    if (events.lastOneSecondUpdate != oneSecondUpdate)
    {
        events.lastOneSecondUpdate = oneSecondUpdate;

        updateBattery();
        updateMeasurement();
        updateGameTimer();
        updateView();

        if (isTimerElapsed(&events.oneHourTimer))
        {
            events.oneHourTimer = HOUR;

            writeState();
        }
    }
}
