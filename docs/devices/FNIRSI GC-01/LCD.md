# FNIRSI GC-01 240x320 LCD display (ST7789V)

## Initialization sequence

* lcd_set_reset();
* sleep(10);
* lcd_clear_reset();
* sleep(20);
* lcd_set_reset();
* sleep(20);

* lcd_send_command(0x11);   // SLPOUT: Sleep out
* sleep(120)
* lcd_send_command(0x36);   // MADCTL: Memory access control
* lcd_send_data(0x0);
* lcd_send_command(0x3a);   // COLMOD: Interface pixel format
* lcd_send_data(0x5);       // Set: 0x5 (16 bit/pixel) (should be 0x55)
* lcd_send_command(0xb2);   // PORCTRL: Porch setting
* lcd_send_data(0xc);       // Default value
* lcd_send_data(0xc);
* lcd_send_data(0x0);
* lcd_send_data(0x33);
* lcd_send_data(0x33);
* lcd_send_command(0xb7);   // GCTRL: Gate control
* lcd_send_data(0x35);      // Default value
* lcd_send_command(0xbb);   // VCOMS: VCOMS setting
* lcd_send_data(0x28);      // Set: 1.1 V (default 0.9 V)
* lcd_send_command(0xc0);   // LCMCTRL: LCM control
* lcd_send_data(0x2c);      // Default value
* lcd_send_command(0xc2);   // VDVVRHEN: VDV and VRH command enable
* lcd_send_data(0x1);       // Default value
* lcd_send_command(0xc3);   // VRHS: VRH set
* lcd_send_data(0xb);       // Default value
* lcd_send_command(0xc4);   // VDVS: VDV set
* lcd_send_data(0x20);      // Default value
* lcd_send_command(0xc6);   // FRCTRL2: Frame rate control in normal mode
* lcd_send_data(0xf);       // Default value
* lcd_send_command(0xd0);   // PWCTRL1: Power control 1
* lcd_send_data(0xa4);      // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
* lcd_send_data(0xa1);
* lcd_send_command(0xe0);   // PVGAMCTRL: Positive voltage gamma control
* lcd_send_data(0xd0);      // Set
* lcd_send_data(0x1);
* lcd_send_data(0x8);
* lcd_send_data(0xf);
* lcd_send_data(0x11);
* lcd_send_data(0x2a);
* lcd_send_data(0x36);
* lcd_send_data(0x55);
* lcd_send_data(0x44);
* lcd_send_data(0x3a);
* lcd_send_data(0xb);
* lcd_send_data(0x6);
* lcd_send_data(0x11);
* lcd_send_data(0x20);
* lcd_send_command(0xe1);   // NVGAMCTRL: Negative voltage gamma control
* lcd_send_data(0xd0);      // Set
* lcd_send_data(0x2);
* lcd_send_data(0x7);
* lcd_send_data(0xa);
* lcd_send_data(0xb);
* lcd_send_data(0x18);
* lcd_send_data(0x34);
* lcd_send_data(0x43);
* lcd_send_data(0x4a);
* lcd_send_data(0x2b);
* lcd_send_data(0x1b);
* lcd_send_data(0x1c);
* lcd_send_data(0x22);
* lcd_send_data(0x1f);
* lcd_send_command(0x29);   // DISPON: Display on
* lcd_send_command(0x2c);   // RAMWR: Memory write

## Data write

* lcd_send_command(0x36);   // MADCTL: Memory access control
* lcd_send_data(0xa0);
* lcd_send_command(0x2a);   // CASET: Column address set
* lcd_send_data(0x0);       // Default (start, end)
* lcd_send_data(0x0);
* lcd_send_data(0x1);
* lcd_send_data(0x3f);
* lcd_send_command(0x2b);   // RASET: Row address set
* lcd_send_data(0x0);       // Default (start, end)
* lcd_send_data(0x0);
* lcd_send_data(0x0);
* lcd_send_data(0xef);
