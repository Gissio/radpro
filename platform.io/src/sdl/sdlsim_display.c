/*
 * Rad Pro
 * Simulator display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <SDL.h>

#include <mcu-renderer-sdl.h>

#include "../cstring.h"
#include "../display.h"
#include "../settings.h"
#include "../system.h"

// Controller

extern mr_t mr;

bool displayOn;
bool displayBacklightOn;
bool pulseLEDOn;
bool vibratorOn;

static uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

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
}

void setDisplayOn(bool value)
{
    displayOn = value;

    mr_sdl_set_display(&mr, displayOn);
}

bool isDisplayOn(void)
{
    return displayOn;
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
        if (event.type == SDL_QUIT)
        {
            writeDatalog();
            writeSettings();

            exit(0);
        }
    }
}

void updateDisplayTitle(void)
{
    char buffer[32];

    strcpy(buffer, FIRMWARE_NAME);
    if (pulseLEDOn || vibratorOn)
        strcat(buffer, " ");
    if (pulseLEDOn)
        strcat(buffer, "🔴");
    if (vibratorOn)
        strcat(buffer, "📳");

    mr_sdl_set_title(&mr, buffer);
}

// Backlight

void initDisplayBacklight(void)
{
}

void setDisplayBacklightOn(bool value)
{
    displayBacklightOn = value;

    mr_sdl_set_backlight(
        &mr,
        value
            ? displayBrightnessValues[settings.displayBrightness]
            : 0);
}

bool isDisplayBacklightOn(void)
{
    return displayBacklightOn;
}

// Pulse LED

void initPulseLEDController(void)
{
}

void setPulseLED(bool value)
{
    pulseLEDOn = value;

    updateDisplayTitle();
}

// Vibrator

void initVibratorController(void)
{
}

void setVibrator(bool value)
{
    vibratorOn = value;

    updateDisplayTitle();
}

#endif
