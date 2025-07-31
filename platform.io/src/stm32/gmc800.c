/*
 * Rad Pro
 * GQ GMC-800 specifics
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(GMC800)

#include "../events.h"
#include "../keyboard.h"
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

    // Configure AHB, APB1, APB2, ADC, PLL
    RCC->CFGR = RCC_CFGR_SW_HSI |        // Select HSI as system clock
                RCC_CFGR_HPRE_DIV1 |     // Set AHB clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_PPRE1_DIV2 |    // Set APB1 clock: 72 MHz / 2 = 36 MHz
                RCC_CFGR_PPRE2_DIV1 |    // Set APB2 clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_ADCPRE_DIV6 |   // Set ADC clock: 72 MHz / 6 = 12 MHz
                RCC_CFGR_PLLSRC_HSE |    // Set PLL source: HSE
                RCC_CFGR_PLLXTPRE_HSE |  // Set PLL HSE predivision factor: /1
                RCC_CFGR_PLLMULL9 |      // Set PLL multiplier: 9x
                RCC_CFGR_USBPRE_DIV1_5 | // Set USB prescaler: 1.5x
                RCC_CFGR_MCO_NOCLOCK;    // Disable MCO

    // Enable HSE
    set_bits(RCC->CR,
             RCC_CR_HSEON);
    wait_until_bits_set(RCC->CR,
                        RCC_CR_HSERDY);

    // Enable PLL
    set_bits(RCC->CR,
             RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR,
                        RCC_CR_PLLRDY);

    // Set PLL as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_PLL);

    // Disable JTAG
    rcc_enable_afio();
    modify_bits(AFIO->MAPR,
                AFIO_MAPR_SWJ_CFG_Msk,
                AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    // Enable GPIOA, GPIOB, GPIOC, GPIOD
    set_bits(RCC->APB2ENR,
             RCC_APB2ENR_IOPAEN |
                 RCC_APB2ENR_IOPBEN |
                 RCC_APB2ENR_IOPCEN |
                 RCC_APB2ENR_IOPDEN);

    // Disable USART reset
    gpio_clear(USART_RESET_EN_PORT,
               USART_RESET_EN_PIN);
    gpio_setup(USART_RESET_EN_PORT,
               USART_RESET_EN_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
}

void startBootloader(void)
{
    // Disable interrupts
    NVIC_DisableAllIRQs();
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->VAL = 0;

    // Set HSI as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_HSI);

    // Disable PLL
    clear_bits(RCC->CR,
               RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR,
                          RCC_CR_PLLRDY);

    // Reset RCC
    RCC->CFGR = 0;

    // Set 0 wait states for flash
    modify_bits(FLASH->ACR,
                FLASH_ACR_LATENCY_Msk,
                FLASH_ACR_LATENCY_0WS);

    // Disable USART reset
    gpio_setup(USART_RESET_EN_PORT,
               USART_RESET_EN_PIN,
               GPIO_MODE_INPUT_FLOATING);

    // Jump to bootloader
    __set_MSP(SYSTEM_VECTOR_TABLE->sp);
    SYSTEM_VECTOR_TABLE->onReset();
}

// Communications

const char *const commId = "GQ GMC-800;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO
    gpio_setup(KEY_LEFT_PORT,
               KEY_LEFT_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_UP_PORT,
               KEY_UP_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_DOWN_PORT,
               KEY_DOWN_PIN,
               GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_RIGHT_PORT,
               KEY_RIGHT_PIN,
               GPIO_MODE_INPUT_PULLUP);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
}

// Display

extern mr_t mr;

bool displayEnabled;

static uint8_t displayTextbuffer[88 * 88];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_INVON),
    MR_SEND_COMMAND(MR_ST7789_GCTRL),
    MR_SEND_DATA(0x74), // // VGH 14.97 V, VGL -9.6 V
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x1f), // VCOM=0.875 V
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4), // AVDD=6.8 V, AVCL=-4.8 V, VDDS=2.3 V
    MR_SEND_DATA(0xa1),
    MR_SEND_COMMAND(MR_ST7789_PVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x01),
    MR_SEND_DATA(0x08),
    MR_SEND_DATA(0x0f),
    MR_SEND_DATA(0x11),
    MR_SEND_DATA(0x2a),
    MR_SEND_DATA(0x36),
    MR_SEND_DATA(0x55),
    MR_SEND_DATA(0x44),
    MR_SEND_DATA(0x3a),
    MR_SEND_DATA(0x0b),
    MR_SEND_DATA(0x06),
    MR_SEND_DATA(0x11),
    MR_SEND_DATA(0x20),
    MR_SEND_COMMAND(MR_ST7789_NVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x02),
    MR_SEND_DATA(0x07),
    MR_SEND_DATA(0x0a),
    MR_SEND_DATA(0x0b),
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

static void onDisplaySetChipselect(bool value)
{
    spi_wait_while_bsy(SPI1);

    gpio_modify(DISPLAY_CSX_PORT,
                DISPLAY_CSX_PIN,
                !value);
}

static void onDisplaySetCommand(bool value)
{
    spi_wait_while_bsy(SPI1);

    gpio_modify(DISPLAY_DCX_PORT,
                DISPLAY_DCX_PIN,
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

void initDisplay(void)
{
    // GPIO
    gpio_set(DISPLAY_RESX_PORT,
             DISPLAY_RESX_PIN);
    gpio_set(DISPLAY_CSX_PORT,
             DISPLAY_CSX_PIN);

    gpio_setup(DISPLAY_RESX_PORT,
               DISPLAY_RESX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_CSX_PORT,
               DISPLAY_CSX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_DCX_PORT,
               DISPLAY_DCX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_SCL_PORT,
               DISPLAY_SCL_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
    gpio_setup(DISPLAY_SDA_PORT,
               DISPLAY_SDA_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);

    set_bits(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);

    // SPI
    SPI1->CR1 = SPI_CR1_CPHA |
                SPI_CR1_CPOL |
                SPI_CR1_MSTR |
                SPI_CR1_SSI |
                SPI_CR1_SSM;
    set_bits(SPI1->CR1, SPI_CR1_SPE);

    // mcu-renderer
    mr_st7789_init(&mr,
                   240,
                   320,
#if defined(DISPLAY_240X320)
                   MR_DISPLAY_ROTATION_0,
#else
                   MR_DISPLAY_ROTATION_270,
#endif
                   displayTextbuffer,
                   sizeof(displayTextbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetChipselect,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend16);

    mr_send_sequence(&mr,
                     displayInitSequence);
}

void enableDisplay(bool value)
{
    mr_st7789_set_display(&mr, value);
    mr_st7789_set_sleep(&mr, !value);

    displayEnabled = value;
}

bool isDisplayEnabled(void)
{
    return displayEnabled;
}

void refreshDisplay(void)
{
}

#endif
