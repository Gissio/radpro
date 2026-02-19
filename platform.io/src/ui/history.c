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
#define HISTORY_TOP 0
#define HISTORY_WIDTH (HISTORY_FRAME_WIDTH + TICKS_SPACING + HISTORY_TICKS_WIDTH)
#define HISTORY_HEIGHT HISTORY_IMAGEBUFFER_HEIGHT

#define HISTORY_FRAME_LEFT HISTORY_LEFT
#define HISTORY_FRAME_TOP HISTORY_TOP
#define HISTORY_FRAME_WIDTH HISTORY_BIN_NUM
#define HISTORY_FRAME_HEIGHT HISTORY_HEIGHT

#define HISTORY_CONTENT_LEFT (HISTORY_FRAME_LEFT + 1)
#define HISTORY_CONTENT_TOP (HISTORY_FRAME_TOP + 1)
#define HISTORY_CONTENT_WIDTH (HISTORY_FRAME_WIDTH - 2)
#define HISTORY_CONTENT_HEIGHT (HISTORY_FRAME_HEIGHT - 2)

#define HISTORY_TICKS_LEFT (HISTORY_FRAME_LEFT + HISTORY_FRAME_WIDTH + TICKS_SPACING)
#define HISTORY_TICKS_TOP (CONTENT_TOP + (CONTENT_HEIGHT - HISTORY_HEIGHT) / 2)
#define HISTORY_TICKS_WIDTH TICKS_LONG
#define HISTORY_TICKS_HEIGHT (CONTENT_HEIGHT - 2 * FONT_SMALL_LINE_HEIGHT)

typedef struct
{
    uint8_t end;
    ColorIndex colorIndex;
} FillSpan;

static const FillSpan fillSpansSpace[] = {
    {HISTORY_FRAME_TOP + HISTORY_FRAME_HEIGHT, COLOR_CONTAINER_BACKGROUND},
};

static const FillSpan fillSpansFrameLine[] = {
    {HISTORY_FRAME_TOP + HISTORY_FRAME_HEIGHT, COLOR_INSTRUMENT_FRAME_TERTIARY},
};

static const FillSpan fillSpansTicksPrimary[] = {
    {HISTORY_CONTENT_TOP, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {HISTORY_CONTENT_TOP + HISTORY_CONTENT_HEIGHT, COLOR_CONTAINER_BACKGROUND},
    {HISTORY_FRAME_TOP + HISTORY_FRAME_HEIGHT, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
};

static int16_t getHistoryY(uint32_t value, int32_t valueOffset, int32_t valueScale)
{
    return (value ? ((value - valueOffset) * valueScale) / 65536 : 0);
}

void drawHistory(float scale, const char *unitString, uint32_t timeTicksNum, const char *periodLabel, const uint8_t *logValues, uint32_t warningValue, uint32_t alarmValue)
{
    // Pre-calculations
    char minLabel[32];
    char maxLabel[32];
    strclr(minLabel);
    strclr(maxLabel);

    int32_t minValue = UCHAR_MAX;
    int32_t maxValue = 0;

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
    int32_t valueScale = 0;
    if (maxValue)
    {
        int32_t unitValue = roundDown(HISTORY_DECADE * log10f(scale * HISTORY_VALUE_MIN) + 1);

        int32_t minExponent = divideDown(minValue + unitValue, HISTORY_DECADE);
        int32_t maxExponent = divideDown(maxValue + unitValue, HISTORY_DECADE) + 1;

        valueTicksNum = maxExponent - minExponent;
        valueOffset = HISTORY_DECADE * minExponent - unitValue;
        valueScale = (65536 * (HISTORY_FRAME_HEIGHT - 1)) / (HISTORY_DECADE * valueTicksNum);

        strcatMetricPower(minLabel, minExponent, -2);
        strcat(maxLabel, unitString);
        strcat(maxLabel, ": ");
        strcatMetricPower(maxLabel, maxExponent, -2);
    }

    // Calculated fill spans
    FillSpan fillSpansGrid[32];
    FillSpan fillSpansTicksSecondary[32];
    for (uint32_t i = 0, fillSpanIndex = 0; i <= valueTicksNum; i++)
    {
        int16_t y = i * (HISTORY_FRAME_HEIGHT - 1) / valueTicksNum;
        fillSpansGrid[fillSpanIndex] = (FillSpan){y, COLOR_INSTRUMENT_FRAME_PRIMARY};
        fillSpansTicksSecondary[fillSpanIndex++] = (FillSpan){y, COLOR_CONTAINER_BACKGROUND};
        fillSpansGrid[fillSpanIndex] = (FillSpan){y + 1, COLOR_INSTRUMENT_FRAME_TERTIARY};
        fillSpansTicksSecondary[fillSpanIndex++] = (FillSpan){y + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY};
    }

#if defined(DISPLAY_COLOR)
    int16_t warningY = warningValue ? getHistoryY(warningValue, valueOffset, valueScale) : HISTORY_FRAME_HEIGHT;
    int16_t alarmY = alarmValue ? getHistoryY(alarmValue, valueOffset, valueScale) : HISTORY_FRAME_HEIGHT;
    warningY = CLAMP_UINT8(warningY);
    alarmY = CLAMP_UINT8(alarmY);
    if (warningY > alarmY)
        warningY = alarmY;
#endif

    // Labels
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

    // Render line
    mr_color_t line[HISTORY_IMAGEBUFFER_HEIGHT];
    uint32_t timeTickIndex = 0;
    int16_t nextTimeTick = 0;
    int16_t previousY = 0;
    FillSpan fillSpansContent[32];
    for (int16_t x = 0; x < HISTORY_IMAGEBUFFER_WIDTH; x++)
    {
        const FillSpan *fillSpan;

        if (x < HISTORY_FRAME_LEFT)
            fillSpan = fillSpansSpace;
        else if (x < (HISTORY_FRAME_LEFT + HISTORY_FRAME_WIDTH))
        {
            // Value
            int16_t bin = x - HISTORY_FRAME_LEFT;

            int16_t y = getHistoryY(logValues[bin], valueOffset, valueScale);

            if (bin == 0)
                previousY = y;
            int16_t minY = (y < previousY) ? y : previousY;
            int16_t maxY = (y > previousY) ? y : previousY;
            if (minY > HISTORY_CONTENT_TOP)
                minY--;
            previousY = y;

            // Fill spans
            uint32_t fillSpanIndex = 0;
#if defined(DISPLAY_MONOCHROME)
            fillSpansContent[fillSpanIndex++] = (FillSpan){minY, COLOR_INSTRUMENT_ENHANCED_TERTIARY};
            fillSpansContent[fillSpanIndex++] = (FillSpan){maxY, COLOR_INSTRUMENT_ENHANCED_SECONDARY};
#elif defined(DISPLAY_COLOR)
            fillSpansContent[fillSpanIndex++] = (FillSpan){MIN(minY, warningY), COLOR_INSTRUMENT_ENHANCED_TERTIARY};
            fillSpansContent[fillSpanIndex++] = (FillSpan){MIN(maxY, warningY), COLOR_INSTRUMENT_ENHANCED_SECONDARY};
            fillSpansContent[fillSpanIndex++] = (FillSpan){MIN(minY, alarmY), COLOR_INSTRUMENT_ENHANCED_TERTIARY_WARNING};
            fillSpansContent[fillSpanIndex++] = (FillSpan){MIN(maxY, alarmY), COLOR_INSTRUMENT_ENHANCED_SECONDARY_WARNING};
            fillSpansContent[fillSpanIndex++] = (FillSpan){minY, COLOR_INSTRUMENT_ENHANCED_TERTIARY_ALARM};
            fillSpansContent[fillSpanIndex++] = (FillSpan){maxY, COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALARM};
#endif

            // Time ticks
            const FillSpan *fillSpansBackground;
            if (bin == nextTimeTick)
            {
                timeTickIndex++;
                nextTimeTick = timeTickIndex * (HISTORY_FRAME_WIDTH - 1) / timeTicksNum;

                fillSpansBackground = fillSpansFrameLine;
            }
            else
                fillSpansBackground = fillSpansGrid;

            int16_t fillSpanY = 0;
            while (fillSpanY < HISTORY_IMAGEBUFFER_HEIGHT)
            {
                fillSpanY = fillSpansBackground->end;
                fillSpansContent[fillSpanIndex++] = *fillSpansBackground++;
            }

            fillSpan = fillSpansContent;
        }
        else if (x < HISTORY_TICKS_LEFT)
            fillSpan = fillSpansSpace;
        else if (x < (HISTORY_TICKS_LEFT + TICKS_SHORT))
            fillSpan = fillSpansTicksSecondary;
        else if (x < (HISTORY_TICKS_LEFT + HISTORY_TICKS_WIDTH))
            fillSpan = fillSpansTicksPrimary;
        else
            fillSpan = fillSpansSpace;

        uint32_t y = 0;
        while (y < HISTORY_IMAGEBUFFER_HEIGHT)
        {
            int16_t fillSpanY = fillSpan->end;
            mr_color_t color = getFillColor(fillSpan->colorIndex);
            fillSpan++;

            while (y < fillSpanY)
                line[(HISTORY_IMAGEBUFFER_HEIGHT - 1) - y++] = color;
        }

        drawImage(&(mr_rectangle_t){HISTORY_IMAGEBUFFER_LEFT + x, HISTORY_IMAGEBUFFER_TOP, 1, HISTORY_IMAGEBUFFER_HEIGHT}, line);
    }
}
