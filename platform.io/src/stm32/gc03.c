/*
 * Rad Pro
 * FNIRSI GC-03 driver
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(GC03)

#include "mcu-renderer-st7789.h"

#include "../peripherals/display.h"
#include "../peripherals/emf.h"
#include "../peripherals/keyboard.h"
#include "../peripherals/tube.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/system.h"

// System

void initGPIO(void)
{
    // Enable GPIOA, GPIOB, GPIOC
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN);
}

void initSystem(void)
{
    // Disable TIM7 (left on by bootloader)
    rcc_disable_tim(TIM7);

    // Set HSI as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSI);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    // Enable HSE
    set_bits(RCC->CR, RCC_CR_HSEON);
    wait_until_bits_set(RCC->CR, RCC_CR_HSERDY);

    // Set 2 wait states for flash
    modify_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk, FLASH_ACR_LATENCY_2WS);

    // Configure AHB, APB1, APB2, ADC, PLL, USB
    RCC->CFGR = RCC_CFGR_SW_HSI |       // Select HSI as system clock
                RCC_CFGR_HPRE_DIV1 |    // Set AHB clock: 96 MHz / 1 = 96 MHz
                RCC_CFGR_PPRE1_DIV2 |   // Set APB1 clock: 96 MHz / 2 = 48 MHz
                RCC_CFGR_PPRE2_DIV1 |   // Set APB2 clock: 96 MHz / 1 = 96 MHz
                RCC_CFGR_ADCPRE_DIV8 |  // Set ADC clock: 96 MHz / 8 = 12 MHz
                RCC_CFGR_PLLSRC_HSE |   // Set PLL source: HSE
                RCC_CFGR_PLLXTPRE_HSE | // Set PLL HSE predivision factor: /1
                RCC_CFGR_PLLMULL12 |    // Set PLL multiplier: 12x
                RCC_CFGR_USBPRE_DIV2;   // Set USB prescaler: 2x

    // Enable PLL
    set_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

    // Set PLL as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

    // Disable JTAG
    rcc_enable_afio();
    modify_bits(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_Msk, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    // Setup PB12 (purpose unknown, copied from original firmware)
    gpio_setup(GPIOB, 12, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
}

// Tube

#define GC03_TUBE_DEFAULT_SIGNATURE (*((uint32_t *)(0x0803fc00)))
#define GC03_TUBE_DEFAULT_SIGNATURE_VALUE 0x4b6e4564
#define GC03_TUBE_DEFAULT_HV_FREQUENCY (*((uint16_t *)(0x0803fc1a)))
#define GC03_TUBE_DEFAULT_HV_DUTYCYCLE_PERIOD 0.0000125F

bool getTubeDefaultHVFrequency(float *value)
{
    // Frequency in units of 100 Hz
    uint32_t frequency = GC03_TUBE_DEFAULT_HV_FREQUENCY;

    // 1-799: the upper limit keeps the derived duty cycle below 100% (on-time 12.5 us)
    if ((GC03_TUBE_DEFAULT_SIGNATURE != GC03_TUBE_DEFAULT_SIGNATURE_VALUE) ||
        ((frequency - 1) >= 799))
        return false;

    *value = (float)(100 * frequency);

    return true;
}

bool getTubeDefaultHVDutyCycle(float *value)
{
    float frequency;
    if (!getTubeDefaultHVFrequency(&frequency))
        return false;

    *value = GC03_TUBE_DEFAULT_HV_DUTYCYCLE_PERIOD * frequency;

    return true;
}

// Communications

const char *const commId = "FNIRSI GC-03;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;

// Keyboard

void KNOB_IRQ_HANDLER(void);

static struct
{
    uint32_t prevABState;
    int32_t subSteps;
    volatile int8_t detentSteps;

    int8_t prevDetentSteps;
    bool keyDown;
} keyboardKnob;

static const int8_t knobQuadratureLUT[16] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0};

// 4-bit CRL field mask of a pin (pins 0-7)
#define GPIO_MASK4L(pin) (0b1111U << (4 * (pin)))
// 4-bit CRH field mask of a pin (pins 8-15)
#define GPIO_MASK4H(pin) (0b1111U << (4 * ((pin) - 8)))

// All 4-bit fields of a CRL/CRH register set to input with pull-up/down
#define INPUT_PULL_CR (0x11111111U * (GPIO_MODE_INPUT_PULLUP & GPIO_MODE_Msk))

// LEFT and RIGHT (pins 8-15), the knob button and knob A/B (pins 0-7)
// share KEY_LEFT_PORT
#define KEY_PORT_MASK (get_bitvalue(KEY_LEFT_PIN) |     \
                       get_bitvalue(KEY_RIGHT_PIN) |    \
                       get_bitvalue(KNOB_A_PIN) |       \
                       get_bitvalue(KNOB_BUTTON_PIN) |  \
                       get_bitvalue(KNOB_B_PIN))
#define KEY_PORT_MASK4L (GPIO_MASK4L(KNOB_A_PIN) |      \
                         GPIO_MASK4L(KNOB_BUTTON_PIN) | \
                         GPIO_MASK4L(KNOB_B_PIN))
#define KEY_PORT_MASK4H (GPIO_MASK4H(KEY_LEFT_PIN) | \
                         GPIO_MASK4H(KEY_RIGHT_PIN))

void initKeyboardHardware(void)
{
    // GPIO: inputs with pull-up (ODR bits set)
    gpio_setup(KEY_POWER_PORT, KEY_POWER_PIN, GPIO_MODE_INPUT_PULLUP);

    KEY_LEFT_PORT->BSRR = KEY_PORT_MASK;
    modify_bits(KEY_LEFT_PORT->CRL, KEY_PORT_MASK4L, KEY_PORT_MASK4L & INPUT_PULL_CR);
    modify_bits(KEY_LEFT_PORT->CRH, KEY_PORT_MASK4H, KEY_PORT_MASK4H & INPUT_PULL_CR);

    KNOB_IRQ_HANDLER();
    keyboardKnob.subSteps = 0;

    // EXTI
    exti_setup(KNOB_A_PORT, KNOB_A_PIN, true, true);
    exti_enable_interrupt(KNOB_A_PIN);
    exti_setup(KNOB_B_PORT, KNOB_B_PIN, true, true);
    exti_enable_interrupt(KNOB_B_PIN);

    nvic_set_priority(KNOB_IRQ, 0xc0);
    NVIC_EnableIRQ(KNOB_IRQ);
}

void KNOB_IRQ_HANDLER(void)
{
    exti_clear_pending_interrupt(KNOB_A_PIN);
    exti_clear_pending_interrupt(KNOB_B_PIN);

    uint32_t currABState = (gpio_get(KNOB_A_PORT, KNOB_A_PIN) << 1) | gpio_get(KNOB_B_PORT, KNOB_B_PIN);

    int32_t subStepsDelta = knobQuadratureLUT[(keyboardKnob.prevABState << 2) | currABState];

    keyboardKnob.prevABState = currABState;

    // A zero delta leaves subSteps unchanged, within (-2, 2)
    keyboardKnob.subSteps += subStepsDelta;

    if (keyboardKnob.subSteps >= 2)
    {
        keyboardKnob.detentSteps++;
        keyboardKnob.subSteps = 0;
    }
    else if (keyboardKnob.subSteps <= -2)
    {
        keyboardKnob.detentSteps--;
        keyboardKnob.subSteps = 0;
    }
}

// Moves the (inverted) input bit of a pin to the bit of a key
#define getKey(keys, pin, key) ((((keys) >> (pin)) & 1) << (key))

uint32_t readKeyboardKeys(void)
{
    // LEFT, RIGHT and the knob button share a port: read it once (keys are active low)
    uint32_t keys = ~KEY_LEFT_PORT->IDR;

    return getKey(keys, KEY_LEFT_PIN, KEY_LEFT) |
           getKey(keys, KEY_RIGHT_PIN, KEY_RIGHT) |
           getKey(keys, KNOB_BUTTON_PIN, KEY_RIGHT) |
           (!gpio_get(KEY_POWER_PORT, KEY_POWER_PIN) << KEY_OK);
}

uint32_t readKeyboardKnob(void)
{
    // Each knob step is reported as a key press lasting one keyboard update
    if (keyboardKnob.keyDown)
    {
        keyboardKnob.keyDown = false;

        return 0;
    }

    // Wrap-safe difference of the 8-bit step counters
    int8_t detentSteps = keyboardKnob.detentSteps;
    int8_t detentDelta = (int8_t)(detentSteps - keyboardKnob.prevDetentSteps);
    if (detentDelta == 0)
        return 0;

    keyboardKnob.prevDetentSteps = detentSteps;

    // Knob steps do not wake the display
    if (!isDisplayAwake())
        return 0;

    keyboardKnob.keyDown = true;

    return (detentDelta > 0) ? (1U << KEY_DOWN) : (1U << KEY_UP);
}

// Display

extern mr_t mr;

static bool displayEnabled;

static uint8_t displayTextbuffer[88 * 88];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x2c), // VCOM=1.2 V
    MR_SEND_COMMAND(MR_ST7789_VRHS),
    MR_SEND_DATA(0x12), // VRH=4.45 V
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4),
    MR_SEND_DATA(0xa1), // AVCL=-4.8 V
    MR_SEND_COMMAND(MR_ST7789_PVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x00),
    MR_SEND_DATA(0x05),
    MR_SEND_DATA(0x0e),
    MR_SEND_DATA(0x15),
    MR_SEND_DATA(0x0d),
    MR_SEND_DATA(0x37),
    MR_SEND_DATA(0x43),
    MR_SEND_DATA(0x47),
    MR_SEND_DATA(0x09),
    MR_SEND_DATA(0x15),
    MR_SEND_DATA(0x12),
    MR_SEND_DATA(0x16),
    MR_SEND_DATA(0x19),
    MR_SEND_COMMAND(MR_ST7789_NVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x00),
    MR_SEND_DATA(0x05),
    MR_SEND_DATA(0x0d),
    MR_SEND_DATA(0x0c),
    MR_SEND_DATA(0x06),
    MR_SEND_DATA(0x2d),
    MR_SEND_DATA(0x44),
    MR_SEND_DATA(0x40),
    MR_SEND_DATA(0x0e),
    MR_SEND_DATA(0x1c),
    MR_SEND_DATA(0x18),
    MR_SEND_DATA(0x16),
    MR_SEND_DATA(0x19),
    MR_END(),
};

static void onDisplaySetReset(bool value)
{
    gpio_modify(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN, !value);
}

static void onDisplaySetChipselect(bool value)
{
    spi_wait_while_busy(DISPLAY_SPI);

    gpio_modify(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, !value);
}

static void onDisplaySetCommand(bool value)
{
    spi_wait_while_busy(DISPLAY_SPI);

    gpio_modify(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN, !value);
}

static void onDisplaySend(uint16_t value)
{
    spi_send(DISPLAY_SPI, value);
}

static void onDisplaySend16(uint16_t value)
{
    spi_send(DISPLAY_SPI, (value >> 8) & 0xff);
    spi_send(DISPLAY_SPI, (value >> 0) & 0xff);
}

void initDisplay(void)
{
    // GPIO: RESX/DCX and SCL/SDA (pins 0-7) share a port each
    gpio_modify(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN, true);
    gpio_modify(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, true);

    modify_bits(DISPLAY_RESX_PORT->CRL,
                GPIO_MASK4L(DISPLAY_RESX_PIN) | GPIO_MASK4L(DISPLAY_DCX_PIN),
                (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << (4 * DISPLAY_RESX_PIN)) |
                    (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << (4 * DISPLAY_DCX_PIN)));
    gpio_setup(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    modify_bits(DISPLAY_SCL_PORT->CRL,
                GPIO_MASK4L(DISPLAY_SCL_PIN) | GPIO_MASK4L(DISPLAY_SDA_PIN),
                (GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL << (4 * DISPLAY_SCL_PIN)) |
                    (GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL << (4 * DISPLAY_SDA_PIN)));

    // SPI
    rcc_enable_spi(DISPLAY_SPI);

    spi_setup(DISPLAY_SPI);
    spi_enable(DISPLAY_SPI);

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
                   sleep,
                   onDisplaySetReset,
                   onDisplaySetChipselect,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend16);

    mr_send_sequence(&mr, displayInitSequence);

    initBacklight();
}

void setDisplayEnabled(bool value)
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
