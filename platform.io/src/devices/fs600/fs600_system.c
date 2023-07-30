/*
 * Rad Pro
 * FS2011 system interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS600

#include <libopencm3/stm32/rcc.h>

#include "../../system.h"

void initSystem(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
}

#endif
