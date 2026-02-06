/*
 * Rad Pro
 * UI menu
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdio.h>
#include <stdint.h>

#include "../ui/draw.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define MENU_LINE_NUM (CONTENT_HEIGHT / MENU_LINE_HEIGHT)

char menuOption[MENU_OPTION_SIZE];

void drawMenu(Menu *menu)
{
    drawTitleBar(menu->title);

    uint32_t startIndex = menu->state->startIndex;
    uint32_t selectedIndex = menu->state->selectedIndex;

    mr_rectangle_t rectangle;
    rectangle.height = MENU_LINE_HEIGHT;
    rectangle.y = CONTENT_TOP;

    for (uint32_t i = 0; i < MENU_LINE_NUM; i++)
    {
        uint32_t index = startIndex + i;

        MenuStyle menuStyle;
        const char *menuItem = menu->onGetOption(index, &menuStyle);

        if (!menuItem)
            break;

        rectangle.x = CONTENT_WIDTH;
        setFont(font_symbols);
        setFillColor((index == selectedIndex) ? COLOR_AMPLIFIED_ENABLED_BACKGROUND : COLOR_CONTAINER_GLOBAL);

        // Submenu
        if (menuStyle & MENUSTYLE_SUBMENU)
        {
            setStrokeColor(COLOR_ELEMENT_NEUTRAL);
            drawRowRight("=", &rectangle);
        }

        // Checked
        if (menuStyle & MENUSTYLE_CHECKED)
        {
            setStrokeColor(COLOR_INSTRUMENT_ENHANCED_SECONDARY);
            drawRowRight(">", &rectangle);
        }

        // Text
#if defined(DISPLAY_MONOCHROME)
        setFont(font_medium);
#elif defined(DISPLAY_COLOR)
        setFont(font_small);
#endif
        setStrokeColor(COLOR_ON_FLAT_ACTIVE);
        drawRowLeft(menuItem, &rectangle);

        rectangle.y += MENU_LINE_HEIGHT;
    }

    // Bottom space
    rectangle.x = CONTENT_LEFT;
    rectangle.width = CONTENT_WIDTH;
    rectangle.height = CONTENT_BOTTOM - rectangle.y;
    setFillColor(COLOR_CONTAINER_GLOBAL);
    drawRectangle(&rectangle);
}

void selectMenuItem(Menu *menu, uint32_t index, uint32_t optionsNum)
{
    menu->state->selectedIndex = index;
    if (index < MENU_LINE_NUM)
        menu->state->startIndex = 0;
    else if (index < (uint32_t)(optionsNum - MENU_LINE_NUM))
        menu->state->startIndex = index;
    else
        menu->state->startIndex = optionsNum - MENU_LINE_NUM;
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
                if (index > MENU_LINE_NUM)
                    menuState->startIndex = index - MENU_LINE_NUM;
                else
                    menuState->startIndex = 0;
            }
        }
        else if (event == EVENT_KEY_DOWN)
        {
            if (menu->onGetOption(menuState->selectedIndex + 1, &menuStyle))
            {
                menuState->selectedIndex++;

                if (menuState->selectedIndex > (menuState->startIndex + MENU_LINE_NUM - 1))
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
