/*
 * Rad Pro
 * STM32 Main module
 *
 * (C) 2022-2024 Gissio
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
#include "pulseled.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"
#include "vibrator.h"
#include "view.h"

int main(void)
{
    // Start primary devices

    initSystem();
    initEvents();
    initFlash();
    initSettings();
    initBuzzer();
    initComm();

    // POWER key delay

    sleep(500);
    setPower(true);

    // Start secondary devices

    initTube();
    initPower();
    initADC();
    initKeyboard();
#if defined(PULSE_LED)
    initPulseLED();
#endif
#if defined(VIBRATOR)
    initVibrator();
#endif
    initDisplay();

    if (!verifyFlash())
    {
        drawNotification("WARNING",
                         "Firmware checksum failure.", true);
        refreshDisplay();
        triggerBacklight();

        playSystemAlert();

        sleep(5000);
    }

#if defined(DEBUG_TESTMODE)

    debugTestMode();

#else
    initMeasurements();
    initGame();

    // Check low battery

    updateADC();

    // Welcome screen

    drawNotification(FIRMWARE_NAME, FIRMWARE_VERSION, true);
    refreshDisplay();
    triggerBacklight();

    sleep(1000);

    // Complete initialization

    initRTCHardware();
    initDatalog();

    setTubeHV(true);
    setEventHandling(true);
    startComm();

    // UI loop

    setMeasurementView(0);

    while (1)
    {
        sleep(1);

        updateGame();
        dispatchEvents();
    }
#endif
}
