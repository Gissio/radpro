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

// +++ TEST
// #include <libopencm3/stm32/gpio.h>

// void initBlink(void)
// {
//     gpio_set_mode(GPIOC,
//                   GPIO_MODE_OUTPUT_2_MHZ,
//                   GPIO_CNF_OUTPUT_PUSHPULL,
//                   GPIO13);
// }

// void blink(void)
// {
//     gpio_clear(GPIOC,
//                GPIO13);

//     sleep(100);

//     gpio_set(GPIOC,
//              GPIO13);

//     sleep(400);
// }
// +++ TEST

int main(void)
{
    // Initialize system

    initSystem();
    initEvents();
    initFlash();
    initSettings();
    initPower();
    initComm();
    initRTC();
    initADC();
    initTube();
    initKeyboard();
    initBuzzer();
    initDisplay();
#if defined(PULSE_LED)
    initPulseLED();
#endif
#if defined(VIBRATOR)
    initVibrator();
#endif

#if defined(DEBUG_TESTMODE)

    runTestMode();

#else

    initMeasurements();
    initGame();
    initDatalog();

    // Power loop

    while (true)
    {
        // Wait for power key event

#if defined(SDLSIM)

        static bool firstStart = true;
        while ((getKeyboardEvent() != EVENT_KEY_POWER) && !firstStart)
        {
            sleep(1);

            dispatchDisplayEvents();
        }
        firstStart = false;

#else

        while (getKeyboardEvent() != EVENT_KEY_POWER)
        {
            sleep(1);

            dispatchDisplayEvents();
        }

#endif

        // Power on

        setPower(true);

        if (!verifyFlash())
        {
            drawNotification("WARNING",
                             "Firmware checksum failure.",
                             true);
            refreshDisplay();
            triggerDisplay();
#if defined(DISPLAY_MONOCHROME)
        setDisplay(true);
#endif

            playSystemAlert();

            sleep(1000);
        }

        drawNotification(FIRMWARE_NAME,
                         FIRMWARE_VERSION,
                         true);
        refreshDisplay();
        triggerDisplay();
#if defined(DISPLAY_MONOCHROME)
        setDisplay(true);
#endif

        sleep(1000);

        setTubeHV(true);
        setEventHandling(true);
        setCommEnabled(true);

        writeDatalog();

        // UI loop

        setMeasurementView(0);

        while (!isPowerOffRequested())
        {
            sleep(1);

            updateGame();
            dispatchEvents();
        }

        setPowerOffRequest(false);

        // Power off

        writeDatalog();
        writeSettings();

        setCommEnabled(false);
        setEventHandling(false);
        setTubeHV(false);

#if defined(DISPLAY_MONOCHROME)
        setDisplay(false);
#endif

        setPower(false);
    }

#endif
}
