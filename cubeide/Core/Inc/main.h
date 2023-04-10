/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GM_DET2_Pin GPIO_PIN_1
#define GM_DET2_GPIO_Port GPIOA
#define PWR_BAT_Pin GPIO_PIN_4
#define PWR_BAT_GPIO_Port GPIOA
#define PWR_CHRG_Pin GPIO_PIN_5
#define PWR_CHRG_GPIO_Port GPIOA
#define GM_DET_Pin GPIO_PIN_6
#define GM_DET_GPIO_Port GPIOA
#define GM_DET_EXTI_IRQn EXTI4_15_IRQn
#define KEY_UP_Pin GPIO_PIN_7
#define KEY_UP_GPIO_Port GPIOA
#define KEY_DOWN_Pin GPIO_PIN_0
#define KEY_DOWN_GPIO_Port GPIOB
#define KEY_SELECT_Pin GPIO_PIN_1
#define KEY_SELECT_GPIO_Port GPIOB
#define KEY_BACK_Pin GPIO_PIN_2
#define KEY_BACK_GPIO_Port GPIOB
#define PWR_EN_Pin GPIO_PIN_10
#define PWR_EN_GPIO_Port GPIOB
#define KEY_POWER_Pin GPIO_PIN_11
#define KEY_POWER_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_12
#define LCD_RESET_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOB
#define LCD_RW_Pin GPIO_PIN_14
#define LCD_RW_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_15
#define LCD_EN_GPIO_Port GPIOB
#define LCD_D0_Pin GPIO_PIN_8
#define LCD_D0_GPIO_Port GPIOA
#define LCD_D1_Pin GPIO_PIN_9
#define LCD_D1_GPIO_Port GPIOA
#define LCD_D2_Pin GPIO_PIN_10
#define LCD_D2_GPIO_Port GPIOA
#define LCD_D3_Pin GPIO_PIN_11
#define LCD_D3_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_12
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_6
#define LCD_D5_GPIO_Port GPIOF
#define LCD_D6_Pin GPIO_PIN_7
#define LCD_D6_GPIO_Port GPIOF
#define LCD_D7_Pin GPIO_PIN_15
#define LCD_D7_GPIO_Port GPIOA
#define LCD_BLK_Pin GPIO_PIN_3
#define LCD_BLK_GPIO_Port GPIOB
#define GM_HV_Pin GPIO_PIN_4
#define GM_HV_GPIO_Port GPIOB
#define BUZZ_Pin GPIO_PIN_5
#define BUZZ_GPIO_Port GPIOB
#define M_VIB_Pin GPIO_PIN_6
#define M_VIB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern TIM_HandleTypeDef htim2;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
