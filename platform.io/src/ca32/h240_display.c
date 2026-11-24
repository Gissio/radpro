/*
 * Rad Pro H240 display
 *
 * License: MIT
 */

#if defined(CA32) && defined(H240)

#include <mcu-renderer-st7789.h>
#include <stddef.h>

#include "../peripherals/display.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "ca32_gpio.h"
#include "h240_display.h"

// CA32 datasheet sections 3 and 4, and factory setup at 0x0800c618.
#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018)
#define SPI1_CR1 (*(volatile uint16_t *)0x40013000)
#define SPI1_CR2 (*(volatile uint16_t *)0x40013004)
#define SPI1_SR (*(volatile uint16_t *)0x40013008)
#define SPI1_DR (*(volatile uint16_t *)0x4001300c)
#define SPI_CR1_MASTER (1U << 2)
#define SPI_CR1_DIV4 (1U << 3)
#define SPI_CR1_DIV16 (3U << 3)
#define SPI_CR1_ENABLE (1U << 6)
#define SPI_CR1_SOFTWARE_NSS (3U << 8)
#define SPI_CR1_BIDIRECTIONAL_TX (3U << 14)
#define SPI_SR_TX_EMPTY (1U << 1)
#define SPI_SR_BUSY (1U << 7)
#define RCC_SPI1_ENABLE (1U << 12)
#define RCC_TIM1_ENABLE (1U << 11)

// Factory PWM setup at 0x0800954c routes TIM1 CH2 to PC12.
#define TIM1_CR1 (*(volatile uint16_t *)0x40012c00)
#define TIM1_EGR (*(volatile uint16_t *)0x40012c14)
#define TIM1_CCMR1 (*(volatile uint16_t *)0x40012c18)
#define TIM1_CCER (*(volatile uint16_t *)0x40012c20)
#define TIM1_PSC (*(volatile uint16_t *)0x40012c28)
#define TIM1_ARR (*(volatile uint32_t *)0x40012c2c)
#define TIM1_CCR2 (*(volatile uint32_t *)0x40012c38)
#define TIM1_BDTR (*(volatile uint16_t *)0x40012c44)
#define TIM1_CH2_PWM1_PRELOAD ((6U << 12) | (1U << 11))
#define TIM1_CH2_ENABLE (1U << 4)
#define TIM1_MAIN_OUTPUT_ENABLE (1U << 15)
#define TIM1_GENERATE_UPDATE 1U
#define TIM1_ENABLE 1U
#define BACKLIGHT_PERIOD (CA32_CORE_CLOCK_HZ / 2700U)

enum
{
    DISPLAY_MOSI = 6,
    DISPLAY_SCK = 8,
    DISPLAY_CS = 9,
    DISPLAY_RESET = 10,
    DISPLAY_DC = 11,
    DISPLAY_BACKLIGHT = 12,
};

extern mr_t mr;

static uint8_t textbuffer[H240_DISPLAY_TEXTBUFFER_SIZE];
static bool displayEnabled;
static bool backlightOwned;
static bool panelInitialized;
static volatile bool spiFault;

// GalaxyCore GC9307 Application Notes v1.0, section 5.2 CTC 2.4-inch.
// MADCTL 0xe8 is the H240 landscape setting observed at factory 0x0800404e.
static const struct
{
    uint8_t command;
    uint8_t size;
    uint8_t data[6];
} panelSetup[] = {
    {0xfe, 0, {0}},
    {0xef, 0, {0}},
    {MR_ST7789_MADCTL, 1, {0xe8}},
    {MR_ST7789_COLMOD, 1, {0x05}},
    {MR_ST7789_INVON, 0, {0}},
    {0x86, 1, {0x98}},
    {0xc9, 1, {0x0a}},
    {0x89, 1, {0x03}},
    {0xe8, 2, {0x12, 0x00}},
    {0xc3, 1, {0x45}},
    {0xc4, 1, {0x20}},
    {0x8b, 1, {0x80}},
    {0xff, 1, {0x62}},
    {0x8d, 1, {0x22}},
    {0x99, 1, {0x3e}},
    {0x9d, 1, {0x4b}},
    {0x8e, 1, {0x0f}},
    {0xf0, 6, {0x82, 0x00, 0x16, 0x15, 0x0b, 0x40}},
    {0xf1, 6, {0x55, 0x98, 0x98, 0x2c, 0x31, 0xbf}},
    {0xf2, 6, {0x42, 0x09, 0x0d, 0x0e, 0x0a, 0x37}},
    {0xf3, 6, {0x47, 0x93, 0x91, 0x17, 0x19, 0x9f}},
};

void initBacklight(void)
{
    ca32GpioOutput(CA32_GPIO_C, DISPLAY_BACKLIGHT, true);

    RCC_APB2ENR |= RCC_TIM1_ENABLE;
    TIM1_PSC = 0;
    TIM1_ARR = BACKLIGHT_PERIOD - 1;
    TIM1_CCR2 = BACKLIGHT_PERIOD;
    TIM1_CCMR1 = (TIM1_CCMR1 & 0x00ff) | TIM1_CH2_PWM1_PRELOAD;
    TIM1_CCER = (TIM1_CCER & ~0x00f0) | TIM1_CH2_ENABLE;
    TIM1_EGR = TIM1_GENERATE_UPDATE;
    TIM1_BDTR |= TIM1_MAIN_OUTPUT_ENABLE;
    TIM1_CR1 |= TIM1_ENABLE;
    ca32GpioAlternate(CA32_GPIO_C, DISPLAY_BACKLIGHT, 6);
    backlightOwned = true;
}

void setBacklight(bool value)
{
    // Active-low drive. Stay within the 15–80% high-duty factory brightness range.
    static const uint8_t highPercent[DISPLAY_BRIGHTNESS_NUM] = {80, 60, 40, 15};

    TIM1_CCR2 = value && !spiFault
                    ? BACKLIGHT_PERIOD * highPercent[settings.displayBrightness] / 100U
                    : BACKLIGHT_PERIOD;
}

static bool waitSpi(uint16_t mask, uint16_t expected)
{
    if (spiFault)
        return false;

    for (unsigned remaining = 1024; remaining != 0; remaining--)
        if ((SPI1_SR & mask) == expected)
            return true;

    spiFault = true;
    ca32GpioWrite(CA32_GPIO_C, DISPLAY_CS, true);
    if (backlightOwned)
        setBacklight(false);
    return false;
}

static void onDisplaySetReset(bool value)
{
    ca32GpioWrite(CA32_GPIO_C, DISPLAY_RESET, !value);
}

static void onDisplaySetChipselect(bool value)
{
    // The last queued byte must leave the shifter before changing chip select.
    bool ready = waitSpi(SPI_SR_TX_EMPTY | SPI_SR_BUSY, SPI_SR_TX_EMPTY);
    ca32GpioWrite(CA32_GPIO_C, DISPLAY_CS, !value || !ready);
}

static void onDisplaySetCommand(bool value)
{
    if (waitSpi(SPI_SR_TX_EMPTY | SPI_SR_BUSY, SPI_SR_TX_EMPTY))
        ca32GpioWrite(CA32_GPIO_C, DISPLAY_DC, !value);
}

static void onDisplaySend(uint16_t value)
{
    if (waitSpi(SPI_SR_TX_EMPTY, SPI_SR_TX_EMPTY))
        SPI1_DR = (uint8_t)value;
}

static void onDisplaySend16(uint16_t value)
{
    onDisplaySend(value >> 8);
    onDisplaySend(value & 0xff);
}

static void sendPanelCommand(uint8_t command, const uint8_t *data, uint8_t size)
{
    mr_set_chipselect(&mr, true);
    mr_send_command(&mr, command);
    for (uint8_t i = 0; i < size; i++)
        mr_send_data(&mr, data[i]);
    mr_set_chipselect(&mr, false);
}

bool h240InitDisplayPanel(uint32_t peripheralClockHz)
{
    uint16_t spiDivider;
    if (peripheralClockHz == H240_FACTORY_CLOCK_HZ)
        spiDivider = SPI_CR1_DIV16;
    else if (peripheralClockHz == CA32_CORE_CLOCK_HZ)
        spiDivider = SPI_CR1_DIV4;
    else
        return false;

    ca32GpioOutput(CA32_GPIO_C, DISPLAY_CS, true);
    ca32GpioOutput(CA32_GPIO_C, DISPLAY_DC, true);
    ca32GpioOutput(CA32_GPIO_C, DISPLAY_RESET, false);

    // External flash uses PA0–PA3 in the factory SRAM SPI routines.
    RCC_APB2ENR |= RCC_SPI1_ENABLE;
    SPI1_CR2 = 0;
    SPI1_CR1 &= ~SPI_CR1_ENABLE;
    SPI1_CR1 = SPI_CR1_MASTER | spiDivider | SPI_CR1_SOFTWARE_NSS | SPI_CR1_BIDIRECTIONAL_TX;
    ca32GpioAlternate(CA32_GPIO_C, DISPLAY_SCK, 5);
    ca32GpioAlternate(CA32_GPIO_C, DISPLAY_MOSI, 5);
    SPI1_CR1 |= SPI_CR1_ENABLE;

    mr_st7789_setup(&mr, DISPLAY_HEIGHT, DISPLAY_WIDTH, MR_DISPLAY_ROTATION_270,
                     textbuffer, sizeof(textbuffer), sleep, onDisplaySetReset,
                     onDisplaySetChipselect, onDisplaySetCommand,
                     onDisplaySend, onDisplaySend16);

    sleep(120);
    onDisplaySetReset(false);
    sleep(120);

    for (unsigned i = 0; i < sizeof(panelSetup) / sizeof(panelSetup[0]); i++)
        sendPanelCommand(panelSetup[i].command, panelSetup[i].data, panelSetup[i].size);

    sendPanelCommand(MR_ST7789_SLPOUT, NULL, 0);
    sleep(120);
    sendPanelCommand(MR_ST7789_DISPOFF, NULL, 0);

    return !spiFault;
}

void initDisplay(void)
{
    initBacklight();
    panelInitialized = h240InitDisplayPanel(CA32_CORE_CLOCK_HZ);
}

bool h240DisplayHealthy(void)
{
    return panelInitialized && !spiFault;
}

void setDisplayEnabled(bool value)
{
    // shortcut: keep GRAM writable while dark, until the UI can wake the panel before drawing.
    if (displayEnabled != value)
        sendPanelCommand(value ? MR_ST7789_DISPON : MR_ST7789_DISPOFF, NULL, 0);
    displayEnabled = value && !spiFault;
}

bool isDisplayEnabled(void)
{
    return displayEnabled && !spiFault;
}

void refreshDisplay(void)
{
    // Drawing writes directly to GRAM.
}

#endif
