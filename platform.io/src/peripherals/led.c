/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/measurements.h"
#include "../measurements/pulses.h"
#include "../peripherals/led.h"
#include "../system/events.h"
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
#if !defined(ALERT_LED)
    LEDMode ledMode = LEDMODE_OFF;
    bool pulseLED = false;

    if (isMeasurementsEnabled())
    {
        if (settings.alertPulseLED && getAlertLevel())
            pulseLED = isAlertFlashing();
        else
            ledMode = settings.pulseLED ? LEDMODE_PULSE : LEDMODE_OFF;
    }

    setLEDMode(ledMode);
    setPulseLED(pulseLED);
#else
    LEDMode ledMode = LEDMODE_OFF;
    bool pulseLED = false;
    bool alertLED = false;

    if (isMeasurementsEnabled())
    {
        if (settings.alertPulseLED && isAlertFlashing())
        {
            pulseLED = (getAlertLevel() == ALERTLEVEL_WARNING);
            alertLED = true;

            if (pulseLED && alertLED)
                ledMode = LEDMODE_MULTIPLEX;
        }
        else
            ledMode = settings.pulseLED ? LEDMODE_PULSE : LEDMODE_OFF;
    }

    setLEDMode(ledMode);
    setPulseLED(pulseLED);
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
    pulseLEDEnable = settings.pulseLED && isMeasurementsEnabled();
#else
    bool alertLEDEnable;

    if (!isMeasurementsEnabled())
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
