/*
 * Rad Pro
 * Pulses
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../devices/led.h"
#include "../devices/pulsesoundenable.h"
#include "../devices/tube.h"
#include "../measurements/average.h"
#include "../measurements/cumulative.h"
#include "../measurements/history.h"
#include "../measurements/instantaneous.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"

#define PULSE_INDICATION_SENSITIVITY_MAX 600.0F
#define PULSE_INDICATION_FACTOR_UNIT 0x10000
#define PULSE_INDICATION_FACTOR_MASK (PULSE_INDICATION_FACTOR_UNIT - 1)

static Menu pulsesMenu;
static Menu pulsesIndicationMenu;
static Menu pulsesThresholdMenu;

static Menu rateWarningMenu;
static Menu doseWarningMenu;
static Menu rateAlarmMenu;
static Menu doseAlarmMenu;

static Menu doseUnitsMenu;
static Menu secondaryDoseUnitsMenu;

static const char *buildRateAlertMenuOption(uint32_t index);

static void setPulsesMenu(void);

const float rateAlerts[] = {
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

const float doseAlerts[] = {
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

const int8_t doseUnitsMinMetricPrefix[] = {-2, -2, 0, 0};

const int8_t doseUnitsBarMinExponent[] = {-8, -6, 0, -2};

PulseUnits pulseUnits[] = {
    {{STRING_SVH, (60 * 1E-6F)},
     {STRING_SV, (60 * 1E-6F / 3600)}},
    {{STRING_REMH, (60 * 1E-4F)},
     {STRING_REM, (60 * 1E-4F / 3600)}},
    {{STRING_CPM, 60},
     {STRING_EMPTY, 1}},
    {{STRING_CPS, 1},
     {STRING_EMPTY, 1}},
};

static struct
{
    // onPulseTick
    uint32_t lastTubePulseCount;

    Dose tubeDose;

    PulsePeriod currentPeriod;

    uint32_t indicationFactor;
    uint32_t indicationRemainder;

    bool rateOverThreshold;

    // onPulseHeartbeat
    PulsePeriod lastPeriod;
    uint32_t lastPeriodTick;

    // updatePulses
    uint32_t lossOfCountTimer;
    bool lastTubeShorted;
    AlertLevel alertLevel;
    bool alertTriggered;

    float deadTimeCompensationRemainder;
} pulses;

void setupPulses(void)
{
    Dose tubeDose = pulses.tubeDose;
    memset(&pulses, 0, sizeof(pulses));
    pulses.tubeDose = tubeDose;

    updateDoseUnits();

    setupInstantaneousRate();
    setupAverageRate();
    setupCumulativeDose();
    setupHistory();

    selectMenuItem(&pulsesMenu, 0, 0);
    selectMenuItem(&pulsesIndicationMenu, 0, 0);
    selectMenuItem(&pulsesThresholdMenu, settings.pulseThreshold, RATE_NUM);

    selectMenuItem(&rateWarningMenu, settings.rateWarning, RATE_NUM);
    selectMenuItem(&doseWarningMenu, settings.doseWarning, DOSE_NUM);
    selectMenuItem(&rateAlarmMenu, settings.rateAlarm, RATE_NUM);
    selectMenuItem(&doseAlarmMenu, settings.doseAlarm, DOSE_NUM);

    selectMenuItem(&doseUnitsMenu, settings.doseUnits, DOSE_UNITS_NUM);
    selectMenuItem(&secondaryDoseUnitsMenu, settings.secondaryDoseUnits, DOSE_UNITS_NUM);
}

void updateDoseUnits(void)
{
    float sensitivity = getTubeSensitivity();

    pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale = (60.0F * 1E-6F) / sensitivity;
    pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale = (60.0F * 1E-6F / 3600.0F) / sensitivity;
    pulseUnits[DOSE_UNITS_REM].rate.scale = (60.0F * 1E-4F) / sensitivity;
    pulseUnits[DOSE_UNITS_REM].dose.scale = (60.0F * 1E-4F / 3600.0F) / sensitivity;

    if (sensitivity < PULSE_INDICATION_SENSITIVITY_MAX)
        pulses.indicationFactor = PULSE_INDICATION_FACTOR_UNIT;
    else
        pulses.indicationFactor = (PULSE_INDICATION_FACTOR_UNIT * PULSE_INDICATION_SENSITIVITY_MAX) / sensitivity;
}

void calculateRate(Rate *rate, PulsePeriod *period)
{
    uint32_t ticks = period->lastTick - period->firstTick;
    uint32_t pulseCount = period->pulseCount;

    if (ticks < 2)
    {
        rate->value = 0;
        rate->confidence = 0;

        return;
    }

    if (pulseCount > ticks)
    {
        // Improves precision with high radiation rates
        pulseCount = addClamped(pulseCount, 1);
        ticks = addClamped(ticks, 1);
    }

    rate->value = (float)(pulseCount - 1) * SYSTICK_FREQUENCY / ticks;
    rate->confidence = getConfidenceInterval(pulseCount - 1);
}

float getRateFactor(void)
{
    return pulseUnits[settings.doseUnits].rate.scale;
}

float getDoseFactor(void)
{
    return pulseUnits[settings.doseUnits].dose.scale;
}

void updatePulseThresholdExceeded(void)
{
    float rateSvH = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * getInstantaneousRate();

    pulses.rateOverThreshold = settings.pulseThreshold ? rateSvH >= rateAlerts[settings.pulseThreshold] : true;
}

bool isPulseThresholdExceeded(void)
{
    return pulses.rateOverThreshold;
}

void onPulseTick(void)
{
    uint32_t currentPulseCount = tubePulseCount;
    uint32_t pulseCount = currentPulseCount - pulses.lastTubePulseCount;
    pulses.lastTubePulseCount = currentPulseCount;

    if (pulseCount)
    {
        // Tube dose
        pulses.tubeDose.pulseCount += pulseCount;

        // Current period
        if (!pulses.currentPeriod.pulseCount)
            pulses.currentPeriod.firstTick = currentTick;
        pulses.currentPeriod.lastTick = currentTick;
        pulses.currentPeriod.pulseCount += pulseCount;

        // Pulse indication
        pulses.indicationRemainder += pulseCount * pulses.indicationFactor;
        if (pulses.indicationRemainder >= PULSE_INDICATION_FACTOR_UNIT)
        {
            pulses.indicationRemainder &= PULSE_INDICATION_FACTOR_MASK;

            if (pulses.rateOverThreshold)
                indicatePulse();
        }
    }
}

void onPulseHeartbeat(void)
{
    if (!isMeasurementsEnabled())
        return;

    // Tube dose
    pulses.tubeDose.time += 1;

    // Last period
    pulses.lastPeriod = pulses.currentPeriod;
    pulses.lastPeriodTick = currentTick;
    pulses.currentPeriod.pulseCount = 0;
}

void updatePulses(void)
{
    // Loss of count
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    if (pulses.lastPeriod.pulseCount)
        pulses.lossOfCountTimer = 0;
    else
        pulses.lossOfCountTimer++;
    if (pulses.lossOfCountTimer >= getLossOfCountTime())
        alertLevel = ALERTLEVEL_ALARM;

    // Tube shorted
    bool tubeShorted = (!pulses.lastPeriod.pulseCount && readTubeDet());
    if (pulses.lastTubeShorted && tubeShorted)
        alertLevel = ALERTLEVEL_ALARM;
    pulses.lastTubeShorted = tubeShorted;

    // Fault alert
    pulses.alertTriggered = (alertLevel > pulses.alertLevel);
    pulses.alertLevel = alertLevel;

    if (pulses.alertTriggered)
        setAlertPending(true);

    // Instantaneous rate
    updateInstantaneousRate(pulses.lastPeriodTick, &pulses.lastPeriod);

    // Compensate period
    PulsePeriod compensatedPeriod = pulses.lastPeriod;
    if (settings.tubeDeadTimeCompensation)
    {
        pulses.deadTimeCompensationRemainder += getDeadTimeCompensationFactor() * pulses.lastPeriod.pulseCount;
        compensatedPeriod.pulseCount = (uint32_t)pulses.deadTimeCompensationRemainder;
        if (pulses.deadTimeCompensationRemainder >= 1)
            pulses.deadTimeCompensationRemainder -= compensatedPeriod.pulseCount;
    }

    // Average rate, cumulative dose, history
    updateAverageRate(&compensatedPeriod);
    updateCumulativeDose(&compensatedPeriod);
    updateHistory();
}

void setTubeTime(uint32_t value)
{
    syncTick();

    pulses.tubeDose.time = value;
}

uint32_t getTubeTime(void)
{
    return pulses.tubeDose.time;
}

void setTubePulseCount(uint32_t value)
{
    syncTick();

    pulses.tubeDose.pulseCount = value;
}

uint32_t getTubePulseCount(void)
{
    return pulses.tubeDose.pulseCount;
}

float getTubeDeadTime(void)
{
    float value = tubeDeadTime * (1.0F / PULSE_MEASUREMENT_FREQUENCY);

    return value;
}

AlertLevel getTubeFaultAlertLevel(void)
{
    return pulses.alertLevel;
}

bool isTubeFaultAlertTriggered(void)
{
    return pulses.alertTriggered;
}

// Pulses indication menu

enum
{
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    PULSEINDICATION_MENU_SOUND,
#endif
#if defined(VIBRATOR)
    PULSEINDICATION_MENU_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    PULSEINDICATION_MENU_PULSE_LED,
#endif
    PULSEINDICATION_MENU_DISPLAY_FLASH,
};

static cstring pulsesIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    STRING_SOUND,
#endif
#if defined(VIBRATOR)
    STRING_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    STRING_PULSE_LED,
#endif
    STRING_DISPLAY_FLASH,
    NULL,
};

static const char *onPulsesIndicationMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case PULSEINDICATION_MENU_SOUND:
        *menuStyle = settings.pulseSound;

        break;
#endif

#if defined(VIBRATOR)
    case PULSEINDICATION_MENU_VIBRATION:
        *menuStyle = settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    case PULSEINDICATION_MENU_PULSE_LED:
        *menuStyle = settings.pulseLED;

        break;
#endif

    case PULSEINDICATION_MENU_DISPLAY_FLASH:
        *menuStyle = settings.pulseDisplayFlash;

        break;
    }

    return getString(pulsesIndicationMenuOptions[index]);
}

static void onPulsesIndicationMenuSelect(uint32_t index)
{
    switch (index)
    {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case PULSEINDICATION_MENU_SOUND:
        settings.pulseSound = !settings.pulseSound;

#if defined(PULSESOUND_ENABLE)
        updatePulseSoundEnable();
#endif

        break;
#endif

#if defined(VIBRATOR)
    case PULSEINDICATION_MENU_VIBRATION:
        settings.pulseVibration = !settings.pulseVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    case PULSEINDICATION_MENU_PULSE_LED:
        settings.pulseLED = !settings.pulseLED;

        updateLED();

        break;
#endif

    case PULSEINDICATION_MENU_DISPLAY_FLASH:
        settings.pulseDisplayFlash = !settings.pulseDisplayFlash;

        break;
    }
}

static MenuState pulsesIndicationMenuState;

static Menu pulsesIndicationMenu = {
    STRING_INDICATION,
    &pulsesIndicationMenuState,
    onPulsesIndicationMenuGetOption,
    onPulsesIndicationMenuSelect,
    setPulsesMenu,
};

static View pulsesIndicationMenuView = {
    onMenuEvent,
    &pulsesIndicationMenu,
};

// Pulses threshold menu

static const char *onPulsesThresholdMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseThreshold);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < RATE_NUM)
        return buildRateAlertMenuOption(index);
    else
        return NULL;
}

static void onPulsesThresholdMenuSelect(uint32_t index)
{
    settings.pulseThreshold = index;

    updatePulseThresholdExceeded();
#if defined(PULSESOUND_ENABLE)
    updatePulseSoundEnable();
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    updateLED();
#endif
}

static MenuState pulsesThresholdMenuState;

static Menu pulsesThresholdMenu = {
    STRING_THRESHOLD,
    &pulsesThresholdMenuState,
    onPulsesThresholdMenuGetOption,
    onPulsesThresholdMenuSelect,
    setPulsesMenu,
};

static View pulsesThresholdMenuView = {
    onMenuEvent,
    &pulsesThresholdMenu,
};

// Pulses menu

static MenuOption pulsesMenuOptions[] = {
    {STRING_INDICATION, &pulsesIndicationMenuView},
    {STRING_THRESHOLD, &pulsesThresholdMenuView},
    {NULL},
};

static const char *onPulsesMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(pulsesMenuOptions[index].title);
}

static void onPulsesMenuSelect(uint32_t index)
{
    setView(pulsesMenuOptions[index].view);
}

static MenuState pulsesMenuState;

static Menu pulsesMenu = {
    STRING_PULSES,
    &pulsesMenuState,
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    setSettingsMenu,
};

View pulsesMenuView = {
    onMenuEvent,
    &pulsesMenu,
};

static void setPulsesMenu(void)
{
    setView(&pulsesMenuView);
}

// Rate alerts

static const char *buildRateAlertMenuOption(uint32_t index)
{
    if (index == 0)
        return getString(STRING_OFF);
    else
    {
        const Unit *unit = &pulseUnits[settings.doseUnits].rate;
        char unitString[32];
        float value = rateAlerts[index] /
                      pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale;

        strclr(menuOption);
        strcpy(unitString, " ");

        buildValueString(menuOption, unitString, value, unit, doseUnitsMinMetricPrefix[settings.doseUnits]);
        strcat(menuOption, unitString);

        return menuOption;
    }
}

static const char *onRateAlertMenuGetOption(uint32_t index, MenuStyle *menuStyle)
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

    const Unit *unit = &pulseUnits[settings.doseUnits].dose;
    char unitString[32];
    float value = doseAlerts[index] / pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale;

    strclr(menuOption);
    strcpy(unitString, " ");

    buildValueString(menuOption, unitString, value, unit, doseUnitsMinMetricPrefix[settings.doseUnits]);
    strcat(menuOption, unitString);

    return menuOption;
}

static const char *onDoseAlertMenuGetOption(uint32_t index, MenuStyle *menuStyle)
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
    setAlertsMenu,
};

View rateWarningMenuView = {
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
    setAlertsMenu,
};

View rateAlarmMenuView = {
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
    setAlertsMenu,
};

View doseWarningMenuView = {
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
    setAlertsMenu,
};

View doseAlarmMenuView = {
    onMenuEvent,
    &doseAlarmMenu,
};

// Dose units menu

static cstring doseUnitsMenuOptions[] = {
    STRING_SIEVERT,
    STRING_REM,
    STRING_CPM,
    STRING_CPS,
    NULL,
};

static const char *onDoseUnitsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.doseUnits);

    return getString(doseUnitsMenuOptions[index]);
}

static void onDoseUnitsMenuSelect(uint32_t index)
{
    settings.doseUnits = index;
}

static MenuState doseUnitsMenuState;

static Menu doseUnitsMenu = {
    STRING_DOSE_UNITS,
    &doseUnitsMenuState,
    onDoseUnitsMenuGetOption,
    onDoseUnitsMenuSelect,
    setMeasurementsMenu,
};

View doseUnitsMenuView = {
    onMenuEvent,
    &doseUnitsMenu,
};

// Secondary dose units menu

static const char *onSecondaryDoseUnitsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.secondaryDoseUnits);

    return getString(doseUnitsMenuOptions[index]);
}

static void onSecondaryDoseUnitsMenuSelect(uint32_t index)
{
    settings.secondaryDoseUnits = index;
}

static MenuState secondaryUnitsMenuState;

static Menu secondaryDoseUnitsMenu = {
    STRING_SECONDARY_DOSE_UNITS,
    &secondaryUnitsMenuState,
    onSecondaryDoseUnitsMenuGetOption,
    onSecondaryDoseUnitsMenuSelect,
    setMeasurementsMenu,
};

View secondaryDoseUnitsMenuView = {
    onMenuEvent,
    &secondaryDoseUnitsMenu,
};
