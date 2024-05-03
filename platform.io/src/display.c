/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include <mcu-renderer.h>

#include "adc.h"
#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

// Fonts

#if defined(DISPLAY_128X64)

#include "fonts/font_tiny5_1.h"
#define FONT_SMALL font_tiny5_1
#define FONT_SMALL_LINE_HEIGHT FONT_TINY5_1_LINE_HEIGHT

#include "fonts/font_roboto_matrixR8_1.h"
#define FONT_MEDIUM font_roboto_matrixR8_1
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTO_MATRIXR8_1_LINE_HEIGHT

#include "fonts/font_roboto_matrixM33_1_digits.h"
#define FONT_LARGE font_roboto_matrixM33_1_digits
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTO_MATRIXM33_1_DIGITS_LINE_HEIGHT

#include "fonts/font_symbols6_1.h"
#define FONT_SYMBOLS font_symbols6_1
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS6_1_LINE_HEIGHT

#include "fonts/font_chess8_1.h"
#define FONT_GAME font_chess8_1

#elif defined(DISPLAY_320X240) || defined(DISPLAY_240X320)

#if defined(DISPLAY_FONT_2BPP)

#include "fonts/font_robotoM12_2.h"
#define FONT_SMALL font_robotoM12_2
#define FONT_SMALL_LINE_HEIGHT FONT_ROBOTOM12_2_LINE_HEIGHT

#include "fonts/font_robotoM18_2.h"
#define FONT_MEDIUM font_robotoM18_2
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTOM18_2_LINE_HEIGHT

#if defined(DISPLAY_320X240)
#include "fonts/font_robotoM86_2_digits.h"
#define FONT_LARGE font_robotoM86_2_digits
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOM86_2_DIGITS_LINE_HEIGHT
#else
#include "fonts/font_robotoM63_2_digits.h"
#define FONT_LARGE font_robotoM63_2_digits
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOM63_2_DIGITS_LINE_HEIGHT
#endif

#include "fonts/font_symbols30_2.h"
#define FONT_SYMBOLS font_symbols30_2
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS30_2_LINE_HEIGHT

#include "fonts/font_chess25_2.h"
#define FONT_GAME font_chess25_2

#else

#include "fonts/font_robotoM12_4.h"
#define FONT_SMALL font_robotoM12_4
#define FONT_SMALL_LINE_HEIGHT FONT_ROBOTOM12_4_LINE_HEIGHT

#include "fonts/font_robotoM18_4.h"
#define FONT_MEDIUM font_robotoM18_4
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTOM18_4_LINE_HEIGHT

#if defined(DISPLAY_320X240)
#include "fonts/font_robotoM86_4_digits.h"
#define FONT_LARGE font_robotoM86_4_digits
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOM86_4_DIGITS_LINE_HEIGHT
#else
#include "fonts/font_robotoM63_4_digits.h"
#define FONT_LARGE font_robotoM63_4_digits
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOM63_4_DIGITS_LINE_HEIGHT
#endif

#include "fonts/font_symbols30_4.h"
#define FONT_SYMBOLS font_symbols30_4
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS30_4_LINE_HEIGHT

#include "fonts/font_chess25_4.h"
#define FONT_GAME font_chess25_4

#endif

#endif

// Display metrics

#if defined(DISPLAY_128X64)

#define DISPLAY_LANDSCAPE

#define CONTENT_PADDING 3

#define TITLEBAR_HEIGHT 8
#define TITLEBAR_CLOCK_WIDTH 30
#define TITLEBAR_BATTERY_WIDTH 15
#define TITLEBAR_BATTERY_OFFSET_Y 1

#define MENU_LINE_HEIGHT 14
#define MENU_SUBMENU_WIDTH 5
#define MENU_CHECKED_WIDTH 8

#define MEASUREMENT_PADDING 7
#define MEASUREMENT_VALUE_HEIGHT 35
#define MEASUREMENT_VALUE_OFFSET_X 5
#define MEASUREMENT_VALUE_OFFSET_Y 2
#define MEASUREMENT_UNIT_X 78
#define MEASUREMENT_CONFIDENCE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_UNIT_BOTTOM)
#define MEASUREMENT_INFO_KEY_Y MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_INFO_KEY_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_INFO_KEY_OFFSET_Y (FONT_MEDIUM_LINE_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_INFO_VALUE_HEIGHT MEASUREMENT_CONFIDENCE_HEIGHT
#define MEASUREMENT_STATE_X 32
#define MEASUREMENT_BAR_LABEL_Y MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_BAR_LABEL_HEIGHT (FONT_SMALL_LINE_HEIGHT + 2)
#define MEASUREMENT_BAR_TICKS_HEIGHT 2
#define MEASUREMENT_BAR_TICKS_UPPER_HEIGHT 1
#define MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT 0
#define MEASUREMENT_BAR_FRAME_HEIGHT 9
#define MEASUREMENT_BAR_FRAME_RIGHT MEASUREMENT_BAR_RIGHT

#define HISTORY_WIDTH 122
#define HISTORY_HEIGHT 42
#define HISTORY_TOP_LABEL_OFFSET_X (HISTORY_WIDTH - 1)
#define HISTORY_TOP_LABEL_OFFSET_Y (HISTORY_Y - CONTENT_Y - FONT_SMALL_LINE_HEIGHT + 1)
#define HISTORY_BOTTOM_LABEL_OFFSET_X (HISTORY_WIDTH - 1)
#define HISTORY_BOTTOM_LABEL_OFFSET_Y 0

#define GAME_BOARD_X 0
#define GAME_BOARD_Y 0
#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8
#define GAME_HISTORY_FIRST_WIDTH 32
#define GAME_HISTORY_FIRST_OFFSET_X 8

#elif defined(DISPLAY_320X240)

#define DISPLAY_LANDSCAPE

#define CONTENT_PADDING 12

#define TITLEBAR_HEIGHT 40
#define TITLEBAR_CLOCK_WIDTH 73
#define TITLEBAR_BATTERY_WIDTH 41
#define TITLEBAR_BATTERY_OFFSET_Y ((TITLEBAR_HEIGHT - 30) / 2)

#define MENU_LINE_HEIGHT 50
#define MENU_SUBMENU_WIDTH 12
#define MENU_CHECKED_WIDTH 21

#define MEASUREMENT_PADDING 15
#define MEASUREMENT_VALUE_HEIGHT 130
#define MEASUREMENT_VALUE_OFFSET_X 9
#define MEASUREMENT_VALUE_OFFSET_Y -1
#define MEASUREMENT_UNIT_X 206
#define MEASUREMENT_CONFIDENCE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_UNIT_BOTTOM)
#define MEASUREMENT_INFO_KEY_Y MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_INFO_KEY_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_INFO_KEY_OFFSET_Y (MEASUREMENT_INFO_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_INFO_VALUE_HEIGHT MEASUREMENT_CONFIDENCE_HEIGHT
#define MEASUREMENT_STATE_X 80
#define MEASUREMENT_BAR_LABEL_Y MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_BAR_LABEL_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_BAR_TICKS_HEIGHT 8
#define MEASUREMENT_BAR_TICKS_UPPER_HEIGHT 4
#define MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT 4
#define MEASUREMENT_BAR_FRAME_HEIGHT 20
#define MEASUREMENT_BAR_FRAME_RIGHT MEASUREMENT_BAR_RIGHT

#define HISTORY_WIDTH 302
#define HISTORY_HEIGHT 130
#define HISTORY_TOP_LABEL_OFFSET_X (HISTORY_WIDTH - 5)
#define HISTORY_TOP_LABEL_OFFSET_Y (HISTORY_Y - CONTENT_Y - FONT_SMALL_LINE_HEIGHT - 2)
#define HISTORY_BOTTOM_LABEL_OFFSET_X (HISTORY_WIDTH - 5)
#define HISTORY_BOTTOM_LABEL_OFFSET_Y 2

#define GAME_BOARD_X 0
#define GAME_BOARD_Y TITLEBAR_BOTTOM
#define GAME_SQUARE_WIDTH 25
#define GAME_SQUARE_HEIGHT 25
#define GAME_HISTORY_FIRST_WIDTH 66
#define GAME_HISTORY_FIRST_OFFSET_X 12

#elif defined(DISPLAY_240X320)

#define DISPLAY_PORTRAIT

#define CONTENT_PADDING 12

#define TITLEBAR_HEIGHT 40
#define TITLEBAR_CLOCK_WIDTH 73
#define TITLEBAR_BATTERY_WIDTH 41
#define TITLEBAR_BATTERY_OFFSET_Y ((TITLEBAR_HEIGHT - 30) / 2)

#define MENU_LINE_HEIGHT 46
#define MENU_SUBMENU_WIDTH 12
#define MENU_CHECKED_WIDTH 21

#define MEASUREMENT_PADDING 15
#define MEASUREMENT_VALUE_HEIGHT 117
#define MEASUREMENT_VALUE_OFFSET_X 10
#define MEASUREMENT_VALUE_OFFSET_Y 24
#define MEASUREMENT_UNIT_X CONTENT_X
#define MEASUREMENT_CONFIDENCE_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_INFO_KEY_Y MEASUREMENT_CONFIDENCE_BOTTOM
#define MEASUREMENT_INFO_KEY_HEIGHT 55
#define MEASUREMENT_INFO_KEY_OFFSET_Y (MEASUREMENT_INFO_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_INFO_VALUE_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_INFO_VALUE_Y)
#define MEASUREMENT_STATE_X (CONTENT_X + CONTENT_WIDTH / 2)
#define MEASUREMENT_BAR_LABEL_Y MEASUREMENT_INFO_KEY_Y
#define MEASUREMENT_BAR_LABEL_HEIGHT (MEASUREMENT_INFO_KEY_HEIGHT - FONT_SMALL_LINE_HEIGHT / 2)
#define MEASUREMENT_BAR_TICKS_HEIGHT 8
#define MEASUREMENT_BAR_TICKS_UPPER_HEIGHT 4
#define MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT 4
#define MEASUREMENT_BAR_FRAME_HEIGHT 20
#define MEASUREMENT_BAR_FRAME_RIGHT (MEASUREMENT_BAR_RIGHT - MEASUREMENT_PADDING)

#define HISTORY_WIDTH 202
#define HISTORY_HEIGHT 130
#define HISTORY_TOP_LABEL_OFFSET_X (HISTORY_WIDTH - 5)
#define HISTORY_TOP_LABEL_OFFSET_Y (HISTORY_Y - CONTENT_Y - FONT_SMALL_LINE_HEIGHT - 2)
#define HISTORY_BOTTOM_LABEL_OFFSET_X (HISTORY_WIDTH - 5)
#define HISTORY_BOTTOM_LABEL_OFFSET_Y 2

#define GAME_BOARD_X ((CONTENT_WIDTH - GAME_BOARD_WIDTH) / 2)
#define GAME_BOARD_Y (TITLEBAR_BOTTOM + FONT_SMALL_LINE_HEIGHT)
#define GAME_SQUARE_WIDTH 25
#define GAME_SQUARE_HEIGHT 25
#define GAME_HISTORY_FIRST_WIDTH 68
#define GAME_HISTORY_FIRST_OFFSET_X 10

#endif

// Common metrics

#define TITLEBAR_X 0
#define TITLEBAR_Y 0
#define TITLEBAR_WIDTH DISPLAY_WIDTH
#define TITLEBAR_BOTTOM (TITLEBAR_Y + TITLEBAR_HEIGHT)
#define TITLEBAR_TEXT_OFFSET_Y ((TITLEBAR_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#define TITLEBAR_TITLE_X 0
#define TITLEBAR_TITLE_OFFSET_X CONTENT_PADDING
#define TITLEBAR_TITLE_WIDTH (TITLEBAR_WIDTH - TITLEBAR_CLOCK_WIDTH - TITLEBAR_BATTERY_WIDTH)
#define TITLEBAR_CLOCK_X (TITLEBAR_TITLE_X + TITLEBAR_TITLE_WIDTH)
#define TITLEBAR_BATTERY_X (TITLEBAR_CLOCK_X + TITLEBAR_CLOCK_WIDTH)

#define CONTENT_X 0
#define CONTENT_Y TITLEBAR_BOTTOM
#define CONTENT_WIDTH DISPLAY_WIDTH
#define CONTENT_HEIGHT (DISPLAY_HEIGHT - TITLEBAR_BOTTOM)
#define CONTENT_RIGHT (CONTENT_X + CONTENT_WIDTH)
#define CONTENT_BOTTOM (CONTENT_Y + CONTENT_HEIGHT)

#define MENU_OFFSET_X CONTENT_PADDING
#define MENU_LINE_NUM (CONTENT_HEIGHT / MENU_LINE_HEIGHT)

#define MEASUREMENT_VALUE_WIDTH CONTENT_WIDTH
#define MEASUREMENT_VALUE_X CONTENT_X
#define MEASUREMENT_VALUE_Y CONTENT_Y
#define MEASUREMENT_VALUE_BOTTOM (MEASUREMENT_VALUE_Y + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_UNIT_Y MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_UNIT_WIDTH (CONTENT_WIDTH - MEASUREMENT_UNIT_X)
#define MEASUREMENT_UNIT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_UNIT_BOTTOM (MEASUREMENT_UNIT_Y + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_UNIT_OFFSET_X (MEASUREMENT_UNIT_WIDTH - MEASUREMENT_PADDING)
#define MEASUREMENT_UNIT_OFFSET_Y 0
#define MEASUREMENT_CONFIDENCE_X MEASUREMENT_UNIT_X
#define MEASUREMENT_CONFIDENCE_Y MEASUREMENT_UNIT_BOTTOM
#define MEASUREMENT_CONFIDENCE_WIDTH MEASUREMENT_UNIT_WIDTH
#define MEASUREMENT_CONFIDENCE_OFFSET_X (MEASUREMENT_CONFIDENCE_WIDTH - MEASUREMENT_PADDING)
#define MEASUREMENT_CONFIDENCE_OFFSET_Y 0
#define MEASUREMENT_CONFIDENCE_BOTTOM (MEASUREMENT_CONFIDENCE_Y + MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_INFO_X CONTENT_X
#define MEASUREMENT_INFO_WIDTH (MEASUREMENT_STATE_RIGHT - CONTENT_X)
#define MEASUREMENT_INFO_KEY_OFFSET_X MEASUREMENT_PADDING
#define MEASUREMENT_INFO_VALUE_Y (MEASUREMENT_INFO_KEY_Y + MEASUREMENT_INFO_KEY_HEIGHT)
#define MEASUREMENT_INFO_VALUE_WIDTH (MEASUREMENT_STATE_RIGHT - MEASUREMENT_INFO_X)
#define MEASUREMENT_INFO_VALUE_OFFSET_X MEASUREMENT_PADDING
#define MEASUREMENT_INFO_VALUE_OFFSET_Y 0
#define MEASUREMENT_STATE_Y MEASUREMENT_INFO_KEY_Y
#define MEASUREMENT_STATE_HEIGHT MEASUREMENT_INFO_KEY_HEIGHT
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_STATE_WIDTH (MEASUREMENT_UNIT_X - MEASUREMENT_STATE_X)
#define MEASUREMENT_STATE_OFFSET_X (CONTENT_WIDTH / 2 - MEASUREMENT_STATE_X)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_STATE_WIDTH (CONTENT_X + CONTENT_WIDTH - MEASUREMENT_STATE_X)
#define MEASUREMENT_STATE_OFFSET_X (MEASUREMENT_STATE_WIDTH - MEASUREMENT_PADDING)
#endif
#define MEASUREMENT_STATE_OFFSET_Y MEASUREMENT_INFO_KEY_OFFSET_Y
#define MEASUREMENT_STATE_RIGHT (MEASUREMENT_STATE_X + MEASUREMENT_STATE_WIDTH)
#define MEASUREMENT_BAR_DECADES 7
#define MEASUREMENT_BAR_WIDTH (MEASUREMENT_STATE_RIGHT - CONTENT_X)
#define MEASUREMENT_BAR_RIGHT MEASUREMENT_STATE_RIGHT
#define MEASUREMENT_BAR_LABEL_OFFSET_Y (MEASUREMENT_BAR_LABEL_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_BAR_TICKS_X (CONTENT_X + MEASUREMENT_PADDING + 2)
#define MEASUREMENT_BAR_TICKS_Y (MEASUREMENT_BAR_LABEL_Y + MEASUREMENT_BAR_LABEL_HEIGHT)
#define MEASUREMENT_BAR_TICKS_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_TICKS_X - 2)
#define MEASUREMENT_BAR_TICKS_LOWER_HEIGHT (MEASUREMENT_BAR_TICKS_HEIGHT - MEASUREMENT_BAR_TICKS_UPPER_HEIGHT)
#define MEASUREMENT_BAR_FRAME_X (CONTENT_X + MEASUREMENT_PADDING)
#define MEASUREMENT_BAR_FRAME_Y (MEASUREMENT_BAR_OUTERMARGIN_Y + MEASUREMENT_BAR_TICKS_LOWERMARGIN_HEIGHT)
#define MEASUREMENT_BAR_FRAME_WIDTH (MEASUREMENT_BAR_FRAME_RIGHT - MEASUREMENT_BAR_FRAME_X)
#define MEASUREMENT_BAR_FRAME_BOTTOM (MEASUREMENT_BAR_FRAME_Y + MEASUREMENT_BAR_FRAME_HEIGHT)
#define MEASUREMENT_BAR_INNERFRAME_X (MEASUREMENT_BAR_FRAME_X + 1)
#define MEASUREMENT_BAR_INNERFRAME_Y (MEASUREMENT_BAR_FRAME_Y + 1)
#define MEASUREMENT_BAR_INNERFRAME_WIDTH (MEASUREMENT_BAR_FRAME_WIDTH - 2)
#define MEASUREMENT_BAR_INNERFRAME_HEIGHT (MEASUREMENT_BAR_FRAME_HEIGHT - 2)
#define MEASUREMENT_BAR_INSTRUMENT_X (MEASUREMENT_BAR_INNERFRAME_X + 1)
#define MEASUREMENT_BAR_INSTRUMENT_Y (MEASUREMENT_BAR_INNERFRAME_Y + 1)
#define MEASUREMENT_BAR_INSTRUMENT_WIDTH (MEASUREMENT_BAR_INNERFRAME_WIDTH - 2)
#define MEASUREMENT_BAR_INSTRUMENT_HEIGHT (MEASUREMENT_BAR_INNERFRAME_HEIGHT - 2)
#define MEASUREMENT_BAR_OUTERMARGIN_X CONTENT_X
#define MEASUREMENT_BAR_OUTERMARGIN_Y (MEASUREMENT_BAR_TICKS_Y + MEASUREMENT_BAR_TICKS_HEIGHT)
#define MEASUREMENT_BAR_OUTERMARGIN_WIDTH (MEASUREMENT_BAR_RIGHT - MEASUREMENT_BAR_OUTERMARGIN_X)
#define MEASUREMENT_BAR_OUTERMARGIN_HEIGHT (CONTENT_BOTTOM - MEASUREMENT_BAR_OUTERMARGIN_Y)
#define MEASUREMENT_BAR_INNERMARGIN_X (CONTENT_X + MEASUREMENT_PADDING)
#define MEASUREMENT_BAR_INNERMARGIN_Y MEASUREMENT_BAR_FRAME_Y
#define MEASUREMENT_BAR_INNERMARGIN_WIDTH MEASUREMENT_BAR_FRAME_WIDTH
#define MEASUREMENT_BAR_INNERMARGIN_HEIGHT MEASUREMENT_BAR_FRAME_HEIGHT

#define HISTORY_X ((CONTENT_WIDTH - HISTORY_WIDTH) / 2)
#define HISTORY_Y (CONTENT_Y + (CONTENT_HEIGHT - HISTORY_HEIGHT) / 2)
#define HISTORY_DATA_X (HISTORY_X + 1)
#define HISTORY_DATA_Y (HISTORY_Y + 1)
#define HISTORY_DATA_WIDTH (HISTORY_WIDTH - 2)
#define HISTORY_DATA_HEIGHT (HISTORY_HEIGHT - 2)
#define HISTORY_TOP_LABEL_X HISTORY_X
#define HISTORY_TOP_LABEL_Y CONTENT_Y
#define HISTORY_TOP_LABEL_WIDTH HISTORY_WIDTH
#define HISTORY_TOP_LABEL_HEIGHT (HISTORY_Y - HISTORY_TOP_LABEL_Y)
#define HISTORY_BOTTOM_LABEL_X HISTORY_X
#define HISTORY_BOTTOM_LABEL_Y (HISTORY_Y + HISTORY_HEIGHT)
#define HISTORY_BOTTOM_LABEL_WIDTH HISTORY_WIDTH
#define HISTORY_BOTTOM_LABEL_HEIGHT (CONTENT_BOTTOM - HISTORY_BOTTOM_LABEL_Y)
#define HISTORY_SPACE_LEFT_X 0
#define HISTORY_SPACE_LEFT_Y HISTORY_TOP_LABEL_Y
#define HISTORY_SPACE_LEFT_WIDTH HISTORY_X
#define HISTORY_SPACE_LEFT_HEIGHT (CONTENT_BOTTOM - HISTORY_TOP_LABEL_Y)
#define HISTORY_SPACE_RIGHT_X (HISTORY_X + HISTORY_WIDTH)
#define HISTORY_SPACE_RIGHT_Y HISTORY_TOP_LABEL_Y
#define HISTORY_SPACE_RIGHT_WIDTH (CONTENT_WIDTH - HISTORY_SPACE_RIGHT_X)
#define HISTORY_SPACE_RIGHT_HEIGHT HISTORY_SPACE_LEFT_HEIGHT

#define RNG_DATA_X CONTENT_X
#define RNG_DATA_Y CONTENT_Y
#define RNG_DATA_WIDTH CONTENT_WIDTH
#define RNG_DATA_HEIGHT ((CONTENT_HEIGHT + FONT_SMALL_LINE_HEIGHT) / 2)
#define RNG_DATA_OFFSET_X CONTENT_PADDING
#define RNG_DATA_OFFSET_Y (RNG_DATA_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define RNG_STATE_X CONTENT_X
#define RNG_STATE_Y (RNG_DATA_Y + RNG_DATA_HEIGHT)
#define RNG_STATE_WIDTH CONTENT_WIDTH
#define RNG_STATE_HEIGHT (CONTENT_BOTTOM - RNG_STATE_Y)
#define RNG_STATE_OFFSET_X (CONTENT_WIDTH / 2)
#define RNG_STATE_OFFSET_Y ((RNG_STATE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT) / 2)

#define STATISTICS_ENTRY_HEIGHT FONT_SMALL_LINE_HEIGHT
#define STATISTICS_ENTRIES_HEIGHT (STATISTICS_ENTRY_NUM * STATISTICS_ENTRY_HEIGHT)
#define STATISTICS_TOPMARGIN_HEIGHT ((CONTENT_HEIGHT - STATISTICS_ENTRIES_HEIGHT) / 2)
#define STATISTICS_BOTTOMMARGIN_HEIGHT (CONTENT_HEIGHT - STATISTICS_TOPMARGIN_HEIGHT - STATISTICS_ENTRIES_HEIGHT)
#define STATISTICS_KEY_X 0
#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2)
#define STATISTICS_KEY_OFFSET_X CONTENT_PADDING
#define STATISTICS_VALUE_X (CONTENT_WIDTH / 2)
#define STATISTICS_VALUE_WIDTH (CONTENT_WIDTH / 2)
#define STATISTICS_VALUE_OFFSET_X 0

enum
{
#if defined(DISPLAY_PORTRAIT)
    STATISTICS_ENTRY_TUBE_TITLE,
#endif
    STATISTICS_ENTRY_TUBE_LIFE_TIME,
    STATISTICS_ENTRY_TUBE_LIFE_PULSES,
    STATISTICS_ENTRY_TUBE_DEAD_TIME,
    STATISTICS_ENTRY_SPACE,
#if defined(DISPLAY_PORTRAIT)
    STATISTICS_ENTRY_DEVICE_TITLE,
#endif
    STATISTICS_ENTRY_DEVICE_ID,
    STATISTICS_ENTRY_DEVICE_VOLTAGE,
    STATISTICS_ENTRY_NUM,
};

#define DATA_MODE_X CONTENT_X
#define DATA_MODE_Y CONTENT_Y
#define DATA_MODE_WIDTH CONTENT_WIDTH
#define DATA_MODE_HEIGHT CONTENT_HEIGHT
#define DATA_MODE_OFFSET_X (CONTENT_WIDTH / 2)
#define DATA_MODE_OFFSET_Y ((DATA_MODE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)

#define GAME_BOARD_WIDTH (8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_HEIGHT (8 * GAME_SQUARE_HEIGHT)
#define GAME_BOARD_RIGHT (GAME_BOARD_X + GAME_BOARD_WIDTH)
#define GAME_BOARD_BOTTOM (GAME_BOARD_Y + GAME_BOARD_HEIGHT)
#define GAME_HISTORY_SECOND_WIDTH (GAME_HISTORY_WIDTH - GAME_HISTORY_FIRST_WIDTH)
#if defined(DISPLAY_LANDSCAPE)
#define GAME_TOP_TIMER_X GAME_BOARD_RIGHT
#define GAME_TOP_TIMER_Y GAME_BOARD_Y
#define GAME_TOP_TIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_TOP_TIMER_HEIGHT (GAME_HISTORY_Y - GAME_BOARD_Y)
#define GAME_TOP_TIMER_OFFSET_X (FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_TOP_TIMER_OFFSET_Y ((2 * GAME_SQUARE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#define GAME_HISTORY_X GAME_TOP_TIMER_X
#define GAME_HISTORY_Y (GAME_BOARD_Y + (GAME_BOARD_HEIGHT - GAME_HISTORY_HEIGHT) / 2)
#define GAME_HISTORY_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_HISTORY_HEIGHT (3 * FONT_SMALL_LINE_HEIGHT)
#define GAME_BOTTOM_TIMER_X GAME_BOARD_RIGHT
#define GAME_BOTTOM_TIMER_Y (GAME_HISTORY_Y + GAME_HISTORY_HEIGHT)
#define GAME_BOTTOM_TIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_BOTTOM_TIMER_HEIGHT (GAME_BOARD_Y + GAME_BOARD_HEIGHT - GAME_BOTTOM_TIMER_Y)
#define GAME_BOTTOM_TIMER_OFFSET_X GAME_TOP_TIMER_OFFSET_X
#define GAME_BOTTOM_TIMER_OFFSET_Y (GAME_BOTTOM_TIMER_HEIGHT - 2 * GAME_SQUARE_HEIGHT + GAME_TOP_TIMER_OFFSET_Y)
#elif defined(DISPLAY_PORTRAIT)
#define GAME_TOP_TIMER_X GAME_BOARD_X
#define GAME_TOP_TIMER_Y CONTENT_Y
#define GAME_TOP_TIMER_WIDTH GAME_BOARD_WIDTH
#define GAME_TOP_TIMER_HEIGHT FONT_SMALL_LINE_HEIGHT
#define GAME_TOP_TIMER_OFFSET_X (GAME_TOP_TIMER_WIDTH - FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_TOP_TIMER_OFFSET_Y 0
#define GAME_HISTORY_X GAME_BOARD_X
#define GAME_HISTORY_Y GAME_BOARD_BOTTOM
#define GAME_HISTORY_WIDTH (GAME_BOARD_WIDTH - GAME_BOTTOM_TIMER_WIDTH)
#define GAME_HISTORY_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_BOTTOM_TIMER_X (GAME_BOARD_RIGHT - GAME_BOTTOM_TIMER_WIDTH)
#define GAME_BOTTOM_TIMER_Y (GAME_BOARD_Y + GAME_BOARD_HEIGHT)
#define GAME_BOTTOM_TIMER_WIDTH (3 * GAME_SQUARE_WIDTH)
#define GAME_BOTTOM_TIMER_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_BOTTOM_TIMER_OFFSET_X (GAME_BOTTOM_TIMER_WIDTH - FONT_SMALL_LINE_HEIGHT / 2)
#define GAME_BOTTOM_TIMER_OFFSET_Y 0
#define GAME_LEFTMARGIN_X CONTENT_X
#define GAME_LEFTMARGIN_Y CONTENT_Y
#define GAME_LEFTMARGIN_WIDTH (GAME_BOARD_X - CONTENT_X)
#define GAME_LEFTMARGIN_HEIGHT CONTENT_HEIGHT
#define GAME_RIGHTMARGIN_X (CONTENT_X + GAME_BOARD_RIGHT)
#define GAME_RIGHTMARGIN_Y CONTENT_Y
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

    COLOR_ALARM,

    COLOR_NORMAL_ENABLED_BACKGROUND,
    COLOR_FLAT_CHECKED_BACKGROUND,
    COLOR_ON_FLAT_ACTIVE,
    COLOR_ON_FLAT_NEUTRAL,

    COLOR_SELECTED_ENABLED_BACKGROUND,
    COLOR_GAME_SQUARE_BLACK,
    COLOR_GAME_SQUARE_WHITE,
    COLOR_GAME_PIECE_BLACK,
    COLOR_GAME_PIECE_WHITE,
} Color;

#define COLOR_INSTRUMENT_OFFSET 5

#if defined(DISPLAY_COLOR)

// Online color blender: https://pinetools.com/blend-colors
// Online RGB565 color picker: https://rgbcolorpicker.com/565

static const mr_color_t displayColors[][3] = {
    // Element active
    {mr_get_color(0x1a1a1a),
     mr_get_color(0xffffff),
     mr_get_color(0x686800)},

    // Element neutral (on container background)
    {mr_get_color(0x636363),
     mr_get_color(0xabadab), // 0xababab
     mr_get_color(0x474700)},

    // Container background [global background]
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x2e2f2e), // 0x2e2e2e
     mr_get_color(0x000000)},

    // Container global [title bar background]
    {mr_get_color(0xffffff), // 0xfcfcfc
     mr_get_color(0x363736), // 0x363636
     mr_get_color(0x000000)},

    // Instrument enhanced primary
    {mr_get_color(0x166ce4),
     mr_get_color(0x28a0ff),
     mr_get_color(0x0f4c24)},

    // Instrument enhanced tertiary (on instrument frame primary)
    {mr_get_color(0xbad3f7),
     mr_get_color(0x3b5266), // 0x3b5366
     mr_get_color(0x051107)},

    // Instrument enhanced tertiary (on instrument frame tertiary)
    {mr_get_color(0x9db6da),
     mr_get_color(0x183043),
     mr_get_color(0x243007)},

    // Instrument frame primary
    {mr_get_color(0xffffff),
     mr_get_color(0x404040),
     mr_get_color(0x030300)},

    // Instrument frame tertiary
    {mr_get_color(0xd6d6d6),
     mr_get_color(0x141214), // 0x141414
     mr_get_color(0x292900)},

    // Instrument enhanced primary alert zone 1
    {mr_get_color(0x1461ce),
     mr_get_color(0x2490e6),
     mr_get_color(0x0e4520)},

    // Instrument enhanced tertiary (on instrument frame primary) alert zone 1
    {mr_get_color(0xa8bedf),
     mr_get_color(0x354a5c), // 0x354b5c
     mr_get_color(0x050f06)},

    // Instrument enhanced tertiary (on instrument frame tertiary) alert zone 1
    {mr_get_color(0x8ea4c5),
     mr_get_color(0x162b3c),
     mr_get_color(0x202b06)},

    // Instrument frame primary alert zone 1
    {mr_get_color(0xe6e6e6),
     mr_get_color(0x3a3a3a),
     mr_get_color(0x030300)},

    // Instrument frame tertiary alert zone 1
    {mr_get_color(0xc1c0c1), // 0xc1c1c1
     mr_get_color(0x121212),
     mr_get_color(0x252500)},

    // Instrument enhanced primary alert zone 2
    {mr_get_color(0x1257ba),
     mr_get_color(0x2082cf),
     mr_get_color(0x0d3e1d)},

    // Instrument enhanced tertiary (on instrument frame primary) alert zone 2
    {mr_get_color(0x98abc9),
     mr_get_color(0x304253), // 0x304453
     mr_get_color(0x050e05)},

    // Instrument enhanced tertiary (on instrument frame tertiary) alert zone 2
    {mr_get_color(0x8094b2),
     mr_get_color(0x142736),
     mr_get_color(0x1d2705)},

    // Instrument frame primary alert zone 2
    {mr_get_color(0xcfcfcf),
     mr_get_color(0x343234), // 0x343434
     mr_get_color(0x030300)},

    // Instrument frame tertiary alert zone 2
    {mr_get_color(0xaeaeae),
     mr_get_color(0x101010),
     mr_get_color(0x212100)},

    // Instrument tickmark primary
    {mr_get_color(0x333333),
     mr_get_color(0xc6c6c6),
     mr_get_color(0x686800)},

    // Instrument tickmark secondary
    {mr_get_color(0x8c8c8c),
     mr_get_color(0x828282),
     mr_get_color(0x474700)},

    // Alarm
    {mr_get_color(0xdf1b1b),
     mr_get_color(0xe52626),
     mr_get_color(0xac1515)},

    // Normal enabled background [menu background]
    {mr_get_color(0xffffff),
     mr_get_color(0x424242),
     mr_get_color(0x000000)},

    // Flat checked background (on container global) [menu selected background]
    {mr_get_color(0xd6dde9),  // Changed alpha to 20 %
     mr_get_color(0x3a4d63),  // 0x394c6a, changed alpha to 50 %
     mr_get_color(0x101c29)}, // 0x111e33, changed alpha to 35 %

    // On flat active (on container global) [menu selected foreground]
    {mr_get_color(0x1a1a1a),
     mr_get_color(0xffffff),
     mr_get_color(0x686800)}, // 0x686800

    // On flat neutral (on container global) [menu selected gadget]
    {mr_get_color(0x737373),
     mr_get_color(0xb0b0b0),
     mr_get_color(0x424200)},

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
};

#endif

// Gamma-corrected linear brightness values

// value = 8000 * [0.25, 0.5, 0.75, 1] ^ 2.2
uint16_t displayBrightnessValue[] = {
    47, 218, 531, 1000};

// Definitions

#if defined(DISPLAY_MONOCHROME)
static const Menu displayContrastMenu;
#elif defined(DISPLAY_COLOR)
static const Menu displayThemeMenu;
#endif
static const Menu displayBrightnessMenu;
static const Menu displaySleepMenu;

static const Menu displayFlashesMenu;

const uint32_t menuLineNum = MENU_LINE_NUM;

// Variables

mr_t mr;

// Functions

void initDisplay(void)
{
    initDisplayController();
    initDisplayBacklight();

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

    selectMenuItem(&displayFlashesMenu,
                   settings.pulseFlashes,
                   PULSE_FLASHES_NUM);
}

// Drawing functions

static inline mr_color_t getFillColor(Color color)
{
#if defined(DISPLAY_MONOCHROME)
    if ((color == COLOR_INSTRUMENT_ENHANCED_PRIMARY) ||
        (color == COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY) ||
        (color == COLOR_INSTRUMENT_FRAME_TERTIARY) ||
        (color == COLOR_INSTRUMENT_TICKMARK_PRIMARY) ||
        (color == COLOR_INSTRUMENT_TICKMARK_SECONDARY) ||
        (color == COLOR_FLAT_CHECKED_BACKGROUND) ||
        (color == COLOR_GAME_SQUARE_BLACK))
        return 0xffff;
    else
        return 0x0000;
#elif defined(DISPLAY_COLOR)
    return displayColors[color][settings.displayTheme];
#endif
}

static void setFillColor(Color color)
{
#if defined(DISPLAY_MONOCHROME)
    mr_color_t fillColor = getFillColor(color);
    mr_color_t textColor = fillColor ? 0x0000 : 0xffff;

    mr_set_fill_color(&mr, fillColor);
    mr_set_text_color(&mr, textColor);
#elif defined(DISPLAY_COLOR)
    mr_set_fill_color(&mr, getFillColor(color));
#endif
}

static void setTextColor(Color color)
{
#if defined(DISPLAY_COLOR)
    mr_set_text_color(&mr, getFillColor(color));
#endif
}

static void drawRectangle(const mr_rectangle_t *rectangle)
{
    mr_draw_rectangle(&mr,
                      rectangle);
}

static void drawMargin(const mr_rectangle_t *outerMargin,
                       const mr_rectangle_t *innerMargin)
{
    mr_rectangle_t r;

    mr_point_t outerRectangleEnd = {
        outerMargin->x + outerMargin->width,
        outerMargin->y + outerMargin->height};

    mr_point_t innerRectangleEnd = {
        innerMargin->x + innerMargin->width,
        innerMargin->y + innerMargin->height};

    // Top
    r.x = outerMargin->x;
    r.y = outerMargin->y;
    r.width = outerMargin->width;
    r.height = innerMargin->y - outerMargin->y;

    mr_draw_rectangle(&mr,
                      &r);

    // Bottom
    r.y = innerRectangleEnd.y;
    r.height = outerRectangleEnd.y - innerRectangleEnd.y;

    mr_draw_rectangle(&mr,
                      &r);

    // Left
    r.y = innerMargin->y;
    r.width = innerMargin->x - outerMargin->x;
    r.height = innerMargin->height;

    mr_draw_rectangle(&mr,
                      &r);

    // Right
    r.x = innerRectangleEnd.x;
    r.width = outerRectangleEnd.y - innerRectangleEnd.y;

    mr_draw_rectangle(&mr,
                      &r);
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
                      mr_color_t *imageBuffer)
{
    mr_draw_image(&mr,
                  rectangle,
                  imageBuffer);
}

static void setFont(const uint8_t *font)
{
    mr_set_font(&mr, font);
}

static int16_t getTextWidth(const char *str)
{
    return mr_get_text_width(&mr, str);
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

static void drawText(const char *str,
                     const mr_rectangle_t *rectangle,
                     const mr_point_t *offset)
{
    mr_draw_text(&mr,
                 str,
                 rectangle,
                 offset);
}

static void drawCenteredText(const char *str,
                             const mr_rectangle_t *rectangle,
                             const mr_point_t *offset)
{
    mr_point_t centeredOffset = *offset;

    centeredOffset.x -= getTextWidth(str) / 2;

    drawText(str,
             rectangle,
             &centeredOffset);
}

static void drawRightAlignedText(const char *str,
                                 const mr_rectangle_t *rectangle,
                                 const mr_point_t *offset)
{
    mr_point_t rightAlignedOffset = *offset;

    rightAlignedOffset.x -= getTextWidth(str);

    drawText(str,
             rectangle,
             &rightAlignedOffset);
}

void drawTitleBar(const char *title)
{
    setFillColor(COLOR_NORMAL_ENABLED_BACKGROUND);

    mr_rectangle_t rectangle;
    mr_point_t offset;
    char buffer[8];

    rectangle.y = TITLEBAR_Y;
    rectangle.height = TITLEBAR_HEIGHT;
    offset.y = TITLEBAR_TEXT_OFFSET_Y;

    // Title
    rectangle.x = TITLEBAR_X;
    rectangle.width = TITLEBAR_TITLE_WIDTH;
    offset.x = TITLEBAR_TITLE_OFFSET_X;

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawText(title,
             &rectangle,
             &offset);

    // Time
    RTCDateTime dateTime;
    
    
    getDeviceDateTime(&dateTime);

    bool show12H = (settings.displayTimeFormat == DISPLAY_TIMEFORMAT_12H);
    int8_t hour = dateTime.hour;

    if(show12H) {
        if(hour == 0) {
            hour = 12;
        } else if (hour > 12) {
            hour -= 12;
        }
    }

    strclr(buffer);
    if (dateTime.year >= RTC_YEAR_MIN)
    {
        strcatUInt32(buffer, hour, (show12H && (hour < 10)) ? 1 : 2);
        strcatChar(buffer, ':');
        strcatUInt32(buffer, dateTime.minute, 2);

        if(show12H) {
            if(dateTime.hour >= 12) {
                strcatChar(buffer, 'P');
            } else {
                strcatChar(buffer, 'A');
            }
            strcatChar(buffer, 'M');
        }
    }

    rectangle.x = TITLEBAR_CLOCK_X;
    rectangle.width = TITLEBAR_CLOCK_WIDTH;
    offset.x = TITLEBAR_CLOCK_WIDTH;

    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawRightAlignedText(buffer,
             &rectangle,
             &offset);

    // Battery
    int8_t batteryLevel = getDeviceBatteryLevel();

    strclr(buffer);
    buffer[0] = (batteryLevel == BATTERY_LEVEL_CHARGING)
                    ? '6'
                    : '0' + batteryLevel;
    buffer[1] = '\0';

    rectangle.x = TITLEBAR_BATTERY_X;
    rectangle.width = TITLEBAR_BATTERY_WIDTH;
    offset.x = 0;
    offset.y = TITLEBAR_BATTERY_OFFSET_Y;

    setFont(FONT_SYMBOLS);
    drawText(buffer,
             &rectangle,
             &offset);

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

    setTextColor(isHold
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

    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawText(unitString,
             &partialRectangle,
             &partialOffset);
}

void drawNotification(const char *title,
                      const char *subtitle)
{
    // Top line
    const mr_rectangle_t topRectangle = {
        0,
        0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT / 2};

    const mr_point_t topOffset = {
        DISPLAY_WIDTH / 2,
        DISPLAY_HEIGHT / 2 -
            FONT_MEDIUM_LINE_HEIGHT};

    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_CONTAINER_GLOBAL);
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

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawCenteredText(subtitle,
                     &bottomRectangle,
                     &bottomOffset);
}

void drawLowBattery(void)
{
    const mr_rectangle_t rectangle = {
        0,
        0,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT};

    const mr_point_t offset = {
        DISPLAY_WIDTH / 2,
        (DISPLAY_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2};

    setFont(FONT_SYMBOLS);
    setTextColor(COLOR_ALARM);
    setFillColor(COLOR_CONTAINER_GLOBAL);
    drawCenteredText("0",
                     &rectangle,
                     &offset);
}

void drawMenu(const Menu *menu)
{
    drawTitleBar(menu->title);

    setFont(FONT_MEDIUM);

    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    mr_rectangle_t rectangle;
    rectangle.height = MENU_LINE_HEIGHT;
    rectangle.y = CONTENT_Y;

    const mr_point_t menuOptionOffset = {
        MENU_OFFSET_X,
        (MENU_LINE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT) / 2};

    const mr_point_t menuGadgetOffset = {
        1,
        (MENU_LINE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT) / 2};

    for (uint32_t i = 0; i < MENU_LINE_NUM; i++)
    {
        uint32_t index = startIndex + i;

        MenuStyle menuStyle;
        const char *menuItem = menu->onGetOption(menu,
                                                 index,
                                                 &menuStyle);

        if (!menuItem)
            break;

        rectangle.x = CONTENT_WIDTH;

        setFillColor((index == selectedIndex)
                         ? COLOR_FLAT_CHECKED_BACKGROUND
                         : COLOR_CONTAINER_GLOBAL);

        // Submenu
        if (menuStyle & MENUSTYLE_SUBMENU)
        {
            rectangle.width = MENU_SUBMENU_WIDTH + MENU_OFFSET_X;
            rectangle.x -= rectangle.width;

            setTextColor(COLOR_ON_FLAT_NEUTRAL);
            drawText("\x80",
                     &rectangle,
                     &menuGadgetOffset);
        }

        // Checked
        if (menuStyle & MENUSTYLE_CHECKED)
        {
            rectangle.width = MENU_CHECKED_WIDTH + MENU_OFFSET_X;
            rectangle.x -= rectangle.width;

            setTextColor(COLOR_INSTRUMENT_ENHANCED_PRIMARY);
            drawText("\x81",
                     &rectangle,
                     &menuGadgetOffset);
        }

        // Text
        rectangle.width = rectangle.x;
        rectangle.x = 0;

        setTextColor(COLOR_ON_FLAT_ACTIVE);
        drawText(menuItem,
                 &rectangle,
                 &menuOptionOffset);

        rectangle.y += MENU_LINE_HEIGHT;
    }

    // Bottom space
    rectangle.x = CONTENT_X;
    rectangle.width = CONTENT_WIDTH;
    rectangle.height = CONTENT_BOTTOM - rectangle.y;

    setFillColor(COLOR_CONTAINER_GLOBAL);
    drawRectangle(&rectangle);
}

void drawTestMode(const char lines[8][32])
{
    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
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

void drawMeasurementValue(const char *valueString,
                          const char *unitString,
                          float confidence,
                          MeasurementStyle style)
{
    // Value
    const mr_rectangle_t valueRectangle = {
        MEASUREMENT_VALUE_X,
        MEASUREMENT_VALUE_Y,
        MEASUREMENT_VALUE_WIDTH,
        MEASUREMENT_VALUE_HEIGHT};

    const mr_point_t valueOffset = {
        MEASUREMENT_VALUE_OFFSET_X,
        MEASUREMENT_VALUE_OFFSET_Y};

    setFont(FONT_LARGE);
    setTextColor(
        (style == MEASUREMENTSTYLE_ALARM)
            ? COLOR_ALARM
        : (style == MEASUREMENTSTYLE_HOLD)
            ? COLOR_ELEMENT_ACTIVE
            : COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawText(valueString,
             &valueRectangle,
             &valueOffset);

#if defined(DISPLAY_MONOCHROME)
    if (style == MEASUREMENTSTYLE_ALARM)
        drawGrayedRectangle(&valueRectangle);
#endif

    // Unit
    const mr_rectangle_t unitRectangle = {
        MEASUREMENT_UNIT_X,
        MEASUREMENT_UNIT_Y,
        MEASUREMENT_UNIT_WIDTH,
        MEASUREMENT_UNIT_HEIGHT};

    const mr_point_t unitOffset = {
        MEASUREMENT_UNIT_OFFSET_X,
        MEASUREMENT_UNIT_OFFSET_Y};

    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawRightAlignedText(unitString,
                         &unitRectangle,
                         &unitOffset);

    // Confidence
    char confidenceString[8];
    char confidenceUnit[8];

    if (confidence != 0)
    {
        strcpy(confidenceString, "\xb1");
        strcatFloat(confidenceString,
                    100 * confidence,
                    confidence < 0.1F ? 1 : 0);
        strcpy(confidenceUnit, "%");
    }
    else
    {
        strclr(confidenceString);
        strclr(confidenceUnit);
    }

    setFont(FONT_SMALL);

    const mr_rectangle_t confidenceRectangle = {
        MEASUREMENT_CONFIDENCE_X,
        MEASUREMENT_CONFIDENCE_Y,
        MEASUREMENT_CONFIDENCE_WIDTH,
        MEASUREMENT_CONFIDENCE_HEIGHT};

    mr_point_t confidenceOffset = {
        MEASUREMENT_CONFIDENCE_OFFSET_X -
            getTextWidth(confidenceString) -
            getTextWidth(confidenceUnit),
        MEASUREMENT_CONFIDENCE_OFFSET_Y};

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
    const mr_rectangle_t infoKeyRectangle = {
        MEASUREMENT_INFO_X,
        MEASUREMENT_INFO_KEY_Y,
        MEASUREMENT_INFO_WIDTH,
        MEASUREMENT_INFO_KEY_HEIGHT};

    const mr_point_t infoKeyOffset = {
        MEASUREMENT_INFO_KEY_OFFSET_X,
        MEASUREMENT_INFO_KEY_OFFSET_Y};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawText(infoKeyString,
             &infoKeyRectangle,
             &infoKeyOffset);

    const mr_rectangle_t stateRectangle = {
        MEASUREMENT_STATE_X,
        MEASUREMENT_STATE_Y,
        MEASUREMENT_STATE_WIDTH,
        MEASUREMENT_STATE_HEIGHT};

    const mr_point_t stateOffset = {
        MEASUREMENT_STATE_OFFSET_X,
        MEASUREMENT_STATE_OFFSET_Y};

    setTextColor(COLOR_ELEMENT_NEUTRAL);
#if defined(DISPLAY_LANDSCAPE)
    drawCenteredText(stateString,
                     &stateRectangle,
                     &stateOffset);
#elif defined(DISPLAY_PORTRAIT)
    drawRightAlignedText(stateString,
                         &stateRectangle,
                         &stateOffset);
#endif

    const mr_rectangle_t infoValueRectangle = {
        MEASUREMENT_INFO_X,
        MEASUREMENT_INFO_VALUE_Y,
        MEASUREMENT_INFO_WIDTH,
        MEASUREMENT_INFO_VALUE_HEIGHT};

    const mr_point_t infoValueOffset = {
        MEASUREMENT_INFO_VALUE_OFFSET_X,
        MEASUREMENT_INFO_VALUE_OFFSET_Y};

    setTextColor((style == MEASUREMENTSTYLE_HOLD)
                     ? COLOR_ELEMENT_ACTIVE
                     : COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawValueAndUnit(infoValueString,
                     infoUnitString,
                     &infoValueRectangle,
                     &infoValueOffset,
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

    return instrumentValue;
}

void drawMeasurementBar(float value,
                        int32_t minExponent,
                        float alertZone1Value,
                        float alertZone2Value)
{
    mr_rectangle_t rectangle;
    mr_color_t color;

    float minValue = powf(10.0F, minExponent);
    uint32_t instrumentValue =
        getMeasurementBarInstrumentValue(value, minValue);
#if defined(DISPLAY_COLOR)
    uint32_t instrumentAlertZone1Value =
        getMeasurementBarInstrumentValue(alertZone1Value, minValue);
    uint32_t instrumentAlertZone2Value =
        getMeasurementBarInstrumentValue(alertZone2Value, minValue);
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

        uint32_t xTickNext = MEASUREMENT_BAR_TICKS_X +
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
                MEASUREMENT_BAR_LABEL_Y,
                width,
                MEASUREMENT_BAR_LABEL_HEIGHT};

            mr_point_t offset = {
                xLabelCenter - xLabel,
                MEASUREMENT_BAR_LABEL_OFFSET_Y,
            };

            setTextColor(COLOR_ELEMENT_ACTIVE);
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
        color = isLargeTick
                    ? COLOR_INSTRUMENT_TICKMARK_PRIMARY
                    : COLOR_CONTAINER_BACKGROUND;

        rectangle = (mr_rectangle_t){
            xTick,
            MEASUREMENT_BAR_TICKS_Y,
            tickWidth,
            MEASUREMENT_BAR_TICKS_UPPER_HEIGHT};

        setFillColor(color);
        drawRectangle(&rectangle);

        color = isLargeTick
                    ? COLOR_INSTRUMENT_TICKMARK_PRIMARY
                : isTick
                    ? COLOR_INSTRUMENT_TICKMARK_SECONDARY
                    : COLOR_CONTAINER_BACKGROUND;

        rectangle.y += MEASUREMENT_BAR_TICKS_UPPER_HEIGHT;
        rectangle.height = MEASUREMENT_BAR_TICKS_LOWER_HEIGHT;

        setFillColor(color);
        drawRectangle(&rectangle);

        xTick = xTickNext;
    }

    // Bar outer frame
    const mr_rectangle_t barFrameRectangle = {
        MEASUREMENT_BAR_FRAME_X,
        MEASUREMENT_BAR_FRAME_Y,
        MEASUREMENT_BAR_FRAME_WIDTH,
        MEASUREMENT_BAR_FRAME_HEIGHT};

    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);
    drawFrame(&barFrameRectangle);

    // Bar inner frame
    rectangle = (mr_rectangle_t){
        MEASUREMENT_BAR_INNERFRAME_X,
        MEASUREMENT_BAR_INNERFRAME_Y,
        1,
        MEASUREMENT_BAR_INNERFRAME_HEIGHT};

    setFillColor(COLOR_INSTRUMENT_FRAME_PRIMARY);
    drawRectangle(&rectangle);

    rectangle.x = MEASUREMENT_BAR_INNERFRAME_X + MEASUREMENT_BAR_INNERFRAME_WIDTH - 1;

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
            MEASUREMENT_BAR_INSTRUMENT_X + xBar,
            MEASUREMENT_BAR_INSTRUMENT_Y,
            xBarNext - xBar,
            MEASUREMENT_BAR_INSTRUMENT_HEIGHT,
        };

        setFillColor(alertZoneColor + ((xBar < instrumentValue)
                                           ? COLOR_INSTRUMENT_ENHANCED_PRIMARY
                                           : COLOR_INSTRUMENT_FRAME_PRIMARY));
        drawRectangle(&rectangle);

        rectangle.y = MEASUREMENT_BAR_INNERFRAME_Y;
        rectangle.height = 1;

        setFillColor(alertZoneColor + COLOR_INSTRUMENT_FRAME_PRIMARY);
        drawRectangle(&rectangle);

        rectangle.y = MEASUREMENT_BAR_INNERFRAME_Y +
                      MEASUREMENT_BAR_INNERFRAME_HEIGHT - 1;

        drawRectangle(&rectangle);

        xBar = xBarNext;
    }

    // Margin
    const mr_rectangle_t barOuterMargin = {
        MEASUREMENT_BAR_OUTERMARGIN_X,
        MEASUREMENT_BAR_OUTERMARGIN_Y,
        MEASUREMENT_BAR_OUTERMARGIN_WIDTH,
        MEASUREMENT_BAR_OUTERMARGIN_HEIGHT};

    const mr_rectangle_t barInnerMargin = {
        MEASUREMENT_BAR_INNERMARGIN_X,
        MEASUREMENT_BAR_INNERMARGIN_Y,
        MEASUREMENT_BAR_INNERMARGIN_WIDTH,
        MEASUREMENT_BAR_INNERMARGIN_HEIGHT};

    setFillColor(COLOR_CONTAINER_BACKGROUND);
    drawMargin(&barOuterMargin,
               &barInnerMargin);
}

static int16_t getHistoryY(uint8_t value,
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
                 uint8_t alertZone1Value,
                 uint8_t alertZone2Value)
{
    // Analysis
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

    char topLabelString[16];
    char bottomLabelString[16];
    int32_t dataExponentMinValue = 0;
    uint32_t dataTickNum = 1;

    strclr(topLabelString);
    strclr(bottomLabelString);

    if (dataMax > 0)
    {
        uint32_t cpsToRateUnit =
            (uint32_t)(HISTORY_DECADE *
                       (16 + log10f(scale * HISTORY_VALUE_MIN))) +
            1;

        int32_t exponentMax =
            (int32_t)((cpsToRateUnit + dataMax) /
                          HISTORY_DECADE -
                      16 + 1);
        int32_t exponentMin =
            (int32_t)((cpsToRateUnit + dataMin) /
                          HISTORY_DECADE -
                      16);

        dataExponentMinValue = (int32_t)((exponentMin + 16) * HISTORY_DECADE -
                                         cpsToRateUnit);
        dataTickNum = exponentMax - exponentMin;

        strcatDecimalPowerWithMetricPrefix(topLabelString, exponentMax, -2);
        strcat(topLabelString, unitString);

        strcatDecimalPowerWithMetricPrefix(bottomLabelString, exponentMin, -2);
        strcat(bottomLabelString, unitString);
    }

    uint32_t dataRange = HISTORY_DECADE * dataTickNum;

    // Labels
    const mr_rectangle_t topLabelRectangle = {
        HISTORY_TOP_LABEL_X,
        HISTORY_TOP_LABEL_Y,
        HISTORY_TOP_LABEL_WIDTH,
        HISTORY_TOP_LABEL_HEIGHT};

    const mr_point_t topLabelOffset = {
        HISTORY_TOP_LABEL_OFFSET_X,
        HISTORY_TOP_LABEL_OFFSET_Y};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawRightAlignedText(topLabelString,
                         &topLabelRectangle,
                         &topLabelOffset);

    const mr_rectangle_t bottomLabelRectangle = {
        HISTORY_BOTTOM_LABEL_X,
        HISTORY_BOTTOM_LABEL_Y,
        HISTORY_BOTTOM_LABEL_WIDTH,
        HISTORY_BOTTOM_LABEL_HEIGHT};

    const mr_point_t bottomLabelOffset = {
        HISTORY_BOTTOM_LABEL_OFFSET_X,
        HISTORY_BOTTOM_LABEL_OFFSET_Y};

    drawRightAlignedText(bottomLabelString,
                         &bottomLabelRectangle,
                         &bottomLabelOffset);

    // Space and frame
    const mr_rectangle_t leftSpaceRectangle = {
        HISTORY_SPACE_LEFT_X,
        HISTORY_SPACE_LEFT_Y,
        HISTORY_SPACE_LEFT_WIDTH,
        HISTORY_SPACE_LEFT_HEIGHT};

    drawRectangle(&leftSpaceRectangle);

    const mr_rectangle_t rightSpaceRectangle = {
        HISTORY_SPACE_RIGHT_X,
        HISTORY_SPACE_RIGHT_Y,
        HISTORY_SPACE_RIGHT_WIDTH,
        HISTORY_SPACE_RIGHT_HEIGHT};

    drawRectangle(&rightSpaceRectangle);

    const mr_rectangle_t frameRectangle = {
        HISTORY_X,
        HISTORY_Y,
        HISTORY_WIDTH,
        HISTORY_HEIGHT};

    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);
    drawFrame(&frameRectangle);

    // Value tick table
    uint8_t valueTickTable[32];
    uint32_t valueTickTableIndex = 0;

    for (uint32_t yTickIndex = 1;
         yTickIndex <= timeTickNum;
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

    int16_t yDataLast;
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
            mr_color_t color = COLOR_INSTRUMENT_FRAME_PRIMARY;
            int16_t yNext = 0;

            // Render time and value ticks
            int16_t yTickNext;
            if (timeTickDraw)
            {
                yTickNext = HISTORY_DATA_HEIGHT;

                color = COLOR_INSTRUMENT_FRAME_TERTIARY;
            }
            else
            {
                yTickNext = valueTickTable[valueTickTableIndex];

                if (valueTickTableIndex & 0b1)
                    color = COLOR_INSTRUMENT_FRAME_TERTIARY;

                if (y == yTickNext)
                    valueTickTableIndex++;
            }
            yNext = yTickNext;

            // Render data
            int16_t yDataNext;
            if (y < yDataBottom)
            {
                yDataNext = yDataBottom;

                if (color == COLOR_INSTRUMENT_FRAME_PRIMARY)
                    color = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY;
                else
                    color = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY;
            }
            else if (y < yDataTop)
            {
                yDataNext = yDataTop;

                color = COLOR_INSTRUMENT_ENHANCED_PRIMARY;
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

                color += COLOR_INSTRUMENT_OFFSET;
            }
            else
            {
                yAlertZoneNext = HISTORY_DATA_HEIGHT;

                color += 2 * COLOR_INSTRUMENT_OFFSET;
            }
            if (yAlertZoneNext < yNext)
                yNext = yAlertZoneNext;
#endif
            // Render to imagebuffer
            mr_color_t fillColor = getFillColor(color);
            for (int i = y; i < yNext; i++)
                imageBuffer[(HISTORY_DATA_HEIGHT - 1) - i] = fillColor;

            y = yNext;
        }

        // Draw
        mr_rectangle_t historyImageRectangle = {
            HISTORY_DATA_X + x,
            HISTORY_DATA_Y,
            1,
            HISTORY_DATA_HEIGHT};

        drawImage(&historyImageRectangle,
                  imageBuffer);
    }
}

void drawRNG(const char *title,
             const char *rngString,
             const char *stateString)
{
    drawTitleBar(title);

    // RNG string
    const mr_rectangle_t rngDataRectangle = {
        RNG_DATA_X,
        RNG_DATA_Y,
        RNG_DATA_WIDTH,
        RNG_DATA_HEIGHT};

    const mr_point_t rngDataOffset = {
        RNG_DATA_OFFSET_X,
        RNG_DATA_OFFSET_Y};

    setFont(FONT_SMALL);
    setTextColor(COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawText(rngString,
             &rngDataRectangle,
             &rngDataOffset);

    // State
    const mr_rectangle_t rngStateRectangle = {
        RNG_STATE_X,
        RNG_STATE_Y,
        RNG_STATE_WIDTH,
        RNG_STATE_HEIGHT};

    const mr_point_t rngStateOffset = {
        RNG_STATE_OFFSET_X,
        RNG_STATE_OFFSET_Y};

    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawCenteredText(stateString,
                     &rngStateRectangle,
                     &rngStateOffset);
}

void drawStatistics(void)
{
    drawTitleBar("Statistics");

    setFont(FONT_SMALL);

    char key[32];
    char valueString[32];
    char unitString[16];

    mr_rectangle_t rectangle;
    mr_point_t offset;

    uint32_t y = CONTENT_Y;

    for (uint32_t i = 0; i < STATISTICS_ENTRY_NUM; i++)
    {
        strclr(key);
        strclr(valueString);
        strclr(unitString);

        switch (i)
        {
#if defined(DISPLAY_PORTRAIT)
        case STATISTICS_ENTRY_TUBE_TITLE:
            strcpy(key, "Tube");

            break;
#endif

        case STATISTICS_ENTRY_TUBE_LIFE_TIME:
#if !defined(DISPLAY_PORTRAIT)
            strcpy(key, "Tube Life Time:");
#else
            strcpy(key, "Life Time:");
#endif
            strcatTime(valueString, getTubeTime());

            break;

        case STATISTICS_ENTRY_TUBE_LIFE_PULSES:
#if !defined(DISPLAY_PORTRAIT)
            strcpy(key, "Tube Life Pulses:");
#else
            strcpy(key, "Life Pulses:");
#endif
            strcatUInt32(valueString, getTubePulseCount(), 0);

            break;

        case STATISTICS_ENTRY_TUBE_DEAD_TIME:
#if !defined(DISPLAY_PORTRAIT)
            strcpy(key, "Tube Dead Time:");
#else
            strcpy(key, "Dead Time:");
#endif
            float deadTime = getTubeDeadTime();
            if (deadTime >= 1)
                strcpy(valueString, "-");
            else
            {
                strcpy(valueString, "< ");
                strcatFloat(valueString, 1000000 * deadTime, 1);
                strcpy(unitString, " \xb5"
                                   "s");
            }

            break;

#if defined(DISPLAY_PORTRAIT)
        case STATISTICS_ENTRY_DEVICE_TITLE:
            strcpy(key, "Device");

            break;
#endif

        case STATISTICS_ENTRY_DEVICE_ID:
#if !defined(DISPLAY_PORTRAIT)
            strcpy(key, "Device ID:");
#else
            strcpy(key, "ID:");
#endif
            strcatUInt32Hex(unitString, getDeviceId());

            break;

        case STATISTICS_ENTRY_DEVICE_VOLTAGE:
#if !defined(DISPLAY_PORTRAIT)
            strcpy(key, "Device Voltage:");
#else
            strcpy(key, "Voltage:");
#endif
            strcatFloat(valueString, getDeviceBatteryVoltage(), 3);
            strcpy(unitString, " V");

            break;
        };

        // Key
        rectangle = (mr_rectangle_t){
            STATISTICS_KEY_X,
            y,
            STATISTICS_KEY_WIDTH,
            STATISTICS_ENTRY_HEIGHT};

        offset = (mr_point_t){
            STATISTICS_KEY_OFFSET_X,
            0};

        if (i == 0)
        {
            rectangle.height += STATISTICS_TOPMARGIN_HEIGHT;
            offset.y += STATISTICS_TOPMARGIN_HEIGHT;
            y += STATISTICS_TOPMARGIN_HEIGHT;
        }
        else if (i == (STATISTICS_ENTRY_NUM - 1))
            rectangle.height += STATISTICS_BOTTOMMARGIN_HEIGHT;

        setTextColor(
            (!valueString[0] && !unitString[0])
                ? COLOR_ELEMENT_ACTIVE
                : COLOR_ELEMENT_NEUTRAL);
        drawText(key,
                 &rectangle,
                 &offset);

        // Value
        rectangle.x = STATISTICS_VALUE_X;
        offset.x = STATISTICS_VALUE_OFFSET_X;

        drawValueAndUnit(valueString,
                         unitString,
                         &rectangle,
                         &offset,
                         false);

        y += FONT_SMALL_LINE_HEIGHT;
    }
}

void drawGame(const uint8_t board[8][8],
              const char time[2][16],
              const char history[3][2][6])
{
    mr_rectangle_t rectangle;
    mr_point_t offset;

#if defined(DISPLAY_COLOR)
    drawTitleBar("Game");
#endif

    // Board
    static const char *const gamePieceMap = "@AACFBDE";

    setFont(FONT_GAME);

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
            setTextColor(pieceBlack
                             ? COLOR_GAME_PIECE_BLACK
                             : COLOR_GAME_PIECE_WHITE);
            setFillColor(pieceSelected
                             ? COLOR_SELECTED_ENABLED_BACKGROUND
                         : squareBlack
                             ? COLOR_GAME_SQUARE_BLACK
                             : COLOR_GAME_SQUARE_WHITE);
#endif

            char pieceString[2];
            pieceString[0] = pieceChar;
            pieceString[1] = '\0';

            rectangle.x = GAME_BOARD_X + GAME_SQUARE_WIDTH * x;
            rectangle.y = GAME_BOARD_Y + GAME_SQUARE_HEIGHT * y;

            drawText(pieceString,
                     &rectangle,
                     &gamePieceOffset);
        }
    }

    // Top timer
    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);

    const mr_rectangle_t topTimerRectangle = {
        GAME_TOP_TIMER_X,
        GAME_TOP_TIMER_Y,
        GAME_TOP_TIMER_WIDTH,
        GAME_TOP_TIMER_HEIGHT};

    const mr_point_t topTimerOffset = {
        GAME_TOP_TIMER_OFFSET_X,
        GAME_TOP_TIMER_OFFSET_Y};

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
    rectangle.y = GAME_HISTORY_Y;
    rectangle.height = FONT_SMALL_LINE_HEIGHT;

    offset = (mr_point_t){
        0,
        0};

    for (uint32_t y = 0; y < 3; y++)
    {
        rectangle.x = GAME_HISTORY_X;
        rectangle.width = GAME_HISTORY_FIRST_WIDTH;
        if (y == 2)
            rectangle.height = GAME_HISTORY_HEIGHT - 2 * FONT_SMALL_LINE_HEIGHT;
        offset.x = GAME_HISTORY_FIRST_OFFSET_X;

        drawText(history[y][0],
                 &rectangle,
                 &offset);

        rectangle.x += GAME_HISTORY_FIRST_WIDTH;
        rectangle.width = GAME_HISTORY_SECOND_WIDTH;
        offset.x = 0;

        drawText(history[y][1],
                 &rectangle,
                 &offset);

        rectangle.y += FONT_SMALL_LINE_HEIGHT;
    }

    // Bottom timer
    const mr_rectangle_t bottomTimerRectangle = {
        GAME_BOTTOM_TIMER_X,
        GAME_BOTTOM_TIMER_Y,
        GAME_BOTTOM_TIMER_WIDTH,
        GAME_BOTTOM_TIMER_HEIGHT};

    const mr_point_t bottomTimerOffset = {
        GAME_BOTTOM_TIMER_OFFSET_X,
        GAME_BOTTOM_TIMER_OFFSET_Y};

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
        GAME_LEFTMARGIN_X,
        GAME_LEFTMARGIN_Y,
        GAME_LEFTMARGIN_WIDTH,
        GAME_LEFTMARGIN_HEIGHT};

    drawRectangle(&gameLeftMarginRectangle);
#endif

#if defined(GAME_RIGHTMARGIN_WIDTH)
    const mr_rectangle_t gameRightMarginRectangle = {
        GAME_RIGHTMARGIN_X,
        GAME_RIGHTMARGIN_Y,
        GAME_RIGHTMARGIN_WIDTH,
        GAME_RIGHTMARGIN_HEIGHT};

    drawRectangle(&gameRightMarginRectangle);
#endif
}

// Display menu

const View displayContrastMenuView;
const View displayThemeMenuView;
const View displayBrightnessMenuView;
const View displaySleepMenuView;

static const OptionView displayMenuOptions[] = {
#if defined(DISPLAY_MONOCHROME)
    {"Contrast", &displayContrastMenuView},
#elif defined(DISPLAY_COLOR)
    {"Theme", &displayThemeMenuView},
#endif
    {"Brightness Level", &displayBrightnessMenuView},
#if defined(DISPLAY_MONOCHROME)
    {"Backlight Timeout", &displaySleepMenuView},
#elif defined(DISPLAY_COLOR)
    {"Sleep", &displaySleepMenuView},
#endif
    {NULL},
};

static const char *onDisplayMenuGetOption(const Menu *menu,
                                          uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return displayMenuOptions[index].option;
}

static void onDisplayMenuSelect(const Menu *menu)
{
    setView(displayMenuOptions[menu->state->selectedIndex].view);
}

static void onDisplaySubMenuBack(const Menu *menu)
{
    setView(&displayMenuView);
}

static MenuState displayMenuState;

static const Menu displayMenu = {
    "Display",
    &displayMenuState,
    onDisplayMenuGetOption,
    onDisplayMenuSelect,
    onSettingsSubMenuBack,
};

const View displayMenuView = {
    onMenuEvent,
    &displayMenu,
};

// Display contrast level menu

#if defined(DISPLAY_MONOCHROME)

static const char *onDisplayContrastMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    if (index >= DISPLAY_CONTRAST_NUM)
        return NULL;

    *menuStyle = (index == settings.displayContrast);

    strcpy(menuOption, "Level ");
    strcatUInt32(menuOption, index + 1, 0);

    return menuOption;
}

static void onDisplayContrastMenuSelect(const Menu *menu)
{
    settings.displayContrast = menu->state->selectedIndex;

    updateDisplayContrast();
}

static MenuState displayContrastMenuState;

static const Menu displayContrastMenu = {
    "Contrast",
    &displayContrastMenuState,
    onDisplayContrastMenuGetOption,
    onDisplayContrastMenuSelect,
    onDisplaySubMenuBack,
};

const View displayContrastMenuView = {
    onMenuEvent,
    &displayContrastMenu,
};

#endif

// Display theme menu

#if defined(DISPLAY_COLOR)

static const char *const displayThemeMenuOptions[] = {
    "Day",
    "Dusk",
    "Night",
    NULL,
};

static const char *onDisplayThemeMenuGetOption(const Menu *menu,
                                               uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayTheme);

    return displayThemeMenuOptions[index];
}

static void onDisplayThemeMenuSelect(const Menu *menu)
{
    settings.displayTheme = menu->state->selectedIndex;

    updateView();
}

static MenuState displayThemeMenuState;

static const Menu displayThemeMenu = {
    "Theme",
    &displayThemeMenuState,
    onDisplayThemeMenuGetOption,
    onDisplayThemeMenuSelect,
    onDisplaySubMenuBack,
};

const View displayThemeMenuView = {
    onMenuEvent,
    &displayThemeMenu,
};

#endif

// Display brightness level menu

static const char *const displayBrightnessMenuOptions[] = {
    "Very Low",
    "Low",
    "Medium",
    "High",
    NULL,
};

static const char *onDisplayBrightnessMenuGetOption(const Menu *menu,
                                                    uint32_t index,
                                                    MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayBrightness);

    return displayBrightnessMenuOptions[index];
}

static void onDisplayBrightnessMenuSelect(const Menu *menu)
{
    settings.displayBrightness = menu->state->selectedIndex;

    setDisplayBacklightOn(true);
}

static MenuState displayBrightnessMenuState;

static const Menu displayBrightnessMenu = {
    "Brightness Level",
    &displayBrightnessMenuState,
    onDisplayBrightnessMenuGetOption,
    onDisplayBrightnessMenuSelect,
    onDisplaySubMenuBack,
};

const View displayBrightnessMenuView = {
    onMenuEvent,
    &displayBrightnessMenu,
};

// Display sleep menu

static const char *const displaySleepMenuOptions[] = {
#if defined(DISPLAY_MONOCHROME)
    "Always Off",
#endif
#if !defined(DISPLAY_240X320)
    "After 10 Seconds",
    "After 30 Seconds",
#else
    "After 10 Sec.",
    "After 30 Sec.",
#endif
    "After 1 Minute",
    "After 2 Minutes",
    "After 5 Minutes",
    "Never",
    NULL,
};

static const char *onDisplaySleepMenuGetOption(const Menu *menu,
                                               uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displaySleep);

    return displaySleepMenuOptions[index];
}

static void onDisplaySleepMenuSelect(const Menu *menu)
{
    settings.displaySleep = menu->state->selectedIndex;

    triggerDisplay();
}

static MenuState displaySleepMenuState;

static const Menu displaySleepMenu = {
#if defined(DISPLAY_MONOCHROME)
    "Backlight Timeout",
#elif defined(DISPLAY_COLOR)
    "Sleep",
#endif
    &displaySleepMenuState,
    onDisplaySleepMenuGetOption,
    onDisplaySleepMenuSelect,
    onDisplaySubMenuBack,
};

const View displaySleepMenuView = {
    onMenuEvent,
    &displaySleepMenu,
};

// Display flashes menu

static const char *const displayFlashesMenuOptions[] = {
    "Off",
    "On",
    NULL,
};

static const char *onDisplayFlashesMenuGetOption(const Menu *menu,
                                                 uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseFlashes);

    return displayFlashesMenuOptions[index];
}

static void onDisplayFlashesMenuSelect(const Menu *menu)
{
    settings.pulseFlashes = menu->state->selectedIndex;
}

static MenuState displayFlashesMenuState;

static const Menu displayFlashesMenu = {
    "Display Flashes",
    &displayFlashesMenuState,
    onDisplayFlashesMenuGetOption,
    onDisplayFlashesMenuSelect,
    onPulsesSubMenuBack,
};

const View displayFlashesMenuView = {
    onMenuEvent,
    &displayFlashesMenu,
};
