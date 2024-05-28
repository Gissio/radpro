# FNIRSI GC-01 240x320 LCD display (ST7789V)

## Initialization sequence

    display_set_reset();
    sleep(10);
    display_clear_reset();
    sleep(20);
    display_set_reset();
    sleep(20);

    display_send_command(0x11);   // SLPOUT: Sleep out
    sleep(120)
    display_send_command(0x36);   // MADCTL: Memory access control
    display_send_data(0x0);
    display_send_command(0x3a);   // COLMOD: Interface pixel format
    display_send_data(0x5);       // Set: 0x5 (16 bit/pixel) (should be 0x55)
    display_send_command(0xb2);   // PORCTRL: Porch setting
    display_send_data(0xc);       // Default value
    display_send_data(0xc);
    display_send_data(0x0);
    display_send_data(0x33);
    display_send_data(0x33);
    display_send_command(0xb7);   // GCTRL: Gate control
    display_send_data(0x35);      // Default value
    display_send_command(0xbb);   // VCOMS: VCOMS setting
    display_send_data(0x28);      // Set: 1.1 V (default 0.9 V)
    display_send_command(0xc0);   // LCMCTRL: LCM control
    display_send_data(0x2c);      // Default value
    display_send_command(0xc2);   // VDVVRHEN: VDV and VRH command enable
    display_send_data(0x1);       // Default value
    display_send_command(0xc3);   // VRHS: VRH set
    display_send_data(0xb);       // Default value
    display_send_command(0xc4);   // VDVS: VDV set
    display_send_data(0x20);      // Default value
    display_send_command(0xc6);   // FRCTRL2: Frame rate control in normal mode
    display_send_data(0xf);       // Default value
    display_send_command(0xd0);   // PWCTRL1: Power control 1
    display_send_data(0xa4);      // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
    display_send_data(0xa1);
    display_send_command(0xe0);   // PVGAMCTRL: Positive voltage gamma control
    display_send_data(0xd0);      // Set
    display_send_data(0x1);
    display_send_data(0x8);
    display_send_data(0xf);
    display_send_data(0x11);
    display_send_data(0x2a);
    display_send_data(0x36);
    display_send_data(0x55);
    display_send_data(0x44);
    display_send_data(0x3a);
    display_send_data(0xb);
    display_send_data(0x6);
    display_send_data(0x11);
    display_send_data(0x20);
    display_send_command(0xe1);   // NVGAMCTRL: Negative voltage gamma control
    display_send_data(0xd0);      // Set
    display_send_data(0x2);
    display_send_data(0x7);
    display_send_data(0xa);
    display_send_data(0xb);
    display_send_data(0x18);
    display_send_data(0x34);
    display_send_data(0x43);
    display_send_data(0x4a);
    display_send_data(0x2b);
    display_send_data(0x1b);
    display_send_data(0x1c);
    display_send_data(0x22);
    display_send_data(0x1f);
    display_send_command(0x29);   // DISPON: Display on
    display_send_command(0x2c);   // RAMWR: Memory write

## Data write

    display_send_command(0x36);   // MADCTL: Memory access control
    display_send_data(0xa0);
    display_send_command(0x2a);   // CASET: Column address set
    display_send_data(0x0);       // Default (start, end)
    display_send_data(0x0);
    display_send_data(0x1);
    display_send_data(0x3f);
    display_send_command(0x2b);   // RASET: Row address set
    display_send_data(0x0);       // Default (start, end)
    display_send_data(0x0);
    display_send_data(0x0);
    display_send_data(0xef);
