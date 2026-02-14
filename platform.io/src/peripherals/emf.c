/*
 * Rad Pro
 * EMF meter
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(EMFMETER)

#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../peripherals/emf.h"

void updateEMFMeter(void)
{
    updateElectricField();
    updateMagneticField();
}

#endif
