/*
 * Rad Pro
 * FNIRSI GC-01 driver
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(GC01)

#include "mcu-renderer-st7789.h"

#include "../peripherals/display.h"
#include "../peripherals/keyboard.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/system.h"

// System

void Reset_Handler(void)
{
    __asm__ volatile(
        // Set SP for bootloader
        "   ldr     r0, =0x08004000\n"
        "   ldr     r0, [r0]\n"
        "   mov     sp, r0\n"

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
        "   bx      lr\n");
}

void initSystem(void)
{
    // Fix bootloader
    NVIC_DisableAllIRQs();
    SCB->VTOR = FIRMWARE_BASE;

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
                RCC_CFGR_HPRE_DIV1 |    // Set AHB clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_PPRE1_DIV2 |   // Set APB1 clock: 72 MHz / 2 = 36 MHz
                RCC_CFGR_PPRE2_DIV1 |   // Set APB2 clock: 72 MHz / 1 = 72 MHz
                RCC_CFGR_ADCPRE_DIV6 |  // Set ADC clock: 72 MHz / 6 = 12 MHz
                RCC_CFGR_PLLSRC_HSE |   // Set PLL source: HSE
                RCC_CFGR_PLLXTPRE_HSE | // Set PLL HSE predivision factor: /1
                RCC_CFGR_PLLMULL9 |     // Set PLL multiplier: 9x
                RCC_CFGR_USBPRE_DIV1_5; // Set USB prescaler: 1.5x

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
}

// Tube

#define TUBE_DEFAULT_SIGNATURE (*((uint8_t *)(0x08003800)))
#define TUBE_DEFAULT_SIGNATURE_VALUE 0xeb
#define TUBE_DEFAULT_HV_FREQUENCY (*((uint16_t *)(0x08003824)))
#define TUBE_DEFAULT_HV_DUTYCYCLE (*((uint16_t *)(0x08003826)))

bool getTubeDefaultHVFrequency(float *value)
{
    if (TUBE_DEFAULT_SIGNATURE == TUBE_DEFAULT_SIGNATURE_VALUE)
    {
        uint32_t intValue = TUBE_DEFAULT_HV_FREQUENCY;
        if (intValue >= 100)
        {
            *value = (float)intValue;

            return true;
        }
    }

    return false;
}

bool getTubeDefaultHVDutyCycle(float *value)
{
    if (TUBE_DEFAULT_SIGNATURE == TUBE_DEFAULT_SIGNATURE_VALUE)
    {
        uint32_t intValue = TUBE_DEFAULT_HV_DUTYCYCLE;

        if (intValue <= 1000)
        {
            *value = 0.001F * (float)intValue;

            return true;
        }
    }

    return false;
}

// Communications

#if defined(CH32)
const char *const commId = "FNIRSI GC-01 (CH32F103C8);" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#elif defined(APM32)
const char *const commId = "FNIRSI GC-01 (APM32F103CB);" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;
#endif

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO
    gpio_setup(KEY_LEFT_PORT, KEY_LEFT_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_RIGHT_PORT, KEY_RIGHT_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_UP_PORT, KEY_UP_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_DOWN_PORT, KEY_DOWN_PIN, GPIO_MODE_INPUT_PULLUP);
    gpio_setup(KEY_OK_PORT, KEY_OK_PIN, GPIO_MODE_INPUT_PULLUP);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_OK] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
}

// Display

extern mr_t mr;

bool displayEnabled;

static uint8_t displayTextbuffer[88 * 88];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x36), // VCOM=1.45 V
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
    gpio_modify(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN, !value);
}

static void onDisplaySetCommand(bool value)
{
    gpio_modify(DISPLAY_DCX_PORT, DISPLAY_DCX_PIN, !value);
}

static void onDisplaySend(uint16_t value)
{
    DISPLAY_WRX_PORT->BRR = get_bitvalue(DISPLAY_WRX_PIN);
    DISPLAY_DATA_PORT->ODR = value;
    DISPLAY_WRX_PORT->BSRR = get_bitvalue(DISPLAY_WRX_PIN);
}

uint8_t displayPin[] = {
    DISPLAY_RESX_PIN,
    DISPLAY_CSX_PIN,
    DISPLAY_DCX_PIN,
    DISPLAY_RDX_PIN,
    DISPLAY_WRX_PIN,
};

void initDisplay(void)
{
    // GPIO
    for (uint8_t i = 0; i < sizeof(displayPin); i++)
    {
        gpio_set(GPIOC, displayPin[i]);
        gpio_setup(GPIOC, displayPin[i], GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    }

    DISPLAY_DATA_PORT->CRL = (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 0) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 4) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 8) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 12) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 16) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 20) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 24) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 28);
    DISPLAY_DATA_PORT->CRH = (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 0) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 4) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 8) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 12) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 16) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 20) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 24) |
                             (GPIO_MODE_OUTPUT_50MHZ_PUSHPULL << 28);

    // mcu-renderer
    mr_st7789_init(&mr,
                   240,
                   320,
                   MR_DISPLAY_ROTATION_270,
                   displayTextbuffer,
                   sizeof(displayTextbuffer),
                   onDisplaySleep,
                   onDisplaySetReset,
                   onDisplaySetChipselect,
                   onDisplaySetCommand,
                   onDisplaySend,
                   onDisplaySend);

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
