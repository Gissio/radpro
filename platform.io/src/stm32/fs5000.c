/*
 * Rad Pro
 * Bosean FS-5000 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(FS5000)

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
    // Set 2 wait states for flash
    modify_bits(FLASH->ACR,
                FLASH_ACR_LATENCY_Msk,
                FLASH_ACR_LATENCY_2WS);

    // Enable HSI16
    set_bits(RCC->CR,
             RCC_CR_HSION);
    wait_until_bits_set(RCC->CR,
                        RCC_CR_HSIRDY);

    // Configure RCC
    modify_bits(RCC->CFGR,
                RCC_CFGR_HPRE_Msk |
                    RCC_CFGR_PPRE1_Msk |
                    RCC_CFGR_PPRE2_Msk,
                RCC_CFGR_HPRE_DIV2 |      // Set AHB clock: 48 MHz / 2 = 24 MHz
                    RCC_CFGR_PPRE1_DIV1 | // Set APB1 clock: 24 MHz / 1 = 24 MHz
                    RCC_CFGR_PPRE2_DIV1   // Set APB2 clock: 24 MHz / 1 = 24 MHz
    );
    RCC->PLLCFGR =
        (7 << RCC_PLLCFGR_PLLPDIV_Pos) |      // Set main PLL PLLSAI2CLK division factor: /7
        (1 << RCC_PLLCFGR_PLLR_Pos) |         // Set main PLL PLLCLK division factor: /4
        RCC_PLLCFGR_PLLREN |                  // Enable main PLL PLLCLK output
        (12 << RCC_PLLCFGR_PLLN_Pos) |        // Set main PLL VCO multiplication factor: 12x
        (0 << RCC_PLLCFGR_PLLM_Pos) |         // Set PLL division factor: /1
        RCC_PLLCFGR_PLLSRC_HSI;               // Set PLL source: HSI16
    RCC->CCIPR = (3 << RCC_CCIPR_ADCSEL_Pos); // Set system clock as ADC clock

    // Enable PLL
    set_bits(RCC->CR,
             RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR,
                        RCC_CR_PLLRDY);

    // Select PLL as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_PLL);

    // Enable RCC SYSCFG
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_SYSCFGEN);

    // Enable RCC GPIOA, GPIOB, GPIOC, GPIOD
    set_bits(RCC->AHB2ENR,
             RCC_AHB2ENR_ADCEN |
                 RCC_AHB2ENR_GPIOAEN |
                 RCC_AHB2ENR_GPIOBEN |
                 RCC_AHB2ENR_GPIOCEN |
                 RCC_AHB2ENR_GPIODEN);
}

// Communications

const char *const commId = "Bosean FS-5000;" FIRMWARE_NAME " " FIRMWARE_VERSION;

// Keyboard

void initKeyboardController(void)
{
    // GPIO
    gpio_setup_input(KEY_LEFT_PORT,
                     KEY_LEFT_PIN,
                     GPIO_PULL_UP);
    gpio_setup_input(KEY_OK_PORT,
                     KEY_OK_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_RIGHT_PORT,
                     KEY_RIGHT_PIN,
                     GPIO_PULL_UP);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_OK] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
}

// Display

extern mr_t mr;

bool displayOn;

static uint8_t displayTextbuffer[86 * 86];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x28), // Set: 1.1 V (default 0.9 V)
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4), // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
    MR_SEND_DATA(0xa1),
    MR_SEND_COMMAND(MR_ST7789_RAMCTRL),
    MR_SEND_DATA(0x00),
    MR_SEND_DATA(0xe0),
    MR_SEND_COMMAND(MR_ST7789_PVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x04),
    MR_SEND_DATA(0x10),
    MR_SEND_DATA(0x13),
    MR_SEND_DATA(0x14),
    MR_SEND_DATA(0x2c),
    MR_SEND_DATA(0x41),
    MR_SEND_DATA(0x54),
    MR_SEND_DATA(0x4e),
    MR_SEND_DATA(0x18),
    MR_SEND_DATA(0x0c),
    MR_SEND_DATA(0x0a),
    MR_SEND_DATA(0x21),
    MR_SEND_DATA(0x25),
    MR_SEND_COMMAND(MR_ST7789_NVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x04),
    MR_SEND_DATA(0x0f),
    MR_SEND_DATA(0x13),
    MR_SEND_DATA(0x15),
    MR_SEND_DATA(0x2c),
    MR_SEND_DATA(0x40),
    MR_SEND_DATA(0x44),
    MR_SEND_DATA(0x52),
    MR_SEND_DATA(0x2d),
    MR_SEND_DATA(0x1d),
    MR_SEND_DATA(0x1e),
    MR_SEND_DATA(0x21),
    MR_SEND_DATA(0x24),

    MR_END(),
};

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

static void onDisplaySetReset(bool value)
{
    gpio_modify(DISPLAY_RESX_PORT,
                DISPLAY_RESX_PIN,
                !value);
}

static void onDisplaySetCommand(bool value)
{
    gpio_modify(DISPLAY_DCX_PORT,
                DISPLAY_DCX_PIN,
                !value);
}

static void onDisplaySend(uint16_t value)
{
    DISPLAY_WRX_PORT->BRR = get_bitvalue(DISPLAY_WRX_PIN);
    *((__IO uint8_t *)&DISPLAY_D0_PORT->ODR) = value;
    DISPLAY_WRX_PORT->BSRR = get_bitvalue(DISPLAY_WRX_PIN);
}

static void onDisplaySend16(uint16_t value)
{
    DISPLAY_WRX_PORT->BRR = get_bitvalue(DISPLAY_WRX_PIN);
    *((__IO uint8_t *)&DISPLAY_D0_PORT->ODR) = (value >> 8) & 0xff;
    DISPLAY_WRX_PORT->BSRR = get_bitvalue(DISPLAY_WRX_PIN);
    DISPLAY_WRX_PORT->BRR = get_bitvalue(DISPLAY_WRX_PIN);
    *((__IO uint8_t *)&DISPLAY_D0_PORT->ODR) = (value >> 0) & 0xff;
    DISPLAY_WRX_PORT->BSRR = get_bitvalue(DISPLAY_WRX_PIN);
}

static GPIO_TypeDef *const displayPortSetup[] = {
    DISPLAY_RESX_PORT,
    DISPLAY_CSX_PORT,
    DISPLAY_DCX_PORT,
    DISPLAY_WRX_PORT,
    DISPLAY_RDX_PORT,
    DISPLAY_D0_PORT,
    DISPLAY_D1_PORT,
    DISPLAY_D2_PORT,
    DISPLAY_D3_PORT,
    DISPLAY_D4_PORT,
    DISPLAY_D5_PORT,
    DISPLAY_D6_PORT,
    DISPLAY_D7_PORT,
};

static const uint8_t displayPinSetup[] = {
    DISPLAY_RESX_PIN,
    DISPLAY_CSX_PIN,
    DISPLAY_DCX_PIN,
    DISPLAY_WRX_PIN,
    DISPLAY_RDX_PIN,
    DISPLAY_D0_PIN,
    DISPLAY_D1_PIN,
    DISPLAY_D2_PIN,
    DISPLAY_D3_PIN,
    DISPLAY_D4_PIN,
    DISPLAY_D5_PIN,
    DISPLAY_D6_PIN,
    DISPLAY_D7_PIN,
};

void initDisplayController(void)
{
    // GPIO
    gpio_set(DISPLAY_POWER_PORT,
             DISPLAY_POWER_PIN);

    gpio_setup_output(DISPLAY_POWER_PORT,
                      DISPLAY_POWER_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);

    gpio_set(DISPLAY_RESX_PORT,
             DISPLAY_RESX_PIN);
    gpio_set(DISPLAY_RDX_PORT,
             DISPLAY_RDX_PIN);
    gpio_set(DISPLAY_WRX_PORT,
             DISPLAY_WRX_PIN);

    for (int i = 0; i < sizeof(displayPinSetup); i++)
        gpio_setup_output(displayPortSetup[i],
                          displayPinSetup[i],
                          GPIO_OUTPUTTYPE_PUSHPULL,
                          GPIO_OUTPUTSPEED_50MHZ,
                          GPIO_PULL_FLOATING);

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
                   onDisplaySend16);

    mr_send_sequence(&mr,
                     displayInitSequence);
}

void setDisplayOn(bool value)
{
    displayOn = value;

    mr_st7789_set_display(&mr, value);
    mr_st7789_set_sleep(&mr, !value);
}

bool isDisplayOn(void)
{
    return displayOn;
}

void refreshDisplay(void)
{
}

#endif
