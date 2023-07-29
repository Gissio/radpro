# FS2011 display

## Command list

0x  Set column address LSB x
1x  Set column address MSB x

2x  Select internal resistor ratio(Rb/Ra) mode (0 to 7) [3]
2c  Power control: 100, enable internal voltage booster
2e  Power control: 110, enable internal voltage regulator
2f  Power control: 111, enable internal voltage follower

4x  Specify DDRAM line x for COM0

81  Set contrast + 00-3f contrast

a0  Set SEG output direction: normal
a1  Set SEG output direction: reverse
a2  Set LCD bias: 1/9 bias
a3  Set LCD bias: 1/7 bias
a4  Display all points: normal display
a5  Display all points: all points on
a6  Sets the LCD display normal/reverse: normal
a7  Sets the LCD display normal/reverse: reverse
ac  Set static indicator mode: off
ad  Set static indicator mode: on
ae  Turn LCD display: off
af  Turn LCD display: on

bx  Set page address x

c0  COM output scan direction: normal direction
c8  COM output scan direction: reverse direction

e0  Set modify-read mode
e2  Software reset
e3  NOP
ec  Flashing off
ed  Flashing on
ee  Release modify-read mode
