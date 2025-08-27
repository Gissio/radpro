/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)

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

    updateLED(false);
}

void updateLED(bool enabled)
{
    // Get state
    bool pulseLEDEnabled = isPulseThresholdExceeded() &&
                           settings.pulseLED &&
                           enabled;
    bool pulseLEDState;

    AlertLevel alertLevel = getAlertLevel();
    if (!settings.alertPulseLED ||
        !enabled)
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

    if ((alertLevel > ALERTLEVEL_NONE) &&
        !isAlertBlink())
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

    // Update pulse LED
#if defined(PULSE_LED)
    enablePulseLED(pulseLEDEnabled);
#endif
    if (pulseLEDState != ledLastPulseLEDState)
    {
#if defined(PULSE_LED)
        cancelPulseLED();
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
        setPulseLED(pulseLEDState);
#endif
        ledLastPulseLEDState = pulseLEDState;
    }

    // Update alert LED
#if defined(ALERT_LED) || defined(ALERT_LED_EN)
    setAlertLED(alertLEDState);
#endif
}

#endif
