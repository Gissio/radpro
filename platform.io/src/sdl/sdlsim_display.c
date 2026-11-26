/*
 * Rad Pro
 * Simulator display
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include <mcu-renderer-sdl.h>

#include "../measurements/datalog.h"
#include "../peripherals/display.h"
#include "../peripherals/led.h"
#include "../peripherals/vibrator.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../system/system.h"
#include "sdlsim.h"

#if defined(DISPLAY_MONOCHROME)
#define SDL_DISPLAY_TYPE MR_SDL_DISPLAY_TYPE_MONOCHROME
#else
#define SDL_DISPLAY_TYPE MR_SDL_DISPLAY_TYPE_COLOR
#endif

#define DISPLAY_UPDATE_INTERVAL_MAX 100

#define TUBE_CPS_STEP 1.12201845F // 10^(1/20)
#define TUBE_CPS_MIN 0.01F
#define TUBE_CPS_MAX 100000.0F

extern mr_t mr;

static const uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

static struct
{
    uint32_t lastUpdateTick;
    bool enabled;

    // Indicators shown in the window title
    bool vibrator;
    bool pulseLED;
    bool alertLED;
} sdlDisplay;

static void updateDisplayTitle(void);

// ST7789 emulation: the firmware renders through the ST7789 driver,
// and the emulated controller writes the pixels to the SDL display

#if defined(ST7789_DEBUG)

#if !defined(DISPLAY_COLOR)
#error "ST7789_DEBUG requires DISPLAY_COLOR"
#endif

#include <mcu-renderer-st7789.h>

static mr_t mr_sdl;
static uint8_t textbuffer[88 * 88];

static struct
{
    bool command;
    uint8_t instruction;
    uint32_t index;

    uint16_t x0, x1, y0, y1;
    uint16_t x, y;
} st7789;

static void onST7789Sleep(uint32_t value)
{
}

static void onST7789Ignore(bool value)
{
}

static void onST7789Command(bool value)
{
    if (st7789.command != value)
        st7789.index = 0;

    st7789.command = value;
}

// CASET/RASET parameters: start high, start low, end high, end low
static void setST7789Bound(uint16_t *start, uint16_t *end, uint8_t value)
{
    if (st7789.index >= 4)
        return;

    uint16_t *bound = (st7789.index < 2) ? start : end;

    if (st7789.index & 1)
        *bound = (*bound & 0xff00) | value;
    else
        *bound = (*bound & 0x00ff) | (value << 8);
}

static void onST7789Send(uint16_t value)
{
    if (st7789.command)
    {
        st7789.instruction = value;

        if (st7789.instruction == MR_ST7789_RAMWR)
        {
            st7789.x = st7789.x0;
            st7789.y = st7789.y0;
        }

        return;
    }

    switch (st7789.instruction)
    {
    case MR_ST7789_CASET:
        setST7789Bound(&st7789.x0, &st7789.x1, value);

        break;

    case MR_ST7789_RASET:
        setST7789Bound(&st7789.y0, &st7789.y1, value);

        break;

    case MR_ST7789_RAMWR:
        if ((st7789.x < mr_sdl.display_width) &&
            (st7789.y < mr_sdl.display_height))
            ((mr_color_t *)mr_sdl.buffer)[st7789.y * mr_sdl.display_width + st7789.x] = value;

        if (++st7789.x > st7789.x1)
        {
            st7789.x = st7789.x0;
            st7789.y++;
        }

        break;
    }

    st7789.index++;
}

#else

#define mr_sdl mr

#endif

// Display

void initDisplay(void)
{
#if defined(ST7789_DEBUG)
    mr_st7789_init(&mr,
                   DISPLAY_HEIGHT,
                   DISPLAY_WIDTH,
                   MR_DISPLAY_ROTATION_270,
                   textbuffer,
                   sizeof(textbuffer),
                   onST7789Sleep,
                   onST7789Ignore,
                   onST7789Ignore,
                   onST7789Command,
                   onST7789Send,
                   onST7789Send);
#endif
    mr_sdl_init(&mr_sdl,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                SDL_DISPLAY_TYPE,
                DISPLAY_UPSCALE,
                FIRMWARE_NAME);

    updateDisplayTitle();
}

void setDisplayEnabled(bool value)
{
    mr_sdl_set_display(&mr_sdl, value);

    sdlDisplay.enabled = value;
}

bool isDisplayEnabled(void)
{
    return sdlDisplay.enabled;
}

void updateDisplayContrast(void)
{
}

void refreshDisplay(void)
{
}

static void updateDisplayTitle(void)
{
    bool indicators = sdlDisplay.vibrator ||
                      sdlDisplay.pulseLED ||
                      sdlDisplay.alertLED;

    char title[128];
    snprintf(title,
             sizeof(title),
             "%s (%.2f cps)%s%s%s%s",
             FIRMWARE_NAME,
             tubeCPS,
             indicators ? " " : "",
             sdlDisplay.vibrator ? "📳" : "",
             sdlDisplay.pulseLED ? "🟢" : "",
             sdlDisplay.alertLED ? "🟥" : "");

    mr_sdl_set_title(&mr_sdl, title);
}

// Left Ctrl decreases, right Ctrl increases the tube rate by 1 dB
static void onKeyDown(uint16_t modifiers)
{
    if (!(modifiers & KMOD_CTRL))
        return;

    tubeCPS *= (modifiers & KMOD_LCTRL) ? (1.0F / TUBE_CPS_STEP) : TUBE_CPS_STEP;
    tubeCPS = fminf(fmaxf(tubeCPS, TUBE_CPS_MIN), TUBE_CPS_MAX);

    updateDisplayTitle();
}

static void updateDisplay(void)
{
    mr_sdl_refresh_display(&mr_sdl);

    sdlDisplay.lastUpdateTick = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            stopDatalog();
            saveSettings();

            exit(0);
        }
        else if (event.type == SDL_KEYDOWN)
            onKeyDown(event.key.keysym.mod);
    }
}

bool onSDLTick(void)
{
    uint32_t sdlTicks = SDL_GetTicks();

    // Simulation ahead of real time: refresh and wait
    if ((int32_t)(sdlTicks - currentTick) < 0)
    {
        updateDisplay();

#if !defined(__EMSCRIPTEN__)
        // Avoid busy-waiting when vsync is not in effect
        SDL_Delay(1);
#endif

        return false;
    }

    // Simulation catching up: keep the window responsive
    if ((sdlTicks - sdlDisplay.lastUpdateTick) >= DISPLAY_UPDATE_INTERVAL_MAX)
        updateDisplay();

    currentTick++;

    onTick();

    return true;
}

// Display backlight

void setBacklight(bool value)
{
    mr_sdl_set_backlight(&mr_sdl,
                         value
                             ? displayBrightnessValues[settings.displayBrightness]
                             : 0);
}

// Indicators

static void setIndicator(bool *indicator, bool value)
{
    // Pulse LED toggles at the pulse rate: skip redundant title updates
    if (*indicator == value)
        return;

    *indicator = value;

    updateDisplayTitle();
}

// Vibrator

void initVibrator(void)
{
}

void setVibrator(bool value)
{
    setIndicator(&sdlDisplay.vibrator, value);
}

// LED

void initPulseLED(void)
{
}

void setPulseLED(bool value)
{
    setIndicator(&sdlDisplay.pulseLED, value);
}

void initAlertLED(void)
{
}

void setAlertLED(bool value)
{
    setIndicator(&sdlDisplay.alertLED, value);
}

void initPulseLEDEnable(void)
{
}

void setPulseLEDEnable(bool value)
{
    setPulseLED(value);
}

void initAlertLEDEnable(void)
{
}

void setAlertLEDEnable(bool value)
{
    setAlertLED(value);
}

#endif
