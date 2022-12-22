/*
 * FS2011 Pro
 * Backlight
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#ifndef SDL_MODE
#include "main.h"

extern TIM_HandleTypeDef htim2;
#endif

#include "backlight.h"

struct {
    bool isOn;
} backlight;

void setBacklight(bool value)
{
#ifndef SDL_MODE
    if (value)
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    else
        HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
#else
    // printf("Set backlight: %d\n", value);
#endif
    backlight.isOn = value;
}

bool isBacklightOn()
{
    return backlight.isOn;
}
