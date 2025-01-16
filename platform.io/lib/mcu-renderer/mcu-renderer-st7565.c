/*
 * MCU renderer
 * ST7565 driver
 *
 * (C) 2023-2025 Gissio
 *
 * License: MIT
 */

#include "mcu-renderer-st7565.h"

static const uint8_t mr_st7565_init_sequence[] = {
    MR_SLEEP(1),
    MR_SET_RESET(true),
    MR_SLEEP(1),
    MR_SET_RESET(false),
    MR_SLEEP(1),

    MR_SEND_COMMAND(MR_ST7565_POWER | MR_ST7565_POWER_VB |
                    MR_ST7565_POWER_VR | MR_ST7565_POWER_VF),
    MR_END(),
};

static const uint8_t mr_st7565_display_on_sequence[] = {
    // Powersave off (compound command)
    MR_SEND_COMMAND(MR_ST7565_ALLPIXEL_OFF),
    MR_SEND_COMMAND(MR_ST7565_DISPLAY_ON),
    MR_END(),
};

static const uint8_t mr_st7565_display_off_sequence[] = {
    // Powersave on (compound command)
    MR_SEND_COMMAND(MR_ST7565_DISPLAY_OFF),
    MR_SEND_COMMAND(MR_ST7565_ALLPIXEL_ON),
    MR_END(),
};

void mr_st7565_init(mr_t *mr,
                    int16_t width,
                    int16_t height,
                    mr_display_rotation_t rotation,
                    uint8_t *framebuffer,
                    mr_sleep_callback_t sleep_callback,
                    mr_set_reset_callback_t set_reset_callback,
                    mr_set_chipselect_callback_t set_chipselect_callback,
                    mr_set_command_callback_t set_command_callback,
                    mr_send_callback_t send_callback)
{
    mr_init(mr);

    mr->display_width = width;
    mr->display_height = height;
    mr->display_rotation = rotation;

    mr->buffer = framebuffer;

    mr->draw_rectangle_callback = mr_draw_rectangle_framebuffer_monochrome_vertical;
    mr->draw_string_callback = mr_draw_string_framebuffer_monochrome_vertical;
#if defined(MCURENDERER_BITMAP_SUPPORT)
    mr->draw_bitmap_callback = mr_draw_bitmap_framebuffer_monochrome_vertical;
#endif
#if defined(MCURENDERER_IMAGE_SUPPORT)
    mr->draw_image_callback = mr_draw_image_framebuffer_monochrome_vertical;
#endif
    mr->sleep_callback = sleep_callback;
    mr->set_reset_callback = set_reset_callback;
    mr->set_chipselect_callback = set_chipselect_callback;
    mr->set_command_callback = set_command_callback;
    mr->send_callback = send_callback;

    mr_send_sequence(mr, mr_st7565_init_sequence);
}

void mr_st7565_set_display(mr_t *mr,
                           bool value)
{
    mr_send_sequence(mr,
                     value
                         ? mr_st7565_display_on_sequence
                         : mr_st7565_display_off_sequence);
}

void mr_st7565_refresh_display(mr_t *mr)
{
    mr_set_chipselect(mr, true);

    for (int16_t pageIndex = 0;
         pageIndex < (mr->display_height / 8);
         pageIndex++)
    {
        // Send page address
        mr_send_command(mr, MR_ST7565_PAGE_ADDRESS | pageIndex);

        // Send column address
        mr_send_command(mr, MR_ST7565_COLUMN_LSB | 0x00);
        mr_send_command(mr, MR_ST7565_COLUMN_MSB | 0x00);

        // Send data
        mr_set_command(mr, false);
        uint8_t *buffer = (uint8_t *)mr->buffer +
                          mr->display_width * pageIndex;

        mr_send_callback_t send = mr->send_callback;

        for (int16_t i = 0;
             i < mr->display_width;
             i++)
            send(buffer[i]);
    }

    mr_set_chipselect(mr, false);
}
