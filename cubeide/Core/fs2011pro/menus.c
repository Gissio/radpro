/*
 * FS2011 Pro
 * Menus
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "display.h"
#include "events.h"
#include "format.h"
#include "game.h"
#include "keyboard.h"
#include "menus.h"
#include "settings.h"
#include "ui.h"

typedef struct
{
    unsigned char startIndex;
    unsigned char selectedIndex;
} MenuState;

typedef const struct
{
    char *title;
    GetMenuOption *getMenuOption;
    const char *const *options;
    MenuState *state;
} Menu;

struct Menus
{
    Menu *currentMenu;

    char menuOption[16];

    unsigned char measurementView;
} menus;

const char *getMenuOption(void *userdata, unsigned int index)
{
    return ((Menu *)userdata)->options[index];
}

const char *const unitsMenuOptions[] = {
    "Sievert",
    "rem",
    "cpm",
    "cps",
    NULL,
};

MenuState unitsMenuState;

Menu unitsMenu = {
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

Menu historyMenu = {
    "History",
    getMenuOption,
    historyMenuOptions,
    &historyMenuState,
};

const char *getRateAlarmMenuOption(void *userdata, unsigned int index)
{
    if (!index)
        return "Off";

    if (index >= RATE_ALARM_NUM)
        return NULL;

    Unit *unit = &units[settings.units].rate;
    float rate = getRateAlarmSvH(index) / units[UNITS_SIEVERTS].rate.scale;
    formatValue(unit->name,
                unit->scale * rate,
                unit->minExponent,
                menus.menuOption);

    return menus.menuOption;
}

MenuState rateAlarmMenuState;

Menu rateAlarmMenu = {
    "Rate alarm",
    getRateAlarmMenuOption,
    NULL,
    &rateAlarmMenuState,
};

const char *getDoseAlarmMenuOption(void *userdata, unsigned int index)
{
    if (!index)
        return "Off";

    if (index >= DOSE_ALARM_NUM)
        return NULL;

    Unit *unit = &units[settings.units].dose;
    float dose = getDoseAlarmSv(index) / units[UNITS_SIEVERTS].dose.scale;
    formatValue(unit->name,
                unit->scale * dose,
                unit->minExponent,
                menus.menuOption);

    return menus.menuOption;
}

MenuState doseAlarmMenuState;

Menu doseAlarmMenu = {
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

Menu pulseSoundMenu = {
    "Pulse sound",
    getMenuOption,
    pulseSoundMenuOptions,
    &pulseSoundMenuState,
};

const char *const backlightMenuOptions[] = {
    "Off",
    "On for 10 seconds",
    "On for 60 seconds",
    "Always on",
    NULL,
};

MenuState backlightMenuState;

Menu backlightMenu = {
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

Menu batteryTypeMenu = {
    "Battery type",
    getMenuOption,
    batteryTypeMenuOptions,
    &batteryTypeMenuState,
};

const char *const gameStartMenuOptions[] = {
    "Play white",
    "Play black",
    "Skill level",
    NULL,
};

MenuState gameStartMenuState;

Menu gameStartMenu = {
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

Menu gameContinueMenu = {
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

Menu gameSkillMenu = {
    "Game Skill",
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
    "Statistics",
    "Game",
    NULL,
};

MenuState settingsMenuState;

Menu settingsMenu = {
    "Settings",
    getMenuOption,
    settingsMenuOptions,
    &settingsMenuState,
};

void selectMenuIndex(Menu *menu, int selectedIndex)
{
    int optionsNum = 0;
    while (menu->getMenuOption((void *)menu, optionsNum) != NULL)
        optionsNum++;

    menu->state->selectedIndex = selectedIndex;
    if (selectedIndex < MENU_VIEW_LINE_NUM)
        menu->state->startIndex = 0;
    else if (selectedIndex < (optionsNum - MENU_VIEW_LINE_NUM))
        menu->state->startIndex = selectedIndex;
    else
        menu->state->startIndex = optionsNum - MENU_VIEW_LINE_NUM;
}

void initMenus()
{
    selectMenuIndex(&settingsMenu, 0);

    selectMenuIndex(&unitsMenu, settings.units);
    selectMenuIndex(&historyMenu, settings.history);
    selectMenuIndex(&rateAlarmMenu, settings.rateAlarm);
    selectMenuIndex(&doseAlarmMenu, settings.doseAlarm);
    selectMenuIndex(&pulseSoundMenu, settings.pulseSound);
    selectMenuIndex(&backlightMenu, settings.backlight);
    selectMenuIndex(&batteryTypeMenu, settings.batteryType);

    selectMenuIndex(&gameStartMenu, 0);
    selectMenuIndex(&gameContinueMenu, 0);
}

void setMenu(Menu *menu)
{
    menus.currentMenu = menu;

    setView(VIEW_MENU);
}

void openSettingsMenu()
{
    menus.measurementView = getView();

    setMenu(&settingsMenu);
}

void openGameMenu()
{
    if (isGameStart())
    {
        setMenu(&gameStartMenu);

        gameContinueMenuState.selectedIndex = 0;
    }
    else
    {

        setMenu(&gameContinueMenu);
    }
}

void drawMenuView()
{
    drawTitle(menus.currentMenu->title);
    drawMenu(menus.currentMenu->getMenuOption,
             (void *)menus.currentMenu,
             menus.currentMenu->state->startIndex,
             menus.currentMenu->state->selectedIndex);
}

void updateMenuOption()
{
    if ((menus.currentMenu == &settingsMenu) ||
        (menus.currentMenu == &gameStartMenu) ||
        (menus.currentMenu == &gameContinueMenu))
        return;

    switch (settingsMenuState.selectedIndex)
    {
    case 0:
        settings.units = menus.currentMenu->state->selectedIndex;
        break;

    case 1:
        settings.history = menus.currentMenu->state->selectedIndex;
        break;

    case 2:
        settings.rateAlarm = menus.currentMenu->state->selectedIndex;
        break;

    case 3:
        settings.doseAlarm = menus.currentMenu->state->selectedIndex;
        break;

    case 4:
        settings.pulseSound = menus.currentMenu->state->selectedIndex;
        break;

    case 5:
        settings.backlight = menus.currentMenu->state->selectedIndex;
        triggerBacklight();
        break;

    case 6:
        settings.batteryType = menus.currentMenu->state->selectedIndex;
        break;

    case 8:
        settings.gameSkillLevel = menus.currentMenu->state->selectedIndex;
        break;
    }
}

void onMenuViewKey(int key)
{
    switch (key)
    {
    case KEY_UP:
        if (menus.currentMenu->state->selectedIndex > 0)
        {
            menus.currentMenu->state->selectedIndex--;

            if (menus.currentMenu->state->selectedIndex < menus.currentMenu->state->startIndex)
                menus.currentMenu->state->startIndex--;
        }

        updateMenuOption();
        updateView();

        break;

    case KEY_DOWN:
        if (menus.currentMenu->getMenuOption((void *)menus.currentMenu,
                                             menus.currentMenu->state->selectedIndex + 1))
        {
            menus.currentMenu->state->selectedIndex++;

            if (menus.currentMenu->state->selectedIndex >
                (menus.currentMenu->state->startIndex + MENU_VIEW_LINE_NUM - 1))
                menus.currentMenu->state->startIndex++;
        }

        updateMenuOption();
        updateView();

        break;

    case KEY_SELECT:
        if (menus.currentMenu == &settingsMenu)
        {
            switch (menus.currentMenu->state->selectedIndex)
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
                setView(VIEW_STATS);
                break;

            case 8:
                openGameMenu();
                break;
            }
        }
        else if (menus.currentMenu == &gameStartMenu)
        {
            switch (menus.currentMenu->state->selectedIndex)
            {
            case 0:
            case 1:
                resetGame(menus.currentMenu->state->selectedIndex);
                setView(VIEW_GAME);
                break;

            case 2:
                setMenu(&gameSkillMenu);
                break;
            }
        }
        else if (menus.currentMenu == &gameContinueMenu)
        {
            switch (menus.currentMenu->state->selectedIndex)
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
        if (menus.currentMenu == &gameSkillMenu)
            openGameMenu();
        else if (menus.currentMenu == &settingsMenu)
            setView(menus.measurementView);
        else
            setMenu(&settingsMenu);
        break;
    }
}
