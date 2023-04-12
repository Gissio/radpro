/*
 * FS2011 Pro
 * Power management
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef SDL_MODE
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc;
extern IWDG_HandleTypeDef hiwdg;
#endif

#include "display.h"
#include "power.h"
#include "settings.h"

#define BATTERY_NUM 2

#define BATTERY_NI_MH_VOLTAGE_MAX 1.3F
#define BATTERY_NI_MH_VOLTAGE_MIN 1.2F
#define BATTERY_ALKALINE_VOLTAGE_MAX 1.4F
#define BATTERY_ALKALINE_VOLTAGE_MIN 1.1F

#define ADC_VOLTAGE_MAX 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)
#define ADC_FACTOR (BATTERY_NUM * ADC_VALUE_MAX / ADC_VOLTAGE_MAX)

#define BATTERY_NI_MH_VALUE_MIN (ADC_FACTOR * BATTERY_NI_MH_VOLTAGE_MIN)
#define BATTERY_NI_MH_VALUE_RANGE (ADC_FACTOR * (BATTERY_NI_MH_VOLTAGE_MAX - BATTERY_NI_MH_VOLTAGE_MIN))
#define BATTERY_ALKALINE_VALUE_MIN (ADC_FACTOR * BATTERY_ALKALINE_VOLTAGE_MIN)
#define BATTERY_ALKALINE_VALUE_RANGE (ADC_FACTOR * (BATTERY_ALKALINE_VOLTAGE_MAX - BATTERY_ALKALINE_VOLTAGE_MIN))

// First order filter (N: time constant in taps): k = e^(-1 / N)
// For N = 60 (seconds):
#define BATTERY_FILTER_CONSTANT 0.98347F

struct Power
{
    float batteryValue;
} power;

void setPower(bool value)
{
#ifndef SDL_MODE
    HAL_GPIO_WritePin(PWR_EN_GPIO_Port, PWR_EN_Pin, value);
#else
    printf("Set power: %d\n", value);
#endif
}

void setHighVoltageGenerator(bool value)
{
#ifndef SDL_MODE
    if (value)
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    else
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
#else
    printf("Set high voltage generator: %d\n", value);
#endif
}

uint32_t getBatteryValue()
{
#ifndef SDL_MODE
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 2);
    int value = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
    return value;
#else
    return (uint32_t)(ADC_FACTOR * 1.27F);
#endif
}

void initPower()
{
    setPower(true);

#ifndef SDL_MODE
    HAL_ADCEx_Calibration_Start(&hadc);
#endif

    power.batteryValue = (float)getBatteryValue();

    setHighVoltageGenerator(true);
}

void waitForInterrupt()
{
#ifndef SDL_MODE
    // HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    __WFI();
#endif
}

void updateWatchdog()
{
#ifndef SDL_MODE
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

void powerWait(uint32_t delay)
{
#ifndef SDL_MODE
    int n = (delay + 99) / 100;
    for (int i = 0; i < n; i++)
    {
        HAL_Delay(100);
        updateWatchdog();
    }
#endif
}

void powerHalt()
{
    while (true)
    {
#ifndef SDL_MODE
        updateWatchdog();
#endif
    }
}

void powerDown()
{
    setBacklight(false);
    setDisplay(false);
    setPower(false);
    powerHalt();
}

void updateBattery()
{
    power.batteryValue = (BATTERY_FILTER_CONSTANT * power.batteryValue +
                          (1.0F - BATTERY_FILTER_CONSTANT) * getBatteryValue());
}

uint8_t getBatteryLevel()
{
#ifndef SDL_MODE
    if (!HAL_GPIO_ReadPin(PWR_CHRG_GPIO_Port, PWR_CHRG_Pin))
        return BATTERY_LEVEL_CHARGING;
#endif

    int value;
    switch (settings.batteryType)
    {
    case BATTERY_NI_MH:
        value = (int)(BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_NI_MH_VALUE_MIN) /
                      BATTERY_NI_MH_VALUE_RANGE);
        break;

    case BATTERY_ALKALINE:
        value = (int)(BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_ALKALINE_VALUE_MIN) /
                      BATTERY_ALKALINE_VALUE_RANGE);
        break;

    default:
        value = 0;
        break;
    }

    return (value < 0) ? 0 : ((value > BATTERY_LEVEL_MAX) ? BATTERY_LEVEL_MAX : value);
}
