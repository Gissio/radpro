/*
 * MCU renderer
 * ILI9341 driver
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#include "mcu-renderer-ili9341.h"

void mr_ili9341_init(mr_t *mr,
                     int16_t width,
                     int16_t height,
                     mr_display_rotation_t rotation,
                     uint8_t *textbuffer,
                     uint32_t textbuffer_size,
                     mr_sleep_callback_t sleep_callback,
                     mr_set_reset_callback_t set_reset_callback,
                     mr_set_command_callback_t set_command_callback,
                     mr_send_callback_t send_callback,
                     mr_send_callback_t send16_callback)
{
    mr_st7789_init(mr,
                   width,
                   height,
                   rotation,
                   textbuffer,
                   textbuffer_size,
                   sleep_callback,
                   set_reset_callback,
                   set_command_callback,
                   send_callback,
                   send16_callback);
}
