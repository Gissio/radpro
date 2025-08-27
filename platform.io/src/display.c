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

#include FONT_SYMBOLS
#include FONT_LARGE
#include FONT_MEDIUM
#include FONT_SMALL

// Landscape layout:

//
//              Value
// -------------------+-----------
//                    |       Unit
// InfoKey            +-----------
// -------------------+ Confidence
// InfoValue          |
//                    |

// Portrait layout:

//
//         Value
// ---------------------
//                  Unit
// ---------------------
//            Confidence
// ---------------------
//
// InfoKey
// ---------------------
// InfoValue
//

// Display metrics

#if defined(DISPLAY_128X64)

#define DISPLAY_LANDSCAPE

#define PADDING_LEFT 3
#define PADDING_RIGHT (PADDING_LEFT - 1)

#define NOTIFICATION_GAP 1

#define TITLEBAR_HEIGHT 8
#define TITLEBAR_SHADOW_HEIGHT 0

#define CONTENT_LINE_MAX 6

#define MENU_LINE_HEIGHT 14

#define MEASUREMENT_VALUECONTENT_SCALE 1.0F
#define MEASUREMENT_INFO_KEY_WIDTH 82
#define MEASUREMENT_PADDING_LEFT 7
#define MEASUREMENT_PADDING_RIGHT 6
#define MEASUREMENT_INFO_PADDING_BOTTOM 0
#define MEASUREMENT_BAR_PADDING_BOTTOM 0

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0
#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8

#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2 - FONT_SMALL_CAP_HEIGHT / 2)

#elif defined(DISPLAY_320X240)

#define DISPLAY_LANDSCAPE

#define PADDING_LEFT 12
#define PADDING_RIGHT PADDING_LEFT

#define NOTIFICATION_GAP 0

#define TITLEBAR_HEIGHT 48
#define TITLEBAR_SHADOW_HEIGHT 1

#define CONTENT_LINE_MAX 9

#define MENU_LINE_HEIGHT 48

#define MEASUREMENT_VALUECONTENT_SCALE 1.08F
#define MEASUREMENT_INFO_KEY_WIDTH 172
#define MEASUREMENT_PADDING_LEFT PADDING_LEFT
#define MEASUREMENT_PADDING_RIGHT PADDING_RIGHT
#define MEASUREMENT_INFO_PADDING_BOTTOM (MEASUREMENT_PADDING_LEFT / 2)
#define MEASUREMENT_BAR_PADDING_BOTTOM MEASUREMENT_PADDING_LEFT

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0
#define GAME_SQUARE_WIDTH 30
#define GAME_SQUARE_HEIGHT 30

#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2 - FONT_SMALL_CAP_HEIGHT / 2)

#elif defined(DISPLAY_240X320)

#define DISPLAY_PORTRAIT

// #if FONT_SMALL_SIZE <= 16
// #define PADDING_LEFT 12
// #else
// #define PADDING_LEFT 8
// #endif
#define PADDING_LEFT 8
#define PADDING_RIGHT PADDING_LEFT

#define NOTIFICATION_GAP 0

#define TITLEBAR_HEIGHT 48
#define TITLEBAR_SHADOW_HEIGHT 1

#define CONTENT_LINE_MAX 9

#define MENU_LINE_HEIGHT 45

#define MEASUREMENT_VALUECONTENT_SCALE 1.08F
#define MEASUREMENT_INFO_KEY_WIDTH CONTENT_WIDTH
#define MEASUREMENT_PADDING_LEFT 12
#define MEASUREMENT_PADDING_RIGHT MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFO_PADDING_BOTTOM (MEASUREMENT_PADDING_LEFT - FONT_SMALL_DESCENT)
#define MEASUREMENT_BAR_PADDING_BOTTOM MEASUREMENT_PADDING_LEFT

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP CONTENT_TOP
#define GAME_SQUARE_WIDTH 30
#define GAME_SQUARE_HEIGHT 30

#define STATISTICS_KEY_WIDTH (110 * 16 / FONT_SMALL_SIZE - FONT_SMALL_CAP_HEIGHT / 2)

#endif

// Common metrics

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

#define MEASUREMENT_VALUECONTENT_HEIGHT ((int)(MEASUREMENT_VALUECONTENT_SCALE * FONT_LARGE_CAP_HEIGHT))
#define MEASUREMENT_VALUECONTENT_OFFSETY (-(FONT_LARGE_ASCENT - FONT_LARGE_CAP_HEIGHT) + (MEASUREMENT_VALUECONTENT_HEIGHT - FONT_LARGE_CAP_HEIGHT) / 2)
#define MEASUREMENT_UNITCONTENT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_CONFIDENCECONTENT_HEIGHT ((int)(0.8F * FONT_SMALL_LINE_HEIGHT))
#define MEASUREMENT_CONFIDENCECONTENT_OFFSETY (-(FONT_MEDIUM_LINE_HEIGHT - MEASUREMENT_UNITCONTENT_HEIGHT) / 2)
#define MEASUREMENT_TRIAD_HEIGHT (MEASUREMENT_VALUECONTENT_HEIGHT + MEASUREMENT_UNITCONTENT_HEIGHT + MEASUREMENT_CONFIDENCECONTENT_HEIGHT)
#define MEASUREMENT_TRIADPADDING_TOP ((CONTENT_HEIGHT - MEASUREMENT_TRIAD_HEIGHT) / 2)

#define MEASUREMENT_VALUE_LEFT CONTENT_LEFT
#define MEASUREMENT_VALUE_TOP CONTENT_TOP
#define MEASUREMENT_VALUE_WIDTH CONTENT_WIDTH
#define MEASUREMENT_VALUE_HEIGHT (MEASUREMENT_TRIADPADDING_TOP + MEASUREMENT_VALUECONTENT_HEIGHT)
#define MEASUREMENT_VALUE_BOTTOM (MEASUREMENT_VALUE_TOP + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_VALUE_OFFSETX (MEASUREMENT_VALUE_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_VALUE_OFFSETY (MEASUREMENT_TRIADPADDING_TOP + MEASUREMENT_VALUECONTENT_OFFSETY)

#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_UNIT_LEFT (CONTENT_LEFT + MEASUREMENT_INFO_KEY_WIDTH)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_UNIT_LEFT CONTENT_LEFT
#endif
#define MEASUREMENT_UNIT_TOP MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_UNIT_WIDTH (CONTENT_WIDTH - MEASUREMENT_UNIT_LEFT)
#define MEASUREMENT_UNIT_HEIGHT MEASUREMENT_UNITCONTENT_HEIGHT
#define MEASUREMENT_UNIT_BOTTOM (MEASUREMENT_UNIT_TOP + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_UNIT_OFFSETX (MEASUREMENT_UNIT_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_UNIT_OFFSETY MEASUREMENT_CONFIDENCECONTENT_OFFSETY

#define MEASUREMENT_CONFIDENCE_LEFT MEASUREMENT_UNIT_LEFT
#define MEASUREMENT_CONFIDENCE_TOP MEASUREMENT_UNIT_BOTTOM
#define MEASUREMENT_CONFIDENCE_WIDTH MEASUREMENT_UNIT_WIDTH
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_CONFIDENCE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_UNIT_BOTTOM)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_CONFIDENCE_HEIGHT FONT_SMALL_LINE_HEIGHT
#endif
#define MEASUREMENT_CONFIDENCE_BOTTOM (MEASUREMENT_CONFIDENCE_TOP + MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_CONFIDENCE_OFFSETX (MEASUREMENT_CONFIDENCE_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_CONFIDENCE_OFFSETY (-(FONT_SMALL_LINE_HEIGHT - MEASUREMENT_CONFIDENCECONTENT_HEIGHT) / 2)

#define MEASUREMENT_INFO_KEYCONTENT_TOP (MEASUREMENT_INFO_VALUE_TOP - MEASUREMENT_INFO_KEYCONTENT_HEIGHT)
#define MEASUREMENT_INFO_KEYCONTENT_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_INFO_VALUECONTENT_HEIGHT FONT_SMALL_LINE_HEIGHT

#define MEASUREMENT_INFO_KEY_LEFT CONTENT_LEFT
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_INFO_KEY_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_INFO_KEY_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define MEASUREMENT_INFO_KEY_HEIGHT (MEASUREMENT_INFO_VALUE_TOP - MEASUREMENT_INFO_KEY_TOP)
#define MEASUREMENT_INFO_KEY_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFO_KEY_OFFSETY (MEASUREMENT_INFO_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT)

#define MEASUREMENT_INFO_VALUE_LEFT MEASUREMENT_INFO_KEY_LEFT
#define MEASUREMENT_INFO_VALUE_TOP (CONTENT_BOTTOM - MEASUREMENT_INFO_VALUE_HEIGHT)
#define MEASUREMENT_INFO_VALUE_WIDTH MEASUREMENT_INFO_KEY_WIDTH
#define MEASUREMENT_INFO_VALUE_HEIGHT (MEASUREMENT_INFO_VALUECONTENT_HEIGHT + MEASUREMENT_INFO_PADDING_BOTTOM)
#define MEASUREMENT_INFO_VALUE_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFO_VALUE_OFFSETY 0

#define MEASUREMENT_ALERT_KEY_LEFT MEASUREMENT_INFO_KEY_LEFT
#define MEASUREMENT_ALERT_KEY_TOP MEASUREMENT_INFO_KEY_TOP
#define MEASUREMENT_ALERT_KEY_WIDTH MEASUREMENT_INFO_KEY_WIDTH
#define MEASUREMENT_ALERT_KEY_HEIGHT (MEASUREMENT_INFO_KEY_HEIGHT + MEASUREMENT_INFO_VALUE_HEIGHT)
#define MEASUREMENT_ALERT_KEY_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_ALERT_KEY_OFFSETY (MEASUREMENT_INFO_KEY_OFFSETY + FONT_SMALL_LINE_HEIGHT / 2)

#define MEASUREMENT_BAR_DECADES 7

#define MEASUREMENT_BAR_WIDTH MEASUREMENT_INFO_KEY_WIDTH

#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_BAR_LABEL_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_BAR_LABEL_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define MEASUREMENT_BAR_LABEL_HEIGHT (MEASUREMENT_BAR_TICKS_TOP - MEASUREMENT_BAR_LABEL_TOP)
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
#define MEASUREMENT_BAR_TICKS_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING_LEFT + 2)
#define MEASUREMENT_BAR_TICKS_TOP (MEASUREMENT_BAR_FRAME_TOP - MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT - MEASUREMENT_BAR_TICKS_HEIGHT)
#define MEASUREMENT_BAR_TICKS_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_TICKS_LEFT - 2)
#define MEASUREMENT_BAR_TICKS_LOWER_HEIGHT (MEASUREMENT_BAR_TICKS_HEIGHT - MEASUREMENT_BAR_TICKS_UPPER_HEIGHT)

#if defined(DISPLAY_MONOCHROME)
#define MEASUREMENT_BAR_FRAME_HEIGHT 8
#elif defined(DISPLAY_COLOR)
#define MEASUREMENT_BAR_FRAME_HEIGHT 18
#endif
#define MEASUREMENT_BAR_FRAME_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING_LEFT)
#define MEASUREMENT_BAR_FRAME_TOP (CONTENT_BOTTOM - MEASUREMENT_BAR_PADDING_BOTTOM - MEASUREMENT_BAR_FRAME_HEIGHT)
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_BAR_FRAME_RIGHT MEASUREMENT_BAR_WIDTH
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_BAR_FRAME_RIGHT (MEASUREMENT_BAR_WIDTH - MEASUREMENT_PADDING_LEFT)
#endif
#define MEASUREMENT_BAR_FRAME_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_FRAME_LEFT)

#define MEASUREMENT_BAR_INSTRUMENT_LEFT (MEASUREMENT_BAR_FRAME_LEFT + 1)
#define MEASUREMENT_BAR_INSTRUMENT_TOP (MEASUREMENT_BAR_FRAME_TOP + 1)
#define MEASUREMENT_BAR_INSTRUMENT_WIDTH (MEASUREMENT_BAR_FRAME_WIDTH - 2)
#define MEASUREMENT_BAR_INSTRUMENT_HEIGHT (MEASUREMENT_BAR_FRAME_HEIGHT - 2)

#define MEASUREMENT_BAR_OUTERPADDING_LEFT CONTENT_LEFT
#define MEASUREMENT_BAR_OUTERPADDING_TOP (MEASUREMENT_BAR_TICKS_TOP + MEASUREMENT_BAR_TICKS_HEIGHT)
#define MEASUREMENT_BAR_OUTERPADDING_WIDTH (MEASUREMENT_BAR_WIDTH - MEASUREMENT_BAR_OUTERPADDING_LEFT)
#define MEASUREMENT_BAR_OUTERPADDING_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_BAR_OUTERPADDING_TOP)

#define MEASUREMENT_BAR_INNERPADDING_LEFT (CONTENT_LEFT + MEASUREMENT_PADDING_LEFT)
#define MEASUREMENT_BAR_INNERPADDING_TOP MEASUREMENT_BAR_FRAME_TOP
#define MEASUREMENT_BAR_INNERPADDING_WIDTH MEASUREMENT_BAR_FRAME_WIDTH
#define MEASUREMENT_BAR_INNERPADDING_HEIGHT MEASUREMENT_BAR_FRAME_HEIGHT

#define HISTORY_WIDTH (HISTORY_BIN_NUM + 2)
#define HISTORY_HEIGHT (CONTENT_HEIGHT - 2 * FONT_SMALL_LINE_HEIGHT)
#define HISTORY_LEFT ((CONTENT_WIDTH - HISTORY_WIDTH) / 2)
#define HISTORY_TOP (CONTENT_TOP + (CONTENT_HEIGHT - HISTORY_HEIGHT) / 2)

#define HISTORY_DATA_LEFT (HISTORY_LEFT + 1)
#define HISTORY_DATA_TOP (HISTORY_TOP + 1)
#define HISTORY_DATA_WIDTH (HISTORY_WIDTH - 2)
#define HISTORY_DATA_HEIGHT (HISTORY_HEIGHT - 2)

#define HISTORY_MAX_LABEL_LEFT CONTENT_LEFT
#define HISTORY_MAX_LABEL_TOP CONTENT_TOP
#define HISTORY_MAX_LABEL_WIDTH CONTENT_WIDTH
#define HISTORY_MAX_LABEL_HEIGHT (HISTORY_TOP - CONTENT_TOP)
#define HISTORY_MAX_LABEL_OFFSET_LEFT (HISTORY_MAX_LABEL_WIDTH - PADDING_RIGHT)
#define HISTORY_MAX_LABEL_OFFSET_TOP 0

#define HISTORY_PERIOD_LABEL_LEFT CONTENT_LEFT
#define HISTORY_PERIOD_LABEL_TOP (HISTORY_TOP + HISTORY_HEIGHT)
#define HISTORY_PERIOD_LABEL_WIDTH (CONTENT_WIDTH / 2)
#define HISTORY_PERIOD_LABEL_HEIGHT (CONTENT_BOTTOM - HISTORY_MIN_LABEL_TOP)
#define HISTORY_PERIOD_LABEL_OFFSET_LEFT PADDING_LEFT
#define HISTORY_PERIOD_LABEL_OFFSET_TOP 0

#define HISTORY_MIN_LABEL_LEFT (HISTORY_PERIOD_LABEL_LEFT + HISTORY_PERIOD_LABEL_WIDTH)
#define HISTORY_MIN_LABEL_TOP (HISTORY_TOP + HISTORY_HEIGHT)
#define HISTORY_MIN_LABEL_WIDTH (CONTENT_WIDTH - HISTORY_PERIOD_LABEL_WIDTH)
#define HISTORY_MIN_LABEL_HEIGHT (CONTENT_BOTTOM - HISTORY_MIN_LABEL_TOP)
#define HISTORY_MIN_LABEL_OFFSET_LEFT (HISTORY_MIN_LABEL_WIDTH - PADDING_RIGHT)
#define HISTORY_MIN_LABEL_OFFSET_TOP 0

#define HISTORY_PADDING_LEFT_LEFT 0
#define HISTORY_PADDING_LEFT_TOP HISTORY_TOP
#define HISTORY_PADDING_LEFT_WIDTH HISTORY_LEFT
#define HISTORY_PADDING_LEFT_HEIGHT HISTORY_HEIGHT

#define HISTORY_PADDING_RIGHT_LEFT (HISTORY_LEFT + HISTORY_WIDTH)
#define HISTORY_PADDING_RIGHT_TOP HISTORY_TOP
#define HISTORY_PADDING_RIGHT_WIDTH (CONTENT_WIDTH - HISTORY_PADDING_RIGHT_LEFT)
#define HISTORY_PADDING_RIGHT_HEIGHT HISTORY_HEIGHT

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

#define STATISTICS_LINE_HEIGHT FONT_SMALL_LINE_HEIGHT
#define STATISTICS_LINES_HEIGHT (STATISTICS_LINE_NUM * STATISTICS_LINE_HEIGHT)
#define STATISTICS_TOPMARGIN_HEIGHT ((CONTENT_HEIGHT - STATISTICS_LINES_HEIGHT) / 2)
#define STATISTICS_BOTTOMMARGIN_HEIGHT (CONTENT_HEIGHT - STATISTICS_TOPMARGIN_HEIGHT - STATISTICS_LINES_HEIGHT)
#define STATISTICS_KEY_LEFT 0
#define STATISTICS_KEY_OFFSETX PADDING_LEFT
#define STATISTICS_VALUE_LEFT STATISTICS_KEY_WIDTH
#define STATISTICS_VALUE_WIDTH (CONTENT_WIDTH - STATISTICS_KEY_WIDTH)
#define STATISTICS_VALUE_OFFSETX (FONT_SMALL_CAP_HEIGHT / 2)

#define GAME_BOARD_WIDTH (8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_HEIGHT (8 * GAME_SQUARE_HEIGHT)
#define GAME_BOARD_RIGHT (GAME_BOARD_LEFT + GAME_BOARD_WIDTH)
#define GAME_BOARD_BOTTOM (GAME_BOARD_TOP + GAME_BOARD_HEIGHT)

#if defined(DISPLAY_LANDSCAPE)
#define GAME_PLAYER_TIMER_LEFT GAME_BOARD_RIGHT
#define GAME_PLAYER_TIMER_TOP (GAME_AI_TIMER_TOP + GAME_AI_TIMER_HEIGHT)
#define GAME_PLAYER_TIMER_WIDTH GAME_AI_TIMER_WIDTH
#define GAME_PLAYER_TIMER_HEIGHT (GAME_BOARD_HEIGHT - GAME_AI_TIMER_HEIGHT)
#define GAME_PLAYER_TIMER_OFFSETX (GAME_PLAYER_TIMER_WIDTH / 2)
#define GAME_PLAYER_TIMER_OFFSETY (GAME_PLAYER_TIMER_HEIGHT - 2 * GAME_SQUARE_HEIGHT + GAME_AI_TIMER_OFFSETY)

#define GAME_AI_TIMER_LEFT GAME_BOARD_RIGHT
#define GAME_AI_TIMER_TOP GAME_BOARD_TOP
#define GAME_AI_TIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_AI_TIMER_HEIGHT (CONTENT_HEIGHT / 2)
#define GAME_AI_TIMER_OFFSETX (GAME_AI_TIMER_WIDTH / 2)
#define GAME_AI_TIMER_OFFSETY ((2 * GAME_SQUARE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#elif defined(DISPLAY_PORTRAIT)
#define GAME_PLAYER_TIMER_LEFT GAME_BOARD_LEFT
#define GAME_PLAYER_TIMER_TOP GAME_BOARD_BOTTOM
#define GAME_PLAYER_TIMER_WIDTH (GAME_BOARD_WIDTH / 2)
#define GAME_PLAYER_TIMER_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_PLAYER_TIMER_OFFSETX (GAME_PLAYER_TIMER_WIDTH / 2)
#define GAME_PLAYER_TIMER_OFFSETY ((GAME_PLAYER_TIMER_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)

#define GAME_AI_TIMER_LEFT (GAME_PLAYER_TIMER_LEFT + GAME_PLAYER_TIMER_WIDTH)
#define GAME_AI_TIMER_TOP GAME_BOARD_BOTTOM
#define GAME_AI_TIMER_WIDTH (GAME_BOARD_WIDTH - GAME_PLAYER_TIMER_WIDTH)
#define GAME_AI_TIMER_HEIGHT GAME_PLAYER_TIMER_HEIGHT
#define GAME_AI_TIMER_OFFSETX (GAME_AI_TIMER_WIDTH / 2)
#define GAME_AI_TIMER_OFFSETY GAME_PLAYER_TIMER_OFFSETY
#endif

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

    COLOR_INSTRUMENT_SECONDARY,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_SECONDARY,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_TERTIARY,
    COLOR_INSTRUMENT_FRAME_SECONDARY,
    COLOR_INSTRUMENT_FRAME_TERTIARY,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALERTZONE1,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_SECONDARY_ALERTZONE1,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_TERTIARY_ALERTZONE1,
    COLOR_INSTRUMENT_FRAME_SECONDARY_ALERTZONE1,
    COLOR_INSTRUMENT_FRAME_TERTIARY_ALERTZONE1,

    COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALERTZONE2,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_SECONDARY_ALERTZONE2,
    COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_TERTIARY_ALERTZONE2,
    COLOR_INSTRUMENT_FRAME_SECONDARY_ALERTZONE2,
    COLOR_INSTRUMENT_FRAME_TERTIARY_ALERTZONE2,

    COLOR_INSTRUMENT_TICK_MARK_TERTIARY,

#if defined(GAME)
    COLOR_SELECTED_ENABLED_BACKGROUND,
    COLOR_GAME_SQUARE_BLACK,
    COLOR_GAME_SQUARE_WHITE,
    COLOR_GAME_PIECE_BLACK,
    COLOR_GAME_PIECE_WHITE,
#endif
} ColorIndex;

#define COLOR_ALERTZONE_OFFSET 5

#if defined(DISPLAY_COLOR)

// Online color blender: https://pinetools.com/blend-colors
// Online RGB565 color picker: https://rgbcolorpicker.com/565

static const mr_color_t displayColors[][3] = {
    // Element active
    {mr_get_color(0x3a393a), // 0x3d3d3d
     mr_get_color(0xc6c6c6),
     mr_get_color(0xbc8c50)},

    // Element neutral
    {mr_get_color(0x525252), // 0x515151
     mr_get_color(0xa5a5a5),
     mr_get_color(0x936d3f)},

    // Container background [global background]
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x1a1a1a),
     mr_get_color(0x000000)},

    // Container global [title bar background]
    {mr_get_color(0xffffff),
     mr_get_color(0x212021), // 0x242424
     mr_get_color(0x0a0a00)},

    // Container global shadow
    {mr_get_color(0xdddddd),
     mr_get_color(0x121212),
     mr_get_color(0x000000)},

    // Amplified enabled [menu item selected background]
    {mr_get_color(0xe1ebf9),
     mr_get_color(0x232e3d),
     mr_get_color(0x08130e)},

    // On flat active [menu item selected foreground]
    {mr_get_color(0x3a3a3a), // 0x3d3d3d
     mr_get_color(0xc6c6c6),
     mr_get_color(0xbc8c50)},

    // Running (green)
    {mr_get_color(0x008300),
     mr_get_color(0x078c07),
     mr_get_color(0x0a6808)},

    // Warning (orange)
    {mr_get_color(0xfe9413),
     mr_get_color(0xfe9b29),
     mr_get_color(0x88490f)},

    // Alarm (red)
    {mr_get_color(0xe30019),
     mr_get_color(0xcb131d),
     mr_get_color(0xb60011)},

    // Instrument regular secondary
    {mr_get_color(0x707070),
     mr_get_color(0xa5a5a5),
     mr_get_color(0x705433)},

    // Instrument enhanced secondary
    {mr_get_color(0x335483),
     mr_get_color(0x85a7d8),
     mr_get_color(0x36624e)},

    // Instrument enhanced tertiary over frame secondary
    {mr_get_color(0xccdef6),
     mr_get_color(0x232e3d),
     mr_get_color(0x08130e)},

    // Instrument enhanced tertiary over frame tertiary
    {mr_get_color(0xa3b1c4),
     mr_get_color(0x06090b),
     mr_get_color(0x201a10)},

    // Instrument frame secondary
    {mr_get_color(0xf0f0f0),
     mr_get_color(0x1a1a1a),
     mr_get_color(0x000000)},

    // Instrument frame tertiary
    {mr_get_color(0xc8c8c8),
     mr_get_color(0x000000),
     mr_get_color(0x271c11)},

    // Instrument enhanced secondary alert zone 1
    {mr_get_color(0x4a72aa),
     mr_get_color(0x5a84bf),
     mr_get_color(0x418065)},

    // Instrument enhanced tertiary over frame secondary alert zone 1
    {mr_get_color(0xc5cdd9),
     mr_get_color(0x313d4a), // 0x2e3947
     mr_get_color(0x11201a)},

    // Instrument enhanced tertiary over frame tertiary alert zone 1
    {mr_get_color(0x9da4ab),
     mr_get_color(0x090b0e),
     mr_get_color(0x221c12)},

    // Instrument frame secondary alert zone 1
    {mr_get_color(0xe8e8e8),
     mr_get_color(0x202020),
     mr_get_color(0x000400)},

    // Instrument frame tertiary alert zone 1
    {mr_get_color(0xc0c0c0),
     mr_get_color(0x000000),
     mr_get_color(0x271c11)},

    // Instrument enhanced secondary alert zone 2
    {mr_get_color(0x6690cb),
     mr_get_color(0x43679a),
     mr_get_color(0x52A482)},

    // Instrument enhanced tertiary over frame secondary alert zone 2
    {mr_get_color(0xb6bdc8),
     mr_get_color(0x394350),
     mr_get_color(0x1c3027)},

    // Instrument enhanced tertiary over frame tertiary alert zone 2
    {mr_get_color(0x8f969c),
     mr_get_color(0x0b0d10),
     mr_get_color(0x242015)},

    // Instrument frame secondary alert zone 2
    {mr_get_color(0xe0e0e0),
     mr_get_color(0x282828),
     mr_get_color(0x000800)},

    // Instrument frame tertiary alert zone 2
    {mr_get_color(0xb8b8b8),
     mr_get_color(0x000000),
     mr_get_color(0x271c11)},

    // Instrument tick mark tertiary
    {mr_get_color(0x8e8e8e),
     mr_get_color(0x666666),
     mr_get_color(0x271c11)},

#if defined(GAME)
    // Selected enabled background [game square selected]
    {mr_get_color(0x335483),
     mr_get_color(0x43679a),
     mr_get_color(0x08130e)},

    // Game square black
    {mr_get_color(0xb89070),
     mr_get_color(0xb89070),
     mr_get_color(0x271C11)},

    // Game square white
    {mr_get_color(0xd0b088),
     mr_get_color(0xd0b088),
     mr_get_color(0x372A1A)},

    // Game piece black
    {mr_get_color(0x000000),
     mr_get_color(0x000000),
     mr_get_color(0x000000)},

    // Game piece white
    {mr_get_color(0xffffff),
     mr_get_color(0xffffff),
     mr_get_color(0x936D3F)},
#endif
};

#endif

void onDisplaySubMenuBack(void);

// Definitions

const mr_rectangle_t contentRectangle = {
    CONTENT_LEFT,
    CONTENT_TOP,
    CONTENT_WIDTH,
    CONTENT_HEIGHT,
};

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
    if ((colorIndex == COLOR_INSTRUMENT_ENHANCED_SECONDARY) ||
        (colorIndex == COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_SECONDARY) ||
        (colorIndex == COLOR_INSTRUMENT_FRAME_TERTIARY) ||
        (colorIndex == COLOR_INSTRUMENT_SECONDARY) ||
        (colorIndex == COLOR_INSTRUMENT_TICK_MARK_TERTIARY) ||
        (colorIndex == COLOR_AMPLIFIED_ENABLED_BACKGROUND) ||
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

static void drawPadding(const mr_rectangle_t *outerMargin,
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
    drawRectangle(&rectangle);

    // Bottom
    rectangle.y = innerMarginBottom;
    rectangle.height = outerMarginBottom - innerMarginBottom;
    drawRectangle(&rectangle);

    // Left
    rectangle.y = innerMargin->y;
    rectangle.width = innerMargin->x - outerMargin->x;
    rectangle.height = innerMargin->height;
    drawRectangle(&rectangle);

    // Right
    rectangle.x = innerMarginRight;
    rectangle.width = outerMarginRight - innerMarginRight;
    drawRectangle(&rectangle);
}

static void drawFrame(const mr_rectangle_t *rectangle)
{
    mr_rectangle_t innerMargin = {
        rectangle->x + 1,
        rectangle->y + 1,
        rectangle->width - 2,
        rectangle->height - 2,
    };
    drawPadding(rectangle,
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

            drawRectangle(&r);
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
    char buffer[128];
    const char *str;
    int16_t width;

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

    default:
        width = 0;

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

void drawCenteredMultilineText(const mr_rectangle_t *rectangle,
                               const char *message)
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);

    const uint8_t *lineStarts[CONTENT_LINE_MAX];
    uint32_t lineLengths[CONTENT_LINE_MAX];
    uint32_t lineNum = 0;

    const uint8_t *messagePosition = (const uint8_t *)message;
    const uint8_t *messageLine = (const uint8_t *)message;

    char buffer[128];

    while (true)
    {
        // Find next word
        const uint8_t *messageWord = messagePosition;

        if (*messagePosition < ' ')
            messagePosition++;
        else
        {
            if (*messagePosition == ' ')
                messagePosition++;
            while (true)
            {
                uint8_t c = *messagePosition;
                if (c <= ' ')
                    break;
                messagePosition++;
            };
        }

        // Word
        if (*messageWord >= ' ')
        {
            uint32_t n = messagePosition - messageLine;
            memcpy(buffer, messageLine, n);
            buffer[n] = '\0';

            if (getTextWidth(buffer) > (DISPLAY_WIDTH - PADDING_LEFT - PADDING_RIGHT))
            {
                lineStarts[lineNum] = messageLine;
                lineLengths[lineNum++] = messageWord - messageLine;

                messageLine = messageWord + (*messageWord == ' ');
            }
        }
        // End of line
        else if ((*messageWord == '\n') ||
                 (*messageWord == '\0'))
        {
            lineStarts[lineNum] = messageLine;
            lineLengths[lineNum++] = messageWord - messageLine;

            messageLine = messagePosition;

            if (*messageWord == '\0')
                break;
        }
    }

    mr_rectangle_t lineRectangle;
    mr_point_t lineOffset;

    uint32_t linesTop = (rectangle->height - lineNum * FONT_SMALL_LINE_HEIGHT) / 2;

    uint32_t y = rectangle->y;

    for (uint32_t i = 0; i < lineNum; i++)
    {
        lineRectangle = (mr_rectangle_t){
            rectangle->x,
            y,
            rectangle->width,
            FONT_SMALL_LINE_HEIGHT,
        };
        lineOffset = (mr_point_t){
            rectangle->width / 2,
            0,
        };
        if (i == 0)
        {
            lineRectangle.height += linesTop;
            lineOffset.y += linesTop;
            y += linesTop;
        }
        else if (i == (lineNum - 1))
            lineRectangle.height = rectangle->y + rectangle->height - y;

        uint32_t lineLength = lineLengths[i];

        memcpy(buffer, lineStarts[i], lineLength);
        buffer[lineLength] = '\0';
        if (lineLength == 0)
            setStrokeColor(COLOR_ELEMENT_NEUTRAL);
        drawCenteredText(buffer,
                         &lineRectangle,
                         &lineOffset);

        y += FONT_SMALL_LINE_HEIGHT;
    }
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

    if (isBatteryCharging())
    {
        colorIndex = COLOR_RUNNING;
        level = chargingIcons[level];
    }
    else
    {
        if (isUSBPowered())
            colorIndex = COLOR_RUNNING;
        else if (level == 0)
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

void drawTitleBar(const char *title)
{
    char buffer[8];
    mr_rectangle_t rectangle = {
        TITLEBAR_WIDTH,
        TITLEBAR_TOP,
        0,
        TITLEBAR_CONTENT_HEIGHT,
    };
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
    if (isInLockMode())
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

    setFillColor(COLOR_CONTAINER_GLOBAL_SHADOW);
    drawRectangle(&rectangle);
#endif

    // Set background
    setFillColor(COLOR_CONTAINER_BACKGROUND);
}

void drawSplash(const char *message)
{
    setFillColor(COLOR_CONTAINER_GLOBAL);

    const mr_rectangle_t displayRectangle = {
        0,
        0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT,
    };

    drawCenteredMultilineText(&displayRectangle, message);
}

void drawNotification(const char *title,
                      const char *message)
{
    drawTitleBar(title);

    drawCenteredMultilineText(&contentRectangle, message);
}

void drawMenu(Menu *menu)
{
    drawTitleBar(menu->title);

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
                         ? COLOR_AMPLIFIED_ENABLED_BACKGROUND
                         : COLOR_CONTAINER_GLOBAL);

        // Submenu
        if (menuStyle & MENUSTYLE_SUBMENU)
        {
            setStrokeColor(COLOR_ELEMENT_NEUTRAL);
            drawRowRight("=", &rectangle);
        }

        // Checked
        if (menuStyle & MENUSTYLE_CHECKED)
        {
            setStrokeColor(COLOR_INSTRUMENT_ENHANCED_SECONDARY);
            drawRowRight(">", &rectangle);
        }

        // Text
#if defined(DISPLAY_MONOCHROME)
        setFont(font_medium);
#elif defined(DISPLAY_COLOR)
        setFont(font_small);
#endif
        setStrokeColor(COLOR_ON_FLAT_ACTIVE);
        drawRowLeft(menuItem, &rectangle);

        rectangle.y += MENU_LINE_HEIGHT;
    }

    // Bottom space
    rectangle.x = CONTENT_LEFT;
    rectangle.width = CONTENT_WIDTH;
    rectangle.height = CONTENT_BOTTOM - rectangle.y;
    setFillColor(COLOR_CONTAINER_GLOBAL);
    drawRectangle(&rectangle);
}

void drawTestMode(const char lines[8][32])
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_CONTAINER_GLOBAL);

    mr_rectangle_t rectangle;
    const mr_point_t offset = {
        0,
        0,
    };
    for (uint32_t y = 0; y < 4; y++)
        for (uint32_t x = 0; x < 2; x++)
        {
            rectangle = (mr_rectangle_t){
                (DISPLAY_WIDTH / 2) * x,
                FONT_SMALL_LINE_HEIGHT * y,
                (DISPLAY_WIDTH / 2),
                FONT_SMALL_LINE_HEIGHT,
            };
            drawText(lines[y * 2 + x],
                     &rectangle,
                     &offset);
        }

    rectangle = (mr_rectangle_t){
        0,
        (4 * FONT_SMALL_LINE_HEIGHT),
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT - (4 * FONT_SMALL_LINE_HEIGHT),
    };
    drawRectangle(&rectangle);
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
                       : COLOR_INSTRUMENT_ENHANCED_SECONDARY);
    drawText(valueString,
             &partialRectangle,
             offset);

    partialRectangle.x += partialRectangle.width;
    partialRectangle.width = rectangle->width - partialRectangle.width;
    mr_point_t partialOffset = {
        0,
        offset->y,
    };
    setStrokeColor(COLOR_INSTRUMENT_SECONDARY);
    drawText(unitString,
             &partialRectangle,
             &partialOffset);
}

static const ColorIndex measurementStyleColorIndex[] = {
    COLOR_INSTRUMENT_ENHANCED_SECONDARY,
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
        MEASUREMENT_VALUE_HEIGHT,
    };
    const mr_point_t valueOffset = {
        MEASUREMENT_VALUE_OFFSETX,
        MEASUREMENT_VALUE_OFFSETY,
    };
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
        MEASUREMENT_UNIT_HEIGHT,
    };
    const mr_point_t unitOffset = {
        MEASUREMENT_UNIT_OFFSETX,
        MEASUREMENT_UNIT_OFFSETY,
    };
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
        MEASUREMENT_CONFIDENCE_HEIGHT,
    };
    mr_point_t confidenceOffset = {
        MEASUREMENT_CONFIDENCE_OFFSETX -
            getTextWidth(confidenceString) -
            getTextWidth(confidenceUnit),
        MEASUREMENT_CONFIDENCE_OFFSETY,
    };
    drawValueAndUnit(confidenceString,
                     confidenceUnit,
                     &confidenceRectangle,
                     &confidenceOffset,
                     style == MEASUREMENTSTYLE_DONE);
}

void drawMeasurementAlert(const char *alertString)
{
    setFont(font_small);

    // Info key
    const mr_rectangle_t infoKeyRectangle = {
        MEASUREMENT_ALERT_KEY_LEFT,
        MEASUREMENT_ALERT_KEY_TOP,
        MEASUREMENT_ALERT_KEY_WIDTH,
        MEASUREMENT_ALERT_KEY_HEIGHT,
    };
    const mr_point_t infoKeyOffset = {
        MEASUREMENT_ALERT_KEY_OFFSETX,
        MEASUREMENT_ALERT_KEY_OFFSETY,
    };
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(alertString,
             &infoKeyRectangle,
             &infoKeyOffset);
}

void drawMeasurementInfo(const char *keyString,
                         const char *valueString,
                         const char *unitString,
                         MeasurementStyle style)
{
    setFont(font_small);

    // Info key
    const mr_rectangle_t keyRectangle = {
        MEASUREMENT_INFO_KEY_LEFT,
        MEASUREMENT_INFO_KEY_TOP,
        MEASUREMENT_INFO_KEY_WIDTH,
        MEASUREMENT_INFO_KEY_HEIGHT,
    };
    const mr_point_t keyOffset = {
        MEASUREMENT_INFO_KEY_OFFSETX,
        MEASUREMENT_INFO_KEY_OFFSETY,
    };
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(keyString,
             &keyRectangle,
             &keyOffset);

    // Info value
    const mr_rectangle_t valueRectangle = {
        MEASUREMENT_INFO_VALUE_LEFT,
        MEASUREMENT_INFO_VALUE_TOP,
        MEASUREMENT_INFO_VALUE_WIDTH,
        MEASUREMENT_INFO_VALUE_HEIGHT,
    };
    const mr_point_t valueOffset = {
        MEASUREMENT_INFO_VALUE_OFFSETX,
        MEASUREMENT_INFO_VALUE_OFFSETY,
    };
    drawValueAndUnit(valueString,
                     unitString,
                     &valueRectangle,
                     &valueOffset,
                     style == MEASUREMENTSTYLE_DONE);
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
            xTickNext = MEASUREMENT_BAR_WIDTH;

        if (wasLargeTick)
        {
            char buffer[8];
            strclr(buffer);
            strcatDecimalPowerWithMetricPrefix(buffer, exponentIndex - 1, -3);

            uint32_t width = (i < (2 * (MEASUREMENT_BAR_DECADES - 1)))
                                 ? xTickNext - xLabel
                                 : MEASUREMENT_BAR_WIDTH - xLabel;
            rectangle = (mr_rectangle_t){
                xLabel,
                MEASUREMENT_BAR_LABEL_TOP,
                width,
                MEASUREMENT_BAR_LABEL_HEIGHT,
            };
            mr_point_t offset = {
                xLabelCenter - xLabel,
                MEASUREMENT_BAR_LABEL_OFFSETY,
            };
            setStrokeColor(COLOR_INSTRUMENT_SECONDARY);
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
                         ? COLOR_INSTRUMENT_TICK_MARK_TERTIARY
                         : COLOR_CONTAINER_BACKGROUND;
        rectangle = (mr_rectangle_t){
            xTick,
            MEASUREMENT_BAR_TICKS_TOP,
            tickWidth,
            MEASUREMENT_BAR_TICKS_UPPER_HEIGHT,
        };
        setFillColor(colorIndex);
        drawRectangle(&rectangle);

        colorIndex = isTick
                         ? COLOR_INSTRUMENT_TICK_MARK_TERTIARY
                         : COLOR_CONTAINER_BACKGROUND;

        rectangle.y += MEASUREMENT_BAR_TICKS_UPPER_HEIGHT;
        rectangle.height = MEASUREMENT_BAR_TICKS_LOWER_HEIGHT;
        setFillColor(colorIndex);
        drawRectangle(&rectangle);

        xTick = xTickNext;
    }

    // Outer frame
    const mr_rectangle_t outerFrameRectangle = {
        MEASUREMENT_BAR_FRAME_LEFT,
        MEASUREMENT_BAR_FRAME_TOP,
        MEASUREMENT_BAR_FRAME_WIDTH,
        MEASUREMENT_BAR_FRAME_HEIGHT,
    };
    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);
    drawFrame(&outerFrameRectangle);

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

            alertZoneColor = COLOR_ALERTZONE_OFFSET;
        }
        else
            alertZoneColor = 2 * COLOR_ALERTZONE_OFFSET;
#endif

        // Draw
        rectangle = (mr_rectangle_t){
            MEASUREMENT_BAR_INSTRUMENT_LEFT + xBar,
            MEASUREMENT_BAR_INSTRUMENT_TOP,
            xBarNext - xBar,
            MEASUREMENT_BAR_INSTRUMENT_HEIGHT,
        };
        setFillColor(alertZoneColor + ((xBar < instrumentValue)
                                           ? COLOR_INSTRUMENT_ENHANCED_SECONDARY
                                           : COLOR_INSTRUMENT_FRAME_SECONDARY));
        drawRectangle(&rectangle);

        xBar = xBarNext;
    }

    // Padding
    const mr_rectangle_t outerPadding = {
        MEASUREMENT_BAR_OUTERPADDING_LEFT,
        MEASUREMENT_BAR_OUTERPADDING_TOP,
        MEASUREMENT_BAR_OUTERPADDING_WIDTH,
        MEASUREMENT_BAR_OUTERPADDING_HEIGHT,
    };
    const mr_rectangle_t innerPadding = {
        MEASUREMENT_BAR_INNERPADDING_LEFT,
        MEASUREMENT_BAR_INNERPADDING_TOP,
        MEASUREMENT_BAR_INNERPADDING_WIDTH,
        MEASUREMENT_BAR_INNERPADDING_HEIGHT,
    };
    setFillColor(COLOR_CONTAINER_BACKGROUND);
    drawPadding(&outerPadding,
                &innerPadding);
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
                 uint32_t xTickNum,
                 const char *periodLabelString,
                 const uint8_t *bins,
                 uint32_t alertZone1,
                 uint32_t alertZone2)
{
    if (!alertZone1)
        alertZone1 = 2 * UINT8_MAX;
    if (!alertZone2)
        alertZone2 = 2 * UINT8_MAX;
    if (alertZone1 > alertZone2)
        alertZone1 = alertZone2;

    // Get min/max
    uint32_t dataMax = 0;
    uint32_t dataMin = UCHAR_MAX;
    for (uint32_t i = 0; i < HISTORY_BIN_NUM; i++)
    {
        uint32_t value = bins[i];

        if (value)
        {
            if (value > dataMax)
                dataMax = value;

            if (value < dataMin)
                dataMin = value;
        }
    }

    // Labels
    char maxLabelString[16];
    char minLabelString[16];
    int32_t dataExponentMinValue = 0;
    uint32_t dataTickNum = 1;

    strclr(maxLabelString);
    strclr(minLabelString);

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

        strcat(maxLabelString, unitString);
        strcat(maxLabelString, ": ");
        strcatDecimalPowerWithMetricPrefix(maxLabelString, exponentMax, -2);
        strcatDecimalPowerWithMetricPrefix(minLabelString, exponentMin, -2);
    }

    uint32_t dataRange = HISTORY_DECADE * dataTickNum;

    setFont(font_small);
    setStrokeColor(COLOR_INSTRUMENT_SECONDARY);

    const mr_rectangle_t periodRectangle = {
        HISTORY_PERIOD_LABEL_LEFT,
        HISTORY_PERIOD_LABEL_TOP,
        HISTORY_PERIOD_LABEL_WIDTH,
        HISTORY_PERIOD_LABEL_HEIGHT,
    };
    const mr_point_t periodOffset = {
        HISTORY_PERIOD_LABEL_OFFSET_LEFT,
        HISTORY_PERIOD_LABEL_OFFSET_TOP,
    };
    drawText(periodLabelString,
             &periodRectangle,
             &periodOffset);

    const mr_rectangle_t maxLabelRectangle = {
        HISTORY_MAX_LABEL_LEFT,
        HISTORY_MAX_LABEL_TOP,
        HISTORY_MAX_LABEL_WIDTH,
        HISTORY_MAX_LABEL_HEIGHT,
    };
    const mr_point_t maxLabelOffset = {
        HISTORY_MAX_LABEL_OFFSET_LEFT,
        HISTORY_MAX_LABEL_OFFSET_TOP,
    };
    drawRightAlignedText(maxLabelString,
                         &maxLabelRectangle,
                         &maxLabelOffset);

    const mr_rectangle_t minLabelRectangle = {
        HISTORY_MIN_LABEL_LEFT,
        HISTORY_MIN_LABEL_TOP,
        HISTORY_MIN_LABEL_WIDTH,
        HISTORY_MIN_LABEL_HEIGHT,
    };
    const mr_point_t minLabelOffset = {
        HISTORY_MIN_LABEL_OFFSET_LEFT,
        HISTORY_MIN_LABEL_OFFSET_TOP,
    };
    drawRightAlignedText(minLabelString,
                         &minLabelRectangle,
                         &minLabelOffset);

    // Space and frame
    const mr_rectangle_t leftSpaceRectangle = {
        HISTORY_PADDING_LEFT_LEFT,
        HISTORY_PADDING_LEFT_TOP,
        HISTORY_PADDING_LEFT_WIDTH,
        HISTORY_PADDING_LEFT_HEIGHT,
    };
    drawRectangle(&leftSpaceRectangle);

    const mr_rectangle_t rightSpaceRectangle = {
        HISTORY_PADDING_RIGHT_LEFT,
        HISTORY_PADDING_RIGHT_TOP,
        HISTORY_PADDING_RIGHT_WIDTH,
        HISTORY_PADDING_RIGHT_HEIGHT,
    };
    drawRectangle(&rightSpaceRectangle);

    const mr_rectangle_t frameRectangle = {
        HISTORY_LEFT,
        HISTORY_TOP,
        HISTORY_WIDTH,
        HISTORY_HEIGHT,
    };
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
    int16_t timeTickNext = HISTORY_DATA_WIDTH / xTickNum;

    int16_t yDataLast = 0;
#if defined(DISPLAY_COLOR)
    int16_t yAlertZone1 = getHistoryY(alertZone1,
                                      dataExponentMinValue,
                                      dataScale);
    int16_t yAlertZone2 = getHistoryY(alertZone2,
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
            timeTickNext = timeTickIndex * HISTORY_DATA_WIDTH / xTickNum;
            timeTickDraw = true;
        }

        // Value tick
        valueTickTableIndex = 0;

        // Data
        int16_t yData = getHistoryY(bins[x],
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
            ColorIndex colorIndex = COLOR_INSTRUMENT_FRAME_SECONDARY;
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

                if (colorIndex == COLOR_INSTRUMENT_FRAME_SECONDARY)
                    colorIndex = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_SECONDARY;
                else
                    colorIndex = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_FRAME_TERTIARY;
            }
            else if (y < yDataTop)
            {
                yDataNext = yDataTop;

                colorIndex = COLOR_INSTRUMENT_ENHANCED_SECONDARY;
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

                colorIndex += COLOR_ALERTZONE_OFFSET;
            }
            else
            {
                yAlertZoneNext = HISTORY_DATA_HEIGHT;

                colorIndex += 2 * COLOR_ALERTZONE_OFFSET;
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
            HISTORY_DATA_HEIGHT,
        };
        drawImage(&historyImageRectangle,
                  imageBuffer);
    }
}

void drawRNG(const char *title,
             bool isLarge,
             const char *rngString,
             const char *infoString)
{
    drawTitleBar(title);

    setStrokeColor(COLOR_INSTRUMENT_ENHANCED_SECONDARY);

    if (!isLarge)
    {
        // RNG string
        const mr_rectangle_t rngStringRectangle = {
            RNG_STRING_LEFT,
            RNG_STRING_TOP,
            RNG_STRING_WIDTH,
            RNG_STRING_HEIGHT,
        };
        const mr_point_t rngStringOffset = {
            RNG_STRING_OFFSETX,
            RNG_STRING_OFFSETY,
        };
        setFont(font_small);
        drawText(rngString,
                 &rngStringRectangle,
                 &rngStringOffset);

        // State
        const mr_rectangle_t rngStateRectangle = {
            RNG_STATE_LEFT,
            RNG_STATE_TOP,
            RNG_STATE_WIDTH,
            RNG_STATE_HEIGHT,
        };
        const mr_point_t rngStateOffset = {
            RNG_STATE_OFFSETX,
            RNG_STATE_OFFSETY,
        };
        setFont(font_medium);
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawCenteredText(infoString,
                         &rngStateRectangle,
                         &rngStateOffset);
    }
    else
    {
        // State
        const mr_point_t rngNumberOffset = {
            (CONTENT_WIDTH / 2),
            ((CONTENT_HEIGHT - FONT_LARGE_LINE_HEIGHT) / 2),
        };
        setFont(font_large);
        drawCenteredText(rngString,
                         &contentRectangle,
                         &rngNumberOffset);
    }
}

static cstring statisticsKeyStrings[] = {
#if defined(DISPLAY_LANDSCAPE)
    STRING_TUBE_LIFE,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_ID,
    STRING_EMPTY,
    STRING_BATTERY,
#elif defined(DISPLAY_PORTRAIT)
    STRING_TUBE,
    STRING_LIFE,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_EMPTY,
    STRING_DEVICE,
    STRING_ID,
    STRING_EMPTY,
    STRING_BATTERY,
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
#if defined(DISPLAY_LANDSCAPE)
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
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

#define STATISTICS_LINE_NUM sizeof(statisticsType)

void drawStatistics(void)
{
    drawTitleBar(getString(STRING_STATISTICS));

    setFont(font_small);

    char keyString[64];
    char valueString[32];
    char unitString[16];

    mr_rectangle_t rectangle;
    mr_point_t offset;

    uint32_t y = CONTENT_TOP;

    for (uint32_t i = 0; i < STATISTICS_LINE_NUM; i++)
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
            STATISTICS_LINE_HEIGHT,
        };
        if (statisticsType[i] == STATISTICS_TITLE)
            rectangle.width = CONTENT_WIDTH;

        offset = (mr_point_t){
            STATISTICS_KEY_OFFSETX,
            0,
        };
        if (i == 0)
        {
            rectangle.height += STATISTICS_TOPMARGIN_HEIGHT;
            offset.y += STATISTICS_TOPMARGIN_HEIGHT;
            y += STATISTICS_TOPMARGIN_HEIGHT;
        }
        else if (i == (STATISTICS_LINE_NUM - 1))
            rectangle.height += STATISTICS_BOTTOMMARGIN_HEIGHT;

        setStrokeColor(
            (!valueString[0] && !unitString[0])
                ? COLOR_ELEMENT_ACTIVE
                : COLOR_ELEMENT_NEUTRAL);
        drawText(keyString,
                 &rectangle,
                 &offset);

        // Value
        if (statisticsType[i] != STATISTICS_TITLE)
        {
            rectangle.x = STATISTICS_VALUE_LEFT;
            rectangle.width = STATISTICS_VALUE_WIDTH;
            offset.x = STATISTICS_VALUE_OFFSETX;
            drawValueAndUnit(valueString,
                             unitString,
                             &rectangle,
                             &offset,
                             false);
        }

        y += FONT_SMALL_LINE_HEIGHT;
    }
}

#if defined(GAME)

void drawGame(const uint8_t board[8][8],
              const char time[2][16])
{
    mr_rectangle_t rectangle;

#if defined(DISPLAY_COLOR) && defined(DISPLAY_240X320)
    drawTitleBar(getString(STRING_GAME));
#endif

    // Board
    static const char *const gamePieceMap = "@AACFBDE";

    setFont(font_symbols);

    rectangle.width = GAME_SQUARE_WIDTH;
    rectangle.height = GAME_SQUARE_HEIGHT;

    const mr_point_t gamePieceOffset = {
        (GAME_SQUARE_WIDTH / 2),
        (GAME_SQUARE_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2,
    };

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

            drawCenteredText(pieceString,
                             &rectangle,
                             &gamePieceOffset);
        }
    }

    // Timers common
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);

    // Player timer
    const mr_rectangle_t playerTimerRectangle = {
        GAME_PLAYER_TIMER_LEFT,
        GAME_PLAYER_TIMER_TOP,
        GAME_PLAYER_TIMER_WIDTH,
        GAME_PLAYER_TIMER_HEIGHT,
    };
    const mr_point_t playerTimerOffset = {
        GAME_PLAYER_TIMER_OFFSETX,
        GAME_PLAYER_TIMER_OFFSETY,
    };
    drawCenteredText(time[0],
                     &playerTimerRectangle,
                     &playerTimerOffset);

    // AI timer
    const mr_rectangle_t aiTimerRectangle = {
        GAME_AI_TIMER_LEFT,
        GAME_AI_TIMER_TOP,
        GAME_AI_TIMER_WIDTH,
        GAME_AI_TIMER_HEIGHT,
    };
    const mr_point_t aiTimerOffset = {
        GAME_AI_TIMER_OFFSETX,
        GAME_AI_TIMER_OFFSETY,
    };
    drawCenteredText(time[1],
                     &aiTimerRectangle,
                     &aiTimerOffset);
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
