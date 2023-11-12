/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "display.h"
#include "menu.h"

char menuOption[32];

const char *onMenuGetOption(const struct Menu *menu, uint32_t index)
{
    return menu->options[index];
}

void selectMenuIndex(const struct Menu *menu, uint32_t index, uint32_t optionsNum)
{
    menu->state->selectedIndex = index;
    if (index < menuLineNum)
        menu->state->startIndex = 0;
    else if (index < (uint32_t)(optionsNum - menuLineNum))
        menu->state->startIndex = index;
    else
        menu->state->startIndex = optionsNum - menuLineNum;
}

void onMenuEvent(const struct View *view, enum Event event)
{
    const struct Menu *menu = (const struct Menu *)view->userdata;
    struct MenuState *menuState = menu->state;

    if (event == EVENT_KEY_UP)
    {
        if (menuState->selectedIndex > 0)
        {
            menuState->selectedIndex--;

            if (menuState->selectedIndex < menuState->startIndex)
                menuState->startIndex--;
        }
        else
        {
            uint32_t index = 0;
            while (menu->onGetOption(menu, index))
                index++;

            menuState->selectedIndex = index - 1;
            if (index > menuLineNum)
                menuState->startIndex = index - menuLineNum;
            else
                menuState->startIndex = 0;
        }
    }
    else if (event == EVENT_KEY_DOWN)
    {
        if (menu->onGetOption(menu, menuState->selectedIndex + 1))
        {
            menuState->selectedIndex++;

            if (menuState->selectedIndex > (menuState->startIndex + menuLineNum - 1))
                menuState->startIndex++;
        }
        else
        {
            menuState->selectedIndex = 0;
            menuState->startIndex = 0;
        }
    }

    switch (event)
    {
    case EVENT_KEY_UP:
    case EVENT_KEY_DOWN:
        refreshView();

        if (menu->onSelect)
            menu->onSelect(menu);

        break;

    case EVENT_KEY_ENTER:
        if (menu->onEnter)
            menu->onEnter(menu);

        break;

    case EVENT_KEY_BACK:
        if (menu->onBack)
            menu->onBack(menu);

        break;

    case EVENT_DRAW:
    {
        drawTitle(menu->title);
        drawMenu(menu);

        break;
    }

    default:
        break;
    }
}
