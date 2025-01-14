/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "led.h"
#include "measurements.h"
#include "settings.h"
#include "tube.h"
#include "vibrator.h"

static const Menu tubeConversionFactorMenu;
static const Menu tubeDeadTimeCompensationMenu;
static const Menu tubeBackgroundCompensationMenu;
#if defined(TUBE_HV_PWM)
static const Menu tubeHVProfileMenu;
static const Menu tubeHVCustomProfileMenu;
static const Menu tubeHVDutyCycleMenu;
static const Menu tubeHVFrequencyMenu;
#endif

static const View tubeConversionFactorMenuView;
static const View tubeDeadTimeCompensationMenuView;
static const View tubeBackgroundCompensationMenuView;
#if defined(TUBE_HV_PWM)
static const View tubeHVProfileMenuView;
static const View tubeHVCustomProfileWarningView;
static const View tubeHVCustomProfileMenuView;
static const View tubeHVDutyCycleMenuView;
static const View tubeHVrequencyMenuView;
#endif

#if defined(TUBE_HV_PWM)
static float getTubeHVCustomProfileFrequency(uint32_t index);
static float getTubeHVCustomProfileDutyCycle(uint32_t index);

static struct
{
    float hvFrequency;
    float hvDutyCycle;
} tube;
#endif

void initTube(void)
{
    selectMenuItem(&tubeConversionFactorMenu,
                   settings.tubeConversionFactor,
                   TUBE_CONVERSIONFACTOR_NUM);
    selectMenuItem(&tubeDeadTimeCompensationMenu,
                   settings.tubeDeadTimeCompensation,
                   TUBE_DEADTIMECOMPENSATION_NUM);
    selectMenuItem(&tubeBackgroundCompensationMenu,
                   settings.tubeBackgroundCompensation,
                   TUBE_BACKGROUNDCOMPENSATION_NUM);

#if defined(TUBE_HV_PWM)
    tube.hvFrequency = getTubeHVCustomProfileFrequency(
        (settings.tubeHVFrequency >= TUBE_HVFREQUENCY_NUM)
            ? TUBE_HVFREQUENCY_NUM - 1
            : settings.tubeHVFrequency);
    tube.hvDutyCycle = getTubeHVCustomProfileDutyCycle(
        (settings.tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM)
            ? TUBE_HVDUTYCYCLE_NUM - 1
            : settings.tubeHVDutyCycle);

    selectMenuItem(&tubeHVProfileMenu,
                   settings.tubeHVProfile,
                   TUBE_HVPROFILE_NUM);
    selectMenuItem(&tubeHVFrequencyMenu,
                   settings.tubeHVFrequency,
                   TUBE_HVFREQUENCY_NUM);
    selectMenuItem(&tubeHVDutyCycleMenu,
                   settings.tubeHVDutyCycle,
                   TUBE_HVDUTYCYCLE_NUM);
#endif

    initTubeController();
}

// Tube menu

static const char *const tubeMenuOptions[] = {
    "Conversion factor",
    "Dead-time compensation",
    "Background compensation",
#if defined(TUBE_HV_PWM)
    "HV profile",
#endif
    NULL,
};

static const View *tubeMenuOptionViews[] = {
    &tubeConversionFactorMenuView,
    &tubeDeadTimeCompensationMenuView,
    &tubeBackgroundCompensationMenuView,
#if defined(TUBE_HV_PWM)
    &tubeHVProfileMenuView,
#endif
    NULL,
};

static const char *onTubeMenuGetOption(const Menu *menu,
                                       uint32_t index,
                                       MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return tubeMenuOptions[index];
}

static void onTubeMenuSelect(const Menu *menu)
{
    setView(tubeMenuOptionViews[menu->state->selectedIndex]);
}

void onTubeSubMenuBack(const Menu *menu)
{
    setView(&tubeMenuView);
}

static MenuState tubeMenuState;

static const Menu tubeMenu = {
    "Geiger tube",
    &tubeMenuState,
    onTubeMenuGetOption,
    onTubeMenuSelect,
    onSettingsSubMenuBack,
};

const View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};

// Custom conversion factor menu

static const float tubeConversionFactorMenuPresets[] = {
    153.8F,
    153.8F,
    60.0F,
    60.0F,
    153.8F,
    153.8F,
};

static const char *const tubeConversionFactorMenuOptions[] = {
    "J305",
    "J321",
    "J613",
    "J614",
    "M4011",
    "SBM-20",
};

static float getTubeConversionFactorForIndex(uint32_t index)
{
    if (index < TUBE_CONVERSIONFACTOR_PRESETS_NUM)
        return tubeConversionFactorMenuPresets[index];

    return TUBE_CONVERSIONFACTOR_VALUE_MIN *
           exp2f((index - TUBE_CONVERSIONFACTOR_PRESETS_NUM) *
                 (TUBE_CONVERSIONFACTOR_VALUE_LOG_MAX_MIN /
                  (TUBE_CONVERSIONFACTOR_VALUE_NUM - 1)));
}

float getTubeConversionFactor(void)
{
    return getTubeConversionFactorForIndex(settings.tubeConversionFactor);
}

static const char *onTubeConversionFactorMenuGetOption(const Menu *menu,
                                                       uint32_t index,
                                                       MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeConversionFactor);

    if (index < TUBE_CONVERSIONFACTOR_PRESETS_NUM)
    {
        strcpy(menuOption, tubeConversionFactorMenuOptions[index]);

        if (index == TUBE_CONVERSIONFACTOR_DEFAULT)
            strcat(menuOption, " (default)");

        return menuOption;
    }
    else if (index < TUBE_CONVERSIONFACTOR_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, getTubeConversionFactorForIndex(index), 1);
        strcat(menuOption, " cpm/\xb5Sv/h");
    }
    else
        return NULL;

    return menuOption;
}

static void onTubeConversionFactorMenuSelect(const Menu *menu)
{
    settings.tubeConversionFactor = menu->state->selectedIndex;

    updateMeasurementUnits();
}

static MenuState tubeConversionFactorMenuState;

static const Menu tubeConversionFactorMenu = {
    "Conversion factor",
    &tubeConversionFactorMenuState,
    onTubeConversionFactorMenuGetOption,
    onTubeConversionFactorMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeConversionFactorMenuView = {
    onMenuEvent,
    &tubeConversionFactorMenu,
};

// Tube dead-time compensation menu

static float getTubeDeadTimeCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return TUBE_DEADTIMECOMPENSATION_MIN * exp2f((index - 1) *
                                                 (TUBE_DEADTIMECOMPENSATION_LOG_MAX_MIN /
                                                  (TUBE_DEADTIMECOMPENSATION_NUM - 2)));
}

float getTubeDeadTimeCompensation(void)
{
    return getTubeDeadTimeCompensationFromIndex(settings.tubeDeadTimeCompensation);
}

static const char *onTubeDeadTimeCompensationMenuGetOption(const Menu *menu,
                                                           uint32_t index,
                                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeDeadTimeCompensation);

    if (index == 0)
        return "Off";
    else if (index < TUBE_DEADTIMECOMPENSATION_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 1000000 * getTubeDeadTimeCompensationFromIndex(index), 1);
        strcat(menuOption, " \xb5s");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeDeadTimeCompensationMenuSelect(const Menu *menu)
{
    settings.tubeDeadTimeCompensation = menu->state->selectedIndex;

    updateDeadTimeCompensation();
}

static MenuState tubeDeadTimeCompensationMenuState;

static const Menu tubeDeadTimeCompensationMenu = {
    "Dead-time compensation",
    &tubeDeadTimeCompensationMenuState,
    onTubeDeadTimeCompensationMenuGetOption,
    onTubeDeadTimeCompensationMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeDeadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

// Tube background compensation menu

static float getTubeBackgroundCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return 1E-9F * index / units[UNITS_SIEVERTS].rate.scale;
}

float getTubeBackgroundCompensation(void)
{
    return getTubeBackgroundCompensationFromIndex(settings.tubeBackgroundCompensation);
}

static const char *onTubeBackgroundCompensationMenuGetOption(const Menu *menu,
                                                             uint32_t index,
                                                             MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeBackgroundCompensation);

    if (index == 0)
        return "Off";
    else if (index < TUBE_BACKGROUNDCOMPENSATION_NUM)
    {
        Unit *rateUnit = &units[settings.units].rate;

        strclr(menuOption);
        strcatFloatAsMetricValueWithPrefix(menuOption,
                                           rateUnit->scale *
                                               getTubeBackgroundCompensationFromIndex(index),
                                           unitsMinMetricPrefixIndex[settings.units]);
        strcat(menuOption, rateUnit->name);

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeBackgroundCompensationMenuSelect(const Menu *menu)
{
    settings.tubeBackgroundCompensation = menu->state->selectedIndex;

    updateDeadTimeCompensation();
}

static MenuState tubeBackgroundCompensationMenuState;

static const Menu tubeBackgroundCompensationMenu = {
    "Background compensation",
    &tubeBackgroundCompensationMenuState,
    onTubeBackgroundCompensationMenuGetOption,
    onTubeBackgroundCompensationMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeBackgroundCompensationMenuView = {
    onMenuEvent,
    &tubeBackgroundCompensationMenu,
};

// Tube HV profile menu

#if defined(TUBE_HV_PWM)

static const char *const tubeHVProfileMenuOptions[] = {
    "Factory default",
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    "Accuracy",
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    "Energy-saving",
#endif
    "Custom",
    NULL,
};

static const char *onTubeHVProfileMenuGetOption(const Menu *menu,
                                                uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVProfile);

    if (index == TUBE_HVPROFILE_CUSTOM)
        *menuStyle |= MENUSTYLE_SUBMENU;

    return tubeHVProfileMenuOptions[index];
}

static void onTubeHVProfileMenuSelect(const Menu *menu)
{
    if (menu->state->selectedIndex < TUBE_HVPROFILE_CUSTOM)
    {
        settings.tubeHVProfile = menu->state->selectedIndex;

        updateTubeHV();
    }
    else
        setView(&tubeHVCustomProfileWarningView);
}

static MenuState tubeHVProfileMenuState;

static const Menu tubeHVProfileMenu = {
    "HV profile",
    &tubeHVProfileMenuState,
    onTubeHVProfileMenuGetOption,
    onTubeHVProfileMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeHVProfileMenuView = {
    onMenuEvent,
    &tubeHVProfileMenu,
};

static void onTubeHVGeneratorSubMenuBack(const Menu *menu)
{
    setView(&tubeHVProfileMenuView);
}

#endif

// Tube HV custom profile warning

#if defined(TUBE_HV_PWM)

static void onHVCustomProfileWarningEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        onTubeHVGeneratorSubMenuBack(NULL);

        break;

    case EVENT_KEY_SELECT:
        settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;

        updateTubeHV();

        setView(&tubeHVCustomProfileMenuView);

        break;

    case EVENT_DRAW:
        drawNotification("WARNING",
                         "Wrong values harm device.");

        break;

    default:
        break;
    }
}

static const View tubeHVCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

#endif

// Tube HV custom profile menu

#if defined(TUBE_HV_PWM)

static const char *const tubeHVCustomProfileMenuOptions[] = {
    "PWM frequency",
    "PWM duty cycle",
    NULL,
};

static const char *onTubeHVCustomProfileMenuGetOption(const Menu *menu,
                                                      uint32_t index,
                                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return tubeHVCustomProfileMenuOptions[index];
}

static void onTubeHVCustomProfileMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case 0:
        setView(&tubeHVrequencyMenuView);

        break;
    case 1:
        setView(&tubeHVDutyCycleMenuView);

        break;
    }
}

static MenuState tubeHVCustomProfileMenuState;

static const Menu tubeHVCustomProfileMenu = {
    "Custom",
    &tubeHVCustomProfileMenuState,
    onTubeHVCustomProfileMenuGetOption,
    onTubeHVCustomProfileMenuSelect,
    onTubeHVGeneratorSubMenuBack,
};

static const View tubeHVCustomProfileMenuView = {
    onMenuEvent,
    &tubeHVCustomProfileMenu,
};

static void onTubeHVCustomProfileSubMenuBack(const Menu *menu)
{
    setView(&tubeHVCustomProfileMenuView);
}

// Tube HV frequency menu

static const char *const tubeHVFrequencyMenuOptions[] = {
    "1.25 kHz",
    "2.5 kHz",
    "5 kHz",
    "10 kHz",
    "20 kHz",
    "40 kHz",
    NULL,
};

static float getTubeHVCustomProfileFrequency(uint32_t index)
{
    uint32_t value = 1250 << index;

    return value;
}

float getTubeHVFrequency(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
        return TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY;

#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_FREQUENCY;
#endif

#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY;
#endif

    case TUBE_HVPROFILE_CUSTOM:
        return tube.hvFrequency;

    default:
        return 1250;
    }
}

void setTubeHVFrequency(float value)
{
    if (value > 100000.0F)
        value = 100000.0F;
    else if (value < 100.0F)
        value = 100.0F;

    tube.hvFrequency = value;

    uint32_t tubeHVFrequency = log2f(value / 1250);
    if (tubeHVFrequency < 0)
        tubeHVFrequency = 0;
    else if (tubeHVFrequency >= TUBE_HVFREQUENCY_NUM)
        tubeHVFrequency = TUBE_HVFREQUENCY_NUM - 1;

    settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;
    settings.tubeHVFrequency = tubeHVFrequency;

    updateTubeHV();
}

static const char *onTubeHVFrequencyMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    return tubeHVFrequencyMenuOptions[index];
}

static void onTubeHVFrequencyMenuSelect(const Menu *menu)
{
    setTubeHVFrequency(
        getTubeHVCustomProfileFrequency(menu->state->selectedIndex));
}

static MenuState tubeHVFrequencyMenuState;

static const Menu tubeHVFrequencyMenu = {
    "PWM frequency",
    &tubeHVFrequencyMenuState,
    onTubeHVFrequencyMenuGetOption,
    onTubeHVFrequencyMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVrequencyMenuView = {
    onMenuEvent,
    &tubeHVFrequencyMenu,
};

#endif

// Tube HV duty cycle menu

#if defined(TUBE_HV_PWM)

static float getTubeHVCustomProfileDutyCycle(uint32_t index)
{
    float value = index + 1;

    return TUBE_HVDUTYCYCLE_MIN * value;
}

float getTubeHVDutyCycle(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
        return TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE;

#if defined(TUBE_HVPROFILE_ACCURACY_DUTYCYCLE)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_DUTYCYCLE;
#endif

#if defined(TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE;
#endif

    case TUBE_HVPROFILE_CUSTOM:
        return tube.hvDutyCycle;

    default:
        return 0;
    }
}

void setTubeHVDutyCycle(float value)
{
    if (value > 1)
        value = 1;
    else if (value < 0)
        value = 0;

    tube.hvDutyCycle = value;

    uint32_t tubeHVDutyCycle = (value - TUBE_HVDUTYCYCLE_MIN / 2) /
                               TUBE_HVDUTYCYCLE_STEP;
    if (tubeHVDutyCycle < 0)
        tubeHVDutyCycle = 0;
    else if (tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM)
        tubeHVDutyCycle = TUBE_HVDUTYCYCLE_NUM - 1;

    settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;
    settings.tubeHVDutyCycle = tubeHVDutyCycle;

    updateTubeHV();
}

static const char *onTubeHVDutyCycleMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVDutyCycle);

    if (index < TUBE_HVDUTYCYCLE_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 100 * getTubeHVCustomProfileDutyCycle(index), 2);
        strcat(menuOption, "%");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVDutyCycleMenuSelect(const Menu *menu)
{
    setTubeHVDutyCycle(
        getTubeHVCustomProfileDutyCycle(menu->state->selectedIndex));
}

static MenuState tubeHVDutyCycleMenuState;

static const Menu tubeHVDutyCycleMenu = {
    "PWM duty cycle",
    &tubeHVDutyCycleMenuState,
    onTubeHVDutyCycleMenuGetOption,
    onTubeHVDutyCycleMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVDutyCycleMenuView = {
    onMenuEvent,
    &tubeHVDutyCycleMenu,
};

#endif

// Pulses menu

enum
{
    PULSES_MENU_OPTIONS_PULSE_CLICKS,
#if defined(PULSE_LED)
    PULSES_MENU_OPTIONS_PULSE_LED,
#endif
    PULSES_MENU_OPTIONS_DISPLAY_FLASHES,
#if defined(VIBRATOR)
    PULSES_MENU_OPTIONS_HAPTIC_PULSES,
#endif
    PULSES_MENU_OPTIONS_PULSE_THRESHOLD,
};

static const char *const pulsesMenuOptions[] = {
    "Pulse clicks",
#if defined(PULSE_LED)
    "Pulse LED",
#endif
    "Display flashes",
#if defined(VIBRATOR)
    "Haptic pulses",
#endif
    "Threshold",
    NULL,
};

static const char *onPulsesMenuGetOption(const Menu *menu,
                                         uint32_t index,
                                         MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return pulsesMenuOptions[index];
}

static void onPulsesMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case PULSES_MENU_OPTIONS_PULSE_CLICKS:
        setView(&pulseClicksMenuView);

        break;

#if defined(PULSE_LED)
    case PULSES_MENU_OPTIONS_PULSE_LED:
        setView(&pulseLEDMenuView);

        break;
#endif

    case PULSES_MENU_OPTIONS_DISPLAY_FLASHES:
        setView(&displayFlashesMenuView);

        break;

#if defined(VIBRATOR)
    case PULSES_MENU_OPTIONS_HAPTIC_PULSES:
        setView(&pulseVibrationsMenuView);

        break;
#endif

    case PULSES_MENU_OPTIONS_PULSE_THRESHOLD:
        setView(&pulseThresholdMenuView);

        break;
    }
}

static MenuState pulsesMenuState;

static const Menu pulsesMenu = {
    "Pulses",
    &pulsesMenuState,
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    onSettingsSubMenuBack,
};

const View pulsesMenuView = {
    onMenuEvent,
    &pulsesMenu,
};

void onPulsesSubMenuBack(const Menu *menu)
{
    setView(&pulsesMenuView);
}
