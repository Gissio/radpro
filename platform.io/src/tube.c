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
#define CONVERSION_FACTOR_NUM 128

#define DUTY_CYCLE_NUM 101

#define DEAD_TIME_MIN 0.000040F
#define DEAD_TIME_MAX 0.000640F
#define DEAD_TIME_LOG_MAX_MIN 4.0F
#define DEAD_TIME_NUM 128

static const struct View tubeTypeMenuView;
static const struct View tubeCustomConversionFactorMenuView;
static const struct View tubeDeadTimeCompensationMenuView;
static const struct View tubeDutyCycleMenuView;

static const struct Menu tubeTubeTypeMenu;
static const struct Menu tubeCustomConversionFactorMenu;
static const struct Menu tubeDeadTimeCompensationMenu;
static const struct Menu tubeDutyCycleMenu;

void initTube(void)
{
    initTubeHardware();

    selectMenuIndex(&tubeTubeTypeMenu, settings.tubeType, TUBE_TYPE_NUM);
    selectMenuIndex(&tubeCustomConversionFactorMenu, settings.tubeConversionFactor, CONVERSION_FACTOR_NUM);
    selectMenuIndex(&tubeDutyCycleMenu, settings.tubeDutyCycle, DUTY_CYCLE_NUM);
    selectMenuIndex(&tubeDeadTimeCompensationMenu, settings.tubeDeadTimeCompensation, DEAD_TIME_NUM);
}

// Tube menu

static const char *const tubeMenuOptions[] = {
    "Conversion factor",
    "Dead-time compensation",
    "HV duty cycle",
    NULL,
};

static const struct View *tubeMenuOptionViews[] = {
    &tubeTypeMenuView,
    &tubeDeadTimeCompensationMenuView,
    &tubeDutyCycleMenuView,
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

// Tube duty cycle menu

static float getTubeDutyCycleFromIndex(uint32_t index)
{
    return 0.5F - 0.005F * index;
}

float getTubeDutyCycle(void)
{
    return getTubeDutyCycleFromIndex(settings.tubeDutyCycle);
}

static const char *onTubeDutyCycleMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (index < DUTY_CYCLE_NUM)
    {
        strcpy(menuOption, "");
        strcatFloat(menuOption, 100 * getTubeDutyCycleFromIndex(index), 1);
        strcat(menuOption, " %");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeDutyCycleMenuSelect(const struct Menu *menu)
{
    settings.tubeDutyCycle = menu->state->selectedIndex;

    updateTubeDutyCycle();
}

static struct MenuState tubeDutyCycleMenuState;

static const struct Menu tubeDutyCycleMenu = {
    "HV duty cycle",
    &tubeDutyCycleMenuState,
    onTubeDutyCycleMenuGetOption,
    NULL,
    onTubeDutyCycleMenuSelect,
    NULL,
    onTubeSubMenuBack,
};

static const struct View tubeDutyCycleMenuView = {
    onMenuEvent,
    &tubeDutyCycleMenu,
};
