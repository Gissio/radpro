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

    // Configure RCC
    RCC->CFGR =
        RCC_CFGR_SW_HSI |     // Select HSI as system clock
        RCC_CFGR_HPRE_DIV2 |  // Set AHB clock: 48 MHz / 2 = 24 MHz
        RCC_CFGR_PPRE1_DIV1 | // Set APB1 clock: 24 MHz / 1 = 24 MHz
        RCC_CFGR_PPRE2_DIV1;  // Set APB2 clock: 24 MHz / 1 = 24 MHz
    RCC->PLLCFGR =
        RCC_PLLCFGR_PLLSRC_HSI |              // Set PLL source: HSI16
        (0 << RCC_PLLCFGR_PLLM_Pos) |         // Set PLL predivision factor: /1
        (12 << RCC_PLLCFGR_PLLM_Pos) |        // Set PLL multiplier: 13x
        (1 << RCC_PLLCFGR_PLLR_Pos) |         // Set main PLL division factor for PLLCLK (system clock): /4
        RCC_PLLCFGR_PLLREN |                  // Enable main PLL PLLCLK output
        (7 << RCC_PLLCFGR_PLLPDIV_Pos);       // Set main PLL division factor for PLLSAI2CLK: /7
    RCC->CCIPR = (3 << RCC_CCIPR_ADCSEL_Pos); // Set system clock as ADC clock
 
    // Enable PLL
    set_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

    // Select PLL as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_PLL);

    // Enable RCC SYSCFG
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_SYSCFGEN);

    // Enable RCC GPIOA, GPIOB, GPIOC
    set_bits(RCC->AHB2ENR,
             RCC_AHB2ENR_GPIOAEN |
                 RCC_AHB2ENR_GPIOBEN |
                 RCC_AHB2ENR_GPIOCEN);
}

// Communications

const char *const commId = "Bosean FS-5000;" FIRMWARE_NAME " " FIRMWARE_VERSION;

// Keyboard

void initKeyboardController(void)
{
    // GPIO
    gpio_setup_input(KEY_LEFT_PORT,
                     KEY_LEFT_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_RIGHT_PORT,
                     KEY_RIGHT_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_SELECT_PORT,
                     KEY_SELECT_PIN,
                     GPIO_PULL_FLOATING);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);
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
    MR_SEND_DATA(0x1),
    MR_SEND_DATA(0x8),
    MR_SEND_DATA(0xf),
    MR_SEND_DATA(0x11),
    MR_SEND_DATA(0x2a),
    MR_SEND_DATA(0x36),
    MR_SEND_DATA(0x55),
    MR_SEND_DATA(0x44),
    MR_SEND_DATA(0x3a),
    MR_SEND_DATA(0xb),
    MR_SEND_DATA(0x6),
    MR_SEND_DATA(0x11),
    MR_SEND_DATA(0x20),
    MR_SEND_COMMAND(MR_ST7789_NVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x2),
    MR_SEND_DATA(0x7),
    MR_SEND_DATA(0xa),
    MR_SEND_DATA(0xb),
    MR_SEND_DATA(0x18),
    MR_SEND_DATA(0x34),
    MR_SEND_DATA(0x43),
    MR_SEND_DATA(0x4a),
    MR_SEND_DATA(0x2b),
    MR_SEND_DATA(0x1b),
    MR_SEND_DATA(0x1c),
    MR_SEND_DATA(0x22),
    MR_SEND_DATA(0x1f),

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
    DISPLAY_DATA_PORT->ODR = value;
    DISPLAY_WRX_PORT->BSRR = get_bitvalue(DISPLAY_WRX_PIN);
}

void initDisplayController(void)
{
    // GPIO
    gpio_set(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN);
    gpio_clear(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN);
    gpio_set(DISPLAY_RDX_PORT, DISPLAY_RDX_PIN);
    gpio_set(DISPLAY_WRX_PORT, DISPLAY_WRX_PIN);

    gpio_setup_output(DISPLAY_RESX_PORT,
                      DISPLAY_RESX_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_RDX_PORT,
                      DISPLAY_RDX_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_WRX_PORT,
                      DISPLAY_WRX_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_DCX_PORT,
                      DISPLAY_DCX_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);
    gpio_setup_output(DISPLAY_CSX_PORT,
                      DISPLAY_CSX_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_50MHZ,
                      GPIO_PULL_FLOATING);

    // DISPLAY_DATA_PORT->CRL =
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 0) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 4) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 8) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 12) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 16) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 20) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 24) |
    //     (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 28);

    // mcu-renderer
    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_270,
                   displayTextbuffer,
                   sizeof(displayTextbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend);

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
