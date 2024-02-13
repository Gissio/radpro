/*
 * MCU renderer
 * ST7789 driver
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#include "mcu-renderer-st7789.h"

static const uint8_t mr_st7789_init_sequence[] = {
    MR_SLEEP(1),
    MR_SET_RESET(true),
    MR_SLEEP(1),
    MR_SET_RESET(false),
    MR_SLEEP(5),
    MR_SEND_COMMAND(MR_ST7789_COLMOD), // Interface pixel format: 16 bpp
    MR_SEND_DATA(MR_ST7789_COLMOD_16BPP),
    MR_SEND_COMMAND(MR_ST7789_DISPON), // Display on
    MR_END(),
};

static const uint8_t mr_st7789_display_on_sequence[] = {
    MR_SEND_COMMAND(MR_ST7789_SLPOUT), // Sleep out
    MR_SLEEP(120),
    MR_END(),
};

static const uint8_t mr_st7789_display_off_sequence[] = {
    MR_SEND_COMMAND(MR_ST7789_SLPIN), // Sleep in
    MR_SLEEP(120),
    MR_END(),
};

static const uint8_t mr_st7789_madctl[] = {
    0,
    MR_ST7789_MADCTL_MV | MR_ST7789_MADCTL_MX,
    MR_ST7789_MADCTL_MX | MR_ST7789_MADCTL_MY,
    MR_ST7789_MADCTL_MV | MR_ST7789_MADCTL_MY,
};

void mr_st7789_set_display(mr_t *mr,
                           bool value);
void mr_st7789_draw_rectangle(mr_t *mr,
                              const mr_rectangle_t *rectangle);
void mr_st7789_draw_image(mr_t *mr,
                          const mr_rectangle_t *rectangle,
                          const mr_color *image);
void mr_st7789_draw_textbuffer(mr_t *mr,
                               uint8_t *buffer,
                               uint32_t buffer_pitch,
                               mr_rectangle_t *rectangle);

void mr_st7789_init(mr_t *mr,
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
    mr_init(mr);

    mr->display_width = width;
    mr->display_height = height;
    mr->display_rotation = rotation;

    mr->buffer = textbuffer;
    mr->buffer_size = textbuffer_size;

    mr->set_display_callback = mr_st7789_set_display;
    mr->draw_rectangle_callback = mr_st7789_draw_rectangle;
    mr->draw_string_callback = mr_draw_string_textbuffer;
    mr->draw_textbuffer_callback = mr_st7789_draw_textbuffer;
#if defined(MCURENDERER_IMAGE_SUPPORT)
    mr->draw_image_callback = mr_st7789_draw_image;
#endif
    mr->sleep_callback = sleep_callback;
    mr->set_reset_callback = set_reset_callback;
    mr->set_command_callback = set_command_callback;
    mr->send_callback = send_callback;
    mr->send16_callback = send16_callback;

    // Send initialization sequence

    mr_send_sequence(mr, mr_st7789_init_sequence);

    // Set rotation

    mr_send_command(mr, MR_ST7789_MADCTL);
    mr_send_data(mr, mr_st7789_madctl[rotation]);
}

void mr_st7789_set_display(mr_t *mr, bool value)
{
    mr_send_sequence(mr,
                     value
                         ? mr_st7789_display_on_sequence
                         : mr_st7789_display_off_sequence);
}

static void mr_st7789_send_short(mr_t *mr,
                             uint16_t value)
{
    mr_send_data(mr, (value >> 8) & 0xff);
    mr_send_data(mr, (value >> 0) & 0xff);
}

static void mr_st7789_send_addresses(mr_t *mr,
                                     uint8_t command,
                                     uint16_t start_address,
                                     uint16_t end_address)
{
    mr_send_command(mr, command);
    mr_st7789_send_short(mr, start_address);
    mr_st7789_send_short(mr, end_address);
}

static void mr_st7789_set_rectangle(mr_t *mr,
                                    const mr_rectangle_t *rectangle)
{
    // Set column addresses
    mr_st7789_send_addresses(mr,
                             MR_ST7789_CASET,
                             rectangle->x,
                             rectangle->x + rectangle->width - 1);

    // Set row addresses
    mr_st7789_send_addresses(mr,
                             MR_ST7789_RASET,
                             rectangle->y,
                             rectangle->y + rectangle->height - 1);

    // Write memory
    mr_send_command(mr, MR_ST7789_RAMWR);
    mr_set_command(mr, false);
}

void mr_st7789_draw_rectangle(mr_t *mr,
                              const mr_rectangle_t *rectangle)
{
    mr_st7789_set_rectangle(mr, rectangle);

    for (uint32_t i = 0;
         i < ((uint32_t)rectangle->width *
              (uint32_t)rectangle->height);
         i++)
        mr_send16(mr, mr->fill_color);
}

void mr_st7789_draw_image(mr_t *mr,
                          const mr_rectangle_t *rectangle,
                          const mr_color *image)
{
    mr_st7789_set_rectangle(mr, rectangle);

    for (uint32_t i = 0;
         i < ((uint32_t)rectangle->width *
              (uint32_t)rectangle->height);
         i++)
        mr_send16(mr, *image++);
}

void mr_st7789_draw_textbuffer(mr_t *mr,
                               uint8_t *buffer,
                               uint32_t buffer_pitch,
                               mr_rectangle_t *rectangle)
{
    mr_st7789_set_rectangle(mr, rectangle);

    for (int16_t y = 0;
         y < rectangle->height;
         y++)
    {
        for (int16_t x = 0;
             x < rectangle->width;
             x++)
            mr_send16(mr, mr->blend_table[buffer[x]]);

        buffer += buffer_pitch;
    }
}
