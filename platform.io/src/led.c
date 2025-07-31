/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "events.h"
#include "led.h"
#include "measurements.h"
#include "power.h"

bool ledLastPulseLEDState;

void initLED(void)
{
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    initPulseLED();
#endif

#if defined(ALERT_LED) || defined(ALERT_LED_EN)
    initAlertLED();
#endif

    updateLED();
}

void updateLED(void)
{
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)

    AlertLevel alertLevel = getAlertLevel();

    bool pulseLEDEnabled = !isDeviceOff() &&
                           isPulseThresholdExceeded() &&
                           settings.pulseLED;
    bool pulseLEDState;

    // Get state
    if ((alertLevel > ALERTLEVEL_NONE) && !settings.alertPulseLED)
        alertLevel = ALERTLEVEL_NONE;

#if defined(PULSE_LED) && !defined(ALERT_LED)
    if (alertLevel == ALERTLEVEL_NONE)
        pulseLEDState = false;
    else
    {
        pulseLEDEnabled = false;
        pulseLEDState = isAlertBlink();
    }
#elif (defined(PULSE_LED) && defined(ALERT_LED)) || (defined(PULSE_LED_EN) && defined(ALERT_LED_EN))
    bool alertLEDState;

    if ((alertLevel > ALERTLEVEL_NONE) && !isAlertBlink())
        alertLevel = ALERTLEVEL_NONE;

    if (alertLevel == ALERTLEVEL_NONE)
    {
#if defined(PULSE_LED)
        pulseLEDState = false;
#else
        pulseLEDState = pulseLEDEnabled;
#endif
        alertLEDState = false;
    }
    else
    {
#if defined(PULSE_LED)
        pulseLEDEnabled = false;
#endif
        pulseLEDState = (alertLevel == ALERTLEVEL_WARNING);
        alertLEDState = true;
    }
#endif

    // Update state
#if defined(PULSE_LED)
    setPulseLEDEnabled(pulseLEDEnabled);
#endif
    if (pulseLEDState != ledLastPulseLEDState)
    {
#if defined(PULSE_LED)
        cancelPulseLEDTimer();
#endif
        setPulseLED(pulseLEDState);
        ledLastPulseLEDState = pulseLEDState;
    }
#if defined(ALERT_LED) || defined(ALERT_LED_EN)
    setAlertLED(alertLEDState);
#endif

#endif
}
