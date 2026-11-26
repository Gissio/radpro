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

#if defined(PULSE_LED) || defined(PULSE_LED_EN)

void initLED(void)
{
#if defined(PULSE_LED)
    initPulseLED();
#else
    initPulseLEDEnable();
#endif
#if defined(ALERT_LED)
    initAlertLED();
#elif defined(ALERT_LED_EN)
    initAlertLEDEnable();
#endif

    updateLED();
}

void updateLED(void)
{
    LEDMode ledMode = LEDMODE_OFF;
    bool pulseLED = false;
#if defined(ALERT_LED) || defined(ALERT_LED_EN)
    bool alertLED = false;
#endif

    if (isMeasurementsEnabled())
    {
#if defined(ALERT_LED) || defined(ALERT_LED_EN)
        // Warning: pulse and alert LED; alarm: alert LED only
        if (settings.alertPulseLED && isAlertFlashing())
        {
            pulseLED = (getAlertLevel() == ALERTLEVEL_WARNING);
            alertLED = true;

            if (pulseLED)
                ledMode = LEDMODE_MULTIPLEX;
        }
#else
        // Pulse LED flashes during alerts
        if (settings.alertPulseLED && getAlertLevel())
            pulseLED = isAlertFlashing();
#endif
        else if (settings.pulseLED)
            ledMode = LEDMODE_PULSE;
    }

#if defined(PULSE_LED)
    setLEDMode(ledMode);
    // In pulse mode, the pulse LED is driven by the pulse LED timer
    if (ledMode != LEDMODE_PULSE)
        setPulseLED(pulseLED);
#else
    setPulseLEDEnable(pulseLED ||
                      ((ledMode == LEDMODE_PULSE) && isPulseThresholdExceeded()));
#endif
#if defined(ALERT_LED)
    setAlertLED(alertLED);
#elif defined(ALERT_LED_EN)
    setAlertLEDEnable(alertLED);
#endif
}

#endif
