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

void initMenus();

void openSettingsMenu();
void openGameMenu();

void drawMenuView();
void onMenuViewKey(int key);

#endif
