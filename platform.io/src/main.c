/*
 * Rad Pro
 * STM32 Main module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "battery.h"
#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "firmware.h"
#include "flash.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "settings.h"
#include "menu.h"
#include "view.h"

int main(void)
{
    // Init base system
    initEvents();
    initBuzzer();

    // POWER key delay
    sleep(500);

    initPower();

    // Check firmware
    if (!checkFirmware())
        playSystemAlert();

    // Init system
    initSettings();
    initEventsSettings();
    initMeasurements();
    initGame();
    initGM();
    initBattery();
    initKeyboard();
    initDisplay();

    // Low battery?
    updateBattery();

    // Welcome screen
    clearDisplayBuffer();
    drawWelcome();
    sendDisplayBuffer();

    triggerBacklight();

    sleep(1000);

    // Initialize view
    enableMeasurement(true);
    setMeasurementView(&instantaneousRateView);

    // Message loop
    while (1)
    {
        sleep(1);

        updateGame();
        updateView();
    }
}
