/*
 * FS2011 Pro
 * Number formatting
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdio.h>
#include <string.h>

#include "format.h"
#include "cmath.h"
#include "settings.h"

static char getMetricPrefix(int exponent)
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
        if (metricPrefix < -4)
            return '-';
        else
            return '+';
    }
}

void strcatNumber(char *buffer, uint64_t value, uint32_t length)
{
    buffer += strlen(buffer);

    uint32_t numberOfDigits = 0;
    uint64_t shiftRegister = value;
    do
    {
        numberOfDigits++;
        shiftRegister /= 10;
    } while (shiftRegister);

    if (numberOfDigits > length)
        length = numberOfDigits;

    for (int i = (length - 1); i >= 0 ; i--)
    {
        buffer[i] = '0' + (value % 10);
        value /= 10;
    }

    buffer[length] = '\0';
}

void strcatTime(char *buffer, uint32_t time)
{
    buffer += strlen(buffer);

    uint32_t seconds = time % 60;
    uint32_t minutes = (time / 60) % 60;
    uint32_t hours = time / 3600;

    if (hours > 0)
    {
        strcatNumber(buffer, hours, 0);
        strcat(buffer, ":");
    }

    strcatNumber(buffer, minutes, 2);
    strcat(buffer, ":");
    strcatNumber(buffer, seconds, 2);
}

static void formatUnits(const char *unitName, int exponent,
                        char *characteristic)
{
    char metricPrefix = getMetricPrefix(exponent);

    if (metricPrefix == ' ')
        strcpy(characteristic, unitName);
    else
    {
        characteristic[0] = metricPrefix;
        strcpy(characteristic + 1, unitName);
    }
}

void formatMantissaAndCharacteristic(const char *unitName, float value, int minExponent,
                                     char *mantissaBuffer, char *characteristicBuffer)
{
    int exponent = getExponent(value);

    unsigned int mantissa = (uint32_t)(value * getPowerOfTen(3 - exponent) + 0.5F);
    if (mantissa >= 10000)
    {
        mantissa /= 10;
        exponent++;
    }

    if (exponent < minExponent)
    {
        mantissa /= (int)getPowerOfTen(minExponent - exponent);
        exponent = minExponent;
    }

    formatUnits(unitName, exponent, characteristicBuffer);

    mantissaBuffer[0] = '\0';

    int decimalPoint = remainderDown(exponent, 3);
    switch (decimalPoint)
    {
    case 0:
        strcatNumber(mantissaBuffer, mantissa / 1000, 0);
        strcat(mantissaBuffer, ".");
        strcatNumber(mantissaBuffer, mantissa % 1000, 3);
        break;

    case 1:
        strcatNumber(mantissaBuffer, mantissa / 100, 1);
        strcat(mantissaBuffer, ".");
        strcatNumber(mantissaBuffer, mantissa % 100, 2);
        break;

    case 2:
        strcatNumber(mantissaBuffer, mantissa / 10, 2);
        strcat(mantissaBuffer, ".");
        strcatNumber(mantissaBuffer, mantissa % 10, 1);
        break;
    }
}

void formatValue(const char *unitName, float value, char *buffer)
{
    int exponent = getExponent(value);

    char characteristicBuffer[16];
    formatUnits(unitName, exponent, characteristicBuffer);

    int decimalPoint = remainderDown(exponent, 3);
    buffer[0] = '\0';
    strcatNumber(buffer, (uint32_t)(value * getPowerOfTen(decimalPoint - exponent)), 0);
    strcat(buffer, " ");
    strcat(buffer, characteristicBuffer);
}

void formatMultiplier(const char *unitName, int exponent, char *buffer)
{
    float value = getPowerOfTen(exponent);

    formatValue(unitName, value, buffer);
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
        strcpy(mantissa, "-.---");
}

void formatDose(uint32_t count,
                char *mantissa, char *characteristic)
{
    Unit *unit = &units[settings.units].dose;

    formatMantissaAndCharacteristic(unit->name,
                                    unit->scale * count,
                                    unit->minExponent,
                                    mantissa,
                                    characteristic);
}
