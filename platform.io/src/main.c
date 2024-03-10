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
    // Initialize system

    initSystem();
    initEvents();
    initFlash();
    initSettings();
    initPower();
    initComm();
    initADC();
    initTube();
    initKeyboard();
    initBuzzer();
    initDisplay();
#if defined(PULSELED)
    initPulseLED();
#endif
#if defined(VIBRATOR)
    initVibrator();
#endif

#if defined(TEST_MODE)

    setPower(true);
    runTestMode();

#else

    initMeasurements();
    initGame();
    initDatalog();

    // Power loop

    while (true)
    {
#if defined(KEYBOARD_WAIT_FOR_POWERON)

        while (getKeyboardEvent() != EVENT_KEY_POWER)
            sleep(1);

#endif

        // Power on

        setPower(true);

        if (!verifyFlash())
        {
            drawNotification("WARNING",
                             "Firmware checksum failure.",
                             true);
            refreshDisplay();
            setDisplayOn(true);
            setDisplayBacklight(true);

            playSystemAlert();

            sleep(1000);
        }

        drawNotification(FIRMWARE_NAME,
                         FIRMWARE_VERSION,
                         true);
        refreshDisplay();
        setDisplayOn(true);
        setDisplayBacklight(true);

        uint32_t splashStartTime = getTick();

        initRTC();

        uint32_t splashTime = getTick() - splashStartTime;
        if (splashTime < 1000)
            splashTime = 1000;
        sleep(splashTime);

        setTubeHV(true);
        enableMeasurements();
        setCommEnabled(true);

        writeDatalog();

        // UI loop

        setMeasurementView(0);
        triggerDisplay();

        while (!isPowerOffRequested())
        {
            sleep(1);

            dispatchGameEvents();
            dispatchEvents();
        }

        // Power off

        writeDatalog();
        writeSettings();

        setCommEnabled(false);
        disableMeasurements();
        setTubeHV(false);

        setDisplayBacklight(false);
        setDisplayOn(false);

        setPower(false);

#if !defined(KEYBOARD_WAIT_FOR_POWERON)
        while (getKeyboardEvent() != EVENT_KEY_POWER)
            sleep(1);
#endif
    }

#endif
}
