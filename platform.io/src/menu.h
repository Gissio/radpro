/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(MENU_H)

#define MENU_H

#include <stddef.h>
#include <stdint.h>

#include "view.h"

struct Menu;

typedef const char *OnMenuGetOption(const struct Menu *menu, uint32_t index);
typedef void OnMenuSelect(const struct Menu *menu);
typedef void OnMenuEnter(const struct Menu *menu);
typedef void OnMenuBack(const struct Menu *menu);

extern char menuOption[16];

struct MenuState
{
    uint16_t startIndex;
    uint16_t selectedIndex;
};

struct Menu
{
    const char *title;

    struct MenuState *state;

    OnMenuGetOption *onGetOption;
    const char *const *options;

    OnMenuSelect *onSelect;
    OnMenuEnter *onEnter;
    OnMenuBack *onBack;
};

const char *onMenuGetOption(const struct Menu *menu, uint32_t index);

void selectMenuIndex(const struct Menu *menu, uint32_t index, uint32_t optionsNum);

void onMenuEvent(const struct View *view, enum Event event);

#endif
