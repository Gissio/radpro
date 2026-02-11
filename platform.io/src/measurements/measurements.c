/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <float.h>

#include "../devices/display.h"
#include "../devices/emf.h"
#include "../devices/keyboard.h"
#include "../devices/led.h"
#include "../devices/pulsesoundenable.h"
#include "../devices/tube.h"
#include "../measurements/average.h"
#include "../measurements/cumulative.h"
#include "../measurements/electricfield.h"
#include "../measurements/history.h"
#include "../measurements/instantaneous.h"
#include "../measurements/magneticfield.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"

typedef enum
{
    MEASUREMENTS_VIEW_INSTANTANTANEOUS_RATE,
    MEASUREMENTS_VIEW_AVERAGE_RATE,
    MEASUREMENTS_VIEW_CUMULATIVE_DOSE,
#if defined(EMFMETER)
    MEASUREMENTS_VIEW_ELECTRIC_FIELD,
    MEASUREMENTS_VIEW_MAGNETIC_FIELD,
#endif
    MEASUREMENTS_VIEW_HISTORY,

    MEASUREMENTS_VIEW_NUM,
} AverageTab;

static struct
{
    bool enabled;

    int32_t viewIndex;

    AlertLevel alertLevel;
    bool alertPending;
    bool alertFlashing;
    bool soundIconActive;
} measurements;

static Menu alertsMenu;
static Menu alertIndicationMenu;

static Menu measurementsMenu;

// Measurements

static ViewPointer measurementViews[] = {
    &instantaneousRateView,
    &averageRateView,
    &cumulativeDoseView,
#if defined(EMFMETER)
    &electricFieldView,
    &magneticFieldView,
#endif
    &historyView,
};

static void setMeasurementView(int32_t index);

void setupMeasurements(void)
{
    measurements.viewIndex = MEASUREMENTS_VIEW_INSTANTANTANEOUS_RATE;

    setupPulses();

#if defined(EMFMETER)
    setupElectricField();
    setupMagneticField();
#endif

    selectMenuItem(&alertsMenu, 0, 0);
    selectMenuItem(&alertIndicationMenu, 0, 0);

    selectMenuItem(&measurementsMenu, 0, 0);
}

void setMeasurementsEnabled(bool value)
{
    measurements.enabled = value;
}

bool isMeasurementsEnabled(void)
{
    return measurements.enabled;
}

void updateMeasurements(void)
{
    if (!measurements.enabled)
        return;

    // Measurements
    updatePulses();
#if defined(EMFMETER)
    updateEMFMeter();
#endif

    // Alerts
    AlertLevel alertLevel;
    AlertLevel alertLevelSource;

    alertLevelSource = getTubeFaultAlertLevel();

    alertLevel = getInstantaneousRateAlertLevel();
    if (alertLevelSource > alertLevel)
        alertLevel = alertLevelSource;

    alertLevelSource = getCumulativeDoseAlertLevel();
    if (alertLevelSource > alertLevel)
        alertLevel = alertLevelSource;

#if defined(EMFMETER)
    alertLevelSource = getElectricFieldAlertLevel();
    if (alertLevelSource > alertLevel)
        alertLevel = alertLevelSource;

    alertLevelSource = getMagneticFieldAlertLevel();
    if (alertLevelSource > alertLevel)
        alertLevel = alertLevelSource;
#endif

    if (alertLevel)
    {
        if (measurements.alertPending)
            triggerAlert(alertLevel == ALERTLEVEL_ALARM);

        if (!measurements.alertLevel)
            measurements.alertFlashing = true;
        else
            measurements.alertFlashing = !measurements.alertFlashing;
    }
    else
    {
        measurements.alertPending = false;
        measurements.alertFlashing = false;
    }
    measurements.alertLevel = alertLevel;

    // Sound control
#if defined(PULSESOUND_ENABLE)
    updatePulseSoundEnable();
#endif

    // LED
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    updateLED();
#endif

    // Set view
    if (!isDisplayEnabled() && measurements.alertPending)
    {
        if (getInstantaneousRateAlertLevel())
            setMeasurementView(MEASUREMENTS_VIEW_INSTANTANTANEOUS_RATE);
        else if (getCumulativeDoseAlertLevel())
            setMeasurementView(MEASUREMENTS_VIEW_CUMULATIVE_DOSE);
#if defined(EMFMETER)
        else if (getElectricFieldAlertLevel())
            setMeasurementView(MEASUREMENTS_VIEW_ELECTRIC_FIELD);
        else if (getMagneticFieldAlertLevel())
            setMeasurementView(MEASUREMENTS_VIEW_MAGNETIC_FIELD);
#endif
    }
}

bool isAlertEnabled(void)
{
    return (settings.rateAlarm ||
            settings.doseAlarm ||
            settings.rateWarning ||
            settings.doseWarning
#if defined(EMFMETER)
            ||
            settings.electricFieldAlarm ||
            settings.magneticFieldAlarm
#endif
    );
}

AlertLevel getAlertLevel(void)
{
    return measurements.alertLevel;
}

void setAlertPending(bool value)
{
    measurements.alertPending = value;
}

bool isAlertPending(void)
{
    return measurements.alertPending;
}

bool isAlertFlashing(void)
{
    return measurements.alertFlashing;
}

bool isSoundIconActive(void)
{
    return measurements.soundIconActive;
}

// Measurement views common

void buildValueString(char *valueString, char *unitString, float value, const Unit *unit, int32_t minMetricPrefixIndex)
{
    if (!unit->name[0] && (value < 10000.0F))
    {
        uint32_t intValue = (uint32_t)value;

        strcatUInt32(valueString, intValue, 0);
        strcpy(unitString, " ");
        if (intValue == 1)
            strcat(unitString, getString(STRING_COUNT));
        else
            strcat(unitString, getString(STRING_COUNTS));
    }
    else
    {
        strcatMetricValue(valueString, unitString, value * unit->scale, minMetricPrefixIndex);

        strcat(unitString, unit->name[0] ? unit->name : getString(STRING_COUNTS));
    }
}

static void setMeasurementView(int32_t index)
{
    if (index >= 0)
        measurements.viewIndex = index;

    setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);

    setView(measurementViews[measurements.viewIndex]);
}

void setMeasurementViewCurrent(void)
{
    setMeasurementView(-1);
}

bool onMeasurementViewEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_SELECT:
        if (!isInLockMode())
        {
            measurements.soundIconActive = false;

            setKeyboardMode(KEYBOARD_MODE_MENU);

            setSettingsMenu();
        }

        break;

    case EVENT_KEY_UP:
        measurements.viewIndex--;
        if (measurements.viewIndex < 0)
            measurements.viewIndex = MEASUREMENTS_VIEW_NUM - 1;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_KEY_DOWN:
        measurements.viewIndex++;
        if (measurements.viewIndex >= MEASUREMENTS_VIEW_NUM)
            measurements.viewIndex = 0;

        setMeasurementView(measurements.viewIndex);

        break;

    case EVENT_KEY_RESET:
        if (measurements.alertPending)
            measurements.alertPending = false;
        else
            return isInLockMode();

        break;

    case EVENT_KEY_TOGGLEPULSESOUND:
        measurements.soundIconActive = true;

        settings.pulseSound = !settings.pulseSound;

#if defined(PULSESOUND_ENABLE)
        updatePulseSoundEnable();
#endif

        triggerVibration();

        break;

    default:
        return false;
    }

    return true;
}

// Alert indication menu

enum
{
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    ALERTINDICATION_MENU_SOUND,
#endif
#if defined(VOICE)
    ALERTINDICATION_MENU_VOICE,
#endif
#if defined(VIBRATOR)
    ALERTINDICATION_MENU_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    ALERTINDICATION_MENU_PULSELED,
#endif
    ALERTINDICATION_MENU_DISPLAYFLASH,
};

static cstring alertIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    STRING_SOUND,
#endif
#if defined(VOICE)
    STRING_VOICE,
#endif
#if defined(VIBRATOR)
    STRING_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    STRING_PULSE_LED,
#endif
    STRING_DISPLAY_FLASH,
    NULL,
};

static const char *onAlertIndicationMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case ALERTINDICATION_MENU_SOUND:
        *menuStyle = settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        *menuStyle = settings.alertVoice;

        break;
#endif

#if defined(VIBRATOR)
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
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case ALERTINDICATION_MENU_SOUND:
        settings.alertSound = !settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        settings.alertVoice = !settings.alertVoice;

        break;
#endif

#if defined(VIBRATOR)
    case ALERTINDICATION_MENU_VIBRATION:
        settings.alertVibration = !settings.alertVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    case ALERTINDICATION_MENU_PULSELED:
        settings.alertPulseLED = !settings.alertPulseLED;

        updateLED();

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
    setAlertsMenu,
};

static View alertIndicationMenuView = {
    onMenuEvent,
    &alertIndicationMenu,
};

// Alerts menu

static MenuOption alertsMenuOptions[] = {
    {STRING_INDICATION, &alertIndicationMenuView},
    {STRING_RATE_WARNING, &rateWarningMenuView},
    {STRING_RATE_ALARM, &rateAlarmMenuView},
    {STRING_DOSE_WARNING, &doseWarningMenuView},
    {STRING_DOSE_ALARM, &doseAlarmMenuView},
#if defined(EMFMETER)
    {STRING_ELECTRIC_FIELD_ALARM, &electricFieldAlarmMenuView},
    {STRING_MAGNETIC_FIELD_ALARM, &magneticFieldAlarmMenuView},
#endif
    {NULL},
};

static const char *onAlertsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
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
    setSettingsMenu,
};

View alertsMenuView = {
    onMenuEvent,
    &alertsMenu,
};

void setAlertsMenu(void)
{
    setView(&alertsMenuView);
}

// Measurements menu

static MenuOption measurementsMenuOptions[] = {
    {STRING_SOURCE, &sourceMenuView},
    {STRING_DOSE_UNITS, &doseUnitsMenuView},
    {STRING_SECONDARY_DOSE_UNITS, &secondaryDoseUnitsMenuView},
#if defined(EMFMETER)
    {STRING_MAGNETIC_FIELD_UNITS, &magneticFieldUnitsMenuView},
#endif
    {STRING_INSTANTANEOUS, &instantaneousMenuView},
    {STRING_AVERAGE, &averageMenuView},
    {NULL},
};

static const char *onMeasurementsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(measurementsMenuOptions[index].title);
}

static void onMeasurementsMenuSelect(uint32_t index)
{
    setView(measurementsMenuOptions[index].view);
}

static MenuState measurementsMenuState;

static Menu measurementsMenu = {
    STRING_MEASUREMENTS,
    &measurementsMenuState,
    onMeasurementsMenuGetOption,
    onMeasurementsMenuSelect,
    setSettingsMenu,
};

View measurementsMenuView = {
    onMenuEvent,
    &measurementsMenu,
};

void setMeasurementsMenu(void)
{
    setView(&measurementsMenuView);
}
