/*
 * MCU renderer
 * Core module
 *
 * (C) 2023-2026 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "mcu-renderer.h"

#define MR_FONT_HEADER_SIZE 22

// Forces inlining of per-pixel helpers, which -Os would otherwise keep out of
// line when they have several callers
#if defined(__GNUC__)
#define MR_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define MR_ALWAYS_INLINE __forceinline
#else
#define MR_ALWAYS_INLINE inline
#endif

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

static inline const mr_font_header_t *mr_get_font_header(mr_t *mr)
{
    return (const mr_font_header_t *)mr->font;
}

// Text decoding

static mr_charcode mr_decode_c_string(const uint8_t **strp)
{
    const uint8_t *str = *strp;
    *strp += 1;

    return *str;
}

static mr_charcode mr_decode_utf8(const uint8_t **strp)
{
    const uint8_t *str = *strp;
    uint8_t lead = str[0];
    uint32_t length;
    mr_charcode codepoint;

    if (!(lead >> 7))
    {
        length = 1;
        codepoint = lead & 0x7f;
    }
    else if ((lead >> 5) == 0x06)
    {
        length = 2;
        codepoint = lead & 0x1f;
    }
    else if ((lead >> 4) == 0x0e)
    {
        length = 3;
        codepoint = lead & 0x0f;
    }
    else if ((lead >> 3) == 0x1e)
    {
        length = 4;
        codepoint = lead & 0x07;
    }
    else
        return 0;

    for (uint32_t i = 1; i < length; i++)
    {
        if ((str[i] >> 6) != 0x02)
            return 0;
        codepoint = (codepoint << 6) | (str[i] & 0x3f);
    }

    *strp += length;

    return codepoint;
}

static mr_charcode mr_decode_utf16(const uint8_t **strp)
{
    const uint16_t *str = (const uint16_t *)*strp;
    uint16_t highSurrogate = str[0];

    if ((highSurrogate >> 11) != 0x1b)
    {
        *strp += 2;

        return highSurrogate;
    }
    else
    {
        uint16_t lowSurrogate = str[1];
        if (((highSurrogate >> 10) != 0x36) &&
            ((lowSurrogate >> 10) != 0x37))
            return 0;

        *strp += 4;

        return (0x10000 +
                ((highSurrogate & 0x3ff) << 10 |
                 (lowSurrogate & 0x3ff)));
    }
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

// Framebuffer pixels

// Sets a monochrome framebuffer pixel. A color is on when its most
// significant bit is set.
static MR_ALWAYS_INLINE void mr_set_pixel_framebuffer_monochrome_vertical(mr_t *mr,
                                                                          const mr_point_t *position,
                                                                          mr_color_t color)
{
    mr_point_t buffer_position = mr_rotate_point(mr, position);
    uint8_t *buffer = (uint8_t *)mr->buffer +
                      (buffer_position.y >> 3) * mr->display_width +
                      buffer_position.x;
    uint8_t mask = 1 << (buffer_position.y & 0b111);

    if (color >> 15)
        *buffer |= mask;
    else
        *buffer &= ~mask;
}

static MR_ALWAYS_INLINE void mr_set_pixel_framebuffer_color(mr_t *mr,
                                                            const mr_point_t *position,
                                                            mr_color_t color)
{
    mr_point_t buffer_position = mr_rotate_point(mr, position);
    mr_color_t *buffer = (mr_color_t *)mr->buffer +
                         buffer_position.y * mr->display_width +
                         buffer_position.x;

    *buffer = color;
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
            mr_set_pixel_framebuffer_monochrome_vertical(mr, &position, mr->fill_color);
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
            mr_set_pixel_framebuffer_color(mr, &position, mr->fill_color);
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
            mr_set_pixel_framebuffer_monochrome_vertical(mr,
                                                         &position,
                                                         mr_get_bitmap_pixel(bitmap, source_index++)
                                                             ? mr->stroke_color
                                                             : mr->fill_color);

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
            mr_set_pixel_framebuffer_color(mr,
                                           &position,
                                           mr_get_bitmap_pixel(bitmap, source_index++)
                                               ? mr->stroke_color
                                               : mr->fill_color);

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
            mr_set_pixel_framebuffer_monochrome_vertical(mr, &position, *image++);
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
            mr_set_pixel_framebuffer_color(mr, &position, *image++);
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
        (mr->blend_table[MR_COLOR_BLEND_TABLE_SIZE - 1] == mr->stroke_color))
        return;

    for (int i = 0; i < MR_COLOR_BLEND_TABLE_SIZE; i++)
        mr->blend_table[i] = mr_fast_blend(mr->stroke_color,
                                           mr->fill_color,
                                           i);
}

static inline uint8_t mr_get_alpha(uint8_t pixel_bitnum,
                                   uint8_t value)
{
    // Scale to 6 bits by replicating the value's bits
    uint8_t alpha = 0;

    for (int8_t shift = 6 - pixel_bitnum;
         shift > -pixel_bitnum;
         shift -= pixel_bitnum)
        alpha |= (shift >= 0) ? (value << shift) : (value >> -shift);

    return (alpha + 1) >> 1;
}

// Glyph decoding

typedef struct
{
    const mr_font_header_t *font_header;
    mr_bitstream_t bitstream;

    // Repeat state: the last repeat_length runs are repeated repeat_num times
    uint32_t repeat_length;
    uint32_t repeat_num;
    uint32_t repeat_index;
    mr_bitstream_t repeat_bitstream;
} mr_glyph_decoder_t;

static void mr_init_glyph_decoder(mr_t *mr,
                                  mr_glyph_decoder_t *decoder)
{
    decoder->font_header = mr_get_font_header(mr);
    decoder->bitstream = mr->glyph.bitstream;
    decoder->repeat_index = 0;
}

// Decodes the next run of equal pixels of the current glyph.
// Returns the run length and sets *value to the pixel value.
static uint16_t mr_get_glyph_run(mr_glyph_decoder_t *decoder,
                                 uint8_t *value)
{
    const mr_font_header_t *font_header = decoder->font_header;
    mr_bitstream_t *bitstream = &decoder->bitstream;

    uint8_t pixel_bitnum = font_header->glyph_pixel_bitnum;
    int16_t white_value = (1 << pixel_bitnum) - 1;

    uint16_t runlength;

    while (true)
    {
        *value = mr_get_unsigned_bits(bitstream, pixel_bitnum);

        if (*value &&
            (*value != white_value))
        {
            runlength = 1;

            break;
        }

        // Black or white run
        int8_t runlength_bitnum = !*value
                                      ? font_header->glyph_repeat_black_bitnum
                                      : font_header->glyph_repeat_white_bitnum;

        uint16_t runlength_remainder =
            mr_get_unsigned_bits(bitstream, runlength_bitnum);

        uint16_t runlength_quotient = 0;
        while (mr_get_unsigned_bits(bitstream, 1))
            runlength_quotient++;

        runlength = (runlength_quotient << runlength_bitnum) +
                    runlength_remainder;

        if (*value)
        {
            runlength++;

            break;
        }

        if (runlength)
            break;

        // Repeat marker: the following runs are repeated
        decoder->repeat_length = 2;
        while (mr_get_unsigned_bits(bitstream, 1))
            decoder->repeat_length++;

        decoder->repeat_num = 1;
        while (mr_get_unsigned_bits(bitstream, 1))
            decoder->repeat_num++;

        decoder->repeat_index = decoder->repeat_length;
        decoder->repeat_bitstream = *bitstream;
    }

    if (decoder->repeat_index)
    {
        decoder->repeat_index--;

        if (!decoder->repeat_index &&
            decoder->repeat_num)
        {
            decoder->repeat_num--;

            decoder->repeat_index = decoder->repeat_length;

            *bitstream = decoder->repeat_bitstream;
        }
    }

    return runlength;
}

// Returns the screen position of the current glyph's top-left pixel
static mr_point_t mr_get_glyph_position(mr_t *mr,
                                        const mr_point_t *pen)
{
    return (mr_point_t){
        pen->x + mr->glyph.boundingbox_left,
        pen->y - (mr->glyph.boundingbox_bottom + mr->glyph.boundingbox_height)};
}

// Glyph drawing

static void mr_draw_glyph_framebuffer_monochrome_vertical(mr_t *mr,
                                                          const mr_point_t *pen,
                                                          const mr_rectangle_t *rectangle)
{
    mr_glyph_decoder_t decoder;
    mr_init_glyph_decoder(mr, &decoder);

    mr_point_t position = mr_get_glyph_position(mr, pen);
    int16_t start_x = position.x;
    int16_t x = 0;
    int16_t y = 0;

    while (y < mr->glyph.boundingbox_height)
    {
        uint8_t value;
        uint16_t runlength = mr_get_glyph_run(&decoder, &value);

        while (runlength--)
        {
            if (value &&
                mr_is_point_in_rect(&position, rectangle))
                mr_set_pixel_framebuffer_monochrome_vertical(mr, &position, mr->stroke_color);

            position.x++;

            if (++x >= mr->glyph.boundingbox_width)
            {
                position.x = start_x;
                position.y++;

                x = 0;
                y++;
            }
        }
    }
}

static void mr_draw_glyph_framebuffer_color(mr_t *mr,
                                            const mr_point_t *pen,
                                            const mr_rectangle_t *rectangle)
{
    mr_glyph_decoder_t decoder;
    mr_init_glyph_decoder(mr, &decoder);

    uint8_t pixel_bitnum = decoder.font_header->glyph_pixel_bitnum;

    mr_point_t position = mr_get_glyph_position(mr, pen);
    int16_t start_x = position.x;
    int16_t x = 0;
    int16_t y = 0;

    while (y < mr->glyph.boundingbox_height)
    {
        uint8_t value;
        uint16_t runlength = mr_get_glyph_run(&decoder, &value);

        mr_color_t color = mr->blend_table[mr_get_alpha(pixel_bitnum, value)];

        while (runlength--)
        {
            if (value &&
                mr_is_point_in_rect(&position, rectangle))
                mr_set_pixel_framebuffer_color(mr, &position, color);

            position.x++;

            if (++x >= mr->glyph.boundingbox_width)
            {
                position.x = start_x;
                position.y++;

                x = 0;
                y++;
            }
        }
    }
}

// Draws the glyph's alpha values into the text buffer, keeping the maximum
// where glyphs overlap
static void mr_draw_glyph_textbuffer(mr_t *mr,
                                     const mr_point_t *pen,
                                     uint32_t buffer_pitch)
{
    mr_point_t position = mr_get_glyph_position(mr, pen);

    // Glyph doesn't fit in the text buffer
    if ((position.x + mr->glyph.boundingbox_width) >= buffer_pitch)
        return;

    mr_glyph_decoder_t decoder;
    mr_init_glyph_decoder(mr, &decoder);

    uint8_t pixel_bitnum = decoder.font_header->glyph_pixel_bitnum;

    uint8_t *buffer_line = (uint8_t *)mr->buffer +
                           buffer_pitch * position.y +
                           position.x;
    uint8_t *buffer = buffer_line;
    int16_t x = 0;
    int16_t y = 0;

    while (y < mr->glyph.boundingbox_height)
    {
        uint8_t value;
        uint16_t runlength = mr_get_glyph_run(&decoder, &value);

        uint8_t alpha = mr_get_alpha(pixel_bitnum, value);

        while (runlength--)
        {
            if (*buffer < alpha)
                *buffer = alpha;

            buffer++;

            if (++x >= mr->glyph.boundingbox_width)
            {
                buffer_line += buffer_pitch;
                buffer = buffer_line;

                x = 0;
                y++;
            }
        }
    }
}

// Text processing

static bool mr_get_glyph(mr_t *mr,
                         mr_charcode charcode)
{
    const mr_font_header_t *font_header = mr_get_font_header(mr);

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

            // Fields: left, bottom (signed), width, height, advance (unsigned)
            const uint8_t *field_bitnum =
                &font_header->glyph_boundingbox_left_bitnum;
            uint8_t *field = (uint8_t *)&mr->glyph.boundingbox_left;

            for (int i = 0; i < 5; i++)
                field[i] = (i < 2)
                               ? mr_get_signed_bits(bitreader, field_bitnum[i])
                               : mr_get_unsigned_bits(bitreader, field_bitnum[i]);

            return true;
        }

        block_charcode++;
        block += glyph_size;
    }

    return false;
}

// Decodes the next character and loads its glyph, skipping characters missing
// from the font. Returns false at the end of the string.
static bool mr_get_next_glyph(mr_t *mr,
                              const uint8_t **str,
                              mr_get_charcode_callback_t get_charcode_callback)
{
    while (true)
    {
        mr_charcode charcode = get_charcode_callback(str);

        if (!charcode)
            return false;

        if (mr_get_glyph(mr, charcode))
            return true;
    }
}

static int16_t mr_get_string_width(mr_t *mr,
                                   const uint8_t *str,
                                   mr_get_charcode_callback_t get_charcode_callback)
{
    int16_t width = 0;

    while (mr_get_next_glyph(mr, &str, get_charcode_callback))
        width += mr->glyph.advance;

    return width;
}

static inline mr_rectangle_t mr_get_font_boundingbox(mr_t *mr)
{
    const mr_font_header_t *font_header = mr_get_font_header(mr);

    return (mr_rectangle_t){
        mr_get_short(&font_header->font_boundingbox_left),
        mr_get_short(&font_header->font_boundingbox_bottom),
        mr_get_short(&font_header->font_boundingbox_width),
        mr_get_short(&font_header->font_boundingbox_height)};
}

typedef void (*mr_draw_framebuffer_glyph_t)(mr_t *mr,
                                           const mr_point_t *pen,
                                           const mr_rectangle_t *rectangle);

static bool mr_draw_string_framebuffer(mr_t *mr,
                                       const uint8_t *str,
                                       const mr_rectangle_t *rectangle,
                                       const mr_point_t *offset,
                                       mr_get_charcode_callback_t get_charcode_callback,
                                       mr_draw_framebuffer_glyph_t draw_glyph)
{
    mr_draw_rectangle(mr, rectangle);

    mr_point_t pen = {
        rectangle->x + offset->x,
        rectangle->y + offset->y + mr_get_ascent(mr)};

    while (mr_get_next_glyph(mr, &str, get_charcode_callback))
    {
        draw_glyph(mr,
                   &pen,
                   rectangle);

        pen.x += mr->glyph.advance;
    }

    return true;
}

bool mr_draw_string_framebuffer_monochrome_vertical(mr_t *mr,
                                                    const uint8_t *str,
                                                    const mr_rectangle_t *rectangle,
                                                    const mr_point_t *offset,
                                                    mr_get_charcode_callback_t get_charcode_callback)
{
    return mr_draw_string_framebuffer(mr,
                                      str,
                                      rectangle,
                                      offset,
                                      get_charcode_callback,
                                      mr_draw_glyph_framebuffer_monochrome_vertical);
}

bool mr_draw_string_framebuffer_color(mr_t *mr,
                                      const uint8_t *str,
                                      const mr_rectangle_t *rectangle,
                                      const mr_point_t *offset,
                                      mr_get_charcode_callback_t get_charcode_callback)
{
    mr_update_blend_table(mr);

    return mr_draw_string_framebuffer(mr,
                                      str,
                                      rectangle,
                                      offset,
                                      get_charcode_callback,
                                      mr_draw_glyph_framebuffer_color);
}

bool mr_draw_string_textbuffer(mr_t *mr,
                               const uint8_t *str,
                               const mr_rectangle_t *rectangle,
                               const mr_point_t *offset,
                               mr_get_charcode_callback_t get_charcode_callback)
{
    mr_rectangle_t font_boundingbox = mr_get_font_boundingbox(mr);

    // The text buffer must hold at least one font bounding box
    int32_t buffer_pitch = mr->buffer_size / font_boundingbox.height;
    if (buffer_pitch < font_boundingbox.width)
        return false;

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
        bool has_glyph = mr_get_next_glyph(mr, &str, get_charcode_callback);
        bool draw_buffer;
        int16_t buffer_width_increment;

        if (!has_glyph)
            draw_buffer = true;
        else
        {
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

            draw_buffer = (buffer_next_width >= buffer_pitch);
        }

        if (draw_buffer)
        {
            // The buffer rectangle is divided in:
            // * A draw rectangle
            // * A move rectangle
            // * A clear rectangle

            // Calculate move rectangle width
            uint16_t move_rectangle_width;
            if (has_glyph &&
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

        if (!has_glyph)
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

    return true;
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

bool mr_draw_text(mr_t *mr,
                  const char *str,
                  const mr_rectangle_t *rectangle,
                  const mr_point_t *offset)
{
    return mr->draw_string_callback(mr,
                                    (const uint8_t *)str,
                                    rectangle,
                                    offset,
                                    mr_decode_c_string);
}

bool mr_draw_utf8_text(mr_t *mr,
                       const uint8_t *str,
                       const mr_rectangle_t *rectangle,
                       const mr_point_t *offset)
{
    return mr->draw_string_callback(mr,
                                    str,
                                    rectangle,
                                    offset,
                                    mr_decode_utf8);
}

bool mr_draw_utf16_text(mr_t *mr,
                        const uint16_t *str,
                        const mr_rectangle_t *rectangle,
                        const mr_point_t *offset)
{
    return mr->draw_string_callback(mr,
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
    return mr_get_short(&mr_get_font_header(mr)->font_cap_height);
}

int16_t mr_get_ascent(mr_t *mr)
{
    return mr_get_short(&mr_get_font_header(mr)->font_ascent);
}

int16_t mr_get_descent(mr_t *mr)
{
    return mr_get_short(&mr_get_font_header(mr)->font_descent);
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
