/*
 * Rad Pro
 * SDLSim display
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <clib/u8g2.h>
#include <SDL.h>

#include "../display.h"
#include "../settings.h"

#if defined(DISPLAY_128X64)

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_UPSCALE 3

#elif defined(DISPLAY_320X240)

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_UPSCALE 2

#endif

extern u8g2_t displayU8g2;

static bool displayEnabled;

#if defined(DISPLAY_MONO)

static bool displayBacklightEnabled;
static bool displayBacklightActive;

#endif

static SDL_Window *displayWindow;
static SDL_Renderer *displayRenderer;
static SDL_Texture *displayTexture;

static uint8_t displayBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

static SDL_Color displayTextureBuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint32_t)];

#if defined(DISPLAY_MONO)

static SDL_Color displayColors[][2][2] = {
    {
        {
            {57, 61, 46, 255},
            {24, 27, 18, 255},
        },
        {
            {57, 61, 46, 255},
            {24, 27, 18, 255},
        },
    },
    {
        {
            {154, 171, 237, 255},
            {37, 35, 146, 255},
        },
        {
            {151, 168, 234, 255},
            {37, 35, 146, 255},
        },
    },
};

#endif

static uint8_t onDisplay(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayGPIO(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void initDisplayHardware(void)
{
    // U8g2
    uint8_t u8g2TileHeight;
    uint8_t *u8g2Buffer;

    u8g2_SetupDisplay(&displayU8g2,
                      onDisplay,
                      u8x8_dummy_cb,
                      u8x8_dummy_cb,
                      &onDisplayGPIO);

#if defined(DISPLAY_128X64)

    u8g2Buffer = u8g2_m_16_8_f(&u8g2TileHeight);

#elif defined(DISPLAY_320X240)

    u8g2Buffer = u8g2_m_40_30_f(&u8g2TileHeight);

#endif

    u8g2_SetupBuffer(&displayU8g2,
                     u8g2Buffer,
                     u8g2TileHeight,
                     u8g2_ll_hvline_vertical_top_lsb,
                     U8G2_R0);

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
        printf("Could not create SDL window: %s\n", SDL_GetError());

        exit(1);
    }

    displayRenderer = SDL_CreateRenderer(displayWindow,
                                         -1,
                                         SDL_RENDERER_PRESENTVSYNC);
    if (!displayRenderer)
    {
        printf("Could not create SDL renderer: %s\n", SDL_GetError());

        exit(1);
    }

    SDL_RenderSetLogicalSize(displayRenderer,
                             DISPLAY_WIDTH * DISPLAY_UPSCALE,
                             DISPLAY_HEIGHT * DISPLAY_UPSCALE);

    displayTexture = SDL_CreateTexture(displayRenderer,
                                       SDL_PIXELFORMAT_ABGR8888,
                                       SDL_TEXTUREACCESS_STREAMING,
                                       DISPLAY_WIDTH,
                                       DISPLAY_HEIGHT);
    if (!displayTexture)
    {
        printf("Could not create SDL texture: %s\n", SDL_GetError());

        exit(1);
    }

    atexit(SDL_Quit);
}

#if defined(PULSE_LED)

void setPulseLED(void)
{
}

#endif

void setDisplay(bool value)
{
    displayEnabled = value;
}

#if defined(DISPLAY_MONO)

void setBacklight(bool value)
{
    displayBacklightEnabled = value;

    if (value)
        displayBacklightActive = true;
}

bool getBacklight(void)
{
    return displayBacklightEnabled;
}

#endif

static const u8x8_display_info_t u8x8_sdl_info = {
    0,                  // chip_enable_level
    1,                  // chip_disable_level
    0,                  // post_chip_enable_wait_ns
    0,                  // pre_chip_disable_wait_ns
    1,                  // reset_pulse_width_ms
    1,                  // post_reset_wait_ms
    0,                  // sda_setup_time_ns
    0,                  // sck_pulse_width_ns
    0,                  // sck_clock_hz
    0,                  // spi_mode: active high, rising edge
    0,                  // i2c_bus_clock_100kHz
    0,                  // data_setup_time_ns
    0,                  // write_pulse_width_ns
    DISPLAY_WIDTH / 8,  // tile_width: number of 8-pixel tiles in x
    DISPLAY_HEIGHT / 8, // tile_height: number of 8-pixel tiles in y
    0,                  // default_x_offset
    0,                  // flipmode_x_offset
    DISPLAY_WIDTH,      // pixel_width
    DISPLAY_HEIGHT,     // pixel_height
};

static uint8_t onDisplayGPIO(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_MILLI:
        break;

    default:
        return 0;
    }

    return 1;
}

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

        uint32_t xStart = ((u8x8_tile_t *)arg_ptr)->x_pos * 8 + u8g2->x_offset;
        uint32_t yStart = ((u8x8_tile_t *)arg_ptr)->y_pos * 8;
        uint32_t xEnd = xStart + ((u8x8_tile_t *)arg_ptr)->cnt * 8;
        uint32_t yEnd = yStart + 8;

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

#if defined(DISPLAY_COLOR)

enum DisplayColors
{
    COLOR_OFF,
    COLOR_STATUSBAR,
    COLOR_TITLEBAR,
    COLOR_DEFAULT,
    COLOR_MENU_SELECTED,
    COLOR_ALERT,
};

static const SDL_Color displayColors[][2][2] = {
    {{{0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00}},
     {{0x00, 0x00, 0x00},
      {0x00, 0x00, 0x00}}},

    {{{0x00, 0x00, 0x00},
      {0xff, 0xff, 0xff}},
     {{0xff, 0xff, 0xff},
      {0x00, 0x00, 0x00}}},

    {{{0x20, 0x20, 0x20},
      {0xff, 0xff, 0xff}},
     {{0xf0, 0xf0, 0xf0},
      {0x00, 0x00, 0x00}}},

    {{{0x20, 0x20, 0x20},
      {0xff, 0xff, 0xff}},
     {{0xf0, 0xf0, 0xf0},
      {0x00, 0x00, 0x00}}},

    {{{0x14, 0x8c, 0xe8},
      {0xff, 0xff, 0xff}},
     {{0x14, 0x8c, 0xe8},
      {0xff, 0xff, 0xff}}},

    // https://colorswall.com/palette/37559
    // https://www.google.com/search?q=tesla+software
    // https://controllerstech.com/interface-tft-display-with-stm32/

    {{{0x20, 0x20, 0x20},
      {0xf5, 0x24, 0x19}},
     {{0xf0, 0xf0, 0xf0},
      {0xf5, 0x24, 0x19}}},
};

const SDL_Color *getDisplayColor(uint32_t index)
{
    return displayColors[index][settings.displayTheme];
}

#endif

void updateDisplay(void)
{
#if defined(DISPLAY_MONO)

    for (uint32_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < DISPLAY_WIDTH; x++)
        {
            bool pixel = displayEnabled & displayBuffer[DISPLAY_WIDTH * y + x];
            bool backgroundHighlight = ((x >> 3) ^ (y >> 3)) & 1;

            SDL_Color color = displayColors[displayBacklightActive]
                                           [backgroundHighlight]
                                           [pixel];
            displayTextureBuffer[DISPLAY_WIDTH * y + x] = color;
        }
    }

#elif defined(DISPLAY_COLOR)

    SDL_Color palette[2] = {{0x00, 0x00, 0x00},
                            {0xff, 0xff, 0xff}};

    for (uint32_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < DISPLAY_WIDTH; x++)
        {
            SDL_Color color = palette[displayBuffer[DISPLAY_WIDTH * y + x]];
            displayTextureBuffer[DISPLAY_WIDTH * y + x] = color;
        }
    }

#endif

    SDL_UpdateTexture(displayTexture,
                      NULL,
                      displayTextureBuffer,
                      DISPLAY_WIDTH * sizeof(SDL_Color));

    SDL_RenderCopy(displayRenderer,
                   displayTexture,
                   NULL,
                   NULL);
    SDL_RenderPresent(displayRenderer);

#if defined(DISPLAY_MONO)

    displayBacklightActive = false;

#endif
}

#endif
