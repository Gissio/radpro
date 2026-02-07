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

#include "../devices/buzzer.h"
#include "../devices/led.h"
#include "../system/events.h"

#define DEBUG_BIT_LENGTH 100
#define DEBUG_BIT0_LENGTH 1
#define DEBUG_BIT1_LENGTH 15

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
        resetWatchdog();
}

void debugBit(bool value)
{
#if defined(BUZZER)
    setBuzzer(true);
#endif
#if defined(PULSE_LED)
    setPulseLED(true);
#endif
    if (value)
        debugWait(DEBUG_BIT0_LENGTH);
    else
        debugWait(DEBUG_BIT1_LENGTH);

#if defined(BUZZER)
    setBuzzer(false);
#endif
#if defined(PULSE_LED)
    setPulseLED(false);
#endif
    if (value)
        debugWait(DEBUG_BIT_LENGTH - DEBUG_BIT0_LENGTH);
    else
        debugWait(DEBUG_BIT_LENGTH - DEBUG_BIT1_LENGTH);
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

void onHardFault(const uint32_t *args)
{
    // args: R0, R1, R2, R3, R12, LR, PC, PSR

    // Send R12, LR, PC, PSR
    for (uint32_t i = 4; i < 8; i++)
        debugUInt32(args[i]);

    setPowerEnabled(false);

    while (true)
        ;
}

void HardFault_Handler(void)
{
    __asm volatile(
        " movs r0, #4           \n"
        " mov r1, lr            \n"
        " tst r0, r1            \n"
        " bne HardFault_PSP     \n"
        " mrs r0, msp           \n"
        " b HardFault_call      \n"
        "HardFault_PSP:         \n"
        " mrs r0, psp           \n"
        "HardFault_call:        \n"
        " ldr r2,=onHardFault   \n"
        " bx r2                 \n");
}
#endif

uint32_t prescalePWMParameters(uint32_t *period, uint32_t *onTime)
{
    uint32_t localPeriod = *period;
    uint32_t localOnTime = *onTime;

    // Get presacler factor
    uint32_t prescalerFactor = getGCD(localPeriod, localOnTime);
    if (prescalerFactor == localPeriod)
        prescalerFactor >>= 1;
    localPeriod /= prescalerFactor;
    localOnTime /= prescalerFactor;

    // Scale prescaler factor
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
