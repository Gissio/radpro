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

    // Enable GPIOA, GPIOB, GPIOC
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN);

    // Setup PB12 (from original firmware)
    gpio_setup(GPIOB, 12, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
}

// Tube

#define TUBE_DEFAULT_SIGNATURE (*((uint32_t *)(0x0803fc00)))
#define TUBE_DEFAULT_SIGNATURE_VALUE 0x4b6e4564
#define TUBE_DEFAULT_HV_FREQUENCY (*((uint16_t *)(0x0803fc1a)))
#define TUBE_DEFAULT_HV_DUTYCYCLE_PERIOD 0.0000125F

bool getTubeDefaultHVFrequency(float *value)
{
    if (TUBE_DEFAULT_SIGNATURE == TUBE_DEFAULT_SIGNATURE_VALUE)
    {
        if ((TUBE_DEFAULT_HV_FREQUENCY >= 1) && (TUBE_DEFAULT_HV_FREQUENCY < 1000))
        {
            *value = 100.0F * TUBE_DEFAULT_HV_FREQUENCY;

            return true;
        }
    }

    return false;
}

bool getTubeDefaultHVDutyCycle(float *value)
{
    float frequency;
    if (!getTubeDefaultHVFrequency(&frequency))
        return false;

    *value = TUBE_DEFAULT_HV_DUTYCYCLE_PERIOD * frequency;

    return true;
}

// EMF meter

void initEMFMeter(void)
{
    setEMFMeterEnabled(false);

    gpio_setup(EMFMETER_EN_PORT, EMFMETER_EN_PIN, GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);
}

void setEMFMeterEnabled(bool value)
{
    gpio_modify(EMFMETER_EN_PORT, EMFMETER_EN_PIN, !value);
}

// Communications

const char *const commId = "FNIRSI GC-03;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;

// Keyboard

void KNOB_IRQ_HANDLER(void);

static struct
{
    int8_t prevABState;
    int8_t subSteps;
    volatile int8_t detentSteps;

    int8_t prevDetentSteps;
    bool keyPressed;
} keyboardKnob;

static const int8_t knobQuadratureLUT[16] = {
    0, 1, -1, 0,
    -1, 0, 0, 1,
    1, 0, 0, -1,
    0, -1, 1, 0};

void initKeyboardHardware(void)
{
    // GPIO
    gpio_setup(KEY_POWER_PORT, KEY_POWER_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_LEFT_PORT, KEY_LEFT_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_RIGHT_PORT, KEY_RIGHT_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KNOB_A_PORT, KNOB_A_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KNOB_B_PORT, KNOB_B_PIN, GPIO_MODE_INPUT_PULLUP);

    KNOB_IRQ_HANDLER();
    keyboardKnob.subSteps = 0;

    // EXTI
    exti_setup(KNOB_A_PORT, KNOB_A_PIN, true, true);
    exti_enable_interrupt(KNOB_A_PIN);
    exti_setup(KNOB_B_PORT, KNOB_B_PIN, true, true);
    exti_enable_interrupt(KNOB_B_PIN);

    NVIC_SetPriority(KNOB_IRQ, 0xc0);
    NVIC_EnableIRQ(KNOB_IRQ);
}

static volatile uint32_t lastKnobEventTime = 0;

void KNOB_IRQ_HANDLER(void)
{
    exti_clear_pending_interrupt(KNOB_A_PIN);
    exti_clear_pending_interrupt(KNOB_B_PIN);

    uint32_t now = currentTick;

    int8_t currABState = (gpio_get(KNOB_A_PORT, KNOB_A_PIN) << 1) | gpio_get(KNOB_B_PORT, KNOB_B_PIN);
    int8_t subStepsDelta = knobQuadratureLUT[(keyboardKnob.prevABState << 2) | currABState];
    keyboardKnob.prevABState = currABState;

    if (subStepsDelta != 0) 
    {
        if ((now - lastKnobEventTime) > 200) {
            keyboardKnob.subSteps = 0;
        }
        lastKnobEventTime = now;

        keyboardKnob.subSteps += subStepsDelta;

        if (keyboardKnob.subSteps >= 2) {
            keyboardKnob.detentSteps++;
            keyboardKnob.subSteps = 0;
        } 
        else if (keyboardKnob.subSteps <= -2) {
            keyboardKnob.detentSteps--;
            keyboardKnob.subSteps = 0;
        }
    }
}

void getKeyboardState(bool *isKeyDown)
{
    bool keyUp = false;
    bool keyDown = false;

    if (!keyboardKnob.keyPressed)
    {
        int32_t detentDelta = keyboardKnob.detentSteps - keyboardKnob.prevDetentSteps;
        if (detentDelta)
        {
            if (isDisplayAwake())
            {
                if (detentDelta > 0)
                    keyDown = true:
                else
                    keyUp = true;
    
                keyboardKnob.keyPressed = true;
            }

            keyboardKnob.prevDetentSteps = keyboardKnob.detentSteps;
        }
    }
    else
        keyboardKnob.keyPressed = false;

    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_UP] = keyUp;
    isKeyDown[KEY_DOWN] = keyDown;
    isKeyDown[KEY_OK] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
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

static void onDisplaySleep(uint32_t value)
{
    sleep(value);
}

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
    // GPIO
    gpio_set(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN);
    gpio_set(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN);

    gpio_setup(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_SCL_PORT, DISPLAY_SCL_PIN, GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
    gpio_setup(DISPLAY_SDA_PORT, DISPLAY_SDA_PIN, GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);

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
                   onDisplaySleep,
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

