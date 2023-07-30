/*
 * Rad Pro
 * STM32 comm interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)

#ifdef STM32F1
#include <libopencm3/stm32/f1/bkp.h>
#endif
#include <libopencm3/stm32/rcc.h>
#ifdef STM32F0
#include <libopencm3/stm32/rtc.h>
#endif

#include "../../comm.h"

void initComm(void)
{
#ifdef STM32F1
    rcc_periph_clock_enable(RCC_BKP);
#endif
}

void sendComm(uint32_t time, uint32_t pulseCount)
{
    // Write time and pulse count in first and second backup registers
#ifdef STM32F0
    RTC_BKPXR(0) = time;
    RTC_BKPXR(1) = pulseCount;
#elif STM32F1
    BKP_DR1 = time;
    BKP_DR2 = pulseCount;
#elif STM32G0
    MMIO32(TAMP_BASE + 0x100) = time;
    MMIO32(TAMP_BASE + 0x104) = pulseCount;
#endif

    // To-do: Send to UART
}

#endif
