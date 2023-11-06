/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"
#include "menu.h"
#include "settings.h"

static const struct Menu pulseClicksMenu;

void initBuzzer(void)
{
    initBuzzerHardware();

    selectMenuIndex(&pulseClicksMenu, settings.pulseClicks, PULSE_CLICKS_NUM);
}

void playSystemAlert(void)
{
    for (uint32_t i = 0; i < 10; i++)
    {
        setBuzzer(true);
        sleep(50);
        setBuzzer(false);
        sleep(50);
    }
}

// Pulse clicks menu

static void onPulseClicksMenuSelect(const struct Menu *menu)
{
    settings.pulseClicks = menu->state->selectedIndex;
}

static const char *const pulseClicksMenuOptions[] = {
    "Off",
    "Quiet",
    "Loud",
    NULL,
};

static struct MenuState pulseClicksMenuState;

static const struct Menu pulseClicksMenu = {
    "Pulse clicks",
    &pulseClicksMenuState,
    onMenuGetOption,
    pulseClicksMenuOptions,
    onPulseClicksMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View pulseClicksMenuView = {
    onMenuEvent,
    &pulseClicksMenu,
};
