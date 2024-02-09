/*
 * Rad Pro
 * FS2011 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS2011)

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "../display.h"
#include "../events.h"
#include "../flash.h"
#include "../keyboard.h"
#include "../settings.h"
#include "../system.h"

#include "stm32.h"

#include "mcu-renderer-st7565.h"

// Flash memory

// +++ TEST
// const FlashRegion flashSettingsRegion = {0x20, 0x21};
// const FlashRegion flashDatalogRegion = {0x21, 0x40};
const FlashRegion flashSettingsRegion = {0x30, 0x31};
const FlashRegion flashDatalogRegion = {0x31, 0x40};
// +++ TEST

// Communications

#if defined(STM32F0) && !defined(GD32)

const char *const commId = "FS2011 (STM32F051C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(STM32F0) && defined(GD32)

const char *const commId = "FS2011 (GD32F150C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(STM32F1)

const char *const commId = "FS2011 (GD32F103C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#endif

// System

void initSystem(void)
{
    // GPIO

#if defined(STM32F0)

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOF);

#elif defined(STM32F1)

    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
                       AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

#endif
}

// Keyboard

void initKeyboardHardware(void)
{
#if defined(STM32F0)

    gpio_mode_setup(KEY_UP_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLUP,
                    KEY_UP_PIN);
    gpio_mode_setup(GPIOB,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLUP,
                    KEY_DOWN_PIN | KEY_ENTER_PIN |
                        KEY_BACK_PIN | KEY_POWER_PIN);

#elif defined(STM32F1)

    gpio_set(KEY_UP_PORT, KEY_UP_PIN); // Pull-up
    gpio_set_mode(KEY_UP_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_UP_PIN);

    gpio_set(GPIOB,
             KEY_DOWN_PIN | KEY_ENTER_PIN |
                 KEY_BACK_PIN | KEY_POWER_PIN); // Pull-up
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_DOWN_PIN | KEY_ENTER_PIN |
                      KEY_BACK_PIN | KEY_POWER_PIN);

#endif
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_ENTER] = !gpio_get(KEY_ENTER_PORT, KEY_ENTER_PIN);
    isKeyDown[KEY_BACK] = !gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);
}

// Display

static uint8_t framebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

extern mr_t mr;

static const uint8_t fs2011_st7567_init_sequence[] = {
    MR_SEND_COMMAND(MR_ST7565_BIAS_1_9),
    MR_SEND_COMMAND(MR_ST7565_SEG_NORMAL),
    MR_SEND_COMMAND(MR_ST7565_COM_REVERSE),
    MR_SEND_COMMAND(MR_ST7565_REGULATIONRATIO_4_5),
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
    GPIOB_BSRR = LCD_E_PIN;

#if defined(STM32F0)

    GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
    GPIOF_BSRR = (0b01100000 << 17) | ((value & 0b01100000) << 1);

#elif defined(STM32F1)

    GPIOA_BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
    GPIOB_BSRR = (0b01100000 << 19) | ((value & 0b01100000) << 3);

#endif

    GPIOB_BRR = LCD_E_PIN;
}

void initDisplayHardware(void)
{
    // GPIO

#if defined(STM32F0)

    gpio_mode_setup(GPIOA,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    LCD_D0_PIN | LCD_D1_PIN |
                        LCD_D2_PIN | LCD_D3_PIN |
                        LCD_D4_PIN | LCD_D7_PIN);
    gpio_set_output_options(GPIOA,
                            GPIO_OTYPE_PP,
                            GPIO_OSPEED_HIGH,
                            LCD_D0_PIN | LCD_D1_PIN |
                                LCD_D2_PIN | LCD_D3_PIN |
                                LCD_D4_PIN | LCD_D7_PIN);

    gpio_mode_setup(GPIOB,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    LCD_RSTB_PIN | LCD_A0_PIN |
                        LCD_RW_PIN | LCD_E_PIN);
    gpio_set_output_options(GPIOB,
                            GPIO_OTYPE_PP,
                            GPIO_OSPEED_HIGH,
                            LCD_BACKLIGHT_PIN | LCD_RSTB_PIN |
                                LCD_A0_PIN | LCD_RW_PIN |
                                LCD_E_PIN);

    gpio_mode_setup(GPIOF,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    LCD_D5_PIN | LCD_D6_PIN);
    gpio_set_output_options(GPIOF,
                            GPIO_OTYPE_PP,
                            GPIO_OSPEED_HIGH,
                            LCD_D5_PIN | LCD_D6_PIN);

    gpio_mode_setup(LCD_BACKLIGHT_PORT,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    LCD_BACKLIGHT_PIN);
    gpio_set_af(LCD_BACKLIGHT_PORT,
                LCD_BACKLIGHT_AF,
                LCD_BACKLIGHT_PIN);

#elif defined(STM32F1)

    gpio_set_mode(GPIOA,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_D0_PIN | LCD_D1_PIN |
                      LCD_D2_PIN | LCD_D3_PIN |
                      LCD_D4_PIN | LCD_D7_PIN);

    gpio_set_mode(GPIOB,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_D5_PIN |
                      LCD_D6_PIN | LCD_RSTB_PIN |
                      LCD_A0_PIN | LCD_RW_PIN |
                      LCD_RSTB_PIN);

    gpio_set_mode(LCD_BACKLIGHT_PORT,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  LCD_BACKLIGHT_PIN);

#endif

    // Backlight timer

    rcc_periph_clock_enable(LCD_BACKLIGHT_TIMER_RCC);

    LCD_BACKLIGHT_TIMER_CCMR(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCMR_MODE; // timer_set_oc_mode(LCD_BACKLIGHT_TIMER, TIM_OC1, TIM_OCM_PWM1);
    TIM_ARR(LCD_BACKLIGHT_TIMER) = LCD_BACKLIGHT_PWM_PERIOD;                        // timer_set_period(LCD_BACKLIGHT_TIMER, BACKLIGHT_PWM_PERIOD);

    setDisplayBacklight(false);

    TIM_CCER(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCER_CC; // timer_enable_oc_output(LCD_BACKLIGHT_TIMER, TIM_OC1);
    TIM_CR1(LCD_BACKLIGHT_TIMER) |= TIM_CR1_CEN;                  // timer_enable_counter(LCD_BACKLIGHT_TIMER);

    // mcu-renderer

    mr_st7565_init(&mr,
                   DISPLAY_WIDTH,
                   DISPLAY_HEIGHT,
                   MR_DISPLAY_ROTATION_0,
                   framebuffer,
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend);

    mr_send_sequence(&mr, fs2011_st7567_init_sequence);

    updateDisplayContrast();
}

void updateDisplayContrast()
{
    mr_send_command(&mr,
                    MR_ST7565_ELECTRONIC_VOLUME);
    mr_send_data(&mr,
                 28 + settings.displayContrast);
}

#endif
