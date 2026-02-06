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

#define MENU_OPTION_SIZE 64

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

typedef const struct Menu_ Menu;

typedef uint32_t MenuStyle;

typedef const char *OnMenuGetOption(uint32_t index, MenuStyle *menuStyle);
typedef void OnMenuSelect(uint32_t index);
typedef void OnMenuBack(void);

typedef struct
{
    uint16_t startIndex;
    uint16_t selectedIndex;
} MenuState;

struct Menu_
{
    cstring title;
    MenuState *state;
    OnMenuGetOption *onGetOption;
    OnMenuSelect *onSelect;
    OnMenuBack *onBack;
};

typedef const struct
{
    cstring title;
    ViewPointer view;
} MenuOption;

extern char menuOption[MENU_OPTION_SIZE];

void drawMenu(Menu *menu);

void selectMenuItem(Menu *menu, uint32_t index, uint32_t optionsNum);

void onMenuEvent(Event event);

#endif
