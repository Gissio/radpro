/*
 * Rad Pro
 * FS2011 display interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <clib/u8g2.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../display.h"
#include "../../events.h"
#include "fs2011.h"

extern u8g2_t displayU8g2;

static bool displayBacklightOn;

static uint8_t onDisplay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayByte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
static uint8_t onDisplayGPIO(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void initDisplay(void)
{
    // LCD data & control
#ifdef STM32F0
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_BACKLIGHT_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_BACKLIGHT_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);

    gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    LCD_D5_PIN | LCD_D6_PIN);
    gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH,
                            LCD_D5_PIN | LCD_D6_PIN);
#elif STM32F1
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_D0_PIN | LCD_D1_PIN | LCD_D2_PIN | LCD_D3_PIN | LCD_D4_PIN | LCD_D7_PIN);

    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_BACKLIGHT_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_RESET_PIN | LCD_RS_PIN | LCD_RW_PIN | LCD_EN_PIN);
#endif

    // U8g2
    uint8_t u8g2TileBufHeight;
    uint8_t *u8g2Buffer;

    u8g2_SetupDisplay(&displayU8g2, onDisplay, u8x8_cad_001, onDisplayByte, onDisplayGPIO);
    u8g2Buffer = u8g2_m_16_8_f(&u8g2TileBufHeight);
    u8g2_SetupBuffer(&displayU8g2, u8g2Buffer, u8g2TileBufHeight, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

    // u8g2_Setup_st7567_enh_dg128064_f(&displayU8g2, U8G2_R0, onDisplayByte, onDisplayGPIO);

    u8g2_InitDisplay(&displayU8g2);

    // Enable
    setDisplay(true);
}

void setDisplay(bool value)
{
    u8g2_SetPowerSave(&displayU8g2, !value);
}

void setBacklight(bool value)
{
    displayBacklightOn = value;

    if (value)
        gpio_set(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
    else
        gpio_clear(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);
}

bool getBacklight(void)
{
    return displayBacklightOn;
}

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

static const uint8_t st7567_132x64_powersave_off_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0a4),         /* all pixel off, issue 142 */
    U8X8_C(0x0af),         /* display on */
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
};

static const uint8_t st7567_132x64_powersave_on_seq[] = {
    U8X8_START_TRANSFER(), /* enable chip, delay is part of the transfer start */
    U8X8_C(0x0ae),         /* display off */
    U8X8_C(0x0a5),         /* enter powersafe: all pixel on, issue 142 */
    U8X8_END_TRANSFER(),   /* disable chip */
    U8X8_END()             /* end of sequence */
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
        uint8_t *p = (uint8_t *)arg_ptr;
        for (uint32_t i = 0; i < arg_int; i++, p++)
        {
            uint8_t value = *p;

            GPIOB_BSRR = LCD_EN_PIN; // gpio_set(LCD_EN_PORT, LCD_EN_PIN);
#ifdef STM32F0
            GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
            GPIOF_BSRR = (0b01100000 << 17) | ((value & 0b01100000) << 1);
#elif STM32F1
            GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
            GPIOB_BSRR = (0b01100000 << 19) | ((value & 0b01100000) << 3);
#endif
            GPIOB_BRR = LCD_EN_PIN; // gpio_clear(LCD_EN_PORT, LCD_EN_PIN);
        }

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
        u8x8_cad_SendSequence(u8x8, dg128064_init_seq);

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

#endif
