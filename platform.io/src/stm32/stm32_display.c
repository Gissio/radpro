/*
 * Rad Pro
 * STM32 display
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <clib/u8g2.h>

#include <libopencm3/stm32/gpio.h>

#include "../display.h"
#include "../events.h"

#include "stm32.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

extern u8g2_t displayU8g2;

#if defined(DISPLAY_MONO)

static bool displayBacklightEnabled;

#endif

static uint8_t onDisplay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayByte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayGPIO(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void initDisplayHardware(void)
{
    // Pulse LED
#if defined(PULSE_LED)

#if defined(PULSE_LED_ACTIVE_LOW)

    setPulseLED(false);

#endif

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(PULSE_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PULSE_LED_PIN);

#elif defined(STM32F1)

    gpio_set_mode(PULSE_LED_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, PULSE_LED_PIN);

#endif

#endif

    // LCD data & control

    gpio_set(LCD_EN_PORT, LCD_EN_PIN);

#if defined(FS2011) && defined(STM32F0)

    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, LCD_PORT_SPEED,
                            LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_BACKLIGHT_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, LCD_PORT_SPEED,
                            LCD_BACKLIGHT_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);

    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D5_PIN | LCD_D6_PIN);
    gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, LCD_PORT_SPEED,
                            LCD_D5_PIN | LCD_D6_PIN);

#elif defined(FS2011) && defined(STM32F1)

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_BACKLIGHT_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);

#elif defined(FS600) || defined(FS1000)

    gpio_set(LCD_CLOCK_PORT, LCD_CLOCK_PIN);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_BACKLIGHT_PIN | LCD_EN_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_CLOCK_PIN | LCD_DATA_PIN);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_VERYHIGH,
                            LCD_BACKLIGHT_PIN | LCD_EN_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_CLOCK_PIN | LCD_DATA_PIN);

#endif

    // U8g2

    uint8_t u8g2TileHeight;
    uint8_t *u8g2Buffer;

    u8g2_SetupDisplay(&displayU8g2, onDisplay, u8x8_cad_001, onDisplayByte, onDisplayGPIO);

#if defined(DISPLAY_128X64)

    u8g2Buffer = u8g2_m_16_8_f(&u8g2TileHeight);

#elif defined(DISPLAY_320X240)

    u8g2Buffer = u8g2_m_40_30_f(&u8g2TileHeight);

#endif

    u8g2_SetupBuffer(&displayU8g2, u8g2Buffer, u8g2TileHeight, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

    u8g2_InitDisplay(&displayU8g2);
}

#if defined(PULSE_LED)

void setPulseLED(bool value)
{
#if defined(PULSE_LED_ACTIVE_LOW)

    if (value)
        gpio_clear(PULSE_LED_PORT, PULSE_LED_PIN);
    else
        gpio_set(PULSE_LED_PORT, PULSE_LED_PIN);

#else

    if (value)
        gpio_set(PULSE_LED_PORT, PULSE_LED_PIN);
    else
        gpio_clear(PULSE_LED_PORT, PULSE_LED_PIN);

#endif
}

#endif

void setDisplay(bool value)
{
    u8g2_SetPowerSave(&displayU8g2, !value);
}

#if defined(DISPLAY_MONO)

void setBacklight(bool value)
{
    displayBacklightEnabled = value;

    if (value)
        gpio_set(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
    else
        gpio_clear(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

bool getBacklight(void)
{
    return displayBacklightEnabled;
}

#endif

static const u8x8_display_info_t dg128064_display_info = {
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
    4,                  // flipmode_x_offset
    DISPLAY_WIDTH,      // pixel_width
    DISPLAY_HEIGHT,     // pixel_height
};

#if defined(FS2011)

static const uint8_t display_init_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(0xa2),        // Bias: 1/9
    U8X8_C(0xa0),        // SEG direction: normal direction
    U8X8_C(0xc8),        // COM direction: reverse direction
    U8X8_C(0x23),        // Regulation ratio: 4.5
    U8X8_CA(0x81, 0x28), // Electronic volume: 40
    U8X8_C(0x2f),        // Built-in power circuit: booster, regulator, follower on
    U8X8_END_TRANSFER(),
    U8X8_END(),
};

#elif defined(FS600) || defined(FS1000)

static const uint8_t display_init_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(0xa2),        // Bias: 1/9
    U8X8_C(0xa0),        // SEG direction: normal direction
    U8X8_C(0xc8),        // COM direction: reverse direction
    U8X8_C(0x24),        // Regulation ratio: 5.0
    U8X8_CA(0x81, 0x20), // Electronic volume: 32
    U8X8_C(0x2f),        // Built-in power circuit: booster, regulator, follower on
    U8X8_END_TRANSFER(),
    U8X8_END(),
};

#elif defined(GC01)

static const uint8_t display_init_seq[] = {
    U8X8_START_TRANSFER(),

    U8X8_END_TRANSFER(),
    U8X8_END(),
};

#endif

static const uint8_t st7567_132x64_powersave_off_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(0xa4), // All pixel: normal display
    U8X8_C(0xaf), // Display: on
    U8X8_END_TRANSFER(),
    U8X8_END(),
};

static const uint8_t st7567_132x64_powersave_on_seq[] = {
    U8X8_START_TRANSFER(),
    U8X8_C(0xae), // Display: off
    U8X8_C(0xa5), // All pixel: all pixel on
    U8X8_END_TRANSFER(),
    U8X8_END(),
};

static uint8_t onDisplayGPIO(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
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
#if defined(FS2011)

        uint8_t *p = (uint8_t *)arg_ptr;
        for (uint32_t byteIndex = 0; byteIndex < arg_int; byteIndex++)
        {
            uint32_t value = *p++;

            GPIOB_BSRR = LCD_EN_PIN;

#if defined(STM32F0)

            GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
            GPIOF_BSRR = (0b01100000 << 17) | ((value & 0b01100000) << 1);

#elif defined(STM32F1)

            GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
            GPIOB_BSRR = (0b01100000 << 19) | ((value & 0b01100000) << 3);

#endif

            GPIOB_BRR = LCD_EN_PIN;
        }

#elif defined(FS600) || defined(FS1000)

        uint8_t *p = (uint8_t *)arg_ptr;
        for (uint32_t byteIndex = 0; byteIndex < arg_int; byteIndex++)
        {
            uint32_t value = 0x1ff << (16 + 11 - 7) | ((*p++) << (12 - 7));

            GPIOB_BRR = LCD_EN_PIN;

            const uint32_t mask = ((LCD_CLOCK_PIN | LCD_DATA_PIN) << 16) | LCD_DATA_PIN;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            value <<= 1;

            GPIOB_BSRR = value & mask;
            GPIOB_BSRR = LCD_CLOCK_PIN;

            GPIOB_BSRR = LCD_EN_PIN;
        }

#endif

        break;
    }

    default:
        return 0;
    }

    return 1;
}

static uint8_t onDisplay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
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
        u8x8_cad_SendSequence(u8x8, display_init_seq);

        break;

    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
        if (arg_int == 0)
            u8x8_cad_SendSequence(u8x8, st7567_132x64_powersave_off_seq);
        else
            u8x8_cad_SendSequence(u8x8, st7567_132x64_powersave_on_seq);

        break;

    case U8X8_MSG_DISPLAY_DRAW_TILE:
        u8x8_cad_StartTransfer(u8x8);

        x = ((u8x8_tile_t *)arg_ptr)->x_pos;
        x *= 8;
        x += u8x8->x_offset;
        u8x8_cad_SendCmd(u8x8, 0x10 | (x >> 4));
        u8x8_cad_SendCmd(u8x8, 0x00 | ((x & 15)));
        u8x8_cad_SendCmd(u8x8, 0xb0 | (((u8x8_tile_t *)arg_ptr)->y_pos));

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

#endif
