/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "comm.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "init.h"
#include "keyboard.h"
#include "measurements.h"
#include "rtc.h"
#include "power.h"
#include "system.h"
#include "tube.h"

typedef enum
{
    INIT_START,
    INIT_FLASHFAILURE,
    INIT_SPLASH,
    INIT_DONE,
} InitState;

static InitState initState = INIT_START;

// Init view

static void onInitViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_PERIOD:
        if ((initState == INIT_START) && verifyFlash())
            initState = INIT_FLASHFAILURE;

        if (initState == INIT_START)
        {
            initState = INIT_FLASHFAILURE;

            drawNotification("WARNING",
                             "Firmware checksum failure.");
            playSystemAlert();
        }
        else if (initState == INIT_FLASHFAILURE)
        {
            initState = INIT_SPLASH;

            drawNotification(FIRMWARE_NAME,
                             FIRMWARE_VERSION);
            initRTC();
        }
        else
        {
            initState = INIT_DONE;

            // Start measurements
            setTubeHV(true);
            enableMeasurements();
            setCommEnabled(true);
            startDatalog();

            setMeasurementView(0);
        }
        break;

    default:
        break;
    }
}

static const View initView = {
    onInitViewEvent,
    NULL,
};

void setSplashView(void)
{
    // Clear screen
    drawNotification("", "");

    // Power on
    setPower(true);
    setDisplayOn(true);
    triggerDisplay();
    initEvents();

    // Consume keyboard events
    while (getKeyboardEvent() != EVENT_NONE)
        ;

    initState = INIT_START;
    setView(&initView);
}

// Deinit view

static void onPowerOffViewEvent(const View *view, Event event)
{
}

static const View powerOffView = {
    onPowerOffViewEvent,
    NULL,
};

void setPowerOffView(void)
{
    // Stop measurements
    stopDatalog();
    writeSettings();

    setCommEnabled(false);
    disableMeasurements();
    setTubeHV(false);

    setDisplayBacklightOn(false);
    setDisplayOn(false);

    // Power off
    setPower(false);
    setView(&powerOffView);
}

// Getters

bool isPowerOffViewEnabled(void)
{
    return (getView() == &powerOffView);
}
