/*
 * Rad Pro
 * UI menu
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(UI_MENU_H)
#define UI_MENU_H

#include <stdint.h>

#include "../system/cstring.h"
#include "../ui/view.h"

#define MENU_OPTION_STRING_SIZE 64

#if defined(DISPLAY_128X64)
#define MENU_LINE_HEIGHT 14
#elif defined(DISPLAY_320X240)
#define MENU_LINE_HEIGHT 48
#elif defined(DISPLAY_240X320)
#define MENU_LINE_HEIGHT 45
#endif

#define MENUSTYLE_CHECKED (1 << 0)
#define MENUSTYLE_SUBMENU (1 << 1)
#define MENUSTYLE_NUM 2

typedef uint16_t menu_size_t;

typedef uint32_t MenuStyle;

typedef const char *OnMenuGetOption(menu_size_t index, MenuStyle *menuStyle);
typedef void OnMenuSelect(menu_size_t index);
typedef void OnMenuBack(void);

typedef struct
{
    menu_size_t startIndex;
    menu_size_t selectedIndex;
} MenuState;

typedef struct
{
    cstring title;
    MenuState *state;
    menu_size_t itemCount;
    OnMenuGetOption *onGetOption;
    OnMenuSelect *onSelect;
    OnMenuBack *onBack;
} Menu;

typedef struct
{
    cstring title;
    SetView *setView;
} ViewOption;

typedef struct
{
    cstring title;
    const Menu *menu;
} MenuOption;

extern char menuOption[MENU_OPTION_STRING_SIZE];

void selectMenuItem(const Menu *menu, menu_size_t index);

void showMenu(const Menu *menu);

#endif
