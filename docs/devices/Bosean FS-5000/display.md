# Bosean FS-50000 240x320 LCD display (ST7789V)

* 0x11 SLPOUT: Wait 10 ms
* 0x36 MADCTL: 0x00
* 0x3a COLMOD: 0x55
* 0xb2 PORCTRL: 0x0c 0x0c 0x00 0x33 0x33
* 0xb7 GCTRL: 0x35
* 0xbb VCOMS: 0x28
* 0xc0 LCMCTRL: 0x2c
* 0xc2 VDVVRHEN: 0x01
* 0xc3 VRHS: 0x22
* 0xc4 VDVS: 0x20
* 0xc6 FRCTRL2: 0x0f
* 0xd0 PWCTRL1: 0xa4 0xa1
* 0xe0 PVGAMCTRL: 0xd0 0x04 0x10 0x13 0x14 0x2c 0x41 0x54 0x4e 0x18 0x0c 0x0a 0x21 0x25
* 0xe1 NVGAMCTRL: 0xd0 0x04 0x0f 0x13 0x15 0x2c 0x40 0x44 0x52 0x2d 0x1d 0x1e 0x21 0x24
* 0x2a CASET: 0x00 0x00 0x00 0xef
* 0x2b RASET: 0x00 0x00 0x01 0x3f

## Initialization sequence

* display_send_command(0x11);   // SLPOUT: Sleep out
* sleep(10)
* display_send_command(0x36);   // MADCTL: Memory access control
* display_send_data(0x00);
* display_send_command(0x3a);   // COLMOD: Interface pixel format
* display_send_data(0x55);      // Set: 0x55 (16-bit interface format, 16 bit control format)
* display_send_command(0xb2);   // PORCTRL: Porch setting
* display_send_data(0x0c);      // Default value
* display_send_data(0x0c);
* display_send_data(0x00);
* display_send_data(0x33);
* display_send_data(0x33);
* display_send_command(0xb7);   // GCTRL: Gate control
* display_send_data(0x35);      // Default value
* display_send_command(0xbb);   // VCOMS: VCOMS setting
* display_send_data(0x28);      // Set: 1.1 V (default 0.9 V)
* display_send_command(0xc0);   // LCMCTRL: LCM control
* display_send_data(0x2c);      // Default value
* display_send_command(0xc2);   // VDVVRHEN: VDV and VRH command enable
* display_send_data(0x1);       // Default value
* display_send_command(0xc3);   // VRHS: VRH set
* display_send_data(0x22);      // -5.25+(vcom+vcom offset-vdv)
* display_send_command(0xc4);   // VDVS: VDV set
* display_send_data(0x20);      // Default value
* display_send_command(0xc6);   // FRCTRL2: Frame rate control in normal mode
* display_send_data(0x0f);      // Default value
* display_send_command(0xd0);   // PWCTRL1: Power control 1
* display_send_data(0xa4);      // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
* display_send_data(0xa1);
* display_send_command(0xe0);   // PVGAMCTRL: Positive voltage gamma control
* display_send_data(0xd0);      // Set
* display_send_data(0x04);
* display_send_data(0x10);
* display_send_data(0x13);
* display_send_data(0x14);
* display_send_data(0x2c);
* display_send_data(0x41);
* display_send_data(0x54);
* display_send_data(0x4e);
* display_send_data(0x18);
* display_send_data(0x0c);
* display_send_data(0x0a);
* display_send_data(0x21);
* display_send_data(0x25);
* display_send_command(0xe1);   // NVGAMCTRL: Negative voltage gamma control
* display_send_data(0xd0);      // Set
* display_send_data(0x04);
* display_send_data(0x0f);
* display_send_data(0x13);
* display_send_data(0x15);
* display_send_data(0x2c);
* display_send_data(0x40);
* display_send_data(0x44);
* display_send_data(0x52);
* display_send_data(0x2d);
* display_send_data(0x1d);
* display_send_data(0x1e);
* display_send_data(0x21);
* display_send_data(0x24);
