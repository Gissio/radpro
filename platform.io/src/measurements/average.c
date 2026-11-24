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

static void clearAverageRate(void);

#if !defined(RADPRO_RAW_COUNTER)
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
#endif

static struct
{
    PulsePeriod period;
    Rate rate;

#if !defined(RADPRO_RAW_COUNTER)
    uint32_t snapshotPulseCount;
    Rate snapshotRate;
    bool done;
#endif
} average;

static AverageTab averageTab;

void resetAverageRate(void)
{
    averageTab = AVERAGE_TAB_TIME;

    clearAverageRate();

#if !defined(RADPRO_RAW_COUNTER)
    selectMenuItem(&averageMenu, settings.averaging);
#endif
}

static void clearAverageRate(void)
{
    memset(&average, 0, sizeof(average));

    if (averageTab == AVERAGE_TAB_ALERT)
        averageTab = AVERAGE_TAB_TIME;
}

static bool isAverageSaturated(void)
{
    return (average.rate.time == UINT32_MAX) || (average.period.pulseCount == UINT32_MAX);
}

#if !defined(RADPRO_RAW_COUNTER)
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
#endif

static void updateAverageRateTab(bool doneTriggered, bool done)
{
    if (isTubeFaultAlertTriggered() || doneTriggered)
        averageTab = AVERAGE_TAB_ALERT;
    else if (averageTab == AVERAGE_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel() && !done)
            averageTab = AVERAGE_TAB_TIME;
    }
}

void updateAverageRate(PulsePeriod *period)
{
    if (isAverageSaturated())
        return;

    // Update average rate
#if defined(RADPRO_RAW_COUNTER)
    average.rate.time = addClamped(average.rate.time, period->seconds);
    average.period.pulseCount = addClamped(average.period.pulseCount, period->pulseCount);
    average.rate.value = (float)average.period.pulseCount / average.rate.time;
    average.rate.confidence = average.period.pulseCount ? getConfidenceInterval(average.period.pulseCount) : 0;
    updateAverageRateTab(false, false);
#else
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

    updateAverageRateTab(doneTriggered, average.done);
#endif
}

float getAverageRate(void)
{
#if defined(RADPRO_RAW_COUNTER)
    return average.rate.value;
#else
    return average.snapshotRate.value;
#endif
}

// Average rate view

NOINLINE static const char *getAverageRateAlertString(void)
{
    if (getTubeFaultAlertLevel())
        return getString(STRING_ALERT_FAULT);
#if !defined(RADPRO_RAW_COUNTER)
    if (average.done)
        return getString(STRING_ALERT_DONE);
#endif

    return NULL;
}

static void advanceAverageRateTab(void)
{
    uint32_t tabNum = getAverageRateAlertString() ? (AVERAGE_TAB_NUM + 1) : AVERAGE_TAB_NUM;

    averageTab++;
    if (averageTab >= tabNum)
        averageTab = AVERAGE_TAB_TIME;
}

static void drawAverageRateTab(const Rate *rate, uint32_t pulseCount, MeasurementStyle style)
{
    char valueString[32];
    char unitString[32];
    strclr(valueString);
    strcpy(unitString, " ");
    const char *keyString = NULL;

    switch (averageTab)
    {
    case AVERAGE_TAB_TIME:
        strcatTime(valueString, rate->time);

        keyString = getString(STRING_TIME);

        break;

    case AVERAGE_TAB_RATE:
        buildPulseValueString(valueString, unitString, rate->value, settings.secondaryDoseUnits, false);

        keyString = getString(STRING_RATE);

        break;

    case AVERAGE_TAB_DOSE:
        buildPulseValueString(valueString, unitString, pulseCount, DOSE_UNITS_CPM, true);

#if defined(RADPRO_RAW_COUNTER)
        keyString = getString(STRING_PULSES);
#else
        keyString = getString(STRING_DOSE);
#endif

        break;

    default:
        break;
    }

    drawMeasurementInfo(keyString, valueString, unitString, style);
}

static void drawAverageRateView(void)
{
#if defined(RADPRO_RAW_COUNTER)
    const Rate *rate = &average.rate;
    uint32_t pulseCount = average.period.pulseCount;
    bool saturated = isAverageSaturated();
    MeasurementStyle style = saturated ? MEASUREMENTSTYLE_DONE : MEASUREMENTSTYLE_NORMAL;
#else
    const Rate *rate = &average.snapshotRate;
    uint32_t pulseCount = average.snapshotPulseCount;
    MeasurementStyle style = average.done ? MEASUREMENTSTYLE_DONE : MEASUREMENTSTYLE_NORMAL;
#endif
    drawPulseMeasurementValue(getString(STRING_AVERAGE), rate->value, false, rate->confidence, style);

    if (averageTab == AVERAGE_TAB_ALERT)
        drawMeasurementAlert(getAverageRateAlertString());
    else
        drawAverageRateTab(rate, pulseCount, style);
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

        clearAverageRate();

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

#if !defined(RADPRO_RAW_COUNTER)
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
#endif
