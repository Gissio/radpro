/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "cmath.h"
#include "cstring.h"
#include "measurements.h"
#include "settings.h"
#include "tube.h"

#define CONVERSION_FACTOR_MIN 25.0F
#define CONVERSION_FACTOR_MAX 400.01F
#define CONVERSION_FACTOR_LOG_MAX_MIN 4.0F
#define CONVERSION_FACTOR_NUM 64

#define DEAD_TIME_MIN 0.000040F
#define DEAD_TIME_MAX 0.000640F
#define DEAD_TIME_LOG_MAX_MIN 4.0F
#define DEAD_TIME_NUM 64

static const struct View tubeTypeMenuView;
static const struct View tubeCustomConversionFactorMenuView;
static const struct View tubeDeadTimeCompensationMenuView;
static const struct View tubeHVDutyCycleMenuView;
static const struct View tubeHVFrequencyMenuView;

static const struct Menu tubeTubeTypeMenu;
static const struct Menu tubeCustomConversionFactorMenu;
static const struct Menu tubeDeadTimeCompensationMenu;
static const struct Menu tubeHVDutyCycleMenu;
static const struct Menu tubeHVFrequencyMenu;

void initTube(void)
{
    initTubeHardware();

    selectMenuIndex(&tubeTubeTypeMenu, settings.tubeType, TUBE_TYPE_NUM);
    selectMenuIndex(&tubeCustomConversionFactorMenu, settings.tubeConversionFactor, CONVERSION_FACTOR_NUM);
    selectMenuIndex(&tubeDeadTimeCompensationMenu, settings.tubeDeadTimeCompensation, DEAD_TIME_NUM);
    selectMenuIndex(&tubeHVDutyCycleMenu, settings.tubeHVDutyCycle, TUBE_HV_DUTY_CYCLE_NUM);
    selectMenuIndex(&tubeHVFrequencyMenu, settings.tubeHVFrequency, TUBE_HV_FREQUENCY_NUM);
}

// Tube menu

static const char *const tubeMenuOptions[] = {
    "Conversion factor",
    "Dead-time compensation",
    "HV duty cycle",
    "HV frequency",
    NULL,
};

static const struct View *tubeMenuOptionViews[] = {
    &tubeTypeMenuView,
    &tubeDeadTimeCompensationMenuView,
    &tubeHVDutyCycleMenuView,
    &tubeHVFrequencyMenuView,
};

static void onTubeMenuEnter(const struct Menu *menu)
{
    setView(tubeMenuOptionViews[menu->state->selectedIndex]);
}

static void onTubeSubMenuBack(const struct Menu *menu)
{
    setView(&tubeMenuView);
}

static struct MenuState tubeMenuState;

static const struct Menu tubeMenu = {
    "Geiger tube",
    &tubeMenuState,
    onMenuGetOption,
    tubeMenuOptions,
    NULL,
    onTubeMenuEnter,
    onSettingsSubMenuBack,
};

const struct View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};

// Tube conversion factor menu

static const float tubeConversionFactors[] = {
#if defined(TUBE_M4011)
    153.F,
#endif
#if defined(TUBE_J305)
    123.F,
#endif
#if defined(TUBE_HH614)
    68.4F,
#endif
};

static const char *const tubeTypeMenuOptions[] = {
#if defined(TUBE_M4011)
    "M4011 (153 cpm/\xb5Sv/h)",
#endif
#if defined(TUBE_J305)
    "J305 (123 cpm/\xb5Sv/h)",
#endif
#if defined(TUBE_HH614)
    "HH614 (68.4 cpm/\xb5Sv/h)",
#endif
    "Custom conversion factor",
    NULL,
};

static void onTubeTypeMenuSelect(const struct Menu *menu)
{
    settings.tubeType = menu->state->selectedIndex;

    updateTubeType();
}

static void onTubeTypeMenuEnter(const struct Menu *menu)
{
    if (menu->state->selectedIndex == TUBE_TYPE_CUSTOM)
        setView(&tubeCustomConversionFactorMenuView);
}

static void onTubeTypeSubMenuBack(const struct Menu *menu)
{
    setView(&tubeTypeMenuView);
}

static struct MenuState tubeTypeMenuState;

static const struct Menu tubeTubeTypeMenu = {
    "Conversion factor",
    &tubeTypeMenuState,
    onMenuGetOption,
    tubeTypeMenuOptions,
    onTubeTypeMenuSelect,
    onTubeTypeMenuEnter,
    onTubeSubMenuBack,
};

static const struct View tubeTypeMenuView = {
    onMenuEvent,
    &tubeTubeTypeMenu,
};

// Tube custom conversion factor menu

static float getTubeCustomConversionFactorFromIndex(uint32_t index)
{
    return CONVERSION_FACTOR_MIN * exp2f(index *
                                         (CONVERSION_FACTOR_LOG_MAX_MIN /
                                          (CONVERSION_FACTOR_NUM - 1)));
}

float getTubeCustomConversionFactor(void)
{
    if (settings.tubeType != TUBE_TYPE_CUSTOM)
        return tubeConversionFactors[settings.tubeType];
    else
        return getTubeCustomConversionFactorFromIndex(settings.tubeConversionFactor);
}

static const char *onTubeCustomConversionFactorMenuGetOption(const struct Menu *menu,
                                                             uint32_t index)
{
    if (index < CONVERSION_FACTOR_NUM)
    {
        strcpy(menuOption, "");
        strcatFloat(menuOption, getTubeCustomConversionFactorFromIndex(index), 1);
        strcat(menuOption, " cpm/\xb5Sv/h");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeCustomConversionFactorMenuSelect(const struct Menu *menu)
{
    settings.tubeConversionFactor = menu->state->selectedIndex;

    updateTubeType();
}

static struct MenuState tubeCustomConversionFactorMenuState;

static const struct Menu tubeCustomConversionFactorMenu = {
    "Custom conversion factor",
    &tubeCustomConversionFactorMenuState,
    onTubeCustomConversionFactorMenuGetOption,
    NULL,
    onTubeCustomConversionFactorMenuSelect,
    NULL,
    onTubeTypeSubMenuBack,
};

static const struct View tubeCustomConversionFactorMenuView = {
    onMenuEvent,
    &tubeCustomConversionFactorMenu,
};

// Tube dead-time compensation menu

static float getTubeDeadTimeCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return DEAD_TIME_MIN * exp2f((index - 1) *
                                 (DEAD_TIME_LOG_MAX_MIN /
                                  (DEAD_TIME_NUM - 2)));
}

float getTubeDeadTimeCompensation(void)
{
    return getTubeDeadTimeCompensationFromIndex(settings.tubeDeadTimeCompensation);
}

static const char *onTubeDeadTimeCompensationMenuGetOption(const struct Menu *menu,
                                                           uint32_t index)
{
    if (index == 0)
        return "Off";
    else if (index < DEAD_TIME_NUM)
    {
        strcpy(menuOption, "");
        strcatFloat(menuOption, 1000000 * getTubeDeadTimeCompensationFromIndex(index), 1);
        strcat(menuOption, " \xb5s");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeDeadTimeCompensationMenuSelect(const struct Menu *menu)
{
    settings.tubeDeadTimeCompensation = menu->state->selectedIndex;

    updateTubeType();
}

static struct MenuState tubeDeadTimeCompensationMenuState;

static const struct Menu tubeDeadTimeCompensationMenu = {
    "Dead-time compensation",
    &tubeDeadTimeCompensationMenuState,
    onTubeDeadTimeCompensationMenuGetOption,
    NULL,
    onTubeDeadTimeCompensationMenuSelect,
    NULL,
    onTubeSubMenuBack,
};

static const struct View tubeDeadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

// Tube HV duty cycle menu

static float getTubeHVDutyCycleFromIndex(uint32_t index)
{
    return 0.5F - 0.005F * index;
}

float getTubeHVDutyCycle(void)
{
    return getTubeHVDutyCycleFromIndex(settings.tubeHVDutyCycle);
}

static const char *onTubeHVDutyCycleMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (index < TUBE_HV_DUTY_CYCLE_NUM)
    {
        strcpy(menuOption, "");
        strcatFloat(menuOption, 100 * getTubeHVDutyCycleFromIndex(index), 1);
        strcat(menuOption, " %");

        if (index == 0)
            strcat(menuOption, " (default)");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVDutyCycleMenuSelect(const struct Menu *menu)
{
    settings.tubeHVDutyCycle = menu->state->selectedIndex;

    updateTubeHV();
}

static struct MenuState tubeHVDutyCycleMenuState;

static const struct Menu tubeHVDutyCycleMenu = {
    "HV duty cycle",
    &tubeHVDutyCycleMenuState,
    onTubeHVDutyCycleMenuGetOption,
    NULL,
    onTubeHVDutyCycleMenuSelect,
    NULL,
    onTubeSubMenuBack,
};

static const struct View tubeHVDutyCycleMenuView = {
    onMenuEvent,
    &tubeHVDutyCycleMenu,
};

// Tube HV frequency menu

static const char *const tubeHVFrequencyMenuOptions[] = {
    "1.25 kHz",
    "2.5 kHz",
    "5 kHz",
    "10 kHz",
    "20 kHz",
    "40 kHz (default)",
    NULL,
};

float getTubeHVFrequency(void)
{
    uint32_t frequency = 1250 << settings.tubeHVFrequency;

    return (float)frequency;
}

static const char *onTubeHVFrequencyMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (index < TUBE_HV_FREQUENCY_NUM)
    {
        strcpy(menuOption, tubeHVFrequencyMenuOptions[index]);

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVFrequencyMenuSelect(const struct Menu *menu)
{
    settings.tubeHVFrequency = menu->state->selectedIndex;

    updateTubeHV();
}

static struct MenuState tubeFrequencyMenuState;

static const struct Menu tubeHVFrequencyMenu = {
    "HV frequency",
    &tubeFrequencyMenuState,
    onTubeHVFrequencyMenuGetOption,
    NULL,
    onTubeHVFrequencyMenuSelect,
    NULL,
    onTubeSubMenuBack,
};

static const struct View tubeHVFrequencyMenuView = {
    onMenuEvent,
    &tubeHVFrequencyMenu,
};
