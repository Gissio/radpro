/*
 * Rad Pro
 * Simulator debugging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdint.h>

uint32_t getGPIO(uint32_t gpio_index)
{
    switch (gpio_index)
    {
    case 0:
        return 0x1234567;

    case 1:
        return 0x89abcdef;

    case 2:
        return 0x02468ace;

    case 3:
        return 0x13579bdf;

    default:
        return 0;
    }
}

void debugWait(uint32_t ms)
{
}

#endif
