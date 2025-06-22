/*
 * Rad Pro
 * Compact formatting
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <float.h>

#include "cmath.h"
#include "cstring.h"

#ifdef __EMSCRIPTEN__

void strclr(char *str)
{
    *str = '\0';
}

#endif

void strcatChar(char *str,
                char c)
{
    str += strlen(str);
    str[0] = c;
    str[1] = '\0';
}

void strcatUInt32(char *str,
                  uint32_t value,
                  uint32_t minLength)
{
    str += strlen(str);

    uint32_t n = 0;
    uint32_t x = value;
    do
    {
        n++;
        x /= 10;
    } while (x);

    if (n > minLength)
        minLength = n;

    for (int32_t i = (minLength - 1); i >= 0; i--)
    {
        str[i] = '0' + (value % 10);
        value /= 10;
    }

    str[minLength] = '\0';
}

void strcatTime(char *str,
                uint32_t time)
{
    uint32_t seconds = time % 60;
    uint32_t minutes = (time / 60) % 60;
    uint32_t hours = time / 3600;

    if (hours > 0)
    {
        strcatUInt32(str, hours, 0);
        strcatChar(str, ':');
    }

    strcatUInt32(str, minutes, 2);
    strcatChar(str, ':');
    strcatUInt32(str, seconds, 2);
}

void strcatFloat(char *str,
                 float value,
                 uint32_t fractionalDecimals)
{
    if (value < 0)
    {
        value = -value;

        strcatChar(str, '-');
    }

    float decimalPower = (float)getDecimalPower(fractionalDecimals);

    value += 0.5F / decimalPower;

    uint32_t valueInt = (uint32_t)value;
    uint32_t valueFrac = (uint32_t)(decimalPower * (value - valueInt));

    strcatUInt32(str, valueInt, 0);
    if (fractionalDecimals)
    {
        strcatChar(str, '.');
        strcatUInt32(str, valueFrac, fractionalDecimals);
    }
}

const char *metricPrefixes[8] = {
    getString(STRING_NANO),
    getString(STRING_MICRO),
    getString(STRING_MILLI),
    "",
    getString(STRING_KILO),
    getString(STRING_MEGA),
    getString(STRING_GIGA),
};

static void strcatMetricPrefix(char *str,
                               int32_t index)
{
    uint32_t metricIndex = index + 3;
    if (metricIndex > 6)
        metricIndex = 3;
    strcat(str, metricPrefixes[metricIndex]);
}

void strcatFloatAsMetricValueAndPrefix(char *str,
                                       char *metricPrefix,
                                       float value,
                                       int32_t minMetricPrefixIndex)
{
    float decimalPower = (value == 0) ? -38 : log10f(value);
    uint32_t shiftedExponent = decimalPower - log10f(0.999505F) + 39.0F;

    int32_t metricPrefixIndex = (shiftedExponent / 3) - (39 / 3);
    if (metricPrefixIndex < minMetricPrefixIndex)
        metricPrefixIndex = minMetricPrefixIndex;
    int32_t metricPower = 3 * metricPrefixIndex;

    int32_t fractionalDecimals = 3 - ((shiftedExponent - 39) - metricPower);
    if (fractionalDecimals > 3)
        fractionalDecimals = 3;

    if (value == 0.0F)
        strcat(str, "‒.‒‒‒"); // Note: uses figure dash! (U+2012)
    else
    {
        float metricValue = powf(10.0F, decimalPower - metricPower);

        strcatFloat(str, metricValue, fractionalDecimals);
    }
    strcatMetricPrefix(metricPrefix, metricPrefixIndex);
}

void strcatFloatAsMetricValueWithPrefix(char *str,
                                        float value,
                                        int32_t minMetricPrefixIndex)
{
    char metricPrefix[4];
    strclr(metricPrefix);

    strcatFloatAsMetricValueAndPrefix(str,
                                      metricPrefix,
                                      value,
                                      minMetricPrefixIndex);

    strcatChar(str, ' ');
    strcat(str, metricPrefix);
}

void strcatDecimalPowerWithMetricPrefix(char *str,
                                        int32_t exponent,
                                        int32_t minMetricPrefixIndex)
{
    uint32_t shiftedExponent = exponent + 39;
    int32_t metricPrefixIndex = (shiftedExponent / 3) - (39 / 3);

    if (metricPrefixIndex < minMetricPrefixIndex)
    {
        int32_t fractionalDecimals = (3 * minMetricPrefixIndex - exponent - 1);

        strcat(str, "0.");
        for (int32_t i = 0; i < fractionalDecimals; i++)
            strcatChar(str, '0');
        strcatChar(str, '1');

        metricPrefixIndex = minMetricPrefixIndex;
    }
    else
    {
        int32_t fractionalDecimals = (exponent - 3 * metricPrefixIndex);

        strcatChar(str, '1');
        for (int32_t i = 0; i < fractionalDecimals; i++)
            strcatChar(str, '0');
    }

    strcatChar(str, ' ');
    strcatMetricPrefix(str, metricPrefixIndex);
}

static char getHexDigit(uint32_t value)
{
    return (value < 10) ? value + '0' : value + ('a' - 10);
}

void strcatUInt8Hex(char *str,
                    uint8_t value)
{
    str += strlen(str);

    *str++ = getHexDigit((value >> 4) & 0xf);
    *str++ = getHexDigit((value >> 0) & 0xf);

    *str++ = '\0';
}

void strcatUInt16Hex(char *str,
                     uint16_t value)
{
    strcatUInt8Hex(str, value >> 8);
    strcatUInt8Hex(str, value & 0xff);
}

void strcatUInt32Hex(char *str,
                     uint32_t value)
{
    strcatUInt16Hex(str, value >> 16);
    strcatUInt16Hex(str, value & 0xffff);
}

void strcatDataHex(char *str,
                   uint8_t *data,
                   uint32_t n)
{
    str += strlen(str);

    for (uint32_t i = 0; i < n; i++)
    {
        uint8_t value = *data++;
        *str++ = getHexDigit((value >> 4) & 0xf);
        *str++ = getHexDigit((value >> 0) & 0xf);
    }

    *str++ = '\0';
}

bool parseUInt32(char *str,
                 uint32_t *value)
{
    uint32_t mantissa = 0;

    while (true)
    {
        char c = *str++;

        if (c < 0x20)
            break;
        else if ((c >= '0') && (c <= '9'))
            mantissa = 10 * mantissa + (c - '0');
        else
            return false;
    }

    *value = mantissa;

    return true;
}

bool parseFloat(char *str,
                float *value)
{
    float mantissa = 0;
    float divisor = 1;
    bool pastDecimalPoint = false;

    if (*str == '-')
    {
        str++;
        divisor = -1;
    }

    while (true)
    {
        char c = *str++;

        if (c < 0x20)
            break;
        else if (c == '.')
            pastDecimalPoint = true;
        else if ((c >= '0') && (c <= '9'))
        {
            mantissa = 10 * mantissa + (c - '0');
            if (pastDecimalPoint)
                divisor = 10 * divisor;
        }
        else
            return false;
    }

    *value = mantissa / divisor;

    return true;
}
