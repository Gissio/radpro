/*
 * FS2011 Pro
 * Menus
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>

#include "display.h"
#include "events.h"
#include "format.h"
#include "game.h"
#include "menus.h"
#include "rng.h"
#include "settings.h"
#include "ui.h"

struct Menus
{
    const struct Menu *currentMenu;

    char menuOption[16];

    View measurementView;
} menus;

static const char *getMenuOption(const struct Menu *menu, uint8_t index)
{
    return menu->options[index];
}

const char *const unitsMenuOptions[] = {
    "Sievert",
    "rem",
    "cpm",
    "cps",
    NULL,
};

MenuState unitsMenuState;

const struct Menu unitsMenu = {
    "Units",
    getMenuOption,
    unitsMenuOptions,
    &unitsMenuState,
};

const char *const historyMenuOptions[] = {
    "2 minutes",
    "10 minutes",
    "1 hour",
    "6 hours",
    "24 hours",
    NULL,
};

MenuState historyMenuState;

const struct Menu historyMenu = {
    "History",
    getMenuOption,
    historyMenuOptions,
    &historyMenuState,
};

static const char *getRateAlarmMenuOption(const struct Menu *menu, uint8_t index)
{
    if (!index)
        return "Off";

    if (index >= RATE_ALARM_NUM)
        return NULL;

    Unit *unit = &units[settings.units].rate;
    float rate = getRateAlarmSvH(index) / units[UNITS_SIEVERTS].rate.scale;
    formatValue(unit->name,
                unit->scale * rate,
                menus.menuOption);

    return menus.menuOption;
}

MenuState rateAlarmMenuState;

const struct Menu rateAlarmMenu = {
    "Rate alarm",
    getRateAlarmMenuOption,
    NULL,
    &rateAlarmMenuState,
};

static const char *getDoseAlarmMenuOption(const struct Menu *menu, uint8_t index)
{
    if (!index)
        return "Off";

    if (index >= DOSE_ALARM_NUM)
        return NULL;

    Unit *unit = &units[settings.units].dose;
    float dose = getDoseAlarmSv(index) / units[UNITS_SIEVERTS].dose.scale;
    formatValue(unit->name,
                unit->scale * dose,
                menus.menuOption);

    return menus.menuOption;
}

MenuState doseAlarmMenuState;

const struct Menu doseAlarmMenu = {
    "Dose alarm",
    getDoseAlarmMenuOption,
    NULL,
    &doseAlarmMenuState,
};

const char *const pulseSoundMenuOptions[] = {
    "Off",
    "Quiet",
    "Loud",
    NULL,
};

MenuState pulseSoundMenuState;

const struct Menu pulseSoundMenu = {
    "Pulse clicks",
    getMenuOption,
    pulseSoundMenuOptions,
    &pulseSoundMenuState,
};

const char *const backlightMenuOptions[] = {
    "Off",
    "On for 10 seconds",
    "On for 60 seconds",
    "Pulse flashes",
    "Always on",
    NULL,
};

MenuState backlightMenuState;

const struct Menu backlightMenu = {
    "Backlight",
    getMenuOption,
    backlightMenuOptions,
    &backlightMenuState,
};

const char *const batteryTypeMenuOptions[] = {
    "Ni-MH",
    "Alkaline",
    NULL,
};

MenuState batteryTypeMenuState;

const struct Menu batteryTypeMenu = {
    "Battery type",
    getMenuOption,
    batteryTypeMenuOptions,
    &batteryTypeMenuState,
};

const char *const tubeTypeMenuOptions[] = {
    "HH614 (68.4 cpm/\x7fSv/h)",
    "M4011 (153 cpm/\x7fSv/h)",
    "SBM-20 (150 cpm/\x7fSv/h)",
    "SI-3BG (1.61 cpm/\x7fSv/h)",
    NULL,
};

MenuState tubeTypeMenuState;

const struct Menu tubeTypeMenu = {
    "Geiger tube type",
    getMenuOption,
    tubeTypeMenuOptions,
    &tubeTypeMenuState,
};

const char *const rngMenuOptions[] = {
    "Letters & numbers",
    "Full ASCII",
    "Hexadecimal",
    "Decimal",
    "6-sided dice",
    "Coin toss",
    NULL,
};

MenuState rngMenuState;

const struct Menu rngMenu = {
    "Random number generator",
    getMenuOption,
    rngMenuOptions,
    &rngMenuState,
};

const char *const gameStartMenuOptions[] = {
    "Play white",
    "Play black",
    "Skill level",
    NULL,
};

MenuState gameStartMenuState;

const struct Menu gameStartMenu = {
    "Game",
    getMenuOption,
    gameStartMenuOptions,
    &gameStartMenuState,
};

const char *const gameContinueMenuOptions[] = {
    "Continue game",
    "New game",
    "Skill level",
    NULL,
};

MenuState gameContinueMenuState;

const struct Menu gameContinueMenu = {
    "Game",
    getMenuOption,
    gameContinueMenuOptions,
    &gameContinueMenuState,
};

const char *const gameSkillMenuOptions[] = {
    "Level 1",
    "Level 2",
    "Level 3",
    "Level 4",
    "Level 5",
    "Level 6",
    "Level 7",
    "Level 8",
    NULL,
};

MenuState gameSkillMenuState;

const struct Menu gameSkillMenu = {
    "Skill level",
    getMenuOption,
    gameSkillMenuOptions,
    &gameSkillMenuState,
};

const char *const settingsMenuOptions[] = {
    "Units",
    "History",
    "Rate alarm",
    "Dose alarm",
    "Pulse clicks",
    "Backlight",
    "Battery type",
    "Geiger tube type",
    "Random number generator",
    "Statistics",
    "Game",
    NULL,
};

MenuState settingsMenuState;

const struct Menu settingsMenu = {
    "Settings",
    getMenuOption,
    settingsMenuOptions,
    &settingsMenuState,
};

static void selectMenuIndex(const struct Menu *menu, uint32_t selectedIndex)
{
    uint8_t optionsNum = 0;
    while (menu->getMenuOption(menu, optionsNum) != NULL)
        optionsNum++;

    menu->state->selectedIndex = selectedIndex;
    if (selectedIndex < MENU_VIEW_LINE_NUM)
        menu->state->startIndex = 0;
    else if (selectedIndex < (uint32_t)(optionsNum - MENU_VIEW_LINE_NUM))
        menu->state->startIndex = selectedIndex;
    else
        menu->state->startIndex = optionsNum - MENU_VIEW_LINE_NUM;
}

void initMenus(void)
{
    selectMenuIndex(&settingsMenu, 0);

    selectMenuIndex(&unitsMenu, settings.units);
    selectMenuIndex(&historyMenu, settings.history);
    selectMenuIndex(&rateAlarmMenu, settings.rateAlarm);
    selectMenuIndex(&doseAlarmMenu, settings.doseAlarm);
    selectMenuIndex(&pulseSoundMenu, settings.pulseSound);
    selectMenuIndex(&backlightMenu, settings.backlight);
    selectMenuIndex(&batteryTypeMenu, settings.batteryType);
    selectMenuIndex(&tubeTypeMenu, settings.tubeType);

    selectMenuIndex(&gameSkillMenu, settings.gameSkillLevel);
}

static void setMenu(const struct Menu *menu)
{
    menus.currentMenu = menu;

    setView(VIEW_MENU);
}

void openSettingsMenu(void)
{
    menus.measurementView = getView();

    setMenu(&settingsMenu);
}

void openGameMenu(void)
{
    if (isGameStart())
    {
        setMenu(&gameStartMenu);

        gameContinueMenuState.selectedIndex = 0;
    }
    else
        setMenu(&gameContinueMenu);
}

void drawMenuView(void)
{
    const struct Menu *currentMenu = menus.currentMenu;

    drawTitle(currentMenu->title);
    drawMenu(currentMenu,
             currentMenu->state->startIndex,
             currentMenu->state->selectedIndex);
}

static void updateMenuOption(void)
{
    const struct Menu *currentMenu = menus.currentMenu;
    uint8_t selectedIndex = currentMenu->state->selectedIndex;

    if (currentMenu == &unitsMenu)
    {
        settings.units = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &historyMenu)
    {
        settings.history = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &rateAlarmMenu)
    {
        settings.rateAlarm = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &doseAlarmMenu)
    {
        settings.doseAlarm = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &pulseSoundMenu)
    {
        settings.pulseSound = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &backlightMenu)
    {
        settings.backlight = selectedIndex;
        updateBacklight();

        writeSettings();
    }
    else if (currentMenu == &batteryTypeMenu)
    {
        settings.batteryType = selectedIndex;

        writeSettings();
    }
    else if (currentMenu == &tubeTypeMenu)
    {
        settings.tubeType = selectedIndex;
        updateTubeType();

        writeSettings();
    }
    else if (currentMenu == &gameSkillMenu)
    {
        settings.gameSkillLevel = selectedIndex;

        writeSettings();
    }
}

void onMenuViewKey(KeyEvent keyEvent)
{
    const struct Menu *currentMenu = menus.currentMenu;

    switch (keyEvent)
    {
    case KEY_UP:
        if (currentMenu->state->selectedIndex > 0)
        {
            currentMenu->state->selectedIndex--;

            if (currentMenu->state->selectedIndex < currentMenu->state->startIndex)
                currentMenu->state->startIndex--;
        }

        updateMenuOption();
        updateView();

        break;

    case KEY_DOWN:
        if (currentMenu->getMenuOption(currentMenu,
                                       currentMenu->state->selectedIndex + 1))
        {
            currentMenu->state->selectedIndex++;

            if (currentMenu->state->selectedIndex >
                (currentMenu->state->startIndex + MENU_VIEW_LINE_NUM - 1))
                currentMenu->state->startIndex++;
        }

        updateMenuOption();
        updateView();

        break;

    case KEY_SELECT:
        if (currentMenu == &settingsMenu)
        {
            switch (currentMenu->state->selectedIndex)
            {
            case 0:
                setMenu(&unitsMenu);
                break;

            case 1:
                setMenu(&historyMenu);
                break;

            case 2:
                setMenu(&rateAlarmMenu);
                break;

            case 3:
                setMenu(&doseAlarmMenu);
                break;

            case 4:
                setMenu(&pulseSoundMenu);
                break;

            case 5:
                setMenu(&backlightMenu);
                break;

            case 6:
                setMenu(&batteryTypeMenu);
                break;

            case 7:
                setMenu(&tubeTypeMenu);
                break;

            case 8:
                setMenu(&rngMenu);
                break;

            case 9:
                setView(VIEW_STATS);
                break;

            case 10:
                openGameMenu();
                break;
            }
        }
        else if (currentMenu == &rngMenu)
        {
            resetRNG(currentMenu->state->selectedIndex);
            setView(VIEW_RNG);
        }
        else if (currentMenu == &gameStartMenu)
        {
            switch (currentMenu->state->selectedIndex)
            {
            case 0:
            case 1:
                resetGame(currentMenu->state->selectedIndex);
                setView(VIEW_GAME);
                break;

            case 2:
                setMenu(&gameSkillMenu);
                break;
            }
        }
        else if (currentMenu == &gameContinueMenu)
        {
            switch (currentMenu->state->selectedIndex)
            {
            case 0:
                setView(VIEW_GAME);
                break;

            case 1:
                resetGame(0);
                openGameMenu();
                break;

            case 2:
                setMenu(&gameSkillMenu);
                break;
            }
        }
        break;

    case KEY_BACK:
        if (currentMenu == &gameSkillMenu)
            openGameMenu();
        else if (currentMenu == &settingsMenu)
            setView(menus.measurementView);
        else
            setMenu(&settingsMenu);
        break;
    }
}
