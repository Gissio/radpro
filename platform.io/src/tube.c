/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "measurements.h"
#include "settings.h"
#include "tube.h"

#define CONVERSIONFACTOR_MIN 25.0F
#define CONVERSIONFACTOR_MAX 400.01F
#define CONVERSIONFACTOR_LOG_MAX_MIN 4.0F
#define CONVERSIONFACTOR_NUM 64

#define DEADTIME_MIN 0.000040F
#define DEADTIME_MAX 0.000640F
#define DEADTIME_LOG_MAX_MIN 4.0F
#define DEADTIME_NUM 64

static const Menu tubeConversionFactorMenu;
static const Menu tubeDeadTimeCompensationMenu;
static const Menu tubeHVProfileMenu;
static const Menu tubeHVCustomProfileMenu;
static const Menu tubeHVDutyCycleMenu;
static const Menu tubeHVFrequencyMenu;

static const View tubeConversionFactorMenuView;
static const View tubeDeadTimeCompensationMenuView;
static const View tubeHVProfileMenuView;
static const View tubeHVCustomProfileWarningView;
static const View tubeHVCustomProfileMenuView;
static const View tubeHVDutyCycleMenuView;
static const View tubeHVrequencyMenuView;

void initTube(void)
{
    initTubeController();

    selectMenuItem(&tubeConversionFactorMenu,
                   settings.tubeConversionFactor,
                   CONVERSIONFACTOR_NUM);
    selectMenuItem(&tubeDeadTimeCompensationMenu,
                   settings.tubeDeadTimeCompensation,
                   DEADTIME_NUM);
    selectMenuItem(&tubeHVProfileMenu,
                   settings.tubeHVProfile,
                   TUBE_HVPROFILE_NUM);
    selectMenuItem(&tubeHVFrequencyMenu,
                   settings.tubeHVFrequency,
                   TUBE_HVFREQUENCY_NUM);
    selectMenuItem(&tubeHVDutyCycleMenu,
                   settings.tubeHVDutyCycle,
                   TUBE_HVDUTYCYCLE_NUM);
}

// Tube menu

static const char *const tubeMenuOptions[] = {
    "Conversion factor",
#if defined(DISPLAY_320X240)
    "Dead-time compensation",
#else
    "Dead-time comp.",
#endif
    "HV profile",
    NULL,
};

static const View *tubeMenuOptionViews[] = {
    &tubeConversionFactorMenuView,
    &tubeDeadTimeCompensationMenuView,
    &tubeHVProfileMenuView,
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

static void onTubeSubMenuBack(const Menu *menu)
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

static const float tubeConversionFactorMenuValues[] = {
    175.0F,
    153.0F,
    68.4F,
    68.4F,
    153.0F,
    175.0F,
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
    if (index < TUBE_CONVERSIONFACTOR_NUM)
        return tubeConversionFactorMenuValues[index];

    index -= TUBE_CONVERSIONFACTOR_NUM;

    return CONVERSIONFACTOR_MIN *
           exp2f(index *
                 (CONVERSIONFACTOR_LOG_MAX_MIN /
                  (CONVERSIONFACTOR_NUM - 1)));
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

    if (index < TUBE_CONVERSIONFACTOR_NUM)
    {
        strcpy(menuOption, tubeConversionFactorMenuOptions[index]);

        if (index == TUBE_CONVERSIONFACTOR_DEFAULT)
            strcat(menuOption, " (default)");

        return menuOption;
    }
    else if (index < (TUBE_CONVERSIONFACTOR_NUM + CONVERSIONFACTOR_NUM))
    {
        strcpy(menuOption, "");
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

    return DEADTIME_MIN * exp2f((index - 1) *
                                (DEADTIME_LOG_MAX_MIN /
                                 (DEADTIME_NUM - 2)));
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
    else if (index < DEADTIME_NUM)
    {
        strcpy(menuOption, "");
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

    updateMeasurementUnits();
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

// Tube HV profile menu

static const char *const tubeHVProfileMenuOptions[] = {
#if !defined(TUBE_HVPROFILE_FACTORYDEFAULT2_FREQUENCY)
    "Factory default",
#elif defined(CH32)
    "Factory default (WCH)",
    "Factory default (Geehy)",
#elif defined(APM32)
    "Factory default (Geehy)",
    "Factory default (WCH)",
#endif
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

// Tube HV custom profile warning

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
                         "Wrong values may harm device.",
                         false);

        break;

    default:
        break;
    }
}

static const View tubeHVCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

// Tube HV custom profile menu

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

float getTubeHVCustomProfileFrequency(uint32_t index)
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

#if defined(TUBE_HVPROFILE_FACTORYDEFAULT2_FREQUENCY)
    case TUBE_HVPROFILE_FACTORYDEFAULT2:
        return TUBE_HVPROFILE_FACTORYDEFAULT2_FREQUENCY;
#endif
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_FREQUENCY;
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY;
#endif
    case TUBE_HVPROFILE_CUSTOM:
        return getTubeHVCustomProfileFrequency(settings.tubeHVFrequency);

    default:
        return 1250;
    }
}

static const char *onTubeHVFrequencyMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    if (index < TUBE_HVFREQUENCY_NUM)
    {
        strcpy(menuOption, tubeHVFrequencyMenuOptions[index]);

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVFrequencyMenuSelect(const Menu *menu)
{
    settings.tubeHVFrequency = menu->state->selectedIndex;

    updateTubeHV();
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

// Tube HV duty cycle menu

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

#if defined(TUBE_HVPROFILE_FACTORYDEFAULT2_DUTYCYCLE)
    case TUBE_HVPROFILE_FACTORYDEFAULT2:
        return TUBE_HVPROFILE_FACTORYDEFAULT2_DUTYCYCLE;
#endif
#if defined(TUBE_HVPROFILE_ACCURACY_DUTYCYCLE)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_DUTYCYCLE;
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE;
#endif
    case TUBE_HVPROFILE_CUSTOM:
        return getTubeHVCustomProfileDutyCycle(settings.tubeHVDutyCycle);

    default:
        return 0;
    }
}

static const char *onTubeHVDutyCycleMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVDutyCycle);

    if (index < TUBE_HVDUTYCYCLE_NUM)
    {
        strcpy(menuOption, " ");
        strcatFloat(menuOption, 100 * getTubeHVCustomProfileDutyCycle(index), 2);
        strcat(menuOption, " %");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVDutyCycleMenuSelect(const Menu *menu)
{
    settings.tubeHVDutyCycle = menu->state->selectedIndex;

    updateTubeHV();
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
