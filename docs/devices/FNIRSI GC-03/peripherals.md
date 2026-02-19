# FNIRSI GC-03 MCU Pinout

* PA2 (AF): TUBE_PWM
* PA3 (Input, pull-up): TUBE_DET
* PA5 (AF): SPI_SCK
* PA6 (AF): SPI_MISO
* PA7 (AF): SPI_MOSI
* PA9 (Input, pull-up): KEY_POWER
* PA10 (Output): PWR_EN
* PA11 (AF): USB_DM
* PA12 (AF): USB_DP

* PB0 (Output): DISPLAY_CSX
* PB1 (Output): M_VIB
* PB5 (Input, pull-up): KNOB_A
* PB6 (Input, pull-up): KNOB_BUTTON
* PB7 (Input, pull-up): KNOB_B
* PB8 (Input, pull-up): KEY_ENTER
* PB9 (Input, pull-up): KEY_BACK
* PB10 (AF, USART3): USART_TX
* PB11 (Input, USART3): USART_RX
* PB12 (Output): (low)
* PB13 (AF): DISPLAY_BACKLIGHT
* PB14 (Output): EMFMETER_EN (inverted)
* PB15 (Output): PULSE_LED

* PC0 (Analog): PWR_BAT (divided by 2)
* PC2 (Analog): EMFMETER_E
* PC3 (Analog): EMFMETER_H
* PC4 (Output): DISPLAY_DCX
* PC5 (Output): DISPLAY_RESX
* PC6 (Input, pull-up): PWR_USB (inverted)
* PC7 (Input, pull-up): PWR_CHRG (inverted)
* PC11 (AF): BUZZ
