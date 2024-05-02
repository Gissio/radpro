/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"
#include "menu.h"
#include "settings.h"
#include "tube.h"

static const Menu pulseClicksMenu;

void initBuzzer(void)
{
    initBuzzerController();

    selectMenuItem(&pulseClicksMenu,
                   settings.pulseClicks,
                   PULSE_CLICKS_NUM);
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

static const char *const pulseClicksMenuOptions[] = {
    "Off",
    "Clicks",
    "Beeps",
    NULL,
};

static const char *onPulseClicksMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseClicks);

    return pulseClicksMenuOptions[index];
}

static void onPulseClicksMenuSelect(const Menu *menu)
{
    settings.pulseClicks = menu->state->selectedIndex;
}

static MenuState pulseClicksMenuState;

static const Menu pulseClicksMenu = {
    "Pulse Clicks",
    &pulseClicksMenuState,
    onPulseClicksMenuGetOption,
    onPulseClicksMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseClicksMenuView = {
    onMenuEvent,
    &pulseClicksMenu,
};
