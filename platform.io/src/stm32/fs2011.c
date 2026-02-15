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

void initSystem(void)
{
    // Set system clock
    setFastSystemClock(false);

#if defined(STM32F0)
    // Enable GPIOA, GPIOB, GPIOF
    set_bits(RCC->AHBENR, RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN);
#elif defined(STM32F1)
    // Disable JTAG, TIM3 partial remap
    rcc_enable_afio();
    modify_bits(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_Msk | AFIO_MAPR_TIM3_REMAP_Msk, AFIO_MAPR_SWJ_CFG_JTAGDISABLE | AFIO_MAPR_TIM3_REMAP_1);

    // Enable GPIOA, GPIOB
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN);
#endif
}

void setFastSystemClock(bool value)
{
    // Set HSI as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSI);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

#if defined(STM32F0)
    if (value)
    {
        // Set 1 wait states for flash
        set_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk);

        // Configure SW, AHB, APB, PLL
        RCC->CFGR = RCC_CFGR_SW_HSI |          // Select HSI as system clock
                    RCC_CFGR_HPRE_DIV1 |       // Set AHB clock: 32 MHz / 1 = 32 MHz
                    RCC_CFGR_PPRE_DIV8 |       // Set APB clock: 32 MHz / 8 = 4 MHz
                    RCC_CFGR_PLLSRC_HSI_DIV2 | // Set PLL source: HSI / 2
                    RCC_CFGR_PLLMUL8;          // Set PLLMUL: 8x

        // Enable PLL
        set_bits(RCC->CR, RCC_CR_PLLON);
        wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

        // Set PLL as system clock
        modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
        wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

        // Update sys tick
        SysTick->LOAD = SysTick->LOAD << 2;
    }
    else
    {
        // Set 0 wait states for flash
        clear_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk);

        // Configure SW, AHB, APB
        RCC->CFGR = RCC_CFGR_SW_HSI |    // Select HSI as system clock
                    RCC_CFGR_HPRE_DIV1 | // Set AHB clock: 8 MHz / 1 = 8 MHz
                    RCC_CFGR_PPRE_DIV2;  // Set APB clock: 8 MHz / 2 = 4 MHz

        // Update sys tick
        SysTick->LOAD = SysTick->LOAD >> 2;
    }
#elif STM32F1
    if (value)
    {
        // Set 2 wait states for flash
        modify_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk, FLASH_ACR_LATENCY_2WS);

        // Configure SW, AHB, APB1, APB2, PLL
        RCC->CFGR = RCC_CFGR_SW_HSI |          // Select HSI as system clock
                    RCC_CFGR_HPRE_DIV1 |       // Set AHB clock: 32 MHz / 1 = 64 MHz
                    RCC_CFGR_PPRE1_DIV8 |      // Set APB1 clock: 32 MHz / 8 = 4 MHz
                    RCC_CFGR_PPRE2_DIV8 |      // Set APB2 clock: 32 MHz / 8 = 4 MHz
                    RCC_CFGR_PLLSRC_HSI_DIV2 | // Set PLL source: HSI / 2
                    RCC_CFGR_PLLMULL8;         // Set PLL multiplier: 8x

        // Enable PLL
        set_bits(RCC->CR, RCC_CR_PLLON);
        wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

        // Set PLL as system clock
        modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
        wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

        // Update sys tick
        SysTick->LOAD = SysTick->LOAD << 2;
    }
    else
    {
        // Set 0 wait states for flash
        clear_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk);

        // Configure SW, AHB, APB1, APB2
        RCC->CFGR = RCC_CFGR_SW_HSI |     // Select HSI as system clock
                    RCC_CFGR_HPRE_DIV1 |  // Set AHB clock: 8 MHz / 1 = 8 MHz
                    RCC_CFGR_PPRE1_DIV2 | // Set APB1 clock: 8 MHz / 2 = 4 MHz
                    RCC_CFGR_PPRE2_DIV2;  // Set APB2 clock: 8 MHz / 2 = 4 MHz

        // Update sys tick
        SysTick->LOAD = SysTick->LOAD >> 2;
    }
#endif
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

void getKeyboardState(bool *isKeyDown)
{
#if defined(KEYBOARD_2lKEYS)
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_PLAYPAUSE_PORT, KEY_PLAYPAUSE_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
#elif defined(KEYBOARD_5_KEYS)
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_PLAYPAUSE_PORT, KEY_PLAYPAUSE_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_MENUOK_PORT, KEY_MENUOK_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_OK] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
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

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

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

static GPIO_TypeDef *const displayPort[] = {
    DISPLAY_RSTB_PORT,
    DISPLAY_A0_PORT,
    DISPLAY_RW_PORT,
    DISPLAY_E_PORT,
    DISPLAY_D0_PORT,
    DISPLAY_D1_PORT,
    DISPLAY_D2_PORT,
    DISPLAY_D3_PORT,
    DISPLAY_D4_PORT,
    DISPLAY_D5_PORT,
    DISPLAY_D6_PORT,
    DISPLAY_D7_PORT,
};

static const uint8_t displayPin[] = {
    DISPLAY_RSTB_PIN,
    DISPLAY_A0_PIN,
    DISPLAY_RW_PIN,
    DISPLAY_E_PIN,
    DISPLAY_D0_PIN,
    DISPLAY_D1_PIN,
    DISPLAY_D2_PIN,
    DISPLAY_D3_PIN,
    DISPLAY_D4_PIN,
    DISPLAY_D5_PIN,
    DISPLAY_D6_PIN,
    DISPLAY_D7_PIN,
};

void initDisplay(void)
{
    // GPIO
    gpio_set(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN);

    for (uint32_t i = 0; i < sizeof(displayPin); i++)
    {
#if defined(STM32F0)
        gpio_setup_output(displayPort[i], displayPin[i], GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
        gpio_setup(displayPort[i], displayPin[i], GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
#endif
    }

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
