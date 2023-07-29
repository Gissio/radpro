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
#include "comm.h"
#include "debug.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "measurements.h"
#include "menu.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "view.h"

int main(void)
{
    // Init base systems
    initSystem();
    initEvents();
    initBuzzer();
debugBeep();

    // POWER key delay
    sleep(500);

    initPower();
debugBeep();

    // Check firmware
    if (!checkFirmware())
        playSystemAlert();

    // Init subsystems
    initSettings();
debugBeep();
    initMeasurements();
debugBeep();
    initGame();
debugBeep();
    initGM();
debugBeep();
    initBattery();
debugBeep();
    initKeyboard();
debugBeep();
    initDisplay();
debugBeep();
    initComm();
debugBeep();

    // Start tasks
    updateBattery(); // Check low battery
debugBeep();
    updateEventsMenus();
debugBeep();

    clearDisplayBuffer();
    drawWelcome();
    sendDisplayBuffer();
debugBeep();

    triggerBacklight();
debugBeep();

    sleep(1000);
debugBeep();

    initRTC();
debugBeep();
    writeDataLogEntry();
debugBeep();
    enableMeasurements(true);
debugBeep();
    setMeasurementView(&instantaneousRateView);
debugBeep();

    // UI loop
    while (1)
    {
        sleep(1);

        updateGame();
        updateView();
    }
}
