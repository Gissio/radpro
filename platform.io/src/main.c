/*
 * FS2011 Pro
 * STM32 Main module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "fwcheck.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "main.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "settings.h"
#include "menus.h"
#include "ui.h"

int main(void)
{
    // Init base system
    initEvents();
    initBuzzer();

    // POWER key delay
    waitSysTicks(500);

    // Check firmware
    checkFirmware();

    // Init system
    initPower();
    initSettings();
    initGM();
    initMeasurement();
    initMenus();
    initKeyboard();
    initDisplay();

    // Welcome screen
    clearDisplayBuffer();
    drawWelcome();
    sendDisplayBuffer();
    updateBacklight();

    waitSysTicks(1000);

    // Initialize view
    enableEvents();
    setView(VIEW_INSTANTANEOUS_RATE);

    // Message loop
    while (1)
    {
        waitSysTicks(1);

        updateGame();
        updateUI();
    }
}
