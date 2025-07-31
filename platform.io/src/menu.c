/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>

#include "display.h"
#include "menu.h"

char menuOption[32];

void selectMenuItem(Menu *menu,
                    uint32_t index,
                    uint32_t optionsNum)
{
    menu->state->selectedIndex = index;
    if (index < menuLineNum)
        menu->state->startIndex = 0;
    else if (index < (uint32_t)(optionsNum - menuLineNum))
        menu->state->startIndex = index;
    else
        menu->state->startIndex = optionsNum - menuLineNum;
}

void onMenuEvent(Event event)
{
    Menu *menu = (Menu *)getView()->userdata;
    MenuState *menuState = menu->state;
    MenuStyle menuStyle;

    switch (event)
    {
    case EVENT_KEY_BACK:
        if (menu->onBack)
            menu->onBack();

        break;

    case EVENT_KEY_SELECT:
        if (menu->onSelect)
            menu->onSelect(menu->state->selectedIndex);

        requestViewUpdate();

        break;

    case EVENT_KEY_UP:
    case EVENT_KEY_DOWN:
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
                while (menu->onGetOption(index, &menuStyle))
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
            if (menu->onGetOption(menuState->selectedIndex + 1, &menuStyle))
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

        requestViewUpdate();

        break;

    case EVENT_DRAW:
    {
        drawMenu(menu);

        break;
    }

    default:
        break;
    }
}
