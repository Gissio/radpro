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
    uint32_t pulseCount;
    Rate rate;

    uint32_t snapshotPulseCount;
    Rate snapshotRate;
    bool done;
} average;

static AverageTab averageTab;

void resetAverageRate(void)
{
    averageTab = AVERAGE_TAB_TIME;

    clearAverageRate();

    selectMenuItem(&averageMenu, settings.averaging);
}

void clearAverageRate(void)
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

static float getAveragingConfidenceThreshold(void)
{
    return averagingConfidences[settings.averaging - AVERAGING_TIME_NUM];
}

static bool isAverageSaturated(void)
{
    return (average.rate.time == UINT32_MAX) || (average.pulseCount == UINT32_MAX);
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

void updateAverageRate(uint32_t pulseCount)
{
    if (isAverageSaturated())
        return;

    // Update average rate: n / t for timed averaging; confidence averaging
    // stops on the pulse count, so (n - 1) / t is unbiased
    average.rate.time = addClamped(average.rate.time, 1);
    average.pulseCount = addClamped(average.pulseCount, pulseCount);

    uint32_t n = average.pulseCount;
    if (n && !isTimeAveraging())
        n--;
    calculateRate(&average.rate, n, average.rate.time);

    // Snapshot is frozen once done
    bool previousDone = average.done;
    average.done = isAverageDone();

    if (!previousDone || !average.done)
    {
        average.snapshotPulseCount = average.pulseCount;
        average.snapshotRate = average.rate;
    }

    bool doneTriggered = (!previousDone && average.done);
    if (doneTriggered)
        triggerAlert(true);

    averageTab = updateMeasurementAlertTab(averageTab, AVERAGE_TAB_ALERT,
                                           isTubeFaultAlertTriggered() || doneTriggered,
                                           getTubeFaultAlertLevel() || average.done);
}

float getAverageRate(void)
{
    return average.snapshotRate.value;
}

// Average rate view

NOINLINE static const char *getAverageRateAlertString(void)
{
    if (getTubeFaultAlertLevel())
        return getString(STRING_ALERT_FAULT);
    else if (average.done)
        return getString(STRING_ALERT_DONE);
    else
        return NULL;
}

void advanceAverageRateTab(void)
{
    averageTab = getNextMeasurementTab(averageTab,
                                       AVERAGE_TAB_NUM + (getAverageRateAlertString() != NULL));
}

void drawAverageRateView(void)
{
    MeasurementStyle style = average.done ? MEASUREMENTSTYLE_DONE : MEASUREMENTSTYLE_NORMAL;

    drawPulseMeasurementValue(getString(STRING_AVERAGE), average.snapshotRate.value, false, average.snapshotRate.confidence, style);

    switch (averageTab)
    {
    case AVERAGE_TAB_TIME:
        drawMeasurementInfoTime(getString(STRING_TIME), average.snapshotRate.time, style);

        break;

    case AVERAGE_TAB_RATE:
        drawPulseMeasurementInfo(getString(STRING_RATE), average.snapshotRate.value, settings.secondaryDoseUnits, false, style);

        break;

    case AVERAGE_TAB_DOSE:
        // Pulse count
        drawPulseMeasurementInfo(getString(STRING_DOSE), average.snapshotPulseCount, DOSE_UNITS_CPM, true, style);

        break;

    default:
        drawMeasurementAlert(getAverageRateAlertString());

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
