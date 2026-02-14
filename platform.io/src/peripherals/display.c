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
#include "../system/settings.h"
#include "../ui/menu.h"

void setDisplayMenu(void);

static Menu displayMenu;
#if defined(DISPLAY_MONOCHROME)
static Menu displayContrastMenu;
#elif defined(DISPLAY_COLOR)
static Menu displayThemeMenu;
#endif
static Menu displayBrightnessMenu;
static Menu displaySleepMenu;

void setupDisplay(void)
{
    selectMenuItem(&displayMenu, 0, 0);
#if defined(DISPLAY_MONOCHROME)
    selectMenuItem(&displayContrastMenu, settings.displayContrast, DISPLAY_CONTRAST_NUM);
#elif defined(DISPLAY_COLOR)
    selectMenuItem(&displayThemeMenu, settings.displayTheme, DISPLAY_THEME_NUM);
#endif
    selectMenuItem(&displaySleepMenu, settings.displaySleep, DISPLAY_SLEEP_NUM);
    selectMenuItem(&displayBrightnessMenu, settings.displayBrightness, DISPLAY_BRIGHTNESS_NUM);
}

// Display contrast level menu

#if defined(DISPLAY_MONOCHROME)

static const char *onDisplayContrastMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    if (index >= DISPLAY_CONTRAST_NUM)
        return NULL;

    *menuStyle = (index == settings.displayContrast);

    strcpy(menuOption, getString(STRING_CONTRAST_LEVEL));
    strcatChar(menuOption, ' ');
    strcatUInt32(menuOption, index + 1, 0);

    return menuOption;
}

static void onDisplayContrastMenuSelect(uint32_t index)
{
    settings.displayContrast = index;

    updateDisplayContrast();
}

static MenuState displayContrastMenuState;

static Menu displayContrastMenu = {
    STRING_CONTRAST,
    &displayContrastMenuState,
    onDisplayContrastMenuGetOption,
    onDisplayContrastMenuSelect,
    setDisplayMenu,
};

View displayContrastMenuView = {
    onMenuEvent,
    &displayContrastMenu,
};

#endif

// Display theme menu

#if defined(DISPLAY_COLOR)

static cstring displayThemeMenuOptions[] = {
    STRING_THEME_DAY,
    STRING_THEME_DUSK,
    STRING_THEME_NIGHT,
    NULL,
};

static const char *onDisplayThemeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayTheme);

    return getString(displayThemeMenuOptions[index]);
}

static void onDisplayThemeMenuSelect(uint32_t index)
{
    settings.displayTheme = index;

    requestViewUpdate();
}

static MenuState displayThemeMenuState;

static Menu displayThemeMenu = {
    STRING_THEME,
    &displayThemeMenuState,
    onDisplayThemeMenuGetOption,
    onDisplayThemeMenuSelect,
    setDisplayMenu,
};

View displayThemeMenuView = {
    onMenuEvent,
    &displayThemeMenu,
};

#endif

// Display brightness level menu

static cstring displayBrightnessMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
    NULL,
};

static const char *onDisplayBrightnessMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displayBrightness);

    return getString(displayBrightnessMenuOptions[index]);
}

static void onDisplayBrightnessMenuSelect(uint32_t index)
{
    settings.displayBrightness = index;
}

static MenuState displayBrightnessMenuState;

static Menu displayBrightnessMenu = {
    STRING_BRIGHTNESS,
    &displayBrightnessMenuState,
    onDisplayBrightnessMenuGetOption,
    onDisplayBrightnessMenuSelect,
    setDisplayMenu,
};

View displayBrightnessMenuView = {
    onMenuEvent,
    &displayBrightnessMenu,
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
    NULL,
};

static const char *onDisplaySleepMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.displaySleep);

    return getString(displaySleepMenuOptions[index]);
}

static void onDisplaySleepMenuSelect(uint32_t index)
{
    settings.displaySleep = index;
}

static MenuState displaySleepMenuState;

static Menu displaySleepMenu = {
    STRING_SLEEP,
    &displaySleepMenuState,
    onDisplaySleepMenuGetOption,
    onDisplaySleepMenuSelect,
    setDisplayMenu,
};

View displaySleepMenuView = {
    onMenuEvent,
    &displaySleepMenu,
};

// Display menu

static MenuOption displayMenuOptions[] = {
#if defined(DISPLAY_COLOR)
    {STRING_THEME, &displayThemeMenuView},
#endif
    {STRING_BRIGHTNESS, &displayBrightnessMenuView},
#if defined(DISPLAY_MONOCHROME)
    {STRING_CONTRAST, &displayContrastMenuView},
#endif
    {STRING_SLEEP, &displaySleepMenuView},
    {NULL},
};

static const char *onDisplayMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(displayMenuOptions[index].title);
}

static void onDisplayMenuSelect(uint32_t index)
{
    setView(displayMenuOptions[index].view);
}

static MenuState displayMenuState;

static Menu displayMenu = {
    STRING_DISPLAY,
    &displayMenuState,
    onDisplayMenuGetOption,
    onDisplayMenuSelect,
    setSettingsMenu,
};

View displayMenuView = {
    onMenuEvent,
    &displayMenu,
};

void setDisplayMenu(void)
{
    setView(&displayMenuView);
}
