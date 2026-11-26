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

    // Selected item at the top, unless that scrolls past the last page
    menu_size_t startIndex = 0;
    if (index >= MENU_LINE_NUM)
    {
        startIndex = menu->itemCount - MENU_LINE_NUM;
        if (index < startIndex)
            startIndex = index;
    }

    menuState->startIndex = startIndex;
    menuState->selectedIndex = index;
}

static void drawMenu(void)
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
        bool selected = (index == selectedIndex);

        rectangle.x = CONTENT_RIGHT;
        setFont(font_symbols);
        setFillColor(selected ? COLOR_AMPLIFIED_ENABLED_BACKGROUND : COLOR_CONTAINER_GLOBAL);

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
        setStrokeColor(selected ? COLOR_ON_AMPLIFIED_ACTIVE : COLOR_ON_FLAT_ACTIVE);
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
    const Menu *menu = currentMenu;
    MenuState *menuState = menu->state;
    menu_size_t itemCount = menu->itemCount;
    menu_size_t index = menuState->selectedIndex;

    // Empty menu: no item to select or navigate to
    if ((event >= EVENT_KEY_SELECT) &&
        (event <= EVENT_KEY_DOWN) &&
        !itemCount)
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        menu->onBack();

        break;

    case EVENT_KEY_SELECT:
        menu->onSelect(index);

        requestViewUpdate();

        break;

    case EVENT_KEY_UP:
    case EVENT_KEY_DOWN:
        // Wrap around at either end
        if (event == EVENT_KEY_UP)
            index = (index ? index : itemCount) - 1;
        else if (++index >= itemCount)
            index = 0;

        menuState->selectedIndex = index;

        // Scroll the selected item into view
        if (index < menuState->startIndex)
            menuState->startIndex = index;
        else if (index >= (menuState->startIndex + MENU_LINE_NUM))
            menuState->startIndex = index - (MENU_LINE_NUM - 1);

        requestViewUpdate();

        break;

    case EVENT_DRAW:
        drawMenu();

        break;

    default:
        break;
    }
}

void showMenu(const Menu *menu)
{
    currentMenu = menu;

    showView(onMenuViewEvent);
}
