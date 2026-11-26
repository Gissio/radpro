/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../stm32/device.h"
#include "../system/cmath.h"
#include "../system/cstring.h"
#include "../system/power.h"

#if defined(DEBUG)

#include "../peripherals/buzzer.h"
#include "../peripherals/led.h"
#include "../system/events.h"

#define DEBUG_BIT_LENGTH 100
#define DEBUG_BIT0_LENGTH 15
#define DEBUG_BIT1_LENGTH 1

uint32_t getGPIO(uint32_t index)
{
    switch (index)
    {
    case 0:
        return GPIOA->IDR;

    case 1:
        return GPIOB->IDR;

    case 2:
        return GPIOC->IDR;

    case 3:
        return GPIOD->IDR;

#if defined(GPIOE)
    case 4:
        return GPIOE->IDR;
#endif

#if defined(GPIOF)
    case 5:
        return GPIOF->IDR;
#endif

    default:
        return 0;
    }
}

void debugWait(uint32_t ms)
{
    uint32_t n = (SYSCLK_FREQUENCY / 8000000) * 200 * ms;

    for (uint32_t i = 0; i < n; i++)
        reloadWatchdog();
}

static void setDebugSignal(bool value)
{
#if defined(BUZZER)
    setBuzzer(value);
#endif
#if defined(PULSE_LED)
    setPulseLED(value);
#endif
}

void debugBit(bool value)
{
    uint32_t onTime = value ? DEBUG_BIT1_LENGTH : DEBUG_BIT0_LENGTH;

    setDebugSignal(true);
    debugWait(onTime);
    setDebugSignal(false);
    debugWait(DEBUG_BIT_LENGTH - onTime);
}

void debugUInt32(uint32_t value)
{
    debugWait(500);

    for (uint32_t j = 0; j < 32; j++)
    {
        debugBit(value >> 31);
        value <<= 1;
    }
}

// Referenced only from HardFault_Handler's inline assembly, which LTO cannot see
__attribute__((used)) void onHardFault(const uint32_t *args)
{
    // args: R0, R1, R2, R3, R12, LR, PC, PSR

    // Send R12, LR, PC, PSR
    for (uint32_t i = 4; i < 8; i++)
        debugUInt32(args[i]);

    setPowerEnabled(false);

    while (true)
        ;
}

// Naked: no prologue may move SP before the stacked frame is captured
__attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile(
        " movs r0, #4           \n"
        " mov r1, lr            \n"
        " tst r0, r1            \n"
        " mrs r0, msp           \n"
        " beq 1f                \n"
        " mrs r0, psp           \n"
        "1:                     \n"
        " bl onHardFault        \n");
}
#endif

uint32_t prescalePWMParameters(uint32_t *period, uint32_t *onTime)
{
    uint32_t localPeriod = *period;
    uint32_t localOnTime = *onTime;

    // Get prescaler factor (if it divides the period, fall back to 1 so ARR != 0)
    uint32_t prescalerFactor = getGCD(localPeriod, localOnTime);
    if (prescalerFactor == localPeriod)
        prescalerFactor = 1;
    localPeriod /= prescalerFactor;
    localOnTime /= prescalerFactor;

    // Scale prescaler factor (custom HV frequencies go down to 100 Hz)
    while (prescalerFactor >> 16)
    {
        localPeriod <<= 1;
        localOnTime <<= 1;
        prescalerFactor >>= 1;
    }

    // Scale period
    while (localPeriod >> 16)
    {
        localPeriod >>= 1;
        localOnTime >>= 1;
        prescalerFactor <<= 1;
    }

    *period = localPeriod;
    *onTime = localOnTime;

    return prescalerFactor;
}

void getDeviceId(char *s)
{
    *s = '\0';
    for (uint32_t i = 0; i < 4 * 3; i++)
        strcatUInt8Hex(s, ((uint8_t *)(&UID0))[i]);
}

#endif
