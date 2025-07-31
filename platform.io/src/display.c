/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include <mcu-renderer.h>

#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "rtc.h"
#include "settings.h"
#include "sound.h"
#include "system.h"
#include "tube.h"
#include "view.h"

// Defines

#include FONT_SMALL
#include FONT_MEDIUM
#include FONT_LARGE
#include FONT_SYMBOLS

// Landscape layout:

//
//              Value
// ----------+--------+-----------
//           |        |       Unit
// AltKey    |  INFO  +-----------
// ----------+--------+ Confidence
// AltValue           |
//                    |

// Portrait layout:

//
//         Value
// ---------------------
//                  Unit
// ---------------------
//            Confidence
// ----------+----------
//           |
// AltKey    |      INFO
// ----------+----------
// AltValue
//

// Display metrics

#if defined(DISPLAY_128X64)

#define DISPLAY_LANDSCAPE

#define PADDING_LEFT 3
#define PADDING_RIGHT (PADDING_LEFT - 1)

#define MENU_LINE_HEIGHT 14

#define MEASUREMENT_PADDING 7
#define MEASUREMENT_VALUE_HEIGHT 36
#define MEASUREMENT_VALUE_OFFSETX (CONTENT_WIDTH - 6)
#define MEASUREMENT_VALUE_OFFSETY 2
#define MEASUREMENT_CONFIDENCE_OFFSETY 0
#define MEASUREMENT_INFO_HEIGHT 12
#define MEASUREMENT_ALT_VALUE_WIDTH 84
#define MEASUREMENT_BAR_LABEL_HEIGHT 9

#define HISTORY_WIDTH 122
#define HISTORY_HEIGHT 42
#define HISTORY_TOP_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 1)
#define HISTORY_TOP_LABEL_OFFSET_TOP (HISTORY_TOP - CONTENT_TOP - FONT_SMALL_LINE_HEIGHT)
#define HISTORY_BOTTOM_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 1)
#define HISTORY_BOTTOM_LABEL_OFFSET_TOP -1

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0
#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8
#define GAME_HISTORY_COLUMN1_WIDTH 32
#define GAME_HISTORY_COLUMN1_OFFSETX 8

#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2)

#elif defined(DISPLAY_320X240)

#define DISPLAY_LANDSCAPE

#define PADDING_LEFT 12
#define PADDING_RIGHT PADDING_LEFT

#define MENU_LINE_HEIGHT 50

#define MEASUREMENT_PADDING 15
#define MEASUREMENT_VALUE_HEIGHT 122
#define MEASUREMENT_VALUE_OFFSETX (CONTENT_WIDTH - 10)
#define MEASUREMENT_VALUE_OFFSETY 3
#define MEASUREMENT_CONFIDENCE_OFFSETY -2
#define MEASUREMENT_INFO_HEIGHT 43
#define MEASUREMENT_ALT_VALUE_WIDTH 212
#define MEASUREMENT_BAR_LABEL_HEIGHT (MEASUREMENT_ALT_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT / 2)

#define HISTORY_WIDTH 302
#define HISTORY_HEIGHT 130
#define HISTORY_TOP_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 5)
#define HISTORY_TOP_LABEL_OFFSET_TOP (HISTORY_TOP - CONTENT_TOP - FONT_SMALL_LINE_HEIGHT - 2)
#define HISTORY_BOTTOM_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 5)
#define HISTORY_BOTTOM_LABEL_OFFSET_TOP 2

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP TITLEBAR_BOTTOM
#define GAME_SQUARE_WIDTH 25
#define GAME_SQUARE_HEIGHT 25
#define GAME_HISTORY_COLUMN1_WIDTH 66
#define GAME_HISTORY_COLUMN1_OFFSETX 12

#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2)

#elif defined(DISPLAY_240X320)

#define DISPLAY_PORTRAIT

#define PADDING_LEFT 8
#define PADDING_RIGHT PADDING_LEFT

#define MENU_LINE_HEIGHT 46

#define MEASUREMENT_PADDING 15
#define MEASUREMENT_VALUE_HEIGHT 155
#define MEASUREMENT_VALUE_OFFSETX (CONTENT_WIDTH - 10)
#define MEASUREMENT_VALUE_OFFSETY 67
#define MEASUREMENT_CONFIDENCE_OFFSETY -2
#define MEASUREMENT_INFO_HEIGHT 35
#define MEASUREMENT_ALT_VALUE_WIDTH CONTENT_RIGHT
#define MEASUREMENT_BAR_LABEL_HEIGHT (MEASUREMENT_ALT_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT / 2)

#define HISTORY_WIDTH 202
#define HISTORY_HEIGHT 130
#define HISTORY_TOP_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 5)
#define HISTORY_TOP_LABEL_OFFSET_TOP (HISTORY_TOP - CONTENT_TOP - FONT_SMALL_LINE_HEIGHT - 2)
#define HISTORY_BOTTOM_LABEL_OFFSET_LEFT (HISTORY_WIDTH - 5)
#define HISTORY_BOTTOM_LABEL_OFFSET_TOP 2

#define GAME_BOARD_LEFT ((CONTENT_WIDTH - GAME_BOARD_WIDTH) / 2)
#define GAME_BOARD_TOP (TITLEBAR_BOTTOM + GAME_BOARD_LEFT)
#define GAME_SQUARE_WIDTH 25
#define GAME_SQUARE_HEIGHT 25
#define GAME_HISTORY_COLUMN1_WIDTH 68
#define GAME_HISTORY_COLUMN1_OFFSETX 10

#define STATISTICS_KEY_WIDTH 110

#endif

// Common metrics

#if defined(DISPLAY_MONOCHROME)
#define TITLEBAR_HEIGHT 8
#define TITLEBAR_SHADOW_HEIGHT 0
#elif defined(DISPLAY_COLOR)
#define TITLEBAR_HEIGHT 40
#define TITLEBAR_SHADOW_HEIGHT 2
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

#define MENU_LINE_NUM (CONTENT_HEIGHT / MENU_LINE_HEIGHT)

#define MEASUREMENT_VALUE_WIDTH CONTENT_WIDTH
#define MEASUREMENT_VALUE_LEFT CONTENT_LEFT
#define MEASUREMENT_VALUE_TOP CONTENT_TOP
#define MEASUREMENT_VALUE_BOTTOM (MEASUREMENT_VALUE_TOP + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_UNIT_TOP MEASUREMENT_VALUE_BOTTOM
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_UNIT_LEFT (CONTENT_LEFT + MEASUREMENT_ALT_VALUE_WIDTH)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_UNIT_LEFT CONTENT_LEFT
#endif
#define MEASUREMENT_UNIT_WIDTH (CONTENT_WIDTH - MEASUREMENT_UNIT_LEFT)
#define MEASUREMENT_UNIT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_UNIT_BOTTOM (MEASUREMENT_UNIT_TOP + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_UNIT_OFFSETX (MEASUREMENT_UNIT_WIDTH - MEASUREMENT_PADDING)
#define MEASUREMENT_UNIT_OFFSETY 0
#define MEASUREMENT_CONFIDENCE_LEFT MEASUREMENT_UNIT_LEFT
#define MEASUREMENT_CONFIDENCE_TOP MEASUREMENT_UNIT_BOTTOM
#define MEASUREMENT_CONFIDENCE_WIDTH MEASUREMENT_UNIT_WIDTH
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_CONFIDENCE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_UNIT_BOTTOM)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_CONFIDENCE_HEIGHT FONT_SMALL_LINE_HEIGHT
#endif
#define MEASUREMENT_CONFIDENCE_BOTTOM (MEASUREMENT_CONFIDENCE_TOP + MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_CONFIDENCE_OFFSETX (MEASUREMENT_CONFIDENCE_WIDTH - MEASUREMENT_PADDING)
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_INFO_TOP MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_INFO_LEFT (MEASUREMENT_ALT_VALUE_RIGHT - MEASUREMENT_INFO_WIDTH)
#define MEASUREMENT_INFO_WIDTH (2 * MEASUREMENT_ALT_VALUE_RIGHT - CONTENT_WIDTH)
#define MEASUREMENT_INFO_OFFSETX (MEASUREMENT_INFO_WIDTH / 2)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_INFO_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#define MEASUREMENT_INFO_LEFT (CONTENT_LEFT + CONTENT_WIDTH / 2)
#define MEASUREMENT_INFO_WIDTH (CONTENT_WIDTH / 2)
#define MEASUREMENT_INFO_OFFSETX (MEASUREMENT_INFO_WIDTH - MEASUREMENT_PADDING)
#endif
#define MEASUREMENT_INFO_OFFSETY (MEASUREMENT_INFO_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_ALT_KEY_LEFT CONTENT_LEFT
#define MEASUREMENT_ALT_KEY_TOP MEASUREMENT_INFO_TOP
#define MEASUREMENT_ALT_KEY_WIDTH (MEASUREMENT_INFO_LEFT - MEASUREMENT_ALT_KEY_LEFT)
#define MEASUREMENT_ALT_KEY_HEIGHT MEASUREMENT_INFO_HEIGHT
#define MEASUREMENT_ALT_KEY_BOTTOM (MEASUREMENT_ALT_KEY_TOP + MEASUREMENT_ALT_KEY_HEIGHT)
#define MEASUREMENT_ALT_KEY_OFFSETX MEASUREMENT_PADDING
#define MEASUREMENT_ALT_KEY_OFFSETY MEASUREMENT_INFO_OFFSETY
#define MEASUREMENT_ALT_VALUE_LEFT MEASUREMENT_ALT_KEY_LEFT
#define MEASUREMENT_ALT_VALUE_TOP MEASUREMENT_ALT_KEY_BOTTOM
#define MEASUREMENT_ALT_VALUE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_ALT_VALUE_TOP)
#define MEASUREMENT_ALT_VALUE_RIGHT (CONTENT_LEFT + MEASUREMENT_ALT_VALUE_WIDTH)
#define MEASUREMENT_ALT_VALUE_OFFSETX MEASUREMENT_ALT_KEY_OFFSETX
#define MEASUREMENT_ALT_VALUE_OFFSETY 0
#define MEASUREMENT_BAR_DECADES 7
#define MEASUREMENT_BAR_WIDTH MEASUREMENT_ALT_VALUE_WIDTH
#define MEASUREMENT_BAR_RIGHT MEASUREMENT_ALT_VALUE_RIGHT
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_BAR_LABEL_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_BAR_LABEL_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define MEASUREMENT_BAR_LABEL_OFFSETY (MEASUREMENT_BAR_LABEL_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#if defined(DISPLAY_MONOCHROME)
#define MEASUREMENT_BAR_TICKS_HEIGHT 2
#define MEASUREMENT_BAR_TICKS_UPPER_HEIGHT 1
#define MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT 0
#elif defined(DISPLAY_COLOR)
#define MEASUREMENT_BAR_TICKS_HEIGHT 8
#define MEASUREMENT_BAR_TICKS_UPPER_HEIGHT 4
#define MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT 4
#endif
#define MEASUREMENT_BAR_TICKS_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING + 2)
#define MEASUREMENT_BAR_TICKS_TOP (MEASUREMENT_BAR_LABEL_TOP + MEASUREMENT_BAR_LABEL_HEIGHT)
#define MEASUREMENT_BAR_TICKS_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_TICKS_LEFT - 2)
#define MEASUREMENT_BAR_TICKS_LOWER_HEIGHT (MEASUREMENT_BAR_TICKS_HEIGHT - MEASUREMENT_BAR_TICKS_UPPER_HEIGHT)
#if defined(DISPLAY_MONOCHROME)
#define MEASUREMENT_BAR_FRAME_HEIGHT 9
#elif defined(DISPLAY_COLOR)
#define MEASUREMENT_BAR_FRAME_HEIGHT 20
#endif
#define MEASUREMENT_BAR_FRAME_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING)
#define MEASUREMENT_BAR_FRAME_TOP (MEASUREMENT_BAR_OUTERMARGIN_TOP + MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT)
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_BAR_FRAME_RIGHT MEASUREMENT_BAR_RIGHT
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_BAR_FRAME_RIGHT (MEASUREMENT_BAR_RIGHT - MEASUREMENT_PADDING)
#endif
#define MEASUREMENT_BAR_FRAME_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_FRAME_LEFT)
#define MEASUREMENT_BAR_FRAME_BOTTOM (MEASUREMENT_BAR_FRAME_TOP + MEASUREMENT_BAR_FRAME_HEIGHT)
#define MEASUREMENT_BAR_INNERFRAME_LEFT (MEASUREMENT_BAR_FRAME_LEFT + 1)
#define MEASUREMENT_BAR_INNERFRAME_TOP (MEASUREMENT_BAR_FRAME_TOP + 1)
#define MEASUREMENT_BAR_INNERFRAME_WIDTH (MEASUREMENT_BAR_FRAME_WIDTH - 2)
#define MEASUREMENT_BAR_INNERFRAME_HEIGHT (MEASUREMENT_BAR_FRAME_HEIGHT - 2)
#define MEASUREMENT_BAR_INSTRUMENT_LEFT (MEASUREMENT_BAR_INNERFRAME_LEFT + 1)
#define MEASUREMENT_BAR_INSTRUMENT_TOP (MEASUREMENT_BAR_INNERFRAME_TOP + 1)
#define MEASUREMENT_BAR_INSTRUMENT_WIDTH (MEASUREMENT_BAR_INNERFRAME_WIDTH - 2)
#define MEASUREMENT_BAR_INSTRUMENT_HEIGHT (MEASUREMENT_BAR_INNERFRAME_HEIGHT - 2)
#define MEASUREMENT_BAR_OUTERMARGIN_LEFT CONTENT_LEFT
#define MEASUREMENT_BAR_OUTERMARGIN_TOP (MEASUREMENT_BAR_TICKS_TOP + MEASUREMENT_BAR_TICKS_HEIGHT)
#define MEASUREMENT_BAR_OUTERMARGIN_WIDTH (MEASUREMENT_BAR_RIGHT - MEASUREMENT_BAR_OUTERMARGIN_LEFT)
#define MEASUREMENT_BAR_OUTERMARGIN_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_BAR_OUTERMARGIN_TOP)
#define MEASUREMENT_BAR_INNERMARGIN_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING)
#define MEASUREMENT_BAR_INNERMARGIN_TOP MEASUREMENT_BAR_FRAME_TOP
#define MEASUREMENT_BAR_INNERMARGIN_WIDTH MEASUREMENT_BAR_FRAME_WIDTH
#define MEASUREMENT_BAR_INNERMARGIN_HEIGHT MEASUREMENT_BAR_FRAME_HEIGHT

#define HISTORY_LEFT ((CONTENT_WIDTH - HISTORY_WIDTH) / 2)
#define HISTORY_TOP (CONTENT_TOP + (CONTENT_HEIGHT - HISTORY_HEIGHT) / 2)
#define HISTORY_DATA_LEFT (HISTORY_LEFT + 1)
#define HISTORY_DATA_TOP (HISTORY_TOP + 1)
#define HISTORY_DATA_WIDTH (HISTORY_WIDTH - 2)
#define HISTORY_DATA_HEIGHT (HISTORY_HEIGHT - 2)
#define HISTORY_TOP_LABEL_LEFT HISTORY_LEFT
#define HISTORY_TOP_LABEL_TOP CONTENT_TOP
#define HISTORY_TOP_LABEL_WIDTH HISTORY_WIDTH
#define HISTORY_TOP_LABEL_HEIGHT (HISTORY_TOP - HISTORY_TOP_LABEL_TOP)
#define HISTORY_BOTTOM_LABEL_LEFT HISTORY_LEFT
#define HISTORY_BOTTOM_LABEL_TOP (HISTORY_TOP + HISTORY_HEIGHT)
#define HISTORY_BOTTOM_LABEL_WIDTH HISTORY_WIDTH
#define HISTORY_BOTTOM_LABEL_HEIGHT (CONTENT_BOTTOM - HISTORY_BOTTOM_LABEL_TOP)
#define HISTORY_SPACE_LEFT_LEFT 0
#define HISTORY_SPACE_LEFT_TOP HISTORY_TOP_LABEL_TOP
#define HISTORY_SPACE_LEFT_WIDTH HISTORY_LEFT
#define HISTORY_SPACE_LEFT_HEIGHT (CONTENT_BOTTOM - HISTORY_TOP_LABEL_TOP)
#define HISTORY_SPACE_RIGHT_LEFT (HISTORY_LEFT + HISTORY_WIDTH)
#define HISTORY_SPACE_RIGHT_TOP HISTORY_TOP_LABEL_TOP
#define HISTORY_SPACE_RIGHT_WIDTH (CONTENT_WIDTH - HISTORY_SPACE_RIGHT_LEFT)
#define HISTORY_SPACE_RIGHT_HEIGHT HISTORY_SPACE_LEFT_HEIGHT

#define RNG_STRING_LEFT CONTENT_LEFT
#define RNG_STRING_TOP CONTENT_TOP
#define RNG_STRING_WIDTH CONTENT_WIDTH
#define RNG_STRING_HEIGHT ((CONTENT_HEIGHT + FONT_SMALL_LINE_HEIGHT) / 2)
#define RNG_STRING_OFFSETX PADDING_LEFT
#define RNG_STRING_OFFSETY (RNG_STRING_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define RNG_STATE_LEFT CONTENT_LEFT
#define RNG_STATE_TOP (RNG_STRING_TOP + RNG_STRING_HEIGHT)
#define RNG_STATE_WIDTH CONTENT_WIDTH
#define RNG_STATE_HEIGHT (CONTENT_BOTTOM - RNG_STATE_TOP)
#define RNG_STATE_OFFSETX (CONTENT_WIDTH / 2)
#define RNG_STATE_OFFSETY ((RNG_STATE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT) / 2)
#define RNG_NUMBER_LEFT CONTENT_LEFT
#define RNG_NUMBER_TOP CONTENT_TOP
#define RNG_NUMBER_WIDTH CONTENT_WIDTH
#define RNG_NUMBER_HEIGHT CONTENT_HEIGHT
#define RNG_NUMBER_OFFSETX (CONTENT_WIDTH / 2)
#define RNG_NUMBER_OFFSETY ((RNG_NUMBER_HEIGHT - FONT_LARGE_LINE_HEIGHT) / 2)

#define STATISTICS_ENTRY_HEIGHT FONT_SMALL_LINE_HEIGHT
#define STATISTICS_ENTRIES_HEIGHT (STATISTICS_ENTRY_NUM * STATISTICS_ENTRY_HEIGHT)
#define STATISTICS_TOPMARGIN_HEIGHT ((CONTENT_HEIGHT - STATISTICS_ENTRIES_HEIGHT) / 2)
#define STATISTICS_BOTTOMMARGIN_HEIGHT (CONTENT_HEIGHT - STATISTICS_TOPMARGIN_HEIGHT - STATISTICS_ENTRIES_HEIGHT)
#define STATISTICS_KEY_LEFT 0
#define STATISTICS_KEY_OFFSETX PADDING_LEFT
#define STATISTICS_VALUE_LEFT STATISTICS_KEY_WIDTH
#define STATISTICS_VALUE_WIDTH (CONTENT_WIDTH - STATISTICS_KEY_WIDTH)
#define STATISTICS_VALUE_OFFSETX 0

#define GAME_BOARD_WIDTH (8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_HEIGHT (8 * GAME_SQUARE_HEIGHT)
#define GAME_BOARD_RIGHT (GAME_BOARD_LEFT + GAME_BOARD_WIDTH)
#define GAME_BOARD_BOTTOM (GAME_BOARD_TOP + GAME_BOARD_HEIGHT)
#define GAME_HISTORY_COLUMN2_WIDTH (GAME_HISTORY_WIDTH - GAME_HISTORY_COLUMN1_WIDTH)
#if defined(DISPLAY_LANDSCAPE)
#define GAME_TOPTIMER_LEFT GAME_BOARD_RIGHT
#define GAME_TOPTIMER_TOP GAME_BOARD_TOP
#define GAME_TOPTIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_TOPTIMER_HEIGHT (GAME_HISTORY_TOP - GAME_BOARD_TOP)
#define GAME_TOPTIMER_OFFSETX (FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_TOPTIMER_OFFSETY ((2 * GAME_SQUARE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#define GAME_HISTORY_LEFT GAME_TOPTIMER_LEFT
#define GAME_HISTORY_TOP (GAME_BOARD_TOP + (GAME_BOARD_HEIGHT - GAME_HISTORY_HEIGHT) / 2)
#define GAME_HISTORY_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_HISTORY_HEIGHT (3 * FONT_SMALL_LINE_HEIGHT)
#define GAME_BOTTOMTIMER_LEFT GAME_BOARD_RIGHT
#define GAME_BOTTOMTIMER_TOP (GAME_HISTORY_TOP + GAME_HISTORY_HEIGHT)
#define GAME_BOTTOMTIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_BOTTOMTIMER_HEIGHT (GAME_BOARD_TOP + GAME_BOARD_HEIGHT - GAME_BOTTOMTIMER_TOP)
#define GAME_BOTTOMTIMER_OFFSETX GAME_TOPTIMER_OFFSETX
#define GAME_BOTTOMTIMER_OFFSETY (GAME_BOTTOMTIMER_HEIGHT - 2 * GAME_SQUARE_HEIGHT + GAME_TOPTIMER_OFFSETY)
#elif defined(DISPLAY_PORTRAIT)
#define GAME_TOPTIMER_LEFT GAME_BOARD_LEFT
#define GAME_TOPTIMER_TOP CONTENT_TOP
#define GAME_TOPTIMER_WIDTH GAME_BOARD_WIDTH
#define GAME_TOPTIMER_HEIGHT (GAME_BOARD_TOP - GAME_TOPTIMER_TOP)
#define GAME_TOPTIMER_OFFSETX (GAME_TOPTIMER_WIDTH - FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_TOPTIMER_OFFSETY 0
#define GAME_HISTORY_LEFT GAME_BOARD_LEFT
#define GAME_HISTORY_TOP GAME_BOARD_BOTTOM
#define GAME_HISTORY_WIDTH (GAME_BOARD_WIDTH - GAME_BOTTOMTIMER_WIDTH)
#define GAME_HISTORY_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_BOTTOMTIMER_LEFT (GAME_BOARD_RIGHT - GAME_BOTTOMTIMER_WIDTH)
#define GAME_BOTTOMTIMER_TOP (GAME_BOARD_TOP + GAME_BOARD_HEIGHT)
#define GAME_BOTTOMTIMER_WIDTH (3 * GAME_SQUARE_WIDTH)
#define GAME_BOTTOMTIMER_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_BOTTOMTIMER_OFFSETX (GAME_BOTTOMTIMER_WIDTH - FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_BOTTOMTIMER_OFFSETY 0
#define GAME_LEFTMARGIN_LEFT CONTENT_LEFT
#define GAME_LEFTMARGIN_TOP CONTENT_TOP
#define GAME_LEFTMARGIN_WIDTH (GAME_BOARD_LEFT - CONTENT_LEFT)
#define GAME_LEFTMARGIN_HEIGHT CONTENT_HEIGHT
#define GAME_RIGHTMARGIN_LEFT (CONTENT_LEFT + GAME_BOARD_RIGHT)
#define GAME_RIGHTMARGIN_TOP CONTENT_TOP
#define GAME_RIGHTMARGIN_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_RIGHTMARGIN_HEIGHT CONTENT_HEIGHT
#endif

// Colors

typedef enum
{
    COLOR_ELEMENT_ACTIVE,
    COLOR_ELEMENT_NEUTRAL,

    COLOR_CONTAINER_BACKGROUND,
    COLOR_CONTAINER_GLOBAL,
    COLOR_CONTAINER_GLOBAL_SHADOW1,
    COLOR_CONTAINER_GLOBAL_SHADOW2,
    COLOR_CONTAINER_GLOBAL_SHADOW1_MENU,
    COLOR_CONTAINER_GLOBAL_SHADOW2_MENU,

    COLOR_INSTRUMENT_ENHANCED_PRIMARY,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY,
    COLOR_INSTRUMENT_FRAME_PRIMARY,
    COLOR_INSTRUMENT_FRAME_TERTIARY,

    COLOR_INSTRUMENT_ENHANCED_PRIMARY_ALERTZONE1,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY_ALERTZONE1,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY_ALERTZONE1,
    COLOR_INSTRUMENT_FRAME_PRIMARY_ALERTZONE1,
    COLOR_INSTRUMENT_FRAME_TERTIARY_ALERTZONE1,

    COLOR_INSTRUMENT_ENHANCED_PRIMARY_ALERTZONE2,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY_ALERTZONE2,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY_ALERTZONE2,
    COLOR_INSTRUMENT_FRAME_PRIMARY_ALERTZONE2,
    COLOR_INSTRUMENT_FRAME_TERTIARY_ALERTZONE2,

    COLOR_INSTRUMENT_TICKMARK_PRIMARY,
    COLOR_INSTRUMENT_TICKMARK_SECONDARY,

    COLOR_RUNNING,
    COLOR_WARNING,
    COLOR_ALARM,

    COLOR_NORMAL_ENABLED_BACKGROUND,
    COLOR_FLAT_CHECKED_BACKGROUND,
    COLOR_ON_FLAT_ACTIVE,
    COLOR_ON_FLAT_NEUTRAL,

#if defined(GAME)
    COLOR_SELECTED_ENABLED_BACKGROUND,
    COLOR_GAME_SQUARE_BLACK,
    COLOR_GAME_SQUARE_WHITE,
    COLOR_GAME_PIECE_BLACK,
    COLOR_GAME_PIECE_WHITE,
#endif
} ColorIndex;

#define COLOR_INSTRUMENT_OFFSET 5

#if defined(DISPLAY_COLOR)

// Online color blender: https://pinetools.com/blend-colors
// Online RGB565 color picker: https://rgbcolorpicker.com/565

static const mr_color_t displayColors[][3] = {
    // Element active
    {mr_get_color(0x1a1a1a),
     mr_get_color(0xf2f0f2), // 0xf2f2f2
     mr_get_color(0x474700)},

    // Element neutral (on container background)
    {mr_get_color(0x666366), // 0x666666
     mr_get_color(0x9a9a9a),
     mr_get_color(0x2e2f00)}, // 0x2e2e00

    // Container background [global background]
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x1f1f1f),
     mr_get_color(0x000000)},

    // Container global [title bar background]
    {mr_get_color(0xfcfdfc), // 0xfcfcfc
     mr_get_color(0x262726), // 0x262626
     mr_get_color(0x0a0a00)},

    // Container global shadow 1
    {mr_get_color(0xd0d0d0),
     mr_get_color(0x181818),
     mr_get_color(0x000000)},

    // Container global shadow 2
    {mr_get_color(0xe8e8e8),
     mr_get_color(0x1f1f1f),
     mr_get_color(0x000000)},

    // Container global shadow 1 menu
    {mr_get_color(0xd0d0d0),
     mr_get_color(0x202020),
     mr_get_color(0x000000)},

    // Container global shadow 2 menu
    {mr_get_color(0xe8e8e8),
     mr_get_color(0x282828),
     mr_get_color(0x000000)},

    // Instrument enhanced primary
    {mr_get_color(0x325b9a),
     mr_get_color(0x5696c7),
     mr_get_color(0x003315)},

    // Instrument enhanced tertiary (on instrument frame primary)
    {mr_get_color(0xd6deeb),
     mr_get_color(0x3b4550),
     mr_get_color(0x081303)}, // 0x081203

    // Instrument enhanced tertiary (on instrument frame tertiary)
    {mr_get_color(0xb8c0cd),
     mr_get_color(0x222c37),
     mr_get_color(0x1a2703)},

    // Instrument frame primary
    {mr_get_color(0xffffff),
     mr_get_color(0x333233), // 0x333333
     mr_get_color(0x0a0a00)},

    // Instrument frame tertiary
    {mr_get_color(0xd9d8d9), // 0xd9d9d9
     mr_get_color(0x141214), // 0x141414
     mr_get_color(0x212400)},

    // Instrument enhanced primary alert zone 1
    {mr_get_color(0x2e548e),
     mr_get_color(0x45789f),
     mr_get_color(0x002911)},

    // Instrument enhanced tertiary (on instrument frame primary) alert zone 1
    {mr_get_color(0xc5cdd9),
     mr_get_color(0x2f3740),
     mr_get_color(0x060f02)}, // 0x060e02

    // Instrument enhanced tertiary (on instrument frame tertiary) alert zone 1
    {mr_get_color(0xaab1bd),
     mr_get_color(0x1b232c),
     mr_get_color(0x151f02)},

    // Instrument frame primary alert zone 1
    {mr_get_color(0xebedeb), // 0xebebeb
     mr_get_color(0x292929), // 0x2b2b2b
     mr_get_color(0x080800)},

    // Instrument frame tertiary alert zone 1
    {mr_get_color(0xc8c8c8),
     mr_get_color(0x101010),
     mr_get_color(0x1a1d00)},

    // Instrument enhanced primary alert zone 2
    {mr_get_color(0x2a4d83),
     mr_get_color(0x37607f),
     mr_get_color(0x00210e)},

    // Instrument enhanced tertiary (on instrument frame primary) alert zone 2
    {mr_get_color(0xb6bdc8),
     mr_get_color(0x262c33),
     mr_get_color(0x050b02)},

    // Instrument enhanced tertiary (on instrument frame tertiary) alert zone 2
    {mr_get_color(0x9da3ae),
     mr_get_color(0x161c23),
     mr_get_color(0x111902)},

    // Instrument frame primary alert zone 2
    {mr_get_color(0xd9d8d9), // 0xd9d9d9
     mr_get_color(0x212121),
     mr_get_color(0x060700)}, // 0x060600

    // Instrument frame tertiary alert zone 2
    {mr_get_color(0xb8b8b8),
     mr_get_color(0x0d0f0d), // 0x0d0d0d
     mr_get_color(0x151700)},

    // Instrument tickmark primary
    {mr_get_color(0x333233), // 0x333333
     mr_get_color(0xe6e6e6),
     mr_get_color(0x474700)},

    // Instrument tickmark secondary
    {mr_get_color(0x707070),
     mr_get_color(0x9c9c9c),
     mr_get_color(0x333200)}, // 0x0x333300

    // Running (green)
    {mr_get_color(0x228722),
     mr_get_color(0x138a00),
     mr_get_color(0x173800)},

    // Warning (orange)
    {mr_get_color(0xf69400),
     mr_get_color(0xf69400),
     mr_get_color(0x5c2c00)},

    // Alarm (red)
    {mr_get_color(0xdf1b1b),
     mr_get_color(0xe52626),
     mr_get_color(0x4d0005)},

    // Normal enabled background [menu background]
    {mr_get_color(0xffffff),
     mr_get_color(0x333233), // 0x333333
     mr_get_color(0x000000)},

    // Flat checked background (on container global) [menu selected background]
    {mr_get_color(0xd6dde9),  // Changed alpha to 20%
     mr_get_color(0x324563),  // Changed alpha to 50%
     mr_get_color(0x00230f)}, // Changed alpha to 50%

    // On flat active (on container global) [menu selected foreground]
    {mr_get_color(0x161718),
     mr_get_color(0xf2f0f2), // 0xf2f2f2
     mr_get_color(0x474700)},

    // On flat neutral (on container global) [menu selected gadget]
    {mr_get_color(0x737473),  // 0x737373
     mr_get_color(0xa3a4a3),  // 0xa3a3a3
     mr_get_color(0x333200)}, // 0x333300

#if defined(GAME)
    // Selected enabled background [game square selected]
    {mr_get_color(0x305792),
     mr_get_color(0x5779a2),
     mr_get_color(0x193422)},

    // Game square black
    {mr_get_color(0xb89070),
     mr_get_color(0xb89070),
     mr_get_color(0x202000)},

    // Game square white
    {mr_get_color(0xd0b088),
     mr_get_color(0xd0b088),
     mr_get_color(0x303000)},

    // Game piece black
    {mr_get_color(0x000000),
     mr_get_color(0x000000),
     mr_get_color(0x000000)},

    // Game piece white
    {mr_get_color(0xffffff),
     mr_get_color(0xffffff),
     mr_get_color(0x484800)},
#endif
};

#endif

void onDisplaySubMenuBack(void);

// Definitions

const uint32_t menuLineNum = MENU_LINE_NUM;

static Menu displayMenu;
#if defined(DISPLAY_MONOCHROME)
static Menu displayContrastMenu;
#elif defined(DISPLAY_COLOR)
static Menu displayThemeMenu;
#endif
static Menu displayBrightnessMenu;
static Menu displaySleepMenu;

mr_t mr;

void resetDisplay(void)
{
    selectMenuItem(&displayMenu,
                   0,
                   0);

#if defined(DISPLAY_MONOCHROME)
    selectMenuItem(&displayContrastMenu,
                   settings.displayContrast,
                   DISPLAY_CONTRAST_NUM);
#elif defined(DISPLAY_COLOR)
    selectMenuItem(&displayThemeMenu,
                   settings.displayTheme,
                   DISPLAY_THEME_NUM);
#endif

    selectMenuItem(&displaySleepMenu,
                   settings.displaySleep,
                   DISPLAY_SLEEP_NUM);
    selectMenuItem(&displayBrightnessMenu,
                   settings.displayBrightness,
                   DISPLAY_BRIGHTNESS_NUM);
}

// Low-level functions

static inline mr_color_t getFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    if ((colorIndex == COLOR_INSTRUMENT_ENHANCED_PRIMARY) ||
        (colorIndex == COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY) ||
        (colorIndex == COLOR_INSTRUMENT_FRAME_TERTIARY) ||
        (colorIndex == COLOR_INSTRUMENT_TICKMARK_PRIMARY) ||
        (colorIndex == COLOR_INSTRUMENT_TICKMARK_SECONDARY) ||
        (colorIndex == COLOR_FLAT_CHECKED_BACKGROUND) ||
        (colorIndex == COLOR_GAME_SQUARE_BLACK))
        return 0xffff;
    else
        return 0x0000;
#elif defined(DISPLAY_COLOR)
    return displayColors[colorIndex][settings.displayTheme];
#endif
}

static void setFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    mr_color_t fillColor = getFillColor(colorIndex);
    mr_color_t strokeColor = fillColor ? 0x0000 : 0xffff;

    mr_set_fill_color(&mr, fillColor);
    mr_set_stroke_color(&mr, strokeColor);
#elif defined(DISPLAY_COLOR)
    mr_set_fill_color(&mr, getFillColor(colorIndex));
#endif
}

static void setStrokeColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_COLOR)
    mr_set_stroke_color(&mr, getFillColor(colorIndex));
#endif
}

static void drawRectangle(const mr_rectangle_t *rectangle)
{
    mr_draw_rectangle(&mr, rectangle);
}

static void drawMargin(const mr_rectangle_t *outerMargin,
                       const mr_rectangle_t *innerMargin)
{
    mr_rectangle_t rectangle;
    uint16_t outerMarginRight = outerMargin->x + outerMargin->width;
    uint16_t outerMarginBottom = outerMargin->y + outerMargin->height;
    uint16_t innerMarginRight = innerMargin->x + innerMargin->width;
    uint16_t innerMarginBottom = innerMargin->y + innerMargin->height;

    // Top
    rectangle.x = outerMargin->x;
    rectangle.y = outerMargin->y;
    rectangle.width = outerMargin->width;
    rectangle.height = innerMargin->y - outerMargin->y;
    mr_draw_rectangle(&mr, &rectangle);

    // Bottom
    rectangle.y = innerMarginBottom;
    rectangle.height = outerMarginBottom - innerMarginBottom;
    mr_draw_rectangle(&mr, &rectangle);

    // Left
    rectangle.y = innerMargin->y;
    rectangle.width = innerMargin->x - outerMargin->x;
    rectangle.height = innerMargin->height;
    mr_draw_rectangle(&mr, &rectangle);

    // Right
    rectangle.x = innerMarginRight;
    rectangle.width = outerMarginRight - innerMarginRight;
    mr_draw_rectangle(&mr, &rectangle);
}

static void drawFrame(const mr_rectangle_t *rectangle)
{
    mr_rectangle_t innerMargin = {
        rectangle->x + 1,
        rectangle->y + 1,
        rectangle->width - 2,
        rectangle->height - 2};
    drawMargin(rectangle,
               &innerMargin);
}

static void drawImage(const mr_rectangle_t *rectangle,
                      const mr_color_t *imageBuffer)
{
    mr_draw_image(&mr, rectangle, imageBuffer);
}

static void setFont(const uint8_t *font)
{
    mr_set_font(&mr, font);
}

static uint16_t getTextWidth(const char *s)
{
    return mr_get_utf8_text_width(&mr, (const uint8_t *)s);
}

static uint16_t getTextHeight(void)
{
    return mr_get_line_height(&mr);
}

#if defined(DISPLAY_MONOCHROME)
static void drawGrayedRectangle(const mr_rectangle_t *rectangle)
{
    mr_rectangle_t r;
    r.width = 1;
    r.height = 1;

    setFillColor(COLOR_CONTAINER_GLOBAL);

    for (r.y = rectangle->y;
         r.y < (rectangle->y + rectangle->height);
         r.y++)
    {
        for (r.x = rectangle->x;
             r.x < (rectangle->x + rectangle->width);
             r.x++)
        {
            if ((r.x ^ r.y) & 0b1)
                continue;

            mr_draw_rectangle(&mr, &r);
        }
    }
}
#endif

typedef enum
{
    TEXTALIGNMENT_LEFTALIGNED,
    TEXTALIGNMENT_CENTERED,
    TEXTALIGNMENT_RIGHTALIGNED,
} TextAlignment;

static void drawAlignedText(const char *s,
                            const mr_rectangle_t *rectangle,
                            const mr_point_t *offset,
                            TextAlignment textAlignment)
{
    char buffer[64];
    const char *str;
    int16_t width = rectangle->width + offset->x;

    switch (textAlignment)
    {
    case TEXTALIGNMENT_LEFTALIGNED:
        // | x---------|
        width = rectangle->width - offset->x;

        break;

    case TEXTALIGNMENT_CENTERED:
        // |-----x-----|
        width = rectangle->width;

        break;

    case TEXTALIGNMENT_RIGHTALIGNED:
        // |---------x |
        width = offset->x;

        break;
    }

    if (getTextWidth(s) <= width)
        str = s;
    else
    {
        strcpy(buffer, s);
        for (uint32_t i = strlen(s); i > 0; i--)
        {
            // Skip UTF-8 continuation bytes
            if ((((uint8_t)buffer[i]) >> 6) == 0x02)
                continue;

            strcpy(buffer + i, getString(STRING_ELLIPSIS));
            if (getTextWidth(buffer) <= width)
                break;
        }
        str = buffer;
    }

    mr_point_t strOffset = *offset;
    switch (textAlignment)
    {
    case TEXTALIGNMENT_CENTERED:
        strOffset.x -= getTextWidth(str) / 2;

        break;

    case TEXTALIGNMENT_RIGHTALIGNED:
        strOffset.x -= getTextWidth(str);

        break;

    default:
        break;
    }

    mr_draw_utf8_text(&mr,
                      (const uint8_t *)str,
                      rectangle,
                      &strOffset);
}

static void drawText(const char *s,
                     const mr_rectangle_t *rectangle,
                     const mr_point_t *offset)
{
    drawAlignedText(s,
                    rectangle,
                    offset,
                    TEXTALIGNMENT_LEFTALIGNED);
}

static void drawCenteredText(const char *s,
                             const mr_rectangle_t *rectangle,
                             const mr_point_t *offset)
{
    drawAlignedText(s,
                    rectangle,
                    offset,
                    TEXTALIGNMENT_CENTERED);
}

static void drawRightAlignedText(const char *s,
                                 const mr_rectangle_t *rectangle,
                                 const mr_point_t *offset)
{
    drawAlignedText(s,
                    rectangle,
                    offset,
                    TEXTALIGNMENT_RIGHTALIGNED);
}

static void drawRowRight(const char *buffer,
                         mr_rectangle_t *rectangle)
{
    uint32_t width = getTextWidth(buffer) + PADDING_RIGHT;
    rectangle->width = width;
    rectangle->x -= width;
    mr_point_t offset = {
        0,
        (rectangle->height - getTextHeight()) / 2,
    };
    drawText(buffer,
             rectangle,
             &offset);
}

static void drawRowLeft(const char *buffer,
                        mr_rectangle_t *rectangle)
{
    rectangle->width = rectangle->x - CONTENT_LEFT;
    rectangle->x = CONTENT_LEFT;
    mr_point_t offset = {
        PADDING_LEFT,
        (rectangle->height - getTextHeight()) / 2,
    };
    drawText(buffer,
             rectangle,
             &offset);
}

static const uint8_t chargingIcons[] = {
    5,
    6,
    6,
    6,
    7,
};

static void setupBatteryIcon(char *buffer)
{
    ColorIndex colorIndex;
    uint8_t level = getBatteryLevel();

    if (isUSBPowered() || isBatteryCharging())
    {
        colorIndex = COLOR_RUNNING;
        level = chargingIcons[level];
    }
    else
    {
        if (level == 0)
            colorIndex = COLOR_ALARM;
        else
            colorIndex = COLOR_ELEMENT_NEUTRAL;
    }
    strclr(buffer);
    strcatChar(buffer, '0' + level);
    setFont(font_symbols);
    setStrokeColor(colorIndex);
}

// Mid-level functions

void drawPowerOff(bool displayBatteryIcon)
{
    char buffer[8];

    const mr_rectangle_t rectangle = {
        0,
        0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
    };
    const mr_point_t offset = {
        (DISPLAY_WIDTH / 2),
        ((DISPLAY_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2),
    };
    setFillColor(COLOR_CONTAINER_BACKGROUND);
    if (displayBatteryIcon)
        setupBatteryIcon(buffer);
    else
        strclr(buffer);
    drawCenteredText(buffer,
                     &rectangle,
                     &offset);
}

static const ColorIndex alertLevelColorIndex[] = {
    COLOR_ELEMENT_NEUTRAL,
    COLOR_WARNING,
    COLOR_ALARM,
};

#if TITLEBAR_SHADOW_HEIGHT > 0
static const ColorIndex titleBarShadowColorIndex[][2] = {
    {
        COLOR_CONTAINER_GLOBAL_SHADOW1,
        COLOR_CONTAINER_GLOBAL_SHADOW1_MENU,
    },
    {
        COLOR_CONTAINER_GLOBAL_SHADOW2,
        COLOR_CONTAINER_GLOBAL_SHADOW2_MENU,
    },
};
#endif

void drawTitleBar(const char *title, bool isMenu)
{
    char buffer[8];
    mr_rectangle_t rectangle = {
        TITLEBAR_WIDTH,
        TITLEBAR_TOP,
        0,
        TITLEBAR_CONTENT_HEIGHT};
    setFillColor(COLOR_CONTAINER_GLOBAL);

    // Time
    RTCDateTime dateTime;
    getDeviceDateTime(&dateTime);
    if (dateTime.year >= RTC_YEAR_MIN)
    {
        strclr(buffer);
        if (settings.rtcTimeFormat == RTC_TIMEFORMAT_24_HOUR)
            strcatUInt32(buffer, dateTime.hour, 2);
        else
        {
            uint32_t hour = dateTime.hour % 12;
            strcatUInt32(buffer, (hour == 0) ? 12 : hour, 1);
        }
        strcatChar(buffer, ':');
        strcatUInt32(buffer, dateTime.minute, 2);

        setFont(font_small);
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawRowRight(buffer, &rectangle);
    }

    // Battery icon
    setupBatteryIcon(buffer);
    drawRowRight(buffer, &rectangle);

    // Alert icon
    if (isAlertEnabled() ||
        (getAlertLevel() > ALERTLEVEL_NONE))
    {
        AlertLevel alertLevel = getAlertLevel();
        bool alertUndismissed = isAlertUndismissed();

        if (!isAlertBlink())
        {
            alertLevel = ALERTLEVEL_NONE;
            alertUndismissed = false;
        }

        setStrokeColor(alertLevelColorIndex[alertLevel]);
        drawRowRight(alertUndismissed ? ";" : ":", &rectangle);
    }

    // Lock icon
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    if (isLockMode())
        drawRowRight("<", &rectangle);

    // Pulse sound icon
    if (isPulseSoundIconEnabled())
        drawRowRight(settings.pulseSound ? "9" : "8",
                     &rectangle);

    // Title
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    drawRowLeft(title, &rectangle);

    // Shadow
#if TITLEBAR_SHADOW_HEIGHT > 0
    rectangle = (mr_rectangle_t){
        TITLEBAR_LEFT,
        TITLEBAR_BOTTOM - TITLEBAR_SHADOW_HEIGHT,
        TITLEBAR_WIDTH,
        1,
    };

    for (uint32_t i = 0; i < TITLEBAR_SHADOW_HEIGHT; i++)
    {
        setFillColor(titleBarShadowColorIndex[isMenu][i]);
        drawRectangle(&rectangle);

        rectangle.y++;
    }
#endif

    // Set background
    setFillColor(COLOR_CONTAINER_BACKGROUND);
}

static void drawValueAndUnit(const char *valueString,
                             const char *unitString,
                             const mr_rectangle_t *rectangle,
                             const mr_point_t *offset,
                             bool isHold)
{
    mr_rectangle_t partialRectangle = *rectangle;
    partialRectangle.width = offset->x +
                             getTextWidth(valueString);
    setStrokeColor(isHold
                       ? COLOR_ELEMENT_ACTIVE
                       : COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawText(valueString,
             &partialRectangle,
             offset);

    partialRectangle.x += partialRectangle.width;
    partialRectangle.width = rectangle->width - partialRectangle.width;
    mr_point_t partialOffset = {
        0,
        offset->y};
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(unitString,
             &partialRectangle,
             &partialOffset);
}

void drawNotification(const char *title,
                      const char *subtitle)
{
    setFillColor(COLOR_CONTAINER_GLOBAL);

    // Top line
    const mr_rectangle_t topRectangle = {
        0,
        0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT / 2};
    const mr_point_t topOffset = {
        DISPLAY_WIDTH / 2,
        DISPLAY_HEIGHT / 2 - FONT_MEDIUM_LINE_HEIGHT};
    setFont(font_medium);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    drawCenteredText(title,
                     &topRectangle,
                     &topOffset);

    // Bottom line
    const mr_rectangle_t bottomRectangle = {
        0,
        DISPLAY_HEIGHT / 2,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT / 2};
    const mr_point_t bottomOffset = {
        DISPLAY_WIDTH / 2,
        FONT_MEDIUM_LINE_HEIGHT - FONT_SMALL_LINE_HEIGHT};
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawCenteredText(subtitle,
                     &bottomRectangle,
                     &bottomOffset);
}

void drawMenu(Menu *menu)
{
    drawTitleBar(menu->title, true);

    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    mr_rectangle_t rectangle;
    rectangle.height = MENU_LINE_HEIGHT;
    rectangle.y = CONTENT_TOP;

    for (uint32_t i = 0; i < MENU_LINE_NUM; i++)
    {
        uint32_t index = startIndex + i;

        MenuStyle menuStyle;
        const char *menuItem = menu->onGetOption(index, &menuStyle);

        if (!menuItem)
            break;

        rectangle.x = CONTENT_WIDTH;
        setFont(font_symbols);
        setFillColor((index == selectedIndex)
                         ? COLOR_FLAT_CHECKED_BACKGROUND
                         : COLOR_NORMAL_ENABLED_BACKGROUND);

        // Submenu
        if (menuStyle & MENUSTYLE_SUBMENU)
        {
            setStrokeColor(COLOR_ON_FLAT_NEUTRAL);
            drawRowRight("=", &rectangle);
        }

        // Checked
        if (menuStyle & MENUSTYLE_CHECKED)
        {
            setStrokeColor(COLOR_INSTRUMENT_ENHANCED_PRIMARY);
            drawRowRight(">", &rectangle);
        }

        // Text
        setFont(font_medium);
        setStrokeColor(COLOR_ON_FLAT_ACTIVE);
        drawRowLeft(menuItem, &rectangle);

        rectangle.y += MENU_LINE_HEIGHT;
    }

    // Bottom space
    rectangle.x = CONTENT_LEFT;
    rectangle.width = CONTENT_WIDTH;
    rectangle.height = CONTENT_BOTTOM - rectangle.y;
    setFillColor(COLOR_NORMAL_ENABLED_BACKGROUND);
    drawRectangle(&rectangle);
}

void drawTestMode(const char lines[8][32])
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_NORMAL_ENABLED_BACKGROUND);

    mr_rectangle_t rectangle;
    const mr_point_t offset = {
        0,
        0};
    for (uint32_t y = 0; y < 4; y++)
        for (uint32_t x = 0; x < 2; x++)
        {
            rectangle = (mr_rectangle_t){
                (DISPLAY_WIDTH / 2) * x,
                FONT_SMALL_LINE_HEIGHT * y,
                (DISPLAY_WIDTH / 2),
                FONT_SMALL_LINE_HEIGHT};
            drawText(lines[y * 2 + x],
                     &rectangle,
                     &offset);
        }

    rectangle = (mr_rectangle_t){
        0,
        (4 * FONT_SMALL_LINE_HEIGHT),
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT - (4 * FONT_SMALL_LINE_HEIGHT)};
    drawRectangle(&rectangle);
}

static const ColorIndex measurementStyleColorIndex[] = {
    COLOR_INSTRUMENT_ENHANCED_PRIMARY,
    COLOR_WARNING,
    COLOR_ALARM,
    COLOR_ELEMENT_ACTIVE,
};

void drawMeasurementValue(const char *valueString,
                          const char *unitString,
                          float confidence,
                          MeasurementStyle style)
{
    // Value
    const mr_rectangle_t valueRectangle = {
        MEASUREMENT_VALUE_LEFT,
        MEASUREMENT_VALUE_TOP,
        MEASUREMENT_VALUE_WIDTH,
        MEASUREMENT_VALUE_HEIGHT};
    const mr_point_t valueOffset = {
        MEASUREMENT_VALUE_OFFSETX,
        MEASUREMENT_VALUE_OFFSETY};
    setFont(font_large);
    setStrokeColor(measurementStyleColorIndex[style]);
    drawRightAlignedText(valueString,
                         &valueRectangle,
                         &valueOffset);

#if defined(DISPLAY_MONOCHROME)
    if ((style == MEASUREMENTSTYLE_ALARM) ||
        (style == MEASUREMENTSTYLE_WARNING))
        drawGrayedRectangle(&valueRectangle);
#endif

    // Unit
    const mr_rectangle_t unitRectangle = {
        MEASUREMENT_UNIT_LEFT,
        MEASUREMENT_UNIT_TOP,
        MEASUREMENT_UNIT_WIDTH,
        MEASUREMENT_UNIT_HEIGHT};
    const mr_point_t unitOffset = {
        MEASUREMENT_UNIT_OFFSETX,
        MEASUREMENT_UNIT_OFFSETY};
    setFont(font_medium);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawRightAlignedText(unitString,
                         &unitRectangle,
                         &unitOffset);

    // Confidence
    char confidenceString[8];
    char confidenceUnit[8];
    strclr(confidenceString);
    strclr(confidenceUnit);
    if (confidence != 0)
    {
        strcat(confidenceString, getString(STRING_PLUSMINUS));
        strcatFloat(confidenceString,
                    100 * confidence,
                    confidence < 0.1F ? 1 : 0);
        strcat(confidenceUnit, getString(STRING_PERCENT));
    }
    setFont(font_small);
    const mr_rectangle_t confidenceRectangle = {
        MEASUREMENT_CONFIDENCE_LEFT,
        MEASUREMENT_CONFIDENCE_TOP,
        MEASUREMENT_CONFIDENCE_WIDTH,
        MEASUREMENT_CONFIDENCE_HEIGHT};
    mr_point_t confidenceOffset = {
        MEASUREMENT_CONFIDENCE_OFFSETX -
            getTextWidth(confidenceString) -
            getTextWidth(confidenceUnit),
        MEASUREMENT_CONFIDENCE_OFFSETY};
    drawValueAndUnit(confidenceString,
                     confidenceUnit,
                     &confidenceRectangle,
                     &confidenceOffset,
                     style == MEASUREMENTSTYLE_HOLD);
}

void drawMeasurementInfo(const char *infoKeyString,
                         const char *infoValueString,
                         const char *infoUnitString,
                         const char *stateString,
                         MeasurementStyle style)
{
    setFont(font_small);

    // Alt key
    const mr_rectangle_t altKeyRectangle = {
        MEASUREMENT_ALT_KEY_LEFT,
        MEASUREMENT_ALT_KEY_TOP,
        MEASUREMENT_ALT_KEY_WIDTH,
        MEASUREMENT_ALT_KEY_HEIGHT};
    const mr_point_t altKeyOffset = {
        MEASUREMENT_ALT_KEY_OFFSETX,
        MEASUREMENT_ALT_KEY_OFFSETY};
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(infoKeyString,
             &altKeyRectangle,
             &altKeyOffset);

    // Info
    const mr_rectangle_t infoRectangle = {
        MEASUREMENT_INFO_LEFT,
        MEASUREMENT_INFO_TOP,
        MEASUREMENT_INFO_WIDTH,
        MEASUREMENT_INFO_HEIGHT};
    const mr_point_t infoOffset = {
        MEASUREMENT_INFO_OFFSETX,
        MEASUREMENT_INFO_OFFSETY};
#if defined(DISPLAY_LANDSCAPE)
    drawCenteredText(stateString,
                     &infoRectangle,
                     &infoOffset);
#elif defined(DISPLAY_PORTRAIT)
    drawRightAlignedText(stateString,
                         &infoRectangle,
                         &infoOffset);
#endif

    // Alt value
    const mr_rectangle_t altValueRectangle = {
        MEASUREMENT_ALT_VALUE_LEFT,
        MEASUREMENT_ALT_VALUE_TOP,
        MEASUREMENT_ALT_VALUE_WIDTH,
        MEASUREMENT_ALT_VALUE_HEIGHT};
    const mr_point_t altValueOffset = {
        MEASUREMENT_ALT_VALUE_OFFSETX,
        MEASUREMENT_ALT_VALUE_OFFSETY};
    drawValueAndUnit(infoValueString,
                     infoUnitString,
                     &altValueRectangle,
                     &altValueOffset,
                     style == MEASUREMENTSTYLE_HOLD);
}

static uint32_t getMeasurementBarInstrumentValue(float value,
                                                 float minValue)
{
    if (value <= minValue)
        return 0;

    float x = value / minValue;
    float y = log10f(x);
    uint32_t instrumentValue = y *
                               MEASUREMENT_BAR_INSTRUMENT_WIDTH /
                               MEASUREMENT_BAR_DECADES;

    if (instrumentValue > MEASUREMENT_BAR_INSTRUMENT_WIDTH)
        return MEASUREMENT_BAR_INSTRUMENT_WIDTH;
    else
        return instrumentValue;
}

void drawMeasurementBar(float value,
                        int32_t minExponent,
                        float alertZone1Value,
                        float alertZone2Value)
{
    mr_rectangle_t rectangle;
    ColorIndex colorIndex;

    float minValue = powf(10.0F, minExponent);
    uint32_t instrumentValue = getMeasurementBarInstrumentValue(value, minValue);
#if defined(DISPLAY_COLOR)
    uint32_t instrumentAlertZone1Value = getMeasurementBarInstrumentValue(alertZone1Value, minValue);
    uint32_t instrumentAlertZone2Value = getMeasurementBarInstrumentValue(alertZone2Value, minValue);

    if (instrumentAlertZone1Value == 0)
        instrumentAlertZone1Value = MEASUREMENT_BAR_INSTRUMENT_WIDTH;
    if (instrumentAlertZone2Value == 0)
        instrumentAlertZone2Value = MEASUREMENT_BAR_INSTRUMENT_WIDTH;
    if (instrumentAlertZone1Value > instrumentAlertZone2Value)
        instrumentAlertZone1Value = instrumentAlertZone2Value;
#endif

    // Labels & ticks
    uint32_t xTick = 0;
    uint32_t xLabel = 0;
    uint32_t xLabelCenter = 0;
    bool wasLargeTick = false;
    for (uint32_t i = 0; i < (2 * (MEASUREMENT_BAR_DECADES + 1) + 1); i++)
    {
        bool isTick = i & 0b1;
        uint32_t index = i >> 1;
        uint32_t exponentIndex = minExponent + index;
        uint32_t metricPowerIndex = (30 + exponentIndex) % 3;
        bool isLargeTick = isTick && (metricPowerIndex == 0);

        uint32_t xTickNext = MEASUREMENT_BAR_TICKS_LEFT +
                             (MEASUREMENT_BAR_TICKS_WIDTH - 1) * index /
                                 MEASUREMENT_BAR_DECADES +
                             isTick;
        if (i == 2 * (MEASUREMENT_BAR_DECADES + 1))
            xTickNext = MEASUREMENT_BAR_RIGHT;

        if (wasLargeTick)
        {
            char buffer[8];
            strclr(buffer);
            strcatDecimalPowerWithMetricPrefix(buffer, exponentIndex - 1, -3);

            uint32_t width = (i < (2 * (MEASUREMENT_BAR_DECADES - 1)))
                                 ? xTickNext - xLabel
                                 : MEASUREMENT_BAR_RIGHT - xLabel;
            rectangle = (mr_rectangle_t){
                xLabel,
                MEASUREMENT_BAR_LABEL_TOP,
                width,
                MEASUREMENT_BAR_LABEL_HEIGHT};
            mr_point_t offset = {
                xLabelCenter - xLabel,
                MEASUREMENT_BAR_LABEL_OFFSETY,
            };
            setStrokeColor(COLOR_ELEMENT_ACTIVE);
            setFillColor(COLOR_CONTAINER_BACKGROUND);
            drawCenteredText(buffer,
                             &rectangle,
                             &offset);

            xLabel = xTickNext;
        }

        xLabelCenter = xTickNext;
        wasLargeTick = isLargeTick;

        uint32_t tickWidth = isTick
                                 ? 1
                                 : xTickNext - xTick;
        colorIndex = isLargeTick
                         ? COLOR_INSTRUMENT_TICKMARK_PRIMARY
                         : COLOR_CONTAINER_BACKGROUND;
        rectangle = (mr_rectangle_t){
            xTick,
            MEASUREMENT_BAR_TICKS_TOP,
            tickWidth,
            MEASUREMENT_BAR_TICKS_UPPER_HEIGHT};
        setFillColor(colorIndex);
        drawRectangle(&rectangle);

        colorIndex = isLargeTick
                         ? COLOR_INSTRUMENT_TICKMARK_PRIMARY
                     : isTick
                         ? COLOR_INSTRUMENT_TICKMARK_SECONDARY
                         : COLOR_CONTAINER_BACKGROUND;

        rectangle.y += MEASUREMENT_BAR_TICKS_UPPER_HEIGHT;
        rectangle.height = MEASUREMENT_BAR_TICKS_LOWER_HEIGHT;
        setFillColor(colorIndex);
        drawRectangle(&rectangle);

        xTick = xTickNext;
    }

    // Bar outer frame
    const mr_rectangle_t barFrameRectangle = {
        MEASUREMENT_BAR_FRAME_LEFT,
        MEASUREMENT_BAR_FRAME_TOP,
        MEASUREMENT_BAR_FRAME_WIDTH,
        MEASUREMENT_BAR_FRAME_HEIGHT};
    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);
    drawFrame(&barFrameRectangle);

    // Bar inner frame
    rectangle = (mr_rectangle_t){
        MEASUREMENT_BAR_INNERFRAME_LEFT,
        MEASUREMENT_BAR_INNERFRAME_TOP,
        1,
        MEASUREMENT_BAR_INNERFRAME_HEIGHT};
    setFillColor(COLOR_INSTRUMENT_FRAME_PRIMARY);
    drawRectangle(&rectangle);

    rectangle.x = MEASUREMENT_BAR_INNERFRAME_LEFT + MEASUREMENT_BAR_INNERFRAME_WIDTH - 1;
    setFillColor(COLOR_INSTRUMENT_FRAME_PRIMARY + 2 * COLOR_INSTRUMENT_OFFSET);
    drawRectangle(&rectangle);

    // Bar
    uint32_t xBar = 0;
    while (xBar < MEASUREMENT_BAR_INSTRUMENT_WIDTH)
    {
        uint32_t xBarNext = 0;
        if (xBar < instrumentValue)
            xBarNext = instrumentValue;
        else
            xBarNext = MEASUREMENT_BAR_INSTRUMENT_WIDTH;

        // Color
        uint32_t alertZoneColor;
#if defined(DISPLAY_MONOCHROME)
        alertZoneColor = 0;
#elif defined(DISPLAY_COLOR)
        if (xBar < instrumentAlertZone1Value)
        {
            if (xBarNext > instrumentAlertZone1Value)
                xBarNext = instrumentAlertZone1Value;

            alertZoneColor = 0;
        }
        else if (xBar < instrumentAlertZone2Value)
        {
            if (xBarNext > instrumentAlertZone2Value)
                xBarNext = instrumentAlertZone2Value;

            alertZoneColor = COLOR_INSTRUMENT_OFFSET;
        }
        else
            alertZoneColor = 2 * COLOR_INSTRUMENT_OFFSET;
#endif

        // Draw
        rectangle = (mr_rectangle_t){
            MEASUREMENT_BAR_INSTRUMENT_LEFT + xBar,
            MEASUREMENT_BAR_INSTRUMENT_TOP,
            xBarNext - xBar,
            MEASUREMENT_BAR_INSTRUMENT_HEIGHT,
        };
        setFillColor(alertZoneColor + ((xBar < instrumentValue)
                                           ? COLOR_INSTRUMENT_ENHANCED_PRIMARY
                                           : COLOR_INSTRUMENT_FRAME_PRIMARY));
        drawRectangle(&rectangle);

        rectangle.y = MEASUREMENT_BAR_INNERFRAME_TOP;
        rectangle.height = 1;
        setFillColor(alertZoneColor + COLOR_INSTRUMENT_FRAME_PRIMARY);
        drawRectangle(&rectangle);

        rectangle.y = MEASUREMENT_BAR_INNERFRAME_TOP +
                      MEASUREMENT_BAR_INNERFRAME_HEIGHT - 1;
        drawRectangle(&rectangle);

        xBar = xBarNext;
    }

    // Margin
    const mr_rectangle_t barOuterMargin = {
        MEASUREMENT_BAR_OUTERMARGIN_LEFT,
        MEASUREMENT_BAR_OUTERMARGIN_TOP,
        MEASUREMENT_BAR_OUTERMARGIN_WIDTH,
        MEASUREMENT_BAR_OUTERMARGIN_HEIGHT};
    const mr_rectangle_t barInnerMargin = {
        MEASUREMENT_BAR_INNERMARGIN_LEFT,
        MEASUREMENT_BAR_INNERMARGIN_TOP,
        MEASUREMENT_BAR_INNERMARGIN_WIDTH,
        MEASUREMENT_BAR_INNERMARGIN_HEIGHT};
    setFillColor(COLOR_CONTAINER_BACKGROUND);
    drawMargin(&barOuterMargin,
               &barInnerMargin);
}

static int16_t getHistoryY(uint32_t value,
                           uint32_t dataExponentMinValue,
                           uint32_t dataScale)
{
    return value
               ? ((value - dataExponentMinValue) * dataScale) >> 8
               : 0;
}

void drawHistory(float scale,
                 const char *unitString,
                 uint32_t timeTickNum,
                 const uint8_t *data,
                 uint32_t alertZone1Value,
                 uint32_t alertZone2Value)
{
    if (!alertZone1Value)
        alertZone1Value = 2 * UINT8_MAX;
    if (!alertZone2Value)
        alertZone2Value = 2 * UINT8_MAX;
    if (alertZone1Value > alertZone2Value)
        alertZone1Value = alertZone2Value;

    // Get min/max
    uint32_t dataMax = 0;
    uint32_t dataMin = UCHAR_MAX;
    for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        uint32_t value = data[i];

        if (value)
        {
            if (value > dataMax)
                dataMax = value;

            if (value < dataMin)
                dataMin = value;
        }
    }

    // Labels
    char topLabelString[16];
    char bottomLabelString[16];
    int32_t dataExponentMinValue = 0;
    uint32_t dataTickNum = 1;

    strclr(topLabelString);
    strclr(bottomLabelString);

    if (dataMax > 0)
    {
        dataMax += HISTORY_MARGIN;
        if (dataMin > HISTORY_MARGIN)
            dataMin -= HISTORY_MARGIN;

        uint32_t cpsToRateUnit = (uint32_t)(HISTORY_DECADE * (16 + log10f(scale * HISTORY_VALUE_MIN))) + 1;

        int32_t exponentMax = (int32_t)((cpsToRateUnit + dataMax) / HISTORY_DECADE - 16 + 1);
        int32_t exponentMin = (int32_t)((cpsToRateUnit + dataMin) / HISTORY_DECADE - 16);

        dataExponentMinValue = (int32_t)((exponentMin + 16) * HISTORY_DECADE - cpsToRateUnit);
        dataTickNum = exponentMax - exponentMin;

        strcatDecimalPowerWithMetricPrefix(topLabelString, exponentMax, -2);
        strcat(topLabelString, unitString);

        strcatDecimalPowerWithMetricPrefix(bottomLabelString, exponentMin, -2);
        strcat(bottomLabelString, unitString);
    }

    uint32_t dataRange = HISTORY_DECADE * dataTickNum;

    // Labels
    const mr_rectangle_t topLabelRectangle = {
        HISTORY_TOP_LABEL_LEFT,
        HISTORY_TOP_LABEL_TOP,
        HISTORY_TOP_LABEL_WIDTH,
        HISTORY_TOP_LABEL_HEIGHT};
    const mr_point_t topLabelOffset = {
        HISTORY_TOP_LABEL_OFFSET_LEFT,
        HISTORY_TOP_LABEL_OFFSET_TOP};
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    drawRightAlignedText(topLabelString,
                         &topLabelRectangle,
                         &topLabelOffset);

    const mr_rectangle_t bottomLabelRectangle = {
        HISTORY_BOTTOM_LABEL_LEFT,
        HISTORY_BOTTOM_LABEL_TOP,
        HISTORY_BOTTOM_LABEL_WIDTH,
        HISTORY_BOTTOM_LABEL_HEIGHT};
    const mr_point_t bottomLabelOffset = {
        HISTORY_BOTTOM_LABEL_OFFSET_LEFT,
        HISTORY_BOTTOM_LABEL_OFFSET_TOP};
    drawRightAlignedText(bottomLabelString,
                         &bottomLabelRectangle,
                         &bottomLabelOffset);

    // Space and frame
    const mr_rectangle_t leftSpaceRectangle = {
        HISTORY_SPACE_LEFT_LEFT,
        HISTORY_SPACE_LEFT_TOP,
        HISTORY_SPACE_LEFT_WIDTH,
        HISTORY_SPACE_LEFT_HEIGHT};
    drawRectangle(&leftSpaceRectangle);

    const mr_rectangle_t rightSpaceRectangle = {
        HISTORY_SPACE_RIGHT_LEFT,
        HISTORY_SPACE_RIGHT_TOP,
        HISTORY_SPACE_RIGHT_WIDTH,
        HISTORY_SPACE_RIGHT_HEIGHT};
    drawRectangle(&rightSpaceRectangle);

    const mr_rectangle_t frameRectangle = {
        HISTORY_LEFT,
        HISTORY_TOP,
        HISTORY_WIDTH,
        HISTORY_HEIGHT};
    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);
    drawFrame(&frameRectangle);

    // Value tick table
    uint8_t valueTickTable[32];
    uint32_t valueTickTableIndex = 0;

    for (uint32_t yTickIndex = 1;
         yTickIndex <= dataTickNum;
         yTickIndex++)
    {
        int16_t yTickNext = yTickIndex * HISTORY_DATA_HEIGHT / dataTickNum;

        valueTickTable[valueTickTableIndex++] = yTickNext;
        valueTickTable[valueTickTableIndex++] = yTickNext + 1;
    }

    // History
    uint32_t dataScale = 257 * HISTORY_DATA_HEIGHT / dataRange;

    int16_t timeTickIndex = 1;
    int16_t timeTickNext = HISTORY_DATA_WIDTH / timeTickNum;

    int16_t yDataLast = 0;
#if defined(DISPLAY_COLOR)
    int16_t yAlertZone1 = getHistoryY(alertZone1Value,
                                      dataExponentMinValue,
                                      dataScale);
    int16_t yAlertZone2 = getHistoryY(alertZone2Value,
                                      dataExponentMinValue,
                                      dataScale);
#endif

    for (int16_t x = 0; x < HISTORY_DATA_WIDTH; x++)
    {
        // Time tick
        bool timeTickDraw = false;
        if (x == timeTickNext)
        {
            timeTickIndex++;
            timeTickNext = timeTickIndex * HISTORY_DATA_WIDTH / timeTickNum;
            timeTickDraw = true;
        }

        // Value tick
        valueTickTableIndex = 0;

        // Data
        int16_t yData = getHistoryY(data[x],
                                    dataExponentMinValue,
                                    dataScale);

        if (x == 0)
            yDataLast = yData;

        int16_t yDataBottom;
        int16_t yDataTop;
        if (yData < yDataLast)
        {
            yDataBottom = yData;
            yDataTop = yDataLast;
        }
        else
        {
            yDataBottom = yDataLast;
            yDataTop = yData;
        }
        yDataBottom--;
        yDataLast = yData;

        // Render
        mr_color_t imageBuffer[HISTORY_DATA_HEIGHT];

        int16_t y = 0;
        while (y < HISTORY_DATA_HEIGHT)
        {
            ColorIndex colorIndex = COLOR_INSTRUMENT_FRAME_PRIMARY;
            int16_t yNext = 0;

            // Render time and value ticks
            int16_t yTickNext;
            if (timeTickDraw)
            {
                yTickNext = HISTORY_DATA_HEIGHT;

                colorIndex = COLOR_INSTRUMENT_FRAME_TERTIARY;
            }
            else
            {
                yTickNext = valueTickTable[valueTickTableIndex];

                if (valueTickTableIndex & 0b1)
                    colorIndex = COLOR_INSTRUMENT_FRAME_TERTIARY;

                if (y == yTickNext)
                    valueTickTableIndex++;
            }
            yNext = yTickNext;

            // Render data
            int16_t yDataNext;
            if (y < yDataBottom)
            {
                yDataNext = yDataBottom;

                if (colorIndex == COLOR_INSTRUMENT_FRAME_PRIMARY)
                    colorIndex = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY;
                else
                    colorIndex = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY;
            }
            else if (y < yDataTop)
            {
                yDataNext = yDataTop;

                colorIndex = COLOR_INSTRUMENT_ENHANCED_PRIMARY;
            }
            else
                yDataNext = HISTORY_DATA_HEIGHT;
            if (yDataNext < yNext)
                yNext = yDataNext;

#if !defined(DISPLAY_MONOCHROME)
            // Render alert zones
            int16_t yAlertZoneNext;
            if (y < yAlertZone1)
                yAlertZoneNext = yAlertZone1;
            else if (y < yAlertZone2)
            {
                yAlertZoneNext = yAlertZone2;

                colorIndex += COLOR_INSTRUMENT_OFFSET;
            }
            else
            {
                yAlertZoneNext = HISTORY_DATA_HEIGHT;

                colorIndex += 2 * COLOR_INSTRUMENT_OFFSET;
            }
            if (yAlertZoneNext < yNext)
                yNext = yAlertZoneNext;
#endif
            // Render to imagebuffer
            mr_color_t fillColor = getFillColor(colorIndex);
            for (int32_t i = y; i < yNext; i++)
                imageBuffer[(HISTORY_DATA_HEIGHT - 1) - i] = fillColor;

            y = yNext;
        }

        // Draw
        mr_rectangle_t historyImageRectangle = {
            HISTORY_DATA_LEFT + x,
            HISTORY_DATA_TOP,
            1,
            HISTORY_DATA_HEIGHT};
        drawImage(&historyImageRectangle,
                  imageBuffer);
    }
}

void drawRNG(const char *title,
             bool largeFont,
             const char *rngString,
             const char *stateString)
{
    drawTitleBar(title, false);

    setStrokeColor(COLOR_INSTRUMENT_ENHANCED_PRIMARY);

    if (!largeFont)
    {
        // RNG string
        const mr_rectangle_t rngStringRectangle = {
            RNG_STRING_LEFT,
            RNG_STRING_TOP,
            RNG_STRING_WIDTH,
            RNG_STRING_HEIGHT};
        const mr_point_t rngStringOffset = {
            RNG_STRING_OFFSETX,
            RNG_STRING_OFFSETY};
        setFont(font_small);
        drawText(rngString,
                 &rngStringRectangle,
                 &rngStringOffset);

        // State
        const mr_rectangle_t rngStateRectangle = {
            RNG_STATE_LEFT,
            RNG_STATE_TOP,
            RNG_STATE_WIDTH,
            RNG_STATE_HEIGHT};
        const mr_point_t rngStateOffset = {
            RNG_STATE_OFFSETX,
            RNG_STATE_OFFSETY};
        setFont(font_medium);
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawCenteredText(stateString,
                         &rngStateRectangle,
                         &rngStateOffset);
    }
    else
    {
        // State
        const mr_rectangle_t rngNumberRectangle = {
            RNG_NUMBER_LEFT,
            RNG_NUMBER_TOP,
            RNG_NUMBER_WIDTH,
            RNG_NUMBER_HEIGHT};
        const mr_point_t rngNumberOffset = {
            RNG_NUMBER_OFFSETX,
            RNG_NUMBER_OFFSETY};
        setFont(font_large);
        drawCenteredText(rngString,
                         &rngNumberRectangle,
                         &rngNumberOffset);
    }
}

static cstring statisticsKeyStrings[] = {
#if defined(DISPLAY_LANDSCAPE) && DISPLAY_MONOCHROME
    STRING_TUBE_LIFE_TIME,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_DEVICE_ID,
    STRING_EMPTY,
    STRING_VOLTAGE,
#elif defined(DISPLAY_LANDSCAPE) && DISPLAY_COLOR
    STRING_TUBE_LIFE_TIME,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_EMPTY,
    STRING_DEVICE_ID,
    STRING_EMPTY,
    STRING_VOLTAGE,
#elif defined(DISPLAY_PORTRAIT)
    STRING_TUBE,
    STRING_LIFE_TIME,
    STRING_PULSES,
    STRING_DEAD_TIME_SHORT,
    STRING_EMPTY,
    STRING_DEVICE,
    STRING_ID,
    STRING_EMPTY,
    STRING_VOLTAGE_SHORT,
#endif
};

enum
{
    STATISTICS_TITLE,
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
};

static const uint8_t statisticsType[] = {
#if defined(DISPLAY_LANDSCAPE) && DISPLAY_MONOCHROME
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
#elif defined(DISPLAY_LANDSCAPE) && DISPLAY_COLOR
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_TITLE,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
#elif defined(DISPLAY_PORTRAIT)
    STATISTICS_TITLE,
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_TITLE,
    STATISTICS_TITLE,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
#endif
};

#define STATISTICS_ENTRY_NUM sizeof(statisticsType)

void drawStatistics(void)
{
    drawTitleBar(getString(STRING_STATISTICS), false);

    setFont(font_small);

    char keyString[32];
    char valueString[32];
    char unitString[16];

    mr_rectangle_t rectangle;
    mr_point_t offset;

    uint32_t y = CONTENT_TOP;

    for (uint32_t i = 0; i < STATISTICS_ENTRY_NUM; i++)
    {
        strcpy(keyString, getString(statisticsKeyStrings[i]));
        strclr(valueString);
        strclr(unitString);

        switch (statisticsType[i])
        {
        case STATISTICS_TITLE:
            break;

        case STATISTICS_TUBE_TIME:
            strcatTime(valueString, getTubeTime());

            break;

        case STATISTICS_TUBE_PULSES:
            strcatUInt32(valueString, getTubePulseCount(), 0);

            break;

        case STATISTICS_DEAD_TIME:
        {
            float deadTime = getTubeDeadTime();
            if (deadTime >= 1)
                strcpy(valueString, getString(STRING_NONE));
            else
            {
                strcpy(valueString, getString(STRING_LESSTHAN));
                strcatFloat(valueString, 1000000 * deadTime, 1);
                strcatChar(unitString, ' ');
                strcat(unitString, getString(STRING_MICROSECONDS));
            }

            break;
        }

        case STATISTICS_DEVICEID_1:
            getDeviceId(valueString);
            valueString[12] = '\0';

            break;

        case STATISTICS_DEVICEID_2:
            getDeviceId(valueString);
            strcpy(valueString, valueString + 12);

            break;

        case STATISTICS_VOLTAGE:
            strcatFloat(valueString, getBatteryVoltage(), 3);
            strclr(unitString);
            strcatChar(unitString, ' ');
            strcat(unitString, getString(STRING_VOLTS));

            break;
        }

        // Key
        rectangle = (mr_rectangle_t){
            STATISTICS_KEY_LEFT,
            y,
            STATISTICS_KEY_WIDTH,
            STATISTICS_ENTRY_HEIGHT};

        offset = (mr_point_t){
            STATISTICS_KEY_OFFSETX,
            0};
        if (i == 0)
        {
            rectangle.height += STATISTICS_TOPMARGIN_HEIGHT;
            offset.y += STATISTICS_TOPMARGIN_HEIGHT;
            y += STATISTICS_TOPMARGIN_HEIGHT;
        }
        else if (i == (STATISTICS_ENTRY_NUM - 1))
            rectangle.height += STATISTICS_BOTTOMMARGIN_HEIGHT;
        setStrokeColor(
            (!valueString[0] && !unitString[0])
                ? COLOR_ELEMENT_ACTIVE
                : COLOR_ELEMENT_NEUTRAL);
        drawText(keyString,
                 &rectangle,
                 &offset);

        // Value
        rectangle.x = STATISTICS_VALUE_LEFT;
        rectangle.width = STATISTICS_VALUE_WIDTH;
        offset.x = STATISTICS_VALUE_OFFSETX;
        drawValueAndUnit(valueString,
                         unitString,
                         &rectangle,
                         &offset,
                         false);

        y += FONT_SMALL_LINE_HEIGHT;
    }
}

#if defined(GAME)

void drawGame(const uint8_t board[8][8],
              const char time[2][16],
              const char history[3][2][6])
{
    mr_rectangle_t rectangle;
    mr_point_t offset;

#if defined(DISPLAY_COLOR)
    drawTitleBar(getString(STRING_GAME), false);
#endif

    // Board
    static const char *const gamePieceMap = "@AACFBDE";

    setFont(font_symbols);

    rectangle.width = GAME_SQUARE_WIDTH;
    rectangle.height = GAME_SQUARE_HEIGHT;

    const mr_point_t gamePieceOffset = {
        0,
        0};

    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            uint8_t pieceCode = board[y][x];

            uint8_t pieceType = (pieceCode >> 0) & 0x7;
            bool pieceBlack = (pieceCode >> 3) & 0x1;
            bool pieceSelected = (pieceCode >> 4) & 0x1;
            bool squareBlack = (x + y) & 0x1;

            char pieceChar = gamePieceMap[pieceType];

#if defined(DISPLAY_MONOCHROME)
            bool isInverted;
            if (pieceSelected)
            {
                pieceChar |= 0x10;
                isInverted = !pieceBlack;
            }
            else
            {
                pieceChar |= (pieceBlack ^ squareBlack ^ 1) << 3;
                isInverted = squareBlack;
            }
            setFillColor(isInverted
                             ? COLOR_GAME_SQUARE_BLACK
                             : COLOR_GAME_SQUARE_WHITE);
#elif defined(DISPLAY_COLOR)
            setStrokeColor(pieceBlack
                               ? COLOR_GAME_PIECE_BLACK
                               : COLOR_GAME_PIECE_WHITE);
            setFillColor(pieceSelected
                             ? COLOR_SELECTED_ENABLED_BACKGROUND
                         : squareBlack
                             ? COLOR_GAME_SQUARE_BLACK
                             : COLOR_GAME_SQUARE_WHITE);
#endif

            char pieceString[2];

            strclr(pieceString);
            strcatChar(pieceString, pieceChar);

            rectangle.x = GAME_BOARD_LEFT + GAME_SQUARE_WIDTH * x;
            rectangle.y = GAME_BOARD_TOP + GAME_SQUARE_HEIGHT * y;

            drawText(pieceString,
                     &rectangle,
                     &gamePieceOffset);
        }
    }

    // Top timer
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);

    const mr_rectangle_t topTimerRectangle = {
        GAME_TOPTIMER_LEFT,
        GAME_TOPTIMER_TOP,
        GAME_TOPTIMER_WIDTH,
        GAME_TOPTIMER_HEIGHT};
    const mr_point_t topTimerOffset = {
        GAME_TOPTIMER_OFFSETX,
        GAME_TOPTIMER_OFFSETY};
#if defined(DISPLAY_LANDSCAPE)
    drawText(time[0],
             &topTimerRectangle,
             &topTimerOffset);
#elif defined(DISPLAY_PORTRAIT)
    drawRightAlignedText(time[0],
                         &topTimerRectangle,
                         &topTimerOffset);
#endif

    // History
    rectangle.y = GAME_HISTORY_TOP;
    rectangle.height = GAME_HISTORY_HEIGHT / 3;

    offset = (mr_point_t){
        0,
        0};

    for (uint32_t y = 0; y < 3; y++)
    {
        rectangle.x = GAME_HISTORY_LEFT;
        rectangle.width = GAME_HISTORY_COLUMN1_WIDTH;
        offset.x = GAME_HISTORY_COLUMN1_OFFSETX;
        drawText(history[y][0],
                 &rectangle,
                 &offset);

        rectangle.x += GAME_HISTORY_COLUMN1_WIDTH;
        rectangle.width = GAME_HISTORY_COLUMN2_WIDTH;
        offset.x = 0;
        drawText(history[y][1],
                 &rectangle,
                 &offset);

        rectangle.y += GAME_HISTORY_HEIGHT / 3;
    }

    // Bottom timer
    const mr_rectangle_t bottomTimerRectangle = {
        GAME_BOTTOMTIMER_LEFT,
        GAME_BOTTOMTIMER_TOP,
        GAME_BOTTOMTIMER_WIDTH,
        GAME_BOTTOMTIMER_HEIGHT};

    const mr_point_t bottomTimerOffset = {
        GAME_BOTTOMTIMER_OFFSETX,
        GAME_BOTTOMTIMER_OFFSETY};

#if defined(DISPLAY_LANDSCAPE)
    drawText(time[1],
             &bottomTimerRectangle,
             &bottomTimerOffset);
#elif defined(DISPLAY_PORTRAIT)
    drawRightAlignedText(time[1],
                         &bottomTimerRectangle,
                         &bottomTimerOffset);
#endif

    // Spaces
#if defined(GAME_LEFTMARGIN_WIDTH)
    const mr_rectangle_t gameLeftMarginRectangle = {
        GAME_LEFTMARGIN_LEFT,
        GAME_LEFTMARGIN_TOP,
        GAME_LEFTMARGIN_WIDTH,
        GAME_LEFTMARGIN_HEIGHT};

    drawRectangle(&gameLeftMarginRectangle);
#endif

#if defined(GAME_RIGHTMARGIN_WIDTH)
    const mr_rectangle_t gameRightMarginRectangle = {
        GAME_RIGHTMARGIN_LEFT,
        GAME_RIGHTMARGIN_TOP,
        GAME_RIGHTMARGIN_WIDTH,
        GAME_RIGHTMARGIN_HEIGHT};

    drawRectangle(&gameRightMarginRectangle);
#endif
}

#endif

// Display contrast level menu

#if defined(DISPLAY_MONOCHROME)

static const char *onDisplayContrastMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    if (index >= DISPLAY_CONTRAST_NUM)
        return NULL;

    *menuStyle = (index == settings.displayContrast);

    strcpy(menuOption, getString(STRING_CONTRAST_LEVEL));
    strcatChar(menuOption, ' ');
    strcatUInt32(menuOption, index + 1, 0);

    return menuOption;
}

static void onDisplayContrastMenuSelect(uint32_t index)
{
    settings.displayContrast = index;

    updateDisplayContrast();
}

static MenuState displayContrastMenuState;

static Menu displayContrastMenu = {
    STRING_CONTRAST,
    &displayContrastMenuState,
    onDisplayContrastMenuGetOption,
    onDisplayContrastMenuSelect,
    onDisplaySubMenuBack,
};

View displayContrastMenuView = {
    onMenuEvent,
    &displayContrastMenu,
};

#endif

// Display theme menu

#if defined(DISPLAY_COLOR)

static cstring displayThemeMenuOptions[] = {
    STRING_THEME_DAY,
    STRING_THEME_DUSK,
    STRING_THEME_NIGHT,
    NULL,
};

static const char *onDisplayThemeMenuGetOption(uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayTheme);

    return getString(displayThemeMenuOptions[index]);
}

static void onDisplayThemeMenuSelect(uint32_t index)
{
    settings.displayTheme = index;

    requestViewUpdate();
}

static MenuState displayThemeMenuState;

static Menu displayThemeMenu = {
    STRING_THEME,
    &displayThemeMenuState,
    onDisplayThemeMenuGetOption,
    onDisplayThemeMenuSelect,
    onDisplaySubMenuBack,
};

View displayThemeMenuView = {
    onMenuEvent,
    &displayThemeMenu,
};

#endif

// Display brightness level menu

static cstring displayBrightnessMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
    NULL,
};

static const char *onDisplayBrightnessMenuGetOption(uint32_t index,
                                                    MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayBrightness);

    return getString(displayBrightnessMenuOptions[index]);
}

static void onDisplayBrightnessMenuSelect(uint32_t index)
{
    settings.displayBrightness = index;
}

static MenuState displayBrightnessMenuState;

static Menu displayBrightnessMenu = {
    STRING_BRIGHTNESS,
    &displayBrightnessMenuState,
    onDisplayBrightnessMenuGetOption,
    onDisplayBrightnessMenuSelect,
    onDisplaySubMenuBack,
};

View displayBrightnessMenuView = {
    onMenuEvent,
    &displayBrightnessMenu,
};

// Display sleep menu

static cstring displaySleepMenuOptions[] = {
#if defined(DISPLAY_MONOCHROME)
    STRING_ALWAYS_OFF,
#endif
    STRING_10_SECONDS,
    STRING_30_SECONDS,
    STRING_1_MINUTE,
    STRING_2_MINUTES,
    STRING_5_MINUTES,
    STRING_ALWAYS_ON,
    NULL,
};

static const char *onDisplaySleepMenuGetOption(uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displaySleep);

    return getString(displaySleepMenuOptions[index]);
}

static void onDisplaySleepMenuSelect(uint32_t index)
{
    settings.displaySleep = index;
}

static MenuState displaySleepMenuState;

static Menu displaySleepMenu = {
    STRING_SLEEP,
    &displaySleepMenuState,
    onDisplaySleepMenuGetOption,
    onDisplaySleepMenuSelect,
    onDisplaySubMenuBack,
};

View displaySleepMenuView = {
    onMenuEvent,
    &displaySleepMenu,
};

// Display menu

static SubMenuOption displayMenuOptions[] = {
#if defined(DISPLAY_COLOR)
    {STRING_THEME, &displayThemeMenuView},
#endif
    {STRING_BRIGHTNESS, &displayBrightnessMenuView},
#if defined(DISPLAY_MONOCHROME)
    {STRING_CONTRAST, &displayContrastMenuView},
#endif
    {STRING_SLEEP, &displaySleepMenuView},
    {NULL},
};

static const char *onDisplayMenuGetOption(uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(displayMenuOptions[index].title);
}

static void onDisplayMenuSelect(uint32_t index)
{
    setView(displayMenuOptions[index].view);
}

void onDisplaySubMenuBack(void)
{
    setView(&displayMenuView);
}

static MenuState displayMenuState;

static Menu displayMenu = {
    STRING_DISPLAY,
    &displayMenuState,
    onDisplayMenuGetOption,
    onDisplayMenuSelect,
    onSettingsSubMenuBack,
};

View displayMenuView = {
    onMenuEvent,
    &displayMenu,
};
