/**
 * MCU renderer
 * Hardware-independent, low-resource graphics library for MCUs
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#if !defined(MCURENDERER_H)
#define MCURENDERER_H

#include <stdbool.h>
#include <stdint.h>

// Switches (defined as compiler define parameters)

#if !defined(MCURENDERER_WITHOUT_IMAGE_SUPPORT)
#define MCURENDERER_IMAGE_SUPPORT
#endif

// Instance

struct mr_t_;

typedef struct mr_t_ mr_t;

// Color

typedef uint16_t mr_color_t;

#define COLOR_BLEND_TABLE_SIZE ((1 << 5) + 1)

/**
 * Macro that converts an RGB888 color code to an RGB565 color.
 *
 * @param color_code The RGB888 color code.
 *
 * @return The RGB565 color.
 */
#define mr_get_color(color_code)                                  \
    (((((color_code >> 16) & 0xff) * 249 + 1014) >> 11) << 11) |  \
        (((((color_code >> 8) & 0xff) * 253 + 505) >> 10) << 5) | \
        (((((color_code >> 0) & 0xff) * 249 + 1014) >> 11) << 0)

// Geometry

typedef struct
{
    int16_t x;
    int16_t y;
} mr_point_t;

typedef struct
{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
} mr_rectangle_t;

typedef enum
{
    MR_DISPLAY_ROTATION_0,
    MR_DISPLAY_ROTATION_90,
    MR_DISPLAY_ROTATION_180,
    MR_DISPLAY_ROTATION_270,
} mr_display_rotation_t;

mr_rectangle_t mr_rotate_rectangle(mr_t *mr,
                                   const mr_rectangle_t *rectangle);

void mr_draw_rectangle_framebuffer_monochrome_vertical(mr_t *mr,
                                                       const mr_rectangle_t *rectangle);
void mr_draw_rectangle_framebuffer_color(mr_t *mr,
                                         const mr_rectangle_t *rectangle);

// Text

typedef struct
{
    const uint8_t *data;
    uint8_t bit_index;
} mr_bitstream_t;

typedef struct
{
    mr_bitstream_t bitstream;

    int8_t boundingbox_left;
    int8_t boundingbox_bottom;
    uint8_t boundingbox_width;
    uint8_t boundingbox_height;
    uint8_t advance;
} mr_glyph_t;

typedef int32_t mr_charcode;

typedef mr_charcode (*mr_get_charcode_callback_t)(uint8_t **str);

void mr_draw_string_framebuffer_monochrome_vertical(mr_t *mr,
                                                    const uint8_t *str,
                                                    const mr_rectangle_t *rectangle,
                                                    const mr_point_t *offset,
                                                    mr_get_charcode_callback_t get_charcode_callback);
void mr_draw_string_framebuffer_color(mr_t *mr,
                                      const uint8_t *str,
                                      const mr_rectangle_t *rectangle,
                                      const mr_point_t *offset,
                                      mr_get_charcode_callback_t get_charcode_callback);
void mr_draw_string_textbuffer(mr_t *mr,
                               const uint8_t *str,
                               const mr_rectangle_t *rectangle,
                               const mr_point_t *offset,
                               mr_get_charcode_callback_t get_charcode_callback);

void mr_draw_image_framebuffer_monochrome_vertical(mr_t *mr,
                                                   const mr_rectangle_t *rectangle,
                                                   const mr_color_t *image);
void mr_draw_image_framebuffer_color(mr_t *mr,
                                     const mr_rectangle_t *rectangle,
                                     const mr_color_t *image);

// I/O

typedef void (*mr_sleep_callback_t)(uint32_t value);
typedef void (*mr_set_reset_callback_t)(bool value);
typedef void (*mr_set_command_callback_t)(bool value);
typedef void (*mr_send_callback_t)(uint16_t value);

void mr_send_command(mr_t *mr,
                     uint8_t command);

void mr_send_data(mr_t *mr,
                  uint8_t value);

typedef enum
{
    MR_SEQ_SLEEP,
    MR_SEQ_SET_RESET,
    MR_SEQ_SEND_COMMAND,
    MR_SEQ_SEND_DATA,
    MR_SEQ_END,
} mr_sequence_t;

#define MR_SLEEP(value) (MR_SEQ_SLEEP), (value)
#define MR_SET_RESET(value) (MR_SEQ_SET_RESET), (value)
#define MR_SEND_COMMAND(value) (MR_SEQ_SEND_COMMAND), (value)
#define MR_SEND_DATA(value) (MR_SEQ_SEND_DATA), (value)
#define MR_END() (MR_SEQ_END)

void mr_send_sequence(mr_t *mr,
                      const uint8_t *sequence);

// Common

typedef void (*mr_draw_rectangle_callback_t)(mr_t *mr,
                                             const mr_rectangle_t *rectangle);
typedef void (*mr_draw_image_callback_t)(mr_t *mr,
                                         const mr_rectangle_t *rectangle,
                                         const mr_color_t *image);
typedef void (*mr_draw_string_callback_t)(mr_t *mr,
                                          const uint8_t *str,
                                          const mr_rectangle_t *rectangle,
                                          const mr_point_t *offset,
                                          mr_get_charcode_callback_t get_charcode_callback);
typedef void (*mr_draw_glyph_callback_t)(mr_t *mr,
                                         mr_point_t pen);
typedef void (*mr_draw_textbuffer_callback_t)(mr_t *mr,
                                              uint8_t *buffer,
                                              uint32_t buffer_pitch,
                                              mr_rectangle_t *rectangle);

struct mr_t_
{
    mr_draw_rectangle_callback_t draw_rectangle_callback;
#if defined(MCURENDERER_IMAGE_SUPPORT)
    mr_draw_image_callback_t draw_image_callback;
#endif
    mr_draw_string_callback_t draw_string_callback;
    mr_draw_glyph_callback_t draw_glyph_callback;
    mr_draw_textbuffer_callback_t draw_textbuffer_callback;
    mr_sleep_callback_t sleep_callback;
    mr_set_reset_callback_t set_reset_callback;
    mr_set_command_callback_t set_command_callback;
    mr_send_callback_t send_callback;
    mr_send_callback_t send16_callback;

    int16_t display_width;
    int16_t display_height;
    mr_display_rotation_t display_rotation;
    void *display;

    void *buffer;
    uint32_t buffer_size;
    uint32_t buffer_pitch;

    mr_color_t fill_color;

    mr_color_t text_color;
    mr_color_t blend_table[COLOR_BLEND_TABLE_SIZE];
    const uint8_t *font;
    mr_glyph_t glyph;
};

void mr_init(mr_t *mr);

inline void mr_set_command(mr_t *mr,
                           bool value)
{
    mr->set_command_callback(value);
}

inline void mr_send(mr_t *mr,
                    uint8_t value)
{
    mr->send_callback(value);
}

inline void mr_send16(mr_t *mr,
                      uint16_t value)
{
    mr->send16_callback(value);
}

// API functions

/**
 * Sets the fill color.
 *
 * @param mr The mcu-renderer instance.
 * @param color The fill color.
 */
void mr_set_fill_color(mr_t *mr,
                       mr_color_t color);

/**
 * Draws a filled rectangle.
 *
 * @param mr The mcu-renderer instance.
 * @param rectangle The rectangle.
 */
void mr_draw_rectangle(mr_t *mr,
                       const mr_rectangle_t *rectangle);

/**
 * Draws an image.
 *
 * @param mr The mcu-renderer instance.
 * @param rectangle The rectangle.
 * @param image The image data.
 */
void mr_draw_image(mr_t *mr,
                   const mr_rectangle_t *rectangle,
                   const mr_color_t *image);

/**
 * Sets the font.
 *
 * @param mr The mcu-renderer instance.
 * @param font The font.
 */
void mr_set_font(mr_t *mr,
                 const uint8_t *font);

/**
 * Sets the text color.
 *
 * @param mr The mcu-renderer instance.
 * @param font The text color.
 */
void mr_set_text_color(mr_t *mr,
                       mr_color_t color);

/**
 * Draws a C-string.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 * @param rectangle The rectangle containing the text.
 * @param offset The upper left corner of the text relative to the text rectangle.
 */
void mr_draw_text(mr_t *mr,
                  const char *str,
                  const mr_rectangle_t *rectangle,
                  const mr_point_t *offset);

/**
 * Draws a UTF-8 string.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 * @param rectangle The rectangle containing the text.
 * @param offset The upper left corner of the text relative to the text rectangle.
 */
void mr_draw_utf8_text(mr_t *mr,
                       const uint8_t *str,
                       const mr_rectangle_t *rectangle,
                       const mr_point_t *offset);

/**
 * Draws a UTF-16 string.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 * @param rectangle The rectangle containing the text.
 * @param offset The upper left corner of the text relative to the text rectangle.
 */
void mr_draw_utf16_text(mr_t *mr,
                        const uint16_t *str,
                        const mr_rectangle_t *rectangle,
                        const mr_point_t *offset);

/**
 * Returns the pixel width of a C-string.
 *
 * The pixel width is calculated from initial to final pen position.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 *
 * @return The pixel width.
 */
int16_t mr_get_text_width(mr_t *mr,
                          const char *str);

/**
 * Returns the pixel width of a UTF-8 string.
 *
 * The pixel width is calculated from initial to final pen position.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 *
 * @return The pixel width.
 */
int16_t mr_get_utf8_text_width(mr_t *mr,
                               const uint8_t *str);

/**
 * Returns the pixel width of a UTF-16 string.
 *
 * The pixel width is calculated from initial to final pen position.
 *
 * @param mr The mcu-renderer instance.
 * @param str The string.
 *
 * @return The pixel width.
 */
int16_t mr_get_utf16_text_width(mr_t *mr,
                                const uint16_t *str);

/**
 * Returns the current font's cap height (height of uppercase A).
 *
 * @param mr The mcu-renderer instance.
 *
 * @return The cap height.
 */
int16_t mr_get_cap_height(mr_t *mr);

/**
 * Returns the current font's ascent (baseline to top of line).
 *
 * @param mr The mcu-renderer instance.
 *
 * @return The ascent.
 */
int16_t mr_get_ascent(mr_t *mr);

/**
 * Returns the current font's descent (bottom of line to baseline).
 *
 * @param mr The mcu-renderer instance.
 *
 * @return The descent.
 */
int16_t mr_get_descent(mr_t *mr);

/**
 * Returns the current font's line height (ascent + descent).
 *
 * @param mr The mcu-renderer instance.
 *
 * @return The line height.
 */
int16_t mr_get_line_height(mr_t *mr);

#endif
