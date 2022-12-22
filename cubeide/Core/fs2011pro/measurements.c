/*
 * FS2011 Pro
 * Instantaneous, rate and dose measurement
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <math.h>
#include <stdio.h>

#include "display.h"
#include "format.h"
#include "keyboard.h"
#include "cmath.h"
#include "measurements.h"
#include "menus.h"
#include "settings.h"
#include "ui.h"

#define OVERLOAD_RATE 500
#define TIME_MAX (ULONG_MAX / TICK_FREQUENCY)

#define INSTANTANEOUS_RATE_HISTORY_STATS_NUM 5
#define INSTANTANEOUS_RATE_PULSE_NUM (10 + 1)

typedef struct
{
    unsigned int firstPulseTick;
    unsigned int pulseCount;
} PeriodStats;

struct InstantaneousRate
{
    unsigned int tick;
    unsigned int lastPulseTick;

    PeriodStats current;
    PeriodStats history[INSTANTANEOUS_RATE_HISTORY_STATS_NUM];

    unsigned int pulseTicksCount;
    unsigned int pulseTicksIndex;
    unsigned int pulseTicks[INSTANTANEOUS_RATE_PULSE_NUM];

    unsigned int snapshotTime;
    unsigned int snapshotCount;
    unsigned int snapshotTicks;
    float snapshotValue;
    float snapshotMaxValue;

    bool isHold;
    unsigned int holdTime;
    unsigned int holdCount;
    float holdValue;
} instantaneousRate;

struct AverageRate
{
    unsigned int tick;
    unsigned int lastPulseTick;
    unsigned int firstPulseTick;
    unsigned int pulseCount;

    unsigned int snapshotTime;
    unsigned int snapshotCount;
    unsigned int snapshotTicks;
    float snapshotValue;

    bool isHold;
    unsigned int holdTime;
    unsigned int holdCount;
    float holdValue;
} averageRate;

struct Dose
{
    unsigned int pulseCount;

    unsigned int snapshotTime;
    unsigned int snapshotValue;

    bool isHold;
    unsigned int holdTime;
    unsigned int holdValue;
} dose;

typedef const struct
{
    char *const name;
    unsigned int samplesPerDataPoint;
} History;

typedef struct
{
    float sampleSum;
    unsigned int sampleNum;

    unsigned char bufferIndex;
    unsigned char buffer[HISTORY_BUFFER_SIZE];
} HistoryState;

History histories[HISTORY_NUM] = {
    {"History (2m)", 1},
    {"History (10m)", 5},
    {"History (1h)", 30},
    {"History (6h)", 180},
    {"History (24h)", 720},
};
HistoryState historyStates[HISTORY_NUM];
unsigned int historySampleIndex;

// Reset

void initMeasurements()
{
    resetInstantaneousRate();
    resetAverageRate();
    resetDose();
    resetHistory();
}

void resetPeriodStats(PeriodStats *periodStats)
{
    periodStats->firstPulseTick = 0;
    periodStats->pulseCount = 0;
}

void resetInstantaneousRate()
{
    instantaneousRate.tick = 0;
    instantaneousRate.lastPulseTick = 0;
    resetPeriodStats(&instantaneousRate.current);
    for (unsigned int i = 0; i < INSTANTANEOUS_RATE_HISTORY_STATS_NUM; i++)
        resetPeriodStats(&instantaneousRate.history[i]);

    instantaneousRate.pulseTicksCount = 0;
    instantaneousRate.pulseTicksIndex = 0;
    for (unsigned int i = 0; i < INSTANTANEOUS_RATE_PULSE_NUM; i++)
        instantaneousRate.pulseTicks[i] = 0;

    instantaneousRate.snapshotTime = 0;
    instantaneousRate.snapshotCount = 0;
    instantaneousRate.snapshotValue = 0;
    instantaneousRate.snapshotMaxValue = 0;

    instantaneousRate.isHold = false;
    instantaneousRate.holdCount = 0;
    instantaneousRate.holdValue = 0;
    instantaneousRate.holdTime = 0;
}

void resetAverageRate()
{
    averageRate.tick = 0;
    averageRate.firstPulseTick = 0;
    averageRate.lastPulseTick = 0;
    averageRate.pulseCount = 0;

    averageRate.snapshotTime = 0;
    averageRate.snapshotCount = 0;
    averageRate.snapshotValue = 0;
}

void resetDose()
{
    dose.pulseCount = 0;

    dose.snapshotTime = 0;
    dose.snapshotValue = 0;
}

void resetHistory()
{
    for (unsigned int i = 0; i < HISTORY_NUM; i++)
    {
        HistoryState *historyState = &historyStates[i];

        historyState->sampleSum = 0;
        historyState->sampleNum = 0;

        historyState->bufferIndex = 0;
        for (unsigned int i = 0; i < HISTORY_BUFFER_SIZE; i++)
            historyState->buffer[i] = 0;
    }

    historySampleIndex = 0;
}

// Callbacks

void onMeasurementTick(unsigned int pulseCount)
{
    if (pulseCount)
    {
        // Instantaneous rate
        if (!instantaneousRate.current.pulseCount)
            instantaneousRate.current.firstPulseTick = instantaneousRate.tick;
        addClamped(&instantaneousRate.current.pulseCount, pulseCount);

        instantaneousRate.pulseTicksCount =
            instantaneousRate.pulseTicksCount + pulseCount;
        if (instantaneousRate.pulseTicksCount > INSTANTANEOUS_RATE_PULSE_NUM)
            instantaneousRate.pulseTicksCount = INSTANTANEOUS_RATE_PULSE_NUM;
        for (unsigned int i = 0; i < pulseCount; i++)
        {
            instantaneousRate.pulseTicks[instantaneousRate.pulseTicksIndex] =
                instantaneousRate.tick;
            instantaneousRate.pulseTicksIndex =
                (instantaneousRate.pulseTicksIndex + 1) % INSTANTANEOUS_RATE_PULSE_NUM;
        }

        instantaneousRate.lastPulseTick = instantaneousRate.tick;

        // Average rate
        if (averageRate.tick < ULONG_MAX)
        {
            if (!averageRate.pulseCount)
                averageRate.firstPulseTick = averageRate.tick;
            addClamped(&averageRate.pulseCount, pulseCount);

            averageRate.lastPulseTick = averageRate.tick;
        }

        // Dose
        if (dose.snapshotTime < TIME_MAX)
            addClamped(&dose.pulseCount, pulseCount);
    }

    instantaneousRate.tick++;

    if ((averageRate.tick < ULONG_MAX) && (averageRate.pulseCount < ULONG_MAX))
        averageRate.tick++;
}

void onMeasurementOneSecond()
{
    unsigned int firstPulseTick;
    unsigned int pulseCount;
    unsigned int ticks;

    // Instantaneous rate
    for (unsigned int i = INSTANTANEOUS_RATE_HISTORY_STATS_NUM - 1; i > 0; i--)
        instantaneousRate.history[i] = instantaneousRate.history[i - 1];
    instantaneousRate.history[0] = instantaneousRate.current;
    resetPeriodStats(&instantaneousRate.current);

    firstPulseTick = 0;
    pulseCount = 0;
    for (unsigned int i = 0; i < INSTANTANEOUS_RATE_HISTORY_STATS_NUM; i++)
    {
        if (instantaneousRate.history[i].pulseCount)
        {
            firstPulseTick = instantaneousRate.history[i].firstPulseTick;
            pulseCount += instantaneousRate.history[i].pulseCount;
        }
    }

    if (pulseCount < INSTANTANEOUS_RATE_PULSE_NUM)
    {
        unsigned int pulseTicksIndex = (INSTANTANEOUS_RATE_PULSE_NUM +
                                        instantaneousRate.pulseTicksIndex -
                                        instantaneousRate.pulseTicksCount) %
                                       INSTANTANEOUS_RATE_PULSE_NUM;
        firstPulseTick = instantaneousRate.pulseTicks[pulseTicksIndex];
        pulseCount = instantaneousRate.pulseTicksCount;
    }

    if (instantaneousRate.pulseTicksCount < INSTANTANEOUS_RATE_PULSE_NUM)
        instantaneousRate.snapshotTime++;
    else
        instantaneousRate.snapshotTime =
            (instantaneousRate.tick + TICK_FREQUENCY - 1 - firstPulseTick) / TICK_FREQUENCY;
    ticks = instantaneousRate.lastPulseTick - firstPulseTick;
    if (ticks && (pulseCount > 1))
    {
        instantaneousRate.snapshotCount = pulseCount - 1;
        instantaneousRate.snapshotTicks = ticks;
    }
    else
    {
        instantaneousRate.snapshotCount = 0;
        instantaneousRate.snapshotTicks = 1;
    }

    // Average rate
    pulseCount = averageRate.pulseCount;
    ticks = averageRate.lastPulseTick - averageRate.firstPulseTick;

    if ((averageRate.snapshotTime < ULONG_MAX) && (averageRate.pulseCount < ULONG_MAX))
        averageRate.snapshotTime++;
    if (ticks && (pulseCount > 1))
    {
        averageRate.snapshotCount = pulseCount - 1;
        averageRate.snapshotTicks = ticks;
    }
    else
    {
        averageRate.snapshotCount = 0;
        averageRate.snapshotTicks = 1;
    }

    // Dose
    if ((dose.snapshotTime < TIME_MAX) && (dose.pulseCount < ULONG_MAX))
        dose.snapshotTime++;
    dose.snapshotValue = dose.pulseCount;
}

void updateMeasurements()
{
    // Instantaneous rate
    instantaneousRate.snapshotValue = (float)instantaneousRate.snapshotCount *
                                      TICK_FREQUENCY / instantaneousRate.snapshotTicks;

    if ((instantaneousRate.pulseTicksCount == INSTANTANEOUS_RATE_PULSE_NUM) &&
        (instantaneousRate.snapshotValue > instantaneousRate.snapshotMaxValue))
        instantaneousRate.snapshotMaxValue = instantaneousRate.snapshotValue;

    // Average rate
    averageRate.snapshotValue = (float)averageRate.snapshotCount *
                                TICK_FREQUENCY / averageRate.snapshotTicks;

    // History
    historySampleIndex =
        (historySampleIndex + 1) % histories[HISTORY_LAST].samplesPerDataPoint;
    for (unsigned int i = 0; i < HISTORY_NUM; i++)
    {
        History *history = &histories[i];
        HistoryState *historyState = &historyStates[i];

        historyState->sampleSum += instantaneousRate.snapshotValue;
        historyState->sampleNum++;

        if ((historySampleIndex % history->samplesPerDataPoint) == 0)
        {
            float average = historyState->sampleSum / historyState->sampleNum;
            int value = (int)(HISTORY_VALUE_DECADE * log10f(average / HISTORY_CPS_MIN));
            value = (value < 0) ? 0 : (value > UCHAR_MAX) ? UCHAR_MAX
                                                          : value;
            historyState->buffer[historyState->bufferIndex] = (unsigned char)value;

            historyState->sampleSum = 0;
            historyState->sampleNum = 0;

            historyState->bufferIndex = (historyState->bufferIndex + 1) % HISTORY_BUFFER_SIZE;
        }
    }
}

bool isInstantaneousRateAlarm()
{
    if (!settings.rateAlarm)
        return false;

    float rateSvH = units[UNITS_SIEVERTS].rate.scale * instantaneousRate.snapshotValue;
    return rateSvH >= getRateAlarmSvH(settings.rateAlarm);
}

bool isDoseAlarm()
{
    if (!settings.doseAlarm)
        return false;

    float doseSv = units[UNITS_SIEVERTS].dose.scale * dose.snapshotValue;
    return doseSv >= getDoseAlarmSv(settings.doseAlarm);
}

unsigned char getHistoryDataPoint(int dataIndex)
{
    HistoryState *historyState = &historyStates[settings.history];

    int bufferIndex =
        (HISTORY_BUFFER_SIZE + (historyState->bufferIndex - 1) - dataIndex) % HISTORY_BUFFER_SIZE;

    return historyState->buffer[bufferIndex];
}

const char *getHistoryName(int historyIndex)
{
    return histories[historyIndex].name;
}

// UI

void drawTitleWithTime(const char *title, unsigned int time)
{
    char timeString[16];
    formatTime(time, timeString);

    char titleString[32];
    sprintf(titleString, "%s (%s)", title, timeString);

    drawTitle(titleString);
}

void drawRate(float rate, unsigned int rateCount)
{
    char mantissa[32];
    char characteristic[32];
    formatRate(rate, mantissa, characteristic);

    drawMeasurementValue(mantissa, characteristic);

    if (rate > 0)
        drawConfidenceIntervals(rateCount);
}

void drawDose(unsigned int dose)
{
    char mantissa[32];
    char characteristic[32];
    formatDose(dose, mantissa, characteristic);

    drawMeasurementValue(mantissa, characteristic);
}

void drawRateMax(float rateMax)
{
    char mantissa[16];
    char characteristic[16];
    formatRate(rateMax, mantissa, characteristic);

    char subtitleString[48];
    sprintf(subtitleString, "Max: %s %s", mantissa, characteristic);

    drawSubtitle(subtitleString);
}

void drawInstantaneousRateView()
{
    unsigned int time;
    unsigned int count;
    float value;

    if (!instantaneousRate.isHold)
    {
        time = instantaneousRate.snapshotTime;
        count = instantaneousRate.snapshotCount;
        value = instantaneousRate.snapshotValue;
    }
    else
    {
        time = instantaneousRate.holdTime;
        count = instantaneousRate.holdCount;
        value = instantaneousRate.holdValue;
    }

    drawTitleWithTime("Instantaneous", time);
    drawRate(value, count);

    if (instantaneousRate.isHold)
        drawSubtitle("HOLD");
    else if (instantaneousRate.snapshotValue >= OVERLOAD_RATE)
        drawSubtitle("OVERLOAD");
    else if (isInstantaneousRateAlarm())
        drawSubtitle("RATE ALARM");
    else if (instantaneousRate.snapshotMaxValue > 0)
        drawRateMax(instantaneousRate.snapshotMaxValue);
}

void drawAverageRateView()
{
    unsigned int time;
    unsigned int count;
    float value;

    if (!averageRate.isHold)
    {
        time = averageRate.snapshotTime;
        count = averageRate.snapshotCount;
        value = averageRate.snapshotValue;
    }
    else
    {
        time = averageRate.holdTime;
        count = averageRate.holdCount;
        value = averageRate.holdValue;
    }

    drawTitleWithTime("Average", time);
    drawRate(value, count);

    if (averageRate.isHold)
        drawSubtitle("HOLD");
    else if ((averageRate.snapshotTime >= TIME_MAX) ||
             (averageRate.snapshotCount >= (ULONG_MAX - 1)))
        drawSubtitle("OVERFLOW");
    else if (averageRate.snapshotValue >= OVERLOAD_RATE)
        drawSubtitle("OVERLOAD");
}

void drawDoseView()
{
    unsigned int time;
    unsigned int value;

    if (!dose.isHold)
    {
        time = dose.snapshotTime;
        value = dose.snapshotValue;
    }
    else
    {
        time = dose.holdTime;
        value = dose.holdValue;
    }

    drawTitleWithTime("Dose", time);
    drawDose(value);

    if (dose.isHold)
        drawSubtitle("HOLD");
    else if ((dose.snapshotTime >= ULONG_MAX) ||
             dose.snapshotValue >= ULONG_MAX)
        drawSubtitle("OVERFLOW");
    else if (isDoseAlarm())
        drawSubtitle("DOSE ALARM");
}

void drawHistoryView()
{
    // Initialization
    Unit *unit = &units[settings.units].rate;

    char labelMin[16] = "";
    char labelMax[16] = "";

    int offset = 0;
    int range = 1;

    // Formatting
    int valueMin = UCHAR_MAX;
    int valueMax = 0;

    for (int i = 0; i < HISTORY_BUFFER_SIZE; i++)
    {
        int value = getHistoryDataPoint(i);
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
        int unitScaleValue = (int)(HISTORY_VALUE_DECADE * log10f(unit->scale * HISTORY_CPS_MIN));

        int exponentMin = divideDown(valueMin + unitScaleValue, HISTORY_VALUE_DECADE);
        int exponentMax = divideDown(valueMax + unitScaleValue, HISTORY_VALUE_DECADE) + 1;

        offset = unitScaleValue - exponentMin * HISTORY_VALUE_DECADE;
        range = exponentMax - exponentMin;

        formatMultiplier(unit->name, exponentMin, unit->minExponent, labelMin);
        formatMultiplier(unit->name, exponentMax, unit->minExponent, labelMax);
    }

    drawTitle(getHistoryName(settings.history));

    drawHistory(labelMin, labelMax, offset, range);
}

void onMeasurementViewKey(int key)
{
    switch (key)
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

    case KEY_BACK_UP:
        switch (getView())
        {
        case VIEW_INSTANTANEOUS_RATE:
            instantaneousRate.isHold = !instantaneousRate.isHold;
            if (instantaneousRate.isHold)
            {
                instantaneousRate.holdTime = instantaneousRate.snapshotTime;
                instantaneousRate.holdCount = instantaneousRate.snapshotCount;
                instantaneousRate.holdValue = instantaneousRate.snapshotValue;
            }
            break;

        case VIEW_AVERAGE_RATE:
            averageRate.isHold = !averageRate.isHold;
            if (averageRate.isHold)
            {
                averageRate.holdTime = averageRate.snapshotTime;
                averageRate.holdCount = averageRate.snapshotCount;
                averageRate.holdValue = averageRate.snapshotValue;
            }
            break;

        case VIEW_DOSE:
            dose.isHold = !dose.isHold;
            if (dose.isHold)
            {
                dose.holdTime = dose.snapshotTime;
                dose.holdValue = dose.snapshotValue;
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
