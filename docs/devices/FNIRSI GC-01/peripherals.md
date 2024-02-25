# FNIRSI GC-01 MCU Pinout

* PA0 (Input): PWR_BAT (divided by 2)
* PA1 (Input, pull-down): KEY_POWER
* PA2 (Output): PWR_EN (set to 1 on power on)
* PA3 (AF): TUBE_HV
* PA4 (Input): PWR_CHRG
* PA8 (PWM): BUZZ (PWM controlled by TIM1)
* PA9 (PWM): M_VIB (PWM controlled by TIM1)
* PA10 (PWM): LCD_BACKLIGHT (PWM controlled by TIM1)
* PA13 (AF): SWDIO
* PA14 (AF): SWCLK
* PA15 (Output): PULSE_LED (set to 0 on power on)

* PB0-PB15 (Output): LCD_DATA

* PC0 (Input, pull-down): KEY_UP
* PC1 (Input, pull-down): KEY_RIGHT
* PC2 (Input, pull-down): KEY_DOWN
* PC3 (Input, pull-down): KEY_LEFT
* PC5 (Output): LCD_RESX
* PC6 (Output): LCD_RDX (Initialized to 1 on start)
* PC7 (Output): LCD_WRX (set to 0 before writing)
* PC8 (Output): LCD_DCX (0: command, 1: data)
* PC9 (Output): LCD_CSX (set to 0 before writing)
* PC10 (Input, pull-down): TUBE_DET
