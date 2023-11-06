/*
 * Rad Pro
 * STM32 Main module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "buzzer.h"
#include "comm.h"
#include "datalog.h"
#include "debug.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "keyboard.h"
#include "measurements.h"
#include "menu.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"
#include "view.h"

int main(void)
{
    // Startup

    initSystem();

#if defined(SDLSIM)

    initFlash();

#endif

    initEvents();
    initSettings();
    initBuzzer();

    // POWER key delay

    sleep(500);
    setPower(true);

    // Start secondary systems

#if defined(CHECK_FIRMWARE)

    if (!checkFirmware())
        playSystemAlert();

#endif

    initTube();
    initADC();
    initPower();
    initKeyboard();
    initDisplay();

#if defined(DEBUG_TESTMODE)

    debugTestMode();

#else
    initComm();
    initMeasurements();
    initGame();

    // Check low battery

    updateADC();

    // Welcome screen

    clearDisplayBuffer();
    drawWelcome();
    sendDisplayBuffer();
    triggerDisplay();
    setDisplay(true);

    sleep(1000);

    initRTC();
    initDatalog();
    startEvents();

    // UI loop

    setMeasurementView(0);

    while (1)
    {
        sleep(1);

        updateGame();
        updateEvents();
    }
#endif
}
