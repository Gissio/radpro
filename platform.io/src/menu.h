/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef MENU_H
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

void selectMenuIndex(const struct Menu *menu, uint32_t index);

void onMenuViewDraw(const struct View *view);
void onMenuViewKey(const struct View *view, KeyEvent keyEvent);

#endif
