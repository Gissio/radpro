/*
 * FS2011 Pro
 * User interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "display.h"
#include "game.h"
#include "keyboard.h"
#include "main.h"
#include "measurements.h"
#include "menus.h"
#include "power.h"
#include "settings.h"
#include "stats.h"
#include "ui.h"

struct UI
{
    uint8_t currentView;
    bool updateView;

    bool backKeyDown;
} ui;

void updateUI(void)
{
    waitSysTicks(0);
    updateEvents();

    // Key events
    int keyEvent = getKeyEvent();

    if (keyEvent == KEY_BACK)
        ui.backKeyDown = true;
    else if (keyEvent == KEY_BACK_UP)
    {
        if (ui.backKeyDown)
            ui.backKeyDown = false;
        else
            keyEvent = -1;
    }
    else if (keyEvent == KEY_POWER_OFF)
    {
        writeSettings();

        disableEvents();
        waitSysTicks(1);
        disableBacklight();
        setDisplay(false);
        setPower(false);

        while (true)
            waitSysTicks(1);
    }

    if (keyEvent >= 0)
    {
        switch (getView())
        {
        case VIEW_INSTANTANEOUS_RATE:
        case VIEW_AVERAGE_RATE:
        case VIEW_DOSE:
        case VIEW_HISTORY:
            onMeasurementViewKey(keyEvent);
            break;

        case VIEW_MENU:
            onMenuViewKey(keyEvent);
            break;

        case VIEW_STATS:
            onStatsViewKey(keyEvent);
            break;

        case VIEW_GAME:
            onGameViewKey(keyEvent);
            break;
        }
    }

    // View events
    if (ui.updateView)
    {
        ui.updateView = false;

        clearDisplayBuffer();
        drawStatusBar();

        switch (ui.currentView)
        {
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

        sendDisplayBuffer();
    }
}

void setView(uint8_t viewIndex)
{
    ui.currentView = viewIndex;
    ui.backKeyDown = false;

    updateView();
}

int getView(void)
{
    return ui.currentView;
}

void updateView(void)
{
    ui.updateView = true;
}
