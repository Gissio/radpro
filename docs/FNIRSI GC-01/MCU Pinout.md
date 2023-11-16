# FNIRSI GC-01 MCU Pinout

* PA0 (Input): PWR_BAT
* PA1 (Input, pull-down): KEY_?
* PA2 (Output): PWR_EN? (set to 1 on power on)
* PA3 (AF): TUBE_HV
* PA4 (Input): PWR_CHRG?
* PA8-10 (PWM): PWM_? (PWM controlled by TIM1)
* PA13 (AF): SWDIO
* PA14 (AF): SWCLK
* PA15 (Output): BUZZ? (set to 0 on power on)

* PB0-PB15 (Output): LCD_DATA

* PC0 (Input, pull-down): KEY_?
* PC1 (Input, pull-down): KEY_?
* PC2 (Input, pull-down): KEY_?
* PC3 (Input, pull-down): KEY_?
* PC5 (Output): LCD_RESET
* PC6 (Output): LCD_CS (Initialized to 1 on start)
* PC7 (Output): LCD_EN2 (set to 0 before writing)
* PC8 (Output): LCD_RS (0: command, 1: data)
* PC9 (Output): LCD_EN1 (set to 0 before writing)
* PC10 (Input, pull-down): TUBE_DET
