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
#define MEASUREMENT_UNIT_WIDTH (CONTENT_WIDTH - MEASUREMENT_UNIT_LEFT)
#define MEASUREMENT_UNIT_HEIGHT FONT_MEDIUM_LINE_HEIGHT
#define MEASUREMENT_UNIT_BOTTOM (MEASUREMENT_UNIT_TOP + MEASUREMENT_UNIT_HEIGHT)
#define MEASUREMENT_UNIT_OFFSETX (MEASUREMENT_UNIT_WIDTH - MEASUREMENT_PADDING_RIGHT)
#define MEASUREMENT_UNIT_OFFSETY MEASUREMENT_CONFIDENCE_TEXTOFFSETY

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
#define MEASUREMENT_CONFIDENCE_TEXTOFFSETY (-(FONT_MEDIUM_LINE_HEIGHT - MEASUREMENT_UNIT_HEIGHT) / 2)

#define MEASUREMENT_INFOKEY_LEFT CONTENT_LEFT
#if defined(DISPLAY_LANDSCAPE)
#define MEASUREMENT_INFOKEY_TOP MEASUREMENT_VALUE_BOTTOM
#elif defined(DISPLAY_PORTRAIT)
#define MEASUREMENT_INFOKEY_TOP MEASUREMENT_CONFIDENCE_BOTTOM
#endif
#define MEASUREMENT_INFOKEY_HEIGHT (MEASUREMENT_INFOVALUE_TOP - MEASUREMENT_INFOKEY_TOP)
#define MEASUREMENT_INFOKEY_OFFSETX MEASUREMENT_PADDING_LEFT
#define MEASUREMENT_INFOKEY_OFFSETY (MEASUREMENT_INFOKEY_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define MEASUREMENT_INFOKEY_TEXTTOP (MEASUREMENT_INFOVALUE_TOP - FONT_SMALL_LINE_HEIGHT)

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
    char confidenceUnit[8];
    strclr(confidenceString);
    strclr(confidenceUnit);
    if (confidence != 0)
    {
        strcat(confidenceString, getString(STRING_PLUSMINUS));
        strcatFloat(confidenceString, 100 * confidence, confidence < 0.1F ? 1 : 0);
        strcat(confidenceUnit, getString(STRING_PERCENT));
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
#define BAR_TICKS_TOP 0
#define BAR_TICKS_WIDTH BAR_FRAME_WIDTH
#define BAR_TICKS_HEIGHT TICKS_LONG

#define BAR_TICK_1 (BAR_TICKS_LEFT + (0 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_2 (BAR_TICKS_LEFT + (1 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_3 (BAR_TICKS_LEFT + (2 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_4 (BAR_TICKS_LEFT + (3 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_5 (BAR_TICKS_LEFT + (4 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_6 (BAR_TICKS_LEFT + (5 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_7 (BAR_TICKS_LEFT + (6 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)
#define BAR_TICK_8 (BAR_TICKS_LEFT + (7 * (BAR_TICKS_WIDTH - 1)) / BAR_DECADES)

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
#define BAR_CONTENT_WIDTH (BAR_FRAME_WIDTH - 2)
#define BAR_CONTENT_HEIGHT (BAR_FRAME_HEIGHT - 2)

typedef struct
{
    uint8_t width;
    uint8_t offset;
} LabelSpan;

static const LabelSpan labelSpansTicks1[] = {
    {BAR_TICK_1 + BAR_LABEL_CELL_HALFWIDTH, BAR_TICK_1},
    {(BAR_TICK_4 + BAR_LABEL_CELL_HALFWIDTH) - (BAR_TICK_1 + BAR_LABEL_CELL_HALFWIDTH), BAR_TICK_4 - (BAR_TICK_1 + BAR_LABEL_CELL_HALFWIDTH)},
    {BAR_LABEL_WIDTH - (BAR_TICK_4 + BAR_LABEL_CELL_HALFWIDTH), BAR_TICK_7 - (BAR_TICK_4 + BAR_LABEL_CELL_HALFWIDTH)},
};

static const LabelSpan labelSpansTicks3[] = {
    {BAR_TICK_3 + BAR_LABEL_CELL_HALFWIDTH, BAR_TICK_3},
    {BAR_LABEL_WIDTH - (BAR_TICK_3 + BAR_LABEL_CELL_HALFWIDTH), BAR_TICK_6 - (BAR_TICK_3 + BAR_LABEL_CELL_HALFWIDTH)},
};

typedef struct
{
    uint8_t end;
    ColorIndex colorIndex;
} FillSpan;

static const FillSpan fillSpansTicksPrimary1[] = {
    {BAR_TICK_1, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_1 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_4, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_4 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_7, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_7 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
};

static const FillSpan fillSpansTicksPrimary3[] = {
    {BAR_TICK_3, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_3 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_6, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_6 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
};

static const FillSpan fillSpansTicksSecondary[] = {
    {BAR_TICK_1, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_1 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_2, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_2 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_3, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_3 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_4, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_4 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_5, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_5 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_6, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_6 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_7, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_7 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_TICK_8, COLOR_CONTAINER_BACKGROUND},
    {BAR_TICK_8 + 1, COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY},
    {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
};

static const FillSpan fillSpansSpace[] = {
    {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
};

static int16_t getMeasurementBarX(float value, float offset)
{
    int16_t x;

    if (value <= offset)
        x = 0;
    else
    {
        x = log10f(value / offset) * ((float)BAR_FRAME_WIDTH / BAR_DECADES);

        if (x >= BAR_FRAME_WIDTH)
            x = BAR_FRAME_WIDTH;
    }

    return BAR_FRAME_LEFT + x;
}

void drawMeasurementBar(float value, int32_t minExponent, float warningValue, float alarmValue)
{
    // Pre-calculations
    const LabelSpan *labelSpansTicks;
    const FillSpan *fillSpansTicksPrimary;
    int32_t tickExponent;
    if ((minExponent == -6) || (minExponent == 0))
    {
        labelSpansTicks = labelSpansTicks1;
        fillSpansTicksPrimary = fillSpansTicksPrimary1;
        tickExponent = minExponent;
    }
    else
    {
        labelSpansTicks = labelSpansTicks3;
        fillSpansTicksPrimary = fillSpansTicksPrimary3;
        tickExponent = minExponent + 2;
    }

    float valueOffset = powf(10.0F, minExponent);
    int16_t valueX = getMeasurementBarX(value, valueOffset);
#if defined(DISPLAY_COLOR)
    int16_t warningX = warningValue ? getMeasurementBarX(warningValue, valueOffset) : BAR_IMAGEBUFFER_WIDTH;
    int16_t alarmX = alarmValue ? getMeasurementBarX(alarmValue, valueOffset) : BAR_IMAGEBUFFER_WIDTH;
    if (warningX > alarmX)
        warningX = alarmX;
    int16_t warningEnd = MIN(valueX, warningX);
    int16_t alarmEnd = MIN(valueX, alarmX);
#endif

    // Calculated fill spans
    FillSpan fillSpansFrame[] = {
        {BAR_FRAME_LEFT, COLOR_CONTAINER_BACKGROUND},
#if defined(DISPLAY_MONOCHROME)
        {valueX, COLOR_INSTRUMENT_ENHANCED_SECONDARY},
#elif defined(DISPLAY_COLOR)
        {warningEnd, COLOR_INSTRUMENT_ENHANCED_SECONDARY},
        {alarmEnd, COLOR_INSTRUMENT_ENHANCED_SECONDARY_WARNING},
        {valueX, COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALARM},
#endif
        {BAR_FRAME_LEFT + BAR_FRAME_WIDTH, COLOR_INSTRUMENT_FRAME_TERTIARY},
        {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
    };

    FillSpan fillSpansContent[] = {
        {BAR_FRAME_LEFT, COLOR_CONTAINER_BACKGROUND},
#if defined(DISPLAY_MONOCHROME)
        {valueX, COLOR_INSTRUMENT_ENHANCED_SECONDARY},
#elif defined(DISPLAY_COLOR)
        {warningEnd, COLOR_INSTRUMENT_ENHANCED_SECONDARY},
        {alarmEnd, COLOR_INSTRUMENT_ENHANCED_SECONDARY_WARNING},
        {valueX, COLOR_INSTRUMENT_ENHANCED_SECONDARY_ALARM},
#endif
        {BAR_CONTENT_LEFT, COLOR_INSTRUMENT_FRAME_TERTIARY},
        {BAR_CONTENT_LEFT + BAR_CONTENT_WIDTH, COLOR_INSTRUMENT_FRAME_PRIMARY},
        {BAR_FRAME_LEFT + BAR_FRAME_WIDTH, COLOR_INSTRUMENT_FRAME_TERTIARY},
        {BAR_IMAGEBUFFER_WIDTH, COLOR_CONTAINER_BACKGROUND},
    };

    // Render labels
    setStrokeColor(COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY);

    int16_t labelX = BAR_LABEL_LEFT;
    uint32_t labelTicksIndex = 0;
    while (labelX < BAR_LABEL_WIDTH)
    {
        char buffer[32];
        strclr(buffer);
        strcatMetricPower(buffer, tickExponent, -3);

        int16_t labelWidth = labelSpansTicks[labelTicksIndex].width;
        int16_t labelOffset = labelSpansTicks[labelTicksIndex].offset;

        int16_t textHalfWidth = getTextWidth(buffer) / 2;
        int16_t textLeft = labelOffset - textHalfWidth;
        if (textLeft < 0)
            labelOffset = textHalfWidth;

        drawCenteredText(buffer,
                         &(mr_rectangle_t){labelX, BAR_LABEL_TOP, labelWidth, BAR_LABEL_HEIGHT},
                         &(mr_point_t){labelOffset, BAR_LABEL_OFFSETY});

        labelTicksIndex++;
        labelX += labelWidth;
        tickExponent += 3;
    }

    // Render line
    mr_color_t line[BAR_IMAGEBUFFER_WIDTH];

    for (int16_t y = 0; y < BAR_IMAGEBUFFER_HEIGHT; y++)
    {
        const FillSpan *fillSpan;
        if (y < (BAR_TICKS_HEIGHT - TICKS_SHORT))
            fillSpan = fillSpansTicksPrimary;
        else if (y < BAR_TICKS_HEIGHT)
            fillSpan = fillSpansTicksSecondary;
        else if (y < BAR_FRAME_TOP)
            fillSpan = fillSpansSpace;
        else if (y < BAR_CONTENT_TOP)
            fillSpan = fillSpansFrame;
        else if (y < (BAR_CONTENT_TOP + BAR_CONTENT_HEIGHT))
            fillSpan = fillSpansContent;
        else if (y < BAR_FRAME_BOTTOM)
            fillSpan = fillSpansFrame;
        else
            fillSpan = fillSpansSpace;

        int16_t x = 0;
        uint32_t fillSpanIndex = 0;
        while (x < BAR_IMAGEBUFFER_WIDTH)
        {
            int16_t fillSpanX = fillSpan[fillSpanIndex].end;
            mr_color_t color = getFillColor(fillSpan[fillSpanIndex].colorIndex);

            while (x < fillSpanX)
                line[x++] = color;

            fillSpanIndex++;
        }

        drawImage(&(mr_rectangle_t){BAR_IMAGEBUFFER_LEFT, BAR_IMAGEBUFFER_TOP + y, BAR_IMAGEBUFFER_WIDTH, 1}, line);
    }

    return;
}
