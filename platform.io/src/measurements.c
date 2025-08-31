/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <float.h>
#include <limits.h>

#include "cmath.h"
#include "cstring.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "led.h"
#include "measurements.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"
#include "sound.h"
#include "system.h"
#include "tube.h"
#include "view.h"

#define INSTANTANEOUS_RATE_QUEUE_SIZE 64
#define INSTANTANEOUS_RATE_QUEUE_MASK (INSTANTANEOUS_RATE_QUEUE_SIZE - 1)
#define INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT 19 // For 50% confidence interval

#define PULSE_INDICATION_SENSITIVITY_MAX 600.0F
#define PULSE_INDICATION_FACTOR_UNIT 0x10000
#define PULSE_INDICATION_FACTOR_MASK (PULSE_INDICATION_FACTOR_UNIT - 1)

enum InstantaneousTab
{
    INSTANTANEOUS_TAB_BAR,
    INSTANTANEOUS_TAB_TIME,
    INSTANTANEOUS_TAB_MAX,
    INSTANTANEOUS_TAB_RATE,

    INSTANTANEOUS_TAB_NUM,

    INSTANTANEOUS_TAB_ALERT = INSTANTANEOUS_TAB_NUM,
};

enum AverageTab
{
    AVERAGE_TAB_TIME,
    AVERAGE_TAB_RATE,
    AVERAGE_TAB_DOSE,

    AVERAGE_TAB_NUM,

    AVERAGE_TAB_ALERT = AVERAGE_TAB_NUM,
};

enum CumulativeDose
{
    CUMULATIVEDOSE_TAB_TIME,
    CUMULATIVEDOSE_TAB_DOSE,

    CUMULATIVEDOSE_TAB_NUM,

    CUMULATIVEDOSE_TAB_ALERT = CUMULATIVEDOSE_TAB_NUM,
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

typedef bool Alert;

typedef struct
{
    uint32_t timeInterval;

    float cumulativeTime;
    uint32_t cumulativePulseCount;

    uint8_t bins[HISTORY_BIN_NUM];
} HistoryState;

typedef const struct
{
    cstring name;
    uint32_t binInterval;
    uint8_t xTickNum;
} History;

static History histories[] = {
    {STRING_HISTORY_10_MINUTES, 10 * 60 / HISTORY_BIN_NUM, 10},
    {STRING_HISTORY_1_HOUR, 60 * 60 / HISTORY_BIN_NUM, 6},
    {STRING_HISTORY_1_DAY, 24 * 60 * 60 / HISTORY_BIN_NUM, 8},
    {STRING_HISTORY_1_WEEK, 7 * 24 * 60 * 60 / HISTORY_BIN_NUM, 7},
    {STRING_HISTORY_1_MONTH, 28 * 24 * 60 * 60 / HISTORY_BIN_NUM, 4},
    {STRING_HISTORY_1_YEAR, 365 * 24 * 60 * 60 / HISTORY_BIN_NUM, 12},
};

#define HISTORY_NUM (sizeof(histories) / sizeof(History))

static const float rateAlertsSvH[] = {
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

static const float doseAlertsSv[] = {
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
        bool lastTubeShorted;
        Alert faultAlarm;

        bool alertDismissed;
        bool alertBlink;
    } heartbeat;

    struct
    {
        uint32_t queueHead;
        uint32_t queueTail;
        Measurement queue[INSTANTANEOUS_RATE_QUEUE_SIZE];

        uint32_t pulseIndicationFactor;
        uint32_t pulseIndicationRemainder;

        Rate rate;
        float maxValue;

        Alert warning;
        Alert alarm;
    } instantaneous;

    enum InstantaneousTab instantaneousTabIndex;
    const char *instantaneousAlertString;

    struct
    {
        uint32_t firstPulseTick;
        uint32_t lastPulseTick;
        uint32_t pulseCount;

        Rate rate;
        bool timerExpired;
        uint32_t timerPulseCount;
        Rate timerRate;
    } average;

    enum AverageTab averageTabIndex;
    const char *averageAlertString;

    struct
    {
        Dose dose;

        Alert warning;
        Alert alarm;
    } cumulativeDose;

    enum CumulativeDose cumulativeDoseTabIndex;
    const char *cumulativeDoseAlertString;

    struct
    {
        HistoryState states[HISTORY_NUM];

        uint32_t tabIndex;
    } history;

    bool enabled;
    int32_t viewIndex;

    bool isPulseSoundIconEnabled;
} measurements;

static const uint8_t instantaneousAveragingIntervals[] = {
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

static View instantaneousRateView;
static View averageRateView;
static View cumulativeDoseView;
static View historyView;

static View rateAlarmMenuView;
static View doseAlarmMenuView;

static Menu alertsMenu;
static Menu rateWarningMenu;
static Menu doseWarningMenu;
static Menu rateAlarmMenu;
static Menu doseAlarmMenu;
static Menu alertIndicationMenu;

static Menu measurementsMenu;
static Menu unitsMenu;
static Menu secondaryUnitsMenu;
static Menu instantaneousMenu;
static Menu averageMenu;

static void onAlertsSubMenuBack(void);
static void onMeasurementsSubMenuBack(void);

static void resetInstantaneousRate(void);
static void resetAverageRate(void);
static void resetCumulativeDose(void);

// Measurements

static ViewPointer measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &cumulativeDoseView,
    &historyView,
};

#define MEASUREMENT_VIEWS_NUM (sizeof(measurementViews) / sizeof(ViewPointer))

void resetMeasurements(void)
{
    Dose tubeDose = measurements.tube.dose;
    Dose cumulativeDose = measurements.cumulativeDose.dose;

    memset(&measurements, 0, sizeof(measurements));

    measurements.tube.dose = tubeDose;
    measurements.cumulativeDose.dose = cumulativeDose;

    updateMeasurementUnits();

    selectMenuItem(&alertsMenu,
                   0,
                   0);
    selectMenuItem(&rateWarningMenu,
                   settings.rateWarning,
                   RATE_NUM);
    selectMenuItem(&doseWarningMenu,
                   settings.doseWarning,
                   DOSE_NUM);
    selectMenuItem(&rateAlarmMenu,
                   settings.rateAlarm,
                   RATE_NUM);
    selectMenuItem(&doseAlarmMenu,
                   settings.doseAlarm,
                   DOSE_NUM);
    selectMenuItem(&alertIndicationMenu,
                   0,
                   0);

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
    cstring name;
    float scale;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} Units;

Units units[] = {
    {{STRING_SVH, (60 * 1E-6F)},
     {STRING_SV, (60 * 1E-6F / 3600)}},
    {{STRING_REMH, (60 * 1E-4F)},
     {STRING_REM, (60 * 1E-4F / 3600)}},
    {{STRING_CPM, 60},
     {STRING_EMPTY, 1}},
    {{STRING_CPS, 1},
     {STRING_EMPTY, 1}},
};

static const int8_t unitsMinMetricPrefixIndex[] = {
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

void setMeasurements(bool value)
{
    measurements.enabled = value;
}

static void updateAlert(Alert *alert, bool value)
{
    if (!*alert && value)
        measurements.heartbeat.alertDismissed = false;

    *alert = value;
}

void onMeasurementsTick(uint32_t pulseCount)
{
    if (!measurements.enabled)
        return;

    if (pulseCount)
    {
        measurements.tube.dose.pulseCount += pulseCount;

        if (!measurements.heartbeat.measurement.pulseCount)
            measurements.heartbeat.measurement.firstPulseTick = getTick();
        measurements.heartbeat.measurement.lastPulseTick = getTick();
        measurements.heartbeat.measurement.pulseCount += pulseCount;

        // Pulse indication divider
        measurements.instantaneous.pulseIndicationRemainder += pulseCount * measurements.instantaneous.pulseIndicationFactor;
        if (measurements.instantaneous.pulseIndicationRemainder >= PULSE_INDICATION_FACTOR_UNIT)
        {
            measurements.instantaneous.pulseIndicationRemainder &= PULSE_INDICATION_FACTOR_MASK;

            triggerPulse();
        }
    }
}

void onMeasurementsHeartbeat(void)
{
    if (!measurements.enabled)
        return;

    measurements.heartbeat.snapshotTick = getTick();
    measurements.heartbeat.snapshotMeasurement = measurements.heartbeat.measurement;
    measurements.heartbeat.measurement.pulseCount = 0;
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

bool isInstantaneousRateConfidenceSufficient(void)
{
    return (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_60_SECONDS) ||
           ((measurements.instantaneous.rate.confidence > 0.0F) &&
            (measurements.instantaneous.rate.confidence < 0.75F));
}

void updateMeasurements(void)
{
    if (!measurements.enabled)
        return;

    // Tube life
    measurements.tube.dose.time += 1;

    // Loss of count and tube shorted detection
    bool faultAlarm = false;

    if (measurements.heartbeat.snapshotMeasurement.pulseCount)
        measurements.heartbeat.lossOfCountTimer = 0;
    else
        measurements.heartbeat.lossOfCountTimer++;
    faultAlarm = (measurements.heartbeat.lossOfCountTimer >= getLossOfCountTime());

    bool isTubeShorted = (!measurements.heartbeat.snapshotMeasurement.pulseCount && getTubeDet());
    if (measurements.heartbeat.lastTubeShorted && isTubeShorted)
        faultAlarm = true;
    measurements.heartbeat.lastTubeShorted = isTubeShorted;

    updateAlert(&measurements.heartbeat.faultAlarm, faultAlarm);

    // Dead-time Compensation
    Measurement compensatedMeasurement;

    compensatedMeasurement.firstPulseTick = measurements.heartbeat.snapshotMeasurement.firstPulseTick;
    compensatedMeasurement.lastPulseTick = measurements.heartbeat.snapshotMeasurement.lastPulseTick;

    measurements.heartbeat.deadTimeCompensationRemainder += measurements.heartbeat.snapshotMeasurement.pulseCount *
                                                            getDeadTimeCompensationFactor(measurements.instantaneous.rate.value);

    compensatedMeasurement.pulseCount = (uint32_t)measurements.heartbeat.deadTimeCompensationRemainder;
    if (measurements.heartbeat.deadTimeCompensationRemainder >= 1)
        measurements.heartbeat.deadTimeCompensationRemainder -= compensatedMeasurement.pulseCount;

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

    uint32_t averagingTime = instantaneousAveragingIntervals[settings.instantaneousAveraging];
    uint32_t averagingPulseCount = (settings.instantaneousAveraging <= INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION)
                                       ? INSTANTANEOUS_RATE_AVERAGING_PULSE_COUNT
                                       : 0;

    uint32_t queueSize = (measurements.instantaneous.queueTail - measurements.instantaneous.queueHead) &
                         INSTANTANEOUS_RATE_QUEUE_MASK;
    for (uint32_t i = 0; i < queueSize; i++)
    {
        uint32_t queueIndex = (measurements.instantaneous.queueTail - 1 - i) & INSTANTANEOUS_RATE_QUEUE_MASK;

        uint32_t time = 1 + (measurements.heartbeat.snapshotTick - measurements.instantaneous.queue[queueIndex].firstPulseTick) / SYSTICK_FREQUENCY;

        if (time > averagingTime)
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
        measurements.instantaneous.rate.time = time;
    }

    calculateRate(instantaneousMeasurement.pulseCount,
                  instantaneousMeasurement.lastPulseTick -
                      instantaneousMeasurement.firstPulseTick,
                  &measurements.instantaneous.rate);

    bool instantaneousRateConfidenceSufficient = isInstantaneousRateConfidenceSufficient();

    if ((measurements.instantaneous.rate.value > measurements.instantaneous.maxValue) &&
        instantaneousRateConfidenceSufficient)
        measurements.instantaneous.maxValue = measurements.instantaneous.rate.value;

    float rateSvH = units[UNITS_SIEVERTS].rate.scale *
                    measurements.instantaneous.rate.value;

    updateAlert(&measurements.instantaneous.warning,
                instantaneousRateConfidenceSufficient &&
                    settings.rateWarning &&
                    (rateSvH >= rateAlertsSvH[settings.rateWarning]));
    updateAlert(&measurements.instantaneous.alarm,
                instantaneousRateConfidenceSufficient &&
                    settings.rateAlarm &&
                    (rateSvH >= rateAlertsSvH[settings.rateAlarm]));
    setPulseThresholdExceeded(settings.pulseThreshold
                                  ? rateSvH >= rateAlertsSvH[settings.pulseThreshold]
                                  : true);

    // Average rate
    if ((measurements.average.rate.time < UINT32_MAX) &&
        (measurements.average.pulseCount < UINT32_MAX))
    {
        addClamped(&measurements.average.rate.time, 1);
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

        // Average timer
        bool timerExpired = (settings.averaging < AVERAGING_TIME_NUM)
                                ? (measurements.average.rate.time >= averagingTimes[settings.averaging])
                                : (measurements.average.rate.confidence != 0.0F) &&
                                      (measurements.average.rate.confidence < averagingConfidences[settings.averaging - AVERAGING_TIME_NUM]);

        if (!timerExpired)
        {
            measurements.average.timerPulseCount = measurements.average.pulseCount;
            measurements.average.timerRate = measurements.average.rate;
        }

        if (timerExpired && !measurements.average.timerExpired)
        {
            measurements.average.timerExpired = true;

            measurements.average.timerPulseCount = measurements.average.pulseCount;
            measurements.average.timerRate = measurements.average.rate;

            triggerAlert();
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

    float doseSv = units[UNITS_SIEVERTS].dose.scale *
                   measurements.cumulativeDose.dose.pulseCount;
    updateAlert(&measurements.cumulativeDose.warning,
                settings.doseWarning &&
                    (doseSv >= doseAlertsSv[settings.doseWarning]));
    updateAlert(&measurements.cumulativeDose.alarm,
                settings.doseAlarm &&
                    (doseSv >= doseAlertsSv[settings.doseAlarm]));

    // History
    for (uint32_t historyIndex = 0; historyIndex < HISTORY_NUM; historyIndex++)
    {
        History *history = &histories[historyIndex];
        HistoryState *historyState = &measurements.history.states[historyIndex];

        if (instantaneousRateConfidenceSufficient &&
            (measurements.instantaneous.rate.value >= 0))
        {
            historyState->cumulativeTime += measurements.instantaneous.rate.value;
            historyState->cumulativePulseCount++;
        }

        historyState->timeInterval++;
        if (historyState->timeInterval >= history->binInterval)
        {
            float averageRate = historyState->cumulativePulseCount
                                    ? historyState->cumulativeTime / historyState->cumulativePulseCount
                                    : 0;
            for (uint32_t i = 0; i < HISTORY_BIN_NUM - 1; i++)
                historyState->bins[i] = historyState->bins[i + 1];
            historyState->bins[HISTORY_BIN_NUM - 1] = getHistoryValue(averageRate);

            historyState->timeInterval = 0;

            historyState->cumulativeTime = 0;
            historyState->cumulativePulseCount = 0;
        }
    }

    // Alerts
    if (getAlertLevel() == ALERTLEVEL_NONE)
    {
        measurements.heartbeat.alertBlink = false;
#if VOICE
        clearVoiceAlert();
#endif
    }
    else
    {
        measurements.heartbeat.alertBlink = !measurements.heartbeat.alertBlink;

        if (isAlertUndismissed())
            triggerAlert();
    }

    // Sound control
#if defined(SOUND_EN)
    updateSound(true);
#endif

    // LED
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    updateLED(true);
#endif

    // Set view
    if (!isDisplayEnabled() && isAlertUndismissed())
    {
        if (isInstantaneousRateAlert())
            setView(&instantaneousRateView);
        else if (isCumulativeDoseAlert())
            setView(&cumulativeDoseView);
    }

    // Update view
    const char *instantaneousAlertString = NULL;
    const char *averageAlertString = NULL;
    const char *cumulativeDoseAlertString = NULL;

    if (measurements.heartbeat.faultAlarm)
    {
        instantaneousAlertString = getString(STRING_ALERT_FAULT);
        averageAlertString = getString(STRING_ALERT_FAULT);
        cumulativeDoseAlertString = getString(STRING_ALERT_FAULT);
    }
    else
    {
        if (measurements.instantaneous.alarm)
            instantaneousAlertString = getString(STRING_ALERT_ALARM);
        else if (measurements.instantaneous.warning)
            instantaneousAlertString = getString(STRING_ALERT_WARNING);

        if (measurements.average.pulseCount == UINT32_MAX)
            averageAlertString = getString(STRING_ALERT_MAX);
        else if (measurements.average.timerExpired)
            averageAlertString = getString(STRING_ALERT_DONE);

        if (measurements.cumulativeDose.alarm)
            cumulativeDoseAlertString = getString(STRING_ALERT_ALARM);
        else if (measurements.cumulativeDose.warning)
            cumulativeDoseAlertString = getString(STRING_ALERT_WARNING);
        else if (measurements.cumulativeDose.dose.pulseCount == UINT32_MAX)
            cumulativeDoseAlertString = getString(STRING_ALERT_MAX);
    }

    if (instantaneousAlertString != measurements.instantaneousAlertString)
    {
        if (instantaneousAlertString)
            measurements.instantaneousTabIndex = INSTANTANEOUS_TAB_ALERT;
        else if (measurements.instantaneousTabIndex == INSTANTANEOUS_TAB_ALERT)
            measurements.instantaneousTabIndex = INSTANTANEOUS_TAB_BAR;
        measurements.instantaneousAlertString = instantaneousAlertString;
    }
    if (averageAlertString != measurements.averageAlertString)
    {
        if (averageAlertString)
            measurements.averageTabIndex = AVERAGE_TAB_ALERT;
        else if (measurements.averageTabIndex == AVERAGE_TAB_ALERT)
            measurements.averageTabIndex = AVERAGE_TAB_TIME;
        measurements.averageAlertString = averageAlertString;
    }
    if (cumulativeDoseAlertString != measurements.cumulativeDoseAlertString)
    {
        if (cumulativeDoseAlertString)
            measurements.cumulativeDoseTabIndex = CUMULATIVEDOSE_TAB_ALERT;
        else if (measurements.cumulativeDoseTabIndex == CUMULATIVEDOSE_TAB_ALERT)
            measurements.cumulativeDoseTabIndex = CUMULATIVEDOSE_TAB_TIME;
        measurements.cumulativeDoseAlertString = cumulativeDoseAlertString;
    }
}

// Measurement views common

static void buildValueString(char *valueString,
                             char *unitString,
                             float value,
                             const Unit *unit,
                             int32_t minMetricPrefixIndex)
{
    if (!unit->name[0] &&
        (value < 10000))
    {
        uint32_t intValue = (uint32_t)value;

        strcatUInt32(valueString, intValue, 0);
        strcpy(unitString, " ");
        if (intValue == 1)
            strcat(unitString, getString(STRING_COUNT));
        else
            strcat(unitString, getString(STRING_COUNTS));

        return;
    }

    strcatFloatAsMetricValueAndPrefix(valueString,
                                      unitString,
                                      value * unit->scale,
                                      minMetricPrefixIndex);

    strcat(unitString, unit->name[0]
                           ? unit->name
                           : getString(STRING_COUNTS));
}

void setMeasurementView(int32_t index)
{
    if (index >= 0)
        measurements.viewIndex = index;

    setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);

    setView(measurementViews[measurements.viewIndex]);
}

static bool onMeasurementEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_SELECT:
        if (!isInLockMode())
        {
            measurements.isPulseSoundIconEnabled = false;
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

    case EVENT_KEY_RESET:
        if (isAlertUndismissed())
            dismissAlert();
        else
            return isInLockMode();

        break;

    case EVENT_KEY_TOGGLEPULSESOUND:
        measurements.isPulseSoundIconEnabled = true;
        togglePulseSound();
        triggerVibration();

        break;

    default:
        return false;
    }

    return true;
}

// Instantaneous rate

static void resetInstantaneousRate(void)
{
    memset(&measurements.instantaneous,
           0,
           sizeof(measurements.instantaneous));

    updateMeasurementUnits();

    if (measurements.instantaneousTabIndex == INSTANTANEOUS_TAB_ALERT)
        measurements.instantaneousTabIndex = INSTANTANEOUS_TAB_BAR;
}

float getInstantaneousRate(void)
{
    return measurements.instantaneous.rate.value;
}

static void onInstantaneousRateViewEvent(Event event)
{
    if (onMeasurementEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.instantaneousTabIndex++;

        if ((measurements.instantaneousAlertString)
                ? measurements.instantaneousTabIndex >= (INSTANTANEOUS_TAB_NUM + 1)
                : measurements.instantaneousTabIndex >= INSTANTANEOUS_TAB_NUM)
            measurements.instantaneousTabIndex = 0;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetInstantaneousRate();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         measurements.instantaneous.rate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.instantaneous.alarm)
            style = MEASUREMENTSTYLE_ALARM;
        else if (measurements.instantaneous.warning)
            style = MEASUREMENTSTYLE_WARNING;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_INSTANTANEOUS));
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.instantaneous.rate.confidence,
                             style);

        if (measurements.instantaneousTabIndex == INSTANTANEOUS_TAB_ALERT)
            drawMeasurementAlert(measurements.instantaneousAlertString);
        else if (measurements.instantaneousTabIndex != INSTANTANEOUS_TAB_BAR)
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (measurements.instantaneousTabIndex)
            {
            case INSTANTANEOUS_TAB_TIME:
                if (measurements.instantaneous.rate.time > 0)
                    strcatTime(valueString,
                               measurements.instantaneous.rate.time);

                keyString = getString(STRING_TIME);

                break;

            case INSTANTANEOUS_TAB_MAX:
                if (measurements.instantaneous.maxValue > 0)
                    buildValueString(valueString,
                                     unitString,
                                     measurements.instantaneous.maxValue,
                                     &units[settings.units].rate,
                                     unitsMinMetricPrefixIndex[settings.units]);

                keyString = getString(STRING_MAX);

                break;

            case INSTANTANEOUS_TAB_RATE:
                if (measurements.instantaneous.rate.value > 0)
                    buildValueString(valueString,
                                     unitString,
                                     measurements.instantaneous.rate.value,
                                     &units[settings.secondaryUnits].rate,
                                     unitsMinMetricPrefixIndex[settings.secondaryUnits]);

                keyString = getString(STRING_RATE);

                break;

            default:
                break;
            }

            drawMeasurementInfo(keyString,
                                valueString,
                                unitString,
                                style);
        }
        else
        {
            float scale = units[settings.units].rate.scale;
            float uSvHScale = scale / units[UNITS_SIEVERTS].rate.scale;
            drawMeasurementBar(measurements.instantaneous.rate.value * scale,
                               unitsMeasurementBarMinExponent[settings.units],
                               rateAlertsSvH[settings.rateWarning] * uSvHScale,
                               rateAlertsSvH[settings.rateAlarm] * uSvHScale);
        }

        break;
    }

    default:
        break;
    }
}

static View instantaneousRateView = {
    onInstantaneousRateViewEvent,
    NULL,
};

// Average rate

static void resetAverageRate(void)
{
    memset(&measurements.average,
           0,
           sizeof(measurements.average));

    if (measurements.averageTabIndex == AVERAGE_TAB_ALERT)
        measurements.averageTabIndex = AVERAGE_TAB_TIME;
}

float getAverageRate(void)
{
    return measurements.average.timerRate.value;
}

static void onAverageRateViewEvent(Event event)
{
    if (onMeasurementEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.averageTabIndex++;

        if ((measurements.averageAlertString)
                ? measurements.averageTabIndex >= (AVERAGE_TAB_NUM + 1)
                : measurements.averageTabIndex >= AVERAGE_TAB_NUM)
            measurements.averageTabIndex = 0;

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
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         measurements.average.timerRate.value,
                         &units[settings.units].rate,
                         unitsMinMetricPrefixIndex[settings.units]);

        if ((measurements.average.pulseCount == UINT32_MAX) ||
            measurements.average.timerExpired)
            style = MEASUREMENTSTYLE_DONE;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_AVERAGE));
        drawMeasurementValue(valueString,
                             unitString,
                             measurements.average.timerRate.confidence,
                             style);

        if (measurements.averageTabIndex == AVERAGE_TAB_ALERT)
            drawMeasurementAlert(measurements.averageAlertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (measurements.averageTabIndex)
            {
            case AVERAGE_TAB_TIME:
                strcatTime(valueString,
                           measurements.average.timerRate.time);

                keyString = getString(STRING_TIME);

                break;

            case AVERAGE_TAB_RATE:
                if (measurements.average.timerRate.value > 0)
                {
                    buildValueString(valueString,
                                     unitString,
                                     measurements.average.timerRate.value,
                                     &units[settings.secondaryUnits].rate,
                                     unitsMinMetricPrefixIndex[settings.secondaryUnits]);
                }

                keyString = getString(STRING_RATE);

                break;

            case AVERAGE_TAB_DOSE:
                if (measurements.average.timerPulseCount > 0)
                    buildValueString(valueString,
                                     unitString,
                                     measurements.average.timerPulseCount,
                                     &units[UNITS_CPM].dose,
                                     unitsMinMetricPrefixIndex[UNITS_CPM]);

                keyString = getString(STRING_DOSE);

                break;

            default:
                break;
            }

            drawMeasurementInfo(keyString,
                                valueString,
                                unitString,
                                style);
        }

        break;
    }

    default:
        break;
    }
}

static View averageRateView = {
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

    if (measurements.cumulativeDoseTabIndex == CUMULATIVEDOSE_TAB_ALERT)
        measurements.cumulativeDoseTabIndex = CUMULATIVEDOSE_TAB_TIME;
}

static void onCumulativeDoseViewEvent(Event event)
{
    if (onMeasurementEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.cumulativeDoseTabIndex++;

        if ((measurements.cumulativeDoseAlertString)
                ? measurements.cumulativeDoseTabIndex >= (CUMULATIVEDOSE_TAB_NUM + 1)
                : measurements.cumulativeDoseTabIndex >= CUMULATIVEDOSE_TAB_NUM)
            measurements.cumulativeDoseTabIndex = 0;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetCumulativeDose();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceCumulativeDose();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString,
                         unitString,
                         measurements.cumulativeDose.dose.pulseCount,
                         &units[settings.units].dose,
                         unitsMinMetricPrefixIndex[settings.units]);

        if (measurements.cumulativeDose.alarm)
            style = MEASUREMENTSTYLE_ALARM;
        else if (measurements.cumulativeDose.warning)
            style = MEASUREMENTSTYLE_WARNING;
        else if (measurements.cumulativeDose.dose.pulseCount == UINT32_MAX)
            style = MEASUREMENTSTYLE_DONE;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_CUMULATIVE));
        drawMeasurementValue(valueString,
                             unitString,
                             0,
                             style);

        if (measurements.cumulativeDoseTabIndex == CUMULATIVEDOSE_TAB_ALERT)
            drawMeasurementAlert(measurements.cumulativeDoseAlertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (measurements.cumulativeDoseTabIndex)
            {
            case CUMULATIVEDOSE_TAB_TIME:
                strcatTime(valueString,
                           measurements.cumulativeDose.dose.time);

                keyString = getString(STRING_TIME);

                break;

            case CUMULATIVEDOSE_TAB_DOSE:
                if (measurements.cumulativeDose.dose.pulseCount > 0)
                    buildValueString(valueString,
                                     unitString,
                                     measurements.cumulativeDose.dose.pulseCount,
                                     &units[settings.secondaryUnits].dose,
                                     unitsMinMetricPrefixIndex[settings.secondaryUnits]);

                keyString = getString(STRING_DOSE);

                break;

            default:
                break;
            }

            drawMeasurementInfo(keyString,
                                valueString,
                                unitString,
                                style);
        }

        break;
    }

    default:
        break;
    }
}

static View cumulativeDoseView = {
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

typedef struct
{
    uint32_t binIndex;
    uint32_t binStart;
    uint32_t binEnd;

    uint32_t cumulativeTime;
    uint64_t cumulativePulseCountInt;
    float cumulativePulseCountFloat;
} LoadHistoryState;

static void updateLoadHistory(LoadHistoryState *state, uint8_t *bins)
{
    uint32_t cumulativeTime = state->cumulativeTime;
    uint64_t cumulativePulseCountInt = state->cumulativePulseCountInt;
    float cumulativePulseCountFloat = state->cumulativePulseCountFloat;

    while (cumulativePulseCountInt >> 32)
    {
        cumulativeTime >>= 8;
        cumulativePulseCountInt >>= 8;
        cumulativePulseCountFloat /= 256;
    }

    float rate = cumulativeTime
                     ? (cumulativePulseCountFloat + (uint32_t)cumulativePulseCountInt) /
                           cumulativeTime
                     : 0.0F;
    bins[state->binIndex] = getHistoryValue(rate);
}

void loadHistory(void)
{
    // Initialization
    uint32_t historyEnd = getDeviceTime();

    LoadHistoryState states[HISTORY_NUM];
    memset(states, 0, HISTORY_NUM * sizeof(LoadHistoryState));

    uint32_t historiesStart[HISTORY_NUM];
    for (uint32_t historyIndex = 0; historyIndex < HISTORY_NUM; historyIndex++)
    {
        uint32_t binInterval = histories[historyIndex].binInterval;
        historiesStart[historyIndex] = historyEnd - HISTORY_BIN_NUM * binInterval;
    }

    // Process records
#if defined(FAST_SYSTEM_CLOCK)
    setFastSystemClock(true);
#endif

    openDatalogRead();

    Dose prevRecord;
    Dose record;
    bool isNewLoggingSession;

    if (readDatalogRecord(&isNewLoggingSession, &prevRecord))
    {
        while (readDatalogRecord(&isNewLoggingSession, &record))
        {
            resetWatchdog();

            // printf("+ %d:%d\n", record.time, record.pulseCount);

            if (!isNewLoggingSession)
            {
                for (uint32_t historyIndex = 0; historyIndex < HISTORY_NUM; historyIndex++)
                {
                    uint32_t historyStart = historiesStart[historyIndex];
                    LoadHistoryState *state = &states[historyIndex];

                    // Overlap record interval with history interval
                    uint32_t recordStart = (prevRecord.time > historyStart) ? prevRecord.time : historyStart;
                    uint32_t recordEnd = (record.time < historyEnd) ? record.time : historyEnd;

                    // if (historyIndex == 5)
                    //     printf("- %d-%d %d-%d %d-%d\n",
                    //            prevRecord.time, record.time,
                    //            historyStart, historyEnd,
                    //            recordStart, recordEnd);

                    if (recordStart < recordEnd)
                    {
                        int32_t intervalTime = record.time - prevRecord.time;
                        uint32_t intervalPulseCount = record.pulseCount - prevRecord.pulseCount;

                        if ((record.time > state->binStart) &&
                            (record.time <= state->binEnd))
                        {
                            state->cumulativeTime += intervalTime;
                            state->cumulativePulseCountInt += intervalPulseCount;
                        }
                        else
                        {
                            uint32_t binInterval = histories[historyIndex].binInterval;
                            uint8_t *bins = measurements.history.states[historyIndex].bins;

                            uint32_t binIndexStart = (recordStart - historyStart) / binInterval;
                            uint32_t binIndexEnd = (recordEnd - 1 - historyStart) / binInterval + 1;

                            // if (historyIndex == 5)
                            //     printf("/ %d-%d\n",
                            //            binIndexStart, binIndexEnd - 1);

                            for (uint32_t binIndex = binIndexStart; binIndex < binIndexEnd; binIndex++)
                            {
                                // Overlap record interval with bin interval
                                uint32_t binStart = historyStart + binIndex * binInterval;
                                uint32_t binEnd = binStart + binInterval;

                                uint32_t overlapStart = (binStart > recordStart) ? binStart : recordStart;
                                uint32_t overlapEnd = (binEnd < recordEnd) ? binEnd : recordEnd;

                                uint32_t overlapTime = overlapEnd - overlapStart;
                                float overlapPulseCount = (float)intervalPulseCount * overlapTime / intervalTime;

                                if (binIndex != state->binIndex)
                                {
                                    // if (historyIndex == 5)
                                    // {
                                    //     printf("  %d (%d): %d %f\n",
                                    //            state->binIndex, binIndex,
                                    //            state->cumulativeTime, state->cumulativePulseCountFloat + (float)state->cumulativePulseCountInt);
                                    // }

                                    updateLoadHistory(state, bins);

                                    state->binIndex = binIndex;
                                    state->binStart = binStart;
                                    state->binEnd = binEnd;

                                    state->cumulativeTime = 0;
                                    state->cumulativePulseCountInt = 0;
                                    state->cumulativePulseCountFloat = 0;
                                }

                                state->cumulativeTime += overlapTime;
                                state->cumulativePulseCountFloat += overlapPulseCount;
                            }
                        }
                    }
                }
            }

            prevRecord = record;
        }

        for (uint32_t historyIndex = 0; historyIndex < HISTORY_NUM; historyIndex++)
        {
            LoadHistoryState *state = &states[historyIndex];
            uint8_t *bins = measurements.history.states[historyIndex].bins;

            updateLoadHistory(state, bins);
        }
    }

    closeDatalogRead();

#if defined(FAST_SYSTEM_CLOCK)
    setFastSystemClock(false);
#endif
}

void resetHistory(void)
{
    memset(&measurements.history.states,
           0,
           sizeof(measurements.history.states));
}

static void onHistoryViewEvent(Event event)
{
    if (onMeasurementEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        measurements.history.tabIndex++;

        if (measurements.history.tabIndex >= HISTORY_NUM)
            measurements.history.tabIndex = 0;

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        const HistoryState *historyState = &measurements.history.states[measurements.history.tabIndex];
        const Unit *rateUnit = &units[settings.units].rate;

        drawTitleBar(getString(STRING_HISTORY));
        drawHistory(rateUnit->scale,
                    rateUnit->name,
                    histories[measurements.history.tabIndex].xTickNum,
                    histories[measurements.history.tabIndex].name,
                    historyState->bins,
                    getHistoryValue(rateAlertsSvH[settings.rateWarning] /
                                    units[UNITS_SIEVERTS].rate.scale),
                    getHistoryValue(rateAlertsSvH[settings.rateAlarm] /
                                    units[UNITS_SIEVERTS].rate.scale));

        break;
    }

    default:
        break;
    }
}

static View historyView = {
    onHistoryViewEvent,
    NULL,
};

// Rate alerts

char *buildRateAlertMenuOption(uint32_t index)
{
    if (index == 0)
        return getString(STRING_OFF);
    else
    {
        const Unit *unit = &units[settings.units].rate;
        char unitString[16];
        float value = rateAlertsSvH[index] /
                      units[UNITS_SIEVERTS].rate.scale;

        strclr(menuOption);
        strcpy(unitString, " ");

        buildValueString(menuOption,
                         unitString,
                         value,
                         unit,
                         unitsMinMetricPrefixIndex[settings.units]);
        strcat(menuOption, unitString);

        return menuOption;
    }
}

static const char *onRateAlertMenuGetOption(uint32_t index,
                                            MenuStyle *menuStyle)
{
    if (getView() == &rateAlarmMenuView)
        *menuStyle = (index == settings.rateAlarm);
    else
        *menuStyle = (index == settings.rateWarning);

    if (index < RATE_NUM)
        return buildRateAlertMenuOption(index);
    else
        return NULL;
}

static void onRateAlertMenuSelect(uint32_t index)
{
    if (getView() == &rateAlarmMenuView)
        settings.rateAlarm = index;
    else
        settings.rateWarning = index;
}

// Dose alerts

char *buildDoseAlertMenuOption(uint32_t index)
{
    if (index == 0)
        return getString(STRING_OFF);

    const Unit *unit = &units[settings.units].dose;
    char unitString[16];
    float value = doseAlertsSv[index] / units[UNITS_SIEVERTS].dose.scale;

    strclr(menuOption);
    strcpy(unitString, " ");

    buildValueString(menuOption,
                     unitString,
                     value,
                     unit,
                     unitsMinMetricPrefixIndex[settings.units]);
    strcat(menuOption, unitString);

    return menuOption;
}

static const char *onDoseAlertMenuGetOption(uint32_t index,
                                            MenuStyle *menuStyle)
{
    if (getView() == &doseAlarmMenuView)
        *menuStyle = (index == settings.doseAlarm);
    else
        *menuStyle = (index == settings.doseWarning);

    if (index < DOSE_NUM)
        return buildDoseAlertMenuOption(index);
    else
        return NULL;
}

static void onDoseAlertMenuSelect(uint32_t index)
{
    if (getView() == &doseAlarmMenuView)
        settings.doseAlarm = index;
    else
        settings.doseWarning = index;
}

// Rate warning menu

static MenuState rateWarningMenuState;

static Menu rateWarningMenu = {
    STRING_RATE_WARNING,
    &rateWarningMenuState,
    onRateAlertMenuGetOption,
    onRateAlertMenuSelect,
    onAlertsSubMenuBack,
};

static View rateWarningMenuView = {
    onMenuEvent,
    &rateWarningMenu,
};

// Rate alarm menu

static MenuState rateAlarmMenuState;

static Menu rateAlarmMenu = {
    STRING_RATE_ALARM,
    &rateAlarmMenuState,
    onRateAlertMenuGetOption,
    onRateAlertMenuSelect,
    onAlertsSubMenuBack,
};

static View rateAlarmMenuView = {
    onMenuEvent,
    &rateAlarmMenu,
};

// Dose warning menu

static MenuState doseWarningMenuState;

static Menu doseWarningMenu = {
    STRING_DOSE_WARNING,
    &doseWarningMenuState,
    onDoseAlertMenuGetOption,
    onDoseAlertMenuSelect,
    onAlertsSubMenuBack,
};

static View doseWarningMenuView = {
    onMenuEvent,
    &doseWarningMenu,
};

// Dose alarm menu

static MenuState doseAlarmMenuState;

static Menu doseAlarmMenu = {
    STRING_DOSE_ALARM,
    &doseAlarmMenuState,
    onDoseAlertMenuGetOption,
    onDoseAlertMenuSelect,
    onAlertsSubMenuBack,
};

static View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};

// Alert indication menu

enum
{
#if defined(BUZZER) || defined(SOUND_EN)
    ALERTINDICATION_MENU_SOUND,
#endif
#if defined(VOICE)
    ALERTINDICATION_MENU_VOICE,
#endif
#if defined(VIBRATION)
    ALERTINDICATION_MENU_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    ALERTINDICATION_MENU_PULSELED,
#endif
    ALERTINDICATION_MENU_DISPLAYFLASH,
};

static cstring alertIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(SOUND_EN)
    STRING_SOUND,
#endif
#if defined(VOICE)
    STRING_VOICE,
#endif
#if defined(VIBRATION)
    STRING_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    STRING_PULSE_LED,
#endif
    STRING_DISPLAY_FLASH,
    NULL,
};

static const char *onAlertIndicationMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BUZZER) || defined(SOUND_EN)
    case ALERTINDICATION_MENU_SOUND:
        *menuStyle = settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        *menuStyle = settings.alertVoice;

        break;
#endif

#if defined(VIBRATION)
    case ALERTINDICATION_MENU_VIBRATION:
        *menuStyle = settings.alertVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    case ALERTINDICATION_MENU_PULSELED:
        *menuStyle = settings.alertPulseLED;

        break;
#endif

    case ALERTINDICATION_MENU_DISPLAYFLASH:
        *menuStyle = settings.alertDisplayFlash;

        break;
    }

    return getString(alertIndicationMenuOptions[index]);
}

static void onAlertIndicationMenuSelect(uint32_t index)
{
    switch (index)
    {
#if defined(BUZZER) || defined(SOUND_EN)
    case ALERTINDICATION_MENU_SOUND:
        settings.alertSound = !settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        settings.alertVoice = !settings.alertVoice;

        break;
#endif

#if defined(VIBRATION)
    case ALERTINDICATION_MENU_VIBRATION:
        settings.alertVibration = !settings.alertVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    case ALERTINDICATION_MENU_PULSELED:
        settings.alertPulseLED = !settings.alertPulseLED;

        break;
#endif

    case ALERTINDICATION_MENU_DISPLAYFLASH:
        settings.alertDisplayFlash = !settings.alertDisplayFlash;

        break;
    }
}

static MenuState alertIndicationMenuState;

static Menu alertIndicationMenu = {
    STRING_INDICATION,
    &alertIndicationMenuState,
    onAlertIndicationMenuGetOption,
    onAlertIndicationMenuSelect,
    onAlertsSubMenuBack,
};

static View alertIndicationMenuView = {
    onMenuEvent,
    &alertIndicationMenu,
};

// Alerts menu

bool isAlertEnabled(void)
{
    return (settings.rateAlarm ||
            settings.doseAlarm ||
            settings.rateWarning ||
            settings.doseWarning);
}

bool isAlertBlink(void)
{
    return measurements.heartbeat.alertBlink;
}

AlertLevel getAlertLevel(void)
{
    if (measurements.instantaneous.alarm ||
        measurements.cumulativeDose.alarm ||
        measurements.heartbeat.faultAlarm)
        return ALERTLEVEL_ALARM;
    else if (measurements.instantaneous.warning ||
             measurements.cumulativeDose.warning)
        return ALERTLEVEL_WARNING;
    else
        return ALERTLEVEL_NONE;
}

bool isInstantaneousRateAlert(void)
{
    return (measurements.instantaneous.alarm ||
            measurements.instantaneous.warning);
}

bool isCumulativeDoseAlert(void)
{
    return (measurements.cumulativeDose.alarm ||
            measurements.cumulativeDose.warning);
}

bool isAlertUndismissed(void)
{
    return (getAlertLevel() > ALERTLEVEL_NONE) &&
           !measurements.heartbeat.alertDismissed;
}

void dismissAlert(void)
{
    measurements.heartbeat.alertDismissed = true;
}

bool isPulseSoundIconEnabled(void)
{
    return measurements.isPulseSoundIconEnabled;
}

static SubMenuOption alertsMenuOptions[] = {
    {STRING_INDICATION, &alertIndicationMenuView},
    {STRING_RATE_WARNING, &rateWarningMenuView},
    {STRING_RATE_ALARM, &rateAlarmMenuView},
    {STRING_DOSE_WARNING, &doseWarningMenuView},
    {STRING_DOSE_ALARM, &doseAlarmMenuView},
    {NULL},
};

static const char *onAlertsMenuGetOption(uint32_t index,
                                         MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(alertsMenuOptions[index].title);
}

static void onAlertsMenuSelect(uint32_t index)
{
    setView(alertsMenuOptions[index].view);
}

static MenuState alertsMenuState;

static Menu alertsMenu = {
    STRING_ALERTS,
    &alertsMenuState,
    onAlertsMenuGetOption,
    onAlertsMenuSelect,
    onSettingsSubMenuBack,
};

View alertsMenuView = {
    onMenuEvent,
    &alertsMenu,
};

static void onAlertsSubMenuBack(void)
{
    setView(&alertsMenuView);
}

// Units menu

static cstring unitsMenuOptions[] = {
    STRING_SIEVERT,
    STRING_REM,
    STRING_CPM,
    STRING_CPS,
    NULL,
};

static const char *onUnitsMenuGetOption(uint32_t index,
                                        MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.units);

    return getString(unitsMenuOptions[index]);
}

static void onUnitsMenuSelect(uint32_t index)
{
    settings.units = index;
}

static MenuState unitsMenuState;

static Menu unitsMenu = {
    STRING_UNITS,
    &unitsMenuState,
    onUnitsMenuGetOption,
    onUnitsMenuSelect,
    onMeasurementsSubMenuBack,
};

static View unitsMenuView = {
    onMenuEvent,
    &unitsMenu,
};

// Secondary units menu

static const char *onSecondaryUnitsMenuGetOption(uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.secondaryUnits);

    return getString(unitsMenuOptions[index]);
}

static void onSecondaryUnitsMenuSelect(uint32_t index)
{
    settings.secondaryUnits = index;
}

static MenuState secondaryUnitsMenuState;

static Menu secondaryUnitsMenu = {
    STRING_SECONDARY_UNITS,
    &secondaryUnitsMenuState,
    onSecondaryUnitsMenuGetOption,
    onSecondaryUnitsMenuSelect,
    onMeasurementsSubMenuBack,
};

static View secondaryUnitsMenuView = {
    onMenuEvent,
    &secondaryUnitsMenu,
};

// Instantaneous menu

static cstring instantaneousMenuOptions[] = {
    STRING_ADAPTIVE_FAST,
    STRING_ADAPTIVE_PRECISION,
    STRING_1_MINUTE,
    STRING_30_SECONDS,
    STRING_10_SECONDS,
    NULL,
};

static const char *onInstantaneousMenuGetOption(uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.instantaneousAveraging);

    return getString(instantaneousMenuOptions[index]);
}

static void onInstantaneousMenuSelect(uint32_t index)
{
    settings.instantaneousAveraging = index;
}

static MenuState instantaneousMenuState;

static Menu instantaneousMenu = {
    STRING_INSTANTANEOUS,
    &instantaneousMenuState,
    onInstantaneousMenuGetOption,
    onInstantaneousMenuSelect,
    onMeasurementsSubMenuBack,
};

static View instantaneousMenuView = {
    onMenuEvent,
    &instantaneousMenu,
};

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
    NULL,
};

static const char *onAverageMenuGetOption(uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.averaging);

    return getString(averageMenuOptions[index]);
}

static void onAverageMenuSelect(uint32_t index)
{
    settings.averaging = index;
}

static MenuState averageMenuState;

static Menu averageMenu = {
    STRING_AVERAGE,
    &averageMenuState,
    onAverageMenuGetOption,
    onAverageMenuSelect,
    onMeasurementsSubMenuBack,
};

static View averageMenuView = {
    onMenuEvent,
    &averageMenu,
};

// Measurements menu

static SubMenuOption measurementMenuOptions[] = {
    {STRING_UNITS, &unitsMenuView},
    {STRING_SECONDARY_UNITS, &secondaryUnitsMenuView},
    {STRING_INSTANTANEOUS, &instantaneousMenuView},
    {STRING_AVERAGE, &averageMenuView},
    {NULL},
};

static const char *onMeasurementsMenuGetOption(uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(measurementMenuOptions[index].title);
}

static void onMeasurementsMenuSelect(uint32_t index)
{
    setView(measurementMenuOptions[index].view);
}

static MenuState measurementsMenuState;

static Menu measurementsMenu = {
    STRING_MEASUREMENTS,
    &measurementsMenuState,
    onMeasurementsMenuGetOption,
    onMeasurementsMenuSelect,
    onSettingsSubMenuBack,
};

View measurementsMenuView = {
    onMenuEvent,
    &measurementsMenu,
};

static void onMeasurementsSubMenuBack(void)
{
    setView(&measurementsMenuView);
}
