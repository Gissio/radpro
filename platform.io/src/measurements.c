/*
 * Rad Pro
 * Instantaneous, rate and dose measurement
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <string.h>

#include "cmath.h"
#include "display.h"
#include "events.h"
#include "format.h"
#include "measurements.h"
#include "settings.h"
#include "ui.h"

#define INTERVAL_STATS_NUM 5
#define PULSE_QUEUE_LENGTH (10 + 1)

#define PULSE_SOUND_QUIET_TICKS ((uint32_t)(0.001F * SYS_TICK_FREQUENCY))
#define PULSE_SOUND_LOUD_TICKS ((uint32_t)(0.015F * SYS_TICK_FREQUENCY))
#define PULSE_BACKLIGHT_FLASH_TICKS ((uint32_t)(0.08F * SYS_TICK_FREQUENCY))

#define OVERLOAD_RATE 1000

#define AVERAGE_TIME_MAX (UINT32_MAX / SYS_TICK_FREQUENCY)

#define ALARM_TICKS ((uint32_t)(0.25F * SYS_TICK_FREQUENCY))

UnitType units[] = {
    {{"Sv/h", (60 * 1E-6F), -6},
     {"Sv", (60 * 1E-6F / 3600), -6}},
    {{"rem/h", (60 * 1E-4F), -6},
     {"rem", (60 * 1E-4F / 3600), -6}},
    {{"cpm", 60, 0},
     {"counts", 1, 0}},
    {{"cps", 1, 0},
     {"counts", 1, 0}},
};

const float rateAlarmsSvH[] = {
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

const float doseAlarmsSv[] = {
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

typedef struct
{
    uint32_t firstPulseTick;
    uint32_t pulseCount;
} IntervalStats;

typedef struct
{
    uint32_t count;
    uint32_t index;
    uint32_t tick[PULSE_QUEUE_LENGTH];
} PulseQueue;

struct
{
    uint32_t tick;
    IntervalStats intervalStats[INTERVAL_STATS_NUM];
    PulseQueue pulseQueue;

    uint32_t snapshotTime;
    uint32_t snapshotPulseCount;
    uint32_t snapshotTicks;
    float snapshotValue;
    float snapshotMaxValue;

    bool isHold;
    uint32_t holdTime;
    uint32_t holdPulseCount;
    float holdValue;
} instantaneousRate;

struct
{
    uint32_t tick;
    uint32_t firstPulseTick;
    uint32_t lastPulseTick;
    uint32_t pulseCount;

    uint32_t snapshotTime;
    uint32_t snapshotPulseCount;
    uint32_t snapshotTicks;
    float snapshotValue;

    bool isHold;
    uint32_t holdTime;
    uint32_t holdPulseCount;
    float holdValue;
} averageRate;

struct
{
    uint32_t pulseCount;

    uint32_t snapshotTime;
    uint32_t snapshotPulseCount;

    bool isHold;
    uint32_t holdTime;
    uint32_t holdPulseCount;
} dose;

typedef const struct
{
    char *const name;
    uint32_t samplesPerDataPoint;
} History;

typedef struct
{
    float sampleSum;
    uint32_t sampleNum;

    uint8_t bufferIndex;
    uint8_t buffer[HISTORY_BUFFER_SIZE];
} HistoryState;

History histories[HISTORY_NUM] = {
    {"History (2m)", 1},
    {"History (10m)", 5},
    {"History (1h)", 30},
    {"History (6h)", 180},
    {"History (24h)", 720},
};
HistoryState historyStates[HISTORY_NUM];
uint32_t historySampleIndex;

static void resetInstantaneousRate(void);
static bool isInstantaneousRateAlarm(void);

static void resetAverageRate(void);

static void resetDose(void);
static bool isDoseAlarm(void);

static void resetHistory(void);

// General

void initMeasurement(void)
{
    resetInstantaneousRate();
    resetAverageRate();
    resetDose();
    resetHistory();
}

void updateUnits(void)
{
    float cpmPerUSvH;
    switch (settings.tubeType)
    {
    case TUBE_HH614:
        cpmPerUSvH = 68.4F;
        break;

    case TUBE_M4011:
        cpmPerUSvH = 153.F;
        break;

    case TUBE_SBM20:
        cpmPerUSvH = 174.F;
        break;

    case TUBE_SI3BG:
        cpmPerUSvH = 1.61F;
        break;

    default:
        cpmPerUSvH = 1;
        break;
    }

    units[0].rate.scale = (60 * 1E-6F) / cpmPerUSvH;
    units[0].dose.scale = (60 * 1E-6F / 3600) / cpmPerUSvH;
    units[1].rate.scale = (60 * 1E-4F) / cpmPerUSvH;
    units[1].dose.scale = (60 * 1E-4F / 3600) / cpmPerUSvH;
}

static void resetIntervalStats(IntervalStats *intervalStats)
{
    intervalStats->firstPulseTick = 0;
    intervalStats->pulseCount = 0;
}

void onMeasurementTick(uint32_t pulseCount)
{
    // Pulse?
    if (pulseCount)
    {
        // Life statistics
        addClamped(&state.lifePulseCount, pulseCount);

        // Instantaneous rate
        if (!instantaneousRate.intervalStats[0].pulseCount)
            instantaneousRate.intervalStats[0].firstPulseTick = instantaneousRate.tick;
        instantaneousRate.intervalStats[0].pulseCount += pulseCount;

        instantaneousRate.pulseQueue.count += pulseCount;
        if (instantaneousRate.pulseQueue.count > PULSE_QUEUE_LENGTH)
            instantaneousRate.pulseQueue.count = PULSE_QUEUE_LENGTH;
        for (uint32_t i = 0; i < pulseCount; i++)
        {
            instantaneousRate.pulseQueue.tick[instantaneousRate.pulseQueue.index] = instantaneousRate.tick;
            instantaneousRate.pulseQueue.index = (instantaneousRate.pulseQueue.index + 1) % PULSE_QUEUE_LENGTH;
        }

        // Average rate
        if (averageRate.tick < AVERAGE_TIME_MAX)
        {
            if (!averageRate.pulseCount)
                averageRate.firstPulseTick = averageRate.tick;
            averageRate.lastPulseTick = averageRate.tick;
            addClamped(&averageRate.pulseCount, pulseCount);
        }

        // Dose
        addClamped(&dose.pulseCount, pulseCount);

        // Buzzer
        switch (settings.pulseSound)
        {
        case PULSE_SOUND_QUIET:
            setBuzzerTimer(PULSE_SOUND_QUIET_TICKS);
            break;

        case PULSE_SOUND_LOUD:
            setBuzzerTimer(PULSE_SOUND_LOUD_TICKS);
            break;
        }

        // Display
        if (settings.backlight == BACKLIGHT_PULSE_FLASHES)
            setBacklightTimer(PULSE_BACKLIGHT_FLASH_TICKS);
    }

    // Update ticks
    instantaneousRate.tick++;

    if (averageRate.tick < AVERAGE_TIME_MAX)
        averageRate.tick++;
}

void onMeasurementOneSecond(void)
{
    uint32_t firstPulseTick;
    uint32_t lastPulseTick;
    uint32_t pulseCount;
    uint32_t ticks;

    if (state.lifeTime < UINT32_MAX)
        state.lifeTime++;

    // Instantaneous rate
    firstPulseTick = 0;
    lastPulseTick = instantaneousRate.pulseQueue.tick[(PULSE_QUEUE_LENGTH + instantaneousRate.pulseQueue.index - 1) % PULSE_QUEUE_LENGTH];
    pulseCount = 0;

    for (uint32_t i = 0; i < INTERVAL_STATS_NUM; i++)
    {
        if (instantaneousRate.intervalStats[i].pulseCount)
        {
            firstPulseTick = instantaneousRate.intervalStats[i].firstPulseTick;
            pulseCount += instantaneousRate.intervalStats[i].pulseCount;
        }
    }

    for (uint32_t i = INTERVAL_STATS_NUM - 1; i > 0; i--)
        instantaneousRate.intervalStats[i] = instantaneousRate.intervalStats[i - 1];
    resetIntervalStats(&instantaneousRate.intervalStats[0]);

    if (pulseCount < PULSE_QUEUE_LENGTH)
    {
        firstPulseTick = instantaneousRate.pulseQueue.tick[(PULSE_QUEUE_LENGTH + instantaneousRate.pulseQueue.index - instantaneousRate.pulseQueue.count) % PULSE_QUEUE_LENGTH];
        pulseCount = instantaneousRate.pulseQueue.count;
    }

    if (instantaneousRate.pulseQueue.count < PULSE_QUEUE_LENGTH)
        instantaneousRate.snapshotTime++;
    else
        instantaneousRate.snapshotTime = (instantaneousRate.tick + (SYS_TICK_FREQUENCY - 1) - firstPulseTick) / SYS_TICK_FREQUENCY;

    ticks = lastPulseTick - firstPulseTick;
    if (ticks && (pulseCount > 1))
    {
        instantaneousRate.snapshotPulseCount = pulseCount - 1;
        instantaneousRate.snapshotTicks = ticks;
    }
    else
    {
        instantaneousRate.snapshotPulseCount = 0;
        instantaneousRate.snapshotTicks = 1;
    }

    // Average rate
    pulseCount = averageRate.pulseCount;
    ticks = averageRate.lastPulseTick - averageRate.firstPulseTick;

    if ((averageRate.tick < AVERAGE_TIME_MAX) && (averageRate.pulseCount < UINT32_MAX))
        averageRate.snapshotTime++;

    if (ticks && (pulseCount > 1))
    {
        averageRate.snapshotPulseCount = pulseCount - 1;
        averageRate.snapshotTicks = ticks;
    }
    else
    {
        averageRate.snapshotPulseCount = 0;
        averageRate.snapshotTicks = 1;
    }

    // Dose
    if ((dose.snapshotTime < UINT32_MAX) && (dose.snapshotPulseCount < UINT32_MAX))
    {
        dose.snapshotTime++;
        dose.snapshotPulseCount = dose.pulseCount;
    }

    if (isInstantaneousRateAlarm() || isDoseAlarm())
    {
        setBuzzerTimer(ALARM_TICKS);
        setBacklightTimer(ALARM_TICKS);
    }
}

void updateMeasurement(void)
{
    // Instantaneous rate
    instantaneousRate.snapshotValue = (float)instantaneousRate.snapshotPulseCount *
                                      SYS_TICK_FREQUENCY / instantaneousRate.snapshotTicks;

    if ((instantaneousRate.pulseQueue.count == PULSE_QUEUE_LENGTH) &&
        (instantaneousRate.snapshotValue > instantaneousRate.snapshotMaxValue))
        instantaneousRate.snapshotMaxValue = instantaneousRate.snapshotValue;

    // Average rate
    averageRate.snapshotValue = (float)averageRate.snapshotPulseCount *
                                SYS_TICK_FREQUENCY / averageRate.snapshotTicks;

    // History
    historySampleIndex =
        (historySampleIndex + 1) % histories[HISTORY_LAST].samplesPerDataPoint;
    for (uint32_t i = 0; i < HISTORY_NUM; i++)
    {
        History *history = &histories[i];
        HistoryState *historyState = &historyStates[i];

        historyState->sampleSum += instantaneousRate.snapshotValue;
        historyState->sampleNum++;

        if ((historySampleIndex % history->samplesPerDataPoint) == 0)
        {
            float average = historyState->sampleSum / historyState->sampleNum;
            int32_t value = (int32_t)(HISTORY_VALUE_DECADE * log10fApprox(average / HISTORY_CPS_MIN));
            value = (value < 0) ? 0 : (value > UCHAR_MAX) ? UCHAR_MAX
                                                          : value;
            historyState->buffer[historyState->bufferIndex] = (uint8_t)value;

            historyState->sampleSum = 0;
            historyState->sampleNum = 0;

            historyState->bufferIndex = (historyState->bufferIndex + 1) % HISTORY_BUFFER_SIZE;
        }
    }
}

// UI

static void drawTitleWithTime(const char *title, uint32_t time)
{
    char titleString[32];
    strcpy(titleString, title);
    strcat(titleString, " (");
    strcatTime(titleString, time);
    strcat(titleString, ")");

    drawTitle(titleString);
}

static void drawRate(float rate, uint32_t rateCount)
{
    char mantissa[32];
    char characteristic[32];
    formatRate(rate, mantissa, characteristic);

    drawMeasurementValue(mantissa, characteristic);

    if (rate > 0)
        drawConfidenceIntervals(rateCount);
}

static void drawDose(uint32_t value)
{
    char mantissa[32];
    char characteristic[32];
    formatDose(value, mantissa, characteristic);

    drawMeasurementValue(mantissa, characteristic);
}

static void drawRateMax(float value)
{
    char mantissa[16];
    char characteristic[16];
    formatRate(value, mantissa, characteristic);

    char subtitleString[48];
    strcpy(subtitleString, "Max: ");
    strcat(subtitleString, mantissa);
    strcat(subtitleString, " ");
    strcat(subtitleString, characteristic);

    drawSubtitle(subtitleString);
}

void onMeasurementViewKey(KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_UP:
        if (getView() == VIEW_INSTANTANEOUS_RATE)
            setView(VIEW_HISTORY);
        else
            setView(getView() - 1);
        break;

    case KEY_DOWN:
        if (getView() == VIEW_HISTORY)
            setView(VIEW_INSTANTANEOUS_RATE);
        else
            setView(getView() + 1);
        break;

    case KEY_SELECT:
        openSettingsMenu();
        break;

    case KEY_BACK_DELAYED:
        switch (getView())
        {
        case VIEW_INSTANTANEOUS_RATE:
            instantaneousRate.isHold = !instantaneousRate.isHold;
            if (instantaneousRate.isHold)
            {
                instantaneousRate.holdTime = instantaneousRate.snapshotTime;
                instantaneousRate.holdPulseCount = instantaneousRate.snapshotPulseCount;
                instantaneousRate.holdValue = instantaneousRate.snapshotValue;
            }
            break;

        case VIEW_AVERAGE_RATE:
            averageRate.isHold = !averageRate.isHold;
            if (averageRate.isHold)
            {
                averageRate.holdTime = averageRate.snapshotTime;
                averageRate.holdPulseCount = averageRate.snapshotPulseCount;
                averageRate.holdValue = averageRate.snapshotValue;
            }
            break;

        case VIEW_DOSE:
            dose.isHold = !dose.isHold;
            if (dose.isHold)
            {
                dose.holdTime = dose.snapshotTime;
                dose.holdPulseCount = dose.snapshotPulseCount;
            }
            break;
        }

        updateView();
        break;

    case KEY_RESET:
        onMeasurementViewKey(KEY_BACK);

        switch (getView())
        {
        case VIEW_INSTANTANEOUS_RATE:
            resetInstantaneousRate();
            break;

        case VIEW_AVERAGE_RATE:
            resetAverageRate();
            break;

        case VIEW_DOSE:
            resetDose();
            break;

        case VIEW_HISTORY:
            resetHistory();
            break;
        }

        updateView();
        break;
    }
}

// Instantaneous rate

static void resetInstantaneousRate(void)
{
    instantaneousRate.tick = 0;
    for (uint32_t i = 0; i < INTERVAL_STATS_NUM; i++)
        resetIntervalStats(&instantaneousRate.intervalStats[i]);

    instantaneousRate.pulseQueue.count = 0;
    instantaneousRate.pulseQueue.index = 0;
    for (uint32_t i = 0; i < PULSE_QUEUE_LENGTH; i++)
        instantaneousRate.pulseQueue.tick[i] = 0;

    instantaneousRate.snapshotTime = 0;
    instantaneousRate.snapshotPulseCount = 0;
    instantaneousRate.snapshotValue = 0;
    instantaneousRate.snapshotMaxValue = 0;

    instantaneousRate.isHold = false;
    instantaneousRate.holdPulseCount = 0;
    instantaneousRate.holdValue = 0;
    instantaneousRate.holdTime = 0;
}

static bool isInstantaneousRateAlarm(void)
{
    if (!settings.rateAlarm)
        return false;

    float rateSvH = units[UNITS_SIEVERTS].rate.scale * instantaneousRate.snapshotValue;
    return rateSvH >= rateAlarmsSvH[settings.rateAlarm];
}

void drawInstantaneousRateView(void)
{
    uint32_t time;
    uint32_t pulseCount;
    float value;

    if (!instantaneousRate.isHold)
    {
        time = instantaneousRate.snapshotTime;
        pulseCount = instantaneousRate.snapshotPulseCount;
        value = instantaneousRate.snapshotValue;
    }
    else
    {
        time = instantaneousRate.holdTime;
        pulseCount = instantaneousRate.holdPulseCount;
        value = instantaneousRate.holdValue;
    }

    drawTitleWithTime("Instantaneous", time);
    drawRate(value, pulseCount);

    if (instantaneousRate.isHold)
        drawSubtitle("HOLD");
    else if (instantaneousRate.snapshotValue >= OVERLOAD_RATE)
        drawSubtitle("OVERLOAD");
    else if (isInstantaneousRateAlarm())
        drawSubtitle("RATE ALARM");
    else if (instantaneousRate.snapshotMaxValue > 0)
        drawRateMax(instantaneousRate.snapshotMaxValue);
}

// Average rate

void resetAverageRate(void)
{
    averageRate.tick = 0;
    averageRate.firstPulseTick = 0;
    averageRate.lastPulseTick = 0;
    averageRate.pulseCount = 0;

    averageRate.snapshotTime = 0;
    averageRate.snapshotPulseCount = 0;
    averageRate.snapshotValue = 0;
}

void drawAverageRateView(void)
{
    uint32_t time;
    uint32_t pulseCount;
    float value;

    if (!averageRate.isHold)
    {
        time = averageRate.snapshotTime;
        pulseCount = averageRate.snapshotPulseCount;
        value = averageRate.snapshotValue;
    }
    else
    {
        time = averageRate.holdTime;
        pulseCount = averageRate.holdPulseCount;
        value = averageRate.holdValue;
    }

    drawTitleWithTime("Average", time);
    drawRate(value, pulseCount);

    if (averageRate.isHold)
        drawSubtitle("HOLD");
    else if ((averageRate.snapshotTime >= AVERAGE_TIME_MAX) ||
             (averageRate.snapshotPulseCount >= UINT32_MAX))
        drawSubtitle("OVERFLOW");
    else if (averageRate.snapshotValue >= OVERLOAD_RATE)
        drawSubtitle("OVERLOAD");
}

// Dose

void setDose(uint32_t time, uint32_t pulseCount)
{
    syncThreads();

    dose.pulseCount = pulseCount;

    dose.snapshotTime = time;
    dose.snapshotPulseCount = pulseCount;
}

void getDose(uint32_t *time, uint32_t *pulseCount)
{
    syncThreads();

    *time = dose.snapshotTime;
    *pulseCount = dose.snapshotPulseCount;
}

static void resetDose(void)
{
    setDose(0, 0);
}

static bool isDoseAlarm(void)
{
    if (!settings.doseAlarm)
        return false;

    float doseSv = units[UNITS_SIEVERTS].dose.scale * dose.snapshotPulseCount;
    return doseSv >= doseAlarmsSv[settings.doseAlarm];
}

void drawDoseView(void)
{
    uint32_t time;
    uint32_t pulseCount;

    if (!dose.isHold)
    {
        time = dose.snapshotTime;
        pulseCount = dose.snapshotPulseCount;
    }
    else
    {
        time = dose.holdTime;
        pulseCount = dose.holdPulseCount;
    }

    drawTitleWithTime("Dose", time);
    drawDose(pulseCount);

    if (dose.isHold)
        drawSubtitle("HOLD");
    else if ((dose.snapshotTime >= UINT32_MAX) ||
             dose.snapshotPulseCount >= UINT32_MAX)
        drawSubtitle("OVERFLOW");
    else if (isDoseAlarm())
        drawSubtitle("DOSE ALARM");
}

// History

static void resetHistory(void)
{
    for (uint32_t i = 0; i < HISTORY_NUM; i++)
    {
        HistoryState *historyState = &historyStates[i];

        historyState->sampleSum = 0;
        historyState->sampleNum = 0;

        historyState->bufferIndex = 0;
        for (uint32_t j = 0; j < HISTORY_BUFFER_SIZE; j++)
            historyState->buffer[j] = 0;
    }

    historySampleIndex = 0;
}

uint8_t getHistoryDataPoint(uint32_t dataIndex)
{
    HistoryState *historyState = &historyStates[settings.history];

    uint32_t bufferIndex =
        (HISTORY_BUFFER_SIZE + (historyState->bufferIndex - 1) - dataIndex) % HISTORY_BUFFER_SIZE;

    return historyState->buffer[bufferIndex];
}

static const char *getHistoryName(uint32_t historyIndex)
{
    return histories[historyIndex].name;
}

void drawHistoryView(void)
{
    // Initialization
    Unit *unit = &units[settings.units].rate;

    char labelMin[16] = "";
    char labelMax[16] = "";

    int32_t offset = 0;
    uint32_t range = 1;

    // Formatting
    uint8_t valueMin = UCHAR_MAX;
    uint8_t valueMax = 0;

    for (uint32_t i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        uint8_t value = getHistoryDataPoint(i);
        if (value > 0)
        {
            if (value < valueMin)
                valueMin = value;
            if (value > valueMax)
                valueMax = value;
        }
    }

    if (valueMax > 0)
    {
        int32_t unitScaleValue = (int32_t)(HISTORY_VALUE_DECADE * log10fApprox(unit->scale * HISTORY_CPS_MIN));

        int32_t exponentMin = divideDown(valueMin + unitScaleValue, HISTORY_VALUE_DECADE);
        int32_t exponentMax = divideDown(valueMax + unitScaleValue, HISTORY_VALUE_DECADE) + 1;

        offset = unitScaleValue - exponentMin * HISTORY_VALUE_DECADE;
        range = exponentMax - exponentMin;

        formatMultiplier(unit->name, exponentMin, labelMin);
        formatMultiplier(unit->name, exponentMax, labelMax);
    }

    drawTitle(getHistoryName(settings.history));

    drawHistory(labelMin, labelMax, offset, range);
}
