/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "stm32.h"

#include "../buzzer.h"
#include "../display.h"
#include "../events.h"
#include "../system.h"
#include "../tube.h"

#include "device.h"

typedef struct
{
    uint32_t sp;
    void (*onReset)(void);
} VectorTable;

#if defined(STM32F0)
#define SYSTEM_VECTOR_TABLE ((VectorTable *)0x1fffec00)
#elif defined(STM32F1)
#define SYSTEM_VECTOR_TABLE ((VectorTable *)0x1ffff000)
#elif defined(STM32G0) || defined(STM32L4)
#define SYSTEM_VECTOR_TABLE ((VectorTable *)0x1fff0000)
#endif

uint32_t getDeviceId(void)
{
    return UID0 ^ UID1 ^ UID2;
}

void startBootloader(void)
{
    __disable_irq();
    NVIC_DisableAllIRQs();

    // Disable SysTick
    SysTick->CTRL = 0;

    __enable_irq();
    __set_MSP(SYSTEM_VECTOR_TABLE->sp);

    SYSTEM_VECTOR_TABLE->onReset();
}

#endif
