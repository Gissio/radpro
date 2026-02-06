/*
 * Rad Pro
 * UI draw
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(UI_DRAW_H)
#define UI_DRAW_H

#include <mcu-renderer.h>

#include "../devices/display.h"

// Colors

typedef enum
{
    COLOR_ELEMENT_ACTIVE,
    COLOR_ELEMENT_NEUTRAL,

    COLOR_CONTAINER_BACKGROUND,
    COLOR_CONTAINER_GLOBAL,
    COLOR_CONTAINER_GLOBAL_SHADOW,

    COLOR_AMPLIFIED_ENABLED_BACKGROUND,
    COLOR_ON_FLAT_ACTIVE,

    COLOR_RUNNING,
    COLOR_WARNING,
    COLOR_ALARM,

    COLOR_INSTRUMENT_FRAME_PRIMARY,
    COLOR_INSTRUMENT_FRAME_TERTIARY,

    COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY_WARNING,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_WARNING,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALARM,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_ALARM,

#if defined(GAME)
    COLOR_SELECTED_ENABLED_BACKGROUND,
    COLOR_GAME_SQUARE_BLACK,
    COLOR_GAME_SQUARE_WHITE,
    COLOR_GAME_PIECE_BLACK,
    COLOR_GAME_PIECE_WHITE,
#endif
} ColorIndex;

// Metrics

#if defined(DISPLAY_128X64)
#define DISPLAY_LANDSCAPE
#define PADDING_LEFT 3
#define PADDING_RIGHT (PADDING_LEFT - 1)
#define CONTENT_MULTILINE_MAX 6
#define TITLEBAR_HEIGHT 8
#define TITLEBAR_SHADOW_HEIGHT 0
#define TICKS_LONG 3
#define TICKS_SHORT 2
#define TICKS_SPACING 1
#define BAR_THICKNESS 9
#elif defined(DISPLAY_320X240)
#define DISPLAY_LANDSCAPE
#define PADDING_LEFT 12
#define PADDING_RIGHT PADDING_LEFT
#define CONTENT_MULTILINE_MAX 9
#define TITLEBAR_HEIGHT 48
#define TITLEBAR_SHADOW_HEIGHT 1
#define TICKS_SPACING 4
#define TICKS_SHORT 4
#define TICKS_LONG 10
#define BAR_THICKNESS 22
#elif defined(DISPLAY_240X320) && !defined(DISPLAY_125PPI)
#define DISPLAY_PORTRAIT
#define PADDING_LEFT 12
#define PADDING_RIGHT PADDING_LEFT
#define CONTENT_MULTILINE_MAX 9
#define TITLEBAR_HEIGHT 48
#define TITLEBAR_SHADOW_HEIGHT 1
#define TICKS_SPACING 4
#define TICKS_SHORT 4
#define TICKS_LONG 8
#define BAR_THICKNESS 18
#elif defined(DISPLAY_240X320) && defined(DISPLAY_125PPI)
#define DISPLAY_PORTRAIT
#define PADDING_LEFT 8
#define PADDING_RIGHT PADDING_LEFT
#define CONTENT_MULTILINE_MAX 9
#define TITLEBAR_HEIGHT 48
#define TITLEBAR_SHADOW_HEIGHT 1
#define TICKS_SPACING 4
#define TICKS_SHORT 4
#define TICKS_LONG 10
#define BAR_THICKNESS 22
#endif

#if !defined(FONT_LARGE_LINE_HEIGHT)
#if defined(DISPLAY_128X64)
#define FONT_LARGE_ASCENT 50
#define FONT_LARGE_CAP_HEIGHT 33
#define FONT_LARGE_LINE_HEIGHT 64
#define FONT_MEDIUM_LINE_HEIGHT 14
#define FONT_SMALL_DESCENT 2
#define FONT_SMALL_LINE_HEIGHT 9
#define FONT_SYMBOLS_LINE_HEIGHT 8
#elif defined(DISPLAY_320X240)
#define FONT_LARGE_ASCENT 123
#define FONT_LARGE_CAP_HEIGHT 82
#define FONT_LARGE_LINE_HEIGHT 157
#define FONT_MEDIUM_LINE_HEIGHT 45
#define FONT_SMALL_DESCENT 7
#define FONT_SMALL_LINE_HEIGHT 30
#define FONT_SYMBOLS_LINE_HEIGHT 24
#elif defined(DISPLAY_240X320) && !defined(DISPLAY_125PPI)
#define FONT_LARGE_ASCENT 90
#define FONT_LARGE_CAP_HEIGHT 60
#define FONT_LARGE_LINE_HEIGHT 115
#define FONT_MEDIUM_LINE_HEIGHT 45
#define FONT_SMALL_DESCENT 7
#define FONT_SMALL_LINE_HEIGHT 30
#define FONT_SYMBOLS_LINE_HEIGHT 24
#elif defined(DISPLAY_240X320) && defined(DISPLAY_125PPI)
#define FONT_LARGE_ASCENT 90
#define FONT_LARGE_CAP_HEIGHT 60
#define FONT_LARGE_LINE_HEIGHT 115
#define FONT_MEDIUM_LINE_HEIGHT 34
#define FONT_SMALL_DESCENT 5
#define FONT_SMALL_LINE_HEIGHT 23
#define FONT_SYMBOLS_LINE_HEIGHT 24
#endif
#endif

#define TITLEBAR_LEFT 0
#define TITLEBAR_TOP 0
#define TITLEBAR_WIDTH DISPLAY_WIDTH
#define TITLEBAR_BOTTOM (TITLEBAR_TOP + TITLEBAR_HEIGHT)
#define TITLEBAR_CONTENT_HEIGHT (TITLEBAR_HEIGHT - TITLEBAR_SHADOW_HEIGHT)

#define CONTENT_LEFT 0
#define CONTENT_TOP TITLEBAR_BOTTOM
#define CONTENT_WIDTH DISPLAY_WIDTH
#define CONTENT_HEIGHT (DISPLAY_HEIGHT - TITLEBAR_BOTTOM)
#define CONTENT_RIGHT (CONTENT_LEFT + CONTENT_WIDTH)
#define CONTENT_BOTTOM (CONTENT_TOP + CONTENT_HEIGHT)

extern const uint8_t font_large[];
extern const uint8_t font_medium[];
extern const uint8_t font_small[];
extern const uint8_t font_symbols[];

extern const mr_rectangle_t contentRectangle;

mr_color_t getFillColor(ColorIndex colorIndex);
void setFillColor(ColorIndex colorIndex);
void setStrokeColor(ColorIndex colorIndex);

void drawRectangle(const mr_rectangle_t *rectangle);
void ditherRectangle(const mr_rectangle_t *rectangle, bool darker);

void drawImage(const mr_rectangle_t *rectangle, const mr_color_t *imageBuffer);

void setFont(const uint8_t *font);
uint16_t getTextWidth(const char *s);
uint16_t getTextHeight(void);
void drawText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset);
void drawCenteredText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset);
void drawRightAlignedText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset);
void drawCenteredMultilineText(const mr_rectangle_t *rectangle, const char *message);
void drawRowLeft(const char *buffer, mr_rectangle_t *rectangle);
void drawRowRight(const char *buffer, mr_rectangle_t *rectangle);

void setupBatteryIcon(char *buffer);

void drawValueAndUnit(const char *valueString, const char *unitString, const mr_rectangle_t *rectangle, const mr_point_t *offset, bool isHold);

#endif
