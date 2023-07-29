/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
#include "view.h"

void powerOff(void)
{
    writeDataLogEntry();
    writeSettings();

    // Disable devices
    enableMeasurements(false);
    stopBacklightTimer();
    stopBuzzerTimer();

    setDisplay(false);
    setPower(false);

    while (getKeyEvent() != KEY_EVENT_POWER_OFF)
        sleep(1);

    // Resume (when using external power supply)
    setPower(true);
    setDisplay(true);
    triggerBacklight();
    enableMeasurements(true);

    writeDataLogEntry();

    refreshView();
}

void powerOffLowBattery(void)
{
    enableMeasurements(false);
    stopBacklightTimer();
    stopBuzzerTimer();

    clearDisplayBuffer();
    drawLowBattery();
    sendDisplayBuffer();

    sleep(5000);

    powerOff();
}
