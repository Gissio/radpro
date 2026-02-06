/*
 * Rad Pro
 * EMF meter
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(EMFMETER)

#include "../devices/emf.h"
#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../system/power.h"

static struct
{
    bool usbPoweredLast;

    bool available;

    bool unavailableTriggered;
} emf;

void updateEMFMeter(void)
{
    bool usbPowered = isUSBPowered();
    bool available = (!emf.usbPoweredLast && !usbPowered);
    emf.unavailableTriggered = emf.available && !available;

    emf.usbPoweredLast = usbPowered;
    emf.available = available;

    setEMFMeterEnabled(!usbPowered);

    updateElectricField();
    updateMagneticField();
}

bool isEMFMeterAvailable(void)
{
    return emf.available;
}

bool isEMFMeterUnavailableTriggered(void)
{
    return emf.unavailableTriggered;
}

#endif
