/*
 * Rad Pro
 * UI history
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../system/cmath.h"
#include "../system/cstring.h"
#include "../ui/draw.h"
#include "../ui/history.h"

#define HISTORY_LABEL_MAX_LEFT CONTENT_LEFT
#define HISTORY_LABEL_MAX_TOP CONTENT_TOP
#define HISTORY_LABEL_MAX_WIDTH CONTENT_WIDTH
#define HISTORY_LABEL_MAX_HEIGHT FONT_SMALL_LINE_HEIGHT
#define HISTORY_LABEL_MAX_OFFSET_LEFT (HISTORY_LABEL_MAX_WIDTH - (CONTENT_WIDTH - (HISTORY_LEFT + HISTORY_WIDTH)))
#define HISTORY_LABEL_MAX_OFFSET_TOP 0

#define HISTORY_LABEL_PERIOD_LEFT CONTENT_LEFT
#define HISTORY_LABEL_PERIOD_TOP (CONTENT_BOTTOM - FONT_SMALL_LINE_HEIGHT)
#define HISTORY_LABEL_PERIOD_WIDTH (CONTENT_WIDTH / 2)
#define HISTORY_LABEL_PERIOD_HEIGHT FONT_SMALL_LINE_HEIGHT
#define HISTORY_LABEL_PERIOD_OFFSET_LEFT HISTORY_LEFT
#define HISTORY_LABEL_PERIOD_OFFSET_TOP 0

#define HISTORY_LABEL_MIN_LEFT (HISTORY_LABEL_PERIOD_LEFT + HISTORY_LABEL_PERIOD_WIDTH)
#define HISTORY_LABEL_MIN_TOP (CONTENT_BOTTOM - FONT_SMALL_LINE_HEIGHT)
#define HISTORY_LABEL_MIN_WIDTH (CONTENT_WIDTH - HISTORY_LABEL_PERIOD_WIDTH)
#define HISTORY_LABEL_MIN_HEIGHT FONT_SMALL_LINE_HEIGHT
#define HISTORY_LABEL_MIN_OFFSET_LEFT (HISTORY_LABEL_MIN_WIDTH - (CONTENT_WIDTH - (HISTORY_LEFT + HISTORY_WIDTH)))
#define HISTORY_LABEL_MIN_OFFSET_TOP 0

#define HISTORY_IMAGEBUFFER_LEFT CONTENT_LEFT
#define HISTORY_IMAGEBUFFER_TOP (HISTORY_LABEL_MAX_TOP + HISTORY_LABEL_MAX_HEIGHT)
#define HISTORY_IMAGEBUFFER_WIDTH CONTENT_WIDTH
#define HISTORY_IMAGEBUFFER_HEIGHT (HISTORY_LABEL_MIN_TOP - HISTORY_IMAGEBUFFER_TOP)

#define HISTORY_LEFT ((CONTENT_WIDTH - HISTORY_WIDTH) / 2)
#define HISTORY_WIDTH (HISTORY_FRAME_WIDTH + TICKS_SPACING + HISTORY_TICKS_WIDTH)

#define HISTORY_FRAME_LEFT HISTORY_LEFT
#define HISTORY_FRAME_WIDTH HISTORY_BIN_NUM
#define HISTORY_FRAME_HEIGHT HISTORY_IMAGEBUFFER_HEIGHT

#define HISTORY_TICKS_LEFT (HISTORY_FRAME_LEFT + HISTORY_FRAME_WIDTH + TICKS_SPACING)
#define HISTORY_TICKS_WIDTH TICKS_LONG

#define HISTORY_MARGIN ((uint32_t)(0.1F * HISTORY_DECADE))

// Lowest y of a data point: its 1-pixel line (y - 1) stays above the frame's
// bottom row and distinct from "no data"
#define HISTORY_DATA_Y_MIN 2

// Column y runs upwards from the frame's bottom row (y = 0); values below
// the plotted range (e.g. alert thresholds) clamp to 0
static int32_t getHistoryY(uint32_t value, int32_t valueOffset, int32_t valueScale)
{
    int32_t y = value ? (((int32_t)value - valueOffset) * valueScale) / 65536 : 0;

    return MAX(y, 0);
}

// Fills rows [start, end) of a column (y = 0 is the bottom row)
static void fillHistoryColumn(mr_color_t *column, int32_t start, int32_t end, ColorIndex colorIndex)
{
    fillImageBuffer(column, HISTORY_FRAME_HEIGHT - end, HISTORY_FRAME_HEIGHT - start, colorIndex);
}

void drawHistory(float scale, const char *unitString, uint32_t timeTicksNum, const char *periodLabel, const uint8_t *logValues, uint32_t warningValue, uint32_t alarmValue)
{
    // Value range
    char minLabel[32];
    char maxLabel[32];
    strclr(minLabel);
    strclr(maxLabel);

    uint32_t minValue = UCHAR_MAX;
    uint32_t maxValue = 0;

    for (uint32_t i = 0; i < HISTORY_BIN_NUM; i++)
    {
        uint32_t value = logValues[i];
        if (value)
        {
            if (value < minValue)
                minValue = value;
            if (value > maxValue)
                maxValue = value;
        }
    }

    uint32_t valueTicksNum = 1;
    int32_t valueOffset = 0;
    if (maxValue)
    {
        if (minValue > HISTORY_MARGIN)
            minValue -= HISTORY_MARGIN;
        maxValue += HISTORY_MARGIN;

        int32_t unitValue = roundDown((HISTORY_DECADE * LOG10E) * logf(scale * HISTORY_VALUE_MIN) + 1);

        int32_t minExponent = divideDown(minValue + unitValue, HISTORY_DECADE);
        int32_t maxExponent = divideDown(maxValue + unitValue, HISTORY_DECADE) + 1;

        valueTicksNum = maxExponent - minExponent;
        valueOffset = HISTORY_DECADE * minExponent - unitValue;

        // "1 µSv/h" or "10 Sv/h"; the bottom label shares the unit (with the
        // unit, it would not fit on 240x320)
        strcatMetricPower(minLabel, minExponent, -2);
        strcatMetricPower(maxLabel, maxExponent, -2);
        char lastChar = maxLabel[strlen(maxLabel) - 1];
        if ((lastChar >= '0') && (lastChar <= '9'))
            strcatChar(maxLabel, ' ');
        strcat(maxLabel, unitString);
    }

    // 16.16 fixed point: rows per log value step, and rows per grid line
    // (rounded up, so the last grid line lands on the frame's top row)
    int32_t valueScale = (65536 * (HISTORY_FRAME_HEIGHT - 1)) / (HISTORY_DECADE * valueTicksNum);
    uint32_t gridScale = (65536 * (HISTORY_FRAME_HEIGHT - 1)) / valueTicksNum + 1;

    // Alert level ends (normal, warning, alarm)
#if defined(DISPLAY_COLOR)
    int32_t warningY = warningValue ? getHistoryY(warningValue, valueOffset, valueScale) : HISTORY_FRAME_HEIGHT;
    int32_t alarmY = alarmValue ? getHistoryY(alarmValue, valueOffset, valueScale) : HISTORY_FRAME_HEIGHT;
    const int32_t levelEnds[] = {MIN(warningY, alarmY), alarmY, HISTORY_FRAME_HEIGHT};
#elif defined(DISPLAY_MONOCHROME)
    (void)warningValue;
    (void)alarmValue;
    const int32_t levelEnds[] = {HISTORY_FRAME_HEIGHT};
#endif

    // Labels
    setFont(font_small);
    setStrokeColor(COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY);

    drawText(periodLabel,
             &(mr_rectangle_t){HISTORY_LABEL_PERIOD_LEFT, HISTORY_LABEL_PERIOD_TOP, HISTORY_LABEL_PERIOD_WIDTH, HISTORY_LABEL_PERIOD_HEIGHT},
             &(mr_point_t){HISTORY_LABEL_PERIOD_OFFSET_LEFT, HISTORY_LABEL_PERIOD_OFFSET_TOP});

    drawRightAlignedText(maxLabel,
                         &(mr_rectangle_t){HISTORY_LABEL_MAX_LEFT, HISTORY_LABEL_MAX_TOP, HISTORY_LABEL_MAX_WIDTH, HISTORY_LABEL_MAX_HEIGHT},
                         &(mr_point_t){HISTORY_LABEL_MAX_OFFSET_LEFT, HISTORY_LABEL_MAX_OFFSET_TOP});

    drawRightAlignedText(minLabel,
                         &(mr_rectangle_t){HISTORY_LABEL_MIN_LEFT, HISTORY_LABEL_MIN_TOP, HISTORY_LABEL_MIN_WIDTH, HISTORY_LABEL_MIN_HEIGHT},
                         &(mr_point_t){HISTORY_LABEL_MIN_OFFSET_LEFT, HISTORY_LABEL_MIN_OFFSET_TOP});

    // Render column by column: background, grid lines, then value
    mr_color_t column[HISTORY_IMAGEBUFFER_HEIGHT];
    uint32_t timeTickIndex = 0;
    int32_t nextTimeTick = 0;
    int32_t previousY = 0;

    for (int32_t x = 0; x < HISTORY_IMAGEBUFFER_WIDTH; x++)
    {
        int32_t bin = x - HISTORY_FRAME_LEFT;
        bool isFrame = (uint32_t)bin < HISTORY_FRAME_WIDTH;
        ColorIndex backgroundColorIndex = COLOR_CONTAINER_BACKGROUND;
        ColorIndex gridColorIndex = COLOR_CONTAINER_BACKGROUND;
        uint32_t gridStep = 1;

        if (isFrame)
        {
            gridColorIndex = COLOR_INSTRUMENT_FRAME_TERTIARY;

            // Time ticks are full frame lines
            if (bin == nextTimeTick)
            {
                timeTickIndex++;
                nextTimeTick = timeTickIndex * (HISTORY_FRAME_WIDTH - 1) / timeTicksNum;

                backgroundColorIndex = COLOR_INSTRUMENT_FRAME_TERTIARY;
            }
            else
                backgroundColorIndex = COLOR_INSTRUMENT_FRAME_PRIMARY;
        }
        else if ((x >= HISTORY_TICKS_LEFT) && (x < (HISTORY_TICKS_LEFT + HISTORY_TICKS_WIDTH)))
        {
            gridColorIndex = COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY;

            // Long ticks only at the frame's bottom and top rows
            if (x >= (HISTORY_TICKS_LEFT + TICKS_SHORT))
                gridStep = valueTicksNum;
        }

        fillHistoryColumn(column, 0, HISTORY_FRAME_HEIGHT, backgroundColorIndex);
        for (uint32_t i = 0; i <= valueTicksNum; i += gridStep)
        {
            int32_t y = (i * gridScale) >> 16;
            fillHistoryColumn(column, y, y + 1, gridColorIndex);
        }

        if (isFrame)
        {
            uint32_t logValue = logValues[bin];
            int32_t y = getHistoryY(logValue, valueOffset, valueScale);
            if (logValue && (y < HISTORY_DATA_Y_MIN))
                y = HISTORY_DATA_Y_MIN;

            // Line from the previous value, 1 pixel thick when flat
            if (bin == 0)
                previousY = y;
            int32_t minY = MIN(y, previousY);
            int32_t maxY = MAX(y, previousY);
            if (minY >= HISTORY_DATA_Y_MIN)
                minY--;
            previousY = y;

            // Fill below the line (tertiary) and the line (secondary), by alert level
            int32_t levelStart = 0;
            for (uint32_t level = 0; level < ARRAY_SIZE(levelEnds); level++)
            {
                int32_t levelEnd = MIN(maxY, levelEnds[level]);
                ColorIndex colorIndex = COLOR_INSTRUMENT_ENHANCED_SECONDARY + 2 * level;

                fillHistoryColumn(column, levelStart, MIN(minY, levelEnd), colorIndex + 1);
                fillHistoryColumn(column, MAX(levelStart, minY), levelEnd, colorIndex);
                levelStart = levelEnd;
            }
        }

        drawImage(&(mr_rectangle_t){HISTORY_IMAGEBUFFER_LEFT + x, HISTORY_IMAGEBUFFER_TOP, 1, HISTORY_IMAGEBUFFER_HEIGHT}, column);
    }
}
