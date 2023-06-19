/*
 * Rad Pro
 * LCD interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#ifndef SDL_MODE

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#else

#include <SDL.h>

#endif

#include <clib/u8g2.h>

#include "resources/font_helvR08.h"
#include "resources/font_helvR24.h"
#include "resources/font_icons.h"
#include "resources/font_tiny5.h"

#include "battery.h"
#include "confidence.h"
#include "display.h"
#include "events.h"
#include "format.h"
#include "fs2011.h"
#include "measurements.h"
#include "settings.h"

#include "resources/dotted4.xbm"

#define LCD_WIDTH 128
#define LCD_HEIGHT 64

#define LCD_CENTER_X (LCD_WIDTH / 2)
#define LCD_CENTER_Y (LCD_HEIGHT / 2)

#define TITLE_Y (LCD_CENTER_Y - 19)
#define SUBTITLE_Y (LCD_CENTER_Y + 19 + 5)

#define MEASUREMENT_VALUE_X (LCD_CENTER_X - 54)
#define MEASUREMENT_VALUE_Y (LCD_CENTER_Y + 24 / 2)
#define MEASUREMENT_VALUE_SIDE_X (LCD_CENTER_X + 29)

#define HISTORY_VIEW_X ((LCD_WIDTH - HISTORY_VIEW_WIDTH) / 2)
#define HISTORY_VIEW_Y_TOP 14
#define HISTORY_VIEW_Y_BOTTOM (HISTORY_VIEW_Y_TOP + HISTORY_VIEW_HEIGHT)

#define STATS_VIEW_Y 30

#define GAME_VIEW_BOARD_X 0
#define GAME_VIEW_BOARD_Y 8
#define GAME_VIEW_BOARD_WIDTH 9 * 8
#define GAME_VIEW_TIME_X (GAME_VIEW_BOARD_WIDTH + 7)
#define GAME_VIEW_TIME_UPPER_Y 8
#define GAME_VIEW_TIME_LOWER_Y (LCD_HEIGHT - 3)
#define GAME_VIEW_MOVES_LINE_WIDTH 25
#define GAME_VIEW_MOVES_LINE_HEIGHT 6
#define GAME_VIEW_MOVES_X (GAME_VIEW_BOARD_WIDTH + 6)
#define GAME_VIEW_MOVES_Y (LCD_CENTER_Y + 5 - GAME_VIEW_MOVES_LINE_HEIGHT * GAME_MOVES_LINE_NUM / 2)
#define GAME_VIEW_BUTTON_X 101
#define GAME_VIEW_BUTTON_Y 54
#define GAME_VIEW_BUTTON_WIDTH 23
#define GAME_VIEW_BUTTON_HEIGHT 9

#define MENU_VIEW_Y_TOP 14
#define MENU_VIEW_LINE_HEIGHT 12
#define MENU_VIEW_LINE_TEXT_X 6

struct
{
    u8g2_t u8g2;
    bool backlightOn;
} display;

const char *const firmwareName = "Rad Pro";
const char *const firmwareVersion = "1.2.1";

#ifndef SDL_MODE
static uint8_t setupU8X8(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayByte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayMessage(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
#else
void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb);
#endif

void initDisplay(void)
{
#ifndef SDL_MODE
    // LCD data & control
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOF);

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_RESET_PIN | LCD_EN_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_BACKLIGHT_PIN);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_RESET_PIN | LCD_EN_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_BACKLIGHT_PIN);

    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D5_PIN | LCD_D6_PIN);
    gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_D5_PIN | LCD_D6_PIN);

    // U8g2
    uint8_t tile_buf_height;
    uint8_t *buf;
    u8g2_SetupDisplay(&display.u8g2, setupU8X8, u8x8_cad_001, onDisplayByte, onDisplayMessage);
    buf = u8g2_m_16_8_f(&tile_buf_height);
    u8g2_SetupBuffer(&display.u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

    // u8g2_Setup_st7567_enh_dg128064_f(&display.u8g2, U8G2_R0, onDisplayByte, onDisplayMessage);
#else
    u8g2_SetupBuffer_SDL_128x64(&display.u8g2, U8G2_R0);
#endif

    u8g2_InitDisplay(&display.u8g2);

    setDisplay(true);
}

#ifndef SDL_MODE
static const u8x8_display_info_t dg128064_display_info =
    {
        /* chip_enable_level = */ 0,
        /* chip_disable_level = */ 1,

        /* post_chip_enable_wait_ns = */ 150, /* */
        /* pre_chip_disable_wait_ns = */ 50,  /* */
        /* reset_pulse_width_ms = */ 1,
        /* post_reset_wait_ms = */ 1,
        /* sda_setup_time_ns = */ 50,   /* */
        /* sck_pulse_width_ns = */ 120, /* */
        /* sck_clock_hz = */ 4000000UL, /* */
        /* spi_mode = */ 0,             /* active high, rising edge */
        /* i2c_bus_clock_100kHz = */ 4,
        /* data_setup_time_ns = */ 40,   /* */
        /* write_pulse_width_ns = */ 80, /* */
        /* tile_width = */ 16,           /* width of 16*8=128 pixel */
        /* tile_hight = */ 8,
        /* default_x_offset = */ 0,
        /* flipmode_x_offset = */ 4,
        /* pixel_width = */ 128,
        /* pixel_height = */ 64,
};

static const uint8_t dg128064_init_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */

    U8X8_C(0x0e2), /* soft reset */
    U8X8_C(0x0ae), /* display off */
    U8X8_C(0x040), /* set display start line to 0 */

    U8X8_C(0x0a0), /* ADC set to reverse */
    U8X8_C(0x0c8), /* common output mode */

    U8X8_C(0x0a6), /* display normal, bit val 0: LCD pixel off. */
    U8X8_C(0x0a2), /* LCD bias 1/9 */

    /* power on sequence from paxinstruments */
    U8X8_C(0x028 | 4), /* all power control circuits on */
    U8X8_DLY(50),
    U8X8_C(0x028 | 6), /* all power control circuits on */
    U8X8_DLY(50),
    U8X8_C(0x028 | 7), /* all power control circuits on */
    U8X8_DLY(50),

    U8X8_C(0x023),      /* v0 voltage resistor ratio */
    U8X8_CA(0x081, 36), /* set contrast, contrast value*/

    U8X8_C(0x0ae), /* display off */
    U8X8_C(0x0a5), /* enter powersafe: all pixel on, issue 142 */

    U8X8_END_TRANSFER(), /* disable chip */
    U8X8_END()           /* end of sequence */
};

static const uint8_t st7567_132x64_powersave0_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0a4),         /* all pixel off, issue 142 */
    U8X8_C(0x0af),         /* display on */
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
};

static const uint8_t st7567_132x64_powersave1_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0ae),         /* display off */
    U8X8_C(0x0a5),         /* enter powersafe: all pixel on, issue 142 */
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
};

static uint8_t onDisplayMessage(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_MILLI:
        sleep(arg_int);
        break;

    case U8X8_MSG_GPIO_RESET:
        if (arg_int)
            gpio_set(LCD_RESET_PORT, LCD_RESET_PIN);
        else
            gpio_clear(LCD_RESET_PORT, LCD_RESET_PIN);
        break;

    default:
        return 0;
    }

    return 1;
}

static uint8_t onDisplayByte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_BYTE_SET_DC:
        if (arg_int)
            gpio_set(LCD_RS_PORT, LCD_RS_PIN);
        else
            gpio_clear(LCD_RS_PORT, LCD_RS_PIN);
        break;

    case U8X8_MSG_BYTE_SEND:
    {
        uint8_t *p = (uint8_t *)arg_ptr;
        for (uint32_t i = 0; i < arg_int; i++, p++)
        {
            uint8_t value = *p;

            GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
            GPIOF_BSRR = (0b01100000 << 17) | ((value & 0b01100000) << 1);

            gpio_set(LCD_EN_PORT, LCD_EN_PIN);
            asm("nop");
            asm("nop");
            gpio_clear(LCD_EN_PORT, LCD_EN_PIN);
        }

        break;
    }

    default:
        return 0;
    }

    return 1;
}

static uint8_t setupU8X8(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t x, c;
    uint8_t *ptr;

    switch (msg)
    {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
        u8x8_d_helper_display_setup_memory(u8x8, &dg128064_display_info);
        break;

    case U8X8_MSG_DISPLAY_INIT:
        u8x8_d_helper_display_init(u8x8);
        u8x8_cad_SendSequence(u8x8, dg128064_init_seq);
        break;

    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
        if (arg_int == 0)
            u8x8_cad_SendSequence(u8x8, st7567_132x64_powersave0_seq);
        else
            u8x8_cad_SendSequence(u8x8, st7567_132x64_powersave1_seq);
        break;

    case U8X8_MSG_DISPLAY_DRAW_TILE:
        u8x8_cad_StartTransfer(u8x8);

        x = ((u8x8_tile_t *)arg_ptr)->x_pos;
        x *= 8;
        x += u8x8->x_offset;
        u8x8_cad_SendCmd(u8x8, 0x010 | (x >> 4));
        u8x8_cad_SendCmd(u8x8, 0x000 | ((x & 15)));
        u8x8_cad_SendCmd(u8x8, 0x0b0 | (((u8x8_tile_t *)arg_ptr)->y_pos));

        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        c *= 8;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        /* The following if condition checks the hardware limits of the st7567
         * controller: It is not allowed to write beyond the display limits.
         * This is in fact an issue within flip mode.
         */
        if (c + x > 132u)
        {
            c = 132u;
            c -= x;
        }
        do
        {
            /* note: SendData can not handle more than 255 bytes */
            u8x8_cad_SendData(u8x8, c, ptr);
            arg_int--;
        } while (arg_int > 0);

        u8x8_cad_EndTransfer(u8x8);

        break;

    default:
        return 0;
    }

    return 1;
}

#else

#define W(x, w) (((x) * (w)) / 100)

SDL_Window *u8g_sdl_window;
SDL_Surface *u8g_sdl_screen;

uint32_t u8g_sdl_multiple = 3;
uint32_t u8g_sdl_color[256];
uint32_t u8g_sdl_height, u8g_sdl_width;

static void u8g_sdl_set_pixel(uint32_t x, uint32_t y, uint32_t index)
{
    if (y >= u8g_sdl_height)
        return;
    if (y < 0)
        return;
    if (x >= u8g_sdl_width)
        return;
    if (x < 0)
        return;

    for (uint32_t i = 0; i < u8g_sdl_multiple; i++)
        for (uint32_t j = 0; j < u8g_sdl_multiple; j++)
        {
#ifndef NO_SDL
            uint32_t offset = (((y * u8g_sdl_multiple) + i) * (u8g_sdl_width * u8g_sdl_multiple) +
                               ((x * u8g_sdl_multiple) + j)) *
                              u8g_sdl_screen->format->BytesPerPixel;

            uint32_t *ptr = (uint32_t *)((uint8_t *)u8g_sdl_screen->pixels + offset);
            *ptr = u8g_sdl_color[index];
#endif
        }
}

static void u8g_sdl_set_8pixel(uint32_t x, uint32_t y, uint8_t pixel)
{
    uint32_t count = 8;
    uint32_t background = 0;

    if ((x / 8 + y / 8) & 1)
        background = 4;

    while (count > 0)
    {
        if ((pixel & 1) == 0)
            u8g_sdl_set_pixel(x, y, background);
        else
            u8g_sdl_set_pixel(x, y, 3);

        pixel >>= 1;
        y++;
        count--;
    }
}

static void u8g_sdl_set_multiple_8pixel(uint32_t x, uint32_t y, uint32_t count, uint8_t *pixel)
{
    uint8_t b;

    while (count > 0)
    {
        b = *pixel;
        u8g_sdl_set_8pixel(x, y, b);
        x++;
        pixel++;
        count--;
    }
}

static void u8g_sdl_init(uint32_t width, uint32_t height)
{
    u8g_sdl_height = height;
    u8g_sdl_width = width;

#ifndef NO_SDL

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Unable to initialize SDL:  %s\n", SDL_GetError());
        exit(1);
    }

    u8g_sdl_window = SDL_CreateWindow("U8g2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, u8g_sdl_width * u8g_sdl_multiple, u8g_sdl_height * u8g_sdl_multiple, 0);

    if (u8g_sdl_window == NULL)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        exit(1);
    }

    u8g_sdl_screen = SDL_GetWindowSurface(u8g_sdl_window);

    if (u8g_sdl_screen == NULL)
    {
        printf("Couldn't create screen: %s\n", SDL_GetError());
        exit(1);
    }

    printf("%d bits-per-pixel mode\n", u8g_sdl_screen->format->BitsPerPixel);
    printf("%d bytes-per-pixel mode\n", u8g_sdl_screen->format->BytesPerPixel);

    u8g_sdl_color[0] = SDL_MapRGB(u8g_sdl_screen->format, 144, 161, 227);
    u8g_sdl_color[1] = SDL_MapRGB(u8g_sdl_screen->format, 0, 0, 255);
    u8g_sdl_color[2] = SDL_MapRGB(u8g_sdl_screen->format, 255, 0, 0);
    u8g_sdl_color[3] = SDL_MapRGB(u8g_sdl_screen->format, 37, 35, 146);
    u8g_sdl_color[4] = SDL_MapRGB(u8g_sdl_screen->format, 154, 171, 237);

    /*
    u8g_sdl_set_pixel(0,0);
    u8g_sdl_set_pixel(1,1);
    u8g_sdl_set_pixel(2,2);
    */

    /* update all */
    SDL_UpdateWindowSurface(u8g_sdl_window);

    atexit(SDL_Quit);
#endif
    return;
}

static const u8x8_display_info_t u8x8_sdl_128x64_info =
    {
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
        /* pixel_width = */ 128,
        /* pixel_height = */ 64};

static const u8x8_display_info_t u8x8_sdl_240x160_info =
    {
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
        /* tile_width = */ 30, /* width of 30*8=240 pixel */
        /* tile_hight = */ 20, /* height: 160 pixel */
        /* default_x_offset = */ 0,
        /* flipmode_x_offset = */ 0,
        /* pixel_width = */ 240,
        /* pixel_height = */ 160};

uint8_t u8x8_d_sdl_128x64(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t x, y, c;
    uint8_t *ptr;
    switch (msg)
    {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
        u8x8_d_helper_display_setup_memory(u8g2, &u8x8_sdl_128x64_info);
        u8g_sdl_init(128, 64);
        break;
    case U8X8_MSG_DISPLAY_INIT:
        u8x8_d_helper_display_init(u8g2);
        break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
        break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
        break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
        break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
        x = ((u8x8_tile_t *)arg_ptr)->x_pos;
        x *= 8;
        x += u8g2->x_offset;

        y = ((u8x8_tile_t *)arg_ptr)->y_pos;
        y *= 8;

        do
        {
            c = ((u8x8_tile_t *)arg_ptr)->cnt;
            ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
            u8g_sdl_set_multiple_8pixel(x, y, c * 8, ptr);
            arg_int--;
            x += c * 8;
        } while (arg_int > 0);

        /* update all */
#ifndef NO_SDL
        SDL_UpdateWindowSurface(u8g_sdl_window);
#endif
        break;
    default:
        return 0;
    }
    return 1;
}

void u8x8_Setup_SDL_128x64(u8x8_t *u8x8)
{
    /* setup defaults */
    u8x8_SetupDefaults(u8x8);

    /* setup specific callbacks */
    u8x8->display_cb = u8x8_d_sdl_128x64;

    /* setup display info */
    u8x8_SetupMemory(u8x8);
}

void u8g2_SetupBuffer_SDL_128x64(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
    static uint8_t buf[128 * 8];
    u8x8_Setup_SDL_128x64(u8g2_GetU8x8(u8g2));
    u8g2_SetupBuffer(u8g2, buf, 8, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}

#endif

void setDisplay(bool value)
{
#ifdef SDL_MODE
    printf("Set display: %d\n", value);
#endif

    u8g2_SetPowerSave(&display.u8g2, !value);
}

void clearDisplayBuffer(void)
{
    u8g2_ClearBuffer(&display.u8g2);
}

void sendDisplayBuffer(void)
{
    u8g2_SendBuffer(&display.u8g2);
}

void setBacklight(bool value)
{
#ifndef SDL_MODE
    display.backlightOn = value;
    if (value)
        gpio_set(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
    else
        gpio_clear(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
#endif
}

bool getBacklight(void)
{
    return display.backlightOn;
}

static void drawTextLeft(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&display.u8g2, x, y, str);
}

static void drawTextCenter(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&display.u8g2, x - u8g2_GetStrWidth(&display.u8g2, str) / 2, y, str);
}

static void drawTextRight(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&display.u8g2, x - u8g2_GetStrWidth(&display.u8g2, str), y, str);
}

void drawLowBattery(void)
{
    u8g2_SetFont(&display.u8g2, font_icons);
    drawTextCenter("0", LCD_CENTER_X, LCD_CENTER_Y + 4);
}

void drawWelcome(void)
{
    u8g2_SetFont(&display.u8g2, font_helvR08);
    drawTextCenter(firmwareName, LCD_CENTER_X, LCD_CENTER_Y - 5);

    u8g2_SetFont(&display.u8g2, font_tiny5);
    drawTextCenter(firmwareVersion, LCD_CENTER_X, LCD_CENTER_Y + 11);
}

void drawStatusBar(void)
{
    uint8_t batteryLevel = getBatteryLevel();

    char alarmIcon = (settings.rateAlarm || settings.doseAlarm) ? '<' : ' ';
    char batteryIcon = (batteryLevel == BATTERY_LEVEL_CHARGING) ? ':' : '0' + batteryLevel;

    char statusBar[4];
    statusBar[0] = alarmIcon;
    statusBar[1] = '?';
    statusBar[2] = batteryIcon;
    statusBar[3] = 0;

    u8g2_SetFont(&display.u8g2, font_icons);
    drawTextRight(statusBar, LCD_WIDTH - 4, 8);
}

void drawTitle(const char *title)
{
    u8g2_SetFont(&display.u8g2, font_tiny5);
    drawTextCenter(title, LCD_CENTER_X, TITLE_Y);
}

void drawSubtitle(const char *subtitle)
{
    u8g2_SetFont(&display.u8g2, font_tiny5);
    drawTextCenter(subtitle, LCD_CENTER_X, SUBTITLE_Y);
}

void drawMeasurementValue(const char *mantissa, const char *characteristic)
{
    // Value
    u8g2_SetFont(&display.u8g2, font_helvR24);
    drawTextLeft(mantissa, MEASUREMENT_VALUE_X, MEASUREMENT_VALUE_Y);

    // Units
    u8g2_SetFont(&display.u8g2, font_helvR08);
    drawTextLeft(characteristic, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y - 16);
}

void drawConfidenceIntervals(uint32_t sampleNum)
{
    if (sampleNum == 0)
        return;

    uint32_t lowerConfidenceInterval;
    uint32_t upperConfidenceInterval;
    getConfidenceIntervals(sampleNum, &lowerConfidenceInterval, &upperConfidenceInterval);

    u8g2_SetFont(&display.u8g2, font_tiny5);

    char confidenceInterval[16];

    strcpy(confidenceInterval, "+");
    strcatNumber(confidenceInterval, upperConfidenceInterval, 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y - 7);

    strcpy(confidenceInterval, "-");
    strcatNumber(confidenceInterval, lowerConfidenceInterval, 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y);
}

void drawHistory(const char *minLabel, const char *maxLabel,
                 int32_t offset, uint32_t range)
{
    // Plot separators
    u8g2_DrawHLine(&display.u8g2, 0, HISTORY_VIEW_Y_TOP, LCD_WIDTH);
    u8g2_DrawHLine(&display.u8g2, 0, HISTORY_VIEW_Y_BOTTOM, LCD_WIDTH);

    // Data
    // uint8_t lastX = LCD_WIDTH;
    // uint8_t lastY = data[0];
    for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        uint8_t value = getHistoryDataPoint(i);
        if (value == 0)
            continue;

        uint32_t x = (LCD_WIDTH - 1) - i * LCD_WIDTH / HISTORY_BUFFER_SIZE;
        uint32_t y = (value + offset) * HISTORY_VALUE_DECADE / HISTORY_VIEW_HEIGHT / range;

        // Pixel
        // u8g2_DrawPixel(&display.u8g2, x, HISTORY_VIEW_Y_BOTTOM - y);
        // Solid
        u8g2_DrawVLine(&display.u8g2, x, HISTORY_VIEW_Y_BOTTOM - y, y);
        // Line
        // u8g2_DrawLine(&display.u8g2, x, HISTORY_VIEW_Y_BOTTOM - y,
        //               lastX, HISTORY_VIEW_Y_BOTTOM - lastY);
        // lastX = x;
        // lastY = y;
    }

    // Time divisors
    u8g2_SetDisplayRotation(&display.u8g2, U8G2_R1);
    for (uint32_t i = 0; i < 7; i++)
    {
        uint32_t x = (i + 1) * LCD_WIDTH / 8 - 1;
        u8g2_DrawXBM(&display.u8g2, HISTORY_VIEW_Y_TOP + 1, x, HISTORY_VIEW_HEIGHT - 1, 1, dotted4_bits);
    }
    u8g2_SetDisplayRotation(&display.u8g2, U8G2_R0);

    // Value divisors
    for (uint32_t i = 0; i < (uint32_t)(range - 1); i++)
    {
        uint32_t y = HISTORY_VIEW_Y_TOP + (i + 1) * HISTORY_VIEW_HEIGHT / range;
        u8g2_DrawXBM(&display.u8g2, 0, y, LCD_WIDTH, 1, dotted4_bits);
    }

    u8g2_SetFont(&display.u8g2, font_tiny5);

    drawTextLeft(minLabel, 1, HISTORY_VIEW_Y_BOTTOM + 1 + 6);
    drawTextLeft(maxLabel, 1, HISTORY_VIEW_Y_TOP - 1);
}

void drawRNGText(char *text)
{
    u8g2_SetFont(&display.u8g2, font_helvR08);

    uint8_t x = 0;
    uint8_t y = LCD_CENTER_Y + 4;

    while (*text != '\0')
    {
        uint8_t width = u8g2_GetGlyphWidth(&display.u8g2, *text);
        if ((x + width) >= LCD_WIDTH)
        {
            x = 0;
            y += 12;
        }

        u8g2_DrawGlyph(&display.u8g2, x, y, *text);

        text++;
        x += width;
    }
}

void drawStats(void)
{
    char line[64];

    u8g2_SetFont(&display.u8g2, font_tiny5);

    strcpy(line, " Life timer: ");
    strcatTime(line, state.lifeTime);
    strcat(line, " ");
    drawTextCenter(line, LCD_CENTER_X, STATS_VIEW_Y);

    strcpy(line, " Life counts: ");
    strcatNumber(line, state.lifePulseCount, 0);
    strcat(line, " ");
    drawTextCenter(line, LCD_CENTER_X, STATS_VIEW_Y + 8);
}

void drawGameBoard(const char board[8][9],
                   const char time[2][6],
                   const char moveHistory[GAME_MOVES_LINE_NUM][2][6],
                   const char *buttonText, bool buttonSelected)
{
    u8g2_SetFont(&display.u8g2, font_icons);
    for (uint32_t y = 0; y < 8; y++)
        drawTextLeft(board[y], GAME_VIEW_BOARD_X, GAME_VIEW_BOARD_Y + 8 * y);

    u8g2_SetFont(&display.u8g2, font_tiny5);
    drawTextLeft(time[0], GAME_VIEW_TIME_X, GAME_VIEW_TIME_UPPER_Y);
    drawTextLeft(time[1], GAME_VIEW_TIME_X, GAME_VIEW_TIME_LOWER_Y);

    for (uint32_t y = 0; y < GAME_MOVES_LINE_NUM; y++)
    {
        for (uint32_t x = 0; x < 2; x++)
        {
            drawTextLeft(moveHistory[y][x],
                         GAME_VIEW_MOVES_X + GAME_VIEW_MOVES_LINE_WIDTH * x,
                         GAME_VIEW_MOVES_Y + GAME_VIEW_MOVES_LINE_HEIGHT * y);
        }
    }

    if (buttonText)
    {
        if (buttonSelected)
            u8g2_DrawRBox(&display.u8g2, GAME_VIEW_BUTTON_X, GAME_VIEW_BUTTON_Y,
                          GAME_VIEW_BUTTON_WIDTH, GAME_VIEW_BUTTON_HEIGHT, 1);
        else
            u8g2_DrawRFrame(&display.u8g2, GAME_VIEW_BUTTON_X, GAME_VIEW_BUTTON_Y,
                            GAME_VIEW_BUTTON_WIDTH, GAME_VIEW_BUTTON_HEIGHT, 1);

        u8g2_SetDrawColor(&display.u8g2, !buttonSelected);
        drawTextCenter(buttonText, GAME_VIEW_BUTTON_X + GAME_VIEW_BUTTON_WIDTH / 2, GAME_VIEW_BUTTON_Y + 7);

        u8g2_SetDrawColor(&display.u8g2, 1);
    }
}

void drawMenu(const struct Menu *menu, uint32_t startIndex, uint32_t selectedIndex)
{
    u8g2_SetFont(&display.u8g2, font_helvR08);

    for (uint32_t i = 0, y = MENU_VIEW_Y_TOP;
         i < MENU_VIEW_LINE_NUM;
         i++, y += MENU_VIEW_LINE_HEIGHT)
    {
        uint32_t index = startIndex + i;

        const char *name = menu->getMenuOption(menu, index);
        if (!name)
            break;

        bool selected = (index == selectedIndex);

        u8g2_SetDrawColor(&display.u8g2, selected);
        u8g2_DrawBox(&display.u8g2, 0, y, LCD_WIDTH, MENU_VIEW_LINE_HEIGHT);

        u8g2_SetDrawColor(&display.u8g2, !selected);
        drawTextLeft(name, MENU_VIEW_LINE_TEXT_X, y + MENU_VIEW_LINE_HEIGHT - 2);
    }

    u8g2_SetDrawColor(&display.u8g2, 1);
}
