/*
 * Rad Pro
 * STM32 LED
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/led.h"
#include "../stm32/device.h"
#include "../system/settings.h"

#include "stm32-ext.h"
#include "stm32.h"


#if defined(PULSE_LED)

#if defined(GC03)

// --- Global variables for color control ---
// Maximum pattern length - 8 ticks (for simplicity)
static volatile uint8_t current_pattern[8]; 
static volatile uint8_t current_pattern_len = 0;
static volatile uint8_t pattern_step = 0;
static volatile bool use_tdm_mode = false; // Flag: whether to use the timer

// Bitmasks for colors (convenient for code)
#define COLOR_OFF 0
#define COLOR_R   1
#define COLOR_G   2
#define COLOR_B   4

// TIM8 interrupt handler
void TIM8_UP_IRQHandler(void)
{
    // Clear the interrupt flag
    if (TIM8->SR & TIM_SR_UIF)
    {
        TIM8->SR = ~TIM_SR_UIF;

        // If TDM mode is off (solid color), do nothing in the interrupt
        if (!use_tdm_mode) return;

        // 1. Turn off ALL diodes (Active Low: 1 = OFF -> BSRR)
        PULSE_LED_R_PORT->BSRR = get_bitvalue(PULSE_LED_R_PIN);
        PULSE_LED_G_PORT->BSRR = get_bitvalue(PULSE_LED_G_PIN);
        PULSE_LED_B_PORT->BSRR = get_bitvalue(PULSE_LED_B_PIN);

        // 2. Get the current color from the pattern
        uint8_t color = current_pattern[pattern_step];

        // 3. Turn on the required diode (Active Low: 0 = ON -> BRR)
        if (color & COLOR_R) PULSE_LED_R_PORT->BRR = get_bitvalue(PULSE_LED_R_PIN);
        if (color & COLOR_G) PULSE_LED_G_PORT->BRR = get_bitvalue(PULSE_LED_G_PIN);
        if (color & COLOR_B) PULSE_LED_B_PORT->BRR = get_bitvalue(PULSE_LED_B_PIN);

        // 4. Step through the pattern
        pattern_step++;
        if (pattern_step >= current_pattern_len) pattern_step = 0;
    }
}
#endif // GC03

void initPulseLED(void)
{
#if defined(GC03) && defined(PULSE_LED)
    // Enable clocking
    set_bits(RCC->APB2ENR, RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_TIM8EN);
    rcc_enable_afio();

    // Configure pins as Push-Pull Output
    gpio_setup(PULSE_LED_R_PORT, PULSE_LED_R_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(PULSE_LED_G_PORT, PULSE_LED_G_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_setup(PULSE_LED_B_PORT, PULSE_LED_B_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);

    setPulseLED(false);

    // Configure TIM8
    // Timer frequency ~4 kHz, so that flickering is not noticeable even with long patterns
    TIM8->PSC = (APB2TIM_FREQUENCY / 1000000) - 1; 
    TIM8->ARR = 250 - 1; // 250 µs per step
    
    set_bits(TIM8->DIER, TIM_DIER_UIE);
    NVIC_SetPriority(TIM8_UP_IRQn, 0x80); 
    NVIC_EnableIRQ(TIM8_UP_IRQn);
    set_bits(TIM8->CR1, TIM_CR1_CEN);

#else // Old code
    setPulseLED(false);
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(PULSE_LED_PORT, PULSE_LED_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PULSE_LED_PORT, PULSE_LED_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
#endif
}

#if defined(GC03)
// Helper function for setting a static color
static void setStaticColor(uint8_t mask) {
    use_tdm_mode = false;
    
    // First, turn everything off
    PULSE_LED_R_PORT->BSRR = get_bitvalue(PULSE_LED_R_PIN);
    PULSE_LED_G_PORT->BSRR = get_bitvalue(PULSE_LED_G_PIN);
    PULSE_LED_B_PORT->BSRR = get_bitvalue(PULSE_LED_B_PIN);

    // Turn on the required one (Active Low -> BRR)
    if (mask & COLOR_R) PULSE_LED_R_PORT->BRR = get_bitvalue(PULSE_LED_R_PIN);
    if (mask & COLOR_G) PULSE_LED_G_PORT->BRR = get_bitvalue(PULSE_LED_G_PIN);
    if (mask & COLOR_B) PULSE_LED_B_PORT->BRR = get_bitvalue(PULSE_LED_B_PIN);
}

// Helper function for setting a pattern
static void setPattern(const uint8_t* pattern, uint8_t len) {
    NVIC_DisableIRQ(TIM8_UP_IRQn);

    // Copy the pattern to the global buffer
    for(int i=0; i<len; i++) current_pattern[i] = pattern[i];
    current_pattern_len = len;
    pattern_step = 0;
    use_tdm_mode = true;
    
    NVIC_EnableIRQ(TIM8_UP_IRQn);
}

// Static variable to store the current rainbow step
static uint8_t rainbow_step = 0;
#endif

void setPulseLED(bool value)
{
#if defined(GC03) && defined(PULSE_LED)
    if (!value) {
        setStaticColor(COLOR_OFF);
        return;
    }

    // Local variable to determine which color to show now
    uint8_t color_to_show = settings.pulseLEDColor;


    if (color_to_show == PULSE_LED_COLOR_RAINBOW)
    {
        // List of colors to cycle through
        const uint8_t rainbow_sequence[] = {
            PULSE_LED_COLOR_RED,
            PULSE_LED_COLOR_YELLOW,
            PULSE_LED_COLOR_GREEN,
            PULSE_LED_COLOR_CYAN,
            PULSE_LED_COLOR_BLUE,
            PULSE_LED_COLOR_MAGENTA,
            PULSE_LED_COLOR_WHITE,
        };
        
        // Get the color from the list
        color_to_show = rainbow_sequence[rainbow_step];

        // Prepare the index for the next time
        rainbow_step++;
        if (rainbow_step >= sizeof(rainbow_sequence)) {
            rainbow_step = 0;
        }
    }

    // Apply display logic for the selected color
    switch (color_to_show)
    {
    // --- SOLID COLORS (100% Brightness) ---
    case PULSE_LED_COLOR_RED:
        setStaticColor(COLOR_R);
        break;
    case PULSE_LED_COLOR_GREEN:
        setStaticColor(COLOR_G);
        break;
    case PULSE_LED_COLOR_BLUE:
        setStaticColor(COLOR_B);
        break;

    // --- MIXED COLORS (Balance adjustment) ---
    
    // YELLOW
    // Pattern: R -> R -> R -> G (75% Red, 25% Green)
    case PULSE_LED_COLOR_YELLOW: {
        const uint8_t p[] = {COLOR_R, COLOR_R, COLOR_R, COLOR_G}; 
        setPattern(p, 4);
        break;
    }

    // CYAN
    // Pattern: G -> B (50% / 50%)
    case PULSE_LED_COLOR_CYAN: {
        const uint8_t p[] = {COLOR_G, COLOR_B};
        setPattern(p, 2);
        break;
    }

    // MAGENTA
    // Pattern: R -> B (50% / 50%)
    case PULSE_LED_COLOR_MAGENTA: {
        const uint8_t p[] = {COLOR_R, COLOR_B};
        setPattern(p, 2);
        break;
    }

    // WHITE
    // Pattern: R -> R -> G -> B (50% Red, 25% Green, 25% Blue)
    case PULSE_LED_COLOR_WHITE: {
        const uint8_t p[] = {COLOR_R, COLOR_R, COLOR_G, COLOR_B};
        setPattern(p, 4);
        break;
    }
    }

#else // For other devices
#if defined(PULSE_LED_ACTIVE_LOW)
    gpio_modify(PULSE_LED_PORT, PULSE_LED_PIN, !value);
#else
    gpio_modify(PULSE_LED_PORT, PULSE_LED_PIN, value);
#endif
#endif
}

#endif

#if defined(ALERT_LED)

void initAlertLED(void)
{
    // GPIO
    setAlertLED(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(ALERT_LED_PORT, ALERT_LED_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(ALERT_LED_PORT, ALERT_LED_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setAlertLED(bool value)
{
#if defined(ALERT_LED_ACTIVE_LOW)
    gpio_modify(ALERT_LED_PORT, ALERT_LED_PIN, !value);
#else
    gpio_modify(ALERT_LED_PORT, ALERT_LED_PIN, value);
#endif
}

#endif

#if defined(PULSE_LED_EN)

void initPulseLEDEnable(void)
{
    // GPIO
    setPulseLEDEnable(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setPulseLEDEnable(bool value)
{
#if defined(PULSE_LED_EN_ACTIVE_LOW)
    gpio_modify(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN, !value);
#else
    gpio_modify(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN, value);
#endif
}

#endif

#if defined(ALERT_LED_EN)

void initAlertLEDEnable(void)
{
    // GPIO
    setAlertLEDEnable(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setAlertLEDEnable(bool value)
{
#if defined(ALERT_LED_EN_ACTIVE_LOW)
    gpio_modify(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN, !value);
#else
    gpio_modify(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN, value);
#endif
}

#endif

#endif
