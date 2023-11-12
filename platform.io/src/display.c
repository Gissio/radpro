/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include <clib/u8g2.h>

#include "adc.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"

#define CENTER_TEXT(box_height, text_height) ((box_height + text_height) / 2)

#if defined(DISPLAY_128X64)

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#include "resources/font_tiny5.h"
#include "resources/robotoR8.h"
#include "resources/robotoR24_digits.h"
#include "resources/icons5.h"
#include "resources/chess8.h"

#define FONT_SMALL font_tiny5
#define FONT_SMALL_HEIGHT 5
#define FONT_SMALL_BASELINE 6
#define FONT_SMALL_BOTTOM 8

#define FONT_MEDIUM font_robotoR8
#define FONT_MEDIUM_HEIGHT 8
#define FONT_MEDIUM_BASELINE 10
#define FONT_MEDIUM_BOTTOM 12

#define FONT_LARGE font_robotoR24
#define FONT_LARGE_HEIGHT 24
#define FONT_LARGE_BASELINE 28
#define FONT_LARGE_BOTTOM 35

#define FONT_ICONS font_icons5
#define FONT_ICONS_SPACE 2

#define FONT_CHESS font_chess8

#define STATUS_BAR_HEIGHT 7

#define TITLE_BAR_HEIGHT 7

#define MENU_LINE_NUM 4
#define MENU_LINE_HEIGHT FONT_MEDIUM_BOTTOM
#define MENU_LEFT (FONT_MEDIUM_HEIGHT / 4)

#define HISTORY_VIEW_HEIGHT 42
#define HISTORY_VIEW_X ((DISPLAY_WIDTH - HISTORY_VIEW_WIDTH) / 2)
#define HISTORY_VIEW_Y_TOP 14
#define HISTORY_VIEW_Y_BOTTOM (HISTORY_VIEW_Y_TOP + HISTORY_VIEW_HEIGHT)

#define STATISTICS_OFFSET (FONT_SMALL_BOTTOM / 2)

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0

#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8

#define GAME_MOVE_CELL_HEIGHT (FONT_SMALL_BOTTOM - 1)

#elif defined(DISPLAY_320X240)

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

#include "resources/robotoR12.h"
#include "resources/robotoR18.h"
#include "resources/robotoR56_digits.h"
#include "resources/icons12.h"
#include "resources/chess26.h"

#define FONT_SMALL font_robotoR12
#define FONT_SMALL_HEIGHT 12
#define FONT_SMALL_BASELINE 14
#define FONT_SMALL_BOTTOM 18

#define FONT_MEDIUM font_robotoR18
#define FONT_MEDIUM_HEIGHT 18
#define FONT_MEDIUM_BASELINE 22
#define FONT_MEDIUM_BOTTOM 27

#define FONT_LARGE font_robotoR56
#define FONT_LARGE_HEIGHT 56
#define FONT_LARGE_BASELINE 62
#define FONT_LARGE_BOTTOM 78

#define FONT_ICONS font_icons12
#define FONT_ICONS_SPACE 6

#define FONT_CHESS font_chess26

#define STATUS_BAR_HEIGHT 16

#define TITLE_BAR_HEIGHT 32

#define MENU_LINE_NUM 6
#define MENU_LINE_HEIGHT 32
#define MENU_LEFT (FONT_MEDIUM_HEIGHT / 2)

#define HISTORY_VIEW_HEIGHT 40
#define HISTORY_VIEW_X ((DISPLAY_WIDTH - HISTORY_VIEW_WIDTH) / 2)
#define HISTORY_VIEW_Y_TOP 14
#define HISTORY_VIEW_Y_BOTTOM (HISTORY_VIEW_Y_TOP + HISTORY_VIEW_HEIGHT)

#define STATISTICS_OFFSET 0

#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP STATUS_BAR_HEIGHT

#define GAME_SQUARE_WIDTH 26
#define GAME_SQUARE_HEIGHT 26

#define GAME_MOVE_CELL_HEIGHT FONT_SMALL_BOTTOM

#endif

#define DISPLAY_CENTER_LEFT (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_TOP (DISPLAY_HEIGHT / 2)

#define CONTENT_TOP (STATUS_BAR_HEIGHT + TITLE_BAR_HEIGHT)

#define MEASUREMENT_RIGHT (57 * FONT_SMALL_HEIGHT / 10)
#define MEASUREMENT_WIDTH (168 * FONT_SMALL_HEIGHT / 10)

#define STATISTICS_TOP (DISPLAY_CENTER_TOP - (11 * FONT_SMALL_HEIGHT / 2) / 2 + STATISTICS_OFFSET)

#define GAME_BOARD_RIGHT (GAME_BOARD_LEFT + 8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_BOTTOM (GAME_BOARD_TOP + 8 * GAME_SQUARE_HEIGHT)

#define GAME_MOVE_CELL_WIDTH (11 * FONT_SMALL_HEIGHT / 3)
#define GAME_MOVE_CELL_MARGIN ((DISPLAY_WIDTH - GAME_BOARD_RIGHT - 2 * GAME_MOVE_CELL_WIDTH) / 3)
#define GAME_MOVE_CELL_LEFT (GAME_BOARD_RIGHT + GAME_MOVE_CELL_MARGIN)
#define GAME_MOVE_CELL_TOP (DISPLAY_CENTER_TOP - 5 * GAME_MOVE_CELL_HEIGHT / 2)

#define GAME_BUTTON_WIDTH (9 * FONT_SMALL_HEIGHT / 2)
#define GAME_BUTTON_HEIGHT (2 * FONT_SMALL_HEIGHT - 1)
#define GAME_BUTTON_LEFT (DISPLAY_WIDTH - GAME_BUTTON_WIDTH - FONT_SMALL_HEIGHT / 2)
#define GAME_BUTTON_TOP (DISPLAY_HEIGHT - GAME_BUTTON_HEIGHT - FONT_SMALL_HEIGHT / 2)
#define GAME_BUTTON_BORDER (FONT_SMALL_HEIGHT / 5)

const uint32_t menuLineNum = MENU_LINE_NUM;

#if defined(PULSE_LED)

static const struct Menu pulseLEDMenu;

#endif

u8g2_t displayU8g2;

#if defined(DISPLAY_MONO)

static const struct Menu backlightMenu;

#elif defined(DISPLAY_COLOR)

static const struct Menu displayThemeMenu;
static const struct Menu displayBrightnessLevelMenu;
static const struct Menu displaySleepMenu;

#endif

void initDisplay(void)
{
    initDisplayHardware();

#if defined(PULSE_LED)

    selectMenuIndex(&pulseLEDMenu, settings.pulseLED, PULSE_LED_NUM);

#endif

#if defined(DISPLAY_MONO)

    selectMenuIndex(&backlightMenu, settings.displayBacklight, DISPLAY_BACKLIGHT_NUM);

#elif defined(DISPLAY_COLOR)

    selectMenuIndex(&displayThemeMenu, settings.displayTheme, DISPLAY_THEME_NUM);
    selectMenuIndex(&displayBrightnessLevelMenu, settings.displayBrightnessLevel, DISPLAY_BRIGHTNESS_NUM);
    selectMenuIndex(&displaySleepMenu, settings.displaySleep, DISPLAY_SLEEP_NUM);

#endif
}

// u8G2 interface

#if defined(DISPLAY_MONO)

static void setDrawColor(bool value)
{
    u8g2_SetDrawColor(&displayU8g2, value);
}

static void drawBox(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    u8g2_DrawBox(&displayU8g2,
                 x,
                 y,
                 w,
                 h);
}

void drawTextLeft(const char *str,
                  const uint8_t *font,
                  uint32_t x, uint32_t y)
{
    u8g2_SetFont(&displayU8g2, font);
    u8g2_DrawStr(&displayU8g2, x, y, str);
}

void clearDisplayBuffer(void)
{
    u8g2_ClearBuffer(&displayU8g2);
}

void sendDisplayBuffer(void)
{
    u8g2_SendBuffer(&displayU8g2);
}

#elif defined(DISPLAY_COLOR)

static void setDrawColor(uint16_t value)
{
    u8g2_SetDrawColor(&displayU8g2, value);
}

static void drawBox(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    // color_DrawBox()

    u8g2_DrawBox(&displayU8g2,
                 x,
                 y,
                 w,
                 h);
}

void drawTextLeft(const char *str,
                  const uint8_t *font,
                  uint32_t x, uint32_t y)
{
    u8g2_SetFont(&displayU8g2, font);
    u8g2_DrawStr(&displayU8g2, x, y, str);
}

void clearDisplayBuffer(void)
{
    u8g2_ClearBuffer(&displayU8g2);
}

void sendDisplayBuffer(void)
{
    u8g2_SendBuffer(&displayU8g2);
}

#endif

static void drawTextCenter(const char *str,
                           const uint8_t *font,
                           uint32_t x, uint32_t y)
{
    u8g2_SetFont(&displayU8g2, font);
    u8g2_DrawStr(&displayU8g2, x - u8g2_GetStrWidth(&displayU8g2, str) / 2, y, str);
    
    // drawTextLeft(str,
    //              font,
    //              x - u8g2_GetStrWidth(&displayU8g2, str) / 2,
    //              y);
}

static void drawTextRight(const char *str,
                          const uint8_t *font,
                          uint32_t x, uint32_t y)
{
    u8g2_SetFont(&displayU8g2, font);
    u8g2_DrawStr(&displayU8g2, x - u8g2_GetStrWidth(&displayU8g2, str), y, str);

    // drawTextRight(str,
    //               font,
    //               x - u8g2_GetStrWidth(&displayU8g2, str),
    //               y);
}

// Common

void drawLowBattery(void)
{
    drawTextCenter("0",
                   FONT_ICONS,
                   DISPLAY_CENTER_LEFT,
                   DISPLAY_CENTER_TOP + FONT_SMALL_HEIGHT / 2);
}

void drawWelcome(void)
{
    drawTextCenter("Rad Pro",
                   FONT_MEDIUM,
                   DISPLAY_CENTER_LEFT,
                   DISPLAY_CENTER_TOP - FONT_SMALL_BOTTOM / 2);

    drawTextCenter(FIRMWARE_VERSION,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   DISPLAY_CENTER_TOP + FONT_MEDIUM_HEIGHT + FONT_SMALL_BOTTOM / 2);
}

void drawStatusBar(void)
{
    // Date and time

    struct RTCDateTime dateTime;
    getRTCDateTime(&dateTime);

    if (dateTime.year >= RTC_YEAR_MIN)
    {
        char buffer[6] = "";

        strcatUInt32(buffer, dateTime.hour, 2);
        strcat(buffer, ":");
        strcatUInt32(buffer, dateTime.minute, 2);

        drawTextLeft(buffer,
                     FONT_SMALL,
                     FONT_SMALL_HEIGHT / 2,
                     CENTER_TEXT(STATUS_BAR_HEIGHT, FONT_SMALL_HEIGHT));
    }

    // Icons

    int8_t batteryLevel = getBatteryLevel();

    char buffer[3] = "";
    uint32_t bufferIndex = 0;

    if (settings.rateAlarm || settings.doseAlarm)
        buffer[bufferIndex++] = ';';
    buffer[bufferIndex++] = (batteryLevel == BATTERY_LEVEL_CHARGING) ? ':' : '0' + batteryLevel;
    buffer[bufferIndex] = '\0';

    drawTextRight(buffer,
                  FONT_ICONS,
                  DISPLAY_WIDTH - FONT_SMALL_HEIGHT / 2 + FONT_ICONS_SPACE,
                  CENTER_TEXT(STATUS_BAR_HEIGHT, FONT_SMALL_HEIGHT));

    // +++ TEST
    // char buffer2[16];
    // strcpy(buffer2, "ADC=");
    // strcatUInt32(buffer2, readBatteryValue(), 0);
    // drawTextCenter(buffer2, FONT_SMALL, DISPLAY_CENTER_LEFT, FONT_SMALL_BASELINE);
    // +++ TEST
}

void drawMenu(const struct Menu *menu)
{
    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    for (uint32_t i = 0, y = CONTENT_TOP;
         i < MENU_LINE_NUM;
         i++, y += MENU_LINE_HEIGHT)
    {
        uint32_t index = startIndex + i;

        const char *name = menu->onGetOption(menu, index);
        if (!name)
            break;

        bool selected = (index == selectedIndex);

        setDrawColor(selected);
        drawBox(0, y, DISPLAY_WIDTH, MENU_LINE_HEIGHT);

        setDrawColor(!selected);
        drawTextLeft(name,
                     FONT_MEDIUM,
                     MENU_LEFT,
                     y + FONT_MEDIUM_BASELINE + (MENU_LINE_HEIGHT - FONT_MEDIUM_BOTTOM) / 2);
    }

    setDrawColor(true);
}

void drawTitle(const char *title)
{
    drawTextCenter(title,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATUS_BAR_HEIGHT + CENTER_TEXT(TITLE_BAR_HEIGHT, FONT_SMALL_HEIGHT));
}

void drawSubtitle(const char *subtitle)
{
    drawTextCenter(subtitle,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   DISPLAY_HEIGHT - STATUS_BAR_HEIGHT - TITLE_BAR_HEIGHT + CENTER_TEXT(TITLE_BAR_HEIGHT, FONT_SMALL_HEIGHT));
}

void drawMeasurementValue(const char *value, const char *units)
{
    // Value

    drawTextLeft(value,
                 FONT_LARGE,
                 DISPLAY_CENTER_LEFT + MEASUREMENT_RIGHT - MEASUREMENT_WIDTH,
                 DISPLAY_CENTER_TOP + FONT_LARGE_HEIGHT / 2);

    // Units

    drawTextLeft(units,
                 FONT_MEDIUM,
                 DISPLAY_CENTER_LEFT + MEASUREMENT_RIGHT,
                 DISPLAY_CENTER_TOP - FONT_LARGE_HEIGHT / 2 + FONT_MEDIUM_HEIGHT);
}

void drawConfidenceIntervals(float lowerConfidenceInterval,
                             float upperConfidenceInterval)
{
    char confidenceInterval[16];

    strcpy(confidenceInterval, "+");
    strcatFloat(confidenceInterval,
                100 * upperConfidenceInterval,
                upperConfidenceInterval < 0.1F ? 1 : 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval,
                 FONT_SMALL,
                 DISPLAY_CENTER_LEFT + MEASUREMENT_RIGHT,
                 DISPLAY_CENTER_TOP + FONT_LARGE_HEIGHT / 2 - FONT_SMALL_BOTTOM);

    strcpy(confidenceInterval, "-");
    strcatFloat(confidenceInterval,
                -100 * lowerConfidenceInterval,
                lowerConfidenceInterval > -0.1F ? 1 : 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval,
                 FONT_SMALL,
                 DISPLAY_CENTER_LEFT + MEASUREMENT_RIGHT,
                 DISPLAY_CENTER_TOP + FONT_LARGE_HEIGHT / 2);
}

void drawHistory(const char *minLabel, const char *maxLabel,
                 int32_t minLimit, uint32_t decades)
{
    // Plot separators

    drawBox(0, CONTENT_TOP, DISPLAY_WIDTH, 1);
    drawBox(0, HISTORY_VIEW_Y_BOTTOM, DISPLAY_WIDTH, 1);

    // Data

    // uint32_t lastX = DISPLAY_WIDTH;
    // uint32_t lastY = data[0];
    for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        uint32_t value = getHistoryDataPoint(i);
        if (value == 0)
            continue;

        uint32_t x = (DISPLAY_WIDTH - 1) - i * DISPLAY_WIDTH / HISTORY_BUFFER_SIZE;
        uint32_t y = (value - minLimit) * HISTORY_VALUE_DECADE / (HISTORY_VIEW_HEIGHT * decades);

        // Pixel
        // drawBox(x, HISTORY_VIEW_Y_BOTTOM - y, 1, 1);

        // Vertical bar
        drawBox(x, HISTORY_VIEW_Y_BOTTOM - y, 1, y);

        // Line
        // drawLine(x, HISTORY_VIEW_Y_BOTTOM - y, lastX, HISTORY_VIEW_Y_BOTTOM - lastY);
        // lastX = x;
        // lastY = y;
    }

    // Time divisors

    for (uint32_t i = 1; i < 8; i++)
    {
        uint32_t x = i * DISPLAY_WIDTH / 8;

        setDrawColor(false);
        drawBox(x, HISTORY_VIEW_Y_TOP + 1, 1, HISTORY_VIEW_HEIGHT - 1);

        setDrawColor(true);
        for (uint32_t y = HISTORY_VIEW_Y_TOP + 3;
             y < HISTORY_VIEW_Y_TOP + HISTORY_VIEW_HEIGHT;
             y += 4)
            drawBox(x, y, 1, 1);
    }

    // Rate divisors

    for (uint32_t i = 1; i < decades; i++)
    {
        uint32_t y = HISTORY_VIEW_Y_TOP + i * HISTORY_VIEW_HEIGHT / decades;

        setDrawColor(false);
        drawBox(0, y, DISPLAY_WIDTH, 1);

        setDrawColor(true);
        for (uint32_t x = 2; x < DISPLAY_WIDTH; x += 4)
            drawBox(x, y, 1, 1);
    }

    drawTextLeft(minLabel,
                 FONT_SMALL,
                 1,
                 HISTORY_VIEW_Y_BOTTOM + 1 + 6);
    drawTextLeft(maxLabel,
                 FONT_SMALL,
                 1,
                 HISTORY_VIEW_Y_TOP - 1);
}

void drawRNGText(char *str)
{
    drawTextLeft(str,
                 FONT_MEDIUM,
                 1,
                 CENTER_TEXT(DISPLAY_HEIGHT, FONT_MEDIUM_HEIGHT));
}

void drawStatistics(void)
{
    char line[64];

    strcpy(line, "Tube life time: ");
    strcatTime(line, getTubeTime());
    drawTextCenter(line,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATISTICS_TOP + (0 * FONT_SMALL_BOTTOM) / 2);

    strcpy(line, "Tube life pulse count: ");
    strcatUInt32(line, getTubePulseCount(), 0);
    drawTextCenter(line,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATISTICS_TOP + (2 * FONT_SMALL_BOTTOM) / 2);

    strcpy(line, "Tube dead-time: ");
    float deadTime = getDeadTime();
    if (deadTime > 100)
        strcat(line, "-");
    else
    {
        strcat(line, "< ");
        strcatFloat(line, 1000000 * deadTime, 1);
        strcat(line, " \xb5"
                     "s");
    }
    drawTextCenter(line,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATISTICS_TOP + (4 * FONT_SMALL_BOTTOM) / 2);

    strcpy(line, "Device ID: ");
    strcatUInt32Hex(line, getDeviceId());
    drawTextCenter(line,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATISTICS_TOP + (7 * FONT_SMALL_BOTTOM) / 2);

    strcpy(line, "Device temperature: ");
    strcatFloat(line, getDeviceTemperature(), 1);
    strcat(line, " \xb0"
                 "C");
    drawTextCenter(line,
                   FONT_SMALL,
                   DISPLAY_CENTER_LEFT,
                   STATISTICS_TOP + (9 * FONT_SMALL_BOTTOM) / 2);
}

void drawGameBoard(const uint8_t board[8][8],
                   const char time[2][8],
                   const char moveHistory[][2][6],
                   const char *buttonText, bool buttonSelected)
{
    for (uint32_t y = 0; y < 8; y++)
        for (uint32_t x = 0; x < 8; x++)
        {
            uint8_t piece = board[y][x];

            char pieceText[2];
            uint8_t pieceInvert;

            pieceText[0] = piece & 0x7f;
            pieceText[1] = '\0';

#if defined(DISPLAY_MONO)

            pieceInvert = (piece >> 7);

#elif defined(DISPLAY_COLOR)

            if (settings.displayTheme == DISPLAY_THEME_DARK)
                pieceInvert = !(piece >> 7);
            else
                pieceInvert = (piece >> 7);

#endif

            uint8_t px = GAME_BOARD_LEFT + GAME_SQUARE_WIDTH * x;
            uint8_t py = GAME_BOARD_TOP + GAME_SQUARE_HEIGHT * y;

            setDrawColor(pieceInvert);
            drawBox(px, py, GAME_SQUARE_WIDTH, GAME_SQUARE_HEIGHT);

            setDrawColor(!pieceInvert);
            drawTextLeft(pieceText, FONT_CHESS, px, py + GAME_SQUARE_HEIGHT);
        }

    setDrawColor(true);
    drawTextLeft(time[0],
                 FONT_SMALL,
                 GAME_BOARD_RIGHT + FONT_SMALL_HEIGHT,
                 GAME_BOARD_TOP + CENTER_TEXT(2 * GAME_SQUARE_HEIGHT, FONT_SMALL_HEIGHT));
    drawTextLeft(time[1],
                 FONT_SMALL,
                 GAME_BOARD_RIGHT + FONT_SMALL_HEIGHT,
                 GAME_BOARD_BOTTOM - 2 * GAME_SQUARE_HEIGHT + CENTER_TEXT(2 * GAME_SQUARE_HEIGHT + 1, FONT_SMALL_HEIGHT));

    for (uint32_t y = 0; y < 5; y++)
        for (uint32_t x = 0; x < 2; x++)
            drawTextLeft(moveHistory[y][x],
                         FONT_SMALL,
                         GAME_MOVE_CELL_LEFT + (GAME_MOVE_CELL_MARGIN + GAME_MOVE_CELL_WIDTH) * x,
                         GAME_MOVE_CELL_TOP + FONT_SMALL_BASELINE + GAME_MOVE_CELL_HEIGHT * y);

    if (buttonText)
    {
        drawBox(GAME_BUTTON_LEFT, GAME_BUTTON_TOP, GAME_BUTTON_WIDTH, GAME_BUTTON_HEIGHT);
        if (!buttonSelected)
        {
            setDrawColor(false);
            drawBox(GAME_BUTTON_LEFT + GAME_BUTTON_BORDER,
                    GAME_BUTTON_TOP + GAME_BUTTON_BORDER,
                    GAME_BUTTON_WIDTH - 2 * GAME_BUTTON_BORDER,
                    GAME_BUTTON_HEIGHT - 2 * GAME_BUTTON_BORDER);
        }

        setDrawColor(!buttonSelected);
        drawTextCenter(buttonText,
                       FONT_SMALL,
                       GAME_BUTTON_LEFT + GAME_BUTTON_WIDTH / 2,
                       GAME_BUTTON_TOP + FONT_SMALL_BASELINE +
                           (GAME_BUTTON_HEIGHT - FONT_SMALL_BOTTOM + 1) / 2);

        setDrawColor(true);
    }
}

#if defined(PULSE_LED)

// Pulse LED menu

static void onPulseLEDMenuSelect(const struct Menu *menu)
{
    settings.pulseLED = menu->state->selectedIndex;
}

static const char *const pulseLEDMenuOptions[] = {
    "Off",
    "On",
    NULL,
};

static struct MenuState pulseLEDMenuState;

static const struct Menu pulseLEDMenu = {
    "Pulse LED",
    &pulseLEDMenuState,
    onMenuGetOption,
    pulseLEDMenuOptions,
    onPulseLEDMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View pulseLEDMenuView = {
    onMenuEvent,
    &pulseLEDMenu,
};

#endif

#if defined(DISPLAY_MONO)

// Backlight menu

static void onBacklightMenuSelect(const struct Menu *menu)
{
    settings.displayBacklight = menu->state->selectedIndex;

    triggerDisplay();
}

static const char *const backlightMenuOptions[] = {
    "Off",
    "On for 30 seconds",
    "On for 1 minute",
    "On for 2 minutes",
    "On for 5 minutes",
    "Pulse flashes",
    "Always on",
    NULL,
};

static struct MenuState backlightMenuState;

static const struct Menu backlightMenu = {
    "Backlight",
    &backlightMenuState,
    onMenuGetOption,
    backlightMenuOptions,
    onBacklightMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View backlightMenuView = {
    onMenuEvent,
    &backlightMenu,
};

#elif defined(DISPLAY_COLOR)

// Display menu

const struct View displayMenuView;
const struct View displayThemeMenuView;
const struct View displayBrightnessLevelMenuView;
const struct View displaySleepMenuView;

static const char *const displayMenuOptions[] = {
    "Theme",
    "Brightness level",
    "Sleep",
    NULL,
};

static const struct View *displayMenuOptionViews[] = {
    &displayThemeMenuView,
    &displayBrightnessLevelMenuView,
    &displaySleepMenuView,
};

static void onDisplayMenuEnter(const struct Menu *menu)
{
    setView(displayMenuOptionViews[menu->state->selectedIndex]);
}

void onDisplaySubMenuBack(const struct Menu *menu);

void onDisplaySubMenuBack(const struct Menu *menu)
{
    setView(&displayMenuView);
}

static struct MenuState displayMenuState;

static const struct Menu displayMenu = {
    "Display",
    &displayMenuState,
    onMenuGetOption,
    displayMenuOptions,
    NULL,
    onDisplayMenuEnter,
    onSettingsSubMenuBack,
};

const struct View displayMenuView = {
    onMenuEvent,
    &displayMenu,
};

// Display theme menu

static void onDisplayThemeMenuSelect(const struct Menu *menu)
{
    settings.displayTheme = menu->state->selectedIndex;

    refreshView();
}

static const char *const displayColorThemeMenuOptions[] = {
    "Dark",
    "Light",
    NULL,
};

static struct MenuState displayThemeMenuState;

static const struct Menu displayThemeMenu = {
    "Theme",
    &displayThemeMenuState,
    onMenuGetOption,
    displayColorThemeMenuOptions,
    onDisplayThemeMenuSelect,
    NULL,
    onDisplaySubMenuBack,
};

const struct View displayThemeMenuView = {
    onMenuEvent,
    &displayThemeMenu,
};

// Display brightness level menu

static void onDisplayBrightnessLevelMenuSelect(const struct Menu *menu)
{
    settings.displayBrightnessLevel = menu->state->selectedIndex;
}

static const char *const displayBrightnessLevelMenuOptions[] = {
    "Low",
    "Medium",
    "High",
    NULL,
};

static struct MenuState displayBrightnessLevelMenuState;

static const struct Menu displayBrightnessLevelMenu = {
    "Brightness level",
    &displayBrightnessLevelMenuState,
    onMenuGetOption,
    displayBrightnessLevelMenuOptions,
    onDisplayBrightnessLevelMenuSelect,
    NULL,
    onDisplaySubMenuBack,
};

const struct View displayBrightnessLevelMenuView = {
    onMenuEvent,
    &displayBrightnessLevelMenu,
};

// Display sleep menu

static void onDisplaySleepMenuSelect(const struct Menu *menu)
{
    settings.displaySleep = menu->state->selectedIndex;

    triggerDisplay();
}

static const char *const displaySleepMenuOptions[] = {
    "30 seconds",
    "1 minute",
    "2 minutes",
    "5 minutes",
    "Always on",
    NULL,
};

static struct MenuState displaySleepMenuState;

static const struct Menu displaySleepMenu = {
    "Sleep",
    &displaySleepMenuState,
    onMenuGetOption,
    displaySleepMenuOptions,
    onDisplaySleepMenuSelect,
    NULL,
    onDisplaySubMenuBack,
};

const struct View displaySleepMenuView = {
    onMenuEvent,
    &displaySleepMenu,
};

#endif
