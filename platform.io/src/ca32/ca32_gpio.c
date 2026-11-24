/* Rad Pro CA32 GPIO. License: MIT */

#if defined(CA32) && defined(H240)

#include <stdint.h>

#include "ca32_gpio.h"

#define GPIO_BASE 0x40022000U
#define GPIO_PORT_STRIDE 0x40U
#define GPIO_OUTPUT_BASE (GPIO_BASE + 0xc0U)
#define GPIO_INPUT_BASE (GPIO_BASE + 0xd0U)

// CA32F01XX Table 3-2 and factory pad configurations read via MEM-AP.
#define GPIO_CONFIG_MASK 0xffffU
#define GPIO_INPUT_MODE 1U
#define GPIO_OUTPUT_MODE 2U
#define GPIO_PULL_UP (1U << 7)
#define GPIO_SPEED (3U << 8)
#define GPIO_ALT_OUTPUT (1U << 13)
#define GPIO_PAD_ENABLE (1U << 14)

static void configurePin(unsigned port, unsigned pin, uint32_t config)
{
    volatile uint32_t *reg = (volatile uint32_t *)(GPIO_BASE + port * GPIO_PORT_STRIDE + pin * 4U);
    *reg = (*reg & ~GPIO_CONFIG_MASK) | GPIO_PAD_ENABLE | GPIO_SPEED | config;
}

void ca32GpioOutput(unsigned port, unsigned pin, bool initialHigh)
{
    ca32GpioWrite(port, pin, initialHigh);
    configurePin(port, pin, GPIO_OUTPUT_MODE);
}

void ca32GpioInput(unsigned port, unsigned pin, bool pullUp)
{
    configurePin(port, pin, GPIO_INPUT_MODE | (pullUp ? GPIO_PULL_UP : 0U));
}

void ca32GpioAlternate(unsigned port, unsigned pin, unsigned function)
{
    uint32_t config = function | GPIO_PULL_UP;
    if (port == CA32_GPIO_C && (pin == 6U || pin == 8U) && function == 5U)
        config |= GPIO_ALT_OUTPUT;

    configurePin(port, pin, config);
}

void ca32GpioAnalog(unsigned port, unsigned pin, unsigned function)
{
    configurePin(port, pin, function);
}

void ca32GpioWrite(unsigned port, unsigned pin, bool high)
{
    volatile uint32_t *reg = (volatile uint32_t *)(GPIO_OUTPUT_BASE + port * 4U);
    uint32_t bit = 1U << pin;
    uint32_t value = *reg;
    *reg = high ? (value | bit) : (value & ~bit);
}

bool ca32GpioRead(unsigned port, unsigned pin)
{
    const volatile uint32_t *reg = (const volatile uint32_t *)(GPIO_INPUT_BASE + port * 4U);
    return (*reg & (1U << pin)) != 0;
}

#endif
