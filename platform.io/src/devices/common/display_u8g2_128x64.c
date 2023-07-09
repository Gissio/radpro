/*
 * Rad Pro
 * U8g2 interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(DISPLAY_U8G2) && defined(DISPLAY_128X64)

#include <string.h>

#include <clib/u8g2.h>

#include "../../battery.h"
#include "../../confidence.h"
#include "../../display.h"
#include "../../format.h"
#include "../../measurements.h"
#include "../../settings.h"

#include "dotted4.xbm"
#include "font_helvR08.h"
#include "font_helvR24.h"
#include "font_tiny5.h"
#include "font_icons.h"
#include "font_chess.h"

u8g2_t displayU8g2;

void clearDisplayBuffer(void)
{
    u8g2_ClearBuffer(&displayU8g2);
}

void sendDisplayBuffer(void)
{
    u8g2_SendBuffer(&displayU8g2);
}

static void drawTextLeft(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&displayU8g2, x, y, str);
}

static void drawTextCenter(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&displayU8g2, x - u8g2_GetStrWidth(&displayU8g2, str) / 2, y, str);
}

static void drawTextRight(const char *str, uint32_t x, uint32_t y)
{
    u8g2_DrawStr(&displayU8g2, x - u8g2_GetStrWidth(&displayU8g2, str), y, str);
}

void drawLowBattery(void)
{
    u8g2_SetFont(&displayU8g2, font_icons);
    drawTextCenter("0", DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 4);
}

void drawWelcome(void)
{
    u8g2_SetFont(&displayU8g2, font_helvR08);
    drawTextCenter(firmwareName, DISPLAY_CENTER_X, DISPLAY_CENTER_Y - 5);

    u8g2_SetFont(&displayU8g2, font_tiny5);
    drawTextCenter(firmwareVersion, DISPLAY_CENTER_X, DISPLAY_CENTER_Y + 11);
}

void drawStatusBar(void)
{
    uint8_t batteryLevel = getBatteryLevel();

    char statusBar[3];
    int statusBarIndex = 0;
    if (settings.rateAlarm || settings.doseAlarm)
    {
        statusBar[0] = ';';
        statusBarIndex++;
    }
    statusBar[statusBarIndex++] = (batteryLevel == BATTERY_LEVEL_CHARGING) ? ':' : '0' + batteryLevel;
    statusBar[statusBarIndex] = '\0';

    u8g2_SetFont(&displayU8g2, font_icons);
    drawTextRight(statusBar, DISPLAY_WIDTH - 4, 8);
}

void drawTitle(const char *title)
{
    u8g2_SetFont(&displayU8g2, font_tiny5);
    drawTextCenter(title, DISPLAY_CENTER_X, TITLE_Y);
}

void drawSubtitle(const char *subtitle)
{
    u8g2_SetFont(&displayU8g2, font_tiny5);
    drawTextCenter(subtitle, DISPLAY_CENTER_X, SUBTITLE_Y);
}

void drawMeasurementValue(const char *mantissa, const char *characteristic)
{
    // Value
    u8g2_SetFont(&displayU8g2, font_helvR24);
    drawTextLeft(mantissa, MEASUREMENT_VALUE_X, MEASUREMENT_VALUE_Y);

    // Units
    u8g2_SetFont(&displayU8g2, font_helvR08);
    drawTextLeft(characteristic, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y - 16);
}

void drawConfidenceIntervals(uint32_t sampleNum)
{
    if (sampleNum == 0)
        return;

    uint32_t lowerConfidenceInterval;
    uint32_t upperConfidenceInterval;
    getConfidenceIntervals(sampleNum, &lowerConfidenceInterval, &upperConfidenceInterval);

    u8g2_SetFont(&displayU8g2, font_tiny5);

    char confidenceInterval[16];

    strcpy(confidenceInterval, "+");
    strcatNumber(confidenceInterval, upperConfidenceInterval, 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y - 7);

    strcpy(confidenceInterval, "-");
    strcatNumber(confidenceInterval, lowerConfidenceInterval, 0);
    strcat(confidenceInterval, "%");
    drawTextLeft(confidenceInterval, MEASUREMENT_VALUE_SIDE_X, MEASUREMENT_VALUE_Y);
}

void drawHistory(const char *minLabel, const char *maxLabel,
                 int32_t offset, uint32_t range)
{
    // Plot separators
    u8g2_DrawHLine(&displayU8g2, 0, HISTORY_VIEW_Y_TOP, DISPLAY_WIDTH);
    u8g2_DrawHLine(&displayU8g2, 0, HISTORY_VIEW_Y_BOTTOM, DISPLAY_WIDTH);

    // Data
    // uint8_t lastX = DISPLAY_WIDTH;
    // uint8_t lastY = data[0];
    for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        uint8_t value = getHistoryDataPoint(i);
        if (value == 0)
            continue;

        uint32_t x = (DISPLAY_WIDTH - 1) - i * DISPLAY_WIDTH / HISTORY_BUFFER_SIZE;
        uint32_t y = (value + offset) * HISTORY_VALUE_DECADE / (HISTORY_VIEW_HEIGHT * range);

        // Pixel
        // u8g2_DrawPixel(&displayU8g2, x, HISTORY_VIEW_Y_BOTTOM - y);
        // Solid
        u8g2_DrawVLine(&displayU8g2, x, HISTORY_VIEW_Y_BOTTOM - y, y);
        // Line
        // u8g2_DrawLine(&displayU8g2, x, HISTORY_VIEW_Y_BOTTOM - y,
        //               lastX, HISTORY_VIEW_Y_BOTTOM - lastY);
        // lastX = x;
        // lastY = y;
    }

    // Time divisors
    u8g2_SetDisplayRotation(&displayU8g2, U8G2_R1);
    for (uint32_t i = 0; i < 7; i++)
    {
        uint32_t x = (i + 1) * DISPLAY_WIDTH / 8 - 1;
        u8g2_DrawXBM(&displayU8g2, HISTORY_VIEW_Y_TOP + 1, x, HISTORY_VIEW_HEIGHT - 1, 1, dotted4_bits);
    }
    u8g2_SetDisplayRotation(&displayU8g2, U8G2_R0);

    // Value divisors
    for (uint32_t i = 0; i < (uint32_t)(range - 1); i++)
    {
        uint32_t y = HISTORY_VIEW_Y_TOP + (i + 1) * HISTORY_VIEW_HEIGHT / range;
        u8g2_DrawXBM(&displayU8g2, 0, y, DISPLAY_WIDTH, 1, dotted4_bits);
    }

    u8g2_SetFont(&displayU8g2, font_tiny5);

    drawTextLeft(minLabel, 1, HISTORY_VIEW_Y_BOTTOM + 1 + 6);
    drawTextLeft(maxLabel, 1, HISTORY_VIEW_Y_TOP - 1);
}

void drawRNGText(char *text)
{
    u8g2_SetFont(&displayU8g2, font_helvR08);

    uint8_t x = 0;
    uint8_t y = DISPLAY_CENTER_Y + 4;

    while (*text != '\0')
    {
        uint8_t width = u8g2_GetGlyphWidth(&displayU8g2, *text);
        if ((x + width) >= DISPLAY_WIDTH)
        {
            x = 0;
            y += 12;
        }

        u8g2_DrawGlyph(&displayU8g2, x, y, *text);

        text++;
        x += width;
    }
}

void drawStats(void)
{
    char line[64];

    u8g2_SetFont(&displayU8g2, font_tiny5);

    strcpy(line, " Life timer: ");
    strcatTime(line, lifeState.time);
    strcat(line, " ");
    drawTextCenter(line, DISPLAY_CENTER_X, STATS_VIEW_Y);

    strcpy(line, " Life counts: ");
    strcatNumber(line, lifeState.pulseCount, 0);
    strcat(line, " ");
    drawTextCenter(line, DISPLAY_CENTER_X, STATS_VIEW_Y + 8);
}

void drawGameBoard(const char board[8][9],
                   const char time[2][6],
                   const char moveHistory[GAME_MOVES_LINE_NUM][2][6],
                   const char *buttonText, bool buttonSelected)
{
    u8g2_SetFont(&displayU8g2, font_chess);
    for (uint32_t y = 0; y < 8; y++)
        drawTextLeft(board[y], GAME_VIEW_BOARD_X, GAME_VIEW_BOARD_Y + 8 * y);

    u8g2_SetFont(&displayU8g2, font_tiny5);
    drawTextLeft(time[0], GAME_VIEW_TIME_X, GAME_VIEW_TIME_UPPER_Y);
    drawTextLeft(time[1], GAME_VIEW_TIME_X, GAME_VIEW_TIME_LOWER_Y);

    for (uint32_t y = 0; y < GAME_MOVES_LINE_NUM; y++)
    {
        for (uint32_t x = 0; x < 2; x++)
        {
            drawTextLeft(moveHistory[y][x],
                         GAME_VIEW_MOVES_X + GAME_VIEW_MOVES_LINE_WIDTH * x,
                         GAME_VIEW_MOVES_Y + GAME_VIEW_MOVES_LINE_HEIGHT * y);
        }
    }

    if (buttonText)
    {
        if (buttonSelected)
            u8g2_DrawRBox(&displayU8g2, GAME_VIEW_BUTTON_X, GAME_VIEW_BUTTON_Y,
                          GAME_VIEW_BUTTON_WIDTH, GAME_VIEW_BUTTON_HEIGHT, 1);
        else
            u8g2_DrawRFrame(&displayU8g2, GAME_VIEW_BUTTON_X, GAME_VIEW_BUTTON_Y,
                            GAME_VIEW_BUTTON_WIDTH, GAME_VIEW_BUTTON_HEIGHT, 1);

        u8g2_SetDrawColor(&displayU8g2, !buttonSelected);
        drawTextCenter(buttonText, GAME_VIEW_BUTTON_X + GAME_VIEW_BUTTON_WIDTH / 2, GAME_VIEW_BUTTON_Y + 7);

        u8g2_SetDrawColor(&displayU8g2, 1);
    }
}

void drawMenu(const struct Menu *menu)
{
    u8g2_SetFont(&displayU8g2, font_helvR08);

    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    for (uint32_t i = 0, y = MENU_VIEW_Y_TOP;
         i < MENU_VIEW_LINE_NUM;
         i++, y += MENU_VIEW_LINE_HEIGHT)
    {
        uint32_t index = startIndex + i;

        const char *name = menu->onGetOption(menu, index);
        if (!name)
            break;

        bool selected = (index == selectedIndex);

        u8g2_SetDrawColor(&displayU8g2, selected);
        u8g2_DrawBox(&displayU8g2, 0, y, DISPLAY_WIDTH, MENU_VIEW_LINE_HEIGHT);

        u8g2_SetDrawColor(&displayU8g2, !selected);
        drawTextLeft(name, MENU_VIEW_LINE_TEXT_X, y + MENU_VIEW_LINE_HEIGHT - 2);
    }

    u8g2_SetDrawColor(&displayU8g2, 1);
}

#endif
