/*
 * FS2011 Pro
 * Number formatting
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "format.h"
#include "cmath.h"
#include "settings.h"

char getMetricPrefix(int exponent)
{
    int metricPrefix = divideDown(exponent, 3);
    switch (metricPrefix)
    {
    case -3:
        return 'n';

    case -2:
        return '\x7f';

    case -1:
        return 'm';

    case 0:
        return ' ';

    case 1:
        return 'k';

    case 2:
        return 'M';

    case 3:
        return 'G';

    default:
        return '?';
    }
}

const char *const formatHexDigit = "0123456789abcdef";

void formatHex(uint32_t value, char *buffer)
{
    for (int i = 7; i >= 0; i--)
    {
        buffer[i] = formatHexDigit[value & 0xf];
        value >>= 4;
    }

    buffer[8] = '\0';
}

void formatUnsignedLongLong(uint64_t value, char *buffer)
{
    uint64_t shiftRegister = value;

    do
    {
        buffer++;
        shiftRegister /= 10;
    } while (shiftRegister);

    *buffer = '\0';

    do
    {
        *--buffer = '0' + (value % 10);
        value /= 10;
    } while (value);
}

void formatUnits(const char *unitName, int exponent,
                 char *characteristic)
{
    char metricPrefix = getMetricPrefix(exponent);

    if (metricPrefix == ' ')
        sprintf(characteristic, "%s", unitName);
    else
        sprintf(characteristic, "%c%s", metricPrefix, unitName);
}

void formatMantissaAndCharacteristic(const char *unitName, float value, int minExponent,
                                     char *mantissaBuffer, char *characteristicBuffer)
{
    int exponent = getExponent(value);

    if (exponent < minExponent)
        exponent = minExponent;

    formatUnits(unitName, exponent, characteristicBuffer);

    int mantissa = (int)(value * getPowerOfTen(3 - exponent));
    int decimalPoint = remainderDown(exponent, 3);
    switch (decimalPoint)
    {
    case 0:
        sprintf(mantissaBuffer, "%d.%03d",
                mantissa / 1000, mantissa % 1000);
        break;

    case 1:
        sprintf(mantissaBuffer, "%d.%02d",
                mantissa / 100, mantissa % 100);
        break;

    case 2:
        sprintf(mantissaBuffer, "%d.%01d",
                mantissa / 10, mantissa % 10);
        break;
    }
}

void formatValue(const char *unitName, float value, int minExponent,
                 char *buffer)
{
    int exponent = getExponent(value);

    char characteristicBuffer[16];
    formatUnits(unitName, exponent, characteristicBuffer);

    int decimalPoint = remainderDown(exponent, 3);
    sprintf(buffer, "%d %s", (int)(value * getPowerOfTen(decimalPoint - exponent)), characteristicBuffer);
}

void formatMultiplier(const char *unitName, int exponent, int minExponent,
                      char *buffer)
{
    float value = getPowerOfTen(exponent);

    formatValue(unitName, value, minExponent, buffer);
}

void formatRate(float rate,
                char *mantissa, char *characteristic)
{
    Unit *unit = &units[settings.units].rate;

    formatMantissaAndCharacteristic(unit->name,
                                    unit->scale * rate,
                                    unit->minExponent,
                                    mantissa,
                                    characteristic);

    if (rate == 0)
        sprintf(mantissa, "-.---");
}

void formatDose(uint32_t count,
                char *mantissa, char *characteristic)
{
    Unit *unit = &units[settings.units].dose;

    formatMantissaAndCharacteristic(unit->name,
                                    unit->scale * count,
                                    unit->minExponent,
                                    mantissa, characteristic);
}

void formatTime(uint32_t time,
                char *buffer)
{
    unsigned int secs = time % 60;
    unsigned int mins = (time / 60) % 60;
    unsigned int hours = time / 3600;

    if (hours == 0)
        sprintf(buffer, "%02u:%02u", mins, secs);
    else
        sprintf(buffer, "%u:%02u:%02u", hours, mins, secs);
}
