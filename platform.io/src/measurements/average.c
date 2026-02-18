/*
 * Rad Pro
 * Average rate
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/average.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/system.h"

typedef enum
{
    AVERAGE_TAB_TIME,
    AVERAGE_TAB_RATE,
    AVERAGE_TAB_DOSE,

    AVERAGE_TAB_NUM,

    AVERAGE_TAB_ALERT = AVERAGE_TAB_NUM,
} AverageTab;

static void resetAverageRate(void);

static const int32_t averagingTimes[] = {
    30 * 24 * 60 * 60, // Off (30 days)
    24 * 60 * 60,      // 24 hours
    12 * 60 * 60,      // 12 hours
    6 * 60 * 60,       // 6 hours
    3 * 60 * 60,       // 3 hours
    60 * 60,           // 1 hour
    30 * 60,           // 30 minutes
    10 * 60,           // 10 minutes
    5 * 60,            // 5 minutes
    60,                // 1 minute
    30,                // 30 seconds
    10,                // 10 seconds
    5,                 // 5 seconds
    1,                 // 1 second
};

static const float averagingConfidences[] = {
    0.505F,  // ±50% confidence
    0.205F,  // ±20% confidence
    0.105F,  // ±10% confidence
    0.0505F, // ±5% confidence
    0.0205F, // ±2% confidence
    0.0105F, // ±1% confidence
};

static struct
{
    PulsePeriod period;
    Rate rate;

    uint32_t snapshotPulseCount;
    Rate snapshotRate;
    bool done;
} average;

static AverageTab averageTab;

void setupAverageRate(void)
{
    averageTab = AVERAGE_TAB_TIME;

    resetAverageRate();

    selectMenuItem(&averageMenu, settings.averaging);
}

static void resetAverageRate(void)
{
    memset(&average, 0, sizeof(average));

    if (averageTab == AVERAGE_TAB_ALERT)
        averageTab = AVERAGE_TAB_TIME;
}

static bool isTimeAveraging(void)
{
    return (settings.averaging < AVERAGING_TIME_NUM);
}

static uint32_t getAveragingTime(void)
{
    return averagingTimes[settings.averaging];
}

static uint32_t getAveragingConfidenceThreshold(void)
{
    return averagingConfidences[settings.averaging - AVERAGING_TIME_NUM];
}

static bool isAverageSaturated(void)
{
    return (average.rate.time == UINT32_MAX) || (average.period.pulseCount == UINT32_MAX);
}

static bool isAverageDone(void)
{
    if (isAverageSaturated())
        return true;

    if (isTimeAveraging())
        return average.rate.time >= getAveragingTime();

    float confidenceThreshold = getAveragingConfidenceThreshold();

    return (average.rate.confidence > 0.0F) &&
           (average.rate.confidence < confidenceThreshold);
}

static bool updateAverageRateTimer()
{
    bool done = isAverageDone();

    bool previousDone = average.done;
    average.done = done;

    if (!(previousDone && done))
    {
        average.snapshotPulseCount = average.period.pulseCount;
        average.snapshotRate = average.rate;
    }

    bool doneTriggered = (!previousDone && done);
    if (doneTriggered)
        triggerAlert(true);

    return doneTriggered;
}

static void updateAverageRateTab(bool doneTriggered)
{
    if (isTubeFaultAlertTriggered() || doneTriggered)
        averageTab = AVERAGE_TAB_ALERT;
    else if (averageTab == AVERAGE_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel() && !average.done)
            averageTab = AVERAGE_TAB_TIME;
    }
}

void updateAverageRate(PulsePeriod *period)
{
    if (isAverageSaturated())
        return;

    // Update average rate
    average.rate.time = addClamped(average.rate.time, 1);
    if (period->pulseCount)
    {
        if (!average.period.pulseCount)
            average.period.firstTick = period->firstTick;
        average.period.lastTick = period->lastTick;
        average.period.pulseCount = addClamped(average.period.pulseCount, period->pulseCount);

        calculateRate(&average.rate, &average.period);
    }

    bool doneTriggered = updateAverageRateTimer();

    updateAverageRateTab(doneTriggered);
}

float getAverageRate(void)
{
    return average.snapshotRate.value;
}

// Average rate view

static const char *getAverageRateAlertString(void)
{
    if (getTubeFaultAlertLevel())
        return getString(STRING_ALERT_FAULT);
    else if (average.done)
        return getString(STRING_ALERT_DONE);
    else
        return NULL;
}

static void advanceAverageRateTab(void)
{
    uint32_t tabNum = getAverageRateAlertString() ? (AVERAGE_TAB_NUM + 1) : AVERAGE_TAB_NUM;

    averageTab++;
    if (averageTab >= tabNum)
        averageTab = AVERAGE_TAB_TIME;
}

static MeasurementStyle getAverageRateMeasurementStyle(void)
{
    if (average.done)
        return MEASUREMENTSTYLE_DONE;

    return MEASUREMENTSTYLE_NORMAL;
}

static void drawAverageRateValue(void)
{
    char valueString[32] = "";
    char unitString[32] = "";
    buildValueString(valueString, unitString, average.snapshotRate.value, &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

    drawTitleBar(getString(STRING_AVERAGE));
    drawMeasurementValue(valueString, unitString, average.snapshotRate.confidence, getAverageRateMeasurementStyle());
}

static void drawAverageRateTab(void)
{
    char valueString[32] = "";
    char unitString[32] = " ";
    const char *keyString = NULL;

    switch (averageTab)
    {
    case AVERAGE_TAB_TIME:
        strcatTime(valueString, average.snapshotRate.time);

        keyString = getString(STRING_TIME);

        break;

    case AVERAGE_TAB_RATE:
        buildValueString(valueString, unitString, average.snapshotRate.value, &pulseUnits[settings.secondaryDoseUnits].rate, doseUnitsMinMetricPrefix[settings.secondaryDoseUnits]);

        keyString = getString(STRING_RATE);

        break;

    case AVERAGE_TAB_DOSE:
        buildValueString(valueString, unitString, average.snapshotPulseCount, &pulseUnits[DOSE_UNITS_CPM].dose, doseUnitsMinMetricPrefix[DOSE_UNITS_CPM]);

        keyString = getString(STRING_DOSE);

        break;

    default:
        break;
    }

    drawMeasurementInfo(keyString, valueString, unitString, getAverageRateMeasurementStyle());
}

static void drawAverageRateView(void)
{
    drawAverageRateValue();

    if (averageTab == AVERAGE_TAB_ALERT)
        drawMeasurementAlert(getAverageRateAlertString());
    else
        drawAverageRateTab();
}

void onAverageRateViewEvent(ViewEvent event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        advanceAverageRateTab();

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetAverageRate();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceAverageRate();

        break;
#endif

    case EVENT_DRAW:
        drawAverageRateView();

        break;

    default:
        break;
    }
}

// Average menu

static cstring averageMenuOptions[] = {
    STRING_UNLIMITED,
    STRING_24_HOURS,
    STRING_12_HOURS,
    STRING_6_HOURS,
    STRING_3_HOURS,
    STRING_1_HOUR,
    STRING_30_MINUTES,
    STRING_10_MINUTES,
    STRING_5_MINUTES,
    STRING_1_MINUTE,
    STRING_30_SECONDS,
    STRING_10_SECONDS,
    STRING_5_SECONDS,
    STRING_1_SECOND,
    STRING_50_CONFIDENCE,
    STRING_20_CONFIDENCE,
    STRING_10_CONFIDENCE,
    STRING_5_CONFIDENCE,
    STRING_2_CONFIDENCE,
    STRING_1_CONFIDENCE,
};

static const char *onAverageMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.averaging);

    return getString(averageMenuOptions[index]);
}

static void onAverageMenuSelect(menu_size_t index)
{
    settings.averaging = index;
}

static MenuState averageMenuState;

const Menu averageMenu = {
    STRING_AVERAGE,
    &averageMenuState,
    ARRAY_SIZE(averageMenuOptions),
    onAverageMenuGetOption,
    onAverageMenuSelect,
    showMeasurementsMenu,
};
