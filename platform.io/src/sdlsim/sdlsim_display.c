/*
 * Rad Pro
 * SDLSim display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <SDL.h>

#include <mcu-renderer-sdl.h>
#include <mcu-renderer-st7789.h>

#include "../display.h"
#include "../settings.h"
#include "../system.h"

extern mr_t mr;

static uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

void initDisplayHardware(void)
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

    writeDatalogEntry(true);
    writeSettings();
}

void setDisplayBacklight(bool value)
{
    mr_sdl_set_backlight(
        &mr,
        value
            ? displayBrightnessValues[settings.displayBrightness]
            : 0);
}

void syncDisplayBacklight(void)
{
}

void updateDisplay(void)
{
    mr_refresh_display(&mr);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            writeDatalogEntry(true);
            writeSettings();

            exit(0);
        }
    }
}

void updateDisplayContrast()
{
}

#endif
