/*
 * Rad Pro
 * FNIRSI GC-01 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(GC01)

#include <libopencm3/cm3/scb.h>

#include <libopencm3/stm32/flash.h>
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

#include "mcu-renderer-st7789.h"

// +++ TEST
// #include "libopencm3/stm32/spi.h"

// #define DISPLAY_SCLK_PORT GPIOA
// #define DISPLAY_SCLK_PIN GPIO5
// #define DISPLAY_SDA_PORT GPIOA
// #define DISPLAY_SDA_PIN GPIO7
// #define DISPLAY_RESET_PORT GPIOB
// #define DISPLAY_RESET_PIN GPIO0
// #define DISPLAY_CS_PORT GPIOB
// #define DISPLAY_CS_PIN GPIO1
// #define DISPLAY_RS_PORT GPIOB
// #define DISPLAY_RS_PIN GPIO10
// +++ TEST

// System

void initSystem(void)
{
    // Clocks

    RCC_CR |= RCC_CR_HSEON; // Enable HSE clock
    while (!(RCC_CR & RCC_CR_HSERDY))
        ;

    RCC_CFGR =                                                    // Configure RCC
        (RCC_CFGR_SW_SYSCLKSEL_HSICLK << RCC_CFGR_SW_SHIFT) |     // Select HSI as system clock
        (RCC_CFGR_HPRE_SYSCLK_NODIV << RCC_CFGR_HPRE_SHIFT) |     // Set AHB clock: 72 / 1 = 72 MHz
        (RCC_CFGR_PPRE1_HCLK_DIV2 << RCC_CFGR_PPRE1_SHIFT) |      // Set APB1 clock: 72 / 2 = 36 MHz
        (RCC_CFGR_PPRE2_HCLK_NODIV << RCC_CFGR_PPRE2_SHIFT) |     // Set APB2 clock: 72 / 1 = 72 MHz
        (RCC_CFGR_PLLMUL_PLL_CLK_MUL9 << RCC_CFGR_PLLMUL_SHIFT) | // Set PLL multiplier
        (RCC_CFGR_PLLSRC_HSE_CLK << 16) |                         // Set PLL source
        (RCC_CFGR_PLLXTPRE_HSE_CLK << 17) |                       // Set PLL predivision factor
        (RCC_CFGR_ADCPRE_PCLK2_DIV8 << RCC_CFGR_ADCPRE_SHIFT) |   // Set ADC clock: 72 / 8 = 9 MHz
        RCC_CFGR_USBPRE_PLL_CLK_DIV1_5 |                          // Set USB prescaler
        RCC_CFGR_MCO_NOCLK;                                       // Disable microcontroller clock output

    FLASH_ACR &= ~(FLASH_ACR_LATENCY_MASK << FLASH_ACR_LATENCY_SHIFT); // Set flash latency
    FLASH_ACR |= (FLASH_ACR_LATENCY_2WS << FLASH_ACR_LATENCY_SHIFT);

    RCC_CR |= RCC_CR_PLLON; // Enable PLL clock
    while (!(RCC_CR & RCC_CR_PLLRDY))
        ;

    RCC_CFGR |= (RCC_CFGR_SW_SYSCLKSEL_PLLCLK << RCC_CFGR_SW_SHIFT); // Select PLL as system clock

    // SCB

    SCB_VTOR = PAYLOAD_BASE;

    // GPIO

    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
                       0);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
}

// Flash memory

const FlashRegion flashSettingsRegion = {0x39, 0x3a};
const FlashRegion flashDatalogRegion = {0x3a, 0x40};

// Communications

const char *const commId = "FNIRSI GC-01;" FIRMWARE_NAME " " FIRMWARE_VERSION;

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO

    gpio_set(KEY_SELECT_PORT,
             KEY_SELECT_PIN); // Pull-up
    gpio_set_mode(KEY_SELECT_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_SELECT_PIN);

    gpio_set(GPIOC,
             KEY_UP_PIN | KEY_RIGHT_PIN |
                 KEY_DOWN_PIN | KEY_LEFT_PIN); // Pull-up
    gpio_set_mode(GPIOC,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_UP_PIN | KEY_RIGHT_PIN |
                      KEY_DOWN_PIN | KEY_LEFT_PIN);
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

static uint8_t textbuffer[2 * 36 * 50];

static const uint8_t gc01_st7789_init_sequence[] = {
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x28), // Set: 1.1 V (default 0.9 V)
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4),
    MR_SEND_DATA(0xa1), // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
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
#if defined(DISPLAY_SCLK_PORT)
    MR_SEND_COMMAND(MR_ST7789_INVON), // Inverse for IPS displays
#endif
    // +++ TEST

    MR_END(),
};

void onDisplaySleep(uint32_t value);
void onDisplaySetReset(bool value);
void onDisplaySetCommand(bool value);
void onDisplaySend(uint16_t value);
void onDisplaySend16(uint16_t value);

void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

void onDisplaySetReset(bool value)
{
    if (value)
        gpio_clear(LCD_RESX_PORT,
                   LCD_RESX_PIN);
    else
        gpio_set(LCD_RESX_PORT,
                 LCD_RESX_PIN);

// +++ TEST
#if defined(DISPLAY_SCLK_PORT)
    if (value)
        gpio_clear(DISPLAY_RESET_PORT,
                   DISPLAY_RESET_PIN);
    else
        gpio_set(DISPLAY_RESET_PORT,
                 DISPLAY_RESET_PIN);
#endif
    // +++ TEST
}

void onDisplaySetCommand(bool value)
{
    if (value)
        gpio_clear(LCD_DCX_PORT,
                   LCD_DCX_PIN);
    else
        gpio_set(LCD_DCX_PORT,
                 LCD_DCX_PIN);

// +++ TEST
#if defined(DISPLAY_SCLK_PORT)

    if (value)
    {
        // Trigger CS before command
        gpio_set(DISPLAY_CS_PORT, DISPLAY_CS_PIN);
        gpio_clear(DISPLAY_CS_PORT, DISPLAY_CS_PIN);

        gpio_clear(DISPLAY_RS_PORT,
                   DISPLAY_RS_PIN);
    }
    else
        gpio_set(DISPLAY_RS_PORT,
                 DISPLAY_RS_PIN);

#endif
    // +++ TEST
}

void onDisplaySend(uint16_t value)
{
    // +++ TEST
#if defined(DISPLAY_SCLK_PORT)
    spi_send(SPI1, value);
#else
    // +++ TEST
    GPIOB_BRR = LCD_WRX_PIN;
    GPIOB_ODR = value;
    GPIOB_BSRR = LCD_WRX_PIN;
    // +++ TEST
#endif
    // +++ TEST
}

// +++ TEST
#if defined(DISPLAY_SCLK_PORT)

void onDisplaySend16(uint16_t value)
{
    spi_send(SPI1, (value >> 8) & 0xff);
    spi_send(SPI1, (value >> 0) & 0xff);
}

#endif
// +++ TEST

void initDisplayHardware(void)
{
    // GPIO

    gpio_set_mode(LCD_BACKLIGHT_PORT,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  LCD_BACKLIGHT_PIN);

// +++ TEST
#if !defined(DISPLAY_SCLK_PORT)

    gpio_set_mode(LCD_DATA_PORT,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_DATA_PINS);

#else

    gpio_set_mode(GPIOB,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  DISPLAY_RESET_PIN | DISPLAY_CS_PIN |
                      DISPLAY_RS_PIN);

    rcc_periph_clock_enable(RCC_SPI1);

    gpio_set_mode(GPIOA,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  DISPLAY_SCLK_PIN | DISPLAY_SDA_PIN);
    // spi_init_master(SPI1,
    //                 SPI_CR1_BAUDRATE_FPCLK_DIV_2,
    //                 SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
    //                 SPI_CR1_CPHA_CLK_TRANSITION_2,
    //                 SPI_CR1_DFF_8BIT,
    //                 SPI_CR1_MSBFIRST);
    // spi_enable_software_slave_management(SPI1);
    // spi_set_nss_high(SPI1);
    // spi_set_baudrate_prescaler(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_2);

    SPI1_CR1 = SPI_CR1_CPHA_CLK_TRANSITION_2 |
               SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE |
               SPI_CR1_MSTR |
               SPI_CR1_SSI |
               SPI_CR1_SSM;
    spi_enable(SPI1);

#endif
    // +++ TEST

    gpio_set(GPIOC,
             LCD_RESX_PIN | LCD_RDX_PIN |
                 LCD_WRX_PIN);
    gpio_set_mode(GPIOC,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  LCD_RESX_PIN | LCD_RDX_PIN |
                      LCD_WRX_PIN | LCD_DCX_PIN |
                      LCD_CSX_PIN);

    // Backlight timer

    // Already set by buzzer:
    // rcc_periph_clock_enable(LCD_BACKLIGHT_TIMER_RCC);

    LCD_BACKLIGHT_TIMER_CCMR(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCMR_MODE; // timer_set_oc_mode(LCD_BACKLIGHT_TIMER, TIM_OC1, TIM_OCM_PWM1);
    // Already set by buzzer:
    // TIM_ARR(LCD_BACKLIGHT_TIMER) = BACKLIGHT_PWM_PERIOD; // timer_set_period(LCD_BACKLIGHT_TIMER, BACKLIGHT_PWM_PERIOD);

    setDisplayBacklight(false);

    TIM_CCER(LCD_BACKLIGHT_TIMER) |= LCD_BACKLIGHT_TIMER_CCER_CC; // timer_enable_oc_output(LCD_BACKLIGHT_TIMER, TIM_OC1);
    // Already set by buzzer:
    // TIM_CR1(LCD_BACKLIGHT_TIMER) |= TIM_CR1_CEN; // timer_enable_counter(LCD_BACKLIGHT_TIMER);

    // mcu-renderer

#if defined(DISPLAY_SCLK_PORT)
    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_90,
                   textbuffer,
                   sizeof(textbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend16);
#else
    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_90,
                   textbuffer,
                   sizeof(textbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend);
#endif

    mr_send_sequence(&mr, gc01_st7789_init_sequence);
}

#endif
