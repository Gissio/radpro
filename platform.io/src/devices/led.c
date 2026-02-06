/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../devices/led.h"
#include "../measurements/measurements.h"
#include "../measurements/pulses.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"

#if defined(PULSE_LED)

void initLED(void)
{
    initPulseLED();
#if defined(ALERT_LED)
    initAlertLED();
#endif

    updateLED();
}

void updateLED(void)
{
    bool pulseLEDIndication;
    bool pulseLED;

#if !defined(ALERT_LED)
    if (!isPoweredOn())
    {
        pulseLEDIndication = false;
        pulseLED = false;
    }
    else
    {
        if (settings.alertPulseLED && getAlertLevel())
        {
            pulseLEDIndication = false;
            pulseLED = isAlertFlashing();
        }
        else
        {
            pulseLEDIndication = settings.pulseLED;
            pulseLED = false;
        }
    }
#else
    bool alertLED;

    if (!isPoweredOn())
    {
        pulseLEDIndication = false;
        pulseLED = false;
        alertLED = false;
    }
    else
    {
        if (settings.alertPulseLED && isAlertFlashing())
        {
            pulseLEDIndication = false;
            pulseLED = (getAlertLevel() == ALERTLEVEL_WARNING);
            alertLED = true;
        }
        else
        {
            pulseLEDIndication = settings.pulseLED;
            pulseLED = false;
            alertLED = false;
        }
    }
#endif

    setPulseLEDIndication(pulseLEDIndication);
    setPulseLED(pulseLED);
#if defined(ALERT_LED)
    setAlertLED(alertLED);
#endif
}

#elif defined(PULSE_LED_EN)

void initLED(void)
{
    initPulseLEDEnable();
#if defined(ALERT_LED_EN)
    initAlertLEDEnable();
#endif

    updateLED();
}

void updateLED(void)
{
    bool pulseLEDEnable;

#if !defined(ALERT_LED_EN)
    pulseLEDEnable = settings.pulseLED && isPoweredOn();
#else
    bool alertLEDEnable;

    if (!isPoweredOn())
    {
        pulseLEDEnable = false;
        alertLEDEnable = false;
    }
    else
    {
        if (settings.alertPulseLED && isAlertFlashing())
        {
            pulseLEDEnable = (getAlertLevel() == ALERTLEVEL_WARNING);
            alertLEDEnable = true;
        }
        else
        {
            pulseLEDEnable = settings.pulseLED && isPulseThresholdExceeded();
            alertLEDEnable = false;
        }
    }
#endif

    setPulseLEDEnable(pulseLEDEnable);
#if defined(ALERT_LED_EN)
    setAlertLEDEnable(alertLEDEnable);
#endif
}

#endif
