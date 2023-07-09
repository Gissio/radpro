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
#include "menu.h"
#include "settings.h"

#define INTERVAL_STATS_NUM 5
#define PULSE_QUEUE_LENGTH (10 + 1)

#define PULSE_CLICKS_QUIET_TICKS ((uint32_t)(0.001F * SYS_TICK_FREQUENCY))
#define PULSE_CLICKS_LOUD_TICKS ((uint32_t)(0.015F * SYS_TICK_FREQUENCY))
#define PULSE_BACKLIGHT_FLASH_TICKS ((uint32_t)(0.08F * SYS_TICK_FREQUENCY))

#define OVERLOAD_RATE 1000

#define ALARM_TICKS ((uint32_t)(0.25F * SYS_TICK_FREQUENCY))

struct
{
    const struct View *currentView;

    struct Settings lastSettings;
} measurements;

struct UnitType units[] = {
    {{"Sv/h", (60 * 1E-6F), -6},
     {"Sv", (60 * 1E-6F / 3600), -6}},
    {{"rem/h", (60 * 1E-4F), -6},
     {"rem", (60 * 1E-4F / 3600), -6}},
    {{"cpm", 60, 0},
     {"counts", 1, 0}},
    {{"cps", 1, 0},
     {"counts", 1, 0}},
};

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

struct IntervalStats
{
    uint32_t firstPulseTick;
    uint32_t pulseCount;
};

struct PulseQueue
{
    uint32_t count;
    uint32_t index;
    uint32_t tick[PULSE_QUEUE_LENGTH];
};

struct
{
    uint32_t tick;
    struct IntervalStats intervalStats[INTERVAL_STATS_NUM];
    struct PulseQueue pulseQueue;

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

struct History
{
    char *const name;
    uint32_t samplesPerDataPoint;
};

struct HistoryState
{
    float sampleSum;
    uint32_t sampleNum;

    uint8_t bufferIndex;
    uint8_t buffer[HISTORY_BUFFER_SIZE];
};

const struct History histories[HISTORY_NUM] = {
    {"History (2m)", 1},
    {"History (10m)", 5},
    {"History (1h)", 30},
    {"History (6h)", 180},
    {"History (24h)", 720},
};
struct HistoryState historyStates[HISTORY_NUM];
uint32_t historySampleIndex;

static const struct Menu unitsMenu;
static const struct Menu historyMenu;
static const struct Menu rateAlarmMenu;
static const struct Menu doseAlarmMenu;
static const struct Menu tubeTypeMenu;
static const struct Menu customTubeMenu;

static void updateTubeType(void);

static void resetInstantaneousRate(void);
static bool isInstantaneousRateAlarm(void);

static void resetAverageRate(void);

static void resetDose(void);
static bool isDoseAlarm(void);

static void resetHistory(void);

// Common

void initMeasurements(void)
{
    measurements.lastSettings = settings;

    selectMenuIndex(&unitsMenu, settings.units);
    selectMenuIndex(&historyMenu, settings.history);
    selectMenuIndex(&rateAlarmMenu, settings.rateAlarm);
    selectMenuIndex(&doseAlarmMenu, settings.doseAlarm);

    selectMenuIndex(&tubeTypeMenu, settings.tubeType);
    selectMenuIndex(&customTubeMenu, settings.conversionFactor);

    updateTubeType();
}

static void updateTubeType(void)
{
    float cpmPerUSvH = 1;

    switch (settings.tubeType)
    {
    case TUBE_TYPE_M4011:
        cpmPerUSvH = 153.F;

        break;

    case TUBE_TYPE_HH614:
        cpmPerUSvH = 68.4F;

        break;

    case TUBE_TYPE_J305:
        cpmPerUSvH = 123.F;

        break;

    case TUBE_TYPE_CUSTOM:
        cpmPerUSvH = getConversionFactor(settings.conversionFactor);

        break;
    }

    units[0].rate.scale = (60 * 1E-6F) / cpmPerUSvH;
    units[0].dose.scale = (60 * 1E-6F / 3600) / cpmPerUSvH;
    units[1].rate.scale = (60 * 1E-4F) / cpmPerUSvH;
    units[1].dose.scale = (60 * 1E-4F / 3600) / cpmPerUSvH;
}

static void resetIntervalStats(struct IntervalStats *intervalStats)
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
        addClamped(&lifeState.pulseCount, pulseCount);

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
        if (averageRate.tick < UINT32_MAX)
        {
            if (!averageRate.pulseCount)
                averageRate.firstPulseTick = averageRate.tick;
            averageRate.lastPulseTick = averageRate.tick;
            addClamped(&averageRate.pulseCount, pulseCount);
        }

        // Dose
        addClamped(&dose.pulseCount, pulseCount);

        // Buzzer
        switch (settings.pulseClicks)
        {
        case PULSE_CLICKS_QUIET:
            setBuzzerTimer(PULSE_CLICKS_QUIET_TICKS);

            break;

        case PULSE_CLICKS_LOUD:
            setBuzzerTimer(PULSE_CLICKS_LOUD_TICKS);

            break;
        }

        // Display
        if (settings.backlight == BACKLIGHT_PULSE_FLASHES)
            setBacklightTimer(PULSE_BACKLIGHT_FLASH_TICKS);
    }

    // Update ticks
    instantaneousRate.tick++;

    if (averageRate.tick < UINT32_MAX)
        averageRate.tick++;
}

void onMeasurementOneSecond(void)
{
    uint32_t firstPulseTick;
    uint32_t lastPulseTick;
    uint32_t pulseCount;
    uint32_t ticks;

    if (lifeState.time < UINT32_MAX)
        lifeState.time++;

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

    if ((averageRate.tick < UINT32_MAX) && (averageRate.pulseCount < UINT32_MAX))
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
        const struct History *history = &histories[i];
        struct HistoryState *historyState = &historyStates[i];

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

void setMeasurementView(const struct View *view)
{
    if (memcmp(&settings, &measurements.lastSettings, sizeof(struct Settings)) != 0)
        writeSettings();

    measurements.currentView = view;
    measurements.lastSettings = settings;

    setView(view);
}

const struct View *getMeasurementView(void)
{
    return measurements.currentView;
}

// Instantaneous rate view

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
}

static bool isInstantaneousRateAlarm(void)
{
    if (!settings.rateAlarm)
        return false;

    float rateSvH = units[UNITS_SIEVERTS].rate.scale * instantaneousRate.snapshotValue;
    return rateSvH >= rateAlarmsSvH[settings.rateAlarm];
}

static void onInstantaneousRateDraw(const struct View *view)
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

static void onInstantaneousRateKey(const struct View *view, KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_EVENT_UP:
        setMeasurementView(&historyView);

        break;

    case KEY_EVENT_DOWN:
        setMeasurementView(&averageRateView);

        break;

    case KEY_EVENT_ENTER:
        setView(&settingsMenuView);

        break;

    case KEY_EVENT_BACK:
        instantaneousRate.isHold = !instantaneousRate.isHold;
        if (instantaneousRate.isHold)
        {
            instantaneousRate.holdTime = instantaneousRate.snapshotTime;
            instantaneousRate.holdPulseCount = instantaneousRate.snapshotPulseCount;
            instantaneousRate.holdValue = instantaneousRate.snapshotValue;
        }

        refreshView();

        break;

    case KEY_EVENT_RESET:
        resetInstantaneousRate();

        refreshView();

        break;
    }
}

const struct View instantaneousRateView = {
    onInstantaneousRateDraw,
    onInstantaneousRateKey,
    NULL,
};

// Average rate view

void resetAverageRate(void)
{
    averageRate.tick = 0;
    averageRate.firstPulseTick = 0;
    averageRate.lastPulseTick = 0;
    averageRate.pulseCount = 0;

    averageRate.snapshotTime = 0;
    averageRate.snapshotPulseCount = 0;
    averageRate.snapshotValue = 0;

    averageRate.isHold = false;
}

static void onAverageRateDraw(const struct View *view)
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
    else if ((averageRate.tick >= UINT32_MAX) ||
             (averageRate.snapshotPulseCount >= UINT32_MAX))
        drawSubtitle("OVERFLOW");
    else if (averageRate.snapshotValue >= OVERLOAD_RATE)
        drawSubtitle("OVERLOAD");
}

static void onAverageRateKey(const struct View *view, KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_EVENT_UP:
        setMeasurementView(&instantaneousRateView);

        break;

    case KEY_EVENT_DOWN:
        setMeasurementView(&doseView);

        break;

    case KEY_EVENT_ENTER:
        setView(&settingsMenuView);

        break;

    case KEY_EVENT_BACK:
        averageRate.isHold = !averageRate.isHold;
        if (averageRate.isHold)
        {
            averageRate.holdTime = averageRate.snapshotTime;
            averageRate.holdPulseCount = averageRate.snapshotPulseCount;
            averageRate.holdValue = averageRate.snapshotValue;
        }

        refreshView();

        break;

    case KEY_EVENT_RESET:
        resetAverageRate();

        refreshView();

        break;
    }
}

const struct View averageRateView = {
    onAverageRateDraw,
    onAverageRateKey,
    NULL,
};

// Dose view

void setDose(uint32_t time, uint32_t pulseCount)
{
    syncTimerThread();

    dose.pulseCount = pulseCount;

    dose.snapshotTime = time;
    dose.snapshotPulseCount = pulseCount;
}

void getDose(uint32_t *time, uint32_t *pulseCount)
{
    syncTimerThread();

    *time = dose.snapshotTime;
    *pulseCount = dose.snapshotPulseCount;
}

static void resetDose(void)
{
    setDose(0, 0);

    dose.isHold = false;
}

static bool isDoseAlarm(void)
{
    if (!settings.doseAlarm)
        return false;

    float doseSv = units[UNITS_SIEVERTS].dose.scale * dose.snapshotPulseCount;
    return doseSv >= doseAlarmsSv[settings.doseAlarm];
}

static void onDoseViewDraw(const struct View *view)
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

static void onDoseViewKey(const struct View *view, KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_EVENT_UP:
        setMeasurementView(&averageRateView);

        break;

    case KEY_EVENT_DOWN:
        setMeasurementView(&historyView);

        break;

    case KEY_EVENT_ENTER:
        setView(&settingsMenuView);

        break;

    case KEY_EVENT_BACK:
        dose.isHold = !dose.isHold;
        if (dose.isHold)
        {
            dose.holdTime = dose.snapshotTime;
            dose.holdPulseCount = dose.snapshotPulseCount;
        }

        refreshView();

        break;

    case KEY_EVENT_RESET:
        resetDose();

        refreshView();

        break;
    }
}

const struct View doseView = {
    onDoseViewDraw,
    onDoseViewKey,
    NULL,
};

// History view

static void resetHistory(void)
{
    for (uint32_t i = 0; i < HISTORY_NUM; i++)
    {
        struct HistoryState *historyState = &historyStates[i];

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
    struct HistoryState *historyState = &historyStates[settings.history];

    uint32_t bufferIndex =
        (HISTORY_BUFFER_SIZE + (historyState->bufferIndex - 1) - dataIndex) % HISTORY_BUFFER_SIZE;

    return historyState->buffer[bufferIndex];
}

static const char *getHistoryName(uint32_t historyIndex)
{
    return histories[historyIndex].name;
}

static void onHistoryViewDraw(const struct View *view)
{
    // Initialization
    struct Unit *unit = &units[settings.units].rate;

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

static void onHistoryViewKey(const struct View *view, KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_EVENT_UP:
        setMeasurementView(&doseView);

        break;

    case KEY_EVENT_DOWN:
        setMeasurementView(&instantaneousRateView);

        break;

    case KEY_EVENT_ENTER:
        setView(&settingsMenuView);

        break;

    case KEY_EVENT_RESET:
        resetHistory();

        refreshView();

        break;
    }
}

const struct View historyView = {
    onHistoryViewDraw,
    onHistoryViewKey,
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
    onMenuViewDraw,
    onMenuViewKey,
    &unitsMenu,
};

// History menu

static void onHistoryMenuSelect(const struct Menu *menu)
{
    settings.history = menu->state->selectedIndex;
}

static const char *const historyMenuOptions[] = {
    "2 minutes",
    "10 minutes",
    "1 hour",
    "6 hours",
    "24 hours",
    NULL,
};

static struct MenuState historyMenuState;

static const struct Menu historyMenu = {
    "History",
    &historyMenuState,
    onMenuGetOption,
    historyMenuOptions,
    onHistoryMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View historyMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &historyMenu,
};

// Rate alarm menu

static void onRateAlarmMenuSelect(const struct Menu *menu)
{
    settings.rateAlarm = menu->state->selectedIndex;
}

static const char *onRateAlarmMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (!index)
        return "Off";

    if (index >= RATE_ALARM_NUM)
        return NULL;

    struct Unit *unit = &units[settings.units].rate;
    float rate = rateAlarmsSvH[index] / units[UNITS_SIEVERTS].rate.scale;
    formatValue(unit->name,
                unit->scale * rate,
                menuOption);

    return menuOption;
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
    onMenuViewDraw,
    onMenuViewKey,
    &rateAlarmMenu,
};

// Dose alarm menu

static void onDoseAlarmMenuSelect(const struct Menu *menu)
{
    settings.doseAlarm = menu->state->selectedIndex;
}

static const char *onDoseAlarmMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (!index)
        return "Off";

    if (index >= DOSE_ALARM_NUM)
        return NULL;

    struct Unit *unit = &units[settings.units].dose;
    float doseValue = doseAlarmsSv[index] / units[UNITS_SIEVERTS].dose.scale;
    formatValue(unit->name,
                unit->scale * doseValue,
                menuOption);

    return menuOption;
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
    onMenuViewDraw,
    onMenuViewKey,
    &doseAlarmMenu,
};

// Custom tube menu

static void onCustomTubeMenuSelect(const struct Menu *menu)
{
    settings.conversionFactor = menu->state->selectedIndex;

    updateTubeType();
}

static void onTubeTypeSubMenuBack(const struct Menu *menu)
{
    setView(&tubeTypeMenuView);
}

static const char *onCustomTubeMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (index >= CONVERSION_FACTOR_STEPS)
        return NULL;

    uint32_t value = (uint32_t)(10.0F * getConversionFactor(index));

    strcpy(menuOption, "");
    strcatNumber(menuOption, value / 10, 0);
    strcat(menuOption, ".");
    strcatNumber(menuOption, value % 10, 0);
    strcat(menuOption, " cpm/\xb5Sv/h");

    return menuOption;
}

static struct MenuState customTubeMenuState;

static const struct Menu customTubeMenu = {
    "Custom conversion factor",
    &customTubeMenuState,
    onCustomTubeMenuGetOption,
    NULL,
    onCustomTubeMenuSelect,
    NULL,
    onTubeTypeSubMenuBack,
};

const struct View customTubeMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &customTubeMenu,
};

// Tube type menu

static void onTubeTypeMenuSelect(const struct Menu *menu)
{
    settings.tubeType = menu->state->selectedIndex;

    updateTubeType();
}

static void onTubeTypeMenuEnter(const struct Menu *menu)
{
    if (menu->state->selectedIndex == 3)
        setView(&customTubeMenuView);
}

static const char *const tubeTypeMenuOptions[] = {
    "J305 (123 cpm/\xb5Sv/h)",
    "HH614 (68.4 cpm/\xb5Sv/h)",
    "M4011 (153 cpm/\xb5Sv/h)",
    "Custom conversion factor",
    NULL,
};

static struct MenuState tubeTypeMenuState;

static const struct Menu tubeTypeMenu = {
    "Geiger tube type",
    &tubeTypeMenuState,
    onMenuGetOption,
    tubeTypeMenuOptions,
    onTubeTypeMenuSelect,
    onTubeTypeMenuEnter,
    onSettingsSubMenuBack,
};

const struct View tubeTypeMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &tubeTypeMenu,
};
