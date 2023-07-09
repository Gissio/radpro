/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>

#include "battery.h"
#include "cmath.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "measurements.h"
#include "rng.h"
#include "settings.h"
#include "stats.h"

const char *const firmwareName = "Rad Pro";
const char *const firmwareVersion = "1.3.0";

struct Settings settings;
struct LifeState lifeState;
struct DoseState doseState;

void initSettings(void)
{
    settings.pulseClicks = PULSE_CLICKS_QUIET;
    settings.backlight = BACKLIGHT_10S;

    initFlash();

    uint32_t address;

    address = getLastEntry(flashSettingsStart, flashSettingsEnd, sizeof(struct Settings));
    if (address)
        settings = *(struct Settings *)getFlashMemory(address);

    address = getLastEntry(flashLifeStateStart, flashLifeStateEnd, sizeof(struct LifeState));
    if (address)
        lifeState = *(struct LifeState *)getFlashMemory(address);

    address = getLastEntry(flashDoseStateStart, flashDoseStateEnd, sizeof(struct DoseState));
    if (address)
        doseState = *(struct DoseState *)getFlashMemory(address);

    resetDataLoggingTimer();

    setDose(doseState.time, doseState.pulseCount);
}

float getConversionFactor(uint32_t index)
{
    return CONVERSION_FACTOR_MIN * exp2fApprox(index *
                                               (CONVERSION_FACTOR_LOG_MAX_MIN /
                                                (CONVERSION_FACTOR_STEPS - 1)));
}

// Settings storage

void writeSettings(void)
{
    flashEntry(flashSettingsStart, flashSettingsEnd, sizeof(struct Settings), (uint32_t *)&settings);
}

void writeLifeState(void)
{
    flashEntry(flashLifeStateStart, flashLifeStateEnd, sizeof(struct LifeState), (uint32_t *)&lifeState);
}

void writeDoseState(void)
{
    getDose(&doseState.time, &doseState.pulseCount);

    flashEntry(flashDoseStateStart, flashDoseStateEnd, sizeof(struct DoseState), (uint32_t *)&doseState);
}

// Settings Menu

const char *const settingsMenuOptions[] = {
    "Units",
    "History",
    "Rate alarm",
    "Dose alarm",
    "Pulse clicks",
    "Backlight",
    "Battery type",
    "Geiger tube type",
    "Data logging",
    "Random number generator",
    "Statistics",
    "Game",
    NULL,
};

const struct View *settingsMenuOptionViews[] = {
    &unitsMenuView,
    &historyMenuView,
    &rateAlarmMenuView,
    &doseAlarmMenuView,
    &pulseClicksMenuView,
    &backlightMenuView,
    &batteryTypeMenuView,
    &tubeTypeMenuView,
    &dataLoggingMenuView,
    &rngMenuView,
    &statsView,
    &gameMenuView,
};

static void onSettingsMenuEnter(const struct Menu *menu)
{
    setView(settingsMenuOptionViews[menu->state->selectedIndex]);
}

static void onSettingsMenuBack(const struct Menu *menu)
{
    setMeasurementView(getMeasurementView());
}

void onSettingsSubMenuBack(const struct Menu *menu)
{
    setView(&settingsMenuView);
}

struct MenuState settingsMenuState;

static const struct Menu settingsMenu = {
    "Settings",
    &settingsMenuState,
    onMenuGetOption,
    settingsMenuOptions,
    NULL,
    onSettingsMenuEnter,
    onSettingsMenuBack,
};

const struct View settingsMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &settingsMenu,
};
