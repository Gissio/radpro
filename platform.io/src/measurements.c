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
#include "pulsecontrol.h"
#include "settings.h"
#include "tube.h"
#include "view.h"
#include "voice.h"

#define ALERTZONE1_USVH 1.0E-6F
#define ALERTZONE2_USVH 1.0E-5F

#define INSTANTANEOUS_RATE_QUEUE_SIZE 64
#define INSTANTANEOUS_RATE_QUEUE_MASK (INSTANTANEOUS_RATE_QUEUE_SIZE - 1)
#define INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT 19 // For 50% confidence interval

#define INSTANTANEOUS_RATE_ALARM_RATE 0
#define INSTANTANEOUS_RATE_ALARM_RATE_MASK (1 << INSTANTANEOUS_RATE_ALARM_RATE)

#define PULSE_INDICATION_SENSITIVITY_MAX 600.0F
#define PULSE_INDICATION_FACTOR_UNIT 0x10000
#define PULSE_INDICATION_FACTOR_MASK (PULSE_INDICATION_FACTOR_UNIT - 1)

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
    const char *name;
    uint16_t samplesPerDataPoint;
    uint8_t timeTickNum;
} History;

static const History histories[] = {
    {getString(STRING_HISTORY10_MIN), 10 * 60 / HISTORY_BUFFER_SIZE, 10},
    {getString(STRING_HISTORY1_H), 60 * 60 / HISTORY_BUFFER_SIZE, 6},
    {getString(STRING_HISTORY24_H), 24 * 60 * 60 / HISTORY_BUFFER_SIZE, 8},
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
    20E-3F,
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
    100E-3F,
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

        uint32_t lossOfCountTimer;
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
    24 * 60 * 60,      // 24 hours
    12 * 60 * 60,      // 12 hours
    6 * 60 * 60,       // 6 hours
    3 * 60 * 60,       // 3 hours
    60 * 60,           // 1 hour
    30 * 60,           // 30 minutes
    10 * 60,           // 10 minutes
    5 * 60,            // 5 minutes
    60,                // 1 minute
    30,                // 30 minute
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

static const View instantaneousRateView;
static const View averageRateView;
static const View cumulativeDoseView;
static const View historyView;

static const Menu alarmsMenu;
static const Menu rateAlarmMenu;
static const Menu doseAlarmMenu;
static const Menu alarmIndicationMenu;
#if defined(VOICE)
static const Menu alarmVolumeMenu;
#endif
static const Menu measurementsMenu;
static const Menu unitsMenu;
static const Menu secondaryUnitsMenu;
static const Menu instantaneousMenu;
static const Menu averageMenu;

static const char stringCounts[] = getString(STRING_COUNTS);

static void onAlarmsSubMenuBack(const Menu *menu);
static void onMeasurementsSubMenuBack(const Menu *menu);

static void resetInstantaneousRate(void);
static void resetAverageRate(void);
static void resetCumulativeDose(void);
static void resetHistory(void);

// Measurements

static const View *const measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &cumulativeDoseView,
    &historyView,
};

#define MEASUREMENT_VIEWS_NUM (sizeof(measurementViews) / sizeof(View *))

void resetMeasurements(void)
{
    Dose tubeDose = measurements.tube.dose;
    Dose cumulativeDose = measurements.cumulativeDose.dose;

    memset(&measurements, 0, sizeof(measurements));

    measurements.tube.dose = tubeDose;
    measurements.cumulativeDose.dose = cumulativeDose;

    updateMeasurementUnits();

#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif

    selectMenuItem(&alarmsMenu,
                   0,
                   0);
    selectMenuItem(&rateAlarmMenu,
                   settings.rateAlarm,
                   RATEALARM_NUM);
    selectMenuItem(&doseAlarmMenu,
                   settings.doseAlarm,
                   DOSEALARM_NUM);
    selectMenuItem(&alarmIndicationMenu,
                   0,
                   0);
#if defined(VOICE)
    selectMenuItem(&alarmVolumeMenu,
                   settings.alarmVolume,
                   ALARM_VOLUME_NUM);
#endif

    selectMenuItem(&measurementsMenu,
                   0,
                   0);
    selectMenuItem(&unitsMenu,
                   settings.units,
                   UNITS_NUM);
    selectMenuItem(&secondaryUnitsMenu,
                   settings.secondaryUnits,
                   UNITS_NUM);
    selectMenuItem(&instantaneousMenu,
                   settings.instantaneousAveraging,
                   INSTANTANEOUSAVERAGING_NUM);
    selectMenuItem(&averageMenu,
                   settings.averaging,
                   AVERAGING_NUM);
}

// Measurement events

typedef struct
{
    const char *name;
    float scale;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} Units;

Units units[] = {
    {{getString(STRING_SVH), (60 * 1E-6F)},
     {getString(STRING_SV), (60 * 1E-6F / 3600)}},
    {{getString(STRING_REMH), (60 * 1E-4F)},
     {getString(STRING_REM), (60 * 1E-4F / 3600)}},
    {{getString(STRING_CPM), 60},
     {stringCounts, 1}},
    {{getString(STRING_CPS), 1},
     {stringCounts, 1}},
};

const int8_t unitsMinMetricPrefixIndex[] = {
    -2, -2, 0, 0};

static const int8_t unitsMeasurementBarMinExponent[] = {
    -8, -6, 0, -2};

void updateMeasurementUnits(void)
{
    float sensitivity = getTubeSensitivity();

    units[UNITS_SIEVERTS].rate.scale = (60 * 1E-6F) / sensitivity;
    units[UNITS_SIEVERTS].dose.scale = (60 * 1E-6F / 3600) / sensitivity;
    units[UNITS_REM].rate.scale = (60 * 1E-4F) / sensitivity;
    units[UNITS_REM].dose.scale = (60 * 1E-4F / 3600) / sensitivity;

    if (sensitivity < PULSE_INDICATION_SENSITIVITY_MAX)
        measurements.instantaneous.pulseIndicationFactor = PULSE_INDICATION_FACTOR_UNIT;
    else
        measurements.instantaneous.pulseIndicationFactor = (PULSE_INDICATION_FACTOR_UNIT * PULSE_INDICATION_SENSITIVITY_MAX) / sensitivity;
}

float getCurrentRateFactor(void)
{
    return units[settings.units].rate.scale;
}

float getCurrentDoseFactor(void)
{
    return units[settings.units].dose.scale;
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

    float confidenceInterval = getConfidenceInterval(pulseCount - 1);

    // Dead-time compensation
    // float absConfidenceInterval = value * (1 + confidenceInterval);
    // float compValue = value * valueFactor;
    // float compAbsConfidenceInterval = absConfidenceInterval * confidenceIntervalFactor;
    // float compConfidenceInterval = (compAbsConfidenceInterval - compValue) / compValue;
    float valueFactor = getDeadTimeCompensationFactor(value);
    float confidenceIntervalFactor = getDeadTimeCompensationFactor(value * (1 + confidenceInterval));

    rate->value = valueFactor * value;
    rate->confidence = confidenceIntervalFactor / valueFactor * (1 + confidenceInterval) - 1;
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
    if (!measurements.enabled)
        return;

    if (pulseCount)
    {
        measurements.tube.dose.pulseCount += pulseCount;

        if (!measurements.period.measurement.pulseCount)
            measurements.period.measurement.firstPulseTick = getTick();
        measurements.period.measurement.lastPulseTick = getTick();
        measurements.period.measurement.pulseCount += pulseCount;

        // Pulse indication divider
        measurements.instantaneous.pulseIndicationRemainder += pulseCount * measurements.instantaneous.pulseIndicationFactor;

        if (measurements.instantaneous.pulseIndicationRemainder >= PULSE_INDICATION_FACTOR_UNIT)
        {
            measurements.instantaneous.pulseIndicationRemainder &= PULSE_INDICATION_FACTOR_MASK;

            triggerPulse();
        }
    }
}

void onMeasurementPeriod(void)
{
    if (!measurements.enabled)
        return;

    measurements.period.snapshotTick = getTick();
    measurements.period.snapshotMeasurement = measurements.period.measurement;
    measurements.period.measurement.pulseCount = 0;
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

bool isInstantaneousRateConfidenceAcceptable(void)
{
    return (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_60SECONDS) ||
           ((measurements.instantaneous.rate.confidence > 0.0F) &&
            (measurements.instantaneous.rate.confidence < 0.75F));
}

void updateMeasurements(void)
{
    if (!measurements.enabled)
        return;

    // Tube life
    measurements.tube.dose.time += 1;

    // Loss-of-count detection
    if (measurements.period.snapshotMeasurement.pulseCount)
        measurements.period.lossOfCountTimer = 0;
    else
        measurements.period.lossOfCountTimer++;
    measurements.period.faultAlarm = (measurements.period.lossOfCountTimer >= getLossOfCountTime());

    // Shortcircuit detection
    bool isTubeShortCircuit = (!measurements.period.snapshotMeasurement.pulseCount && getTubeDet());
    if (measurements.period.lastShortCircuit && isTubeShortCircuit)
        measurements.period.faultAlarm = true;
    measurements.period.lastShortCircuit = isTubeShortCircuit;

    // Dear-time Compensation
    Measurement compensatedMeasurement;

    compensatedMeasurement.firstPulseTick = measurements.period.snapshotMeasurement.firstPulseTick;
    compensatedMeasurement.lastPulseTick = measurements.period.snapshotMeasurement.lastPulseTick;

    measurements.period.deadTimeCompensationRemainder += measurements.period.snapshotMeasurement.pulseCount *
                                                         getDeadTimeCompensationFactor(measurements.instantaneous.rate.value);

    compensatedMeasurement.pulseCount = (uint32_t)measurements.period.deadTimeCompensationRemainder;
    if (measurements.period.deadTimeCompensationRemainder >= 1)
        measurements.period.deadTimeCompensationRemainder -= compensatedMeasurement.pulseCount;

    // Instantaneous rate
    if (compensatedMeasurement.pulseCount)
    {
        // Enqueue
        measurements.instantaneous.queue[measurements.instantaneous.queueTail] = compensatedMeasurement;
        measurements.instantaneous.queueTail = (measurements.instantaneous.queueTail + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;

        if (measurements.instantaneous.queueTail == measurements.instantaneous.queueHead)
            measurements.instantaneous.queueHead = (measurements.instantaneous.queueTail + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;
    }

    Measurement instantaneousMeasurement;
    instantaneousMeasurement.firstPulseTick = 0;
    instantaneousMeasurement.lastPulseTick = 0;
    instantaneousMeasurement.pulseCount = 0;

    measurements.instantaneous.rate.time = 0;

    uint32_t averagingPeriod = instantaneousAveragingPeriods[settings.instantaneousAveraging];
    uint32_t averagingPulseCount = (settings.instantaneousAveraging <= INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION)
                                       ? INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT
                                       : 0;

    uint32_t queueSize = (measurements.instantaneous.queueTail - measurements.instantaneous.queueHead) &
                         INSTANTANEOUS_RATE_QUEUE_MASK;
    for (uint32_t i = 0; i < queueSize; i++)
    {
        uint32_t queueIndex = (measurements.instantaneous.queueTail - 1 - i) & INSTANTANEOUS_RATE_QUEUE_MASK;

        uint32_t timePeriod = 1 + (measurements.period.snapshotTick - measurements.instantaneous.queue[queueIndex].firstPulseTick) / SYSTICK_FREQUENCY;

        if (timePeriod > averagingPeriod)
        {
            if (instantaneousMeasurement.pulseCount > averagingPulseCount)
            {
                measurements.instantaneous.queueHead = (queueIndex + 1) & INSTANTANEOUS_RATE_QUEUE_MASK;

                break;
            }
        }

        instantaneousMeasurement.firstPulseTick = measurements.instantaneous.queue[queueIndex].firstPulseTick;
        if (!instantaneousMeasurement.pulseCount)
            instantaneousMeasurement.lastPulseTick = measurements.instantaneous.queue[queueIndex].lastPulseTick;
        instantaneousMeasurement.pulseCount += measurements.instantaneous.queue[queueIndex].pulseCount;
        measurements.instantaneous.rate.time = timePeriod;
    }

    calculateRate(instantaneousMeasurement.pulseCount,
                  instantaneousMeasurement.lastPulseTick -
                      instantaneousMeasurement.firstPulseTick,
                  &measurements.instantaneous.rate);

    if (isInstantaneousRateConfidenceAcceptable() &&
        (measurements.instantaneous.rate.value > measurements.instantaneous.maxValue))
        measurements.instantaneous.maxValue = measurements.instantaneous.rate.value;

    float svH = units[UNITS_SIEVERTS].rate.scale * measurements.instantaneous.rate.value;

    setPulseThreshold(svH < rateAlarmsSvH[settings.pulseThreshold]);

    bool instantaneousRateAlarm = false;
    if (isInstantaneousRateConfidenceAcceptable())
    {
        if (settings.rateAlarm)
            instantaneousRateAlarm |= (svH >= rateAlarmsSvH[settings.rateAlarm]) << INSTANTANEOUS_RATE_ALARM_RATE;
    }

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
                measurements.average.firstPulseTick = compensatedMeasurement.firstPulseTick;

            measurements.average.lastPulseTick = compensatedMeasurement.lastPulseTick;

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
        bool timerExpired = (settings.averaging < AVERAGING_TIME_NUM)
                                ? (measurements.average.rate.time >= averagingTimes[settings.averaging])
                                : (measurements.average.rate.confidence != 0.0F) &&
                                      (measurements.average.rate.confidence < averagingConfidences[settings.averaging - AVERAGING_TIME_NUM]);

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

    bool cumulativeDoseAlarm = false;
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
        if (historyState->sampleIndex >= history->samplesPerDataPoint)
            historyState->sampleIndex = 0;

        if (isInstantaneousRateConfidenceAcceptable() &&
            (measurements.instantaneous.rate.value >= 0))
            historyState->averageSum += measurements.instantaneous.rate.value;
        historyState->averageCount++;

        if (historyState->sampleIndex == 0)
        {
            float averageRate = historyState->averageSum / historyState->averageCount;
            historyState->buffer[historyState->bufferIndex] = getHistoryValue(averageRate);

            historyState->averageSum = 0;
            historyState->averageCount = 0;

            historyState->bufferIndex++;
            if (historyState->bufferIndex >= HISTORY_BUFFER_SIZE)
                historyState->bufferIndex = 0;
        }
    }

    // Pulse control
#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif

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
    if ((unit->name == stringCounts) &&
        (value < 10000))
    {
        uint32_t intValue = (uint32_t)value;

        strcatUInt32(valueString, intValue, 0);
        strcpy(unitString, " ");
        if (intValue == 1)
            strcat(unitString, getString(STRING_COUNT));
        else
            strcat(unitString, stringCounts);

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

// Instantaneous rate

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

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            triggerVibration();

            if (measurements.instantaneous.alarm)
                measurements.instantaneous.alarm = false;
            else
                resetInstantaneousRate();

            requestViewUpdate();
        }

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        const char *stateString;
        const char *keyString = NULL;
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
            stateString = getString(STRING_INFO_FAULT);

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.instantaneous.alarm)
        {
            stateString = getString(STRING_INFO_ALARM);

            style = MEASUREMENTSTYLE_ALARM;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar(getString(STRING_INSTANTANEOUS), false);
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

            keyString = getString(STRING_TIME);

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

            keyString = getString(STRING_MAX);

            break;
        }

        case INSTANTANEOUS_TAB_RATE:
            if (measurements.instantaneous.rate.value > 0)
            {
                buildValueString(valueString,
                                 unitString,
                                 measurements.instantaneous.rate.value,
                                 &units[settings.secondaryUnits].rate,
                                 unitsMinMetricPrefixIndex[settings.secondaryUnits]);
            }

            keyString = getString(STRING_RATE);

            break;
        }

        if (measurements.instantaneousTabIndex != INSTANTANEOUS_TAB_BAR)
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

static const View instantaneousRateView = {
    onInstantaneousRateViewEvent,
    NULL,
};

// Average rate

static void resetAverageRate(void)
{
    memset(&measurements.average,
           0,
           sizeof(measurements.average));
}

float getAverageRate(void)
{
    return measurements.average.timedRate.value;
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

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            triggerVibration();

            resetAverageRate();

            requestViewUpdate();
        }

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceAverageRate();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        const char *stateString;
        const char *keyString = NULL;
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
            stateString = getString(STRING_INFO_FAULT);

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.average.pulseCount == UINT32_MAX)
        {
            stateString = getString(STRING_INFO_MAX);

            style = MEASUREMENTSTYLE_HOLD;
        }
        else if (measurements.average.timerExpired)
        {
            stateString = getString(STRING_INFO_DONE);

            style = MEASUREMENTSTYLE_HOLD;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar(getString(STRING_AVERAGE), false);
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

            keyString = getString(STRING_TIME);

            break;

        case AVERAGE_TAB_RATE:
            if (measurements.average.timedRate.value > 0)
            {
                buildValueString(valueString,
                                 unitString,
                                 measurements.average.timedRate.value,
                                 &units[settings.secondaryUnits].rate,
                                 unitsMinMetricPrefixIndex[settings.secondaryUnits]);
            }

            keyString = getString(STRING_RATE);

            break;

        case AVERAGE_TAB_DOSE:
            if (measurements.average.timedPulseCount > 0)
                buildValueString(valueString,
                                 unitString,
                                 measurements.average.timedPulseCount,
                                 &units[UNITS_CPM].dose,
                                 unitsMinMetricPrefixIndex[UNITS_CPM]);

            keyString = getString(STRING_DOSE);

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

static const View averageRateView = {
    onAverageRateViewEvent,
    NULL,
};

// Cumulative dose

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

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            triggerVibration();

            if (measurements.cumulativeDose.alarm)
                measurements.cumulativeDose.alarm = false;
            else
                resetCumulativeDose();

            requestViewUpdate();
        }

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceCumulativeDose();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[16];
        char unitString[16];
        const char *stateString;
        const char *keyString = NULL;
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
            stateString = getString(STRING_INFO_FAULT);

            style = MEASUREMENTSTYLE_NORMAL;
        }
        else if (measurements.cumulativeDose.alarm)
        {
            stateString = getString(STRING_INFO_ALARM);

            style = MEASUREMENTSTYLE_ALARM;
        }
        else if (measurements.cumulativeDose.dose.pulseCount == UINT32_MAX)
        {
            stateString = getString(STRING_INFO_MAX);

            style = MEASUREMENTSTYLE_HOLD;
        }
        else
        {
            stateString = "";

            style = MEASUREMENTSTYLE_NORMAL;
        }

        drawTitleBar(getString(STRING_CUMULATIVE), false);
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

            keyString = getString(STRING_TIME);

            break;

        case CUMULATIVE_TAB_DOSE:
            if (measurements.cumulativeDose.dose.pulseCount > 0)
            {
                buildValueString(valueString,
                                 unitString,
                                 measurements.cumulativeDose.dose.pulseCount,
                                 &units[settings.secondaryUnits].dose,
                                 unitsMinMetricPrefixIndex[settings.secondaryUnits]);
            }

            keyString = getString(STRING_DOSE);

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

static const View cumulativeDoseView = {
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

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        if (!isLockMode())
        {
            triggerVibration();

            resetHistory();

            requestViewUpdate();
        }

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        HistoryState *historyState = &measurements.history.states[measurements.history.tabIndex];
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

static const View historyView = {
    onHistoryViewEvent,
    NULL,
};

// Rate alarm menu

char *buildRateAlarmMenuOption(uint32_t index)
{
    const Unit *rateUnit = &units[settings.units].rate;
    float value = rateAlarmsSvH[index] / units[UNITS_SIEVERTS].rate.scale;

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
        return getString(STRING_OFF);
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
    getString(STRING_RATE_ALARM),
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
        return getString(STRING_OFF);
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
    getString(STRING_DOSE_ALARM),
    &doseAlarmMenuState,
    onDoseAlarmMenuGetOption,
    onDoseAlarmMenuSelect,
    onAlarmsSubMenuBack,
};

static const View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};

// Alarm indication menu

static const char *const alarmIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(VOICE)
    getString(STRING_SOUND),
#endif
#if defined(VOICE)
    getString(STRING_VOICE),
#endif
#if defined(VIBRATION)
    getString(STRING_VIBRATION),
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_CONTROL)
    getString(STRING_PULSE_LED),
#endif
    getString(STRING_DISPLAY_FLASH),
    NULL,
};

static const char *onAlarmIndicationMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (settings.alarmIndication >> index) & 1;

    return alarmIndicationMenuOptions[index];
}

static void onAlarmIndicationMenuSelect(const Menu *menu)
{
    settings.alarmIndication ^= (1 << menu->state->selectedIndex);
}

static MenuState alarmIndicationMenuState;

static const Menu alarmIndicationMenu = {
    getString(STRING_INDICATION),
    &alarmIndicationMenuState,
    onAlarmIndicationMenuGetOption,
    onAlarmIndicationMenuSelect,
    onAlarmsSubMenuBack,
};

static const View alarmIndicationMenuView = {
    onMenuEvent,
    &alarmIndicationMenu,
};

// Alarm volume menu

#if defined(VOICE)

static const char *const alarmVolumeMenuOptions[] = {
    getString(STRING_VERY_LOW),
    getString(STRING_LOW),
    getString(STRING_MEDIUM),
    getString(STRING_HIGH),
    NULL,
};

static const char *onAlarmVolumeMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.alarmVolume);

    return alarmVolumeMenuOptions[index];
}

static void onAlarmVolumeMenuSelect(const Menu *menu)
{
    uint32_t index = menu->state->selectedIndex;

    settings.alarmVolume = index;

    updateVoiceVolume();
}

static MenuState alarmVolumeMenuState;

static const Menu alarmVolumeMenu = {
    getString(STRING_VOLUME),
    &alarmVolumeMenuState,
    onAlarmVolumeMenuGetOption,
    onAlarmVolumeMenuSelect,
    onAlarmsSubMenuBack,
};

const View alarmVolumeMenuView = {
    onMenuEvent,
    &alarmVolumeMenu,
};

#endif

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

bool isInstantaneousRateAlarm(void)
{
    return measurements.instantaneous.alarm;
}

bool isCumulativeDoseAlarm(void)
{
    return measurements.cumulativeDose.alarm;
}

static const OptionView alarmsMenuOptions[] = {
    {getString(STRING_RATE_ALARM), &rateAlarmMenuView},
    {getString(STRING_DOSE_ALARM), &doseAlarmMenuView},
    {getString(STRING_INDICATION), &alarmIndicationMenuView},
#if defined(VOICE)
    {getString(STRING_VOLUME), &alarmVolumeMenuView},
#endif
    {NULL},
};

static const char *onAlarmsMenuGetOption(const Menu *menu,
                                         uint32_t index,
                                         MenuStyle *menuStyle)
{
    const OptionView *optionView = &alarmsMenuOptions[index];

    *menuStyle = MENUSTYLE_SUBMENU;

    return optionView->option;
}

static void onAlarmsMenuSelect(const Menu *menu)
{
    const OptionView *optionView = &alarmsMenuOptions[menu->state->selectedIndex];

    setView(optionView->view);
}

static MenuState alarmsMenuState;

static const Menu alarmsMenu = {
    getString(STRING_ALARMS),
    &alarmsMenuState,
    onAlarmsMenuGetOption,
    onAlarmsMenuSelect,
    onSettingsSubMenuBack,
};

const View alarmsMenuView = {
    onMenuEvent,
    &alarmsMenu,
};

static void onAlarmsSubMenuBack(const Menu *menu)
{
    setView(&alarmsMenuView);
}

// Units menu

static const char *const unitsMenuOptions[] = {
    getString(STRING_SIEVERT),
    getString(STRING_REM),
    getString(STRING_CPM),
    getString(STRING_CPS),
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
    getString(STRING_UNITS),
    &unitsMenuState,
    onUnitsMenuGetOption,
    onUnitsMenuSelect,
    onMeasurementsSubMenuBack,
};

static const View unitsMenuView = {
    onMenuEvent,
    &unitsMenu,
};

// Secondary units menu

static const char *onSecondaryUnitsMenuGetOption(const Menu *menu,
                                                 uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.secondaryUnits);

    return unitsMenuOptions[index];
}

static void onSecondaryUnitsMenuSelect(const Menu *menu)
{
    settings.secondaryUnits = menu->state->selectedIndex;
}

static MenuState secondaryUnitsMenuState;

static const Menu secondaryUnitsMenu = {
    getString(STRING_UNITS),
    &secondaryUnitsMenuState,
    onSecondaryUnitsMenuGetOption,
    onSecondaryUnitsMenuSelect,
    onMeasurementsSubMenuBack,
};

static const View secondaryUnitsMenuView = {
    onMenuEvent,
    &secondaryUnitsMenu,
};

// Instantaneous menu

static const char *const instantaneousMenuOptions[] = {
    getString(STRING_ADAPTIVE_FAST),
    getString(STRING_ADAPTIVE_PRECISION),
    getString(STRING_1_MINUTE),
    getString(STRING_30_SECONDS),
    getString(STRING_10_SECONDS),
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
    getString(STRING_INSTANTANEOUS),
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
    getString(STRING_UNLIMITED),
    getString(STRING_24_HOURS),
    getString(STRING_12_HOURS),
    getString(STRING_6_HOURS),
    getString(STRING_3_HOURS),
    getString(STRING_1_HOUR),
    getString(STRING_30_MINUTES),
    getString(STRING_10_MINUTES),
    getString(STRING_5_MINUTES),
    getString(STRING_1_MINUTE),
    getString(STRING_30_SECONDS),
    getString(STRING_10_SECONDS),
    getString(STRING_5_SECONDS),
    getString(STRING_1_SECOND),
    getString(STRING_50_CONFIDENCE),
    getString(STRING_20_CONFIDENCE),
    getString(STRING_10_CONFIDENCE),
    getString(STRING_5_CONFIDENCE),
    getString(STRING_2_CONFIDENCE),
    getString(STRING_1_CONFIDENCE),
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
    getString(STRING_AVERAGE),
    &averageMenuState,
    onAverageMenuGetOption,
    onAverageMenuSelect,
    onMeasurementsSubMenuBack,
};

static const View averageMenuView = {
    onMenuEvent,
    &averageMenu,
};

// Measurements menu

static const OptionView measurementMenuOptions[] = {
    {getString(STRING_UNITS), &unitsMenuView},
    {getString(STRING_SECONDARY_UNITS), &secondaryUnitsMenuView},
    {getString(STRING_INSTANTANEOUS), &instantaneousMenuView},
    {getString(STRING_AVERAGE), &averageMenuView},
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

static const Menu measurementsMenu = {
    getString(STRING_MEASUREMENTS),
    &measurementsMenuState,
    onMeasurementsMenuGetOption,
    onMeasurementsMenuSelect,
    onSettingsSubMenuBack,
};

const View measurementsMenuView = {
    onMenuEvent,
    &measurementsMenu,
};

static void onMeasurementsSubMenuBack(const Menu *menu)
{
    setView(&measurementsMenuView);
}
