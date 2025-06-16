/*
 * Rad Pro
 * Bosean FS-600/FS-1000 specifics
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(FS600) || defined(FS1000)

#include "../display.h"
#include "../events.h"
#include "../flash.h"
#include "../keyboard.h"
#include "../settings.h"
#include "../system.h"

#include "device.h"

#include "mcu-renderer-st7565.h"

// System

void initSystem(void)
{
    // Set SYSCLK to HSI16/2
    modify_bits(RCC->CR,
                RCC_CR_HSIDIV_Msk,
                RCC_CR_HSIDIV_DIV2);

    // Disable UCPD strobes
    set_bits(RCC->APBENR2,
             RCC_APBENR2_SYSCFGEN);
    set_bits(SYSCFG->CFGR1,
             SYSCFG_CFGR1_UCPD1_STROBE |
                 SYSCFG_CFGR1_UCPD2_STROBE);

    // Enable GPIOA, GPIOB, GPIOC, GPIOD
    set_bits(RCC->IOPENR,
             RCC_IOPENR_GPIOAEN |
                 RCC_IOPENR_GPIOBEN |
                 RCC_IOPENR_GPIOCEN |
                 RCC_IOPENR_GPIODEN);
}

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
    gpio_setup_input(KEY_LEFT_PORT,
                     KEY_LEFT_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_RIGHT_PORT,
                     KEY_RIGHT_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_UP_PORT,
                     KEY_UP_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_DOWN_PORT,
                     KEY_DOWN_PIN,
                     GPIO_PULL_DOWN);
    gpio_setup_input(KEY_OK_PORT,
                     KEY_OK_PIN,
                     GPIO_PULL_FLOATING);
}

void getKeyboardState(bool *isKeyDown)
{
#if defined(KEYBOARD_2KEYS)
    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
#elif defined(KEYBOARD_5KEYS)
    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_UP] = gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_OK] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
#endif
}

// Display

extern mr_t mr;

bool displayEnabled;

static uint8_t displayFramebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7565_BIAS_1_9),
    MR_SEND_COMMAND(MR_ST7565_SEG_NORMAL),
    MR_SEND_COMMAND(MR_ST7565_COM_REVERSE),
    MR_SEND_COMMAND(MR_ST7565_REGULATIONRATIO_5_0),
    MR_END(),
};

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

static void onDisplaySetReset(bool value)
{
    gpio_modify(DISPLAY_RSTB_PORT,
                DISPLAY_RSTB_PIN,
                !value);
}

static void onDisplaySetChipselect(bool value)
{
}

static void onDisplaySetCommand(bool value)
{
    gpio_modify(DISPLAY_A0_PORT,
                DISPLAY_A0_PIN,
                !value);
}

static void onDisplaySend(uint16_t value)
{
    GPIOB->BRR = get_bitvalue(DISPLAY_CSB_PIN);

    uint32_t data = 0x1ff << (16 + 11 - 7) |
                    (value << (12 - 7));

    const uint32_t mask = ((get_bitvalue(DISPLAY_SCLK_PIN) | get_bitvalue(DISPLAY_SDA_PIN)) << 16) |
                          get_bitvalue(DISPLAY_SDA_PIN);

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    data <<= 1;

    GPIOB->BSRR = data & mask;
    GPIOB->BSRR = get_bitvalue(DISPLAY_SCLK_PIN);

    GPIOB->BSRR = get_bitvalue(DISPLAY_CSB_PIN);
}

void initDisplay(void)
{
    // GPIO
    gpio_set(DISPLAY_RSTB_PORT,
             DISPLAY_RSTB_PIN);
    gpio_set(DISPLAY_CSB_PORT,
             DISPLAY_CSB_PIN);
    gpio_set(DISPLAY_SCLK_PORT,
             DISPLAY_SCLK_PIN);

    gpio_setup_output(DISPLAY_RSTB_PORT,
                      DISPLAY_RSTB_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_CSB_PORT,
                      DISPLAY_CSB_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_A0_PORT,
                      DISPLAY_A0_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_SCLK_PORT,
                      DISPLAY_SCLK_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_SDA_PORT,
                      DISPLAY_SDA_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);

    // mcu-renderer
    mr_st7565_init(&mr,
                   DISPLAY_WIDTH,
                   DISPLAY_HEIGHT,
                   MR_DISPLAY_ROTATION_0,
                   displayFramebuffer,
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetChipselect,
                   onDisplaySetCommand,
                   onDisplaySend);

    mr_send_sequence(&mr,
                     displayInitSequence);

    updateDisplayContrast();
}

void enableDisplay(bool value)
{
    mr_st7565_set_display(&mr, value);

    displayEnabled = value;
}

bool isDisplayEnabled(void)
{
    return displayEnabled;
}

void updateDisplayContrast(void)
{
    mr_send_command(&mr,
                    MR_ST7565_ELECTRONIC_VOLUME);
    mr_send_command(&mr,
                    20 + (settings.displayContrast << 2));
}

void refreshDisplay(void)
{
    mr_st7565_refresh_display(&mr);
}

#endif
