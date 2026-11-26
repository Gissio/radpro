/*
 * Rad Pro
 * FS2011 driver
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(FS2011)

#include "mcu-renderer-st7565.h"

#include "../peripherals/display.h"
#include "../peripherals/keyboard.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../system/system.h"

// System

void initGPIO(void)
{
#if defined(STM32F0)
    // Enable GPIOA, GPIOB, GPIOF
    set_bits(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN);
#elif defined(STM32F1)
    // Enable GPIOA, GPIOB
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);
#endif
}

void initSystem(void)
{
    // Set system clock
    setFastSystemClock(false);

#if defined(STM32F1)
    // Disable JTAG, TIM2 partial remap (CH2 on PB3), TIM3 partial remap (CH1 on PB4)
    rcc_enable_afio();
    modify_bits(AFIO->MAPR,
                AFIO_MAPR_SWJ_CFG_Msk | AFIO_MAPR_TIM2_REMAP_Msk | AFIO_MAPR_TIM3_REMAP_Msk,
                AFIO_MAPR_SWJ_CFG_JTAGDISABLE | AFIO_MAPR_TIM2_REMAP_0 | AFIO_MAPR_TIM3_REMAP_1);
#endif
}

// Fast clock: HSI / 2 * 8 = 32 MHz, 1 flash wait state;
// slow clock: HSI = 8 MHz, 0 wait states. APB clocks are 4 MHz in both.
#define FAST_AHB_FREQUENCY 32000000

#if defined(STM32F0)
#define FAST_FLASH_LATENCY FLASH_ACR_LATENCY
#define FAST_CFGR (RCC_CFGR_SW_HSI |          \
                   RCC_CFGR_HPRE_DIV1 |       \
                   RCC_CFGR_PPRE_DIV8 |       \
                   RCC_CFGR_PLLSRC_HSI_DIV2 | \
                   RCC_CFGR_PLLMUL8)
#define SLOW_CFGR (RCC_CFGR_SW_HSI |    \
                   RCC_CFGR_HPRE_DIV1 | \
                   RCC_CFGR_PPRE_DIV2)
#elif defined(STM32F1)
#define FAST_FLASH_LATENCY FLASH_ACR_LATENCY_1WS
#define FAST_CFGR (RCC_CFGR_SW_HSI |          \
                   RCC_CFGR_HPRE_DIV1 |       \
                   RCC_CFGR_PPRE1_DIV8 |      \
                   RCC_CFGR_PPRE2_DIV8 |      \
                   RCC_CFGR_PLLSRC_HSI_DIV2 | \
                   RCC_CFGR_PLLMULL8)
#define SLOW_CFGR (RCC_CFGR_SW_HSI |     \
                   RCC_CFGR_HPRE_DIV1 |  \
                   RCC_CFGR_PPRE1_DIV2 | \
                   RCC_CFGR_PPRE2_DIV2)
#endif

void setFastSystemClock(bool value)
{
    // Set HSI as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSI);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    if (value)
    {
        // Set flash wait states; configure SW, AHB, APB and PLL
        modify_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk, FAST_FLASH_LATENCY);
        RCC->CFGR = FAST_CFGR;

        // Enable PLL
        set_bits(RCC->CR, RCC_CR_PLLON);
        wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

        // Set PLL as system clock
        modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
        wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

        // Update sys tick
        SysTick->LOAD = FAST_AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    }
    else
    {
        // Set 0 wait states for flash; configure SW, AHB and APB
        clear_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk);
        RCC->CFGR = SLOW_CFGR;

        // Update sys tick
        SysTick->LOAD = AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    }
}

// Communications

#if defined(STM32F0) && !defined(GD32)
const char *const commId = "FS2011 (STM32F051C8);" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#elif defined(STM32F0) && defined(GD32)
const char *const commId = "FS2011 (GD32F150C8);" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#elif defined(STM32F1)
const char *const commId = "FS2011 (GD32F103C8);" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#endif

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO
#if defined(STM32F0)
    gpio_setup_input(KEY_PLAYPAUSE_PORT, KEY_PLAYPAUSE_PIN, GPIO_PULL_PULLUP);
    gpio_setup_input(KEY_MENUOK_PORT, KEY_MENUOK_PIN, GPIO_PULL_PULLUP);
    gpio_setup_input(KEY_UP_PORT, KEY_UP_PIN, GPIO_PULL_PULLUP);
    gpio_setup_input(KEY_DOWN_PORT, KEY_DOWN_PIN, GPIO_PULL_PULLUP);
    gpio_setup_input(KEY_POWER_PORT, KEY_POWER_PIN, GPIO_PULL_PULLUP);
#elif defined(STM32F1)
    gpio_setup(KEY_PLAYPAUSE_PORT, KEY_PLAYPAUSE_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_MENUOK_PORT, KEY_MENUOK_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_UP_PORT, KEY_UP_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_DOWN_PORT, KEY_DOWN_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_POWER_PORT, KEY_POWER_PIN, GPIO_MODE_INPUT_PULLUP);
#endif
}

// Moves the (inverted) input bit of a pin to the bit of a key
#define getKey(keys, pin, key) ((((keys) >> (pin)) & 1) << (key))

uint32_t readKeyboardKeys(void)
{
    // All keys but KEY_UP share a port: read it once (keys are active low)
    uint32_t keys = ~KEY_POWER_PORT->IDR;

#if defined(KEYBOARD_2_KEYS)
    return getKey(keys, KEY_PLAYPAUSE_PIN, KEY_LEFT) |
           getKey(keys, KEY_POWER_PIN, KEY_RIGHT);
#elif defined(KEYBOARD_5_KEYS)
    return getKey(keys, KEY_PLAYPAUSE_PIN, KEY_LEFT) |
           getKey(keys, KEY_MENUOK_PIN, KEY_RIGHT) |
           (!gpio_get(KEY_UP_PORT, KEY_UP_PIN) << KEY_UP) |
           getKey(keys, KEY_DOWN_PIN, KEY_DOWN) |
           getKey(keys, KEY_POWER_PIN, KEY_OK);
#endif
}

// Display

extern mr_t mr;

static bool displayEnabled;

static uint8_t displayFramebuffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7565_BIAS_1_9),
    MR_SEND_COMMAND(MR_ST7565_SEG_NORMAL),
    MR_SEND_COMMAND(MR_ST7565_COM_REVERSE),
    MR_SEND_COMMAND(MR_ST7565_REGULATIONRATIO_4_5),
    MR_END(),
};

static void onDisplaySetReset(bool value)
{
    gpio_modify(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN, !value);
}

static void onDisplaySetChipselect(bool value)
{
}

static void onDisplaySetCommand(bool value)
{
    gpio_modify(DISPLAY_A0_PORT, DISPLAY_A0_PIN, !value);
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

#if defined(STM32F0)
// 2-bit MODER/OSPEEDR field mask of a pin
#define GPIO_MASK(pin) (0b11U << (2 * (pin)))

// Sets pins as 50 MHz push-pull outputs (OTYPER and PUPDR keep their
// push-pull/floating reset values)
#define setupDisplayOutputs(port, mask)                    \
    modify_bits((port)->MODER, mask, (mask) & 0x55555555); \
    set_bits((port)->OSPEEDR, mask)
#elif defined(STM32F1)
// 4-bit CRH field mask of a pin (all display pins are pins 8-15)
#define GPIO_MASK(pin) (0b1111U << (4 * ((pin) - 8)))

// Sets pins as 50 MHz push-pull outputs
#define setupDisplayOutputs(port, mask) \
    modify_bits((port)->CRH, mask, (mask) & (0x11111111 * GPIO_MODE_OUTPUT_50MHZ_PUSHPULL))
#endif

void initDisplay(void)
{
    // GPIO (port layout as in onDisplaySend)
    gpio_modify(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN, true);

    setupDisplayOutputs(DISPLAY_RSTB_PORT,
                        GPIO_MASK(DISPLAY_RSTB_PIN) |
                            GPIO_MASK(DISPLAY_A0_PIN) |
                            GPIO_MASK(DISPLAY_RW_PIN) |
#if defined(STM32F1)
                            GPIO_MASK(DISPLAY_D5_PIN) |
                            GPIO_MASK(DISPLAY_D6_PIN) |
#endif
                            GPIO_MASK(DISPLAY_E_PIN));
    setupDisplayOutputs(DISPLAY_D0_PORT,
                        GPIO_MASK(DISPLAY_D0_PIN) |
                            GPIO_MASK(DISPLAY_D1_PIN) |
                            GPIO_MASK(DISPLAY_D2_PIN) |
                            GPIO_MASK(DISPLAY_D3_PIN) |
                            GPIO_MASK(DISPLAY_D4_PIN) |
                            GPIO_MASK(DISPLAY_D7_PIN));
#if defined(STM32F0)
    setupDisplayOutputs(DISPLAY_D5_PORT,
                        GPIO_MASK(DISPLAY_D5_PIN) |
                            GPIO_MASK(DISPLAY_D6_PIN));
#endif

    // mcu-renderer
    mr_st7565_init(&mr,
                   DISPLAY_WIDTH,
                   DISPLAY_HEIGHT,
                   MR_DISPLAY_ROTATION_0,
                   displayFramebuffer,
                   sleep,
                   onDisplaySetReset,
                   onDisplaySetChipselect,
                   onDisplaySetCommand,
                   onDisplaySend);

    mr_send_sequence(&mr, displayInitSequence);

    updateDisplayContrast();

    initBacklight();
}

void setDisplayEnabled(bool value)
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
    mr_send_command(&mr, MR_ST7565_ELECTRONIC_VOLUME);
    mr_send_command(&mr, 20 + (settings.displayContrast << 2));
}

void refreshDisplay(void)
{
    mr_st7565_refresh_display(&mr);
}

#endif
