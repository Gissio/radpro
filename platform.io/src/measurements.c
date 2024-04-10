/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <limits.h>

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

#define TUBE_FAULT_TIMEOUT 300

#define MEASUREMENT_QUEUE_SIZE 32
#define MEASUREMENT_QUEUE_MASK (MEASUREMENT_QUEUE_SIZE - 1)

#define AVERAGING_PERIOD_TIME_CONSTANT 20.0F
#define AVERAGING_PERIOD_TIME_MIN 5.0F
#define AVERAGING_PERIOD_TIME_MAX 300.0F
#define AVERAGING_TRANSITION_SVH_MIN 0.3E-6F
#define AVERAGING_TRANSITION_SVH_MAX 1.0E-6F

#define ALERTZONE1_USVH 1.0E-6F
#define ALERTZONE2_USVH 1.0E-5F

enum
{
    INSTANTANEOUS_TAB_BAR,
    INSTANTANEOUS_TAB_TIME,
    INSTANTANEOUS_TAB_MAX,

    INSTANTANEOUS_TAB_NUM,
};

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
    uint8_t timeTickNum;
} History;

static const History histories[] = {
    {"History (10 min)", 10 * 60 / HISTORY_BUFFER_SIZE, 10},
    {"History (1 h)", 60 * 60 / HISTORY_BUFFER_SIZE, 6},
    {"History (24 h)", 24 * 60 * 60 / HISTORY_BUFFER_SIZE, 8},
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

        uint32_t faultPulseTime;
        uint32_t faultLastPulseTime;
    } tick;

    struct
    {
        uint32_t queueHead;
        uint32_t queueTail;
        Measurement queue[MEASUREMENT_QUEUE_SIZE];

        Rate rate;
        float maxValue;
    } instantaneous;

    uint32_t instantaneousTabIndex;

    struct
    {
        uint32_t firstPulseTick;
        uint32_t lastPulseTick;
        uint32_t pulseCount;

        Rate rate;
        bool timerExpired;
        Rate timerRate;
    } average;

    struct
    {
        Dose dose;

        uint32_t updateTime;
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

static const int32_t averageTimerTimes[] = {
    30 * 24 * 60 * 60, // Off (actually 30 days)
    5 * 60,            // 5 minutes
    10 * 60,           // 10 minutes
    30 * 60,           // 30 minutes
    60 * 60,           // 60 minutes
};

static const float averageTimerConfidences[] = {
    0.405F,  // ±40 % confidence
    0.205F,  // ±20 % confidence
    0.105F,  // ±10 % confidence
    0.0505F, // ±5 % confidence
};

static const Menu unitsMenu;
static const Menu rateAlarmMenu;
static const Menu doseAlarmMenu;
static const Menu averageTimerMenu;

static void updatePulseEnabled(void);

static bool isTubeFaultAlarm(void);
static bool isInstantaneousRateAlarm(void);
static bool isDoseAlarm(void);

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

    updateMeasurementUnits();
    updatePulseEnabled();
}

// Events

typedef struct
{
    char *const name;
    float scale;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} Units;

static Units units[] = {
    {{"Sv/h", (60 * 1E-6F)},
     {"Sv", (60 * 1E-6F / 3600)}},
    {{"rem/h", (60 * 1E-4F)},
     {"rem", (60 * 1E-4F / 3600)}},
    {{
         "cpm",
         60,
     },
     {"counts", 1}},
    {{"cps", 1},
     {"counts", 1}},
};

static const int8_t unitsMinMetricPrefixIndex[] = {
    -2, -2, 0, 0};

static const int8_t unitsMeasurementBarMinExponent[] = {
    -8, -6, 0, -2};

void updateMeasurementUnits(void)
{
    float conversionFactor = getTubeConversionFactor();

    units[UNITS_SIEVERTS].rate.scale = (60 * 1E-6F) / conversionFactor;
    units[UNITS_SIEVERTS].dose.scale = (60 * 1E-6F / 3600) / conversionFactor;
    units[UNITS_REM].rate.scale = (60 * 1E-4F) / conversionFactor;
    units[UNITS_REM].dose.scale = (60 * 1E-4F / 3600) / conversionFactor;
}

static float getDeadTimeCompensationFactor(float value)
{
    if (!settings.tubeDeadTimeCompensation)
        return 1.0F;
    else
    {
        float denominator = 1 - value * getTubeDeadTimeCompensation();

        if (denominator < 0.1F)
            denominator = 0.1F;

        return 1.0F / denominator;
    }
}

static uint32_t getInstantaneousAveragingPeriod(float value)
{
    float scale = units[UNITS_SIEVERTS].rate.scale;

    float svH = scale * value;
    float averagingPeriod;

    if ((svH > AVERAGING_TRANSITION_SVH_MIN) &&
        (svH < AVERAGING_TRANSITION_SVH_MAX))
    {
        float transitionExponent =
            log10f((AVERAGING_PERIOD_TIME_CONSTANT /
                    AVERAGING_TRANSITION_SVH_MIN /
                    AVERAGING_PERIOD_TIME_MIN) *
                   scale) /
            log10f(AVERAGING_TRANSITION_SVH_MIN /
                   AVERAGING_TRANSITION_SVH_MAX);

        averagingPeriod = (AVERAGING_PERIOD_TIME_MIN *
                           powf(svH / AVERAGING_TRANSITION_SVH_MAX,
                                transitionExponent));
    }
    else
    {
        if (value < (AVERAGING_PERIOD_TIME_CONSTANT / AVERAGING_PERIOD_TIME_MAX))
            averagingPeriod = AVERAGING_PERIOD_TIME_MAX;
        else if (value > AVERAGING_PERIOD_TIME_CONSTANT)
            averagingPeriod = 1;
        else
            averagingPeriod = AVERAGING_PERIOD_TIME_CONSTANT / value;

        if ((svH > AVERAGING_TRANSITION_SVH_MAX) &&
            (averagingPeriod > AVERAGING_PERIOD_TIME_MIN))
            averagingPeriod = AVERAGING_PERIOD_TIME_MIN;
    }

    return (uint32_t)(averagingPeriod + 0.999F);
}

static void calculateRate(uint32_t pulseCount, uint32_t ticks, Rate *rate)
{
    if (ticks < 2)
    {
        rate->value = 0;
        rate->confidence = 0;

        return;
    }

    // Improves accuracy when radiation rate is high
    if (pulseCount > ticks)
    {
        addClamped(&pulseCount, 1);
        addClamped(&ticks, 1);
    }

    // Value and confidence intervals
    float value = ((float)(pulseCount - 1) * SYSTICK_FREQUENCY) / ticks;

    float confidenceInterval =
        getConfidenceInterval(pulseCount - 1);

    // Dead-time compensation
    // float absConfidenceInterval = value * (1 + confidenceInterval);
    // float compValue = value * valueFactor;
    // float compAbsConfidenceInterval = absConfidenceInterval * confidenceIntervalFactor;
    // float compConfidenceInterval = (compAbsConfidenceInterval - compValue) / compValue;
    float valueFactor = getDeadTimeCompensationFactor(value);
    float confidenceIntervalFactor =
        getDeadTimeCompensationFactor(value * (1 + confidenceInterval));

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

uint8_t getHistoryValue(float value)
{
    int32_t historyValue = (int32_t)(HISTORY_DECADE *
                                     log10f(value / HISTORY_VALUE_MIN));
    if (historyValue < 0)
        historyValue = 0;
    else if (historyValue > UCHAR_MAX)
        historyValue = UCHAR_MAX;

    return historyValue;
}

void updateMeasurements(void)
{
    // Dead-time compensation
    Measurement compensatedMeasurement;

    compensatedMeasurement.firstPulseTick =
        measurements.tick.snapshotMeasurement.firstPulseTick;
    compensatedMeasurement.lastPulseTick =
        measurements.tick.snapshotMeasurement.lastPulseTick;

    measurements.tick.pulseCountFraction +=
        measurements.tick.snapshotMeasurement.pulseCount *
        getDeadTimeCompensationFactor(measurements.instantaneous.rate.value);
    compensatedMeasurement.pulseCount =
        (uint32_t)measurements.tick.pulseCountFraction;
    measurements.tick.pulseCountFraction -= compensatedMeasurement.pulseCount;

    // Pulse activity
    measurements.tick.faultPulseTime++;
    if (compensatedMeasurement.pulseCount)
        measurements.tick.faultLastPulseTime = measurements.tick.faultPulseTime;

    // Instantaneous rate
    if (compensatedMeasurement.pulseCount)
    {
        // Enqueue
        measurements.instantaneous.queue[measurements.instantaneous.queueTail] =
            compensatedMeasurement;
        measurements.instantaneous.queueTail =
            (measurements.instantaneous.queueTail + 1) & MEASUREMENT_QUEUE_MASK;

        if (measurements.instantaneous.queueTail ==
            measurements.instantaneous.queueHead)
        {
            measurements.instantaneous.queueHead =
                (measurements.instantaneous.queueTail + 1) & MEASUREMENT_QUEUE_MASK;
        }
    }

    Measurement instantaneousMeasurement;
    instantaneousMeasurement.firstPulseTick = 0;
    instantaneousMeasurement.lastPulseTick = 0;
    instantaneousMeasurement.pulseCount = 0;

    measurements.instantaneous.rate.time = 0;

    uint32_t averagingPeriod =
        getInstantaneousAveragingPeriod(measurements.instantaneous.rate.value);

    uint32_t queueSize = (measurements.instantaneous.queueTail -
                          measurements.instantaneous.queueHead) &
                         MEASUREMENT_QUEUE_MASK;
    for (uint32_t i = 0; i < queueSize; i++)
    {
        uint32_t queueIndex =
            (measurements.instantaneous.queueTail - 1 - i) & MEASUREMENT_QUEUE_MASK;

        uint32_t timePeriod =
            1 +
            (measurements.tick.snapshotTick -
             measurements.instantaneous.queue[queueIndex].firstPulseTick) /
                SYSTICK_FREQUENCY;

        if (timePeriod > averagingPeriod)
        {
            measurements.instantaneous.queueHead =
                (queueIndex + 1) & MEASUREMENT_QUEUE_MASK;

            break;
        }

        instantaneousMeasurement.firstPulseTick =
            measurements.instantaneous.queue[queueIndex].firstPulseTick;
        if (!instantaneousMeasurement.pulseCount)
            instantaneousMeasurement.lastPulseTick =
                measurements.instantaneous.queue[queueIndex].lastPulseTick;
        instantaneousMeasurement.pulseCount +=
            measurements.instantaneous.queue[queueIndex].pulseCount;
        measurements.instantaneous.rate.time = timePeriod;
    }

    calculateRate(instantaneousMeasurement.pulseCount,
                  instantaneousMeasurement.lastPulseTick -
                      instantaneousMeasurement.firstPulseTick,
                  &measurements.instantaneous.rate);

    measurements.instantaneous.rate.value -= getTubeBackgroundCompensation();
    if (measurements.instantaneous.rate.value < 0)
        measurements.instantaneous.rate.value = 0.0F;

    if ((instantaneousMeasurement.pulseCount > 10) &&
        (measurements.instantaneous.rate.value >
         measurements.instantaneous.maxValue))
        measurements.instantaneous.maxValue =
            measurements.instantaneous.rate.value;

    updatePulseEnabled();

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

            measurements.average.rate.value -= getTubeBackgroundCompensation();
            if (measurements.average.rate.value < 0)
                measurements.average.rate.value = 0.0F;
        }

        if ((measurements.average.rate.time < UINT32_MAX) &&
            (measurements.average.pulseCount < UINT32_MAX))
            measurements.average.rate.time++;

        // Average timer
        bool timerExpired =
            (settings.averageTimer < AVERAGETIMER_TIME_NUM)
                ? (measurements.average.rate.time >=
                   averageTimerTimes[settings.averageTimer])
                : (measurements.average.rate.confidence != 0.0F) &&
                      (measurements.average.rate.confidence <
                       averageTimerConfidences[settings.averageTimer - AVERAGETIMER_TIME_NUM]);

        if (!timerExpired)
            measurements.average.timerRate = measurements.average.rate;

        if (timerExpired && !measurements.average.timerExpired)
        {
            measurements.average.timerExpired = true;
            measurements.average.timerRate = measurements.average.rate;

            triggerAlarm();
        }
        else if (!timerExpired && measurements.average.timerExpired)
            measurements.average.timerExpired = false;
    }

    // Cumulative dose
    if ((measurements.cumulative.dose.time < UINT32_MAX) &&
        (measurements.cumulative.dose.pulseCount < UINT32_MAX))
    {
        addClamped(&measurements.cumulative.dose.time, 1);

        if (compensatedMeasurement.pulseCount)
        {
            addClamped(&measurements.cumulative.dose.pulseCount,
                       compensatedMeasurement.pulseCount);

            measurements.cumulative.updateTime =
                measurements.cumulative.dose.time;
        }
    }

    // Tube life
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
            historyState->buffer[historyState->bufferIndex] =
                getHistoryValue(averageRate);

            historyState->averageSum = 0;
            historyState->averageCount = 0;

            historyState->bufferIndex++;
            if (historyState->bufferIndex >= HISTORY_BUFFER_SIZE)
                historyState->bufferIndex = 0;
        }
    }

    // Alarms
    if (isInstantaneousRateAlarm() ||
        isDoseAlarm() ||
        isTubeFaultAlarm())
        triggerAlarm();
}

// View

static void buildValueString(char *valueString,
                             char *unitString,
                             float value,
                             const Unit *unit,
                             int32_t minMetricPrefixIndex)
{
    strcatFloatAsMetricValueAndPrefix(valueString,
                                      unitString,
                                      value * unit->scale,
                                      minMetricPrefixIndex);
    strcat(unitString, unit->name);
}

void setMeasurementView(int32_t index)
{
    if (index >= 0)
        measurements.viewIndex = index;

    setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);

    setView(measurementViews[measurements.viewIndex]);
}

static void onMeasurementEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_SELECT:
        setKeyboardMode(KEYBOARD_MODE_MENU);

        setView(&settingsMenuView);

        break;

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

    default:
        break;
    }
}

// Instantaneous

static const float rateAlarmsSvH[] = {
    0,
    0.5E-6F,
    1E-6F,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
    50E-6F,
    100E-6F,
};

static void updatePulseEnabled(void)
{
    float svH = units[UNITS_SIEVERTS].rate.scale *
                measurements.instantaneous.rate.value;

    setPulsesEnabled(svH >= rateAlarmsSvH[settings.pulseThreshold]);
}

static void resetInstantaneousRate(void)
{
    memset(&measurements.instantaneous,
           0,
           sizeof(measurements.instantaneous));
}

float getInstantaneousRate(void)
{
    return measurements.instantaneous.rate.value;
}

static bool isTubeFaultAlarm(void)
{
    return (measurements.tick.faultPulseTime - measurements.tick.faultLastPulseTime) >=
           TUBE_FAULT_TIMEOUT;
}

static bool isInstantaneousRateAlarm(void)
{
    if (!settings.rateAlarm)
        return false;

    float svH = units[UNITS_SIEVERTS].rate.scale *
                measurements.instantaneous.rate.value;
    return svH >= rateAlarmsSvH[settings.rateAlarm];
}

static void onInstantaneousRateViewEvent(const View *view,
                                         Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.instantaneousTabIndex++;

        if (measurements.instantaneousTabIndex >= INSTANTANEOUS_TAB_NUM)
            measurements.instantaneousTabIndex = 0;

        updateView();

        break;

    case EVENT_KEY_RESET:
        resetInstantaneousRate();

        updateView();

        break;

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[8];
        char *stateString;
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         measurements.instantaneous.rate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (isInstantaneousRateAlarm())
        {
            stateString = "ALARM";

            style = MEASUREMENTSTYLE_ALARM;
        }
        else if (isTubeFaultAlarm())
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Instantaneous");
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.instantaneous.rate.confidence,
                             style);

        switch (measurements.instantaneousTabIndex)
        {
        case INSTANTANEOUS_TAB_BAR:
        {
            float scale = units[settings.units].rate.scale;
            float alertZoneScale = scale / units[UNITS_SIEVERTS].rate.scale;

            drawMeasurementBar(measurements.instantaneous.rate.value * scale,
                               unitsMeasurementBarMinExponent[settings.units],
                               ALERTZONE1_USVH * alertZoneScale,
                               ALERTZONE2_USVH * alertZoneScale);

            break;
        }

        case INSTANTANEOUS_TAB_TIME:
            if (measurements.instantaneous.rate.time == 0)
                strclr(valueString);
            else
            {
                strclr(valueString);
                strcatTime(valueString,
                           measurements.instantaneous.rate.time);
            }

            drawMeasurementInfo("Time",
                                valueString,
                                "",
                                stateString,
                                style);

            break;

        case INSTANTANEOUS_TAB_MAX:
        {
            strclr(valueString);
            strclr(unitString);

            if (measurements.instantaneous.maxValue > 0)
            {
                strcpy(unitString, " ");
                buildValueString(valueString,
                                 unitString,
                                 measurements.instantaneous.maxValue,
                                 &units[settings.units].rate,
                                 unitsMinMetricPrefixIndex[settings.units]);
            }

            drawMeasurementInfo("Max",
                                valueString,
                                unitString,
                                stateString,
                                style);

            break;
        }
        }

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
    memset(&measurements.average,
           0,
           sizeof(measurements.average));
}

static void onAverageRateViewEvent(const View *view,
                                   Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_RESET:
        resetAverageRate();

        updateView();

        break;

    case EVENT_DRAW:
    {
        char timeString[16];
        char valueString[16];
        char unitString[8];
        char *stateString;
        MeasurementStyle style;

        strclr(timeString);
        strcatTime(timeString,
                   measurements.average.timerRate.time);

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         measurements.average.timerRate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.average.timerExpired)
        {
            stateString = "DONE";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (measurements.average.pulseCount == UINT32_MAX)
        {
            stateString = "OVER";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (isTubeFaultAlarm())
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Average");
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.average.timerRate.confidence,
                             style);
        drawMeasurementInfo("Time",
                            timeString,
                            "",
                            stateString,
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
};

void setDoseTime(uint32_t value)
{
    measurements.cumulative.dose.time = value;
    measurements.cumulative.updateTime = value;
}

uint32_t getDoseTime(void)
{
    return measurements.cumulative.dose.time;
}

void setDosePulseCount(uint32_t value)
{
    measurements.cumulative.dose.pulseCount = value;
    measurements.cumulative.updateTime =
        measurements.cumulative.dose.time;
}

uint32_t getDosePulseCount(void)
{
    return measurements.cumulative.dose.pulseCount;
}

static void resetDose(void)
{
    memset(&measurements.cumulative,
           0,
           sizeof(measurements.cumulative));
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
                            Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_RESET:
        resetDose();

        updateView();

        break;

    case EVENT_DRAW:
    {
        char timeString[16];
        char valueString[16];
        char unitString[8];
        char *stateString;
        MeasurementStyle style;

        strclr(timeString);
        strcatTime(timeString,
                   measurements.cumulative.dose.time);

        float doseValue = measurements.cumulative.dose.pulseCount;
        doseValue -= getTubeBackgroundCompensation() *
                     measurements.cumulative.updateTime;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         doseValue,
                         &units[settings.units].dose,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.cumulative.dose.pulseCount == UINT32_MAX)
        {
            stateString = "OVER";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (isDoseAlarm())
        {
            stateString = "ALARM";

            style = MEASUREMENTSTYLE_ALARM;
        }
        else if (isTubeFaultAlarm())
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Dose");
        drawMeasurementValue(valueString,
                             unitString,
                             0,
                             style);
        drawMeasurementInfo("Time",
                            timeString,
                            "",
                            stateString,
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
    memset(&measurements.history.states,
           0,
           sizeof(measurements.history.states));

    measurements.history.sampleIndex = 0;
}

static void onHistoryViewEvent(const View *view, Event event)
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
        Unit *rateUnit = &units[settings.units].rate;

        uint8_t data[HISTORY_BUFFER_SIZE];
        for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
        {
            uint32_t dataIndex = historyState->bufferIndex + i;
            if (dataIndex >= HISTORY_BUFFER_SIZE)
                dataIndex -= HISTORY_BUFFER_SIZE;

            data[i] = historyState->buffer[dataIndex];
        }

        drawTitleBar(histories[measurements.history.tabIndex].name);
        drawHistory(rateUnit->scale,
                    rateUnit->name,
                    histories[measurements.history.tabIndex].timeTickNum,
                    data,
                    getHistoryValue(ALERTZONE1_USVH / units[UNITS_SIEVERTS].rate.scale),
                    getHistoryValue(ALERTZONE2_USVH / units[UNITS_SIEVERTS].rate.scale));

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
    "\xb1"
    "40 % confidence",
    "\xb1"
    "20 % confidence",
    "\xb1"
    "10 % confidence",
    "\xb1"
    "5 % confidence",
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

static char *buildRateAlarmMenuOption(uint32_t index)
{
    Unit *rateUnit = &units[settings.units].rate;
    float value = rateAlarmsSvH[index] /
                  units[UNITS_SIEVERTS].rate.scale;

    strclr(menuOption);
    strcatFloatAsMetricValueWithPrefix(menuOption,
                                       rateUnit->scale * value,
                                       unitsMinMetricPrefixIndex[settings.units]);
    strcat(menuOption, rateUnit->name);

    return menuOption;
}

static const char *onRateAlarmMenuGetOption(const Menu *menu,
                                            uint32_t index,
                                            MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rateAlarm);

    if (index == 0)
        return "Off";
    else if (index < RATEALARM_NUM)
        return buildRateAlarmMenuOption(index);
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

        strclr(menuOption);
        strcatFloatAsMetricValueWithPrefix(menuOption,
                                           doseUnit->scale * value,
                                           unitsMinMetricPrefixIndex[settings.units]);
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

// Pulses threshold menu

static const char *onPulsesThresholdMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseThreshold);

    if (index == 0)
        return "Off";
    else if (index < RATEALARM_NUM)
        return buildRateAlarmMenuOption(index);
    else
        return NULL;
}

static void onPulsesThresholdMenuSelect(const Menu *menu)
{
    settings.pulseThreshold = menu->state->selectedIndex;
}

static MenuState pulsesThresholdMenuState;

static const Menu pulsesThresholdMenu = {
    "Pulses threshold",
    &pulsesThresholdMenuState,
    onPulsesThresholdMenuGetOption,
    onPulsesThresholdMenuSelect,
    onPulsesSubMenuBack,
};

const View pulsesThresholdMenuView = {
    onMenuEvent,
    &pulsesThresholdMenu,
};
