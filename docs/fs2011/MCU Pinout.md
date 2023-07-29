# FS2011 MCU Pinout

## Power

PB10 (push-pull output): PWR_EN (enable power)
PA5 (pull-up input): PWR_CHRG (charging battery)
PA4 (analog floating input): PWR_BAT (battery voltage)

## LSE crystal (32.768 kHz)

PC14-OSC32_IN
PC15-OSC32_OUT

## SWD (Serial Wire Debug)

PA14: SWCLK (serial wire clock)
PA13: SWDIO (serial wire data I/O)

## Geiger-Müller radiation detector

PB4 (TIM3 PWM output): GM_HV_CTRL (high-voltage control)
PA6 (pull-up input): GM_DET (pulse detector for tube 1)
PA1 (floating input): GM_DET2 (pulse detector for tube 2)

## Keyboard

PB11 (pull-up input): KEY_POWER
PA7 (pull-up input): KEY_UP
PB0 (pull-up input): KEY_DOWN
PB2 (pull-up input): KEY_SELECT
PB1 (pull-up input): KEY_BACK

## Display

PB12 (push-pull output): LCD_RW
PB13 (push-pull output): LCD_RS
PB14 (push-pull output): LCD_RESET
PB15 (push-pull output): LCD_EN

PA8 (push-pull output): LCD_D0
PA9 (push-pull output): LCD_D1
PA10 (push-pull output): LCD_D2
PA11 (push-pull output): LCD_D3
PA12 (push-pull output): LCD_D4
PF6/PB8[GD32F103] (push-pull output): LCD_D5
PF7/PB9[GD32F103] (push-pull output): LCD_D6
PA15 (push-pull output): LCD_D7

PB3 (TIM2 PWM output): LCD_BACKLIGHT (cathode)

## Buzzer

PB5 (push-pull output): BUZZ_OUT

## Other

PB6 (push-pull output): M_VIB (vibration motor, not connected)
