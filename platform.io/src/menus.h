/*
 * Rad Pro
 * Menus
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef MENUS_H
#define MENUS_H

#include <stdint.h>

#include "keyboard.h"

typedef struct
{
    uint8_t startIndex;
    uint8_t selectedIndex;
} MenuState;

struct Menu;

typedef const char *GetMenuOption(const struct Menu *menu, uint8_t index);

struct Menu
{
    char *title;
    GetMenuOption *getMenuOption;
    const char *const *options;
    MenuState *state;
};

void initMenus(void);

void openSettingsMenu(void);
void openGameMenu(void);

void drawMenuView(void);
void onMenuViewKey(KeyEvent keyEvent);

#endif
