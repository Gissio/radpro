/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../peripherals/display.h"
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/menu.h"

void showDisplayMenu(void);

static const Menu displayMenu;
#if defined(DISPLAY_MONOCHROME)
static const Menu displayContrastMenu;
#elif defined(DISPLAY_COLOR)
static const Menu displayThemeMenu;
#endif
static const Menu displayBrightnessMenu;
static const Menu displaySleepMenu;

void setupDisplay(void)
{
    selectMenuItem(&displayMenu, 0);
#if defined(DISPLAY_MONOCHROME)
    selectMenuItem(&displayContrastMenu, settings.displayContrast);
#elif defined(DISPLAY_COLOR)
    selectMenuItem(&displayThemeMenu, settings.displayTheme);
#endif
    selectMenuItem(&displaySleepMenu, settings.displaySleep);
    selectMenuItem(&displayBrightnessMenu, settings.displayBrightness);
}

// Display contrast level menu

#if defined(DISPLAY_MONOCHROME)

static const char *onDisplayContrastMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayContrast);

    strcpy(menuOption, getString(STRING_CONTRAST_LEVEL));
    strcatChar(menuOption, ' ');
    strcatUInt32(menuOption, index + 1, 0);

    return menuOption;
}

static void onDisplayContrastMenuSelect(menu_size_t index)
{
    settings.displayContrast = index;

    updateDisplayContrast();
}

static MenuState displayContrastMenuState;

static const Menu displayContrastMenu = {
    STRING_CONTRAST,
    &displayContrastMenuState,
    DISPLAY_CONTRAST_NUM,
    onDisplayContrastMenuGetOption,
    onDisplayContrastMenuSelect,
    showDisplayMenu,
};

#endif

// Display theme menu

#if defined(DISPLAY_COLOR)

static cstring displayThemeMenuOptions[] = {
    STRING_THEME_DAY,
    STRING_THEME_DUSK,
    STRING_THEME_NIGHT,
};

static const char *onDisplayThemeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayTheme);

    return getString(displayThemeMenuOptions[index]);
}

static void onDisplayThemeMenuSelect(menu_size_t index)
{
    settings.displayTheme = index;

    requestViewUpdate();
}

static MenuState displayThemeMenuState;

static const Menu displayThemeMenu = {
    STRING_THEME,
    &displayThemeMenuState,
    ARRAY_SIZE(displayThemeMenuOptions),
    onDisplayThemeMenuGetOption,
    onDisplayThemeMenuSelect,
    showDisplayMenu,
};

#endif

// Display brightness level menu

static cstring displayBrightnessMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
};

static const char *onDisplayBrightnessMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayBrightness);

    return getString(displayBrightnessMenuOptions[index]);
}

static void onDisplayBrightnessMenuSelect(menu_size_t index)
{
    settings.displayBrightness = index;
}

static MenuState displayBrightnessMenuState;

static const Menu displayBrightnessMenu = {
    STRING_BRIGHTNESS,
    &displayBrightnessMenuState,
    ARRAY_SIZE(displayBrightnessMenuOptions),
    onDisplayBrightnessMenuGetOption,
    onDisplayBrightnessMenuSelect,
    showDisplayMenu,
};

// Display sleep menu

static cstring displaySleepMenuOptions[] = {
#if defined(DISPLAY_MONOCHROME)
    STRING_ALWAYS_OFF,
#endif
    STRING_10_SECONDS,
    STRING_30_SECONDS,
    STRING_1_MINUTE,
    STRING_2_MINUTES,
    STRING_5_MINUTES,
    STRING_ALWAYS_ON,
};

static const char *onDisplaySleepMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displaySleep);

    return getString(displaySleepMenuOptions[index]);
}

static void onDisplaySleepMenuSelect(menu_size_t index)
{
    settings.displaySleep = index;
}

static MenuState displaySleepMenuState;

static const Menu displaySleepMenu = {
    STRING_SLEEP,
    &displaySleepMenuState,
    ARRAY_SIZE(displaySleepMenuOptions),
    onDisplaySleepMenuGetOption,
    onDisplaySleepMenuSelect,
    showDisplayMenu,
};

// Display menu

static const MenuOption displayMenuOptions[] = {
#if defined(DISPLAY_COLOR)
    {STRING_THEME, &displayThemeMenu},
#endif
    {STRING_BRIGHTNESS, &displayBrightnessMenu},
#if defined(DISPLAY_MONOCHROME)
    {STRING_CONTRAST, &displayContrastMenu},
#endif
    {STRING_SLEEP, &displaySleepMenu},
};

static const char *onDisplayMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(displayMenuOptions[index].title);
}

static void onDisplayMenuSelect(menu_size_t index)
{
    showMenu(displayMenuOptions[index].menu);
}

static MenuState displayMenuState;

static const Menu displayMenu = {
    STRING_DISPLAY,
    &displayMenuState,
    ARRAY_SIZE(displayMenuOptions),
    onDisplayMenuGetOption,
    onDisplayMenuSelect,
    showSettingsMenu,
};

void showDisplayMenu(void)
{
    showMenu(&displayMenu);
}
