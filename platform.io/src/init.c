/*
 * Rad Pro
 * Initialization
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "comm.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "init.h"
#include "keyboard.h"
#include "measurements.h"
#include "rtc.h"
#include "power.h"
#include "system.h"
#include "tube.h"

// Init state

typedef enum
{
    POWERON_VIEW_FLASHFAILURE,
    POWERON_VIEW_SPLASH,
} PowerOnViewState;

static PowerOnViewState powerOnViewState;

// Power-on view

static void onPowerOnViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (powerOnViewState == POWERON_VIEW_FLASHFAILURE)
            drawNotification("WARNING",
                             "Firmware checksum failure.");
        else
            drawNotification(FIRMWARE_NAME,
                             FIRMWARE_VERSION);

        break;

    case EVENT_POST_DRAW:
        if (powerOnViewState == POWERON_VIEW_SPLASH)
            initRTC();

        break;

    case EVENT_PERIOD:
        if (powerOnViewState == POWERON_VIEW_FLASHFAILURE)
            powerOnViewState = POWERON_VIEW_SPLASH;
        else
        {
            // Start measurements
            setTubeHV(true);
            enableMeasurements();
            enableComm(true);
            openDatalog();

            setMeasurementView(0);
        }

    default:
        break;
    }
}

static const View powerOnView = {
    onPowerOnViewEvent,
    NULL,
};

void setPowerOnView(void)
{
    // Power on
    setPower(true);

    // Initialize modules
    initMeasurements();
#if defined(GAME)
    initGame();
#endif
    initDatalog();

    if (!verifyFlash()) 
    {
        powerOnViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlarm();
    }
    else
        powerOnViewState = POWERON_VIEW_SPLASH;

    requestDisplayBacklightTrigger();
    clearKeyboardEvents();
    triggerVibration();
    startEvents();

    setView(&powerOnView);
}

// Power-off view

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
    closeDatalog();
    writeSettings();

    enableComm(false);
    disableMeasurements();
    setTubeHV(false);
    cancelDisplayBacklight();

    // Power off
    setView(&powerOffView);
    setPower(false);
}

bool isPowerOffViewActive(void)
{
    return (getView() == &powerOffView);
}
