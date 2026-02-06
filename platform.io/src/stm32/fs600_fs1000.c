/*
 * Rad Pro
 * Bosean FS-600/FS-1000 driver
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(FS600) || defined(FS1000)

#include "mcu-renderer-st7565.h"

#include "../devices/display.h"
#include "../devices/keyboard.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../system/system.h"

// System

void initSystem(void)
{
    // Set SYSCLK to HSI16/2
    modify_bits(RCC->CR, RCC_CR_HSIDIV_Msk, RCC_CR_HSIDIV_DIV2);

    // Set system clock
    setFastSystemClock(false);

    // Enable SYSCFG (for EXTI, UPCD)
    set_bits(RCC->APBENR2, RCC_APBENR2_SYSCFGEN);

    // Disable UCPD strobes
    set_bits(SYSCFG->CFGR1, SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);

    // Enable GPIOA, GPIOB, GPIOC, GPIOD
    set_bits(RCC->IOPENR, RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN | RCC_IOPENR_GPIODEN);
}

void setFastSystemClock(bool value)
{
    // Set HSI as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSISYS);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    if (value)
    {
        // Set 2 wait states for flash
        modify_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk, FLASH_ACR_LATENCY_2WS);

        // Configure SW, AHB, APB
        RCC->CFGR = RCC_CFGR_SW_HSI |    // Select HSI as system clock
                    RCC_CFGR_HPRE_DIV8 | // Set AHB clock: 64 MHz / 8 = 8 MHz
                    RCC_CFGR_PPRE_DIV2;  // Set APB clock: 64 MHz / 2 = 4 MHz

        // Configure PLL
        RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI |          // Set PLL source: HSI16
                       (0b000 << RCC_PLLCFGR_PLLM_Pos) | // Set PLL division factor: /1
                       (8 << RCC_PLLCFGR_PLLN_Pos) |     // Set main PLL VCO multiplication factor: 8x
                       RCC_PLLCFGR_PLLREN |              // Enable main PLL PLLCLK output
                       (0b001 << RCC_PLLCFGR_PLLR_Pos);  // Set main PLL PLLCLK division factor: /2

        // Enable PLL
        set_bits(RCC->CR, RCC_CR_PLLON);
        wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

        // Set PLL as system clock
        modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
        wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLLRCLK);

        // Update sys tick
        SysTick->LOAD = SysTick->LOAD << 3;
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
        SysTick->LOAD = SysTick->LOAD >> 3;
    }
}

// Bootloader

void startBootloader(void)
{
    // Disable interrupts
    NVIC_DisableAllIRQs();
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->VAL = 0;

    // Set SYSCLK to HSI16/1
    modify_bits(RCC->CR, RCC_CR_HSIDIV_Msk, RCC_CR_HSIDIV_DIV1);

    // Jump to bootloader
    __set_MSP(SYSTEM_VECTOR_TABLE->sp);
    SYSTEM_VECTOR_TABLE->onReset();
}

// Communications

#if defined(FS600)
const char *const commId = "Bosean FS-600;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#elif defined(FS1000)
const char *const commId = "Bosean FS-1000;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#endif

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO
    gpio_setup_input(KEY_LEFT_PORT, KEY_LEFT_PIN, GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_RIGHT_PORT, KEY_RIGHT_PIN, GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_UP_PORT, KEY_UP_PIN, GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_DOWN_PORT, KEY_DOWN_PIN, GPIO_PULL_PULLDOWN);
    gpio_setup_input(KEY_OK_PORT, KEY_OK_PIN, GPIO_PULL_FLOATING);
}

void getKeyboardState(bool *isKeyDown)
{
#if defined(KEYBOARD_2_KEYS)
    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
#elif defined(KEYBOARD_5_KEYS)
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
    GPIOB->BRR = get_bitvalue(DISPLAY_CSB_PIN);

    uint32_t data = 0x1ff << (16 + 11 - 7) |
                    (value << (12 - 7));

    const uint32_t mask = ((get_bitvalue(DISPLAY_SCLK_PIN) |
                            get_bitvalue(DISPLAY_SDA_PIN))
                           << 16) |
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
    gpio_set(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN);
    gpio_set(DISPLAY_CSB_PORT, DISPLAY_CSB_PIN);
    gpio_set(DISPLAY_SCLK_PORT, DISPLAY_SCLK_PIN);

    gpio_setup_output(DISPLAY_RSTB_PORT, DISPLAY_RSTB_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_CSB_PORT, DISPLAY_CSB_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_A0_PORT, DISPLAY_A0_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_SCLK_PORT, DISPLAY_SCLK_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_SDA_PORT, DISPLAY_SDA_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING);

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
