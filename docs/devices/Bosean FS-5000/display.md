# LCD Displays

## FPC-H24C159-01W Initialization sequence

    display_send_command(0x3a);     // COLMOD (should be 0x55)
    display_send_data(0x06);
    display_send_command(0xbb);     // VCOMS (default: 0x20)
    display_send_data(0x2b);
    display_send_command(0xc3);     // VRHS (default: 0xb)
    display_send_data(0x11);
    display_send_command(0xd0);     // PWCTRL1 (default: 0xa4, 0x81)
    display_send_data(0xa4);
    display_send_data(0xa1);
    display_send_command(0xe0);     // PVGAMCTRL
    display_send_data(0xd0);
    display_send_data(0x00);
    display_send_data(0x05);
    display_send_data(0x0e);
    display_send_data(0x15);
    display_send_data(0x0d);
    display_send_data(0x37);
    display_send_data(0x43);
    display_send_data(0x47);
    display_send_data(0x09);
    display_send_data(0x15);
    display_send_data(0x12);
    display_send_data(0x16);
    display_send_data(0x19);
    display_send_command(0xe1);     // NVGAMCTRL
    display_send_data(0xd0);
    display_send_data(0x00);
    display_send_data(0x05);
    display_send_data(0x0d);
    display_send_data(0x0c);
    display_send_data(0x06);
    display_send_data(0x2d);
    display_send_data(0x44);
    display_send_data(0x40);
    display_send_data(0x0e);
    display_send_data(0x1c);
    display_send_data(0x18);
    display_send_data(0x16);
    display_send_data(0x19);
