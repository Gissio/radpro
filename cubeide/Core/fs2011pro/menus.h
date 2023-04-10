/*
 * FS2011 Pro
 * Menus
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef MENUS_H
#define MENUS_H

#include <stdint.h>

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

void initMenus();

void openSettingsMenu();
void openGameMenu();

void drawMenuView();
void onMenuViewKey(int key);

#endif
