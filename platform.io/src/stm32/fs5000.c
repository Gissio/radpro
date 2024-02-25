/*
 * Rad Pro
 * Bosean FS-5000 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS5000)

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

#include "device.h"

#include "mcu-renderer-st7789.h"

// System

void initSystem(void)
{
}

// Communications

const char *const commId = "Bosean FS-5000;" FIRMWARE_NAME " " FIRMWARE_VERSION;

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO

    gpio_mode_setup(KEY_LEFT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLDOWN,
                    KEY_LEFT_PIN);
    gpio_mode_setup(KEY_RIGHT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_RIGHT_PIN);
    gpio_mode_setup(KEY_SELECT_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_NONE,
                    KEY_SELECT_PIN);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);
}

// Display

extern mr_t mr;

static uint8_t displayTextbuffer[2 * 36 * 50];

static const uint8_t displayInitSequence[] = {
    MR_END(),
};

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

static void onDisplaySetReset(bool value)
{
    if (value)
        gpio_clear(LCD_RESX_PORT,
                   LCD_RESX_PIN);
    else
        gpio_set(LCD_RESX_PORT,
                 LCD_RESX_PIN);
}

static void onDisplaySetCommand(bool value)
{
    if (value)
        gpio_clear(LCD_DCX_PORT,
                   LCD_DCX_PIN);
    else
        gpio_set(LCD_DCX_PORT,
                 LCD_DCX_PIN);
}

static void onDisplaySend(uint16_t value)
{
    GPIOC_BRR = LCD_CSX_PIN;
    GPIOC_BRR = LCD_WRX_PIN;
    GPIOB_ODR = value;
    GPIOC_BSRR = LCD_WRX_PIN;
    GPIOC_BSRR = LCD_CSX_PIN;
}

void initDisplayHardware(void)
{
    // GPIO

    gpio_set(GPIOC,
             LCD_RESX_PIN | LCD_RDX_PIN |
                 LCD_WRX_PIN);
    gpio_mode_setup(GPIOB,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    LCD_RESX_PIN | LCD_RDX_PIN |
                        LCD_WRX_PIN | LCD_DCX_PIN |
                        LCD_CSX_PIN);

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

    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_0,
                   displayTextbuffer,
                   sizeof(displayTextbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend);

    mr_send_sequence(&mr, displayInitSequence);
}

#endif
