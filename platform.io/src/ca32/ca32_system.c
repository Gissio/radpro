/* Rad Pro CA32 startup and system tick. License: MIT */

#if defined(CA32) && defined(H240)

#include <stdint.h>

#include "ca32_gpio.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/system.h"

#define REG32(address) (*(volatile uint32_t *)(address))

#define RCC_CR REG32(0x40021000U)
#define RCC_CFGR REG32(0x40021004U)
#define RCC_CSR REG32(0x40021024U)
#define RCC_HSI_ENABLE (1U << 0)
#define RCC_HSI_READY (1U << 1)
#define RCC_SYSTEM_SOURCE_MASK 3U
#define RCC_SYSTEM_STATUS_MASK (3U << 2)
#define RCC_AHB_APB_DIVIDER_MASK (0x3ffU << 4)
#define RCC_CLEAR_RESET_FLAGS (1U << 24)

#define SYSTICK_CTRL REG32(0xe000e010U)
#define SYSTICK_LOAD REG32(0xe000e014U)
#define SYSTICK_VAL REG32(0xe000e018U)
#define SYSHND_PRIORITY3 REG32(0xe000ed20U)
#define SYSTICK_PRIORITY_FIELD (0xffU << 24)
#define SYSTICK_PRIORITY_NORMAL (0x80U << 24)
#define SYSTICK_CORE_CLOCK (1U << 2)
#define SYSTICK_INTERRUPT (1U << 1)
#define SYSTICK_ENABLE (1U << 0)

extern uint32_t __stack_top;
extern uint32_t __data_load[];
extern uint32_t __data_start[];
extern uint32_t __data_end[];
extern uint32_t __bss_start[];
extern uint32_t __bss_end[];

int main(void);
void Reset_Handler(void);
void SysTick_Handler(void);
void TIM3_IRQHandler(void);

static void failSafe(void)
{
    __asm__ volatile("cpsid i" ::: "memory");
    ca32GpioOutput(CA32_GPIO_A, 15, false);
    for (;;)
        __asm__ volatile("wfi");
}

// External IRQ 16 is TIM3 in the factory vectors. CA32 aliases flash to zero.
__attribute__((section(".isr_vector"), used))
static const uintptr_t vectors[48] = {
    [0] = (uintptr_t)&__stack_top,
    [1] = (uintptr_t)Reset_Handler,
    [2] = (uintptr_t)failSafe,
    [3] = (uintptr_t)failSafe,
    [15] = (uintptr_t)SysTick_Handler,
    [16 + 16] = (uintptr_t)TIM3_IRQHandler,
};

void Reset_Handler(void)
{
    __asm__ volatile("cpsid i" ::: "memory");
    const uint32_t *source = __data_load;
    for (uint32_t *dest = __data_start; dest != __data_end; ++dest)
        *dest = *source++;
    for (uint32_t *dest = __bss_start; dest != __bss_end; ++dest)
        *dest = 0;

    main();
    failSafe();
}

void initGPIO(void)
{
    ca32GpioOutput(CA32_GPIO_A, 15, false);
}

void initSystem(void)
{
    // Use the documented 16 MHz internal clock and the factory HSI selector.
    RCC_CR |= RCC_HSI_ENABLE;
    for (uint32_t timeout = CA32_CORE_CLOCK_HZ; !(RCC_CR & RCC_HSI_READY); --timeout)
        if (timeout == 0)
            failSafe();

    RCC_CFGR &= ~(RCC_SYSTEM_SOURCE_MASK | RCC_AHB_APB_DIVIDER_MASK);
    for (uint32_t timeout = CA32_CORE_CLOCK_HZ; RCC_CFGR & RCC_SYSTEM_STATUS_MASK; --timeout)
        if (timeout == 0)
            failSafe();
}

void initEvents(void)
{
    SYSTICK_CTRL = 0;
    SYSTICK_LOAD = CA32_CORE_CLOCK_HZ / SYSTICK_FREQUENCY - 1;
    SYSTICK_VAL = 0;
    // Let TIM3 sample the detector while the UI tick is running.
    SYSHND_PRIORITY3 = (SYSHND_PRIORITY3 & ~SYSTICK_PRIORITY_FIELD) | SYSTICK_PRIORITY_NORMAL;
    SYSTICK_CTRL = SYSTICK_CORE_CLOCK | SYSTICK_INTERRUPT | SYSTICK_ENABLE;
    __asm__ volatile("cpsie i" ::: "memory");
}

void SysTick_Handler(void)
{
    currentTick++;
    onTick();
}

void sleep(uint32_t ticks)
{
    uint32_t start = currentTick;
    while ((uint32_t)(currentTick - start) < ticks)
        __asm__ volatile("wfi");
}

bool wasResetByWatchdog(void)
{
    // The initial H240 profile does not start a hardware watchdog.
    return false;
}

void clearResetFlags(void)
{
    RCC_CSR |= RCC_CLEAR_RESET_FLAGS;
}

#endif
