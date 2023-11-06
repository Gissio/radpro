/*
 * Rad Pro
 * ADC
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "display.h"
#include "events.h"
#include "power.h"

void updateADC(void)
{
    updateADCHardware();

    if (isLowBattery())
    {
        stopEvents();

        triggerDisplay();
        clearDisplayBuffer();
        drawLowBattery();
        sendDisplayBuffer();

        sleep(5000);

        powerOff();
    }
}
