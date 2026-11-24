/* Rad Pro CA32 GPIO. License: MIT */

#ifndef CA32_GPIO_H
#define CA32_GPIO_H

#include <stdbool.h>

#define CA32_CORE_CLOCK_HZ 16000000U

enum
{
    CA32_GPIO_A,
    CA32_GPIO_B,
    CA32_GPIO_C,
};

void ca32GpioOutput(unsigned port, unsigned pin, bool initialHigh);
void ca32GpioInput(unsigned port, unsigned pin, bool pullUp);
void ca32GpioAlternate(unsigned port, unsigned pin, unsigned function);
void ca32GpioAnalog(unsigned port, unsigned pin, unsigned function);
void ca32GpioWrite(unsigned port, unsigned pin, bool high);
bool ca32GpioRead(unsigned port, unsigned pin);

#endif
