/*
 * FS2011 Pro
 * Power management
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdio.h>

#ifndef SDL_MODE
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc;
extern IWDG_HandleTypeDef hiwdg;
#endif

#include "backlight.h"
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
// For N = 30 (seconds):
#define BATTERY_FILTER_CONSTANT 0.967F

struct Power
{
    int batteryState;
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

float getBatteryValue()
{
#ifndef SDL_MODE
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 0);
    return HAL_ADC_GetValue(&hadc);
#else
    return (ADC_FACTOR * 1.27F);
#endif
}

void initPower()
{
    setPower(true);
    setBacklight(false);
    setHighVoltageGenerator(true);

#ifndef SDL_MODE
    HAL_ADCEx_Calibration_Start(&hadc);
#endif
}

void waitForInterrupt()
{
#ifndef SDL_MODE
    __WFI();
#endif
}

void updateWatchdog()
{
#ifndef SDL_MODE
    HAL_IWDG_Refresh(&hiwdg);
#endif
}

void powerDown(int ms)
{
#ifndef SDL_MODE
    uint32_t startTick = uwTick;
#endif

    while (true)
    {
#ifndef SDL_MODE
        waitForInterrupt();

        int deltaTime = (uwTick - startTick);
        if (deltaTime >= ms)
        {
            setBacklight(false);
            setDisplay(false);
            setPower(false);
        }
        else
            updateWatchdog();
#endif
    }
}

void updateBattery()
{
    switch (power.batteryState)
    {
    case 0:
        power.batteryState++;
        power.batteryValue = getBatteryValue();
        break;

    case 1:
        power.batteryValue = (BATTERY_FILTER_CONSTANT * power.batteryValue +
                              (1.0F - BATTERY_FILTER_CONSTANT) * getBatteryValue());
        break;
    }
}

signed char getBatteryLevel()
{
    if (power.batteryState == 0)
        return -1;

#ifndef SDL_MODE
    if (!HAL_GPIO_ReadPin(PWR_CHRG_GPIO_Port, PWR_CHRG_Pin))
        return BATTERY_LEVEL_CHARGING;
#endif

    int value;
    switch (settings.batteryType)
    {
    case BATTERY_NI_MH:
        value = BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_NI_MH_VALUE_MIN) /
                BATTERY_NI_MH_VALUE_RANGE;
        break;

    case BATTERY_ALKALINE:
        value = BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_ALKALINE_VALUE_MIN) /
                BATTERY_ALKALINE_VALUE_RANGE;
        break;

    default:
        value = 0;
        break;
    }

    return (value < 0) ? 0 : ((value > BATTERY_LEVEL_MAX) ? BATTERY_LEVEL_MAX : value);
}
