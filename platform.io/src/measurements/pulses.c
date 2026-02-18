/*
 * Rad Pro
 * Pulses
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/average.h"
#include "../measurements/cumulative.h"
#include "../measurements/history.h"
#include "../measurements/instantaneous.h"
#include "../peripherals/led.h"
#include "../peripherals/pulsesoundenable.h"
#include "../peripherals/tube.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"

#define PULSE_INDICATION_SENSITIVITY_MAX 600.0F
#define PULSE_INDICATION_FACTOR_UNIT 0x10000
#define PULSE_INDICATION_FACTOR_MASK (PULSE_INDICATION_FACTOR_UNIT - 1)

static const Menu pulsesMenu;
static const Menu pulsesIndicationMenu;
static const Menu pulsesThresholdMenu;

static const char *buildRateAlertMenuOption(menu_size_t index);

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
    AlertLevel faultAlertLevel;
    bool faultAlertTriggered;

    float deadTimeCompensationRemainder;

    // Selected menu
    menu_size_t alertsMenuIndex;
} pulses;

void setupPulses(void)
{
    syncTick();
    Dose tubeDose = pulses.tubeDose;
    memset(&pulses, 0, sizeof(pulses));
    pulses.lastTubePulseCount = tubePulseCount;
    pulses.tubeDose = tubeDose;

    updateDoseUnits();

    setupInstantaneousRate();
    setupAverageRate();
    setupCumulativeDose();
    setupHistory();

    selectMenuItem(&pulsesMenu, 0);
    selectMenuItem(&pulsesIndicationMenu, 0);
    selectMenuItem(&pulsesThresholdMenu, settings.pulseThreshold);

    selectMenuItem(&rateWarningMenu, settings.rateWarning);
    selectMenuItem(&doseWarningMenu, settings.doseWarning);
    selectMenuItem(&rateAlarmMenu, settings.rateAlarm);
    selectMenuItem(&doseAlarmMenu, settings.doseAlarm);

    selectMenuItem(&doseUnitsMenu, settings.doseUnits);
    selectMenuItem(&secondaryDoseUnitsMenu, settings.secondaryDoseUnits);
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
    float rateSievertsPerHour = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * getInstantaneousRate();

    pulses.rateOverThreshold = settings.pulseThreshold ? rateSievertsPerHour >= rateAlerts[settings.pulseThreshold] : true;
}

bool isPulseThresholdExceeded(void)
{
    return pulses.rateOverThreshold;
}

void onPulseTick(void)
{
    // Measurement
#if defined(SIMULATOR)
    onTubeTick();
#endif

    uint32_t currentPulseCount = tubePulseCount;
    uint32_t pulseCount = currentPulseCount - pulses.lastTubePulseCount;
    pulses.lastTubePulseCount = currentPulseCount;

    // Tube dose
    pulses.tubeDose.pulseCount += pulseCount;

    // Current period
    if (!pulses.currentPeriod.pulseCount)
        pulses.currentPeriod.firstTick = currentTick;
    pulses.currentPeriod.lastTick = currentTick;
    pulses.currentPeriod.pulseCount += pulseCount;

    // Pulse indication
    if (pulseCount && isMeasurementsEnabled())
    {
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
    // Fault alert
    AlertLevel faultAlertLevel = ALERTLEVEL_NONE;
    if (pulses.lastPeriod.pulseCount)
        pulses.lossOfCountTimer = 0;
    else
        pulses.lossOfCountTimer++;
    if (pulses.lossOfCountTimer >= getLossOfCountTime())
        faultAlertLevel = ALERTLEVEL_ALARM;

    bool tubeShorted = (!pulses.lastPeriod.pulseCount && readTubeDet());
    if (pulses.lastTubeShorted && tubeShorted)
        faultAlertLevel = ALERTLEVEL_ALARM;
    pulses.lastTubeShorted = tubeShorted;

    pulses.faultAlertTriggered = (faultAlertLevel > pulses.faultAlertLevel);
    pulses.faultAlertLevel = faultAlertLevel;
    if (pulses.faultAlertTriggered)
        setAlertPending(true);

    // Instantaneous rate
    updateInstantaneousRate(pulses.lastPeriodTick, &pulses.lastPeriod);

    // Compensate period
    PulsePeriod compensatedPeriod = pulses.lastPeriod;
    if (settings.tubeDeadTimeCompensation)
    {
        pulses.deadTimeCompensationRemainder += getDeadTimeCompensationFactor() * pulses.lastPeriod.pulseCount;
        compensatedPeriod.pulseCount = (uint32_t)pulses.deadTimeCompensationRemainder;
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
    return pulses.faultAlertLevel;
}

bool isTubeFaultAlertTriggered(void)
{
    return pulses.faultAlertTriggered;
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
};

static const char *onPulsesIndicationMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
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

static void onPulsesIndicationMenuSelect(menu_size_t index)
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

static const Menu pulsesIndicationMenu = {
    STRING_INDICATION,
    &pulsesIndicationMenuState,
    ARRAY_SIZE(pulsesIndicationMenuOptions),
    onPulsesIndicationMenuGetOption,
    onPulsesIndicationMenuSelect,
    showPulsesMenu,
};

// Pulses threshold menu

static const char *onPulsesThresholdMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseThreshold);

    return buildRateAlertMenuOption(index);
}

static void onPulsesThresholdMenuSelect(menu_size_t index)
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

static const Menu pulsesThresholdMenu = {
    STRING_THRESHOLD,
    &pulsesThresholdMenuState,
    ARRAY_SIZE(rateAlerts),
    onPulsesThresholdMenuGetOption,
    onPulsesThresholdMenuSelect,
    showPulsesMenu,
};

// Pulses menu

static const MenuOption pulsesMenuOptions[] = {
    {STRING_INDICATION, &pulsesIndicationMenu},
    {STRING_THRESHOLD, &pulsesThresholdMenu},
};

static const char *onPulsesMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(pulsesMenuOptions[index].title);
}

static void onPulsesMenuSelect(menu_size_t index)
{
    showMenu(pulsesMenuOptions[index].menu);
}

static MenuState pulsesMenuState;

static const Menu pulsesMenu = {
    STRING_PULSES,
    &pulsesMenuState,
    ARRAY_SIZE(pulsesMenuOptions),
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    showSettingsMenu,
};

void showPulsesMenu(void)
{
    showMenu(&pulsesMenu);
}

// Alerts

static const char *buildAlertMenuOption(menu_size_t index, const float *alerts, const Unit *unit, float scale)
{
    if (index == 0)
        return getString(STRING_OFF);

    char unitString[32];
    float value = alerts[index] / scale;

    strclr(menuOption);
    strcpy(unitString, " ");
    buildValueString(menuOption, unitString, value, unit, doseUnitsMinMetricPrefix[settings.doseUnits]);
    strcat(menuOption, unitString);

    return menuOption;
}

// Rate alerts

static const char *buildRateAlertMenuOption(menu_size_t index)
{
    return buildAlertMenuOption(index, rateAlerts, &pulseUnits[settings.doseUnits].rate, pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale);
}

// Dose alerts

static const char *buildDoseAlertMenuOption(menu_size_t index)
{
    return buildAlertMenuOption(index, doseAlerts, &pulseUnits[settings.doseUnits].dose, pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale);
}

// Rate warning menu

static const char *onRateWarningMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rateWarning);

    return buildRateAlertMenuOption(index);
}

static void onRateWarningMenuSelect(menu_size_t index)
{
    settings.rateWarning = index;
}

static MenuState rateWarningMenuState;

const Menu rateWarningMenu = {
    STRING_RATE_WARNING,
    &rateWarningMenuState,
    ARRAY_SIZE(rateAlerts),
    onRateWarningMenuGetOption,
    onRateWarningMenuSelect,
    showAlertsMenu,
};

// Rate alarm menu

static const char *onRateAlarmMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rateAlarm);

    return buildRateAlertMenuOption(index);
}

static void onRateAlarmMenuSelect(menu_size_t index)
{
    settings.rateAlarm = index;
}

static MenuState rateAlarmMenuState;

const Menu rateAlarmMenu = {
    STRING_RATE_ALARM,
    &rateAlarmMenuState,
    ARRAY_SIZE(rateAlerts),
    onRateAlarmMenuGetOption,
    onRateAlarmMenuSelect,
    showAlertsMenu,
};

// Dose warning menu

static const char *onDoseWarningMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.doseWarning);

    return buildDoseAlertMenuOption(index);
}

static void onDoseWarningMenuSelect(menu_size_t index)
{
    settings.doseWarning = index;
}

static MenuState doseWarningMenuState;

const Menu doseWarningMenu = {
    STRING_DOSE_WARNING,
    &doseWarningMenuState,
    ARRAY_SIZE(doseAlerts),
    onDoseWarningMenuGetOption,
    onDoseWarningMenuSelect,
    showAlertsMenu,
};

// Dose alarm menu

static const char *onDoseAlarmMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.doseAlarm);

    return buildDoseAlertMenuOption(index);
}

static void onDoseAlarmMenuSelect(menu_size_t index)
{
    settings.doseAlarm = index;
}

static MenuState doseAlarmMenuState;

const Menu doseAlarmMenu = {
    STRING_DOSE_ALARM,
    &doseAlarmMenuState,
    ARRAY_SIZE(doseAlerts),
    onDoseAlarmMenuGetOption,
    onDoseAlarmMenuSelect,
    showAlertsMenu,
};

// Dose units menu

static cstring doseUnitsMenuOptions[] = {
    STRING_SIEVERT,
    STRING_REM,
    STRING_CPM,
    STRING_CPS,
};

static const char *onDoseUnitsMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.doseUnits);

    return getString(doseUnitsMenuOptions[index]);
}

static void onDoseUnitsMenuSelect(menu_size_t index)
{
    settings.doseUnits = index;
}

static MenuState doseUnitsMenuState;

const Menu doseUnitsMenu = {
    STRING_DOSE_UNITS,
    &doseUnitsMenuState,
    ARRAY_SIZE(doseUnitsMenuOptions),
    onDoseUnitsMenuGetOption,
    onDoseUnitsMenuSelect,
    showMeasurementsMenu,
};

// Secondary dose units menu

static const char *onSecondaryDoseUnitsMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.secondaryDoseUnits);

    return getString(doseUnitsMenuOptions[index]);
}

static void onSecondaryDoseUnitsMenuSelect(menu_size_t index)
{
    settings.secondaryDoseUnits = index;
}

static MenuState secondaryUnitsMenuState;

const Menu secondaryDoseUnitsMenu = {
    STRING_SECONDARY_DOSE_UNITS,
    &secondaryUnitsMenuState,
    ARRAY_SIZE(doseUnitsMenuOptions),
    onSecondaryDoseUnitsMenuGetOption,
    onSecondaryDoseUnitsMenuSelect,
    showMeasurementsMenu,
};
