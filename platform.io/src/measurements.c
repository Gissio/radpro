/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2025 Gissio
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

#define ALERTZONE1_USVH 1.0E-6F
#define ALERTZONE2_USVH 1.0E-5F

#define TUBE_FAULT_TIMEOUT 300

#define REMAINING_PULSE_COUNT_LOWER_LIMIT 10

#define INSTANTANEOUS_RATE_QUEUE_SIZE 64
#define INSTANTANEOUS_RATE_QUEUE_MASK (INSTANTANEOUS_RATE_QUEUE_SIZE - 1)
#define INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT 19 // For 50% confidence interval
#define INSTANTANEOUS_RATE_MAX_PULSE_COUNT 10

#define PULSE_INDICATION_CONVERSION_FACTOR_MAX 600.0F
#define PULSE_INDICATION_FACTOR_UNIT 0x10000
#define PULSE_INDICATION_FACTOR_MASK 0xffff

enum
{
    INSTANTANEOUS_TAB_BAR,
    INSTANTANEOUS_TAB_PERIOD,
    INSTANTANEOUS_TAB_MAX,
    INSTANTANEOUS_TAB_RATE,

    INSTANTANEOUS_TAB_NUM,
};

enum
{
    AVERAGE_TAB_TIME,
    AVERAGE_TAB_RATE,
    AVERAGE_TAB_DOSE,

    AVERAGE_TAB_NUM,
};

enum
{
    CUMULATIVE_TAB_TIME,
    CUMULATIVE_TAB_DOSE,

    CUMULATIVE_TAB_NUM,
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

    uint16_t sampleIndex;

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
    200E-6F,
    500E-6F,
    1E-3F,
    2E-3F,
    5E-3F,
    10E-3F,
};

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
    1E-3F,
    2E-3F,
    5E-3F,
    10E-3F,
    20E-3F,
    50E-3F,
};

static struct
{
    struct
    {
        Dose dose;
    } tube;

    struct
    {
        Measurement measurement;

        uint32_t snapshotTick;
        Measurement snapshotMeasurement;

        float deadTimeCompensationRemainder;

        uint32_t faultTimer;
        bool faultAlarm;
        bool lastShortCircuit;
    } period;

    struct
    {
        uint32_t queueHead;
        uint32_t queueTail;
        Measurement queue[INSTANTANEOUS_RATE_QUEUE_SIZE];

        uint32_t pulseIndicationFactor;
        uint32_t pulseIndicationRemainder;

        Rate rate;
        float maxValue;
        bool lastAlarm;
        bool alarm;
    } instantaneous;

    uint32_t instantaneousTabIndex;

    struct
    {
        uint32_t firstPulseTick;
        uint32_t lastPulseTick;
        uint32_t pulseCount;

        Rate rate;
        bool timerExpired;
        uint32_t timedPulseCount;
        Rate timedRate;
    } average;

    uint32_t averageTabIndex;

    struct
    {
        Dose dose;
        bool lastAlarm;
        bool alarm;
    } cumulativeDose;

    uint32_t cumulativeDoseTabIndex;

    struct
    {
        HistoryState states[HISTORY_NUM];

        uint32_t tabIndex;
    } history;

    bool enabled;
    int32_t viewIndex;
} measurements;

uint8_t instantaneousAveragingPeriods[] = {
    0, 5, 60, 30, 10};

static const int32_t averagingTimes[] = {
    30 * 24 * 60 * 60, // Off (actually 30 days)
    5 * 60,            // 5 minutes
    10 * 60,           // 10 minutes
    30 * 60,           // 30 minutes
    60 * 60,           // 60 minutes
};

static const float averagingConfidences[] = {
    0.405F,  // ±40% confidence
    0.205F,  // ±20% confidence
    0.105F,  // ±10% confidence
    0.0505F, // ±5% confidence
};

static const View rateAlarmMenuView;
static const View doseAlarmMenuView;
static const View alarmSignalingMenuView;
static const View unitsMenuView;
static const View instantaneousMenuView;
static const View averageMenuView;

static const Menu rateAlarmMenu;
static const Menu doseAlarmMenu;
static const Menu alarmSignalingMenu;
static const Menu unitsMenu;
static const Menu instantaneousMenu;
static const Menu averageMenu;

static void resetInstantaneousRate(void);
static void resetAverageRate(void);
static void resetCumulativeDose(void);
static void resetHistory(void);

// Measurements

const View *const measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &cumulativeDoseView,
    &historyView,
};

#define MEASUREMENT_VIEWS_NUM (sizeof(measurementViews) / sizeof(View *))

void initMeasurements(void)
{
    selectMenuItem(&rateAlarmMenu,
                   settings.rateAlarm,
                   RATEALARM_NUM);
    selectMenuItem(&doseAlarmMenu,
                   settings.doseAlarm,
                   DOSEALARM_NUM);
    selectMenuItem(&alarmSignalingMenu,
                   0,
                   ALARMSIGNALING_NUM);

    selectMenuItem(&unitsMenu,
                   settings.units,
                   UNITS_NUM);
    selectMenuItem(&instantaneousMenu,
                   settings.instantaneousAveraging,
                   INSTANTANEOUSAVERAGING_NUM);
    selectMenuItem(&averageMenu,
                   settings.averaging,
                   AVERAGING_NUM);

    Dose tubeDose = measurements.tube.dose;
    Dose cumulativeDose = measurements.cumulativeDose.dose;
    memset(&measurements, 0, sizeof(measurements));
    measurements.tube.dose = tubeDose;
    measurements.cumulativeDose.dose = cumulativeDose;
    updateMeasurementUnits();
}

// Measurement events

const char countsString[] = "counts";

Units units[] = {
    {{"Sv/h", (60 * 1E-6F)},
     {"Sv", (60 * 1E-6F / 3600)}},
    {{"rem/h", (60 * 1E-4F)},
     {"rem", (60 * 1E-4F / 3600)}},
    {{"cpm", 60},
     {countsString, 1}},
    {{"cps", 1},
     {countsString, 1}},
};

const int8_t unitsMinMetricPrefixIndex[] = {
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

    if (conversionFactor < PULSE_INDICATION_CONVERSION_FACTOR_MAX)
        measurements.instantaneous.pulseIndicationFactor = PULSE_INDICATION_FACTOR_UNIT;
    else
        measurements.instantaneous.pulseIndicationFactor =
            (PULSE_INDICATION_FACTOR_UNIT * PULSE_INDICATION_CONVERSION_FACTOR_MAX) /
            conversionFactor;
}

void updateDeadTimeCompensation(void)
{
    if (measurements.period.deadTimeCompensationRemainder < 0)
        measurements.period.deadTimeCompensationRemainder = 0;
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

static void calculateRate(uint32_t pulseCount, uint32_t ticks, Rate *rate)
{
    if (ticks < 2)
    {
        rate->value = 0;
        rate->confidence = 0;

        return;
    }

    // Improves precision when radiation rate is high
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

void enableMeasurements(void)
{
    measurements.enabled = true;
}

void disableMeasurements(void)
{
    measurements.enabled = false;
}

void onMeasurementTick(uint32_t pulseCount)
{
    if (measurements.enabled && pulseCount)
    {
        measurements.tube.dose.pulseCount += pulseCount;

        if (!measurements.period.measurement.pulseCount)
            measurements.period.measurement.firstPulseTick = getTick();
        measurements.period.measurement.lastPulseTick = getTick();
        measurements.period.measurement.pulseCount += pulseCount;

        // Pulse indication divider
        measurements.instantaneous.pulseIndicationRemainder +=
            pulseCount * measurements.instantaneous.pulseIndicationFactor;

        if (measurements.instantaneous.pulseIndicationRemainder >=
            PULSE_INDICATION_FACTOR_UNIT)
        {
            measurements.instantaneous.pulseIndicationRemainder &=
                PULSE_INDICATION_FACTOR_MASK;

            triggerPulse();
        }
    }
}

void onMeasurementPeriod(void)
{
    if (measurements.enabled)
    {
        measurements.period.snapshotTick = getTick();
        measurements.period.snapshotMeasurement = measurements.period.measurement;
        measurements.period.measurement.pulseCount = 0;
    }
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
    if (!measurements.enabled)
        return;

    // Tube life
    measurements.tube.dose.time++;

    // Fault detection
    if (measurements.period.snapshotMeasurement.pulseCount)
        measurements.period.faultTimer = 0;
    else
        measurements.period.faultTimer++;

    bool isTubeShortCircuit =
        (!measurements.period.snapshotMeasurement.pulseCount && getTubeDet());
    if (measurements.period.lastShortCircuit && isTubeShortCircuit)
        measurements.period.faultTimer = TUBE_FAULT_TIMEOUT;
    measurements.period.lastShortCircuit = isTubeShortCircuit;

    measurements.period.faultAlarm = (measurements.period.faultTimer >= TUBE_FAULT_TIMEOUT);

    // Compensations
    Measurement compensatedMeasurement;

    compensatedMeasurement.firstPulseTick =
        measurements.period.snapshotMeasurement.firstPulseTick;
    compensatedMeasurement.lastPulseTick =
        measurements.period.snapshotMeasurement.lastPulseTick;

    measurements.period.deadTimeCompensationRemainder +=
        measurements.period.snapshotMeasurement.pulseCount *
        getDeadTimeCompensationFactor(measurements.instantaneous.rate.value);
    float tubeBackgroundCompensation = getTubeBackgroundCompensation();
    measurements.period.deadTimeCompensationRemainder -= tubeBackgroundCompensation;

    if (measurements.period.deadTimeCompensationRemainder >= 1)
    {
        compensatedMeasurement.pulseCount =
            (uint32_t)measurements.period.deadTimeCompensationRemainder;
        measurements.period.deadTimeCompensationRemainder -= compensatedMeasurement.pulseCount;
    }
    else
    {
        compensatedMeasurement.pulseCount = 0;

        if (measurements.period.deadTimeCompensationRemainder <
            -REMAINING_PULSE_COUNT_LOWER_LIMIT)
            measurements.period.deadTimeCompensationRemainder =
                -REMAINING_PULSE_COUNT_LOWER_LIMIT;
    }

    // Instantaneous rate
    if (compensatedMeasurement.pulseCount)
    {
        // Enqueue
        measurements.instantaneous.queue[measurements.instantaneous.queueTail] =
            compensatedMeasurement;
        measurements.instantaneous.queueTail =
            (measurements.instantaneous.queueTail + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;

        if (measurements.instantaneous.queueTail ==
            measurements.instantaneous.queueHead)
        {
            measurements.instantaneous.queueHead =
                (measurements.instantaneous.queueTail + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;
        }
    }

    Measurement instantaneousMeasurement;
    instantaneousMeasurement.firstPulseTick = 0;
    instantaneousMeasurement.lastPulseTick = 0;
    instantaneousMeasurement.pulseCount = 0;

    measurements.instantaneous.rate.time = 0;

    uint32_t averagingPeriod =
        instantaneousAveragingPeriods[settings.instantaneousAveraging];
    uint32_t averagingPulseCount =
        (settings.instantaneousAveraging <=
         INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION)
            ? INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT
            : 0;

    uint32_t queueSize = (measurements.instantaneous.queueTail -
                          measurements.instantaneous.queueHead) &
                         INSTANTANEOUS_RATE_QUEUE_MASK;
    for (uint32_t i = 0; i < queueSize; i++)
    {
        uint32_t queueIndex =
            (measurements.instantaneous.queueTail - 1 - i) & INSTANTANEOUS_RATE_QUEUE_MASK;

        uint32_t timePeriod =
            1 +
            (measurements.period.snapshotTick -
             measurements.instantaneous.queue[queueIndex].firstPulseTick) /
                SYSTICK_FREQUENCY;

        if (timePeriod > averagingPeriod)
        {
            if (instantaneousMeasurement.pulseCount >
                averagingPulseCount)
            {
                measurements.instantaneous.queueHead =
                    (queueIndex + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;

                break;
            }
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

    if ((instantaneousMeasurement.pulseCount >
         INSTANTANEOUS_RATE_MAX_PULSE_COUNT) &&
        (measurements.instantaneous.rate.value >
         measurements.instantaneous.maxValue))
        measurements.instantaneous.maxValue =
            measurements.instantaneous.rate.value;

    float svH = units[UNITS_SIEVERTS].rate.scale *
                measurements.instantaneous.rate.value;

    setPulseThreshold(svH < rateAlarmsSvH[settings.pulseThreshold]);

    bool instantaneousRateAlarm = false;
    // Trigger alarm if alarm enabled and confidence interval is below 75%
    if (settings.rateAlarm && (measurements.instantaneous.rate.confidence < 0.75))
        instantaneousRateAlarm = (svH >= rateAlarmsSvH[settings.rateAlarm]);

    if (measurements.instantaneous.lastAlarm != instantaneousRateAlarm)
    {
        measurements.instantaneous.lastAlarm = instantaneousRateAlarm;
        measurements.instantaneous.alarm = instantaneousRateAlarm;
    }

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

        // Average timer
        bool timerExpired =
            (settings.averaging < AVERAGING_TIME_NUM)
                ? (measurements.average.rate.time >=
                   averagingTimes[settings.averaging])
                : (measurements.average.rate.confidence != 0.0F) &&
                      (measurements.average.rate.confidence <
                       averagingConfidences[settings.averaging - AVERAGING_TIME_NUM]);

        if (!timerExpired)
        {
            measurements.average.timedPulseCount = measurements.average.pulseCount;
            measurements.average.timedRate = measurements.average.rate;
        }

        if (timerExpired && !measurements.average.timerExpired)
        {
            measurements.average.timerExpired = true;

            measurements.average.timedPulseCount = measurements.average.pulseCount;
            measurements.average.timedRate = measurements.average.rate;

            triggerAlarm();
        }
        else if (!timerExpired && measurements.average.timerExpired)
            measurements.average.timerExpired = false;
    }

    // Cumulative dose
    if ((measurements.cumulativeDose.dose.time < UINT32_MAX) &&
        (measurements.cumulativeDose.dose.pulseCount < UINT32_MAX))
    {
        addClamped(&measurements.cumulativeDose.dose.time, 1);
        addClamped(&measurements.cumulativeDose.dose.pulseCount,
                   compensatedMeasurement.pulseCount);
    }

    float cumulativeDoseAlarm = false;
    if (settings.doseAlarm)
    {
        float doseSv = units[UNITS_SIEVERTS].dose.scale *
                       measurements.cumulativeDose.dose.pulseCount;
        cumulativeDoseAlarm = (doseSv >= doseAlarmsSv[settings.doseAlarm]);
    }

    if (measurements.cumulativeDose.lastAlarm != cumulativeDoseAlarm)
    {
        measurements.cumulativeDose.lastAlarm = cumulativeDoseAlarm;
        measurements.cumulativeDose.alarm = cumulativeDoseAlarm;
    }

    // History
    for (uint32_t i = 0; i < HISTORY_NUM; i++)
    {
        const History *history = &histories[i];
        HistoryState *historyState = &measurements.history.states[i];

        historyState->sampleIndex++;
        if (historyState->sampleIndex >=
            history->samplesPerDataPoint)
            historyState->sampleIndex = 0;

        historyState->averageSum += measurements.instantaneous.rate.value;
        historyState->averageCount++;

        if (historyState->sampleIndex == 0)
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
    if (isAlarm())
        triggerAlarm();
}

// Measurement views common

static void buildValueString(char *valueString,
                             char *unitString,
                             float value,
                             const Unit *unit,
                             int32_t minMetricPrefixIndex)
{
    if ((unit->name == countsString) &&
        (value < 10000))
    {
        uint32_t intValue = (uint32_t)value;

        strcatUInt32(valueString, intValue, 0);
        strcpy(unitString, " ");
        if (intValue == 1)
            strcat(unitString, "count");
        else
            strcat(unitString, countsString);

        return;
    }

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
        if (!isLockMode())
        {
            setKeyboardMode(KEYBOARD_MODE_MENU);

            setView(&settingsMenuView);
        }

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

static void resetInstantaneousRate(void)
{
    memset(&measurements.instantaneous,
           0,
           sizeof(measurements.instantaneous));

    updateMeasurementUnits();
}

float getInstantaneousRate(void)
{
    return measurements.instantaneous.rate.value;
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
        if (!isLockMode())
        {
            if (measurements.instantaneous.alarm)
                measurements.instantaneous.alarm = false;
            else
                resetInstantaneousRate();

            updateView();
        }

        break;

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        char *stateString;
        char *keyString = NULL;
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);

        buildValueString(valueString,
                         unitString,
                         measurements.instantaneous.rate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.period.faultAlarm)
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.instantaneous.alarm)
        {
            stateString = "ALARM";

            style = MEASUREMENTSTYLE_ALARM;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Instantaneous", false);
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.instantaneous.rate.confidence,
                             style);

        strclr(valueString);
        strcpy(unitString, " ");

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

        case INSTANTANEOUS_TAB_PERIOD:
            if (measurements.instantaneous.rate.time > 0)
                strcatTime(valueString,
                           measurements.instantaneous.rate.time);

            keyString = "Time";

            break;

        case INSTANTANEOUS_TAB_MAX:
        {
            if (measurements.instantaneous.maxValue > 0)
            {
                buildValueString(valueString,
                                 unitString,
                                 measurements.instantaneous.maxValue,
                                 &units[settings.units].rate,
                                 unitsMinMetricPrefixIndex[settings.units]);
            }

            keyString = "Max";

            break;
        }

        case INSTANTANEOUS_TAB_RATE:
            if (measurements.instantaneous.rate.value > 0)
            {
                uint32_t unitsIndex = ((settings.units != UNITS_CPM) &&
                                       (settings.units != UNITS_CPS))
                                          ? UNITS_CPM
                                          : UNITS_SIEVERTS;

                buildValueString(valueString,
                                 unitString,
                                 measurements.instantaneous.rate.value,
                                 &units[unitsIndex].rate,
                                 unitsMinMetricPrefixIndex[unitsIndex]);
            }

            keyString = "Rate";

            break;
        }

        if (measurements.instantaneousTabIndex !=
            INSTANTANEOUS_TAB_BAR)
            drawMeasurementInfo(keyString,
                                valueString,
                                unitString,
                                stateString,
                                style);
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
    case EVENT_KEY_BACK:
        measurements.averageTabIndex++;

        if (measurements.averageTabIndex >= AVERAGE_TAB_NUM)
            measurements.averageTabIndex = 0;

        updateView();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            resetAverageRate();

            updateView();
        }

        break;

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        char *stateString;
        char *keyString = NULL;
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);

        buildValueString(valueString,
                         unitString,
                         measurements.average.timedRate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.period.faultAlarm)
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.average.pulseCount == UINT32_MAX)
        {
            stateString = "OVER";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (measurements.average.timerExpired)
        {
            stateString = "DONE";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Average", false);
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.average.timedRate.confidence,
                             style);

        strclr(valueString);
        strcpy(unitString, " ");

        switch (measurements.averageTabIndex)
        {
        case AVERAGE_TAB_TIME:
            strcatTime(valueString,
                       measurements.average.timedRate.time);

            keyString = "Time";

            break;

        case AVERAGE_TAB_RATE:
            if (measurements.average.rate.value > 0)
            {
                uint32_t unitsIndex = ((settings.units != UNITS_CPM) &&
                                       (settings.units != UNITS_CPS))
                                          ? UNITS_CPM
                                          : UNITS_SIEVERTS;

                buildValueString(valueString,
                                 unitString,
                                 measurements.average.timedRate.value,
                                 &units[unitsIndex].rate,
                                 unitsMinMetricPrefixIndex[unitsIndex]);
            }

            keyString = "Rate";

            break;

        case AVERAGE_TAB_DOSE:
            if (measurements.average.pulseCount > 0)
                buildValueString(valueString,
                                 unitString,
                                 measurements.average.timedPulseCount,
                                 &units[UNITS_CPM].dose,
                                 unitsMinMetricPrefixIndex[UNITS_CPM]);

            keyString = "Dose";

            break;
        }

        drawMeasurementInfo(keyString,
                            valueString,
                            unitString,
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

void setCumulativeDoseTime(uint32_t value)
{
    measurements.cumulativeDose.dose.time = value;
}

uint32_t getCumulativeDoseTime(void)
{
    return measurements.cumulativeDose.dose.time;
}

void setCumulativeDosePulseCount(uint32_t value)
{
    measurements.cumulativeDose.dose.pulseCount = value;
}

uint32_t getCumulativeDosePulseCount(void)
{
    return measurements.cumulativeDose.dose.pulseCount;
}

static void resetCumulativeDose(void)
{
    memset(&measurements.cumulativeDose,
           0,
           sizeof(measurements.cumulativeDose));
}

static void onCumulativeDoseViewEvent(const View *view,
                                      Event event)
{
    onMeasurementEvent(view, event);

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.cumulativeDoseTabIndex++;

        if (measurements.cumulativeDoseTabIndex >= CUMULATIVE_TAB_NUM)
            measurements.cumulativeDoseTabIndex = 0;

        updateView();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            if (measurements.cumulativeDose.alarm)
                measurements.cumulativeDose.alarm = false;
            else
                resetCumulativeDose();

            updateView();
        }

        break;

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        char *stateString;
        char *keyString = NULL;
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);

        buildValueString(valueString,
                         unitString,
                         measurements.cumulativeDose.dose.pulseCount,
                         &units[settings.units].dose,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.period.faultAlarm)
        {
            stateString = "FAULT";

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.cumulativeDose.alarm)
        {
            stateString = "ALARM";

            style = MEASUREMENTSTYLE_ALARM;
        }
        else if (measurements.cumulativeDose.dose.pulseCount == UINT32_MAX)
        {
            stateString = "OVER";

            style = MEASUREMENTSTYLE_HOLD;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar("Cumulative", false);
        drawMeasurementValue(valueString,
                             unitString,
                             0,
                             style);

        strclr(valueString);
        strcpy(unitString, " ");

        switch (measurements.cumulativeDoseTabIndex)
        {
        case CUMULATIVE_TAB_TIME:
            strcatTime(valueString,
                       measurements.cumulativeDose.dose.time);

            keyString = "Time";

            break;

        case CUMULATIVE_TAB_DOSE:
            if (measurements.cumulativeDose.dose.pulseCount > 0)
            {
                uint32_t unitsIndex = ((settings.units != UNITS_CPM) &&
                                       (settings.units != UNITS_CPS))
                                          ? UNITS_CPM
                                          : UNITS_SIEVERTS;

                buildValueString(valueString,
                                 unitString,
                                 measurements.cumulativeDose.dose.pulseCount,
                                 &units[unitsIndex].dose,
                                 unitsMinMetricPrefixIndex[unitsIndex]);
            }

            keyString = "Dose";

            break;
        }

        drawMeasurementInfo(keyString,
                            valueString,
                            unitString,
                            stateString,
                            style);

        break;
    }

    default:
        break;
    }
}

const View cumulativeDoseView = {
    onCumulativeDoseViewEvent,
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
        if (!isLockMode())
        {
            resetHistory();

            updateView();
        }

        break;

    case EVENT_DRAW:
    {
        HistoryState *historyState =
            &measurements.history.states[measurements.history.tabIndex];
        const Unit *rateUnit = &units[settings.units].rate;

        uint8_t data[HISTORY_BUFFER_SIZE];
        for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
        {
            uint32_t dataIndex = historyState->bufferIndex + i;
            if (dataIndex >= HISTORY_BUFFER_SIZE)
                dataIndex -= HISTORY_BUFFER_SIZE;

            data[i] = historyState->buffer[dataIndex];
        }

        drawTitleBar(histories[measurements.history.tabIndex].name, false);
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

// Alarms menu

bool isAlarmEnabled(void)
{
    return (settings.rateAlarm || settings.doseAlarm);
}

bool isAlarm(void)
{
    return (measurements.instantaneous.alarm ||
            measurements.cumulativeDose.alarm ||
            measurements.period.faultAlarm);
}

static const OptionView alarmsMenuOptions[] = {
    {"Rate alarm", &rateAlarmMenuView},
    {"Dose alarm", &doseAlarmMenuView},
    {"Signaling", &alarmSignalingMenuView},
    {NULL},
};

static const char *onAlarmsMenuGetOption(const Menu *menu,
                                         uint32_t index,
                                         MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return alarmsMenuOptions[index].option;
}

static void onAlarmsMenuSelect(const Menu *menu)
{
    setView(alarmsMenuOptions[menu->state->selectedIndex].view);
}

static MenuState displayMenuState;

static const Menu displayMenu = {
    "Alarms",
    &displayMenuState,
    onAlarmsMenuGetOption,
    onAlarmsMenuSelect,
    onSettingsSubMenuBack,
};

const View alarmsMenuView = {
    onMenuEvent,
    &displayMenu,
};

static void onAlarmsSubMenuBack(const Menu *menu)
{
    setView(&alarmsMenuView);
}

// Rate alarm menu

static char *buildRateAlarmMenuOption(uint32_t index)
{
    const Unit *rateUnit = &units[settings.units].rate;
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
    onAlarmsSubMenuBack,
};

static const View rateAlarmMenuView = {
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
        const Unit *doseUnit = &units[settings.units].dose;
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
    onAlarmsSubMenuBack,
};

static const View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};

// Alarm signaling menu

static const char *const alarmSignalingMenuOptions[] = {
    "Sound",
#if defined(VIBRATION)
    "Vibration",
#endif
#if defined(ALERT_LED)
    "Alert LED",
#elif defined(PULSE_LED)
    "Pulse LED",
#endif
    "Display flash",
    NULL,
};

static const char *onAlarmSignalingMenuGetOption(const Menu *menu,
                                                 uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (settings.alarmSignaling >> index) & 1;

    return alarmSignalingMenuOptions[index];
}

static void onAlarmSignalingMenuSelect(const Menu *menu)
{
    settings.alarmSignaling ^= (1 << menu->state->selectedIndex);
}

static MenuState alarmSignalingMenuState;

static const Menu alarmSignalingMenu = {
    "Signaling",
    &alarmSignalingMenuState,
    onAlarmSignalingMenuGetOption,
    onAlarmSignalingMenuSelect,
    onAlarmsSubMenuBack,
};

static const View alarmSignalingMenuView = {
    onMenuEvent,
    &alarmSignalingMenu,
};

// Measurements menu

static const OptionView measurementMenuOptions[] = {
    {"Units", &unitsMenuView},
    {"Instantaneous", &instantaneousMenuView},
    {"Average", &averageMenuView},
    {NULL},
};

static const char *onMeasurementsMenuGetOption(const Menu *menu,
                                               uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return measurementMenuOptions[index].option;
}

static void onMeasurementsMenuSelect(const Menu *menu)
{
    setView(measurementMenuOptions[menu->state->selectedIndex].view);
}

static MenuState measurementsMenuState;

static const Menu measurementMenu = {
    "Measurements",
    &measurementsMenuState,
    onMeasurementsMenuGetOption,
    onMeasurementsMenuSelect,
    onSettingsSubMenuBack,
};

const View measurementsMenuView = {
    onMenuEvent,
    &measurementMenu,
};

static void onMeasurementsSubMenuBack(const Menu *menu)
{
    setView(&measurementsMenuView);
}

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
    onMeasurementsSubMenuBack,
};

static const View unitsMenuView = {
    onMenuEvent,
    &unitsMenu,
};

// Instantaneous menu

static const char *const instantaneousMenuOptions[] = {
    "Adaptive fast",
    "Adaptive precision",
    "60 seconds",
    "30 seconds",
    "10 seconds",
    NULL,
};

static const char *onInstantaneousMenuGetOption(const Menu *menu,
                                                uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.instantaneousAveraging);

    return instantaneousMenuOptions[index];
}

static void onInstantaneousMenuSelect(const Menu *menu)
{
    settings.instantaneousAveraging = menu->state->selectedIndex;
}

static MenuState instantaneousMenuState;

static const Menu instantaneousMenu = {
    "Instantaneous",
    &instantaneousMenuState,
    onInstantaneousMenuGetOption,
    onInstantaneousMenuSelect,
    onMeasurementsSubMenuBack,
};

static const View instantaneousMenuView = {
    onMenuEvent,
    &instantaneousMenu,
};

// Average menu

static const char *const averageMenuOptions[] = {
    "Unlimited",
    "5 minutes",
    "10 minutes",
    "30 minutes",
    "60 minutes",
    "\xb1"
    "40% confidence",
    "\xb1"
    "20% confidence",
    "\xb1"
    "10% confidence",
    "\xb1"
    "5% confidence",
    NULL,
};

static const char *onAverageMenuGetOption(const Menu *menu,
                                          uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.averaging);

    return averageMenuOptions[index];
}

static void onAverageMenuSelect(const Menu *menu)
{
    settings.averaging = menu->state->selectedIndex;
}

static MenuState averageMenuState;

static const Menu averageMenu = {
    "Average",
    &averageMenuState,
    onAverageMenuGetOption,
    onAverageMenuSelect,
    onMeasurementsSubMenuBack,
};

static const View averageMenuView = {
    onMenuEvent,
    &averageMenu,
};

// Pulse threshold menu

static const char *onPulseThresholdMenuGetOption(const Menu *menu,
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

static void onPulseThresholdMenuSelect(const Menu *menu)
{
    settings.pulseThreshold = menu->state->selectedIndex;
}

static MenuState pulseThresholdMenuState;

static const Menu pulsesThresholdMenu = {
    "Threshold",
    &pulseThresholdMenuState,
    onPulseThresholdMenuGetOption,
    onPulseThresholdMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseThresholdMenuView = {
    onMenuEvent,
    &pulsesThresholdMenu,
};
