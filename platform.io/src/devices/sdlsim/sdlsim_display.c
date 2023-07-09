/*
 * Rad Pro
 * SDLSim display interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include <clib/u8g2.h>
#include <SDL.h>

#include "../../display.h"

#define DISPLAY_UPSCALE 3

extern u8g2_t displayU8g2;

static bool displayOn;
static bool displayBacklightState;
static bool displayBacklightOn;

static SDL_Window *displayWindow;
static SDL_Renderer *displayRenderer;
static SDL_Texture *displayTexture;

static uint8_t displayBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

static SDL_Color displayTextureBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t)];
static SDL_Color displayPalette[] = {
    {57, 61, 46, 255},
    {24, 27, 18, 255},
    {57, 61, 46, 255},
    {24, 27, 18, 255},

    {154, 171, 237, 255},
    {37, 35, 146, 255},
    {151, 168, 234, 255},
    {37, 35, 146, 255},
};

static uint8_t onDisplay(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void initDisplay(void)
{
    // U8g2
    uint8_t u8g2TileBufHeight;
    uint8_t *u8g2Buf;

    u8g2_SetupDisplay(&displayU8g2, onDisplay, u8x8_dummy_cb, u8x8_dummy_cb, u8x8_dummy_cb);
    u8g2Buf = u8g2_m_16_8_f(&u8g2TileBufHeight);
    u8g2_SetupBuffer(&displayU8g2, u8g2Buf, u8g2TileBufHeight, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

    u8g2_InitDisplay(&displayU8g2);

    // SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Unable to initialize SDL:  %s\n", SDL_GetError());

        exit(1);
    }

    displayWindow = SDL_CreateWindow("U8g2",
                                     SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     DISPLAY_WIDTH * DISPLAY_UPSCALE,
                                     DISPLAY_HEIGHT * DISPLAY_UPSCALE,
                                     0);
    if (!displayWindow)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());

        exit(1);
    }

    displayRenderer = SDL_CreateRenderer(displayWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!displayRenderer)
    {
        printf("Couldn't create renderer: %s\n", SDL_GetError());

        exit(1);
    }

    SDL_RenderSetLogicalSize(displayRenderer,
                             DISPLAY_UPSCALE * DISPLAY_WIDTH,
                             DISPLAY_UPSCALE * DISPLAY_HEIGHT);

    displayTexture = SDL_CreateTexture(displayRenderer,
                                       SDL_PIXELFORMAT_ABGR8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       DISPLAY_WIDTH,
                                       DISPLAY_HEIGHT);
    if (!displayTexture)
    {
        printf("Couldn't create texture: %s\n", SDL_GetError());

        exit(1);
    }

    atexit(SDL_Quit);

    setDisplay(true);
}

void setDisplay(bool value)
{
    printf("Set display: %d\n", value);

    u8g2_SetPowerSave(&displayU8g2, !value);

    displayOn = value;
}

void setBacklight(bool value)
{
    displayBacklightState = value;

    if (value)
        displayBacklightOn = true;
}

bool getBacklight(void)
{
    return displayBacklightState;
}

static const u8x8_display_info_t u8x8_sdl_info = {
    /* chip_enable_level = */ 0,
    /* chip_disable_level = */ 1,

    /* post_chip_enable_wait_ns = */ 0,
    /* pre_chip_disable_wait_ns = */ 0,
    /* reset_pulse_width_ms = */ 0,
    /* post_reset_wait_ms = */ 0,
    /* sda_setup_time_ns = */ 0,
    /* sck_pulse_width_ns = */ 0,
    /* sck_clock_hz = */ 4000000UL, /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
    /* spi_mode = */ 1,
    /* i2c_bus_clock_100kHz = */ 0,
    /* data_setup_time_ns = */ 0,
    /* write_pulse_width_ns = */ 0,
    /* tile_width = */ 16,
    /* tile_hight = */ 8,
    /* default_x_offset = */ 0,
    /* flipmode_x_offset = */ 0,
    /* pixel_width = */ DISPLAY_WIDTH,
    /* pixel_height = */ DISPLAY_HEIGHT,
};

static uint8_t onDisplay(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
        u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_info);

        break;

    case U8X8_MSG_DISPLAY_INIT:
        u8x8_d_helper_display_init(u8g2);

        break;

    case U8X8_MSG_DISPLAY_DRAW_TILE:
    {
        uint8_t *data = ((u8x8_tile_t *)arg_ptr)->tile_ptr;

        uint32_t width = ((u8x8_tile_t *)arg_ptr)->cnt * 8;
        uint32_t height = 8;

        uint32_t xStart = ((u8x8_tile_t *)arg_ptr)->x_pos * 8 + u8g2->x_offset;
        uint32_t yStart = ((u8x8_tile_t *)arg_ptr)->y_pos * 8;
        uint32_t xEnd = xStart + width;
        uint32_t yEnd = yStart + height;

        for (uint32_t x = xStart; x < xEnd; x++)
        {
            uint8_t value = *data++;

            for (uint32_t y = yStart; y < yEnd; y++)
            {
                displayBuffer[DISPLAY_WIDTH * y + x] = value & 1;

                value >>= 1;
            }
        }

        break;
    }

    default:
        return 0;
    }

    return 1;
}

void updateDisplay(void)
{
    for (uint32_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < DISPLAY_WIDTH; x++)
        {
            bool pixel = displayOn & displayBuffer[DISPLAY_WIDTH * y + x];
            bool backgroundHighlight = ((x >> 3) ^ (y >> 3)) & 1;

            SDL_Color color = displayPalette[pixel |
                                             (backgroundHighlight << 1) |
                                             (displayBacklightOn << 2)];
            displayTextureBuffer[DISPLAY_WIDTH * y + x] = color;
        }
    }

    SDL_UpdateTexture(displayTexture,
                      NULL,
                      displayTextureBuffer,
                      DISPLAY_WIDTH * sizeof(SDL_Color));

    SDL_RenderCopy(displayRenderer, displayTexture, NULL, NULL);
    SDL_RenderPresent(displayRenderer);

    displayBacklightOn = false;
}

#endif
