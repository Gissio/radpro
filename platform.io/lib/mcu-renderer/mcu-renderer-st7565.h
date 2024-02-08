/*
 * MCU renderer
 * ST7565 driver
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#if !defined(MCURENDERER_ST7565_H)
#define MCURENDERER_ST7565_H

#include "mcu-renderer.h"

// ST7565 commands

#define MR_ST7565_COLUMN_LSB 0x00
#define MR_ST7565_COLUMN_MSB 0x10
#define MR_ST7565_REGULATIONRATIO_3_0 0x20
#define MR_ST7565_REGULATIONRATIO_3_5 0x21
#define MR_ST7565_REGULATIONRATIO_4_0 0x22
#define MR_ST7565_REGULATIONRATIO_4_5 0x23
#define MR_ST7565_REGULATIONRATIO_5_0 0x24
#define MR_ST7565_REGULATIONRATIO_5_5 0x25
#define MR_ST7565_REGULATIONRATIO_6_0 0x26
#define MR_ST7565_REGULATIONRATIO_6_5 0x27
#define MR_ST7565_POWER 0x28
#define MR_ST7565_POWER_VF 0x29
#define MR_ST7565_POWER_VR 0x2a
#define MR_ST7565_POWER_VR_VF 0x2b
#define MR_ST7565_POWER_VB 0x2c
#define MR_ST7565_POWER_VB_VF 0x2d
#define MR_ST7565_POWER_VB_VR 0x2e
#define MR_ST7565_POWER_VB_VR_VF 0x2f
#define MR_ST7565_START_LINE 0x40
#define MR_ST7565_ELECTRONIC_VOLUME 0x81
#define MR_ST7565_SEG_NORMAL 0xa0
#define MR_ST7565_SEG_REVERSE 0xa1
#define MR_ST7565_BIAS_1_9 0xa2
#define MR_ST7565_BIAS_1_7 0xa3
#define MR_ST7565_ALLPIXEL_OFF 0xa4
#define MR_ST7565_ALLPIXEL_ON 0xa5
#define MR_ST7565_INVERSE_OFF 0xa6
#define MR_ST7565_INVERSE_ON 0xa7
#define MR_ST7565_DISPLAY_OFF 0xae
#define MR_ST7565_DISPLAY_ON 0xaf
#define MR_ST7565_PAGE_ADDRESS 0xb0
#define MR_ST7565_COM_NORMAL 0xc0
#define MR_ST7565_COM_REVERSE 0xc8
#define MR_ST7565_READ_MODIFY_WRITE_START 0xe0
#define MR_ST7565_SOFTWARE_RESET 0xe2
#define MR_ST7565_NOP 0xe3
#define MR_ST7565_READ_MODIFY_WRITE_END 0xee
#define MR_ST7565_BOOSTER 0xf8

/**
 * Initializes an mcu-renderer ST7565 instance.
 *
 * @param mr The mcu-renderer instance.
 * @param width The physical display width (e.g. 128).
 * @param height The physical display height (e.g. 64).
 * @param rotation The desired display rotation.
 * @param framebuffer A user-provided framebuffer of size (width * height / 8).
 * @param sleep_callback A user-provided sleep callback.
 * @param set_reset_callback A user-provided set reset callback.
 * @param set_command_callback A user-provided set command callback.
 * @param send_callback A user-provided send 8-bit data callback.
 */
void mr_st7565_init(mr_t *mr,
                    int16_t width,
                    int16_t height,
                    mr_display_rotation_t rotation,
                    uint8_t *framebuffer,
                    mr_sleep_callback_t sleep_callback,
                    mr_set_reset_callback_t set_reset_callback,
                    mr_set_command_callback_t set_command_callback,
                    mr_send_callback_t send_callback);

#endif
