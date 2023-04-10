/*
 * FS2011 Pro
 * Sound
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#ifndef SDL_MODE
#include "main.h"

extern TIM_HandleTypeDef htim6;
#endif

#include "buzzer.h"

void triggerBuzzer(uint32_t buzzerTicks)
{
#ifndef SDL_MODE
    __disable_irq();

    uint32_t counter = __HAL_TIM_GET_COUNTER(&htim6);
    uint32_t autoreload = __HAL_TIM_GET_AUTORELOAD(&htim6);

    if ((counter == 0) || ((autoreload - counter) < buzzerTicks))
    {
        HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_SET);

        __HAL_TIM_DISABLE(&htim6);
        __HAL_TIM_SET_COUNTER(&htim6, 0);
        __HAL_TIM_SET_AUTORELOAD(&htim6, buzzerTicks - 1);
        __HAL_TIM_ENABLE(&htim6);
    }

    __enable_irq();
#else
    // printf("Trigger buzzer: %d\n", buzzerTicks);
#endif
}

#ifndef SDL_MODE
void onBuzzerOff()
{
    HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);
}
#endif
