/*
 * Rad Pro
 * UI measurements
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../system/cmath.h"
#include "../system/cstring.h"
#include "../ui/draw.h"
#include "../ui/measurements.h"

// Landscape layout:
//
//               Value
// ---------------------+-----------
//                      |       Unit
// Key                  +-----------
// ---------------------+ Confidence
// Value                |
//                      |

// Portrait layout:
//
//         Value
// ---------------------
//                  Unit
// ---------------------
//            Confidence
// ---------------------
//
// Key
// ---------------------
// Value
//

#if defined(DISPLAY_128X64)
#define MEASUREMENT_VALUE_TEXTSCALE 1.0F
#define MEASUREMENT_INFOKEY_WIDTH 82
#define MEASUREMENT_PADDING_LEFT 7
#define MEASUREMENT_PADDING_RIGHT 6
#define MEASUREMENT_PADDING_BOTTOM 0
#define MEASUREMENT_PADDING_TEXTBOTTOM 0
#elif defined(DISPLAY_320X240)
#define MEASUREMENT_VALUE_TEXTSCALE 1.08F
#define MEASUREMENT_INFOKEY_WIDTH 172
#define MEASUREMENT_PADDING_LEFT PADDING_LEFT
#define MEASUREMENT_PADDING_RIGHT PADDING_RIGHT
#define MEASUREMENT_PADDING_BOTTOM MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_PADDING_TEXTBOTTOM (MEASUREMENT_PADDING_LEFT / 2)
#elif defined(DISPLAY_240X320)
#define MEASUREMENT_VALUE_TEXTSCALE 1.08F
#define MEASUREMENT_INFOKEY_WIDTH CONTENT_WIDTH
#define MEASUREMENT_PADDING_LEFT 12
#define MEASUREMENT_PADDING_RIGHT MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_PADDING_BOTTOM MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_PADDING_TEXTBOTTOM (MEASUREMENT_PADDING_LEFT - FONT_SMALL_DESCENT)
#endif

#define MEASUREMENT_VALUEUNITCONFIDENCE_HEIGHT (MEASUREMENT_VALUE_TEXTHEIGHT + MEASUREMENT_UNIT_HEIGHT + MEASUREMENT_CONFIDENCE_TEXTHEIGHT)
#define MEASUREMENT_VALUEUNITCONFIDENCE_OFFSETY ((CONTENT_HEIGHT - MEASUREMENT_VALUEUNITCONFIDENCE_HEIGHT) / 2)

#define MEASUREMENT_VALUE_LEFT CONTENT_LEFT
#define MEASUREMENT_VALUE_TOP CONTENT_TOP
#define MEASUREMENT_VALUE_WIDTH CONTENT_WIDTH
#define MEASUREMENT_VALUE_HEIGHT (MEASUREMENT_VALUEUNITCONFIDENCE_OFFSETY + MEASUREMENT_VALUE_TEXTHEIGHT)
#define MEASUREMENT_VALUE_BOTTOM (MEASUREMENT_VALUE_TOP + MEASUREMENT_VALUE_HEIGHT)
#define MEASUREMENT_VALUE_OFFSETX (MEASUREMENT_VALUE_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_VALUE_OFFSETY (MEASUREMENT_VALUEUNITCONFIDENCE_OFFSETY + MEASUREMENT_VALUE_TEXTOFFSETY)
#define MEASUREMENT_VALUE_TEXTHEIGHT ((int32_t)(MEASUREMENT_VALUE_TEXTSCALE * FONT_LARGE_CAP_HEIGHT))
#define MEASUREMENT_VALUE_TEXTOFFSETY (-(FONT_LARGE_ASCENT - FONT_LARGE_CAP_HEIGHT) + (MEASUREMENT_VALUE_TEXTHEIGHT - FONT_LARGE_CAP_HEIGHT) / 2)

#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_UNIT_LEFT (CONTENT_LEFT + MEASUREMENT_INFOKEY_WIDTH)
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_UNIT_LEFT CONTENT_LEFT
#endif
#define MEASUREMENT_UNIT_TOP MEASUREMENT_VALUE_BOTTOM
#define MEASUREMENT_UNIT_WIDTH (CONTENT_RIGHT - MEASUREMENT_UNIT_LEFT)
#define MEASUREMENT_UNIT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_UNIT_BOTTOM (MEASUREMENT_UNIT_TOP + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_UNIT_OFFSETX (MEASUREMENT_UNIT_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_UNIT_OFFSETY (-(FONT_MEDIUM_LINE_HEIGHT - MEASUREMENT_UNIT_HEIGHT) / 2)

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
#define MEASUREMENT_CONFIDENCE_OFFSETY (-(FONT_SMALL_LINE_HEIGHT - MEASUREMENT_CONFIDENCE_TEXTHEIGHT) / 2)
#define MEASUREMENT_CONFIDENCE_TEXTHEIGHT ((int32_t)(0.8F * FONT_SMALL_LINE_HEIGHT))

#define MEASUREMENT_INFOKEY_LEFT CONTENT_LEFT
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_INFOKEY_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_INFOKEY_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define MEASUREMENT_INFOKEY_HEIGHT (MEASUREMENT_INFOVALUE_TOP - MEASUREMENT_INFOKEY_TOP)
#define MEASUREMENT_INFOKEY_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFOKEY_OFFSETY (MEASUREMENT_INFOKEY_HEIGHT - FONT_SMALL_LINE_HEIGHT)

#define MEASUREMENT_INFOVALUE_LEFT MEASUREMENT_INFOKEY_LEFT
#define MEASUREMENT_INFOVALUE_TOP (CONTENT_BOTTOM - MEASUREMENT_INFOVALUE_HEIGHT)
#define MEASUREMENT_INFOVALUE_WIDTH MEASUREMENT_INFOKEY_WIDTH
#define MEASUREMENT_INFOVALUE_HEIGHT (FONT_SMALL_LINE_HEIGHT + MEASUREMENT_PADDING_TEXTBOTTOM)
#define MEASUREMENT_INFOVALUE_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFOVALUE_OFFSETY 0

#define MEASUREMENT_ALERT_LEFT MEASUREMENT_INFOKEY_LEFT
#define MEASUREMENT_ALERT_TOP MEASUREMENT_INFOKEY_TOP
#define MEASUREMENT_ALERT_WIDTH MEASUREMENT_INFOKEY_WIDTH
#define MEASUREMENT_ALERT_HEIGHT (MEASUREMENT_INFOKEY_HEIGHT + MEASUREMENT_INFOVALUE_HEIGHT)
#define MEASUREMENT_ALERT_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_ALERT_OFFSETY (MEASUREMENT_INFOKEY_OFFSETY + FONT_SMALL_LINE_HEIGHT / 2)

static const ColorIndex measurementStyleColorIndex[] = {
    COLOR_INSTRUMENT_ENHANCED_SECONDARY,
    COLOR_WARNING,
    COLOR_ALARM,
    COLOR_ELEMENT_ACTIVE,
};

void drawMeasurementValue(const char *valueString, const char *unitString, float confidence, MeasurementStyle style)
{
    // Value
    const mr_rectangle_t valueRectangle = {
        MEASUREMENT_VALUE_LEFT,
        MEASUREMENT_VALUE_TOP,
        MEASUREMENT_VALUE_WIDTH,
        MEASUREMENT_VALUE_HEIGHT,
    };
    setFont(font_large);
    setStrokeColor(measurementStyleColorIndex[style]);
    drawRightAlignedText(valueString,
                         &valueRectangle,
                         &(mr_point_t){MEASUREMENT_VALUE_OFFSETX, MEASUREMENT_VALUE_OFFSETY});

#if defined(DISPLAY_MONOCHROME)
    if (style == MEASUREMENTSTYLE_WARNING)
        ditherRectangle(&valueRectangle, false);
    else if (style == MEASUREMENTSTYLE_ALARM)
        ditherRectangle(&valueRectangle, true);
#endif

    // Unit
    setFont(font_medium);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawRightAlignedText(unitString,
                         &(mr_rectangle_t){MEASUREMENT_UNIT_LEFT, MEASUREMENT_UNIT_TOP, MEASUREMENT_UNIT_WIDTH, MEASUREMENT_UNIT_HEIGHT},
                         &(mr_point_t){MEASUREMENT_UNIT_OFFSETX, MEASUREMENT_UNIT_OFFSETY});

    // Confidence
    char confidenceString[8];
    const char *confidenceUnit = "";
    strclr(confidenceString);
    if (confidence != 0)
    {
        strcat(confidenceString, getString(STRING_PLUSMINUS));
        // Below 9.95% one decimal is shown; 9.95% and up rounds to whole percents ("±10%", not "±10.0%")
        strcatFloat(confidenceString, 100 * confidence, confidence < 0.0995F ? 1 : 0);
        confidenceUnit = getString(STRING_PERCENT);
    }
    setFont(font_small);
    drawValueAndUnit(confidenceString,
                     confidenceUnit,
                     &(mr_rectangle_t){MEASUREMENT_CONFIDENCE_LEFT, MEASUREMENT_CONFIDENCE_TOP, MEASUREMENT_CONFIDENCE_WIDTH, MEASUREMENT_CONFIDENCE_HEIGHT},
                     &(mr_point_t){MEASUREMENT_CONFIDENCE_OFFSETX - getTextWidth(confidenceString) - getTextWidth(confidenceUnit), MEASUREMENT_CONFIDENCE_OFFSETY},
                     style == MEASUREMENTSTYLE_DONE);
}

void drawMeasurementAlert(const char *alertString)
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(alertString,
             &(mr_rectangle_t){MEASUREMENT_ALERT_LEFT, MEASUREMENT_ALERT_TOP, MEASUREMENT_ALERT_WIDTH, MEASUREMENT_ALERT_HEIGHT},
             &(mr_point_t){MEASUREMENT_ALERT_OFFSETX, MEASUREMENT_ALERT_OFFSETY});
}

void drawMeasurementInfo(const char *keyString, const char *valueString, const char *unitString, MeasurementStyle style)
{
    setFont(font_small);

    // Key
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(keyString,
             &(mr_rectangle_t){MEASUREMENT_INFOKEY_LEFT, MEASUREMENT_INFOKEY_TOP, MEASUREMENT_INFOKEY_WIDTH, MEASUREMENT_INFOKEY_HEIGHT},
             &(mr_point_t){MEASUREMENT_INFOKEY_OFFSETX, MEASUREMENT_INFOKEY_OFFSETY});

    // Value
    drawValueAndUnit(valueString,
                     unitString,
                     &(mr_rectangle_t){MEASUREMENT_INFOVALUE_LEFT, MEASUREMENT_INFOVALUE_TOP, MEASUREMENT_INFOVALUE_WIDTH, MEASUREMENT_INFOVALUE_HEIGHT},
                     &(mr_point_t){MEASUREMENT_INFOVALUE_OFFSETX, MEASUREMENT_INFOVALUE_OFFSETY},
                     style == MEASUREMENTSTYLE_DONE);
}

// Bar

#define BAR_DECADES 7

#define BAR_LEFT CONTENT_LEFT
#if defined(DISPLAY_LANDSCAPE)
#define BAR_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define BAR_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define BAR_WIDTH MEASUREMENT_INFOKEY_WIDTH
#define BAR_HEIGHT (CONTENT_BOTTOM - BAR_TOP)

#define BAR_LABEL_LEFT BAR_LEFT
#define BAR_LABEL_TOP BAR_TOP
#define BAR_LABEL_WIDTH BAR_WIDTH
#define BAR_LABEL_HEIGHT (BAR_HEIGHT - BAR_IMAGEBUFFER_HEIGHT)
#define BAR_LABEL_OFFSETY (BAR_LABEL_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define BAR_LABEL_CELL_HALFWIDTH ((3 * BAR_TICKS_WIDTH / BAR_DECADES) / 2)

#define BAR_IMAGEBUFFER_LEFT BAR_LEFT
#define BAR_IMAGEBUFFER_TOP (BAR_LABEL_TOP + BAR_LABEL_HEIGHT)
#define BAR_IMAGEBUFFER_WIDTH BAR_WIDTH
#define BAR_IMAGEBUFFER_HEIGHT (BAR_TICKS_HEIGHT + TICKS_SPACING + BAR_FRAME_HEIGHT + MEASUREMENT_PADDING_BOTTOM)

#define BAR_TICKS_LEFT BAR_FRAME_LEFT
#define BAR_TICKS_WIDTH BAR_FRAME_WIDTH
#define BAR_TICKS_HEIGHT TICKS_LONG
#define BAR_TICK(index) (BAR_TICKS_LEFT + ((index) * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)

#define BAR_FRAME_LEFT PADDING_LEFT
#define BAR_FRAME_TOP (BAR_TICKS_HEIGHT + TICKS_SPACING)
#if defined(DISPLAY_LANDSCAPE)
#define BAR_FRAME_WIDTH (BAR_IMAGEBUFFER_WIDTH - PADDING_LEFT)
#elif defined(DISPLAY_PORTRAIT)
#define BAR_FRAME_WIDTH (BAR_IMAGEBUFFER_WIDTH - PADDING_LEFT - PADDING_RIGHT)
#endif
#define BAR_FRAME_HEIGHT BAR_THICKNESS
#define BAR_FRAME_RIGHT (BAR_FRAME_LEFT + BAR_FRAME_WIDTH)
#define BAR_FRAME_BOTTOM (BAR_FRAME_TOP + BAR_FRAME_HEIGHT)

#define BAR_CONTENT_LEFT (BAR_FRAME_LEFT + 1)
#define BAR_CONTENT_TOP (BAR_FRAME_TOP + 1)
#define BAR_CONTENT_RIGHT (BAR_FRAME_RIGHT - 1)
#define BAR_CONTENT_BOTTOM (BAR_FRAME_BOTTOM - 1)

// Primary ticks (with labels) every 3 decades, starting at firstTick
#define BAR_PRIMARY_TICKS 0x49

static const uint8_t barTicks[] = {
    BAR_TICK(0),
    BAR_TICK(1),
    BAR_TICK(2),
    BAR_TICK(3),
    BAR_TICK(4),
    BAR_TICK(5),
    BAR_TICK(6),
    BAR_TICK(7),
};

static int16_t getMeasurementBarX(float value, float offset)
{
    // Negated comparison: NaN maps to the left edge
    if (!(value > offset))
        return BAR_FRAME_LEFT;

    // Clamps as float, before the int16_t conversion: +Inf maps to the right edge
    float x = logf(value / offset) * (LOG10E * BAR_FRAME_WIDTH / BAR_DECADES);
    if (x >= BAR_FRAME_WIDTH)
        return BAR_FRAME_RIGHT;

    return BAR_FRAME_LEFT + (int16_t)x;
}

void drawMeasurementBar(float value, int32_t minExponent, float warningValue, float alarmValue)
{
    // Labels sit on ticks with exponents divisible by 3 (metric prefixes)
    uint32_t firstTick = ((minExponent == -6) || (minExponent == 0)) ? 0 : 2;

    setStrokeColor(COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY);

    int32_t labelLeft = BAR_LABEL_LEFT;
    for (uint32_t tick = firstTick; tick < BAR_DECADES; tick += 3)
    {
        char buffer[32];
        strclr(buffer);
        strcatMetricPower(buffer, minExponent + tick, -3);

        // Cells reach halfway to the next label (the last one to the right
        // edge); a label that would cross the left edge is shifted right
        int32_t tickX = BAR_LABEL_LEFT + barTicks[tick];
        int32_t labelRight = ((tick + 3) < BAR_DECADES)
                                 ? (tickX + BAR_LABEL_CELL_HALFWIDTH)
                                 : (BAR_LABEL_LEFT + BAR_LABEL_WIDTH);
        int32_t labelOffset = MAX(tickX - labelLeft, getTextWidth(buffer) / 2);

        drawCenteredText(buffer,
                         &(mr_rectangle_t){labelLeft, BAR_LABEL_TOP, labelRight - labelLeft, BAR_LABEL_HEIGHT},
                         &(mr_point_t){labelOffset, BAR_LABEL_OFFSETY});

        labelLeft = labelRight;
    }

    // Value, split by alert level (normal, warning, alarm)
    float valueOffset = getDecimalPowerFloat(minExponent);
    int32_t valueX = getMeasurementBarX(value, valueOffset);
#if defined(DISPLAY_COLOR)
    int32_t warningX = warningValue ? getMeasurementBarX(warningValue, valueOffset) : BAR_FRAME_RIGHT;
    int32_t alarmX = alarmValue ? getMeasurementBarX(alarmValue, valueOffset) : BAR_FRAME_RIGHT;
    const int32_t levelEnds[] = {MIN(warningX, alarmX), alarmX, BAR_FRAME_RIGHT};
#elif defined(DISPLAY_MONOCHROME)
    (void)warningValue;
    (void)alarmValue;
    const int32_t levelEnds[] = {BAR_FRAME_RIGHT};
#endif

    // Render line by line: background, then ticks or frame, then value
    mr_color_t line[BAR_IMAGEBUFFER_WIDTH];

    for (int32_t y = 0; y < BAR_IMAGEBUFFER_HEIGHT; y++)
    {
        fillImageBuffer(line, 0, BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND);

        if (y < BAR_TICKS_HEIGHT)
        {
            // Primary ticks are long, all ticks reach the short tick rows
            uint32_t ticks = (y < (BAR_TICKS_HEIGHT - TICKS_SHORT))
                                 ? (BAR_PRIMARY_TICKS << firstTick)
                                 : 0xff;

            for (uint32_t tick = 0; tick <= BAR_DECADES; tick++)
                if ((ticks >> tick) & 1)
                    fillImageBuffer(line, barTicks[tick], barTicks[tick] + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY);
        }
        else if ((y >= BAR_FRAME_TOP) && (y < BAR_FRAME_BOTTOM))
        {
            fillImageBuffer(line, BAR_FRAME_LEFT, BAR_FRAME_RIGHT, COLOR_INSTRUMENT_FRAME_TERTIARY);
            if ((y >= BAR_CONTENT_TOP) && (y < BAR_CONTENT_BOTTOM))
                fillImageBuffer(line, BAR_CONTENT_LEFT, BAR_CONTENT_RIGHT, COLOR_INSTRUMENT_FRAME_PRIMARY);

            int32_t x = BAR_FRAME_LEFT;
            for (uint32_t level = 0; level < ARRAY_SIZE(levelEnds); level++)
            {
                int32_t levelEnd = MIN(valueX, levelEnds[level]);
                fillImageBuffer(line, x, levelEnd, COLOR_INSTRUMENT_ENHANCED_SECONDARY + 2 * level);
                x = levelEnd;
            }
        }

        drawImage(&(mr_rectangle_t){BAR_IMAGEBUFFER_LEFT, BAR_IMAGEBUFFER_TOP + y, BAR_IMAGEBUFFER_WIDTH, 1}, line);
    }
}
