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

char menuOption[16];

const char *onMenuGetOption(const struct Menu *menu, uint32_t index)
{
    return menu->options[index];
}

void selectMenuIndex(const struct Menu *menu, uint32_t index)
{
    uint8_t optionsNum = 0;
    while (menu->onGetOption(menu, optionsNum) != NULL)
        optionsNum++;

    menu->state->selectedIndex = index;
    if (index < MENU_VIEW_LINE_NUM)
        menu->state->startIndex = 0;
    else if (index < (uint32_t)(optionsNum - MENU_VIEW_LINE_NUM))
        menu->state->startIndex = index;
    else
        menu->state->startIndex = optionsNum - MENU_VIEW_LINE_NUM;
}

void onMenuViewDraw(const struct View *view)
{
    const struct Menu *menu = (const struct Menu *)view->userdata;

    drawTitle(menu->title);
    drawMenu(menu);
}

void onMenuViewKey(const struct View *view, KeyEvent keyEvent)
{
    const struct Menu *menu = (const struct Menu *)view->userdata;
    struct MenuState *menuState = menu->state;

    switch (keyEvent)
    {
    case KEY_EVENT_UP:
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
            if (index > MENU_VIEW_LINE_NUM)
                menuState->startIndex = index - MENU_VIEW_LINE_NUM;
            else
                menuState->startIndex = 0;
        }

        break;

    case KEY_EVENT_DOWN:
        if (menu->onGetOption(menu, menuState->selectedIndex + 1))
        {
            menuState->selectedIndex++;

            if (menuState->selectedIndex > (menuState->startIndex + MENU_VIEW_LINE_NUM - 1))
                menuState->startIndex++;
        }
        else
        {
            menuState->selectedIndex = 0;
            menuState->startIndex = 0;
        }

        break;
    }

    switch (keyEvent)
    {
    case KEY_EVENT_UP:
    case KEY_EVENT_DOWN:
        refreshView();

        if (menu->onSelect)
            menu->onSelect(menu);

        break;

    case KEY_EVENT_ENTER:
        if (menu->onEnter)
            menu->onEnter(menu);

        break;

    case KEY_EVENT_BACK:
        if (menu->onBack)
            menu->onBack(menu);

        break;
    }
}
