/*
 * Rad Pro
 * FS2011 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS2011)

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
#if defined(STM32F0)

    // Enable GPIOA, GPIOB, GPIOF
    set_bits(RCC->AHBENR,
             RCC_AHBENR_GPIOAEN |
                 RCC_AHBENR_GPIOBEN |
                 RCC_AHBENR_GPIOFEN);

#elif defined(STM32F1)

    // Disable JTAG, TIM3 partial remap
    rcc_enable_afio();
    modify_bits(AFIO->MAPR,
                AFIO_MAPR_SWJ_CFG_Msk | AFIO_MAPR_TIM3_REMAP_Msk,
                AFIO_MAPR_SWJ_CFG_JTAGDISABLE | AFIO_MAPR_TIM3_REMAP_1);

    // Enable GPIOA, GPIOB
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_IOPAEN |
                 RCC_APB2ENR_IOPBEN);

#endif
}

// Communications

#if defined(STM32F0) && !defined(GD32)
const char *const commId = "FS2011 (STM32F051C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;
#elif defined(STM32F0) && defined(GD32)
const char *const commId = "FS2011 (GD32F150C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;
#elif defined(STM32F1)
const char *const commId = "FS2011 (GD32F103C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;
#endif

// Keyboard

void initKeyboardController(void)
{
#if defined(STM32F0)
    gpio_setup_input(KEY_PLAYPAUSE_PORT,
                     KEY_PLAYPAUSE_PIN,
                     GPIO_PULL_UP);
    gpio_setup_input(KEY_MENUOK_PORT,
                     KEY_MENUOK_PIN,
                     GPIO_PULL_UP);
    gpio_setup_input(KEY_UP_PORT,
                     KEY_UP_PIN,
                     GPIO_PULL_UP);
    gpio_setup_input(KEY_DOWN_PORT,
                     KEY_DOWN_PIN,
                     GPIO_PULL_UP);
    gpio_setup_input(KEY_POWER_PORT,
                     KEY_POWER_PIN,
                     GPIO_PULL_UP);
#elif defined(STM32F1)
    gpio_setup(KEY_PLAYPAUSE_PORT,
               KEY_PLAYPAUSE_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_MENUOK_PORT,
               KEY_MENUOK_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_UP_PORT,
               KEY_UP_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_DOWN_PORT,
               KEY_DOWN_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_POWER_PORT,
               KEY_POWER_PIN,
               GPIO_MODE_INPUT_PULLUP);
#endif
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_PLAYPAUSE_PORT, KEY_PLAYPAUSE_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_MENUOK_PORT, KEY_MENUOK_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
}

// Display

extern mr_t mr;

static uint8_t displayFramebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7565_BIAS_1_9),
    MR_SEND_COMMAND(MR_ST7565_SEG_NORMAL),
    MR_SEND_COMMAND(MR_ST7565_COM_REVERSE),
    MR_SEND_COMMAND(MR_ST7565_REGULATIONRATIO_4_5),
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

static void onDisplaySetCommand(bool value)
{
    gpio_modify(DISPLAY_A0_PORT,
                DISPLAY_A0_PIN,
                !value);
}

static void onDisplaySend(uint16_t value)
{
    DISPLAY_E_PORT->BSRR = get_bitvalue(DISPLAY_E_PIN);
#if defined(STM32F0)
    GPIOA->BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
    GPIOF->BSRR = (0b01100000 << 17) | ((value & 0b01100000) << 1);
#elif defined(STM32F1)
    GPIOA->BSRR = (0b10011111 << 24) | ((value & 0b10011111) << 8);
    GPIOB->BSRR = (0b01100000 << 19) | ((value & 0b01100000) << 3);
#endif
    DISPLAY_E_PORT->BRR = get_bitvalue(DISPLAY_E_PIN);
}

void initDisplayController(void)
{
    // GPIO
    gpio_set(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN);
    gpio_clear(DISPLAY_RW_PORT, DISPLAY_RW_PIN);
    gpio_clear(DISPLAY_E_PORT, DISPLAY_E_PIN);

#if defined(STM32F0)
    gpio_setup_output(DISPLAY_RSTB_PORT,
                      DISPLAY_RSTB_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_A0_PORT,
                      DISPLAY_A0_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_RW_PORT,
                      DISPLAY_RW_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_E_PORT,
                      DISPLAY_E_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D0_PORT,
                      DISPLAY_D0_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D1_PORT,
                      DISPLAY_D1_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D2_PORT,
                      DISPLAY_D2_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D3_PORT,
                      DISPLAY_D3_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D4_PORT,
                      DISPLAY_D4_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D5_PORT,
                      DISPLAY_D5_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D6_PORT,
                      DISPLAY_D6_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_D7_PORT,
                      DISPLAY_D7_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(DISPLAY_RSTB_PORT,
               DISPLAY_RSTB_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_A0_PORT,
               DISPLAY_A0_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_RW_PORT,
               DISPLAY_RW_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_E_PORT,
               DISPLAY_E_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D0_PORT,
               DISPLAY_D0_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D1_PORT,
               DISPLAY_D1_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D2_PORT,
               DISPLAY_D2_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D3_PORT,
               DISPLAY_D3_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D4_PORT,
               DISPLAY_D4_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D5_PORT,
               DISPLAY_D5_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D6_PORT,
               DISPLAY_D6_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_D7_PORT,
               DISPLAY_D7_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
#endif

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

    mr_send_sequence(&mr,
                     displayInitSequence);

    updateDisplayContrast();
}

void updateDisplayContrast(void)
{
    mr_send_command(&mr,
                    MR_ST7565_ELECTRONIC_VOLUME);
    mr_send_command(&mr,
                    20 + (settings.displayContrast << 2));
}

#endif
