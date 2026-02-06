/*
 * Rad Pro
 * History
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../devices/rtc.h"
#include "../devices/voice.h"
#include "../measurements/datalog.h"
#include "../measurements/history.h"
#include "../measurements/instantaneous.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../system/system.h"
#include "../ui/history.h"
#include "../ui/system.h"

typedef enum
{
    HISTORY_TAB_10_MINUTES,
    HISTORY_TAB_1_HOUR,
    HISTORY_TAB_1_DAY,
    HISTORY_TAB_1_WEEK,
    HISTORY_TAB_1_MONTH,
    HISTORY_TAB_1_YEAR,

    HISTORY_TAB_NUM,
} HistoryTab;

typedef const struct
{
    cstring name;
    uint32_t binInterval;
    uint8_t timeTicksNum;
} History;

typedef struct
{
    uint32_t timeInterval;

    uint32_t cumulativeTime;
    float cumulativePulseCount;

    uint8_t logValues[HISTORY_BIN_NUM];
} HistoryState;

static History histories[] = {
    {STRING_HISTORY_10_MINUTES, 10 * 60 / (HISTORY_BIN_NUM - 1), 10},
    {STRING_HISTORY_1_HOUR, 60 * 60 / (HISTORY_BIN_NUM - 1), 6},
    {STRING_HISTORY_1_DAY, 24 * 60 * 60 / (HISTORY_BIN_NUM - 1), 8},
    {STRING_HISTORY_1_WEEK, 7 * 24 * 60 * 60 / (HISTORY_BIN_NUM - 1), 7},
    {STRING_HISTORY_1_MONTH, 28 * 24 * 60 * 60 / (HISTORY_BIN_NUM - 1), 4},
    {STRING_HISTORY_1_YEAR, 365 * 24 * 60 * 60 / (HISTORY_BIN_NUM - 1), 12},
};

static HistoryState historyStates[HISTORY_TAB_NUM];

HistoryTab historyTab;

void setupHistory(void)
{
    resetHistory();
}

void resetHistory(void)
{
    memset(&historyStates, 0, sizeof(historyStates));
}

typedef struct
{
    uint32_t binIndex;
    uint32_t binStart;
    uint32_t binEnd;

    uint32_t cumulativeTime;
    uint64_t cumulativePulseCountInt;
    float cumulativePulseCountFloat;
} LoadHistoryState;

uint8_t getHistoryLogValue(float value)
{
    if (value < HISTORY_VALUE_MIN)
        return 0;
    else {
        uint32_t logValue = (int32_t)(HISTORY_DECADE * log10f(value / HISTORY_VALUE_MIN));

        if (logValue > UCHAR_MAX)
            logValue = UCHAR_MAX;

        return logValue;
    }
}

static void updateLoadHistory(LoadHistoryState *state, uint8_t *logValues)
{
    uint32_t cumulativeTime = state->cumulativeTime;
    uint64_t cumulativePulseCountInt = state->cumulativePulseCountInt;
    float cumulativePulseCountFloat = state->cumulativePulseCountFloat;

    while (cumulativePulseCountInt >> 32)
    {
        cumulativeTime >>= 8;
        cumulativePulseCountInt >>= 8;
        cumulativePulseCountFloat /= 256.0F;
    }

    float rate = cumulativeTime ? (cumulativePulseCountFloat + (float)(uint32_t)cumulativePulseCountInt) / (float)cumulativeTime : 0.0F;
    logValues[state->binIndex] = getHistoryLogValue(rate);
}

void loadHistory(void)
{
    uint32_t historyEnd = getDeviceTime();

    LoadHistoryState states[HISTORY_TAB_NUM];
    memset(states, 0, HISTORY_TAB_NUM * sizeof(LoadHistoryState));

    uint32_t historiesStart[HISTORY_TAB_NUM];
    for (uint32_t historyIndex = 0; historyIndex < HISTORY_TAB_NUM; historyIndex++)
    {
        uint32_t binInterval = histories[historyIndex].binInterval;
        historiesStart[historyIndex] = historyEnd - HISTORY_BIN_NUM * binInterval;
    }

#if defined(FAST_SYSTEM_CLOCK)
    setFastSystemClock(true);
#endif

    if (startDatalogDump())
    {
        DatalogRecord prevRecord;
        DatalogRecord record;

        if (readDatalog(&prevRecord))
        {
            while (readDatalog(&record))
            {
                resetWatchdog();

                if (!record.sessionStart)
                {
                    for (uint32_t historyIndex = 0; historyIndex < HISTORY_TAB_NUM; historyIndex++)
                    {
                        uint32_t historyStart = historiesStart[historyIndex];
                        LoadHistoryState *state = &states[historyIndex];

                        // Overlap record interval with history interval
                        uint32_t recordStart = (prevRecord.dose.time > historyStart) ? prevRecord.dose.time : historyStart;
                        uint32_t recordEnd = (record.dose.time < historyEnd) ? record.dose.time : historyEnd;

                        if (recordStart < recordEnd)
                        {
                            int32_t intervalTime = record.dose.time - prevRecord.dose.time;
                            uint32_t intervalPulseCount = record.dose.pulseCount - prevRecord.dose.pulseCount;

                            if ((record.dose.time > state->binStart) &&
                                (record.dose.time <= state->binEnd))
                            {
                                state->cumulativeTime += intervalTime;
                                state->cumulativePulseCountInt += intervalPulseCount;
                            }
                            else
                            {
                                uint32_t binInterval = histories[historyIndex].binInterval;
                                uint8_t *logValues = historyStates[historyIndex].logValues;

                                uint32_t binIndexStart = (recordStart - historyStart) / binInterval;
                                uint32_t binIndexEnd = (recordEnd - 1 - historyStart) / binInterval + 1;

                                for (uint32_t binIndex = binIndexStart; binIndex < binIndexEnd; binIndex++)
                                {
                                    // Overlap record interval with bin interval
                                    uint32_t binStart = historyStart + binIndex * binInterval;
                                    uint32_t binEnd = binStart + binInterval;

                                    uint32_t overlapStart = (binStart > recordStart) ? binStart : recordStart;
                                    uint32_t overlapEnd = (binEnd < recordEnd) ? binEnd : recordEnd;

                                    uint32_t overlapTime = overlapEnd - overlapStart;
                                    float overlapPulseCount = (float)intervalPulseCount * (float)overlapTime / (float)intervalTime;

                                    if (binIndex != state->binIndex)
                                    {
                                        updateLoadHistory(state, logValues);

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

                prevRecord.dose = record.dose;
            }

            for (uint32_t historyIndex = 0; historyIndex < HISTORY_TAB_NUM; historyIndex++)
            {
                LoadHistoryState *state = &states[historyIndex];
                uint8_t *logValues = historyStates[historyIndex].logValues;

                updateLoadHistory(state, logValues);
            }
        }
    }

#if defined(FAST_SYSTEM_CLOCK)
    setFastSystemClock(false);
#endif
}

void updateHistory(void)
{
    for (uint32_t historyIndex = 0; historyIndex < HISTORY_TAB_NUM; historyIndex++)
    {
        History *history = &histories[historyIndex];
        HistoryState *historyState = &historyStates[historyIndex];

        if (isInstantaneousRateConfidenceGood() && (getInstantaneousRate() >= 0))
        {
            historyState->cumulativeTime++;
            historyState->cumulativePulseCount += getInstantaneousRate();
        }

        historyState->timeInterval++;
        if (historyState->timeInterval >= history->binInterval)
        {
            float binRate = historyState->cumulativeTime ? historyState->cumulativePulseCount / historyState->cumulativeTime : 0;
            for (uint32_t i = 0; i < HISTORY_BIN_NUM - 1; i++)
                historyState->logValues[i] = historyState->logValues[i + 1];
            historyState->logValues[HISTORY_BIN_NUM - 1] = getHistoryLogValue(binRate);

            historyState->timeInterval = 0;

            historyState->cumulativeTime = 0;
            historyState->cumulativePulseCount = 0;
        }
    }
}

// History view

static void onHistoryViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        historyTab++;

        if (historyTab >= HISTORY_TAB_NUM)
            historyTab = HISTORY_TAB_10_MINUTES;

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        const HistoryState *historyState = &historyStates[historyTab];
        const Unit *rateUnit = &pulseUnits[settings.doseUnits].rate;

        drawTitleBar(getString(STRING_HISTORY));
        drawHistory(rateUnit->scale,
                    rateUnit->name,
                    histories[historyTab].timeTicksNum,
                    histories[historyTab].name,
                    historyState->logValues,
                    getHistoryLogValue(rateAlerts[settings.rateWarning] / pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale),
                    getHistoryLogValue(rateAlerts[settings.rateAlarm] / pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale));

        break;
    }

    default:
        break;
    }
}

View historyView = {
    onHistoryViewEvent,
    NULL,
};
