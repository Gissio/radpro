// Rad Pro measurement firmware check
//
// Host program around the firmware's instantaneous.c (included below). The
// other firmware functions it needs (calculateRate,
// getDeadTimeCompensationFactor, getMeasurementAlertLevel,
// updateMeasurementAlertLevel) are copied verbatim from the sources by
// check.py; everything else is a stub.
//
// Usage: harness <instantaneousAveraging> <warningThresholdCps> [deadTime] < counts
// Reads per-second pulse counts from stdin and prints per second:
// rate confidence alertLevel alertArmed
// The warning is at the given threshold, the alarm at 3x the threshold.

#include <stdio.h>
#include <stdlib.h>

#include "measurements/instantaneous.c"

Settings settings;
PulseUnits pulseUnits[4] = {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};
const float rateAlerts[16] = {0, 1, 3};
const int8_t doseUnitsBarMinExponent[4];

static bool alertArmed;
static float tubeDeadTimeCompensation;

void setAlertPending(bool value)
{
    alertArmed |= value;
}

float getTubeDeadTimeCompensation(void)
{
    return tubeDeadTimeCompensation;
}

// Stubs
void updatePulseThresholdExceeded(void) {}
uint32_t updateMeasurementAlertTab(uint32_t tab, uint32_t alertTab, bool alertTriggered, bool alertActive) { return tab; }
uint32_t getNextMeasurementTab(uint32_t tab, uint32_t tabNum) { return 0; }
bool isTubeFaultAlertTriggered(void) { return false; }
AlertLevel getTubeFaultAlertLevel(void) { return ALERTLEVEL_NONE; }
MeasurementStyle getAlertMeasurementStyle(AlertLevel alertLevel) { return 0; }
uint32_t getCumulativeDosePulseCount(void) { return 0; }
void selectMenuItem(const Menu *menu, menu_size_t index) {}
void showMeasurementsMenu(void) {}
void drawPulseMeasurementInfo(const char *keyString, float value, uint32_t doseUnits, bool isDose, MeasurementStyle style) {}
void drawPulseMeasurementValue(const char *title, float value, bool isDose, float confidence, MeasurementStyle style) {}
void drawMeasurementBar(float value, int32_t minExponent, float warning, float alarm) {}
void drawMeasurementAlert(const char *alertString) {}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: harness <instantaneousAveraging> <warningThresholdCps> [deadTime] < counts\n");

        return 1;
    }

    settings.instantaneousAveraging = atoi(argv[1]);
    settings.rateWarning = 1;
    settings.rateAlarm = 2;
    // Readings in units of the warning threshold
    pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale = (float)(1.0 / atof(argv[2]));
    if (argc > 3)
        tubeDeadTimeCompensation = (float)atof(argv[3]);

    resetInstantaneousRate();

    unsigned long count;
    while (scanf("%lu", &count) == 1)
    {
        alertArmed = false;
        updateInstantaneousRate((uint32_t)count);
        printf("%.9g %.9g %d %d\n", instantaneous.rate.value, instantaneous.rate.confidence,
               (int)instantaneous.alertLevel, (int)alertArmed);
    }

    return 0;
}
