/*
 * Rad Pro
 * Initialization
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "comm.h"
#include "cstring.h"
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
#include "rng.h"
#include "rtc.h"
#include "settings.h"
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
            drawNotification(getString(STRING_NOTIFICATION_WARNING),
                             getString(STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawNotification(getString(STRING_APP_NAME),
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
#if defined(DATA_MODE)
            if (settings.dataMode)
                openComm();
#elif !defined(PWR_USB)
            openComm();
#endif
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

void powerOn(void)
{
    // Power on
    setPower(true);

    // Initialize modules
    resetMeasurements();
#if defined(GAME)
    resetGame();
#endif
    resetDatalog();

    if (!verifyFlash())
    {
        powerOnViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlarm();
    }
    else
        powerOnViewState = POWERON_VIEW_SPLASH;

    requestDisplayBacklightTrigger();
    triggerVibration();
    startEvents();

    // Reset modules
    resetSettings();
    resetEvents();
    resetPower();
    resetTube();
    resetDisplay();
    resetDatalog();
    resetRTC();
    resetRNG();

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

void powerOff(void)
{
    // Stop measurements
    closeDatalog();
    writeSettings();

    closeComm();
    disableMeasurements();
    setTubeHV(false);
    cancelDisplayBacklight();

    // Power off
    setView(&powerOffView);
    setPower(false);
}

bool isPoweredOff(void)
{
    return (getView() == &powerOffView);
}
