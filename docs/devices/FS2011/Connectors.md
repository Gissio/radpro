# FS2011 connectors

## 4-pin SWD connector

From top to bottom:

* Pin 1: GND
* Pin 2: SWCLK (MCU PA14)
* Pin 3: SWDIO (MCU PA13)
* Pin 4: +3V3

## 6-pin keyboard connector

From left to right (on PCB, from side opposite to display):

* Pin 1: GND
* Pin 2: KEY_UP (to PA7)
* Pin 3: KEY_DOWN (to PB0)
* Pin 4: KEY_BACK (to PB2)
* Pin 5: KEY_SELECT (to PB1)
* Pin 6: KEY_POWER (to PB11 via D13 (digital input) & PROG)

## 22-pin display connector

From left to right (on PCB, from side opposite to display):

* Pin 1: DISPLAY_3V3
* Pin 2: DISPLAY_3V3
* Pin 3: not connected (?)
* Pin 4: not connected (?)
* Pin 5: DISPLAY_GND
* Pin 6: not connected (?)
* Pin 7: DISPLAY_3V3
* Pin 8: DISPLAY_D7 (to PA15)
* Pîn 9: DISPLAY_D6 (to PF7 on STM32F051 and GD32F150, to PB8 on GD32F103)
* Pin 10: DISPLAY_D5 (to PF6 on STM32F051 and GD32F150, to PB9 on GD32F103)
* Pin 11: DISPLAY_D4 (to PA12)
* Pin 12: DISPLAY_D3 (to PA11)
* Pin 13: DISPLAY_D2 (to PA10)
* Pin 14: DISPLAY_D1 (to PA9)
* Pin 15: DISPLAY_D0 (to PA8)
* Pin 16: DISPLAY_E
 (to PB15)
* Pin 17: DISPLAY_RSTB (to PB14)
* Pin 18: DISPLAY_A0 (to PB13)
* Pin 19: DISPLAY_BLK (to PB3)
* Pin 20: DISPLAY_BLA (+3V3)
* Pin 21: DISPLAY_RW (to PB12)
* Pin 22: DISPLAY_GND
