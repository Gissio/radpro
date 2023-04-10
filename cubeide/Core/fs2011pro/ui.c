/*
 * FS2011 Pro
 * User interface
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "display.h"
#include "game.h"
#include "keyboard.h"
#include "measurements.h"
#include "menus.h"
#include "power.h"
#include "settings.h"
#include "stats.h"
#include "ui.h"

#ifdef SDL_MODE
void onSDLTick();
#endif

struct UI
{
    uint8_t currentView;
    bool updateView;

    bool backKeyDown;
} ui;

void setView(uint8_t viewIndex)
{
    ui.currentView = viewIndex;
    ui.backKeyDown = false;

    updateView();
}

int getView()
{
    return ui.currentView;
}

void updateView()
{
    ui.updateView = true;
}

void updateUI()
{
#ifdef SDL_MODE
    onSDLTick();
#endif

    updateWatchdog();

    updateEvents();

    // Key events
    int key = getEventsKey();

    if (key == KEY_BACK)
        ui.backKeyDown = true;
    else if (key == KEY_BACK_UP)
    {
        if (ui.backKeyDown)
            ui.backKeyDown = false;
        else
            key = -1;
    }
    else if (key == KEY_POWER_OFF)
    {
        writeSettings();

        powerDown(0);
    }

    if (key >= 0)
    {
        switch (getView())
        {
        case VIEW_INSTANTANEOUS_RATE:
        case VIEW_AVERAGE_RATE:
        case VIEW_DOSE:
        case VIEW_HISTORY:
            onMeasurementViewKey(key);
            break;

        case VIEW_MENU:
            onMenuViewKey(key);
            break;

        case VIEW_STATS:
            onStatsViewKey(key);
            break;

        case VIEW_GAME:
            onGameViewKey(key);
            break;
        }
    }

    // View events
    if (ui.updateView)
    {
        ui.updateView = false;

        clearDisplay();

        drawStatusBar();

        switch (ui.currentView)
        {
        case VIEW_WELCOME:
            drawWelcome();
            break;

        case VIEW_INSTANTANEOUS_RATE:
            drawInstantaneousRateView();
            break;

        case VIEW_AVERAGE_RATE:
            drawAverageRateView();
            break;

        case VIEW_DOSE:
            drawDoseView();
            break;

        case VIEW_HISTORY:
            drawHistoryView();
            break;

        case VIEW_MENU:
            drawMenuView();
            break;

        case VIEW_STATS:
            drawStatsView();
            break;

        case VIEW_GAME:
            drawGameView();
            break;
        }

        updateDisplay();
    }
}
