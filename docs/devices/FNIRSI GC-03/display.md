# FNIRSI GC-01 240x320 LCD display (ST7789V)

## Initialization sequence

    Peripherals::GPIOC.BSRR = 0x20;     // Set DISPLAY_RESX
    sleepMs(120);
    Peripherals::GPIOC.BSRR = 0x200000; // Clear DISPLAY_RESX
    sleepMs(50);
    Peripherals::GPIOC.BSRR = 0x20;     // Set DISPLAY_RESX
    sleepMs(50);

    lcdSendCommand(0x11);   // SLPOUT: Sleep out
    lcdSendCommand(0x29);   // DISPON: Display on
    sleepMs(120);
    lcdSendCommand(0x36);   // MADCTL: Memory access
    lcdSendData(0);
    lcdSendCommand(0x3a);   // COLMOD: Interface pixel 
    lcdSendData(0x500);     // Set: 0x5 (16 bit/pixel) 
    lcdSendCommand(0xb2);   // PORCTRL: Porch setting
    lcdSendData(0xc0c);     // Default value
    lcdSendData(0);
    lcdSendData(0x3333);
    lcdSendCommand(0xb7);   // GCTRL: Gate control
    lcdSendData(0x3500);    // Default value
    lcdSendCommand(0xbb);   // VCOMS: VCOMS setting
    lcdSendData(0x2800);    // Set: 1.1 V (default 0.9 V)
    lcdSendCommand(0xc0);   // LCMCTRL: LCM control
    lcdSendData(0x2c00);    // Default value
    lcdSendCommand(0xc2);   // VDVVRHEN: VDV and VRH command enable
    lcdSendData(0x100);     // Default value
    lcdSendCommand(0xc3);   // VRHS: VRH set
    lcdSendData(0xb00);     // Default value
    lcdSendCommand(0xc4);   // VDVS: VDV set
    lcdSendData(0x2000);    // Default value
    lcdSendCommand(0xc6);   // FRCTRL2: Frame rate control in normal mode
    lcdSendData(0xf00);     // Default value
    lcdSendCommand(0xd0);   // PWCTRL1: Power control 1
    lcdSendData(0xa4a1);    // Set: AVDD 6.8 V, AVCL -4.8 V, VDDS 2.3 V
    lcdSendCommand(0xe0);   // PVGAMCTRL: Positive voltage gamma control
    lcdSendData(0xd001);    // Set
    lcdSendData(0x80f);
    lcdSendData(0x112a);
    lcdSendData(0x3655);
    lcdSendData(0x443a);
    lcdSendData(0xb06);
    lcdSendData(0x1120);
    lcdSendCommand(0xe1);   // NVGAMCTRL: Negative voltage gamma control
    lcdSendData(0xd002);    // Set
    lcdSendData(0x70a);
    lcdSendData(0xb18);
    lcdSendData(0x3443);
    lcdSendData(0x4a2b);
    lcdSendData(0x1b1c);
    lcdSendData(0x221f);
    lcdSendCommand(0x29);   // DISPON: Display on
    lcdSendCommand(0x2c);   // RAMWR: Memory write
    lcdSetRotation(0);

    lcdClearArea(0,0,0xf0,0x140);
    sleepMs(200);
