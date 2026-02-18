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

char menuOption[MENU_OPTION_STRING_SIZE];

static const Menu *currentMenu;

void selectMenuItem(const Menu *menu, menu_size_t index)
{
    MenuState *menuState = menu->state;

    menuState->selectedIndex = index;

    if (index < MENU_LINE_NUM)
        menuState->startIndex = 0;
    else
    {
        menu_size_t firstVisibleIndex = (menu->itemCount - MENU_LINE_NUM);

        if (index < firstVisibleIndex)
            menuState->startIndex = index;
        else
            menuState->startIndex = firstVisibleIndex;
    }
}

void drawMenu()
{
    drawTitleBar(currentMenu->title);

    MenuState *menuState = currentMenu->state;

    menu_size_t startIndex = menuState->startIndex;
    menu_size_t selectedIndex = menuState->selectedIndex;

    mr_rectangle_t rectangle;
    rectangle.height = MENU_LINE_HEIGHT;
    rectangle.y = CONTENT_TOP;

    for (menu_size_t i = 0; i < MENU_LINE_NUM; i++)
    {
        menu_size_t index = startIndex + i;

        if (index >= currentMenu->itemCount)
            break;

        MenuStyle menuStyle;
        const char *menuItem = currentMenu->onGetOption(index, &menuStyle);

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

static void onMenuViewEvent(ViewEvent event)
{
    MenuState *menuState = currentMenu->state;

    switch (event)
    {
    case EVENT_KEY_BACK:
        if (currentMenu->onBack)
            currentMenu->onBack();

        break;

    case EVENT_KEY_SELECT:
        if (currentMenu->onSelect)
            currentMenu->onSelect(menuState->selectedIndex);

        requestViewUpdate();

        break;

    case EVENT_KEY_UP:
        if (menuState->selectedIndex > 0)
        {
            menuState->selectedIndex--;

            if (menuState->selectedIndex < menuState->startIndex)
                menuState->startIndex--;
        }
        else
        {
            menu_size_t index = currentMenu->itemCount;

            menuState->selectedIndex = index - 1;
            if (index > MENU_LINE_NUM)
                menuState->startIndex = index - MENU_LINE_NUM;
            else
                menuState->startIndex = 0;
        }

        requestViewUpdate();

        break;

    case EVENT_KEY_DOWN:
        if (menuState->selectedIndex < (currentMenu->itemCount - 1))
        {
            menuState->selectedIndex++;

            if (menuState->selectedIndex > (menuState->startIndex + (MENU_LINE_NUM - 1)))
                menuState->startIndex++;
        }
        else
        {
            menuState->selectedIndex = 0;
            menuState->startIndex = 0;
        }

        requestViewUpdate();

        break;

    case EVENT_DRAW:
    {
        drawMenu();

        break;
    }

    default:
        break;
    }
}

void showMenu(const Menu *menu)
{
    currentMenu = menu;

    showView(onMenuViewEvent);
}
