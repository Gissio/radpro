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
#include "led.h"
#include "measurements.h"
#include "menu.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"
#include "vibrator.h"
#include "view.h"

int main(void)
{
    // System initialization
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
#if defined(PULSE_LED)
    initLED();
#endif
#if defined(VIBRATOR)
    initVibrator();
#endif

#if defined(TEST_MODE)

    setPower(true);
    runTestMode();

#else

    initMeasurements();
#if defined(GAME)
    initGame();
#endif
    initDatalog();

#if defined(KEYBOARD_WAIT_POWERON)
    sleep(1000);
#endif

    // Main loop
    while (true)
    {
        // Power on
        setPower(true);

        // Firmware checksum
        if (!verifyFlash())
        {
            drawNotification("WARNING",
                             "Firmware checksum failure.");
            refreshDisplay();
            triggerDisplay();
            setDisplayOn(true);

            playSystemAlert();

            sleep(1000);
        }

        // Splash screen
        drawNotification(FIRMWARE_NAME,
                         FIRMWARE_VERSION);
        refreshDisplay();
        triggerDisplay();
        setDisplayOn(true);
        uint32_t splashStartTime = getTick();
        initRTC();
        uint32_t splashTime = getTick() - splashStartTime;
        if (splashTime < 1000)
            splashTime = 1000;
        sleep(splashTime);

        // Enable devices
        setTubeHV(true);
        enableMeasurements();
        setCommEnabled(true);

        startDatalog();

        // Consume keyboard events
        while (getKeyboardEvent() != EVENT_NONE)
            ;

        // UI setup
        setMeasurementView(0);
        triggerDisplay();

        // UI loop
        while (!isPowerOffRequested())
        {
            sleep(1);

#if defined(GAME)
            dispatchGameEvents();
#endif
            dispatchEvents();
        }

        // Disable devices
        stopDatalog();
        writeSettings();

        setCommEnabled(false);
        disableMeasurements();
        setTubeHV(false);

        setDisplayBacklightOn(false);
        setDisplayOn(false);

        // Power off
        setPower(false);

        while (getKeyboardEvent() != EVENT_KEY_POWER)
            sleep(1);
    }

#endif
}
