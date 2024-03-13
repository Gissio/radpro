/*
 * Rad Pro
 * FNIRSI GC-01 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(GC01)

#include "stm32.h"

#include "../display.h"
#include "../events.h"
#include "../flash.h"
#include "../keyboard.h"
#include "../settings.h"
#include "../system.h"

#include "device.h"

#include "mcu-renderer-st7789.h"

// +++ TEST
// #define DISPLAY_SERIAL
// +++ TEST

// System

void initSystem(void)
{
    // Set stack pointer to fix bootloader madness
    __set_MSP(*((uint32_t *)FIRMWARE_BASE));

    // Enable HSE
    set_bits(RCC->CR, RCC_CR_HSEON);
    wait_until_bits_set(RCC->CR, RCC_CR_HSERDY);

    // Set 2 wait states for flash
    modify_bits(FLASH->ACR,
                FLASH_ACR_LATENCY_Msk,
                FLASH_ACR_LATENCY_2WS);

    // Configure RCC
    RCC->CFGR =
        RCC_CFGR_SW_HSI |        // Select HSI as system clock
        RCC_CFGR_HPRE_DIV1 |     // Set AHB clock: 72 MHz / 1 = 72 MHz
        RCC_CFGR_PPRE1_DIV2 |    // Set APB1 clock: 72 MHz / 2 = 36 MHz
        RCC_CFGR_PPRE2_DIV1 |    // Set APB2 clock: 72 MHz / 1 = 72 MHz
        RCC_CFGR_ADCPRE_DIV8 |   // Set ADC clock: 72 MHz / 8 = 9 MHz
        RCC_CFGR_PLLSRC_HSE |    // Set PLL source: HSE
        RCC_CFGR_PLLXTPRE_HSE |  // Set PLL HSE predivision factor: 1x
        RCC_CFGR_PLLMULL9 |      // Set PLL multiplier: 9x
        RCC_CFGR_USBPRE_DIV1_5 | // Set USB prescaler: 1.5x
        RCC_CFGR_MCO_NOCLOCK;    // Disable MCO

    // Enable PLL
    set_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

    // Select PLL as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

    // Set vector table
    NVIC_DisableAllIRQs();
    SCB->VTOR = FIRMWARE_BASE;

    // Disable JTAG
    rcc_enable_afio();
    modify_bits(AFIO->MAPR,
                AFIO_MAPR_SWJ_CFG_Msk,
                AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    // Enable RCC GPIOA, GPIOB, GPIOC
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_IOPAEN |
                 RCC_APB2ENR_IOPBEN |
                 RCC_APB2ENR_IOPCEN);
}

// Flash memory

#if defined(CH32)
const FlashRegion flashSettingsRegion = {0x3a, 0x3b};
const FlashRegion flashDatalogRegion = {0x3b, 0x40};
#elif defined(APM32)
const FlashRegion flashSettingsRegion = {0x40, 0x41};
const FlashRegion flashDatalogRegion = {0x41, 0x80};
#endif

// Communications

#if defined(CH32)
const char *const commId = "FNIRSI GC-01 (CH32F103C8);" FIRMWARE_NAME " " FIRMWARE_VERSION;
#elif defined(APM32)
const char *const commId = "FNIRSI GC-01 (APM32F103CB);" FIRMWARE_NAME " " FIRMWARE_VERSION;
#endif

// Keyboard

void initKeyboardController(void)
{
    gpio_setup(KEY_LEFT_PORT,
               KEY_LEFT_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_RIGHT_PORT,
               KEY_RIGHT_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_UP_PORT,
               KEY_UP_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_DOWN_PORT,
               KEY_DOWN_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_SELECT_PORT,
               KEY_SELECT_PIN,
               GPIO_MODE_INPUT_PULLUP);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);
}

// Display

extern mr_t mr;

static uint8_t displayTextbuffer[2 * 36 * 50];

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

// +++ TEST
#if defined(DISPLAY_SERIAL)
    MR_SEND_COMMAND(MR_ST7789_INVON), // Inverse for IPS displays
#endif
    // +++ TEST

    MR_END(),
};

// +++ TEST
#if defined(DISPLAY_SERIAL)

#define DISPLAY_SCLK_PORT GPIOA
#define DISPLAY_SCLK_PIN 5
#define DISPLAY_SDA_PORT GPIOA
#define DISPLAY_SDA_PIN 7
#define DISPLAY_RESET_PORT GPIOB
#define DISPLAY_RESET_PIN 0
#define DISPLAY_CS_PORT GPIOB
#define DISPLAY_CS_PIN 1
#define DISPLAY_RS_PORT GPIOB
#define DISPLAY_RS_PIN 10

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

static void onDisplaySetReset(bool value)
{
    gpio_modify(DISPLAY_RESET_PORT,
                DISPLAY_RESET_PIN,
                !value);
}

static void onDisplaySetCommand(bool value)
{
    if (value)
    {
        // Trigger CS before command
        gpio_set(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
        gpio_clear(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
    }

    gpio_modify(DISPLAY_RS_PORT,
                DISPLAY_RS_PIN,
                !value);
}

static void onDisplaySend(uint16_t value)
{
    spi_send(SPI1, value);
}

static void onDisplaySend16(uint16_t value)
{
    spi_send(SPI1, (value >> 8) & 0xff);
    spi_send(SPI1, (value >> 0) & 0xff);
}

#else

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

#endif

void initDisplayController(void)
{
// +++ TEST
#if defined(DISPLAY_SERIAL)

    gpio_set(DISPLAY_RESET_PORT, DISPLAY_RESET_PIN);
    gpio_set(DISPLAY_CS_PORT, DISPLAY_CS_PIN);

    gpio_setup(DISPLAY_RESET_PORT,
               DISPLAY_RESET_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_CS_PORT,
               DISPLAY_CS_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_RS_PORT,
               DISPLAY_RS_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_SCLK_PORT,
               DISPLAY_SCLK_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
    gpio_setup(DISPLAY_SDA_PORT,
               DISPLAY_SDA_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);

    set_bits(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);

    // Configure SPI
    SPI1->CR1 = SPI_CR1_CPHA |
                SPI_CR1_CPOL |
                SPI_CR1_MSTR |
                SPI_CR1_SSI |
                SPI_CR1_SSM;

    // Enable SPI
    set_bits(SPI1->CR1,
             SPI_CR1_SPE);

#else

    gpio_set(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN);
    gpio_set(DISPLAY_RDX_PORT, DISPLAY_RDX_PIN);
    gpio_set(DISPLAY_WRX_PORT, DISPLAY_WRX_PIN);
    gpio_clear(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN);

    gpio_setup(DISPLAY_RESX_PORT,
               DISPLAY_RESX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_RDX_PORT,
               DISPLAY_RDX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_WRX_PORT,
               DISPLAY_WRX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_DCX_PORT,
               DISPLAY_DCX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_CSX_PORT,
               DISPLAY_CSX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);

    DISPLAY_DATA_PORT->CRL =
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 0) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 4) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 8) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 12) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 16) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 20) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 24) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 28);
    DISPLAY_DATA_PORT->CRH =
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 0) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 4) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 8) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 12) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 16) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 20) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 24) |
        (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 28);

#endif
    // +++ TEST

    // mcu-renderer

#if defined(DISPLAY_SERIAL)

    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_90,
                   displayTextbuffer,
                   sizeof(displayTextbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend16);

#else

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

#endif

    mr_send_sequence(&mr,
                     displayInitSequence);
}

#endif
