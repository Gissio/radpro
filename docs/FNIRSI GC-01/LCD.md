# FNIRSI GC-01 240x320 LCD display (ST7789V)

## Initialization sequence

* lcd_set_reset();
* sleep(10); // 1.25 ms
* lcd_clear_reset();
* sleep(20); // 2.5 ms
* lcd_set_reset();
* sleep(20);

* lcd_set_cs();

* lcd_send_command(0x11);   //
* sleep(120) // 15 ms
* lcd_send_command(0x36);   // 
* lcd_send_data(0x0);
* lcd_send_command(0x3a);   // 
* lcd_send_data(0x5);
* lcd_send_command(0xb2);   // 
* lcd_send_data(0xc);
* lcd_send_data(0xc);
* lcd_send_data(0x0);
* lcd_send_data(0x33);
* lcd_send_data(0x33);
* lcd_send_command(0xb7);   // 
* lcd_send_data(0x35);
* lcd_send_command(0xbb);   // 
* lcd_send_data(0x28);
* lcd_send_command(0xc0);   // 
* lcd_send_data(0x2c);
* lcd_send_command(0xc2);   // 
* lcd_send_data(0x1);
* lcd_send_command(0xc3);   // 
* lcd_send_data(0xb);
* lcd_send_command(0xc4);   // 
* lcd_send_data(0x20);
* lcd_send_command(0xc6);   // 
* lcd_send_data(0xf);
* lcd_send_command(0xd0);   // 
* lcd_send_data(0xa4);
* lcd_send_data(0xa1);
* lcd_send_command(0xe0);   // 
* lcd_send_data(0xd0);
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
* lcd_send_command(0xe1);   // 
* lcd_send_data(0xd0);
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
* lcd_send_command(0x29);   // 
* lcd_send_command(0x2c);   // 

* lcd_send_command(0x36);   // 
* lcd_send_data(0xa0);

* lcd_send_command(0x2a);   // Set width?
* lcd_send_data(0x0);
* lcd_send_data(0x0);
* lcd_send_data(0x1);
* lcd_send_data(0x3f);

* lcd_send_command(0x2b);   // Set height?
* lcd_send_data(0x0);
* lcd_send_data(0x0);
* lcd_send_data(0x0);
* lcd_send_data(0xef);
