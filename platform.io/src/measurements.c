/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <string.h>

#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "measurements.h"
#include "menu.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

#define MEASUREMENT_QUEUE_SIZE 32
#define MEASUREMENT_QUEUE_MASK (MEASUREMENT_QUEUE_SIZE - 1)

#define AVERAGING_PERIOD_TIME_CONSTANT 18.0F
#define AVERAGING_PERIOD_TIME_LIMIT 5.0F

#if defined(DISPLAY_128X64)
#define HISTORY_BUFFER_SIZE 120
#elif defined(DISPLAY_320X240)
#define HISTORY_BUFFER_SIZE 300
#elif defined(DISPLAY_240X320)
#define HISTORY_BUFFER_SIZE 200
#endif

#define HISTORY_CPS_MIN 0.02F
#define HISTORY_VALUE_DECADE 40

typedef struct
{
    uint32_t firstPulseTick;
    uint32_t lastPulseTick;
    uint32_t pulseCount;
} Measurement;

typedef struct
{
    uint32_t time;
    float value;
    float confidence;
} Rate;

typedef struct
{
    float averageSum;
    uint32_t averageCount;

    uint16_t bufferIndex;
    uint8_t buffer[HISTORY_BUFFER_SIZE];
} HistoryState;

typedef struct
{
    char *const name;
    uint16_t samplesPerDataPoint;
    uint8_t xTickNum;
} History;

static const History histories[] = {
    {"History (10 m)", 10 * 60 / HISTORY_BUFFER_SIZE, 10},
    {"History (1 h)", 60 * 60 / HISTORY_BUFFER_SIZE, 6},
    {"History (24 h)", 24 * 60 * 60 / HISTORY_BUFFER_SIZE, 6},
};

#define HISTORY_NUM (sizeof(histories) / sizeof(History))

static struct
{
    struct
    {
        Measurement measurement;

        uint32_t snapshotTick;
        Measurement snapshotMeasurement;

        float pulseCountFraction;
    } tick;

    struct
    {
        uint32_t queueTail;
        Measurement queue[MEASUREMENT_QUEUE_SIZE];

        Rate rate;
        float maxValue;

        bool isHold;
        Rate holdRate;
    } instantaneous;

    struct
    {
        uint32_t firstPulseTick;
        uint32_t lastPulseTick;
        uint32_t pulseCount;

        Rate rate;
        Rate timerRate;

        bool isHold;
        Rate holdRate;
    } average;

    struct
    {
        Dose dose;

        bool isHold;
        Dose holdDose;
    } cumulative;

    struct
    {
        Dose dose;
    } tube;

    struct
    {
        HistoryState states[HISTORY_NUM];

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

static const Menu unitsMenu;
static const Menu rateAlarmMenu;
static const Menu doseAlarmMenu;
static const Menu averageTimerMenu;

// Measurements

const View *const measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &doseView,
    &historyView,
};

#define MEASUREMENT_VIEWS_NUM (sizeof(measurementViews) / sizeof(View *))

void initMeasurements(void)
{
    selectMenuItem(&unitsMenu,
                   settings.units,
                   UNITS_NUM);
    selectMenuItem(&averageTimerMenu,
                   settings.averageTimer,
                   AVERAGETIMER_NUM);
    selectMenuItem(&rateAlarmMenu,
                   settings.rateAlarm,
                   RATEALARM_NUM);
    selectMenuItem(&doseAlarmMenu,
                   settings.doseAlarm,
                   DOSEALARM_NUM);

    updateTube();
}

// Events

static bool isInstantaneousRateAlarm(void);
static bool isDoseAlarm(void);

typedef struct
{
    char *const name;
    float scale;
    int8_t minMetricPower;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} Units;

static Units units[] = {
    {{"Sv/h", (60 * 1E-6F), 0},
     {"Sv", (60 * 1E-6F / 3600), 0}},
    {{"rem/h", (60 * 1E-4F), 0},
     {"rem", (60 * 1E-4F / 3600), 0}},
    {{"cpm", 60, 2},
     {"count", 1, 2}},
    {{"cps", 1, 2},
     {"count", 1, 2}},
};

void updateTube(void)
{
    float conversionFactor = getTubeConversionFactor();

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
    float uSvHScale = units[UNITS_SIEVERTS].rate.scale * 1E6F;

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

static void calculateRate(uint32_t pulseCount, uint32_t ticks, Rate *rate)
{
    if (!ticks)
        return;

    // Value and confidence intervals

    float value = (float)((pulseCount - 1) * SYSTICK_FREQUENCY) / ticks;

    float confidenceInterval =
        getConfidenceInterval(pulseCount - 1);

    // Dead-time compensation

    // float absConfidenceInterval = value * (1 + confidenceInterval);
    // float compValue = value * valueFactor;
    // float compAbsConfidenceInterval = absConfidenceInterval * confidenceIntervalFactor;
    // float compConfidenceInterval = (compAbsConfidenceInterval - compValue) / compValue;
    float valueFactor = getTubeCompensationFactor(value);
    float confidenceIntervalFactor =
        getTubeCompensationFactor(value * (1 + confidenceInterval));

    rate->value = valueFactor * value;
    rate->confidence =
        confidenceIntervalFactor / valueFactor *
            (1 + confidenceInterval) -
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

void onMeasurementsOneSecond(void)
{
    // Dead-time compensation

    Measurement compensatedMeasurement;

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

        Measurement instantaneousMeasurement;
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
                    SYSTICK_FREQUENCY;

        measurements.instantaneous.queueTail =
            (measurements.instantaneous.queueTail + 1) & MEASUREMENT_QUEUE_MASK;

        if ((instantaneousMeasurement.pulseCount > 10) &&
            (measurements.instantaneous.rate.value >
             measurements.instantaneous.maxValue))
            measurements.instantaneous.maxValue =
                measurements.instantaneous.rate.value;
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
        {
            measurements.average.timerRate = measurements.average.rate;

            if (measurements.average.rate.time ==
                averageTimerTimes[settings.averageTimer])
                triggerAlarm();
        }
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
        const History *history = &histories[i];
        HistoryState *historyState = &measurements.history.states[i];

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

            historyState->bufferIndex++;
            if (historyState->bufferIndex >= HISTORY_BUFFER_SIZE)
                historyState->bufferIndex = 0;
        }
    }

    // Alarms

    if (isInstantaneousRateAlarm() || isDoseAlarm())
        triggerAlarm();
}

// View

static void buildValueString(char *str,
                             char **unitString,
                             float value,
                             const Unit *unit)
{
    strcpy(str, "");
    strcatFloatWithMetricPrefix(str,
                                value * unit->scale,
                                unit->minMetricPower);
    strcat(str, unit->name);
    if (value == 0.0F)
        strcpy(str, "\x7f.\x7f\x7f\x7f");
    str[5] = '\0';

    *unitString = &str[6];
}

void setMeasurementView(int32_t index)
{
    if (index >= 0)
        measurements.viewIndex = index;

    setKeyboardDelayedBack(true);

    setView(measurementViews[measurements.viewIndex]);
}

static void onMeasurementEvent(const View *view, enum Event event)
{
    switch (event)
    {
    case EVENT_KEY_UP:
        measurements.viewIndex--;
        if (measurements.viewIndex < 0)
            measurements.viewIndex = MEASUREMENT_VIEWS_NUM - 1;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_KEY_DOWN:
        measurements.viewIndex++;
        if (measurements.viewIndex >= (int32_t)MEASUREMENT_VIEWS_NUM)
            measurements.viewIndex = 0;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_KEY_ENTER:
        setKeyboardDelayedBack(false);

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

    float svH = units[UNITS_SIEVERTS].rate.scale *
                measurements.instantaneous.rate.value;
    return svH >= rateAlarmsSvH[settings.rateAlarm];
}

// +++ TEST
static void onInstantaneousRateViewEvent(const View *view2,
// +++ TEST
                                         enum Event event)
{
    onMeasurementEvent(view2, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.instantaneous.isHold = !measurements.instantaneous.isHold;

        if (measurements.instantaneous.isHold)
            measurements.instantaneous.holdRate = measurements.instantaneous.rate;

        updateView();

        break;

    case EVENT_KEY_RESET:
        resetInstantaneousRate();

        updateView();

        break;

    case EVENT_DRAW:
    {
        Rate rate;
        if (measurements.instantaneous.isHold)
            rate = measurements.instantaneous.holdRate;
        else
            rate = measurements.instantaneous.rate;

        char valueString[32];
        char *unitString;
        char timeString[16];
        char *stateString = "";
        char stateValueString[32];
        char stateUnitString[16];

        buildValueString(valueString,
                         &unitString,
                         rate.value,
                         &units[settings.units].rate);

        strcpy(timeString, "");
        strcatTime(timeString, rate.time);

        strcpy(stateValueString, "");
        strcpy(stateUnitString, "");

        enum MeasurementStyle style = MEASUREMENTSTYLE_NORMAL;

        if (measurements.instantaneous.isHold)
        {
            stateString = "Hold";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (isInstantaneousRateAlarm())
        {
            stateString = "Rate alarm";

            style = MEASUREMENTSTYLE_ALARM;
        }
        else if (measurements.instantaneous.maxValue > 0)
        {
            stateString = "Max";

            char *unit;

            buildValueString(stateValueString,
                             &unit,
                             measurements.instantaneous.maxValue,
                             &units[settings.units].rate);

            strcpy(stateUnitString, " ");
            strcat(stateUnitString, unit);
        }

        drawMeasurement("Instantaneous",
                        valueString,
                        unitString,
                        rate.confidence,
                        timeString,
                        stateString,
                        stateValueString,
                        stateUnitString,
                        style);

        break;
    }

    default:
        break;
    }
}

const View instantaneousRateView = {
    onInstantaneousRateViewEvent,
    NULL,
};

// Average

static void resetAverageRate(void)
{
    memset(&measurements.average, 0, sizeof(measurements.average));
}

static void onAverageRateViewEvent(const View *view,
                                   enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        if (measurements.average.isHold ||
            (measurements.average.rate.time <
             averageTimerTimes[settings.averageTimer]))
        {
            measurements.average.isHold = !measurements.average.isHold;

            if (measurements.average.isHold)
                measurements.average.holdRate = measurements.average.rate;

            updateView();
        }

        break;

    case EVENT_KEY_RESET:
        resetAverageRate();

        updateView();

        break;

    case EVENT_DRAW:
    {
        Rate rate;
        if (measurements.average.isHold)
            rate = measurements.average.holdRate;
        else
            rate = measurements.average.timerRate;

        char timeString[16];
        char valueString[32];
        char *unitString;
        char *stateString;

        strcpy(timeString, "");
        strcatTime(timeString, rate.time);

        buildValueString(valueString,
                         &unitString,
                         rate.value,
                         &units[settings.units].rate);

        stateString = "";
        enum MeasurementStyle style = MEASUREMENTSTYLE_NORMAL;

        if (measurements.average.isHold)
        {
            stateString = "Hold";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (measurements.average.rate.time >=
                 averageTimerTimes[settings.averageTimer])
        {
            stateString = "Done";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (measurements.average.pulseCount == UINT32_MAX)
        {
            stateString = "Overflow";

            style = MEASUREMENTSTYLE_HOLD;
        }

        drawMeasurement("Average",
                        valueString,
                        unitString,
                        rate.confidence,
                        timeString,
                        stateString,
                        "",
                        "",
                        style);

        break;
    }

    default:
        break;
    }
}

const View averageRateView = {
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

    float doseSv = units[UNITS_SIEVERTS].dose.scale *
                   measurements.cumulative.dose.pulseCount;
    return doseSv >= doseAlarmsSv[settings.doseAlarm];
}

static void onDoseViewEvent(const View *view,
                            enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.cumulative.isHold = !measurements.cumulative.isHold;

        if (measurements.cumulative.isHold)
            measurements.cumulative.holdDose = measurements.cumulative.dose;

        updateView();

        break;

    case EVENT_KEY_RESET:
        resetDose();

        updateView();

        break;

    case EVENT_DRAW:
    {
        Dose dose;

        if (!measurements.cumulative.isHold)
            dose = measurements.cumulative.dose;
        else
            dose = measurements.cumulative.holdDose;

        char timeString[16];
        char valueString[32];
        char *unitString;
        char *stateString;

        strcpy(timeString, "");
        strcatTime(timeString, dose.time);

        buildValueString(valueString,
                         &unitString,
                         dose.pulseCount,
                         &units[settings.units].dose);

        stateString = "";
        enum MeasurementStyle style = MEASUREMENTSTYLE_NORMAL;

        if (measurements.cumulative.isHold)
        {
            stateString = "Hold";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (dose.pulseCount == UINT32_MAX)
        {
            stateString = "Overflow";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (isDoseAlarm())
        {
            stateString = "Dose alarm";

            style = MEASUREMENTSTYLE_ALARM;
        }

        drawMeasurement("Dose",
                        valueString,
                        unitString,
                        0,
                        timeString,
                        stateString,
                        "",
                        "",
                        style);

        break;
    }

    default:
        break;
    }
}

const View doseView = {
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

static void onHistoryViewEvent(const View *view, enum Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.history.tabIndex++;

        if (measurements.history.tabIndex >= HISTORY_NUM)
            measurements.history.tabIndex = 0;

        updateView();

        break;

    case EVENT_KEY_RESET:
        resetHistory();

        updateView();

        break;

    case EVENT_DRAW:
    {
        HistoryState *historyState =
            &measurements.history.states[measurements.history.tabIndex];

        uint32_t valueMax = 0;
        uint32_t valueMin = UCHAR_MAX;
        for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
        {
            uint32_t value = historyState->buffer[i];

            if (!value)
                continue;

            if (value > valueMax)
                valueMax = value;

            if (value < valueMin)
                valueMin = value;
        }

        char bottomLegendString[16] = "";
        char topLegendString[16] = "";
        int32_t exponentValueMin = 0;
        uint32_t yTickNum = 1;

        if (valueMax > 0)
        {
            Unit *rateUnit = &units[settings.units].rate;

            uint32_t cpsToRateUnit =
                (uint32_t)(HISTORY_VALUE_DECADE *
                           (16 + log10f(rateUnit->scale * HISTORY_CPS_MIN)));

            int32_t exponentMax =
                (int32_t)((cpsToRateUnit + valueMax) /
                              HISTORY_VALUE_DECADE -
                          16 + 1);
            int32_t exponentMin =
                (int32_t)((cpsToRateUnit + valueMin) /
                              HISTORY_VALUE_DECADE -
                          16);

            exponentValueMin = (int32_t)((exponentMin + 16) * HISTORY_VALUE_DECADE -
                                         cpsToRateUnit);
            yTickNum = exponentMax - exponentMin;

            strcatDecimalPowerWithMetricPrefix(topLegendString, exponentMax);
            strcat(topLegendString, rateUnit->name);

            strcatDecimalPowerWithMetricPrefix(bottomLegendString, exponentMin);
            strcat(bottomLegendString, rateUnit->name);
        }

        uint8_t data[HISTORY_BUFFER_SIZE];
        uint32_t valueRange = HISTORY_VALUE_DECADE * yTickNum;

        for (uint32_t dataIndex = 0;
             dataIndex < HISTORY_BUFFER_SIZE;
             dataIndex++)
        {
            uint32_t bufferIndex = 2 * HISTORY_BUFFER_SIZE -
                                   ((HISTORY_BUFFER_SIZE - 1) - dataIndex) +
                                   (historyState->bufferIndex - 1);
            while (bufferIndex >= HISTORY_BUFFER_SIZE)
                bufferIndex -= HISTORY_BUFFER_SIZE;

            uint32_t value = historyState->buffer[bufferIndex];

            if (value)
                data[dataIndex] = 255 * (value - exponentValueMin) /
                                  valueRange;
            else
                data[dataIndex] = 0;
        }

        drawHistory(histories[measurements.history.tabIndex].name,
                    topLegendString,
                    bottomLegendString,
                    data,
                    histories[measurements.history.tabIndex].xTickNum,
                    yTickNum);

        break;
    }

    default:
        break;
    }
}

const View historyView = {
    onHistoryViewEvent,
    NULL,
};

// Units menu

static const char *const unitsMenuOptions[] = {
    "Sievert",
    "rem",
    "cpm",
    "cps",
    NULL,
};

static const char *onUnitsMenuGetOption(const Menu *menu,
                                        uint32_t index,
                                        MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.units);

    return unitsMenuOptions[index];
}

static void onUnitsMenuSelect(const Menu *menu)
{
    settings.units = menu->state->selectedIndex;
}

static MenuState unitsMenuState;

static const Menu unitsMenu = {
    "Units",
    &unitsMenuState,
    onUnitsMenuGetOption,
    onUnitsMenuSelect,
    onSettingsSubMenuBack,
};

const View unitsMenuView = {
    onMenuEvent,
    &unitsMenu,
};

// Average timer menu

static const char *const averageTimerMenuOptions[] = {
    "Off",
    "5 minutes",
    "10 minutes",
    "30 minutes",
    "60 minutes",
    NULL,
};

static const char *onAverageTimerMenuGetOption(const Menu *menu,
                                               uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.averageTimer);

    return averageTimerMenuOptions[index];
}

static void onAverageTimerMenuSelect(const Menu *menu)
{
    settings.averageTimer = menu->state->selectedIndex;
}

static MenuState averageTimerMenuState;

static const Menu averageTimerMenu = {
    "Average timer",
    &averageTimerMenuState,
    onAverageTimerMenuGetOption,
    onAverageTimerMenuSelect,
    onSettingsSubMenuBack,
};

const View averageTimerMenuView = {
    onMenuEvent,
    &averageTimerMenu,
};

// Rate alarm menu

static const char *onRateAlarmMenuGetOption(const Menu *menu,
                                            uint32_t index,
                                            MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rateAlarm);

    if (index == 0)
        return "Off";
    else if (index < RATEALARM_NUM)
    {
        Unit *rateUnit = &units[settings.units].rate;
        float value = rateAlarmsSvH[index] /
                      units[UNITS_SIEVERTS].rate.scale;

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

static void onRateAlarmMenuSelect(const Menu *menu)
{
    settings.rateAlarm = menu->state->selectedIndex;
}

static MenuState rateAlarmMenuState;

static const Menu rateAlarmMenu = {
    "Rate alarm",
    &rateAlarmMenuState,
    onRateAlarmMenuGetOption,
    onRateAlarmMenuSelect,
    onSettingsSubMenuBack,
};

const View rateAlarmMenuView = {
    onMenuEvent,
    &rateAlarmMenu,
};

// Dose alarm menu

static const char *onDoseAlarmMenuGetOption(const Menu *menu,
                                            uint32_t index,
                                            MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.doseAlarm);

    if (index == 0)
        return "Off";
    else if (index < DOSEALARM_NUM)
    {
        Unit *doseUnit = &units[settings.units].dose;
        float value = doseAlarmsSv[index] /
                      units[UNITS_SIEVERTS].dose.scale;

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

static void onDoseAlarmMenuSelect(const Menu *menu)
{
    settings.doseAlarm = menu->state->selectedIndex;
}

static MenuState doseAlarmMenuState;

static const Menu doseAlarmMenu = {
    "Dose alarm",
    &doseAlarmMenuState,
    onDoseAlarmMenuGetOption,
    onDoseAlarmMenuSelect,
    onSettingsSubMenuBack,
};

const View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};
