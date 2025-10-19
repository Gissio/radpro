/*
 * Rad Pro
 * Bosean FS-5000 specifics
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(FS5000)

#include "../display.h"
#include "../events.h"
#include "../keyboard.h"
#include "../system.h"

#include "device.h"

#include "mcu-renderer-st7789.h"

// System

void initSystem(void)
{
    // Set SP and VTOR for bootloader
    __set_MSP(*((uint32_t *)FIRMWARE_BASE));
    NVIC_DisableAllIRQs();
    SCB->VTOR = FIRMWARE_BASE;

    // Enable HSI
    set_bits(RCC->CR, RCC_CR_HSION);
    wait_until_bits_set(RCC->CR, RCC_CR_HSIRDY);

    // Set HSI as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_HSI);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_HSI);

    // Disable MSI
    clear_bits(RCC->CR, RCC_CR_MSION);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    // Set 3 wait states for flash
    modify_bits(FLASH->ACR,
                FLASH_ACR_LATENCY_Msk,
                FLASH_ACR_LATENCY_3WS);

    // Enable prefetch
    set_bits(FLASH->ACR, FLASH_ACR_PRFTEN);

    // Configure AHB, APB1, APB2
    RCC->CFGR = RCC_CFGR_SW_HSI |      // Set HSI
                RCC_CFGR_HPRE_DIV1 |   // Set AHB clock: 64 MHz / 1 = 64 MHz
                RCC_CFGR_PPRE1_DIV16 | // Set APB1 clock: 64 MHz / 16 = 4 MHz
                RCC_CFGR_PPRE2_DIV16;  // Set APB2 clock: 64 MHz / 16 = 4 MHz

    // Configure PLL
    RCC->PLLCFGR = RCC_PLLCFGR_PLLSRC_HSI |          // Set PLL source: HSI
                   (0b000 << RCC_PLLCFGR_PLLM_Pos) | // Set PLL division factor: /1
                   (8 << RCC_PLLCFGR_PLLN_Pos) |     // Set main PLL VCO multiplication factor: 8x
                   RCC_PLLCFGR_PLLREN |              // Enable main PLL PLLCLK output
                   (0b00 << RCC_PLLCFGR_PLLR_Pos);   // Set main PLL PLLCLK division factor: /2

    // Enable PLL
    set_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_set(RCC->CR, RCC_CR_PLLRDY);

    // Set PLL as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_PLL);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_PLL);

    // Enable SYSCFG (for EXTI)
    set_bits(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    // Enable GPIOA, GPIOB, GPIOC, GPIOD, ADC
    set_bits(RCC->AHB2ENR,
             RCC_AHB2ENR_ADCEN |
                 RCC_AHB2ENR_GPIOAEN |
                 RCC_AHB2ENR_GPIOBEN |
                 RCC_AHB2ENR_GPIOCEN |
                 RCC_AHB2ENR_GPIODEN);

    // ADC
    RCC->CCIPR = (0b11 << RCC_CCIPR_ADCSEL_Pos); // Set system clock as ADC clock
    set_bits(((ADC_Common_TypeDef *)((uint8_t *)ADC1 + 0x300))->CCR,
             (0b0010 << ADC_CCR_PRESC_Pos)); // ADC clock divided by 4
}

void startBootloader(void)
{
    // Disable interrupts
    NVIC_DisableAllIRQs();
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->VAL = 0;

    // Enable MSI
    set_bits(RCC->CR, RCC_CR_MSION);
    wait_until_bits_set(RCC->CR, RCC_CR_MSIRDY);

    // Set MSI as system clock
    modify_bits(RCC->CFGR,
                RCC_CFGR_SW_Msk,
                RCC_CFGR_SW_MSI);
    wait_until_bits_value(RCC->CFGR,
                          RCC_CFGR_SWS_Msk,
                          RCC_CFGR_SWS_MSI);

    // Disable PLL
    clear_bits(RCC->CR, RCC_CR_PLLON);
    wait_until_bits_clear(RCC->CR, RCC_CR_PLLRDY);

    // Reset RCC
    RCC->CFGR = 0;
    RCC->PLLCFGR = 0x1000;

    // Set 0 wait states for flash
    modify_bits(FLASH->ACR,
                FLASH_ACR_LATENCY_Msk,
                FLASH_ACR_LATENCY_0WS);

    // Jump to bootloader
    __set_MSP(SYSTEM_VECTOR_TABLE->sp);
    SYSTEM_VECTOR_TABLE->onReset();
}

// Communications

const char *const commId = "Bosean FS-5000;" FIRMWARE_NAME " " FIRMWARE_VERSION "/" LANGUAGE;

// Keyboard

void initKeyboardHardware(void)
{
    // GPIO
    gpio_setup_input(KEY_LEFT_PORT,
                     KEY_LEFT_PIN,
                     GPIO_PULL_PULLUP);
    gpio_setup_input(KEY_OK_PORT,
                     KEY_OK_PIN,
                     GPIO_PULL_FLOATING);
    gpio_setup_input(KEY_RIGHT_PORT,
                     KEY_RIGHT_PIN,
                     GPIO_PULL_PULLUP);
}

void getKeyboardState(bool *isKeyDown)
{
    isKeyDown[KEY_LEFT] = !gpio_get(KEY_LEFT_PORT, KEY_LEFT_PIN);
    isKeyDown[KEY_OK] = !gpio_get(KEY_OK_PORT, KEY_OK_PIN);
    isKeyDown[KEY_RIGHT] = !gpio_get(KEY_RIGHT_PORT, KEY_RIGHT_PIN);
}

// Display

extern mr_t mr;

bool displayEnabled;

static uint8_t displayTextbuffer[88 * 88];

static const uint8_t displayInitSequence[] = {
    MR_SEND_COMMAND(MR_ST7789_RAMCTRL),
    MR_SEND_DATA(0x00),
    MR_SEND_DATA(0xe0),
    MR_SEND_COMMAND(MR_ST7789_VCOMS),
    MR_SEND_DATA(0x36), // VCOM=1.45 V
    MR_SEND_COMMAND(MR_ST7789_VRHS),
    MR_SEND_DATA(0x12), // VRH=4.45 V
    MR_SEND_COMMAND(MR_ST7789_PWCTRL1),
    MR_SEND_DATA(0xa4), // AVDD=6.8 V, AVCL=-4.8 V, VDDS=2.3 V
    MR_SEND_DATA(0xa1),
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
    gpio_modify(DISPLAY_RESX_PORT,
                DISPLAY_RESX_PIN,
                !value);
}

static void onDisplaySetChipselect(bool value)
{
    gpio_modify(DISPLAY_CSX_PORT,
                DISPLAY_CSX_PIN,
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

void initDisplay(void)
{
    // GPIO
    gpio_set(DISPLAY_POWER_PORT, DISPLAY_POWER_PIN);

    gpio_setup_output(DISPLAY_POWER_PORT,
                      DISPLAY_POWER_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);

    gpio_set(DISPLAY_RESX_PORT, DISPLAY_RESX_PIN);
    gpio_set(DISPLAY_CSX_PORT, DISPLAY_CSX_PIN);
    gpio_set(DISPLAY_RDX_PORT, DISPLAY_RDX_PIN);
    gpio_set(DISPLAY_WRX_PORT, DISPLAY_WRX_PIN);

    for (uint32_t i = 0; i < sizeof(displayPinSetup); i++)
        gpio_setup_output(displayPortSetup[i],
                          displayPinSetup[i],
                          GPIO_OUTPUTTYPE_PUSHPULL,
                          GPIO_OUTPUTSPEED_50MHZ,
                          GPIO_PULL_FLOATING);

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

void setDisplayEnable(bool value)
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
