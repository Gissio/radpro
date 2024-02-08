# Bosean FS-600 128x64 LCD display (ST7567)

## Initialization sequence

* lcd_clear_en();
* sleep(20);
* lcd_set_en();
* sleep(20);
* lcd_send_command(0xe2); // Software reset
* lcd_send_command(0xa4); // Display all points: normal display
* lcd_send_command(0xe6); // Sets the LCD display normal/reverse: normal
* lcd_send_command(0xa2); // Set LCD bias: 1/9 bias
* lcd_send_command(0xf8); // Set internal booster...
* lcd_send_command(0x00); // ... to 4x
* lcd_send_command(0xa0); // Set SEG output direction: normal
* lcd_send_command(0xc8); // COM output scan direction: reverse direction
* lcd_send_command(0x2c); // Power control: 100, enable internal voltage booster
* lcd_send_command(0x2e); // Power control: 110, enable internal voltage regulator
* lcd_send_command(0x2f); // Power control: 111, enable internal voltage follower
* lcd_send_command(0x24); // Select internal resistor ratio(Rb/Ra) mode (0 to 7) [4]
* lcd_send_command(0x81); // Set contrast
* lcd_send_command(0x20); // Set contrast value: 00-3f contrast
* lcd_send_command(0x40); // Specify DDRAM line x for COM0 [0]
* lcd_send_command(0xae); // Turn LCD display: off
