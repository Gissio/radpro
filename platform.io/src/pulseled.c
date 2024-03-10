/*
 * Rad Pro
 * Pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(PULSELED)

#include "menu.h"
#include "pulseled.h"
#include "settings.h"

static const Menu pulseLEDMenu;

void initPulseLED(void)
{
    initPulseLEDController();

    selectMenuItem(&pulseLEDMenu,
                   settings.pulseLED,
                   PULSELED_NUM);
}

// Pulse LED menu

static const char *const pulseLEDMenuOptions[] = {
    "Off",
    "On",
    NULL,
};

static const char *onPulseLEDMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseLED);

    return pulseLEDMenuOptions[index];
}

static void onPulseLEDMenuSelect(const Menu *menu)
{
    settings.pulseLED = menu->state->selectedIndex;
}

static MenuState pulseLEDMenuState;

static const Menu pulseLEDMenu = {
    "Pulse LED",
    &pulseLEDMenuState,
    onPulseLEDMenuGetOption,
    onPulseLEDMenuSelect,
    onSettingsSubMenuBack,
};

const View pulseLEDMenuView = {
    onMenuEvent,
    &pulseLEDMenu,
};

#endif
