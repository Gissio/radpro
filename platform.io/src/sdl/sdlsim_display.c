/*
 * Rad Pro
 * Simulator display
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include <mcu-renderer-sdl.h>

#include "../cstring.h"
#include "../datalog.h"
#include "../display.h"
#include "../events.h"
#include "../settings.h"
#include "../system.h"

// Display

extern mr_t mr;

bool displayEnabled;
bool vibrationOn;
bool pulseLEDOn;
bool alertLEDOn;

static uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

extern float tubeCPS;

void updateDisplayTitle(void);

void initDisplayController(void)
{
    // mcu-renderer
#if defined(DISPLAY_MONOCHROME)
    mr_sdl_init(&mr,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                MR_SDL_DISPLAY_TYPE_MONOCHROME,
                DISPLAY_UPSCALE,
                FIRMWARE_NAME);
#elif defined(DISPLAY_COLOR)
    mr_sdl_init(&mr,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                MR_SDL_DISPLAY_TYPE_COLOR,
                DISPLAY_UPSCALE,
                FIRMWARE_NAME);
#endif
    updateDisplayTitle();
}

void enableDisplay(bool value)
{
    mr_sdl_set_display(&mr, value);

    displayEnabled = value;
}

bool isDisplayEnabled(void)
{
    return displayEnabled;
}

void updateDisplayContrast(void)
{
}

void refreshDisplay(void)
{
}

void updateDisplay(void)
{
    mr_sdl_refresh_display(&mr);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            closeDatalog();
            writeSettings();

            exit(0);

            break;

        case SDL_KEYDOWN:
        {
            float tubeCPSAdjustment = expf(logf(10) / 20);

            if (event.key.keysym.mod & KMOD_LCTRL)
                tubeCPS /= tubeCPSAdjustment;
            else if (event.key.keysym.mod & KMOD_RCTRL)
                tubeCPS *= tubeCPSAdjustment;

            if (tubeCPS < 0.01F)
                tubeCPS = 0.01F;
            else if (tubeCPS > 100000.0F)
                tubeCPS = 100000.0F;

            updateDisplayTitle();

            break;
        }
        }
    }
}

extern volatile uint32_t eventsCurrentTick;

bool updateSDLTicks()
{
    int32_t deltaTicks = SDL_GetTicks() - eventsCurrentTick;
    eventsCurrentTick++;

    onTick();

    if (deltaTicks < 0)
    {
        updateDisplay();

        return false;
    }

    return true;
}

void updateDisplayTitle(void)
{
    char buffer[256];

    sprintf(buffer, "%s (%.2f cps)", FIRMWARE_NAME, tubeCPS);

    if (vibrationOn || pulseLEDOn || alertLEDOn)
    {
        strcat(buffer, " ");

        if (vibrationOn)
            strcat(buffer, "📳");

        if (pulseLEDOn)
            strcat(buffer, "🔴");

        if (alertLEDOn)
            strcat(buffer, "⚠️");
    }

    mr_sdl_set_title(&mr, buffer);
}

// Display backlight

void initDisplayBacklight(void)
{
}

void setDisplayBacklight(bool value)
{
    mr_sdl_set_backlight(
        &mr,
        value
            ? displayBrightnessValues[settings.displayBrightness]
            : 0);
}

// Vibration

void initVibration(void)
{
}

void setVibration(bool value)
{
    vibrationOn = value;

    updateDisplayTitle();
}

// LED

void initLED(void)
{
}

void setPulseLED(bool value)
{
    pulseLEDOn = value;

    updateDisplayTitle();
}

void setAlertLED(bool value)
{
    alertLEDOn = value;

    updateDisplayTitle();
}

#endif
