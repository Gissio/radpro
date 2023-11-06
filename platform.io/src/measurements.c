/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <string.h>

#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "menu.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

#define MEASUREMENT_QUEUE_SIZE 32
#define MEASUREMENT_QUEUE_MASK (MEASUREMENT_QUEUE_SIZE - 1)

#define AVERAGING_PERIOD_TIME_CONSTANT 18.0F
#define AVERAGING_PERIOD_TIME_LIMIT 5.0F

struct Measurement
{
    uint32_t firstPulseTick;
    uint32_t lastPulseTick;
    uint32_t pulseCount;
};

struct Rate
{
    uint32_t time;
    float value;
    float lowerConfidenceInterval;
    float upperConfidenceInterval;
};

struct HistoryState
{
    float averageSum;
    uint32_t averageCount;

    uint8_t bufferIndex;
    uint8_t buffer[HISTORY_BUFFER_SIZE];
};

struct History
{
    char *const name;
    uint32_t samplesPerDataPoint;
};

static const struct History histories[] = {
    {"History (10 m)", 5},
    {"History (1 h)", 30},
    {"History (6 h)", 180},
    {"History (24 h)", 720},
    {"History (7 d)", 5040},
};

#define HISTORY_NUM (sizeof(histories) / sizeof(struct History))

static struct
{
    struct
    {
        struct Measurement measurement;

        uint32_t snapshotTick;
        struct Measurement snapshotMeasurement;

        float pulseCountFraction;
    } tick;

    struct
    {
        uint32_t queueTail;
        struct Measurement queue[MEASUREMENT_QUEUE_SIZE];

        struct Rate rate;
        float maxValue;

        bool isHold;
        struct Rate holdRate;
    } instantaneous;

    struct
    {
        uint32_t firstPulseTick;
        uint32_t lastPulseTick;
        uint32_t pulseCount;

        struct Rate rate;
        struct Rate timerRate;

        bool isHold;
        struct Rate holdRate;
    } average;

    struct
    {
        struct Dose dose;

        bool isHold;
        struct Dose holdDose;
    } cumulative;

    struct
    {
        struct Dose dose;
    } tube;

    struct
    {
        struct HistoryState states[HISTORY_NUM];

        uint32_t tabIndex;
        uint32_t sampleIndex;
    } history;

    int32_t viewIndex;
} measurements;

static const uint32_t averageTimerTimes[] = {
    30 * 24 * 60 * 60, // Off (actually 30 days)
    5 * 60,            // 5 minutes
    10 * 60,           // 10 minutes
    30 * 60,           // 30 minutes
    60 * 60,           // 60 minutes
};

static const struct Menu unitsMenu;
static const struct Menu rateAlarmMenu;
static const struct Menu doseAlarmMenu;
static const struct Menu averageTimerMenu;

// Measurements

const struct View *const measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &doseView,
    &historyView,
};

#define MEASUREMENT_VIEWS_NUM (sizeof(measurementViews) / sizeof(struct View *))

void initMeasurements(void)
{
    selectMenuIndex(&unitsMenu, settings.units, UNITS_SETTING_NUM);
    selectMenuIndex(&averageTimerMenu, settings.averageTimer, AVERAGE_TIMER_NUM);
    selectMenuIndex(&rateAlarmMenu, settings.rateAlarm, RATE_ALARM_NUM);
    selectMenuIndex(&doseAlarmMenu, settings.doseAlarm, DOSE_ALARM_NUM);

    updateTubeType();
}

// Events

static bool isInstantaneousRateAlarm(void);
static bool isDoseAlarm(void);

struct Unit
{
    char *const name;
    float scale;
    int8_t minMetricPower;
};

struct Units
{
    struct Unit rate;
    struct Unit dose;
};

static struct Units units[] = {
    {{"Sv/h", (60 * 1E-6F), 0},
     {"Sv", (60 * 1E-6F / 3600), 0}},
    {{"rem/h", (60 * 1E-4F), 0},
     {"rem", (60 * 1E-4F / 3600), 0}},
    {{"cpm", 60, 2},
     {"count", 1, 2}},
    {{"cps", 1, 2},
     {"count", 1, 2}},
};

void updateTubeType(void)
{
    float conversionFactor = getTubeCustomConversionFactor();

    units[0].rate.scale = (60 * 1E-6F) / conversionFactor;
    units[0].dose.scale = (60 * 1E-6F / 3600) / conversionFactor;
    units[1].rate.scale = (60 * 1E-4F) / conversionFactor;
    units[1].dose.scale = (60 * 1E-4F / 3600) / conversionFactor;
}

static float getTubeCompensationFactor(float value)
{
    if (!settings.tubeDeadTimeCompensation)
        return 1.0F;
    else
    {
        float denominator = 1 - value * getTubeDeadTimeCompensation();

        if (denominator > 0.1F)
            return 1.0F / denominator;
        else
            return 10.0F;
    }
}

static uint32_t calculateAveragingPeriod(float value)
{
    float uSvHScale = units[UNITS_SETTING_SIEVERTS].rate.scale * 1E6F;

    float uSvH = value * uSvHScale;
    float averagingPeriod;

    if (uSvH == 0.0F)
        averagingPeriod = 300;
    else if ((uSvH > 0.3F) && (uSvH < 1))
    {
        float averagingPeriodExponent =
            log10f((AVERAGING_PERIOD_TIME_CONSTANT / 0.3F /
                    AVERAGING_PERIOD_TIME_LIMIT) *
                   uSvHScale) /
            log10f(0.3F);

        averagingPeriod = (AVERAGING_PERIOD_TIME_LIMIT *
                           powf(uSvH, averagingPeriodExponent));
    }
    else
    {
        averagingPeriod = (AVERAGING_PERIOD_TIME_CONSTANT / value);

        if ((uSvH > 1) && (averagingPeriod > AVERAGING_PERIOD_TIME_LIMIT))
            averagingPeriod = AVERAGING_PERIOD_TIME_LIMIT;
    }

    return (uint32_t)averagingPeriod;
}

static void calculateRate(uint32_t pulseCount, uint32_t ticks, struct Rate *rate)
{
    if (!ticks)
        return;

    // Value and confidence intervals

    float value = (float)((pulseCount - 1) * SYS_TICK_FREQUENCY) / ticks;

    float lowerConfidenceInterval;
    float upperConfidenceInterval;
    getConfidenceIntervals(&lowerConfidenceInterval,
                           &upperConfidenceInterval,
                           pulseCount - 1);

    // Dead-time compensation

    // float absLowerConfidenceInterval = value * (1 + lowerConfidenceInterval);
    // float absUpperConfidenceInterval = value * (1 + upperConfidenceInterval);
    // float compValue = value * valueFactor;
    // float compAbsLowerConfidenceInterval = absLowerConfidenceInterval * lowerConfidenceIntervalFactor;
    // float compAbsUpperConfidenceInterval = absUpperConfidenceInterval * upperConfidenceIntervalFactor;
    // float compLowerConfidenceInterval = (compAbsLowerConfidenceInterval - compValue) / compValue;
    // float compUpperConfidenceInterval = (compAbsUpperConfidenceInterval - compValue) / compValue;
    float valueFactor = getTubeCompensationFactor(value);
    float lowerConfidenceIntervalFactor =
        getTubeCompensationFactor(value * (1 + lowerConfidenceInterval));
    float upperConfidenceIntervalFactor =
        getTubeCompensationFactor(value * (1 + upperConfidenceInterval));

    rate->value = valueFactor * value;
    rate->lowerConfidenceInterval =
        lowerConfidenceIntervalFactor / valueFactor *
            (1 + lowerConfidenceInterval) -
        1;
    rate->upperConfidenceInterval =
        upperConfidenceIntervalFactor / valueFactor *
            (1 + upperConfidenceInterval) -
        1;
}

void onMeasurementTick(uint32_t pulseCount)
{
    if (pulseCount)
    {
        if (!measurements.tick.measurement.pulseCount)
            measurements.tick.measurement.firstPulseTick = getTick();
        measurements.tick.measurement.lastPulseTick = getTick();
        measurements.tick.measurement.pulseCount += pulseCount;

        triggerPulse();
    }
}

void onMeasurementPeriod(void)
{
    measurements.tick.snapshotTick = getTick();
    measurements.tick.snapshotMeasurement = measurements.tick.measurement;
    measurements.tick.measurement.pulseCount = 0;
}

void updateMeasurements(void)
{
    // Dead-time compensation

    struct Measurement compensatedMeasurement;

    compensatedMeasurement.firstPulseTick =
        measurements.tick.snapshotMeasurement.firstPulseTick;
    compensatedMeasurement.lastPulseTick =
        measurements.tick.snapshotMeasurement.lastPulseTick;

    measurements.tick.pulseCountFraction +=
        measurements.tick.snapshotMeasurement.pulseCount *
        getTubeCompensationFactor(measurements.instantaneous.rate.value);
    compensatedMeasurement.pulseCount =
        (uint32_t)measurements.tick.pulseCountFraction;
    measurements.tick.pulseCountFraction -= compensatedMeasurement.pulseCount;

    // Instantaneous rate

    if (compensatedMeasurement.pulseCount)
    {
        measurements.instantaneous.queue[measurements.instantaneous.queueTail] =
            compensatedMeasurement;

        struct Measurement instantaneousMeasurement;
        instantaneousMeasurement.firstPulseTick = compensatedMeasurement.lastPulseTick;
        instantaneousMeasurement.lastPulseTick = compensatedMeasurement.lastPulseTick;
        instantaneousMeasurement.pulseCount = 0;

        uint32_t averagingPeriod =
            1000 * calculateAveragingPeriod(measurements.instantaneous.rate.value);

        bool clear = false;
        for (uint32_t i = MEASUREMENT_QUEUE_SIZE; i > 0; i--)
        {
            uint32_t queueIndex =
                (measurements.instantaneous.queueTail + i) & MEASUREMENT_QUEUE_MASK;

            if (!measurements.instantaneous.queue[queueIndex].pulseCount)
                break;

            if (!clear)
            {
                instantaneousMeasurement.pulseCount +=
                    measurements.instantaneous.queue[queueIndex].pulseCount;
                instantaneousMeasurement.firstPulseTick =
                    measurements.instantaneous.queue[queueIndex].firstPulseTick;

                uint32_t measurementPeriod = instantaneousMeasurement.lastPulseTick -
                                             instantaneousMeasurement.firstPulseTick;
                if (measurementPeriod >= averagingPeriod)
                    clear = true;
            }
            else
                measurements.instantaneous.queue[queueIndex].pulseCount = 0;
        }

        calculateRate(instantaneousMeasurement.pulseCount,
                      instantaneousMeasurement.lastPulseTick -
                          instantaneousMeasurement.firstPulseTick,
                      &measurements.instantaneous.rate);

        measurements.instantaneous.rate.time =
            1 + (measurements.tick.snapshotTick -
                 instantaneousMeasurement.firstPulseTick) /
                    SYS_TICK_FREQUENCY;

        measurements.instantaneous.queueTail =
            (measurements.instantaneous.queueTail + 1) & MEASUREMENT_QUEUE_MASK;
    }
    else
        measurements.instantaneous.rate.time++;

    // Average rate

    if ((measurements.average.rate.time < UINT32_MAX) &&
        (measurements.average.pulseCount < UINT32_MAX))
    {
        if (compensatedMeasurement.pulseCount)
        {
            if (!measurements.average.pulseCount)
                measurements.average.firstPulseTick =
                    compensatedMeasurement.firstPulseTick;

            measurements.average.lastPulseTick =
                compensatedMeasurement.lastPulseTick;

            addClamped(&measurements.average.pulseCount,
                       compensatedMeasurement.pulseCount);

            calculateRate(measurements.average.pulseCount,
                          measurements.average.lastPulseTick -
                              measurements.average.firstPulseTick,
                          &measurements.average.rate);
        }

        if ((measurements.average.rate.time < UINT32_MAX) &&
            (measurements.average.pulseCount < UINT32_MAX))
            measurements.average.rate.time++;

        if (measurements.average.rate.time <=
            averageTimerTimes[settings.averageTimer])
            measurements.average.timerRate = measurements.average.rate;
    }

    // Cumulative dose

    if ((measurements.cumulative.dose.time < UINT32_MAX) &&
        (measurements.cumulative.dose.pulseCount < UINT32_MAX))
    {
        addClamped(&measurements.cumulative.dose.time, 1);
        addClamped(&measurements.cumulative.dose.pulseCount,
                   compensatedMeasurement.pulseCount);
    }

    // Tube

    if ((measurements.tube.dose.time < UINT32_MAX) &&
        (measurements.tube.dose.pulseCount < UINT32_MAX))
    {
        addClamped(&measurements.tube.dose.time, 1);
        addClamped(&measurements.tube.dose.pulseCount,
                   compensatedMeasurement.pulseCount);
    }

    // History

    measurements.history.sampleIndex++;
    if (measurements.history.sampleIndex >
        histories[HISTORY_NUM - 1].samplesPerDataPoint)
        measurements.history.sampleIndex = 0;

    for (uint32_t i = 0; i < HISTORY_NUM; i++)
    {
        const struct History *history = &histories[i];
        struct HistoryState *historyState = &measurements.history.states[i];

        historyState->averageSum += measurements.instantaneous.rate.value;
        historyState->averageCount++;

        if ((measurements.history.sampleIndex %
             history->samplesPerDataPoint) == 0)
        {
            float averageRate = historyState->averageSum /
                                historyState->averageCount;
            int32_t value = (int32_t)(HISTORY_VALUE_DECADE *
                                      log10f(averageRate / HISTORY_CPS_MIN));
            if (value < 0)
                value = 0;
            else if (value > UCHAR_MAX)
                value = UCHAR_MAX;
            historyState->buffer[historyState->bufferIndex] = (uint8_t)value;

            historyState->averageSum = 0;
            historyState->averageCount = 0;

            historyState->bufferIndex =
                (historyState->bufferIndex + 1) & HISTORY_BUFFER_MASK;
        }
    }

    // Alarms

    if (isInstantaneousRateAlarm() || isDoseAlarm())
        triggerAlarm();
}

// View

static void drawTitleWithTime(const char *title, uint32_t time)
{
    char titleString[32];

    strcpy(titleString, title);
    strcat(titleString, " (");
    strcatTime(titleString, time);
    strcat(titleString, ")");

    drawTitle(titleString);
}

static void drawRate(float value,
                     float lowerConfidenceInterval,
                     float upperConfidenceInterval)
{
    char buffer[32] = "";
    struct Unit *rateUnit = &units[settings.units].rate;

    strcatFloatWithMetricPrefix(buffer,
                                value * rateUnit->scale,
                                rateUnit->minMetricPower);
    strcat(buffer, rateUnit->name);

    if (value != 0.0F)
        drawConfidenceIntervals(lowerConfidenceInterval,
                                upperConfidenceInterval);
    else
        strcpy(buffer, "\x80.\x80\x80\x80");

    buffer[5] = '\0';

    drawMeasurementValue(buffer, &buffer[6]);
}

static void drawDose(uint32_t value)
{
    char buffer[32] = "";
    struct Unit *doseUnit = &units[settings.units].dose;

    strcatFloatWithMetricPrefix(buffer,
                                value * doseUnit->scale,
                                doseUnit->minMetricPower);
    strcat(buffer, doseUnit->name);

    buffer[5] = '\0';

    drawMeasurementValue(buffer, &buffer[6]);
}

static void drawMaxRate(float value)
{
    char buffer[32];
    struct Unit *rateUnit = &units[settings.units].rate;

    strcpy(buffer, "Max: ");
    strcatFloatWithMetricPrefix(buffer,
                                value * rateUnit->scale,
                                rateUnit->minMetricPower);
    strcat(buffer, rateUnit->name);

    drawSubtitle(buffer);
}

void setMeasurementView(int32_t index)
{
    if (index >= 0)
        measurements.viewIndex = index;

    setView(measurementViews[measurements.viewIndex]);
}

static void onMeasurementEvent(const struct View *view, enum Event event)
{
    switch (event)
    {
    case EVENT_UP:
        measurements.viewIndex--;
        if (measurements.viewIndex < 0)
            measurements.viewIndex = MEASUREMENT_VIEWS_NUM - 1;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_DOWN:
        measurements.viewIndex++;
        if (measurements.viewIndex >= (int32_t)MEASUREMENT_VIEWS_NUM)
            measurements.viewIndex = 0;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_ENTER:
        setView(&settingsMenuView);

        break;

    default:
        break;
    }
}

// Instantaneous

static const float rateAlarmsSvH[] = {
    0,
    0.2E-6F,
    0.5E-6F,
    1E-6F,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
    50E-6F,
    100E-6F,
};

static void resetInstantaneousRate(void)
{
    memset(&measurements.instantaneous, 0, sizeof(measurements.instantaneous));
}

float getInstantaneousRate(void)
{
    return measurements.instantaneous.rate.value;
}

static bool isInstantaneousRateAlarm(void)
{
    if (!settings.rateAlarm)
        return false;

    float svH = units[UNITS_SETTING_SIEVERTS].rate.scale *
                measurements.instantaneous.rate.value;
    return svH >= rateAlarmsSvH[settings.rateAlarm];
}

static void onInstantaneousRateViewEvent(const struct View *view,
                                         enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_BACK:
        measurements.instantaneous.isHold = !measurements.instantaneous.isHold;

        if (measurements.instantaneous.isHold)
            measurements.instantaneous.holdRate = measurements.instantaneous.rate;

        refreshView();

        break;

    case EVENT_RESET:
        resetInstantaneousRate();

        refreshView();

        break;

    case EVENT_DRAW:
    {
        struct Rate rate;
        if (measurements.instantaneous.isHold)
            rate = measurements.instantaneous.holdRate;
        else
            rate = measurements.instantaneous.rate;

        drawTitleWithTime("Instantaneous", rate.time);
        drawRate(rate.value, rate.lowerConfidenceInterval,
                 rate.upperConfidenceInterval);

        if (measurements.instantaneous.isHold)
            drawSubtitle("HOLD");
        else if (isInstantaneousRateAlarm())
            drawSubtitle("RATE ALARM");
        else if (measurements.instantaneous.maxValue > 0)
            drawMaxRate(measurements.instantaneous.maxValue);

        break;
    }

    default:
        break;
    }
}

const struct View instantaneousRateView = {
    onInstantaneousRateViewEvent,
    NULL,
};

// Average

static void resetAverageRate(void)
{
    memset(&measurements.average, 0, sizeof(measurements.average));
}

static void onAverageRateViewEvent(const struct View *view,
                                   enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_BACK:
        if (measurements.average.isHold ||
            (measurements.average.rate.time <
             averageTimerTimes[settings.averageTimer]))
        {
            measurements.average.isHold = !measurements.average.isHold;

            if (measurements.average.isHold)
                measurements.average.holdRate = measurements.average.rate;

            refreshView();
        }

        break;

    case EVENT_RESET:
        resetAverageRate();

        refreshView();

        break;

    case EVENT_DRAW:
    {
        struct Rate rate;
        if (measurements.average.isHold)
            rate = measurements.average.holdRate;
        else
            rate = measurements.average.timerRate;

        drawTitleWithTime("Average", rate.time);
        drawRate(rate.value, rate.lowerConfidenceInterval,
                 rate.upperConfidenceInterval);

        if (measurements.average.isHold)
            drawSubtitle("HOLD");
        else if (measurements.average.rate.time >=
                 averageTimerTimes[settings.averageTimer])
            drawSubtitle("DONE");
        else if (measurements.average.pulseCount == UINT32_MAX)
            drawSubtitle("OVERFLOW");

        break;
    }

    default:
        break;
    }
}

const struct View averageRateView = {
    onAverageRateViewEvent,
    NULL,
};

// Dose

static const float doseAlarmsSv[] = {
    0,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
    50E-6F,
    100E-6F,
    200E-6F,
    500E-6F,
    1000E-6F,
};

void setDoseTime(uint32_t value)
{
    measurements.cumulative.dose.time = value;
}

uint32_t getDoseTime(void)
{
    return measurements.cumulative.dose.time;
}

void setDosePulseCount(uint32_t value)
{
    measurements.cumulative.dose.pulseCount = value;
}

uint32_t getDosePulseCount(void)
{
    return measurements.cumulative.dose.pulseCount;
}

static void resetDose(void)
{
    memset(&measurements.cumulative, 0, sizeof(measurements.cumulative));
}

static bool isDoseAlarm(void)
{
    if (!settings.doseAlarm)
        return false;

    float doseSv = units[UNITS_SETTING_SIEVERTS].dose.scale *
                   measurements.cumulative.dose.pulseCount;
    return doseSv >= doseAlarmsSv[settings.doseAlarm];
}

static void onDoseViewEvent(const struct View *view,
                            enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_BACK:
        measurements.cumulative.isHold = !measurements.cumulative.isHold;

        if (measurements.cumulative.isHold)
            measurements.cumulative.holdDose = measurements.cumulative.dose;

        refreshView();

        break;

    case EVENT_RESET:
        resetDose();

        refreshView();

        break;

    case EVENT_DRAW:
    {
        struct Dose dose;

        if (!measurements.cumulative.isHold)
            dose = measurements.cumulative.dose;
        else
            dose = measurements.cumulative.holdDose;

        drawTitleWithTime("Dose", dose.time);
        drawDose(dose.pulseCount);

        if (measurements.cumulative.isHold)
            drawSubtitle("HOLD");
        else if (dose.pulseCount == UINT32_MAX)
            drawSubtitle("OVERFLOW");
        else if (isDoseAlarm())
            drawSubtitle("DOSE ALARM");

        break;
    }

    default:
        break;
    }
}

const struct View doseView = {
    onDoseViewEvent,
    NULL,
};

// Tube

void setTubeTime(uint32_t value)
{
    measurements.tube.dose.time = value;
}

uint32_t getTubeTime(void)
{
    return measurements.tube.dose.time;
}

void setTubePulseCount(uint32_t value)
{
    measurements.tube.dose.pulseCount = value;
}

uint32_t getTubePulseCount(void)
{
    return measurements.tube.dose.pulseCount;
}

// History

static void resetHistory(void)
{
    memset(&measurements.history.states, 0,
           sizeof(measurements.history.states));

    measurements.history.sampleIndex = 0;
}

uint32_t getHistoryDataPoint(uint32_t dataIndex)
{
    struct HistoryState *historyState =
        &measurements.history.states[measurements.history.tabIndex];

    uint32_t bufferIndex = ((historyState->bufferIndex - 1) - dataIndex) &
                           HISTORY_BUFFER_MASK;

    return historyState->buffer[bufferIndex];
}

static void onHistoryViewEvent(const struct View *view, enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_BACK:
        measurements.history.tabIndex++;
        if (measurements.history.tabIndex >= HISTORY_NUM)
            measurements.history.tabIndex = 0;

        refreshView();

        break;

    case EVENT_RESET:
        resetHistory();

        refreshView();

        break;

    case EVENT_DRAW:
    {
        uint32_t minValue = UCHAR_MAX;
        uint32_t maxValue = 0;

        for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
        {
            uint32_t value = getHistoryDataPoint(i);
            if (value > 0)
            {
                if (value < minValue)
                    minValue = value;
                if (value > maxValue)
                    maxValue = value;
            }
        }

        char minLabel[16] = "";
        char maxLabel[16] = "";
        int32_t minLimit = 0;
        uint32_t decades = 1;

        if (maxValue > 0)
        {
            struct Unit *rateUnit = &units[settings.units].rate;

            uint32_t cpsToRateUnit =
                (uint32_t)(HISTORY_VALUE_DECADE *
                           (16 + log10f(rateUnit->scale * HISTORY_CPS_MIN)));

            int32_t exponentMin =
                (int32_t)((cpsToRateUnit + minValue) /
                              HISTORY_VALUE_DECADE -
                          16);
            int32_t exponentMax =
                (int32_t)((cpsToRateUnit + maxValue) /
                              HISTORY_VALUE_DECADE -
                          16 + 1);

            minLimit = (int32_t)((exponentMin + 16) * HISTORY_VALUE_DECADE -
                                 cpsToRateUnit);
            decades = exponentMax - exponentMin;

            strcatDecimalPowerWithMetricPrefix(minLabel, exponentMin);
            strcat(minLabel, rateUnit->name);

            strcatDecimalPowerWithMetricPrefix(maxLabel, exponentMax);
            strcat(maxLabel, rateUnit->name);
        }

        drawTitle(histories[measurements.history.tabIndex].name);

        drawHistory(minLabel, maxLabel, minLimit, decades);

        break;
    }

    default:
        break;
    }
}

const struct View historyView = {
    onHistoryViewEvent,
    NULL,
};

// Units menu

static void onUnitsMenuSelect(const struct Menu *menu)
{
    settings.units = menu->state->selectedIndex;
}

static const char *const unitsMenuOptions[] = {
    "Sievert",
    "rem",
    "cpm",
    "cps",
    NULL,
};

static struct MenuState unitsMenuState;

static const struct Menu unitsMenu = {
    "Units",
    &unitsMenuState,
    onMenuGetOption,
    unitsMenuOptions,
    onUnitsMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View unitsMenuView = {
    onMenuEvent,
    &unitsMenu,
};

// Average timer menu

static void onAverageTimerMenuSelect(const struct Menu *menu)
{
    settings.averageTimer = menu->state->selectedIndex;
}

static const char *const averageTimerMenuOptions[] = {
    "Off",
    "5 minutes",
    "10 minutes",
    "30 minutes",
    "60 minutes",
    NULL,
};

static struct MenuState averageTimerMenuState;

static const struct Menu averageTimerMenu = {
    "Average timer",
    &averageTimerMenuState,
    onMenuGetOption,
    averageTimerMenuOptions,
    onAverageTimerMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View averageTimerMenuView = {
    onMenuEvent,
    &averageTimerMenu,
};

// Rate alarm menu

static const char *onRateAlarmMenuGetOption(const struct Menu *menu,
                                            uint32_t index)
{
    if (index == 0)
        return "Off";
    else if (index < RATE_ALARM_NUM)
    {
        struct Unit *rateUnit = &units[settings.units].rate;
        float value = rateAlarmsSvH[index] /
                      units[UNITS_SETTING_SIEVERTS].rate.scale;

        strcpy(menuOption, "");
        strcatFloatWithMetricPrefix(menuOption,
                                    rateUnit->scale * value,
                                    rateUnit->minMetricPower);
        strcat(menuOption, rateUnit->name);

        return menuOption;
    }
    else
        return NULL;
}

static void onRateAlarmMenuSelect(const struct Menu *menu)
{
    settings.rateAlarm = menu->state->selectedIndex;
}

static struct MenuState rateAlarmMenuState;

static const struct Menu rateAlarmMenu = {
    "Rate alarm",
    &rateAlarmMenuState,
    onRateAlarmMenuGetOption,
    NULL,
    onRateAlarmMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View rateAlarmMenuView = {
    onMenuEvent,
    &rateAlarmMenu,
};

// Dose alarm menu

static const char *onDoseAlarmMenuGetOption(const struct Menu *menu,
                                            uint32_t index)
{
    if (index == 0)
        return "Off";
    else if (index < DOSE_ALARM_NUM)
    {
        struct Unit *doseUnit = &units[settings.units].dose;
        float value = doseAlarmsSv[index] /
                      units[UNITS_SETTING_SIEVERTS].dose.scale;

        strcpy(menuOption, "");
        strcatFloatWithMetricPrefix(menuOption,
                                    doseUnit->scale * value,
                                    doseUnit->minMetricPower);
        strcat(menuOption, doseUnit->name);

        return menuOption;
    }
    else
        return NULL;
}

static void onDoseAlarmMenuSelect(const struct Menu *menu)
{
    settings.doseAlarm = menu->state->selectedIndex;
}

static struct MenuState doseAlarmMenuState;

static const struct Menu doseAlarmMenu = {
    "Dose alarm",
    &doseAlarmMenuState,
    onDoseAlarmMenuGetOption,
    NULL,
    onDoseAlarmMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};
