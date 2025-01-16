/*
 * MCU renderer
 * Core module
 *
 * (C) 2023-2025 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include <string.h>

#include "mcu-renderer.h"

#define MR_FONT_HEADER_SIZE 22

typedef struct
{
    int16_t font_cap_height;
    int16_t font_ascent;
    int16_t font_descent;
    int16_t font_boundingbox_left;
    int16_t font_boundingbox_bottom;
    int16_t font_boundingbox_width;
    int16_t font_boundingbox_height;

    uint8_t glyph_boundingbox_left_bitnum;
    uint8_t glyph_boundingbox_bottom_bitnum;
    uint8_t glyph_boundingbox_width_bitnum;
    uint8_t glyph_boundingbox_height_bitnum;
    uint8_t glyph_advance_bitnum;
    uint8_t glyph_pixel_bitnum;
    uint8_t glyph_repeat_black_bitnum;
    uint8_t glyph_repeat_white_bitnum;
} mr_font_header_t;

// Text decoding

static mr_charcode mr_decode_c_string(uint8_t **strp)
{
    mr_charcode charcode = **strp;

    *strp += 1;

    return charcode;
}

static mr_charcode mr_decode_utf8(uint8_t **strp)
{
    uint8_t *str = *strp;
    mr_charcode value;

    if (str[0] < 0x80)
    {
        value = str[0];

        *strp += 1;
    }
    else if ((str[0] & 0xe0) == 0xc0)
    {
        value = ((str[0] & 0x1f) << 6) |
                ((str[1] & 0x3f) << 0);

        *strp += 2;
    }
    else if ((str[0] & 0xf0) == 0xe0)
    {
        value = ((str[0] & 0x0f) << 12) |
                ((str[1] & 0x3f) << 6) |
                ((str[2] & 0x3f) << 0);

        *strp += 3;
    }
    else if ((str[0] & 0xf8) == 0xf0 && (str[0] <= 0xf4))
    {
        value = ((str[0] & 0x07) << 18) |
                ((str[1] & 0x3f) << 12) |
                ((str[2] & 0x3f) << 6) |
                ((str[3] & 0x3f) << 0);

        *strp += 4;
    }
    else
    {
        // Invalid

        value = -1;

        *strp += 1;
    }

    return value;
}

static mr_charcode mr_decode_utf16(uint8_t **strp)
{
    uint16_t *str = (uint16_t *)*strp;
    mr_charcode value;

    if ((str[0] < 0xd800) || (str[0] >= 0xe000))
    {
        value = str[0];

        *strp += 2;
    }
    else if ((str[0] >= 0xd800) && (str[0] < 0xdc00) &&
             (str[1] >= 0xdc00) && (str[1] <= 0xde00))
    {
        value = 0x00010000 +
                (((str[0] & 0x03ff) << 10) |
                 ((str[1] & 0x03ff) << 0));

        *strp += 4;
    }
    else
    {
        // Invalid
        value = -1;

        *strp += 2;
    }

    return value;
}

// Data decoding

static uint8_t mr_get_unsigned_bits(mr_bitstream_t *br, uint8_t bitnum)
{
    uint8_t start_bit_index = br->bit_index;
    uint8_t end_bit_index = start_bit_index + bitnum;

    uint8_t value = (*br->data >> start_bit_index);

    if (end_bit_index >= 8)
    {
        br->data++;

        value |= *br->data << (8 - start_bit_index);

        end_bit_index -= 8;
    }

    br->bit_index = end_bit_index;

    value &= (1 << bitnum) - 1;

    return value;
}

static int8_t mr_get_signed_bits(mr_bitstream_t *br,
                                 uint8_t bitnum)
{
    int8_t value = (int8_t)mr_get_unsigned_bits(br, bitnum);

    uint8_t shift = 8 - bitnum;
    value <<= shift;
    value >>= shift;

    return value;
}

static int16_t mr_get_short(const int16_t *data)
{
    const uint8_t *data_bytes = (uint8_t *)data;

    int16_t value = (data_bytes[0] << 8) | data_bytes[1];

    return value;
}

static int32_t mr_get_variable_length_word(const uint8_t **data)
{
    int32_t value = 0;

    while (true)
    {
        uint8_t n = **data;

        value = (value << 7) | (n & 0x7f);

        (*data)++;

        if (!(n & 0x80))
            break;
    }

    return value;
}

// Geometry

#if defined(MCURENDERER_DISPLAY_ROTATION)

static inline mr_point_t mr_rotate_point(mr_t *mr,
                                         const mr_point_t *point)
{
    switch (mr->display_rotation)
    {
    default:
        return *point;

    case MR_DISPLAY_ROTATION_90:
        return (mr_point_t){
            mr->display_width - point->y,
            point->x};

    case MR_DISPLAY_ROTATION_180:
        return (mr_point_t){
            mr->display_width - point->x,
            mr->display_height - point->y};

    case MR_DISPLAY_ROTATION_270:
        return (mr_point_t){
            point->y,
            mr->display_height - point->x};
    }
}

#else

static inline mr_point_t mr_rotate_point(mr_t *mr,
                                         const mr_point_t *point)
{
    return *point;
}

#endif

static inline int16_t mr_min(int16_t a,
                             int16_t b)
{
    return (a < b) ? a : b;
}

static inline int16_t mr_max(int16_t a,
                             int16_t b)
{
    return (a > b) ? a : b;
}

static bool mr_intersect_rectangles(const mr_rectangle_t *a,
                                    const mr_rectangle_t *b,
                                    mr_rectangle_t *out)
{
    int16_t a_min, a_max, b_min, b_max;

    a_min = a->x;
    a_max = a_min + a->width;
    b_min = b->x;
    b_max = b_min + b->width;

    if ((a_min > b_max) ||
        (b_min > a_max))
        return false;

    out->x = mr_max(a_min, b_min);
    out->width = mr_min(a_max, b_max) - out->x;

    a_min = a->y;
    a_max = a_min + a->height;
    b_min = b->y;
    b_max = b_min + b->height;

    if ((a_min > b_max) ||
        (b_min > a_max))
        return false;

    out->y = mr_max(a_min, b_min);
    out->height = mr_min(a_max, b_max) - out->y;

    return true;
}

static inline bool mr_is_point_in_rect(const mr_point_t *p,
                                       const mr_rectangle_t *r)
{
    return ((p->x >= r->x) &&
            (p->x < (r->x + r->width)) &&
            (p->y >= r->y) &&
            (p->y < (r->y + r->height)));
}

// I/O

void mr_send_command(mr_t *mr,
                     uint8_t command)
{
    mr_set_command(mr, true);
    mr_send(mr, command);
}

void mr_send_data(mr_t *mr,
                  uint8_t value)
{
    mr_set_command(mr, false);
    mr_send(mr, value);
}

void mr_send_sequence(mr_t *mr,
                      const uint8_t *sequence)
{
    mr_set_chipselect(mr, true);

    while (true)
    {
        mr_sequence_t sequenceCommand = *sequence++;
        uint8_t value = *sequence++;

        switch (sequenceCommand)
        {
        case MR_SEQ_SLEEP:
            mr->sleep_callback(value);

            break;

        case MR_SEQ_SET_RESET:
            mr->set_reset_callback(value);

            break;

        case MR_SEQ_SEND_COMMAND:
            mr_send_command(mr, value);

            break;

        case MR_SEQ_SEND_DATA:
            mr_send_data(mr, value);

            break;

        case MR_SEQ_END:
            return;
        }
    }

    mr_set_chipselect(mr, false);
}

// Rectangle rendering

void mr_draw_rectangle_framebuffer_monochrome_vertical(mr_t *mr,
                                                       const mr_rectangle_t *rectangle)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            mr_color_t source_color = mr->fill_color;

            mr_point_t dest_position = mr_rotate_point(mr, &position);
            uint8_t *dest_buffer = (uint8_t *)mr->buffer +
                                   (dest_position.y >> 3) * mr->display_width +
                                   dest_position.x;
            uint8_t dest_buffer_mask = 1 << (dest_position.y & 0b111);

            if (source_color >> 15)
                *dest_buffer |= dest_buffer_mask;
            else
                *dest_buffer &= ~dest_buffer_mask;
        }
    }
}

void mr_draw_rectangle_framebuffer_color(mr_t *mr,
                                         const mr_rectangle_t *rectangle)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            mr_color_t source_color = mr->fill_color;

            mr_point_t dest_position = mr_rotate_point(mr, &position);
            mr_color_t *dest_buffer = (mr_color_t *)mr->buffer +
                                      dest_position.y * mr->display_width +
                                      dest_position.x;

            *dest_buffer = source_color;
        }
    }
}

// Image rendering

void mr_draw_bitmap_framebuffer_monochrome_vertical(mr_t *mr,
                                                    const mr_rectangle_t *rectangle,
                                                    const uint8_t *bitmap)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        uint32_t source_index = 0;

        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            bool source_pixel = ((bitmap[source_index >> 3]) >> (source_index & 0b111)) & 0b1;
            source_index++;
            mr_color_t source_color = source_pixel ? mr->stroke_color : mr->fill_color;

            mr_point_t dest_position = mr_rotate_point(mr, &position);
            uint8_t *dest_buffer = (uint8_t *)mr->buffer +
                                   (dest_position.y >> 3) * mr->display_width +
                                   dest_position.x;
            uint8_t dest_buffer_mask = 1 << (dest_position.y & 0b111);

            if (source_color >> 15)
                *dest_buffer |= dest_buffer_mask;
            else
                *dest_buffer &= ~dest_buffer_mask;
        }

        bitmap += ((source_index + 7) >> 3);
    }
}

void mr_draw_bitmap_framebuffer_color(mr_t *mr,
                                      const mr_rectangle_t *rectangle,
                                      const uint8_t *bitmap)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        uint32_t source_index = 0;

        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            bool source_pixel = ((bitmap[source_index >> 3]) >> (source_index & 0b111)) & 0b1;
            source_index++;
            mr_color_t source_color = source_pixel ? mr->stroke_color : mr->fill_color;

            mr_point_t dest_position = mr_rotate_point(mr, &position);
            mr_color_t *dest_buffer = (mr_color_t *)mr->buffer +
                                      dest_position.y * mr->display_width +
                                      dest_position.x;

            *dest_buffer = source_color;
        }

        bitmap += ((source_index + 7) >> 3);
    }
}

void mr_draw_image_framebuffer_monochrome_vertical(mr_t *mr,
                                                   const mr_rectangle_t *rectangle,
                                                   const mr_color_t *image)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            mr_color_t source_color = *image++;
            mr_point_t dest_position = mr_rotate_point(mr, &position);
            uint8_t *dest_buffer = (uint8_t *)mr->buffer +
                                   (dest_position.y >> 3) * mr->display_width +
                                   dest_position.x;
            uint8_t dest_buffer_mask = 1 << (dest_position.y & 0b111);

            if (source_color >> 15)
                *dest_buffer |= dest_buffer_mask;
            else
                *dest_buffer &= ~dest_buffer_mask;
        }
    }
}

void mr_draw_image_framebuffer_color(mr_t *mr,
                                     const mr_rectangle_t *rectangle,
                                     const mr_color_t *image)
{
    mr_point_t position;

    for (position.y = rectangle->y;
         position.y < rectangle->y + rectangle->height;
         position.y++)
    {
        for (position.x = rectangle->x;
             position.x < rectangle->x + rectangle->width;
             position.x++)
        {
            mr_color_t source_color = *image++;

            mr_point_t dest_position = mr_rotate_point(mr, &position);
            mr_color_t *dest_buffer = (mr_color_t *)mr->buffer +
                                      dest_position.y * mr->display_width +
                                      dest_position.x;

            *dest_buffer = source_color;
        }
    }
}

// Text rendering

static inline mr_color_t mr_fast_blend(uint32_t foreground_color,
                                       uint32_t background_color,
                                       uint8_t alpha)
{
    // alpha = (alpha_8bit + 4) >> 3;

    foreground_color = (foreground_color | (foreground_color << 16)) &
                       0b00000111111000001111100000011111;
    background_color = (background_color | (background_color << 16)) &
                       0b00000111111000001111100000011111;

    uint32_t result = ((((foreground_color - background_color) * alpha) >> 5) +
                       background_color) &
                      0b00000111111000001111100000011111;

    return (mr_color_t)((result >> 16) | result);
}

static void mr_update_blend_table(mr_t *mr)
{
    if ((mr->blend_table[0] == mr->fill_color) &&
        (mr->blend_table[COLOR_BLEND_TABLE_SIZE - 1] == mr->stroke_color))
        return;

    for (int i = 0; i < COLOR_BLEND_TABLE_SIZE; i++)
        mr->blend_table[i] = mr_fast_blend(mr->stroke_color,
                                           mr->fill_color,
                                           i);
}

static inline uint8_t mr_get_alpha(uint8_t pixel_bitnum,
                                   uint8_t value)
{
    switch (pixel_bitnum)
    {
    case 1:
        value = (value << 6) - value;

        break;

    case 2:
        value = (value << 4) + (value << 2) + value;

        break;

    default:
        value = (value << (6 - pixel_bitnum)) +
                (value >> ((pixel_bitnum - 3) << 1));

        break;
    }

    return (value + 1) >> 1;
}

#define mr_draw_glyph_template(name, init, loop, draw, advance)                         \
    static void name                                                                    \
    {                                                                                   \
        const mr_font_header_t *font_header =                                           \
            (const mr_font_header_t *)mr->font;                                         \
                                                                                        \
        uint8_t pixel_bitnum = font_header->glyph_pixel_bitnum;                         \
        int16_t white_value = (1 << pixel_bitnum) - 1;                                  \
                                                                                        \
        mr_bitstream_t *bitstream = &mr->glyph.bitstream;                               \
                                                                                        \
        mr_rectangle_t glyph_rectangle = {                                              \
            mr->glyph.boundingbox_left,                                                 \
            mr->glyph.boundingbox_bottom + mr->glyph.boundingbox_height,                \
            mr->glyph.boundingbox_width,                                                \
            mr->glyph.boundingbox_height};                                              \
                                                                                        \
        mr_point_t glyph_position = {                                                   \
            0,                                                                          \
            0};                                                                         \
                                                                                        \
        uint32_t repeat_length;                                                         \
        uint32_t repeat_num;                                                            \
        uint32_t repeat_index = 0;                                                      \
        mr_bitstream_t repeat_bitstream = {                                             \
            NULL,                                                                       \
            0};                                                                         \
                                                                                        \
        init;                                                                           \
                                                                                        \
        while (glyph_position.y < glyph_rectangle.height)                               \
        {                                                                               \
            uint8_t value = mr_get_unsigned_bits(bitstream, pixel_bitnum);              \
                                                                                        \
            uint16_t runlength;                                                         \
                                                                                        \
            if (!value ||                                                               \
                (value == white_value))                                                 \
            {                                                                           \
                int8_t runlength_bitnum = !value                                        \
                                              ? font_header->glyph_repeat_black_bitnum  \
                                              : font_header->glyph_repeat_white_bitnum; \
                                                                                        \
                uint16_t runlength_remainder =                                          \
                    mr_get_unsigned_bits(bitstream, runlength_bitnum);                  \
                                                                                        \
                uint16_t runlength_quotient = 0;                                        \
                while (mr_get_unsigned_bits(bitstream, 1))                              \
                    runlength_quotient++;                                               \
                                                                                        \
                runlength = (runlength_quotient << runlength_bitnum) +                  \
                            runlength_remainder;                                        \
                                                                                        \
                if (!value)                                                             \
                {                                                                       \
                    if (!runlength)                                                     \
                    {                                                                   \
                        repeat_length = 2;                                              \
                        while (mr_get_unsigned_bits(bitstream, 1))                      \
                            repeat_length++;                                            \
                                                                                        \
                        repeat_num = 1;                                                 \
                        while (mr_get_unsigned_bits(bitstream, 1))                      \
                            repeat_num++;                                               \
                                                                                        \
                        repeat_index = repeat_length;                                   \
                        repeat_bitstream = *bitstream;                                  \
                                                                                        \
                        continue;                                                       \
                    }                                                                   \
                }                                                                       \
                else                                                                    \
                    runlength++;                                                        \
            }                                                                           \
            else                                                                        \
                runlength = 1;                                                          \
                                                                                        \
            loop;                                                                       \
                                                                                        \
            while (runlength--)                                                         \
            {                                                                           \
                draw;                                                                   \
                                                                                        \
                glyph_position.x++;                                                     \
                                                                                        \
                if (glyph_position.x >= glyph_rectangle.width)                          \
                {                                                                       \
                    advance;                                                            \
                                                                                        \
                    glyph_position.x = 0;                                               \
                    glyph_position.y++;                                                 \
                }                                                                       \
            }                                                                           \
                                                                                        \
            if (repeat_index)                                                           \
            {                                                                           \
                repeat_index--;                                                         \
                                                                                        \
                if (!repeat_index &&                                                    \
                    repeat_num)                                                         \
                {                                                                       \
                    repeat_num--;                                                       \
                                                                                        \
                    repeat_index = repeat_length;                                       \
                                                                                        \
                    *bitstream = repeat_bitstream;                                      \
                }                                                                       \
            }                                                                           \
        }                                                                               \
    }

//

#define mr_draw_glyph_framebuffer_monochrome_vertical_prototype                    \
    mr_draw_glyph_framebuffer_monochrome_vertical(mr_t *mr,                        \
                                                  const mr_point_t *pen,           \
                                                  const mr_rectangle_t *rectangle, \
                                                  uint32_t buffer_pitch)

#define mr_draw_glyph_framebuffer_monochrome_vertical_init \
    mr_point_t position = {                                \
        pen->x + glyph_rectangle.x,                        \
        pen->y - glyph_rectangle.y};                       \
    int16_t start_x = position.x;

#define mr_draw_glyph_framebuffer_monochrome_vertical_loop

#define mr_draw_glyph_framebuffer_monochrome_vertical_draw           \
    if (value &&                                                     \
        mr_is_point_in_rect(&position, rectangle))                   \
    {                                                                \
        mr_point_t buffer_position = mr_rotate_point(mr, &position); \
        uint8_t *buffer = (uint8_t *)mr->buffer +                    \
                          (buffer_position.y >> 3) * buffer_pitch +  \
                          buffer_position.x;                         \
        uint8_t mask = 1 << (buffer_position.y & 0b111);             \
                                                                     \
        if (mr->stroke_color)                                        \
            *buffer |= mask;                                         \
        else                                                         \
            *buffer &= ~mask;                                        \
    }                                                                \
                                                                     \
    position.x++;

#define mr_draw_glyph_framebuffer_monochrome_vertical_advance \
    position.x = start_x;                                     \
    position.y++;

mr_draw_glyph_template(mr_draw_glyph_framebuffer_monochrome_vertical_prototype,
                       mr_draw_glyph_framebuffer_monochrome_vertical_init,
                       mr_draw_glyph_framebuffer_monochrome_vertical_loop,
                       mr_draw_glyph_framebuffer_monochrome_vertical_draw,
                       mr_draw_glyph_framebuffer_monochrome_vertical_advance);

//

#define mr_draw_glyph_framebuffer_color_prototype                    \
    mr_draw_glyph_framebuffer_color(mr_t *mr,                        \
                                    const mr_point_t *pen,           \
                                    const mr_rectangle_t *rectangle, \
                                    uint32_t buffer_pitch)

#define mr_draw_glyph_framebuffer_color_init \
    mr_point_t position = {                  \
        pen->x + glyph_rectangle.x,          \
        pen->y - glyph_rectangle.y};         \
    int16_t start_x = position.x;

#define mr_draw_glyph_framebuffer_color_loop           \
    uint8_t alpha = mr_get_alpha(pixel_bitnum, value); \
                                                       \
    mr_color_t color = mr->blend_table[alpha];

#define mr_draw_glyph_framebuffer_color_draw                         \
    if (value &&                                                     \
        mr_is_point_in_rect(&position, rectangle))                   \
    {                                                                \
        mr_point_t buffer_position = mr_rotate_point(mr, &position); \
        mr_color_t *buffer = (mr_color_t *)mr->buffer +              \
                             buffer_position.y * buffer_pitch +      \
                             buffer_position.x;                      \
                                                                     \
        *buffer = color;                                             \
    }                                                                \
                                                                     \
    position.x++;

#define mr_draw_glyph_framebuffer_color_advance \
    position.x = start_x;                       \
    position.y++;

mr_draw_glyph_template(mr_draw_glyph_framebuffer_color_prototype,
                       mr_draw_glyph_framebuffer_color_init,
                       mr_draw_glyph_framebuffer_color_loop,
                       mr_draw_glyph_framebuffer_color_draw,
                       mr_draw_glyph_framebuffer_color_advance);

//

#define mr_draw_glyph_textbuffer_prototype          \
    mr_draw_glyph_textbuffer(mr_t *mr,              \
                             const mr_point_t *pen, \
                             uint32_t buffer_pitch)

#define mr_draw_glyph_textbuffer_init                         \
    mr_point_t position = {                                   \
        pen->x + glyph_rectangle.x,                           \
        pen->y - glyph_rectangle.y};                          \
                                                              \
    if ((position.x + glyph_rectangle.width) >= buffer_pitch) \
        return;                                               \
                                                              \
    uint8_t *buffer = (uint8_t *)mr->buffer +                 \
                      buffer_pitch * position.y +             \
                      position.x;                             \
    uint8_t *buffer_line = buffer;

#define mr_draw_glyph_textbuffer_loop \
    uint8_t alpha = mr_get_alpha(pixel_bitnum, value);

#define mr_draw_glyph_textbuffer_draw \
    if (*buffer < alpha)              \
        *buffer = alpha;              \
                                      \
    buffer++;

#define mr_draw_glyph_textbuffer_advance \
    buffer_line += buffer_pitch;         \
    buffer = buffer_line;

mr_draw_glyph_template(mr_draw_glyph_textbuffer_prototype,
                       mr_draw_glyph_textbuffer_init,
                       mr_draw_glyph_textbuffer_loop,
                       mr_draw_glyph_textbuffer_draw,
                       mr_draw_glyph_textbuffer_advance);

// Text processing

static bool mr_get_glyph(mr_t *mr,
                         mr_charcode charcode)
{
    const mr_font_header_t *font_header =
        (const mr_font_header_t *)mr->font;

    // Search block
    const uint8_t *block;
    const uint8_t *block_end = mr->font + MR_FONT_HEADER_SIZE;
    mr_charcode block_charcode = -1;

    while (true)
    {
        const uint8_t *block_next = block_end;

        uint32_t block_offset =
            mr_get_variable_length_word(&block_next);
        if (block_offset == 0)
            break;

        mr_charcode block_charcode_next =
            mr_get_variable_length_word(&block_next);
        if (charcode < block_charcode_next)
            break;

        block = block_next;
        block_end = block_next + block_offset;
        block_charcode = block_charcode_next;
    }

    if (block_charcode == -1)
        return false;

    // Search glyph
    while (block < block_end)
    {
        int32_t glyph_size = mr_get_variable_length_word(&block);

        if (block_charcode == charcode)
        {
            mr_bitstream_t *bitreader = &mr->glyph.bitstream;

            bitreader->data = block;
            bitreader->bit_index = 0;

            mr->glyph.boundingbox_left =
                mr_get_signed_bits(bitreader,
                                   font_header->glyph_boundingbox_left_bitnum);
            mr->glyph.boundingbox_bottom =
                mr_get_signed_bits(bitreader,
                                   font_header->glyph_boundingbox_bottom_bitnum);
            mr->glyph.boundingbox_width =
                mr_get_unsigned_bits(bitreader,
                                     font_header->glyph_boundingbox_width_bitnum);
            mr->glyph.boundingbox_height =
                mr_get_unsigned_bits(bitreader,
                                     font_header->glyph_boundingbox_height_bitnum);
            mr->glyph.advance =
                mr_get_unsigned_bits(bitreader,
                                     font_header->glyph_advance_bitnum);

            return true;
        }

        block_charcode++;
        block += glyph_size;
    }

    return false;
}

static int16_t mr_get_string_width(mr_t *mr,
                                   const uint8_t *str,
                                   mr_get_charcode_callback_t get_charcode_callback)
{
    int16_t width = 0;

    while (true)
    {
        mr_charcode charcode = get_charcode_callback((uint8_t **)&str);

        if (!charcode)
            break;

        if (mr_get_glyph(mr, charcode))
            width += mr->glyph.advance;
    }

    return width;
}

static inline mr_rectangle_t mr_get_font_boundingbox(mr_t *mr)
{
    const mr_font_header_t *font_header =
        (const mr_font_header_t *)mr->font;

    return (mr_rectangle_t){
        mr_get_short(&font_header->font_boundingbox_left),
        mr_get_short(&font_header->font_boundingbox_bottom),
        mr_get_short(&font_header->font_boundingbox_width),
        mr_get_short(&font_header->font_boundingbox_height)};
}

#define mr_draw_string(name, init, callback)                                \
    void name(mr_t *mr,                                                     \
              const uint8_t *str,                                           \
              const mr_rectangle_t *rectangle,                              \
              const mr_point_t *offset,                                     \
              mr_get_charcode_callback_t get_charcode_callback)             \
    {                                                                       \
        mr_draw_rectangle(mr, rectangle);                                   \
                                                                            \
        init;                                                               \
                                                                            \
        mr_point_t pen = {                                                  \
            rectangle->x + offset->x,                                       \
            rectangle->y + offset->y + mr_get_ascent(mr)};                  \
                                                                            \
        while (true)                                                        \
        {                                                                   \
            mr_charcode charcode = get_charcode_callback((uint8_t **)&str); \
                                                                            \
            if (!charcode)                                                  \
                break;                                                      \
                                                                            \
            if (!mr_get_glyph(mr, charcode))                                \
                continue;                                                   \
                                                                            \
            callback(mr,                                                    \
                     &pen,                                                  \
                     rectangle,                                             \
                     mr->display_width);                                    \
                                                                            \
            pen.x += mr->glyph.advance;                                     \
        }                                                                   \
    }

#define mr_draw_string_framebuffer_monochrome_vertical_init

#define mr_draw_string_framebuffer_color_init \
    mr_update_blend_table(mr);

mr_draw_string(mr_draw_string_framebuffer_monochrome_vertical,
               mr_draw_string_framebuffer_monochrome_vertical_init,
               mr_draw_glyph_framebuffer_monochrome_vertical);
mr_draw_string(mr_draw_string_framebuffer_color,
               mr_draw_string_framebuffer_color_init,
               mr_draw_glyph_framebuffer_color);

void mr_draw_string_textbuffer(mr_t *mr,
                               const uint8_t *str,
                               const mr_rectangle_t *rectangle,
                               const mr_point_t *offset,
                               mr_get_charcode_callback_t get_charcode_callback)
{
    mr_rectangle_t font_boundingbox = mr_get_font_boundingbox(mr);

    int32_t buffer_pitch = mr->buffer_size / font_boundingbox.height;
    if (buffer_pitch < font_boundingbox.width)
        return;

    // Pen in buffer coordinates
    mr_point_t pen = {
        0,
        font_boundingbox.y + font_boundingbox.height};
    // Buffer rectangle in screen coordinates
    mr_rectangle_t buffer_rectangle = {
        rectangle->x + offset->x,
        rectangle->y + offset->y + mr_get_ascent(mr) - pen.y,
        0,
        font_boundingbox.height};
    // String rectangle in screen coordinates
    mr_rectangle_t text_rectangle = buffer_rectangle;

    mr_update_blend_table(mr);

    while (true)
    {
        mr_charcode charcode = get_charcode_callback((uint8_t **)&str);
        bool draw_buffer;
        int16_t buffer_width_increment;

        if (!charcode)
            draw_buffer = true;
        else
        {
            if (!mr_get_glyph(mr, charcode))
                continue;

            if (!text_rectangle.width)
            {
                pen.x -= mr->glyph.boundingbox_left;
                buffer_rectangle.x += mr->glyph.boundingbox_left;
                text_rectangle.x += mr->glyph.boundingbox_left;
            }

            int16_t glyph_boundingbox_right =
                mr->glyph.boundingbox_left +
                mr->glyph.boundingbox_width;

            int16_t buffer_next_width = pen.x + glyph_boundingbox_right;
            buffer_width_increment = buffer_next_width - buffer_rectangle.width;

            draw_buffer = (buffer_next_width > buffer_pitch);
        }

        if (draw_buffer)
        {
            // The buffer rectangle is divided in:
            // * A draw rectangle
            // * A move rectangle
            // * A clear rectangle

            // Calculate move rectangle width
            uint16_t move_rectangle_width;
            if (charcode &&
                (mr->glyph.boundingbox_left < 0))
            {
                move_rectangle_width = mr_min(-mr->glyph.boundingbox_left,
                                              buffer_rectangle.width);
            }
            else
                move_rectangle_width = 0;

            buffer_rectangle.width -= move_rectangle_width;

            // Send draw rectangle
            mr_rectangle_t buffer_clipped_rectangle;
            if (mr_intersect_rectangles(rectangle,
                                        &buffer_rectangle,
                                        &buffer_clipped_rectangle))
            {
                mr->draw_textbuffer_callback(mr,
                                             (uint8_t *)mr->buffer +
                                                 buffer_pitch *
                                                     (buffer_clipped_rectangle.y - buffer_rectangle.y) +
                                                 (buffer_clipped_rectangle.x - buffer_rectangle.x),
                                             buffer_pitch,
                                             &buffer_clipped_rectangle);
            }

            // Update move and clear rectangles
            uint8_t *line = mr->buffer;
            for (uint16_t y = 0; y < buffer_rectangle.height; y++)
            {
                memcpy(line, line + buffer_rectangle.width, move_rectangle_width);
                memset(line + move_rectangle_width, 0, buffer_rectangle.width);
                line += buffer_pitch;
            }

            pen.x -= buffer_rectangle.width;
            buffer_rectangle.x += buffer_rectangle.width;
            buffer_rectangle.width = move_rectangle_width;
        }

        if (!charcode)
            break;

        mr_draw_glyph_textbuffer(mr,
                                 &pen,
                                 buffer_pitch);

        pen.x += mr->glyph.advance;
        buffer_rectangle.width += buffer_width_increment;
        text_rectangle.width += buffer_width_increment;
    }

    // Draw fill rectangles
    if (!mr_intersect_rectangles(rectangle,
                                 &text_rectangle,
                                 &text_rectangle))
        mr_draw_rectangle(mr, rectangle);
    else
    {
        mr_rectangle_t fill_rectangle;

        fill_rectangle.x = rectangle->x;
        fill_rectangle.y = rectangle->y;
        fill_rectangle.width = rectangle->width;
        fill_rectangle.height = text_rectangle.y - rectangle->y;
        mr_draw_rectangle(mr, &fill_rectangle);

        fill_rectangle.y = text_rectangle.y;
        fill_rectangle.width = text_rectangle.x - rectangle->x;
        fill_rectangle.height = text_rectangle.height;
        mr_draw_rectangle(mr, &fill_rectangle);

        fill_rectangle.x = text_rectangle.x + text_rectangle.width;
        fill_rectangle.width = (rectangle->x + rectangle->width) -
                               fill_rectangle.x;
        mr_draw_rectangle(mr, &fill_rectangle);

        fill_rectangle.x = rectangle->x;
        fill_rectangle.y = text_rectangle.y + text_rectangle.height;
        fill_rectangle.width = rectangle->width;
        fill_rectangle.height = (rectangle->y + rectangle->height) -
                                fill_rectangle.y;
        mr_draw_rectangle(mr, &fill_rectangle);
    }
}

// Internal functions

void mr_init(mr_t *mr)
{
    memset(mr, 0, sizeof(mr_t));
}

// API functions

void mr_set_stroke_color(mr_t *mr,
                         mr_color_t color)
{
    mr->stroke_color = color;
}

void mr_set_fill_color(mr_t *mr,
                       mr_color_t color)
{
    mr->fill_color = color;
}

void mr_draw_rectangle(mr_t *mr,
                       const mr_rectangle_t *rectangle)
{
    mr->draw_rectangle_callback(mr, rectangle);
}

void mr_set_font(mr_t *mr,
                 const uint8_t *font)
{
    mr->font = font;
}

void mr_draw_text(mr_t *mr,
                  const char *str,
                  const mr_rectangle_t *rectangle,
                  const mr_point_t *offset)
{
    mr->draw_string_callback(mr,
                             (const uint8_t *)str,
                             rectangle,
                             offset,
                             mr_decode_c_string);
}

void mr_draw_utf8_text(mr_t *mr,
                       const uint8_t *str,
                       const mr_rectangle_t *rectangle,
                       const mr_point_t *offset)
{
    mr->draw_string_callback(mr,
                             str,
                             rectangle,
                             offset,
                             mr_decode_utf8);
}

void mr_draw_utf16_text(mr_t *mr,
                        const uint16_t *str,
                        const mr_rectangle_t *rectangle,
                        const mr_point_t *offset)
{
    mr->draw_string_callback(mr,
                             (const uint8_t *)str,
                             rectangle,
                             offset,
                             mr_decode_utf16);
}

int16_t mr_get_text_width(mr_t *mr,
                          const char *str)
{
    return mr_get_string_width(mr,
                               (const uint8_t *)str,
                               mr_decode_c_string);
}

int16_t mr_get_utf8_text_width(mr_t *mr,
                               const uint8_t *str)
{
    return mr_get_string_width(mr,
                               str,
                               mr_decode_utf8);
}

int16_t mr_get_utf16_text_width(mr_t *mr,
                                const uint16_t *str)
{
    return mr_get_string_width(mr,
                               (uint8_t *)str,
                               mr_decode_utf16);
}

int16_t mr_get_cap_height(mr_t *mr)
{
    const mr_font_header_t *font_header =
        (const mr_font_header_t *)mr->font;

    return mr_get_short(&font_header->font_cap_height);
}

int16_t mr_get_ascent(mr_t *mr)
{
    const mr_font_header_t *font_header =
        (const mr_font_header_t *)mr->font;

    return mr_get_short(&font_header->font_ascent);
}

int16_t mr_get_descent(mr_t *mr)
{
    const mr_font_header_t *font_header =
        (const mr_font_header_t *)mr->font;

    return mr_get_short(&font_header->font_descent);
}

int16_t mr_get_line_height(mr_t *mr)
{
    return mr_get_ascent(mr) + mr_get_descent(mr);
}

#if defined(MCURENDERER_BITMAP_SUPPORT)
void mr_draw_bitmap(mr_t *mr,
                    const mr_rectangle_t *rectangle,
                    const uint8_t *bitmap)
{
    mr->draw_bitmap_callback(mr, rectangle, bitmap);
}
#endif

#if defined(MCURENDERER_IMAGE_SUPPORT)
void mr_draw_image(mr_t *mr,
                   const mr_rectangle_t *rectangle,
                   const mr_color_t *image)
{
    mr->draw_image_callback(mr, rectangle, image);
}
#endif
