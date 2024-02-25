# FS2011 MCU Pinout

* PA1 (Input, floating): TUBE_DET2 (pulse detector for tube 2)
* PA2 (AF1): USART2_TX
* PA3 (AF1): USART2_RX
* PA4 (Analog, floating): PWR_BAT (battery voltage)
* PA5 (Input, pull-up): PWR_CHRG (charging battery)
* PA6 (Input, pull-up): TUBE_DET (pulse detector for tube 1)
* PA7 (Input, pull-up): KEY_UP
* PA8 (Output): LCD_D0
* PA9 (Output): LCD_D1
* PA10 (Output): LCD_D2
* PA11 (Output): LCD_D3
* PA12 (Output): LCD_D4
* PA13 (SWDIO): SWD data
* PA14 (SWCLK): SWD clock
* PA15 (Output): LCD_D7

* PB0 (Input, pull-up): KEY_DOWN
* PB1 (Input, pull-up): KEY_BACK
* PB2 (Input, pull-up): KEY_SELECT
* PB3 (AF, TIM2 PWM): LCD_BACKLIGHT (cathode)
* PB4 (AF, TIM3 PWM): TUBE_HV (high voltage control)
* PB5 (Output): BUZZ
* PB6 (Output): M_VIB (vibration motor, not connected)
* PB8[GD32F103] (Output): LCD_D5
* PB9[GD32F103] (Output): LCD_D6
* PB10 (Output): PWR_EN (enable power)
* PB11 (Input, pull-up): KEY_POWER
* PB12 (Output): LCD_RSTB
* PB13 (Output): LCD_A0
* PB14 (Output): LCD_RW
* PB15 (Output): LCD_E

* PF6[STM32F051/GD32F150] (Output): LCD_D5
* PF7[STM32F051/GD32F150] (Output): LCD_D6
