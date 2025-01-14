/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(MENU_H)
#define MENU_H

#include <stddef.h>
#include <stdint.h>

#include "view.h"

typedef struct Menu_ Menu;

#define MENUSTYLE_CHECKED (1 << 0)
#define MENUSTYLE_SUBMENU (1 << 1)
#define MENUSTYLE_NUM 2

typedef uint32_t MenuStyle;

typedef const char *OnMenuGetOption(const Menu *menu,
                                    uint32_t index,
                                    MenuStyle *menuStyle);
typedef void OnMenuSelect(const Menu *menu);
typedef void OnMenuBack(const Menu *menu);

extern char menuOption[32];

typedef struct
{
    uint16_t startIndex;
    uint16_t selectedIndex;
} MenuState;

struct Menu_
{
    const char *title;
    MenuState *state;
    OnMenuGetOption *onGetOption;
    OnMenuSelect *onSelect;
    OnMenuBack *onBack;
};

void selectMenuItem(const Menu *menu,
                    uint32_t index,
                    uint32_t optionsNum);

void onMenuEvent(const View *view, Event event);

#endif
