/*
 * Rad Pro
 * GQ GMC-800 driver
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(GMC800)

#include "mcu-renderer-st7789.h"

#include "../peripherals/display.h"
#include "../peripherals/keyboard.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/system.h"

// System

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Overrides the weak startup Reset_Handler. A bootloader may enter the
// app with its own MSP, VTOR, interrupts and SysTick still active: these
// are fixed before .data/.bss are set up, so no interrupt can run on
// uninitialized RAM.
__attribute__((naked, noreturn)) void Reset_Handler(void)
{
    __asm__ volatile(
        // Set SP (r0 keeps FIRMWARE_BASE for VTOR)
        "   ldr     r0, =" TOSTRING(FIRMWARE_BASE) "\n"
        "   ldr     r1, [r0]\n"
        "   mov     sp, r1\n"

        // r2: system control space base
        "   ldr     r2, =0xe000e000\n"

        // Stop SysTick (SysTick->CTRL = 0), clear a pending SysTick (SCB->ICSR = PENDSTCLR)
        "   movs    r1, #0\n"
        "   str     r1, [r2, #0x010]\n"
        "   mov     r1, #0x02000000\n"
        "   str     r1, [r2, #0xd04]\n"

        // Disable and clear all NVIC interrupts (NVIC->ICER[0..7], NVIC->ICPR[0..7])
        "   mov     r1, #0xffffffff\n"
        "   movs    r3, #8\n"

        "LoopDisableIRQs:\n"
        "   str     r1, [r2, #0x180]\n"
        "   str     r1, [r2, #0x280]\n"
        "   adds    r2, r2, #4\n"
        "   subs    r3, r3, #1\n"
        "   bne     LoopDisableIRQs\n"

        // Set VTOR (SCB->VTOR at 0xe000ed08; r2 has advanced by 0x20)
        "   str     r0, [r2, #0xce8]\n"
        "   dsb\n"
        "   isb\n"

        // Call the clock system initialization function
        "   bl      SystemInit\n"

        // Copy the data segment initializers from flash to SRAM
        "   ldr     r0, =_sdata\n"
        "   ldr     r1, =_edata\n"
        "   ldr     r2, =_sidata\n"
        "   movs    r3, #0\n"
        "   b       LoopCopyDataInit\n"

        "CopyDataInit:\n"
        "   ldr     r4, [r2, r3]\n"
        "   str     r4, [r0, r3]\n"
        "   adds    r3, r3, #4\n"

        "LoopCopyDataInit:\n"
        "   adds    r4, r0, r3\n"
        "   cmp     r4, r1\n"
        "   bcc     CopyDataInit\n"

        // Zero fill the bss segment
        "   ldr     r2, =_sbss\n"
        "   ldr     r4, =_ebss\n"
        "   movs    r3, #0\n"
        "   b       LoopFillZerobss\n"

        "FillZerobss:\n"
        "   str     r3, [r2]\n"
        "   adds    r2, r2, #4\n"

        "LoopFillZerobss:\n"
        "   cmp     r2, r4\n"
        "   bcc     FillZerobss\n"

        // Call static constructors
        "   bl      __libc_init_array\n"

        // Call the application's entry point
        "   bl      main\n"

        "LoopForever:\n"
        "   b       LoopForever\n");
}

void initGPIO(void)
{
    // Enable GPIOA, GPIOB, GPIOC, GPIOD
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN);

    // PULSE_SOUND_EN output level high (pulse sound disabled), set before
    // initEvents(): from then on, the tick drives the pin
    gpio_modify(PULSE_SOUND_EN_PORT, PULSE_SOUND_EN_PIN, true);
}

void initSystem(void)
{
    // Disable USB clock: on GD32F30x, USBDPSC is locked while the USBD
    // clock is enabled, which the bootloader may have left on
    clear_bits(RCC->APB1ENR, RCC_APB1ENR_USBEN);

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

    // Configure AHB, APB1, APB2, ADC, PLL
    RCC->CFGR = RCC_CFGR_SW_HSI |       // Select HSI as system clock
                RCC_CFGR_HPRE_DIV1 |    // Set AHB clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_PPRE1_DIV2 |   // Set APB1 clock: 72 MHz / 2 = 36 MHz
                RCC_CFGR_PPRE2_DIV1 |   // Set APB2 clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_ADCPRE_DIV6 |  // Set ADC clock: 72 MHz / 6 = 12 MHz
                RCC_CFGR_PLLSRC_HSE |   // Set PLL source: HSE
                RCC_CFGR_PLLXTPRE_HSE | // Set PLL HSE predivision factor: /1
                RCC_CFGR_PLLMULL9;      // Set PLL multiplier: 9x

    // Enable PLL
    set_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

    // Set PLL as system clock
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_PLL);

    // Disable JTAG
    rcc_enable_afio();
    modify_bits(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_Msk, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);

    // Disable USART RTS reset
    gpio_modify(SYSTEM_RESET_EN_PORT, SYSTEM_RESET_EN_PIN, false);
    gpio_setup(SYSTEM_RESET_EN_PORT,
               SYSTEM_RESET_EN_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
}

// Bootloader

void startBootloader(void)
{
    // Disable interrupts
    NVIC_DisableAllIRQs();
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->VAL = 0;

    // Set HSI as system clock (HSI is never turned off)
    modify_bits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSI);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    // Reset RCC
    RCC->CFGR = 0;

    // Set 0 wait states for flash
    modify_bits(FLASH->ACR, FLASH_ACR_LATENCY_Msk, FLASH_ACR_LATENCY_0WS);

    // Enable USART RTS reset
    gpio_setup(SYSTEM_RESET_EN_PORT, SYSTEM_RESET_EN_PIN, GPIO_MODE_INPUT_FLOATING);

    // Jump to bootloader
    __set_MSP(SYSTEM_VECTOR_TABLE->sp);
    SYSTEM_VECTOR_TABLE->onReset();
}

// Communications

#if defined(GMC800V2)
const char *const commId = "GQ GMC-800V2;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#else
const char *const commId = "GQ GMC-800;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#endif

// Keyboard

// 4-bit CRL field mask of a pin (pins 0-7)
#define GPIO_MASK4L(pin) (0b1111U << (4 * (pin)))
// 4-bit CRH field mask of a pin (pins 8-15)
#define GPIO_MASK4H(pin) (0b1111U << (4 * ((pin) - 8)))

// All keys are pins 8-15 of KEY_LEFT_PORT
#define KEY_MASK (get_bitvalue(KEY_LEFT_PIN) |  \
                  get_bitvalue(KEY_RIGHT_PIN) | \
                  get_bitvalue(KEY_UP_PIN) |    \
                  get_bitvalue(KEY_DOWN_PIN))
#define KEY_MASK4H (GPIO_MASK4H(KEY_LEFT_PIN) |  \
                    GPIO_MASK4H(KEY_RIGHT_PIN) | \
                    GPIO_MASK4H(KEY_UP_PIN) |    \
                    GPIO_MASK4H(KEY_DOWN_PIN))

void initKeyboardHardware(void)
{
    // GPIO: inputs with pull-up (ODR bits set)
    KEY_LEFT_PORT->BSRR = KEY_MASK;
    modify_bits(KEY_LEFT_PORT->CRH,
                KEY_MASK4H,
                KEY_MASK4H & (0x11111111U * (GPIO_MODE_INPUT_PULLUP & GPIO_MODE_Msk)));
}

// Moves the (inverted) input bit of a pin to the bit of a key
#define getKey(keys, pin, key) ((((keys) >> (pin)) & 1) << (key))

uint32_t readKeyboardKeys(void)
{
    // Keys are active low
    uint32_t keys = ~KEY_LEFT_PORT->IDR;

    return getKey(keys, KEY_LEFT_PIN, KEY_LEFT) |
           getKey(keys, KEY_RIGHT_PIN, KEY_RIGHT) |
           getKey(keys, KEY_UP_PIN, KEY_UP) |
           getKey(keys, KEY_DOWN_PIN, KEY_DOWN);
}

// Display

extern mr_t mr;

static bool displayEnabled;

static uint8_t displayTextbuffer[88 * 88];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_INVON),
    MR_SEND_COMMAND(MR_ST7789_GCTRL),
    MR_SEND_DATA(0x74), // VGH=14.97 V, VGL=-9.6 V
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x1d), // VCOM=0.825 V (flicker-balanced with the gamma tables below)
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4),
    MR_SEND_DATA(0xa1), // AVCL=-4.8 V
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
    MR_SEND_DATA(0x21), // V61 (GQ: 0x11), darker shadows
    MR_SEND_DATA(0x30), // V62 (GQ: 0x20), darker shadows
    MR_SEND_COMMAND(MR_ST7789_NVGAMCTRL),
    MR_SEND_DATA(0xd0),
    MR_SEND_DATA(0x02),
    MR_SEND_DATA(0x07),
    MR_SEND_DATA(0x0a),
    MR_SEND_DATA(0x0b),
    MR_SEND_DATA(0x18),
    MR_SEND_DATA(0x34),
    MR_SEND_DATA(0x43),
    // Dark end matches PVGAMCTRL, so all gray levels are DC-balanced at one VCOM
    MR_SEND_DATA(0x44), // V43 (GQ: 0x4a)
    MR_SEND_DATA(0x3a), // J1, V50 (GQ: 0x2b)
    MR_SEND_DATA(0x0b), // V57 (GQ: 0x1b)
    MR_SEND_DATA(0x06), // V59 (GQ: 0x1c)
    MR_SEND_DATA(0x21), // V61 (GQ: 0x22)
    MR_SEND_DATA(0x30), // V62 (GQ: 0x1f)
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
    // GPIO: CSX, SCL and SDA (pins 0-7) share a port
    gpio_modify(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN, true);
    gpio_modify(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, true);

    gpio_setup(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    modify_bits(DISPLAY_CSX_PORT->CRL,
                GPIO_MASK4L(DISPLAY_CSX_PIN) | GPIO_MASK4L(DISPLAY_SCL_PIN) | GPIO_MASK4L(DISPLAY_SDA_PIN),
                (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << (4 * DISPLAY_CSX_PIN)) |
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
