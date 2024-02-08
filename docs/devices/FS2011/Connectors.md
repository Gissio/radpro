# FS2011 connectors

## 4-pin SWD connector

From top to bottom:

* Pin 1: GND
* Pin 2: SWCLK (MCU PA14)
* Pin 3: SWDIO (MCU PA13)
* Pin 4: +3V3

## 6-pin keyboard connector

From left to right (on PCB, from side opposite to LCD):

* Pin 1: GND
* Pin 2: KEY_UP (to PA7)
* Pin 3: KEY_DOWN (to PB0)
* Pin 4: KEY_BACK (to PB2)
* Pin 5: KEY_SELECT (to PB1)
* Pin 6: KEY_POWER (to PB11 via D13 (digital input) & PROG)

## 22-pin LCD connector

From left to right (on PCB, from side opposite to LCD):

* Pin 1: LCD_3V3
* Pin 2: LCD_3V3
* Pin 3: not connected (?)
* Pin 4: not connected (?)
* Pin 5: LCD_GND
* Pin 6: not connected (?)
* Pin 7: LCD_3V3
* Pin 8: LCD_D7 (to PA15)
* Pîn 9: LCD_D6 (to PF7 on STM32F051 and GD32F150, to PB8 on GD32F103)
* Pin 10: LCD_D5 (to PF6 on STM32F051 and GD32F150, to PB9 on GD32F103)
* Pin 11: LCD_D4 (to PA12)
* Pin 12: LCD_D3 (to PA11)
* Pin 13: LCD_D2 (to PA10)
* Pin 14: LCD_D1 (to PA9)
* Pin 15: LCD_D0 (to PA8)
* Pin 16: LCD_E
 (to PB15)
* Pin 17: LCD_RSTB (to PB14)
* Pin 18: LCD_A0 (to PB13)
* Pin 19: LCD_BLK (to PB3)
* Pin 20: LCD_BLA (+3V3)
* Pin 21: LCD_RW (to PB12)
* Pin 22: LCD_GND
