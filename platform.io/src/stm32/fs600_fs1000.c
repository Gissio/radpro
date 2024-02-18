/*
 * Rad Pro
 * Bosean FS-600/FS-1000 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS600) || defined(FS1000)

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/syscfg.h>

#include "../display.h"
#include "../events.h"
#include "../flash.h"
#include "../keyboard.h"
#include "../settings.h"
#include "../system.h"

#include "stm32.h"

#include "mcu-renderer-st7565.h"

// System

void initSystem(void)
{
    // Clocks

    rcc_set_hsisys_div(RCC_CR_HSIDIV_DIV2);

    // GPIO

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);

    // Disable UCPD strobes
    SYSCFG_CFGR1 |= (SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);
}

// Flash memory

#if !defined(DEBUG)
const FlashRegion flashSettingsRegion = {0x10, 0x11};
const FlashRegion flashDatalogRegion = {0x11, 0x40};
#else
const FlashRegion flashSettingsRegion = {0x20, 0x21};
const FlashRegion flashDatalogRegion = {0x21, 0x40};
#endif

// Communications

#if defined(FS600)

const char *const commId = "Bosean FS-600;" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(FS1000)

const char *const commId = "Bosean FS-1000;" FIRMWARE_NAME " " FIRMWARE_VERSION;

#endif

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO

#if defined(KEYBOARD_5KEYS)

    gpio_mode_setup(KEY_DOWN_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLDOWN,
                    KEY_DOWN_PIN);
    gpio_mode_setup(KEY_UP_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_UP_PIN);
    gpio_mode_setup(KEY_SELECT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_SELECT_PIN);
    gpio_mode_setup(GPIOC,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_RIGHT | KEY_LEFT);

#elif defined(KEYBOARD_2KEYS)

    gpio_mode_setup(KEY_SELECT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_SELECT_PIN);
    gpio_mode_setup(KEY_LEFT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_LEFT);

#endif
}

void getKeyboardState(bool *isKeyDown)
{
#if defined(FS600)

    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_UP] = gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);

#elif defined(FS1000)

    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);

#endif
}

// Display

static uint8_t displayFramebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

extern mr_t mr;

static const uint8_t fs600_fs1000_st7567_init_sequence[] = {
    MR_SEND_COMMAND(MR_ST7565_BIAS_1_9),
    MR_SEND_COMMAND(MR_ST7565_SEG_NORMAL),
    MR_SEND_COMMAND(MR_ST7565_COM_REVERSE),
    MR_SEND_COMMAND(MR_ST7565_REGULATIONRATIO_5_0),
    MR_END(),
};

void onDisplaySleep(uint32_t value);
void onDisplaySetReset(bool value);
void onDisplaySetCommand(bool value);
void onDisplaySend(uint16_t value);

void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

void onDisplaySetReset(bool value)
{
    if (value)
        gpio_clear(LCD_RSTB_PORT,
                   LCD_RSTB_PIN);
    else
        gpio_set(LCD_RSTB_PORT,
                 LCD_RSTB_PIN);
}

void onDisplaySetCommand(bool value)
{
    if (value)
        gpio_clear(LCD_A0_PORT,
                   LCD_A0_PIN);
    else
        gpio_set(LCD_A0_PORT,
                 LCD_A0_PIN);
}

void onDisplaySend(uint16_t value)
{
    uint32_t data = 0x1ff << (16 + 11 - 7) |
                    (value << (12 - 7));

    GPIOB_BRR = LCD_CSB_PIN;

    const uint32_t mask =
        ((LCD_SCLK_PIN | LCD_SDA_PIN) << 16) |
        LCD_SDA_PIN;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    data <<= 1;

    GPIOB_BSRR = data & mask;
    GPIOB_BSRR = LCD_SCLK_PIN;

    GPIOB_BSRR = LCD_CSB_PIN;
}

void initDisplayHardware(void)
{
    // GPIO

    gpio_set(LCD_CSB_PORT, LCD_CSB_PIN);
    gpio_set(LCD_SCLK_PORT, LCD_SCLK_PIN);

    gpio_mode_setup(GPIOB,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    LCD_CSB_PIN | LCD_RSTB_PIN |
                        LCD_A0_PIN | LCD_SCLK_PIN |
                        LCD_SDA_PIN);
    gpio_set_output_options(GPIOB,
                            GPIO_OTYPE_PP,
                            GPIO_OSPEED_VERYHIGH,
                            LCD_BACKLIGHT_PIN | LCD_CSB_PIN |
                                LCD_RSTB_PIN | LCD_A0_PIN |
                                LCD_SCLK_PIN | LCD_SDA_PIN);

    gpio_mode_setup(LCD_BACKLIGHT_PORT,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    LCD_BACKLIGHT_PIN);
    gpio_set_af(LCD_BACKLIGHT_PORT,
                LCD_BACKLIGHT_AF,
                LCD_BACKLIGHT_PIN);

    // Backlight timer

    rcc_periph_clock_enable(LCD_BACKLIGHT_TIMER_RCC);

    LCD_BACKLIGHT_TIMER_CCMR(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCMR_MODE; // timer_set_oc_mode(LCD_BACKLIGHT_TIMER, TIM_OC1, TIM_OCM_PWM1);
    TIM_ARR(LCD_BACKLIGHT_TIMER) = LCD_BACKLIGHT_PWM_PERIOD - 1;                    // timer_set_period(LCD_BACKLIGHT_TIMER, BACKLIGHT_PWM_PERIOD);

    setDisplayBacklight(false);

    TIM_CCER(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCER_CC; // timer_enable_oc_output(LCD_BACKLIGHT_TIMER, TIM_OC1);
    TIM_BDTR(LCD_BACKLIGHT_TIMER) |= TIM_BDTR_MOE;                // timer_enable_break_main_output(LCD_BACKLIGHT_TIMER);
    TIM_CR1(LCD_BACKLIGHT_TIMER) |= TIM_CR1_CEN;                  // timer_enable_counter(LCD_BACKLIGHT_TIMER);

    // mcu-renderer

    mr_st7565_init(&mr,
                   DISPLAY_WIDTH,
                   DISPLAY_HEIGHT,
                   MR_DISPLAY_ROTATION_0,
                   displayFramebuffer,
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend);

    mr_send_sequence(&mr, fs600_fs1000_st7567_init_sequence);

    updateDisplayContrast();
}

void updateDisplayContrast(void)
{
    mr_send_command(&mr,
                    MR_ST7565_ELECTRONIC_VOLUME);
    mr_send_data(&mr,
                 24 + (settings.displayContrast << 2));
}

#endif
