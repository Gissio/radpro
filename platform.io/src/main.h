/*
 * FS2011 Pro
 * STM32 Main module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef MAIN_H
#define MAIN_H

#ifndef SDL_MODE

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/crc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/cm3/dwt.h>

#define PWR_EN_PORT GPIOB
#define PWR_EN_PIN GPIO10
#define PWR_BAT_PORT GPIOA
#define PWR_BAT_PIN GPIO4
#define PWR_CHRG_PORT GPIOA
#define PWR_CHRG_PIN GPIO5

#define GM_HV_PORT GPIOB
#define GM_HV_PIN GPIO4
#define GM_DET_PORT GPIOA
#define GM_DET_PIN GPIO6
#define GM_DET2_PORT GPIOA
#define GM_DET2_PIN GPIO1

#define KEY_POWER_PORT GPIOB
#define KEY_POWER_PIN GPIO11
#define KEY_SELECT_PORT GPIOB
#define KEY_SELECT_PIN GPIO1
#define KEY_BACK_PORT GPIOB
#define KEY_BACK_PIN GPIO2
#define KEY_UP_PORT GPIOA
#define KEY_UP_PIN GPIO7
#define KEY_DOWN_PORT GPIOB
#define KEY_DOWN_PIN GPIO0

#define LCD_RESET_PORT GPIOB
#define LCD_RESET_PIN GPIO12
#define LCD_EN_PORT GPIOB
#define LCD_EN_PIN GPIO15
#define LCD_RS_PORT GPIOB
#define LCD_RS_PIN GPIO13
#define LCD_RW_PORT GPIOB
#define LCD_RW_PIN GPIO14
#define LCD_D0_PORT GPIOA
#define LCD_D0_PIN GPIO8
#define LCD_D1_PORT GPIOA
#define LCD_D1_PIN GPIO9
#define LCD_D2_PORT GPIOA
#define LCD_D2_PIN GPIO10
#define LCD_D3_PORT GPIOA
#define LCD_D3_PIN GPIO11
#define LCD_D4_PORT GPIOA
#define LCD_D4_PIN GPIO12
#define LCD_D5_PORT GPIOF
#define LCD_D5_PIN GPIO6
#define LCD_D6_PORT GPIOF
#define LCD_D6_PIN GPIO7
#define LCD_D7_PORT GPIOA
#define LCD_D7_PIN GPIO15
#define LCD_BACKLIGHT_PORT GPIOB
#define LCD_BACKLIGHT_PIN GPIO3

#define BUZZ_PORT GPIOB
#define BUZZ_PIN GPIO5

#define M_VIB_PORT GPIOB
#define M_VIB_PIN GPIO6

#else

#include <stdio.h>

#endif

#endif
