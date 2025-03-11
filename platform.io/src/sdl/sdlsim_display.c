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
#include <mcu-renderer-st7789.h>

#include "../cstring.h"
#include "../datalog.h"
#include "../display.h"
#include "../events.h"
#include "../settings.h"
#include "../system.h"

static void updateDisplayTitle(void);

// Display

extern mr_t mr;

bool displayEnabled;
bool vibrationOn;
bool pulseLEDOn;
bool alertLEDOn;

static uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

#if defined(ST7789_DEBUG)

mr_t mr_sdl;
char textbuffer[88 * 88];
uint32_t st7789Index;
bool st7789Command;
uint8_t st7789Instruction;

uint32_t st7789x0;
uint32_t st7789x1;
uint32_t st7789y0;
uint32_t st7789y1;

uint32_t st7789x;
uint32_t st7789y;

void onSleep(uint32_t value)
{
}

static void onSetReset(bool value)
{
}

static void onChipSelect(bool value)
{
}

static void onCommand(bool value)
{
    if (st7789Command != value)
        st7789Index = 0;

    st7789Command = value;
}

static void onSend(uint16_t value)
{
    if (st7789Command)
    {
        if (st7789Index == 0)
            st7789Instruction = value;

        switch (st7789Instruction)
        {
        case MR_ST7789_RAMWR:
            st7789x = st7789x0;
            st7789y = st7789y0;

            break;
        }
    }
    else
    {
        switch (st7789Instruction)
        {
        case MR_ST7789_CASET:
            if (st7789Index == 0)
                st7789x0 = (st7789x0 & ~0xff00) | (value << 8);
            else if (st7789Index == 1)
                st7789x0 = (st7789x0 & ~0x00ff) | (value << 0);
            else if (st7789Index == 2)
                st7789x1 = (st7789x1 & ~0xff00) | (value << 8);
            else if (st7789Index == 3)
                st7789x1 = (st7789x1 & ~0x00ff) | (value << 0);

            break;

        case MR_ST7789_RASET:
            if (st7789Index == 0)
                st7789y0 = (st7789y0 & ~0xff00) | (value << 8);
            else if (st7789Index == 1)
                st7789y0 = (st7789y0 & ~0x00ff) | (value << 0);
            else if (st7789Index == 2)
                st7789y1 = (st7789y1 & ~0xff00) | (value << 8);
            else if (st7789Index == 3)
                st7789y1 = (st7789y1 & ~0x00ff) | (value << 0);

            break;

        case MR_ST7789_RAMWR:
        {
            mr_color_t *framebuffer = (mr_color_t *)mr_sdl.buffer;
            framebuffer[st7789y * mr_sdl.display_width + st7789x] = value;
            st7789x += 1;
            if (st7789x > st7789x1)
            {
                st7789x = st7789x0;
                st7789y += 1;
            }

            break;
        }
        }

        st7789Index++;
    }
}

#else

#define mr_sdl mr

#endif

extern float tubeCPS;

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
    mr_sdl = mr;
#elif defined(DISPLAY_COLOR)
#if defined(ST7789_DEBUG)
    mr_st7789_init(&mr,
                   DISPLAY_HEIGHT,
                   DISPLAY_WIDTH,
                   MR_DISPLAY_ROTATION_270,
                   &textbuffer,
                   sizeof(textbuffer),
                   onSleep,
                   onSetReset,
                   onChipSelect,
                   onCommand,
                   onSend,
                   onSend);
#endif
    mr_sdl_init(&mr_sdl,
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
    mr_sdl_set_display(&mr_sdl, value);

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
    mr_sdl_refresh_display(&mr_sdl);

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

static void updateDisplayTitle(void)
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

    mr_sdl_set_title(&mr_sdl, buffer);
}

// Display backlight

void initDisplayBacklight(void)
{
}

void setDisplayBacklight(bool value)
{
    mr_sdl_set_backlight(
        &mr_sdl,
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
