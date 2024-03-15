/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include <mcu-renderer.h>

#include "adc.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"

#if defined(DISPLAY_128X64)

#include "fonts/font_tiny5_1.h"
#include "fonts/font_robotoR8_1.h"
#include "fonts/font_robotoM18_1_digits.h"
#include "fonts/font_symbols5_1.h"
#include "fonts/font_chess8_1.h"

#define FONT_SMALL font_tiny5_1
#define FONT_SMALL_CAP_HEIGHT FONT_TINY5_1_CAP_HEIGHT
#define FONT_SMALL_LINE_HEIGHT FONT_TINY5_1_LINE_HEIGHT

#define FONT_MEDIUM font_robotoR8_1
#define FONT_MEDIUM_CAP_HEIGHT FONT_ROBOTOR8_1_CAP_HEIGHT
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTOR8_1_LINE_HEIGHT

#define FONT_LARGE font_robotoM18_1_digits
#define FONT_LARGE_CAP_HEIGHT FONT_ROBOTOM18_1_DIGITS_CAP_HEIGHT
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOM18_1_DIGITS_LINE_HEIGHT

#define FONT_SYMBOLS font_symbols5_1
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS5_1_LINE_HEIGHT

#define FONT_GAME font_chess8_1

#elif defined(DISPLAY_320X240) || defined(DISPLAY_240X320)

#if defined(DISPLAY_FONT_3BPP)

#include "fonts/font_robotoR12_3.h"
#include "fonts/font_robotoR18_3.h"
#include "fonts/font_robotoR48_3_digits.h"
#include "fonts/font_symbols30_3.h"
#include "fonts/font_chess25_3.h"

#define FONT_SMALL font_robotoR12_3
#define FONT_SMALL_CAP_HEIGHT FONT_ROBOTOR12_3_CAP_HEIGHT
#define FONT_SMALL_LINE_HEIGHT FONT_ROBOTOR12_3_LINE_HEIGHT

#define FONT_MEDIUM font_robotoR18_3
#define FONT_MEDIUM_CAP_HEIGHT FONT_ROBOTOR18_3_CAP_HEIGHT
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTOR18_3_LINE_HEIGHT

#define FONT_LARGE font_robotoR48_3_digits
#define FONT_LARGE_CAP_HEIGHT FONT_ROBOTOR48_3_DIGITS_CAP_HEIGHT
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOR48_3_DIGITS_LINE_HEIGHT

#define FONT_SYMBOLS font_symbols30_3
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS30_3_LINE_HEIGHT

#define FONT_GAME font_chess25_3

#else

#include "fonts/font_robotoR12_4.h"
#include "fonts/font_robotoR18_4.h"
#include "fonts/font_robotoR48_4_digits.h"
#include "fonts/font_symbols30_4.h"
#include "fonts/font_chess25_4.h"

#define FONT_SMALL font_robotoR12_4
#define FONT_SMALL_CAP_HEIGHT FONT_ROBOTOR12_4_CAP_HEIGHT
#define FONT_SMALL_LINE_HEIGHT FONT_ROBOTOR12_4_LINE_HEIGHT

#define FONT_MEDIUM font_robotoR18_4
#define FONT_MEDIUM_CAP_HEIGHT FONT_ROBOTOR18_4_CAP_HEIGHT
#define FONT_MEDIUM_LINE_HEIGHT FONT_ROBOTOR18_4_LINE_HEIGHT

#define FONT_LARGE font_robotoR48_4_digits
#define FONT_LARGE_CAP_HEIGHT FONT_ROBOTOR48_4_DIGITS_CAP_HEIGHT
#define FONT_LARGE_LINE_HEIGHT FONT_ROBOTOR48_4_DIGITS_LINE_HEIGHT

#define FONT_SYMBOLS font_symbols30_4
#define FONT_SYMBOLS_LINE_HEIGHT FONT_SYMBOLS30_4_LINE_HEIGHT

#define FONT_GAME font_chess25_4

#endif

#endif

#if defined(DISPLAY_128X64)

#define NOTIFICATION_TOP_VERTICAL_OFFSET 0
#define NOTIFICATION_BOTTOM_VERTICAL_OFFSET 4

#define DATA_X 12

#define TITLE_HEIGHT 13

#define MENU_TITLE_HEIGHT 8
#define MENU_LINE_HEIGHT 12
#define MENU_LINE_OFFSET_X 3
#define MENU_SUBMENU_WIDTH 5
#define MENU_CHECKED_WIDTH 8

#define MEASUREMENT_VALUE_WIDTH 76
#define MEASUREMENT_VALUE_HEIGHT 23
#define MEASUREMENT_VALUE_OFFSET_X (DATA_X - 1)
#define MEASUREMENT_VALUE_OFFSET_Y -1
#define MEASUREMENT_CONFIDENCE_X (MEASUREMENT_VALUE_X + MEASUREMENT_VALUE_WIDTH)
#define MEASUREMENT_CONFIDENCE_Y MEASUREMENT_VALUE_Y
#define MEASUREMENT_CONFIDENCE_HEIGHT 7
#define MEASUREMENT_CONFIDENCE_OFFSET_X 0
#define MEASUREMENT_CONFIDENCE_OFFSET_Y 1
#define MEASUREMENT_UNIT_X MEASUREMENT_CONFIDENCE_X
#define MEASUREMENT_UNIT_Y (MEASUREMENT_CONFIDENCE_Y + MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_UNIT_HEIGHT (MEASUREMENT_VALUE_HEIGHT - MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_UNIT_OFFSET_X 0
#define MEASUREMENT_UNIT_OFFSET_Y 0
#define MEASUREMENT_TIME_Y (MEASUREMENT_VALUE_Y + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_TIME_WIDTH MEASUREMENT_VALUE_WIDTH
#define MEASUREMENT_TIME_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_TIME_VALUE_HEIGHT (DISPLAY_HEIGHT - MEASUREMENT_TIME_VALUE_Y)
#define MEASUREMENT_TIME_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_STATE_X MEASUREMENT_VALUE_WIDTH
#define MEASUREMENT_STATE_Y (MEASUREMENT_VALUE_Y + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_STATE_WIDTH (DISPLAY_WIDTH - MEASUREMENT_STATE_X)
#define MEASUREMENT_STATE_HEIGHT FONT_SMALL_LINE_HEIGHT
#define MEASUREMENT_STATE_OFFSET_X 0

#define STATISTICS_NUM 5
#define STATISTICS_WIDTH 72

#define HISTORY_TOP_LEGEND_X 82
#define HISTORY_TOP_LEGEND_Y 14
#define HISTORY_TOP_LEGEND_WIDTH (HISTORY_SPACE_RIGHT_X - HISTORY_TOP_LEGEND_X)
#define HISTORY_TOP_LEGEND_OFFSET_X (HISTORY_TOP_LEGEND_WIDTH - 1)
#define HISTORY_TOP_LEGEND_OFFSET_Y 0
#define HISTORY_BOTTOM_LEGEND_OFFSET_X (HISTORY_WIDTH - 1)
#define HISTORY_BOTTOM_LEGEND_OFFSET_Y 0
#define HISTORY_X 4
#define HISTORY_Y 21
#define HISTORY_WIDTH 120
#define HISTORY_HEIGHT 36

#define GAME_BOARD_X 0
#define GAME_BOARD_Y 0
#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8
#define GAME_TOP_TIMER_WIDTH 23
#define GAME_HISTORY_SPACE_LEFT_WIDTH 32
#define GAME_HISTORY_SPACE_LEFT_OFFSET_X 8
#define GAME_SPACE_X (GAME_TOP_TIMER_X + GAME_TOP_TIMER_WIDTH)
#define GAME_SPACE_Y (STATUSBAR_Y + STATUSBAR_HEIGHT)
#define GAME_SPACE_WIDTH (DISPLAY_WIDTH - GAME_SPACE_X)
#define GAME_SPACE_HEIGHT (GAME_HISTORY_Y - GAME_SPACE_Y)

#elif defined(DISPLAY_320X240)

#define DATA_X 26

#define MENU_TITLE_HEIGHT 28
#define MENU_LINE_HEIGHT 48

#define MEASUREMENT_VALUE_WIDTH 202
#define MEASUREMENT_VALUE_HEIGHT (MEASUREMENT_TIME_Y - MEASUREMENT_VALUE_Y)
#define MEASUREMENT_CONFIDENCE_X (MEASUREMENT_VALUE_X + MEASUREMENT_VALUE_WIDTH)
#define MEASUREMENT_CONFIDENCE_Y MEASUREMENT_VALUE_Y
#define MEASUREMENT_CONFIDENCE_HEIGHT 32
#define MEASUREMENT_CONFIDENCE_OFFSET_X 0
#define MEASUREMENT_CONFIDENCE_OFFSET_Y 10
#define MEASUREMENT_UNIT_X MEASUREMENT_CONFIDENCE_X
#define MEASUREMENT_UNIT_Y (MEASUREMENT_CONFIDENCE_Y + MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_UNIT_HEIGHT (MEASUREMENT_VALUE_HEIGHT - MEASUREMENT_CONFIDENCE_HEIGHT)
#define MEASUREMENT_UNIT_OFFSET_X 0
#define MEASUREMENT_UNIT_OFFSET_Y 0
#define MEASUREMENT_TIME_Y 167
#define MEASUREMENT_TIME_WIDTH MEASUREMENT_VALUE_WIDTH
#define MEASUREMENT_TIME_VALUE_HEIGHT (DISPLAY_HEIGHT - MEASUREMENT_TIME_VALUE_Y)
#define MEASUREMENT_STATE_X MEASUREMENT_VALUE_WIDTH
#define MEASUREMENT_STATE_Y MEASUREMENT_TIME_Y
#define MEASUREMENT_STATE_WIDTH (DISPLAY_WIDTH - MEASUREMENT_STATE_X)
#define MEASUREMENT_STATE_OFFSET_X 0

#define STATISTICS_NUM 7
#define STATISTICS_WIDTH 172

#define HISTORY_TOP_LEGEND_OFFSET_X (HISTORY_WIDTH - 4)
#define HISTORY_TOP_LEGEND_OFFSET_Y 7
#define HISTORY_BOTTOM_LEGEND_OFFSET_X (HISTORY_WIDTH - 4)
#define HISTORY_BOTTOM_LEGEND_OFFSET_Y 2
#define HISTORY_X 10
#define HISTORY_Y 108
#define HISTORY_WIDTH 300
#define HISTORY_HEIGHT 96

#define GAME_BOARD_X 0
#define GAME_BOARD_Y (STATUSBAR_Y + STATUSBAR_HEIGHT)
#define GAME_TOP_TIMER_WIDTH (DISPLAY_WIDTH - GAME_TOP_TIMER_X)
#define GAME_HISTORY_SPACE_LEFT_WIDTH 66
#define GAME_HISTORY_SPACE_LEFT_OFFSET_X 12
#define GAME_SPACE_X 0
#define GAME_SPACE_Y (GAME_BOARD_Y + GAME_BOARD_HEIGHT)
#define GAME_SPACE_WIDTH DISPLAY_WIDTH
#define GAME_SPACE_HEIGHT (DISPLAY_HEIGHT - GAME_SPACE_Y)

#elif defined(DISPLAY_240X320)

#define DATA_X 34

#define MENU_TITLE_HEIGHT 24
#define MENU_LINE_HEIGHT 46

#define MEASUREMENT_VALUE_WIDTH DISPLAY_WIDTH
#define MEASUREMENT_VALUE_HEIGHT 63
#define MEASUREMENT_UNIT_X 0
#define MEASUREMENT_UNIT_Y (MEASUREMENT_VALUE_Y + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_UNIT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_UNIT_OFFSET_X (MEASUREMENT_VALUE_WIDTH - DATA_X)
#define MEASUREMENT_UNIT_OFFSET_Y 0
#define MEASUREMENT_CONFIDENCE_X 0
#define MEASUREMENT_CONFIDENCE_Y (MEASUREMENT_UNIT_Y + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_CONFIDENCE_HEIGHT (MEASUREMENT_TIME_Y - MEASUREMENT_CONFIDENCE_Y)
#define MEASUREMENT_CONFIDENCE_OFFSET_X (MEASUREMENT_VALUE_WIDTH - DATA_X)
#define MEASUREMENT_CONFIDENCE_OFFSET_Y 0
#define MEASUREMENT_TIME_Y 198
#define MEASUREMENT_TIME_WIDTH MEASUREMENT_VALUE_WIDTH
#define MEASUREMENT_TIME_VALUE_HEIGHT (DISPLAY_HEIGHT - MEASUREMENT_TIME_VALUE_Y)
#define MEASUREMENT_STATE_X 0
#define MEASUREMENT_STATE_Y 256
#define MEASUREMENT_STATE_WIDTH DISPLAY_WIDTH
#define MEASUREMENT_STATE_OFFSET_X DATA_X

#define STATISTICS_NUM 9
#define STATISTICS_WIDTH 126

#define HISTORY_TOP_LEGEND_OFFSET_X (HISTORY_WIDTH - 4)
#define HISTORY_TOP_LEGEND_OFFSET_Y 20
#define HISTORY_BOTTOM_LEGEND_OFFSET_X (HISTORY_WIDTH - 4)
#define HISTORY_BOTTOM_LEGEND_OFFSET_Y 2
#define HISTORY_X 20
#define HISTORY_Y 120
#define HISTORY_WIDTH 200
#define HISTORY_HEIGHT 96

#define GAME_BOARD_X 20
#define GAME_BOARD_Y (STATUSBAR_Y + STATUSBAR_HEIGHT + FONT_SMALL_LINE_HEIGHT)
#define GAME_TIMERS_RIGHTALIGNED
#define GAME_TOP_TIMER_X GAME_BOARD_X
#define GAME_TOP_TIMER_Y FONT_SMALL_LINE_HEIGHT
#define GAME_TOP_TIMER_WIDTH GAME_BOARD_WIDTH
#define GAME_TOP_TIMER_HEIGHT FONT_SMALL_LINE_HEIGHT
#define GAME_TOP_TIMER_OFFSET_X (GAME_TOP_TIMER_WIDTH - 4)
#define GAME_TOP_TIMER_OFFSET_Y 0
#define GAME_BOTTOM_TIMER_X GAME_BOARD_X
#define GAME_BOTTOM_TIMER_Y (GAME_BOARD_Y + GAME_BOARD_HEIGHT)
#define GAME_BOTTOM_TIMER_WIDTH GAME_BOARD_WIDTH
#define GAME_BOTTOM_TIMER_HEIGHT FONT_SMALL_LINE_HEIGHT
#define GAME_BOTTOM_TIMER_OFFSET_X GAME_TOP_TIMER_OFFSET_X
#define GAME_BOTTOM_TIMER_OFFSET_Y 0
#define GAME_HISTORY_X GAME_BOARD_X
#define GAME_HISTORY_Y (GAME_BOTTOM_TIMER_Y + GAME_BOTTOM_TIMER_HEIGHT)
#define GAME_HISTORY_WIDTH GAME_BOARD_WIDTH
#define GAME_HISTORY_HEIGHT (DISPLAY_HEIGHT - (GAME_BOTTOM_TIMER_Y + GAMTE_BOTTOM_TIMER_HEIGHT))
#define GAME_HISTORY_SPACE_LEFT_WIDTH 107
#define GAME_HISTORY_SPACE_RIGHT_WIDTH (GAME_BOARD_WIDTH - GAME_HISTORY_SPACE_LEFT_WIDTH)
#define GAME_HISTORY_SPACE_LEFT_OFFSET_X 52
#define GAME_SPACE_X 0
#define GAME_SPACE_Y GAME_TOP_TIMER_Y
#define GAME_SPACE_WIDTH (GAME_BOARD_X - GAME_SPACE_X)
#define GAME_SPACE_HEIGHT (DISPLAY_HEIGHT - GAME_SPACE_Y)
#define GAME_SPACE2_X (GAME_BOARD_X + GAME_BOARD_WIDTH)
#define GAME_SPACE2_Y GAME_TOP_TIMER_Y
#define GAME_SPACE2_WIDTH (DISPLAY_WIDTH - GAME_SPACE2_X)
#define GAME_SPACE2_HEIGHT (DISPLAY_HEIGHT - GAME_SPACE_Y)

#endif

#if defined(DISPLAY_320X240) || defined(DISPLAY_240X320)

#define NOTIFICATION_TOP_VERTICAL_OFFSET -1
#define NOTIFICATION_BOTTOM_VERTICAL_OFFSET 9

#define TITLE_HEIGHT 60

#define MENU_SUBMENU_WIDTH 12
#define MENU_CHECKED_WIDTH 21
#define MENU_LINE_OFFSET_X 11

#define MEASUREMENT_VALUE_OFFSET_X (DATA_X - 1)
#define MEASUREMENT_VALUE_OFFSET_Y -9
#define MEASUREMENT_TIME_HEIGHT (FONT_SMALL_LINE_HEIGHT + 1)
#define MEASUREMENT_STATE_HEIGHT (FONT_SMALL_LINE_HEIGHT + 1)

#define GAME_SQUARE_WIDTH 25
#define GAME_SQUARE_HEIGHT 25

#endif

#if defined(DISPLAY_128X64) || defined(DISPLAY_320X240)

#define GAME_TOP_TIMER_X (GAME_BOARD_X + GAME_BOARD_WIDTH)
#define GAME_TOP_TIMER_Y GAME_BOARD_Y
#define GAME_TOP_TIMER_HEIGHT (GAME_HISTORY_Y - GAME_TOP_TIMER_Y)
#define GAME_TOP_TIMER_OFFSET_X (FONT_SMALL_LINE_HEIGHT / 3)
#define GAME_TOP_TIMER_OFFSET_Y ((2 * GAME_SQUARE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#define GAME_HISTORY_X GAME_TOP_TIMER_X
#define GAME_HISTORY_Y (GAME_BOARD_Y + (GAME_BOARD_HEIGHT - GAME_HISTORY_HEIGHT) / 2)
#define GAME_HISTORY_WIDTH GAME_TOP_TIMER_WIDTH
#define GAME_HISTORY_HEIGHT (3 * FONT_SMALL_LINE_HEIGHT)
#define GAME_HISTORY_SPACE_RIGHT_WIDTH (DISPLAY_WIDTH - GAME_TOP_TIMER_X - GAME_HISTORY_SPACE_LEFT_WIDTH)
#define GAME_BOTTOM_TIMER_X GAME_TOP_TIMER_X
#define GAME_BOTTOM_TIMER_Y (GAME_HISTORY_Y + GAME_HISTORY_HEIGHT)
#define GAME_BOTTOM_TIMER_WIDTH (DISPLAY_WIDTH - GAME_TOP_TIMER_X)
#define GAME_BOTTOM_TIMER_HEIGHT (GAME_BOARD_Y + GAME_BOARD_HEIGHT - GAME_BOTTOM_TIMER_Y)
#define GAME_BOTTOM_TIMER_OFFSET_X GAME_TOP_TIMER_OFFSET_X
#define GAME_BOTTOM_TIMER_OFFSET_Y (GAME_BOTTOM_TIMER_HEIGHT - 2 * GAME_SQUARE_HEIGHT + GAME_TOP_TIMER_OFFSET_Y)

#endif

#define STATUSBAR_X 0
#define STATUSBAR_Y 0
#define STATUSBAR_WIDTH DISPLAY_WIDTH
#define STATUSBAR_HEIGHT FONT_SMALL_LINE_HEIGHT

#define TITLE_Y (STATUSBAR_Y + STATUSBAR_HEIGHT)

#define MENU_TITLE_Y (STATUSBAR_Y + STATUSBAR_HEIGHT)
#define MENU_TITLE_BOTTOM (MENU_TITLE_Y + MENU_TITLE_HEIGHT)
#define MENU_LINES_TOP MENU_TITLE_BOTTOM
#define MENU_LINE_NUM ((DISPLAY_HEIGHT - MENU_TITLE_BOTTOM) / MENU_LINE_HEIGHT)

#define MEASUREMENT_VALUE_X 0
#define MEASUREMENT_VALUE_Y (TITLE_Y + TITLE_HEIGHT)
#define MEASUREMENT_CONFIDENCE_WIDTH (DISPLAY_WIDTH - MEASUREMENT_CONFIDENCE_X)
#define MEASUREMENT_UNIT_WIDTH MEASUREMENT_CONFIDENCE_WIDTH
#define MEASUREMENT_TIME_X 0
#define MEASUREMENT_TIME_OFFSET_X DATA_X
#define MEASUREMENT_TIME_OFFSET_Y 0
#define MEASUREMENT_TIME_VALUE_X 0
#define MEASUREMENT_TIME_VALUE_Y (MEASUREMENT_TIME_Y + MEASUREMENT_TIME_HEIGHT)
#define MEASUREMENT_TIME_VALUE_WIDTH MEASUREMENT_TIME_WIDTH
#define MEASUREMENT_TIME_VALUE_OFFSET_X DATA_X
#define MEASUREMENT_TIME_VALUE_OFFSET_Y 0
#define MEASUREMENT_STATE_OFFSET_Y 0
#define MEASUREMENT_STATE_VALUE_X MEASUREMENT_STATE_X
#define MEASUREMENT_STATE_VALUE_Y (MEASUREMENT_STATE_Y + MEASUREMENT_STATE_HEIGHT)
#define MEASUREMENT_STATE_VALUE_WIDTH MEASUREMENT_STATE_WIDTH
#define MEASUREMENT_STATE_VALUE_HEIGHT (DISPLAY_HEIGHT - MEASUREMENT_STATE_VALUE_Y)
#define MEASUREMENT_STATE_VALUE_OFFSET_X MEASUREMENT_STATE_OFFSET_X
#define MEASUREMENT_STATE_VALUE_OFFSET_Y 0

#if !defined(HISTORY_TOP_LEGEND_X)
#define HISTORY_TOP_LEGEND_X HISTORY_X
#define HISTORY_TOP_LEGEND_Y (TITLE_Y + TITLE_HEIGHT)
#define HISTORY_TOP_LEGEND_WIDTH HISTORY_WIDTH
#endif
#define HISTORY_TOP_LEGEND_HEIGHT (HISTORY_Y - HISTORY_TOP_LEGEND_Y)
#define HISTORY_BOTTOM_LEGEND_X HISTORY_X
#define HISTORY_BOTTOM_LEGEND_Y (HISTORY_Y + HISTORY_HEIGHT)
#define HISTORY_BOTTOM_LEGEND_WIDTH HISTORY_WIDTH
#define HISTORY_BOTTOM_LEGEND_HEIGHT (DISPLAY_HEIGHT - HISTORY_BOTTOM_LEGEND_Y)
#define HISTORY_SPACE_LEFT_X 0
#define HISTORY_SPACE_LEFT_Y HISTORY_TOP_LEGEND_Y
#define HISTORY_SPACE_LEFT_WIDTH HISTORY_X
#define HISTORY_SPACE_LEFT_HEIGHT (DISPLAY_HEIGHT - HISTORY_TOP_LEGEND_Y)
#define HISTORY_SPACE_RIGHT_X (HISTORY_X + HISTORY_WIDTH)
#define HISTORY_SPACE_RIGHT_Y HISTORY_TOP_LEGEND_Y
#define HISTORY_SPACE_RIGHT_WIDTH (DISPLAY_WIDTH - HISTORY_SPACE_RIGHT_X)
#define HISTORY_SPACE_RIGHT_HEIGHT HISTORY_SPACE_LEFT_HEIGHT

#define RNG_STRING_X 0
#define RNG_STRING_Y (TITLE_Y + TITLE_HEIGHT)
#define RNG_STRING_WIDTH DISPLAY_WIDTH
#define RNG_STRING_HEIGHT (DISPLAY_HEIGHT - 2 * RNG_STRING_Y)
#define RNG_STRING_OFFSET_X DATA_X
#define RNG_STRING_OFFSET_Y ((RNG_STRING_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#define RNG_STATE_X 0
#define RNG_STATE_Y (RNG_STRING_Y + RNG_STRING_HEIGHT)
#define RNG_STATE_WIDTH DISPLAY_WIDTH
#define RNG_STATE_HEIGHT (DISPLAY_HEIGHT - RNG_STATE_Y)

#define STATISTICS_X 0
#define STATISTICS_Y (TITLE_Y + TITLE_HEIGHT)
#define STATISTICS_HEIGHT FONT_SMALL_LINE_HEIGHT
#define STATISTICS_OFFSET_X DATA_X
#define STATISTICS_VALUE_X (STATISTICS_X + STATISTICS_WIDTH)
#define STATISTICS_VALUE_WIDTH (DISPLAY_WIDTH - STATISTICS_WIDTH)
#define STATISTICS_VALUE_HEIGHT FONT_SMALL_LINE_HEIGHT

#define GAME_BOARD_WIDTH (8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_HEIGHT (8 * GAME_SQUARE_HEIGHT)

#if defined(DISPLAY_MONOCHROME)
static const Menu displayContrastMenu;
#elif defined(DISPLAY_COLOR)
static const Menu displayThemeMenu;
#endif
static const Menu displayBrightnessMenu;
static const Menu displaySleepMenu;

const uint32_t menuLineNum = MENU_LINE_NUM;

mr_t mr;

bool displayOn;

// Linear display brightness values (gamma corrected):
// value = 8000 * [0.25, 0.5, 0.75, 1] ^ 2.2
uint16_t displayBrightnessValue[] = {
    47, 218, 531, 1000};

// Colors

// Online color blender: https://pinetools.com/blend-colors
// Online RGB565 color picker: https://rgbcolorpicker.com/565

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

#if defined(DISPLAY_COLOR)

static const mr_color displayColors[][3] = {
    // Element active
    {mr_get_color(0x1a1a1a),
     mr_get_color(0xffffff),
     mr_get_color(0x686c00)}, // 0x686800

    // Element neutral (on container background)
    {mr_get_color(0x626262),
     mr_get_color(0xa7a7a7),
     mr_get_color(0x474700)},

    // Container background
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x2e2f2e), // 0x2e2e2e
     mr_get_color(0x000000)},

    // Container global
    {mr_get_color(0xfcfdfc), // 0xfcfcfc
     mr_get_color(0x363736), // 0x363636
     mr_get_color(0x000000)},

    // Instrument enhanced primary
    {mr_get_color(0x16ce4),
     mr_get_color(0x28a0ff),
     mr_get_color(0x0f4c24)},

    // Instrument enhanced tertiary (on instrument frame primary)
    {mr_get_color(0xb9d2f6),
     mr_get_color(0x385c78),
     mr_get_color(0x030f07)},

    // Instrument enhanced tertiary (on instrument frame tertiary)
    {mr_get_color(0x9cb6da),
     mr_get_color(0x1c405c),
     mr_get_color(0x222e07)},

    // Instrument frame primary
    {mr_get_color(0xffffff),
     mr_get_color(0x404040),
     mr_get_color(0x030300)},

    // Instrument frame tertiary
    {mr_get_color(0xd6d6d6),
     mr_get_color(0x141414),
     mr_get_color(0x292900)},

    // Alarm
    {mr_get_color(0xdf1b1b),
     mr_get_color(0xe52626),
     mr_get_color(0xac1515)},

    // Normal enabled background
    {mr_get_color(0xffffff),
     mr_get_color(0x424242),
     mr_get_color(0x000000)},

    // Flat checked background (on container global)
    {mr_get_color(0xe7ebf1),
     mr_get_color(0x34404a),  // Alt: 0x35393f
     mr_get_color(0x080f1a)}, // Changed alpha to 18%

    // On flat active (on container global)
    {mr_get_color(0x191919),
     mr_get_color(0xffffff),
     mr_get_color(0x686c00)}, // 0x686800

    // On flat neutral (on container global)
    {mr_get_color(0x717171),
     mr_get_color(0xaaadaa), // 0xaaaaaa
     mr_get_color(0x424200)},

    // Selected enabled background
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
}

// Drawing functions

void setDisplayOn(bool value)
{
    displayOn = value;

    mr_set_display(&mr, value);
}

bool isDisplayOn(void)
{
    return displayOn;
}

void refreshDisplay(void)
{
#if !defined(SIMULATOR)
    mr_refresh_display(&mr);
#endif
}

static void setFillColor(Color color)
{
#if defined(DISPLAY_MONOCHROME)
    mr_color mr_fill_color;
    mr_color mr_text_color;

    if ((color == COLOR_INSTRUMENT_ENHANCED_PRIMARY) ||
        (color == COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY) ||
        (color == COLOR_INSTRUMENT_FRAME_TERTIARY) ||
        (color == COLOR_FLAT_CHECKED_BACKGROUND) ||
        (color == COLOR_GAME_SQUARE_BLACK))
    {
        mr_text_color = 0x0000;
        mr_fill_color = 0xffff;
    }
    else
    {
        mr_text_color = 0xffff;
        mr_fill_color = 0x0000;
    }

    mr_set_fill_color(&mr, mr_fill_color);
    mr_set_text_color(&mr, mr_text_color);
#elif defined(DISPLAY_COLOR)
    mr_set_fill_color(&mr, displayColors[color][settings.displayTheme]);
#endif
}

static void setTextColor(Color color)
{
#if defined(DISPLAY_COLOR)
    mr_set_text_color(&mr, displayColors[color][settings.displayTheme]);
#endif
}

static void drawRectangle(const mr_rectangle_t *rectangle)
{
    mr_draw_rectangle(&mr,
                      rectangle);
}

static void setFont(const uint8_t *font)
{
    mr_set_font(&mr, font);
}

static int16_t getTextWidth(const char *str)
{
    return mr_get_text_width(&mr, str);
}

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

void drawStatusBar(void)
{
    setFillColor(COLOR_NORMAL_ENABLED_BACKGROUND);

    // Date and time

    RTCDateTime dateTime;
    getDeviceDateTime(&dateTime);

    char timeString[6] = "";

    if (dateTime.year >= RTC_YEAR_MIN)
    {
        strcatUInt32(timeString, dateTime.hour, 2);
        strcat(timeString, ":");
        strcatUInt32(timeString, dateTime.minute, 2);
    }

    const mr_rectangle_t titleRectangle = {
        STATUSBAR_X, STATUSBAR_Y,
        STATUSBAR_WIDTH / 2, STATUSBAR_HEIGHT};

    const mr_point_t titleOffset = {
        FONT_SMALL_CAP_HEIGHT / 2, 0};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawText(timeString,
             &titleRectangle,
             &titleOffset);

    // Symbols

    int8_t batteryLevel = getDeviceBatteryLevel();

    char symbolsString[4] = "";
    uint32_t bufferIndex = 0;

    if (settings.datalogInterval != DATALOGGING_OFF)
        symbolsString[bufferIndex++] = '8';
    if (settings.rateAlarm || settings.doseAlarm)
        symbolsString[bufferIndex++] = '7';
    symbolsString[bufferIndex++] =
        (batteryLevel == BATTERY_LEVEL_CHARGING) ? '6' : '0' + batteryLevel;
    symbolsString[bufferIndex] = '\0';

    const mr_rectangle_t symbolsRectangle = {
        STATUSBAR_X + STATUSBAR_WIDTH / 2, STATUSBAR_Y,
        STATUSBAR_WIDTH / 2, STATUSBAR_HEIGHT};

    const mr_point_t symbolsOffset = {
        DISPLAY_WIDTH / 2 - FONT_SMALL_CAP_HEIGHT / 2,
        (STATUSBAR_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2};

    setFont(FONT_SYMBOLS);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawRightAlignedText(symbolsString,
                         &symbolsRectangle,
                         &symbolsOffset);
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
        0, offset->y};

    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawText(unitString,
             &partialRectangle,
             &partialOffset);
}

void drawNotification(const char *title,
                      const char *subtitle,
                      bool fullscreen)
{
    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_CONTAINER_GLOBAL);

    // Top line

    if (fullscreen)
    {
        const mr_rectangle_t statusBarRectangle = {
            STATUSBAR_X, STATUSBAR_Y,
            STATUSBAR_WIDTH, STATUSBAR_HEIGHT};

        drawRectangle(&statusBarRectangle);
    }

    const mr_rectangle_t topRectangle = {
        0, STATUSBAR_Y + STATUSBAR_HEIGHT,
        DISPLAY_WIDTH, DISPLAY_HEIGHT / 2 - STATUSBAR_HEIGHT};

    const mr_point_t topOffset = {
        DISPLAY_WIDTH / 2,
        DISPLAY_HEIGHT / 2 -
            STATUSBAR_HEIGHT -
            FONT_MEDIUM_LINE_HEIGHT +
            NOTIFICATION_TOP_VERTICAL_OFFSET};

    drawCenteredText(title,
                     &topRectangle,
                     &topOffset);

    // Bottom line

    const mr_rectangle_t bottomRectangle = {
        0, DISPLAY_HEIGHT / 2,
        DISPLAY_WIDTH, DISPLAY_HEIGHT / 2};

    const mr_point_t bottomOffset = {
        DISPLAY_WIDTH / 2,
        NOTIFICATION_BOTTOM_VERTICAL_OFFSET};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawCenteredText(subtitle,
                     &bottomRectangle,
                     &bottomOffset);
}

void drawLowBattery(void)
{
    const mr_rectangle_t rectangle = {
        0, 0,
        DISPLAY_WIDTH, DISPLAY_HEIGHT};

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

static void drawTitle(const char *title)
{
    const mr_rectangle_t titleRectangle = {
        0, TITLE_Y,
        DISPLAY_WIDTH, TITLE_HEIGHT};

    const mr_point_t titleOffset = {
        DATA_X, TITLE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT};

    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);
    drawText(title,
             &titleRectangle,
             &titleOffset);
}

void drawMenu(const Menu *menu)
{
    const mr_rectangle_t titleRectangle = {
        0, MENU_TITLE_Y,
        DISPLAY_WIDTH, MENU_TITLE_HEIGHT};

    mr_point_t titleOffset = {
        DISPLAY_WIDTH / 2,
        (MENU_TITLE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_CONTAINER_BACKGROUND);
    drawCenteredText(menu->title,
                     &titleRectangle,
                     &titleOffset);

    setFont(FONT_MEDIUM);

    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    mr_rectangle_t rectangle;
    rectangle.height = MENU_LINE_HEIGHT;
    rectangle.y = MENU_LINES_TOP;

    const mr_point_t menuOptionOffset = {
        MENU_LINE_OFFSET_X,
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

        rectangle.x = DISPLAY_WIDTH;

        setFillColor((index == selectedIndex)
                         ? COLOR_FLAT_CHECKED_BACKGROUND
                         : COLOR_CONTAINER_GLOBAL);

        // Submenu

        if (menuStyle & MENUSTYLE_SUBMENU)
        {
            rectangle.width = MENU_SUBMENU_WIDTH + MENU_LINE_OFFSET_X;
            rectangle.x -= rectangle.width;

            setTextColor(COLOR_ON_FLAT_NEUTRAL);
            drawText("\x80",
                     &rectangle,
                     &menuGadgetOffset);
        }

        // Checked

        if (menuStyle & MENUSTYLE_CHECKED)
        {
            rectangle.width = MENU_CHECKED_WIDTH + MENU_LINE_OFFSET_X;
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

    rectangle.x = 0;
    rectangle.width = DISPLAY_WIDTH;
    rectangle.height = DISPLAY_HEIGHT - rectangle.y;

    setFillColor(COLOR_CONTAINER_GLOBAL);
    drawRectangle(&rectangle);
}

void drawTestMode(const char lines[8][32])
{
    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    setFillColor(COLOR_NORMAL_ENABLED_BACKGROUND);

    mr_rectangle_t rectangle;
    const mr_point_t offset = {0, 0};

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
        0, (4 * FONT_SMALL_LINE_HEIGHT),
        DISPLAY_WIDTH, DISPLAY_HEIGHT - (4 * FONT_SMALL_LINE_HEIGHT)};

    drawRectangle(&rectangle);
}

void drawMeasurement(const char *title,
                     const char *valueString,
                     const char *unitString,
                     float confidence,
                     const char *time,
                     const char *stateString,
                     const char *stateValueString,
                     const char *stateUnitString,
                     MeasurementStyle style)
{
    drawTitle(title);

    mr_color instrumentColor =
        (style == MEASUREMENTSTYLE_HOLD)
            ? COLOR_ELEMENT_ACTIVE
            : COLOR_INSTRUMENT_ENHANCED_PRIMARY;

    // Value

    const mr_rectangle_t valueRectangle = {
        MEASUREMENT_VALUE_X, MEASUREMENT_VALUE_Y,
        MEASUREMENT_VALUE_WIDTH, MEASUREMENT_VALUE_HEIGHT};

    const mr_point_t valueOffset = {
        MEASUREMENT_VALUE_OFFSET_X, MEASUREMENT_VALUE_OFFSET_Y};

    setFont(FONT_LARGE);
    setTextColor((style == MEASUREMENTSTYLE_ALARM)
                     ? COLOR_ALARM
                     : instrumentColor);
    drawText(valueString,
             &valueRectangle,
             &valueOffset);

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
        strcpy(confidenceString, "");
        strcpy(confidenceUnit, "");
    }

    const mr_rectangle_t confidenceRectangle = {
        MEASUREMENT_CONFIDENCE_X, MEASUREMENT_CONFIDENCE_Y,
        MEASUREMENT_CONFIDENCE_WIDTH, MEASUREMENT_CONFIDENCE_HEIGHT};

    mr_point_t confidenceOffset = {
        MEASUREMENT_CONFIDENCE_OFFSET_X,
        MEASUREMENT_CONFIDENCE_OFFSET_Y};

    setFont(FONT_SMALL);

#if defined(DISPLAY_240X320)
    confidenceOffset.x -= getTextWidth(confidenceString) +
                          getTextWidth(confidenceUnit);
#endif

    drawValueAndUnit(confidenceString,
                     confidenceUnit,
                     &confidenceRectangle,
                     &confidenceOffset,
                     style == MEASUREMENTSTYLE_HOLD);

    // Unit

    const mr_rectangle_t unitRectangle = {
        MEASUREMENT_UNIT_X, MEASUREMENT_UNIT_Y,
        MEASUREMENT_UNIT_WIDTH, MEASUREMENT_UNIT_HEIGHT};

    const mr_point_t unitOffset = {
        MEASUREMENT_UNIT_OFFSET_X, MEASUREMENT_UNIT_OFFSET_Y};

    setFont(FONT_MEDIUM);

#if defined(DISPLAY_240X320)
    drawRightAlignedText(unitString,
                         &unitRectangle,
                         &unitOffset);
#else
    drawText(unitString,
             &unitRectangle,
             &unitOffset);
#endif

    // Time

    const mr_rectangle_t timeRectangle = {
        MEASUREMENT_TIME_X, MEASUREMENT_TIME_Y,
        MEASUREMENT_TIME_WIDTH, MEASUREMENT_TIME_HEIGHT};

    const mr_point_t timeOffset = {
        MEASUREMENT_TIME_OFFSET_X, MEASUREMENT_TIME_OFFSET_Y};

    setFont(FONT_SMALL);
    drawText("Time",
             &timeRectangle,
             &timeOffset);

    // Time value

    const mr_rectangle_t timeValueRectangle = {
        MEASUREMENT_TIME_VALUE_X, MEASUREMENT_TIME_VALUE_Y,
        MEASUREMENT_TIME_VALUE_WIDTH, MEASUREMENT_TIME_VALUE_HEIGHT};

    const mr_point_t timeValueOffset = {
        MEASUREMENT_TIME_VALUE_OFFSET_X, MEASUREMENT_TIME_VALUE_OFFSET_Y};

    setTextColor((style == MEASUREMENTSTYLE_HOLD)
                     ? COLOR_ELEMENT_ACTIVE
                     : COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawText(time,
             &timeValueRectangle,
             &timeValueOffset);

    // State

    const mr_rectangle_t stateRectangle = {
        MEASUREMENT_STATE_X, MEASUREMENT_STATE_Y,
        MEASUREMENT_STATE_WIDTH, MEASUREMENT_STATE_HEIGHT};

    const mr_point_t stateOffset = {
        MEASUREMENT_STATE_OFFSET_X, MEASUREMENT_STATE_OFFSET_Y};

    setTextColor(COLOR_ELEMENT_NEUTRAL);
    drawText(stateString,
             &stateRectangle,
             &stateOffset);

    // State value

    const mr_rectangle_t stateValueRectangle = {
        MEASUREMENT_STATE_VALUE_X, MEASUREMENT_STATE_VALUE_Y,
        MEASUREMENT_STATE_VALUE_WIDTH, MEASUREMENT_STATE_VALUE_HEIGHT};

    mr_point_t stateValueOffset = {
        MEASUREMENT_STATE_VALUE_OFFSET_X,
        MEASUREMENT_STATE_VALUE_OFFSET_Y};

    drawValueAndUnit(stateValueString,
                     stateUnitString,
                     &stateValueRectangle,
                     &stateValueOffset,
                     style == MEASUREMENTSTYLE_HOLD);
}

void drawHistory(const char *title,
                 const char *topLegendString,
                 const char *bottomLegendString,
                 const uint8_t *data,
                 uint32_t xTickNum,
                 uint32_t yTickNum)
{
    drawTitle(title);

    // Legends

    const mr_rectangle_t topLegendRectangle = {
        HISTORY_TOP_LEGEND_X, HISTORY_TOP_LEGEND_Y,
        HISTORY_TOP_LEGEND_WIDTH, HISTORY_TOP_LEGEND_HEIGHT};

    const mr_point_t topLegendOffset = {
        HISTORY_TOP_LEGEND_OFFSET_X, HISTORY_TOP_LEGEND_OFFSET_Y};

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawRightAlignedText(topLegendString,
                         &topLegendRectangle,
                         &topLegendOffset);

    const mr_rectangle_t bottomLegendRectangle = {
        HISTORY_BOTTOM_LEGEND_X, HISTORY_BOTTOM_LEGEND_Y,
        HISTORY_BOTTOM_LEGEND_WIDTH, HISTORY_BOTTOM_LEGEND_HEIGHT};

    const mr_point_t bottomLegendOffset = {
        HISTORY_BOTTOM_LEGEND_OFFSET_X, HISTORY_BOTTOM_LEGEND_OFFSET_Y};

    drawRightAlignedText(bottomLegendString,
                         &bottomLegendRectangle,
                         &bottomLegendOffset);

    // Space

    const mr_rectangle_t leftSpaceRectangle = {
        HISTORY_SPACE_LEFT_X, HISTORY_SPACE_LEFT_Y,
        HISTORY_SPACE_LEFT_WIDTH, HISTORY_SPACE_LEFT_HEIGHT};

    drawRectangle(&leftSpaceRectangle);

    const mr_rectangle_t rightSpaceRectangle = {
        HISTORY_SPACE_RIGHT_X, HISTORY_SPACE_RIGHT_Y,
        HISTORY_SPACE_RIGHT_WIDTH, HISTORY_SPACE_RIGHT_HEIGHT};

    drawRectangle(&rightSpaceRectangle);

    // y-tick table

    uint8_t yTickTable[32];
    uint32_t yTickTableIndex = 0;

    yTickTable[yTickTableIndex++] = 1;

    for (uint32_t yTickIndex = 1;
         yTickIndex <= yTickNum;
         yTickIndex++)
    {
        int16_t yTickNext = yTickIndex * (HISTORY_HEIGHT - 1) / yTickNum;

        yTickTable[yTickTableIndex++] = yTickNext;
        yTickTable[yTickTableIndex++] = yTickNext + 1;
    }

    // History

    setFillColor(COLOR_INSTRUMENT_FRAME_TERTIARY);

    int16_t xTickIndex = 0;
    int16_t xTickNext = 0;

    int16_t yDataLast = (HISTORY_HEIGHT - 1);

    for (int16_t x = 0; x < HISTORY_WIDTH; x++)
    {
        // x tick

        bool xTickDraw = false;
        if (x == xTickNext)
        {
            xTickIndex++;
            xTickNext = xTickIndex * (HISTORY_WIDTH - 1) / xTickNum;
            xTickDraw = true;
        }

        // y tick

        yTickTableIndex = 0;

        // Data

        int16_t yData;
        if (x < (HISTORY_WIDTH - 1))
            yData = 1 + (((HISTORY_HEIGHT - 1) * (255 - data[x + 1])) >> 8);
        else
            yData = HISTORY_HEIGHT - 1;

        int16_t yDataTop;
        int16_t yDataBottom;
        if (yData < yDataLast)
        {
            yDataTop = yData;
            yDataBottom = yDataLast;
        }
        else
        {
            yDataTop = yDataLast;
            yDataBottom = yData;
        }

        if (yDataBottom < (HISTORY_HEIGHT - 1))
            yDataBottom++;
        yDataLast = yData;

        // Drawing

        mr_rectangle_t rectangle;
        rectangle.x = HISTORY_X + x;
        rectangle.width = 1;

        int16_t y = 0;
        while (y < HISTORY_HEIGHT)
        {
            mr_color color = COLOR_INSTRUMENT_FRAME_PRIMARY;

            // x and y tick

            int16_t yTickNext;
            if (xTickDraw)
            {
                yTickNext = HISTORY_HEIGHT;

                color = COLOR_INSTRUMENT_FRAME_TERTIARY;
            }
            else
            {
                yTickNext = yTickTable[yTickTableIndex];

                if (!(yTickTableIndex & 0b1))
                    color = COLOR_INSTRUMENT_FRAME_TERTIARY;

                if (y == yTickNext)
                    yTickTableIndex++;
            }

            int16_t yDataNext;
            if ((x == 0) ||
                (x == (HISTORY_WIDTH - 1)) ||
                (y == (HISTORY_HEIGHT - 1)))
                yDataNext = HISTORY_HEIGHT;
            else if (y < yDataTop)
                yDataNext = yDataTop;
            else if (y < yDataBottom)
            {
                yDataNext = yDataBottom;

                color = COLOR_INSTRUMENT_ENHANCED_PRIMARY;
            }
            else
            {
                yDataNext = (HISTORY_HEIGHT - 1);

                if (color == COLOR_INSTRUMENT_FRAME_PRIMARY)
                    color = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_PRIMARY;
                else
                    color = COLOR_INSTRUMENT_ENHANCED_TERTIARY_OVER_TERTIARY;
            }

            int16_t yNext = yTickNext < yDataNext
                                ? yTickNext
                                : yDataNext;

            rectangle.y = HISTORY_Y + y;
            rectangle.height = yNext - y;

            setFillColor(color);
            drawRectangle(&rectangle);

            y += rectangle.height;
        }
    }
}

void drawRNG(const char *title,
             const char *rngString,
             const char *stateString)
{
    drawTitle(title);

    // RNG string

    const mr_rectangle_t rngStringRectangle = {
        RNG_STRING_X, RNG_STRING_Y,
        RNG_STRING_WIDTH, RNG_STRING_HEIGHT};

    const mr_point_t rngStringOffset = {
        RNG_STRING_OFFSET_X, RNG_STRING_OFFSET_Y};

    setFont(FONT_SMALL);
    setTextColor(COLOR_INSTRUMENT_ENHANCED_PRIMARY);
    drawText(rngString,
             &rngStringRectangle,
             &rngStringOffset);

    // State

    const mr_rectangle_t rngStateRectangle = {
        RNG_STATE_X, RNG_STATE_Y,
        RNG_STATE_WIDTH, RNG_STATE_HEIGHT};

    const mr_point_t rngStateOffset = {
        DISPLAY_WIDTH / 2, 0};

    setFont(FONT_MEDIUM);
    setTextColor(COLOR_ELEMENT_ACTIVE);
    drawCenteredText(stateString,
                     &rngStateRectangle,
                     &rngStateOffset);
}

void drawStatistics(void)
{
    drawTitle("Statistics");

    setFont(FONT_SMALL);

    char key[32];
    char valueString[32];
    char unitString[16];

    mr_rectangle_t rectangle;
    mr_point_t offset;

    uint32_t y = STATISTICS_Y;

    for (uint32_t i = 0; i < STATISTICS_NUM; i++)
    {
        strcpy(key, "");
        strcpy(valueString, "");
        strcpy(unitString, "");

        switch (i)
        {
#if defined(DISPLAY_240X320)
        case 1:
            strcpy(key, "Tube");

            break;

        case 6:
            strcpy(key, "Device");

            break;
#endif

#if defined(DISPLAY_128X64)
        case 0:
            strcpy(key, "Tube life time");
#elif defined(DISPLAY_320X240)
        case 1:
            strcpy(key, "Tube life time");
#elif defined(DISPLAY_240X320)
        case 2:
            strcpy(key, "Life time");
#endif
            strcatTime(valueString, getTubeTime());

            break;

#if defined(DISPLAY_128X64)
        case 1:
            strcpy(key, "Tube life pulses");
#elif defined(DISPLAY_320X240)
        case 2:
            strcpy(key, "Tube life pulses");
#elif defined(DISPLAY_240X320)
        case 3:
            strcpy(key, "Life pulses");
#endif
            strcatUInt32(valueString, getTubePulseCount(), 0);

            break;

#if defined(DISPLAY_128X64)
        case 2:
            strcpy(key, "Tube dead-time");
#elif defined(DISPLAY_320X240)
        case 3:
            strcpy(key, "Tube dead-time");
#elif defined(DISPLAY_240X320)
        case 4:
            strcpy(key, "Dead-time");
#endif
            float deadTime = getTubeDeadTime();
            if (deadTime >= 1)
                strcpy(valueString, "-");
            else
            {
                strcat(valueString, "< ");
                strcatFloat(valueString, 1000000 * deadTime, 1);
                strcpy(unitString, " \xb5"
                                   "s");
            }

            break;

#if defined(DISPLAY_128X64)
        case 3:
            strcpy(key, "Device ID");
#elif defined(DISPLAY_320X240)
        case 5:
            strcpy(key, "Device ID");
#elif defined(DISPLAY_240X320)
        case 7:
            strcpy(key, "ID");
#endif
            strcatUInt32Hex(unitString, getDeviceId());

            break;

#if defined(DISPLAY_128X64)
        case 4:
            strcpy(key, "Device temp.");
#elif defined(DISPLAY_320X240)
        case 6:
            strcpy(key, "Device temp.");
#elif defined(DISPLAY_240X320)
        case 8:
            strcpy(key, "Temp.");
#endif
            strcatFloat(valueString, getDeviceTemperature(), 1);
            strcpy(unitString, " \xb0"
                               "C");

            break;
        };

        // Key

        rectangle = (mr_rectangle_t){
            STATISTICS_X, y,
            STATISTICS_WIDTH, STATISTICS_HEIGHT};

        offset = (mr_point_t){
            STATISTICS_OFFSET_X, 0};

        if (!valueString[0] && !unitString[0])
            setTextColor(COLOR_ELEMENT_ACTIVE);
        else
            setTextColor(COLOR_ELEMENT_NEUTRAL);

        drawText(key,
                 &rectangle,
                 &offset);

        // Value & unit

        rectangle = (mr_rectangle_t){
            STATISTICS_VALUE_X, y,
            STATISTICS_VALUE_WIDTH, STATISTICS_VALUE_HEIGHT};

        offset = (mr_point_t){
            0, 0};

        drawValueAndUnit(valueString,
                         unitString,
                         &rectangle,
                         &offset,
                         false);

        y += FONT_SMALL_LINE_HEIGHT;
    }

    rectangle = (mr_rectangle_t){
        0, y,
        DISPLAY_WIDTH, DISPLAY_HEIGHT - y};

    drawRectangle(&rectangle);
}

void drawGame(const uint8_t board[8][8],
              const char time[2][16],
              const char history[3][2][6])
{
    mr_rectangle_t rectangle;
    mr_point_t offset;

    // Board

    static const char *const gamePieceMap = "@AACFBDE";

    setFont(FONT_GAME);

    rectangle.width = GAME_SQUARE_WIDTH;
    rectangle.height = GAME_SQUARE_HEIGHT;

    const mr_point_t gamePieceOffset = {
        0, 0};

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

    // Top timers

    setFont(FONT_SMALL);
    setTextColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);

    const mr_rectangle_t topTimerRectangle = {
        GAME_TOP_TIMER_X, GAME_TOP_TIMER_Y,
        GAME_TOP_TIMER_WIDTH, GAME_TOP_TIMER_HEIGHT};

    const mr_point_t topTimerOffset = {
        GAME_TOP_TIMER_OFFSET_X, GAME_TOP_TIMER_OFFSET_Y};

#if defined(GAME_TIMERS_RIGHTALIGNED)
    drawRightAlignedText(time[0],
                         &topTimerRectangle,
                         &topTimerOffset);
#else
    drawText(time[0],
             &topTimerRectangle,
             &topTimerOffset);
#endif

    // History

    rectangle.y = GAME_HISTORY_Y;
    rectangle.height = FONT_SMALL_LINE_HEIGHT;

    offset = (mr_point_t){0, 0};

    for (uint32_t y = 0; y < 3; y++)
    {
        rectangle.x = GAME_HISTORY_X;
        rectangle.width = GAME_HISTORY_SPACE_LEFT_WIDTH;
        offset.x = GAME_HISTORY_SPACE_LEFT_OFFSET_X;

        drawText(history[y][0],
                 &rectangle,
                 &offset);

        rectangle.x += GAME_HISTORY_SPACE_LEFT_WIDTH;
        rectangle.width = GAME_HISTORY_SPACE_RIGHT_WIDTH;
        offset.x = 0;

        drawText(history[y][1],
                 &rectangle,
                 &offset);

        rectangle.y += FONT_SMALL_LINE_HEIGHT;
    }

    // Bottom timer

    const mr_rectangle_t bottomTimerRectangle = {
        GAME_BOTTOM_TIMER_X, GAME_BOTTOM_TIMER_Y,
        GAME_BOTTOM_TIMER_WIDTH, GAME_BOTTOM_TIMER_HEIGHT};

    const mr_point_t bottomTimerOffset = {
        GAME_BOTTOM_TIMER_OFFSET_X, GAME_BOTTOM_TIMER_OFFSET_Y};

#if defined(GAME_TIMERS_RIGHTALIGNED)
    drawRightAlignedText(time[1],
                         &bottomTimerRectangle,
                         &bottomTimerOffset);
#else
    drawText(time[1],
             &bottomTimerRectangle,
             &bottomTimerOffset);
#endif

    // Spaces

    const mr_rectangle_t gameSpaceRectangle = {
        GAME_SPACE_X, GAME_SPACE_Y,
        GAME_SPACE_WIDTH, GAME_SPACE_HEIGHT};

    drawRectangle(&gameSpaceRectangle);

#if defined(GAME_SPACE2_X)
    const mr_rectangle_t gameSpace2Rectangle = {
        GAME_SPACE2_X, GAME_SPACE2_Y,
        GAME_SPACE2_WIDTH, GAME_SPACE2_HEIGHT};

    drawRectangle(&gameSpace2Rectangle);
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
    {"Brightness level", &displayBrightnessMenuView},
#if defined(DISPLAY_MONOCHROME)
    {"Backlight", &displaySleepMenuView},
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
    "Low",
    "Medium",
    "High",
    "Very high",
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

    setDisplayBacklight(true);
}

static MenuState displayBrightnessMenuState;

static const Menu displayBrightnessMenu = {
    "Brightness level",
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
    "Off",
#endif
#if defined(DISPLAY_240X320)
    "On for 30 sec.",
#else
    "On for 30 seconds",
#endif
    "On for 1 minute",
    "On for 2 minutes",
    "On for 5 minutes",
#if defined(DISPLAY_MONOCHROME)
    "Pulse flashes",
#endif
    "Always on",
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
    "Backlight",
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
