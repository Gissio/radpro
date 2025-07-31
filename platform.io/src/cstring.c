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

void strclr(char *s)
{
    *s = '\0';
}

#endif

void strcatChar(char *s, char c)
{
    s += strlen(s);
    s[0] = c;
    s[1] = '\0';
}

void strcatUInt32(char *s,
                  uint32_t value,
                  uint32_t minLength)
{
    s += strlen(s);

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
        s[i] = '0' + (value % 10);
        value /= 10;
    }

    s[minLength] = '\0';
}

void strcatTime(char *s, uint32_t time)
{
    uint32_t seconds = time % 60;
    uint32_t minutes = (time / 60) % 60;
    uint32_t hours = time / 3600;

    if (hours > 0)
    {
        strcatUInt32(s, hours, 0);
        strcatChar(s, ':');
    }

    strcatUInt32(s, minutes, 2);
    strcatChar(s, ':');
    strcatUInt32(s, seconds, 2);
}

void strcatFloat(char *s,
                 float value,
                 uint32_t fractionalDecimals)
{
    if (value < 0)
    {
        value = -value;

        strcatChar(s, '-');
    }

    float decimalPower = (float)getDecimalPower(fractionalDecimals);

    value += 0.5F / decimalPower;

    uint32_t valueInt = (uint32_t)value;
    uint32_t valueFrac = (uint32_t)(decimalPower * (value - valueInt));

    strcatUInt32(s, valueInt, 0);
    if (fractionalDecimals)
    {
        strcatChar(s, '.');
        strcatUInt32(s, valueFrac, fractionalDecimals);
    }
}

cstring metricPrefixes[8] = {
    STRING_NANO,
    STRING_MICRO,
    STRING_MILLI,
    STRING_EMPTY,
    STRING_KILO,
    STRING_MEGA,
    STRING_GIGA,
};

static void strcatMetricPrefix(char *s, int32_t index)
{
    uint32_t metricIndex = index + 3;
    if (metricIndex > 6)
        metricIndex = 3;
    strcat(s, getString(metricPrefixes[metricIndex]));
}

void strcatFloatAsMetricValueAndPrefix(char *s,
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
        strcat(s, "‒.‒‒‒"); // Note: uses figure dash! (U+2012)
    else
    {
        float metricValue = powf(10.0F, decimalPower - metricPower);

        strcatFloat(s, metricValue, fractionalDecimals);
    }
    strcatMetricPrefix(metricPrefix, metricPrefixIndex);
}

void strcatDecimalPowerWithMetricPrefix(char *s,
                                        int32_t exponent,
                                        int32_t minMetricPrefixIndex)
{
    uint32_t shiftedExponent = exponent + 39;
    int32_t metricPrefixIndex = (shiftedExponent / 3) - (39 / 3);

    if (metricPrefixIndex < minMetricPrefixIndex)
    {
        int32_t fractionalDecimals = (3 * minMetricPrefixIndex - exponent - 1);

        strcat(s, "0.");
        for (int32_t i = 0; i < fractionalDecimals; i++)
            strcatChar(s, '0');
        strcatChar(s, '1');

        metricPrefixIndex = minMetricPrefixIndex;
    }
    else
    {
        int32_t fractionalDecimals = (exponent - 3 * metricPrefixIndex);

        strcatChar(s, '1');
        for (int32_t i = 0; i < fractionalDecimals; i++)
            strcatChar(s, '0');
    }

    strcatChar(s, ' ');
    strcatMetricPrefix(s, metricPrefixIndex);
}

static char getHexDigit(uint32_t value)
{
    return (value < 10) ? value + '0' : value + ('a' - 10);
}

void strcatUInt8Hex(char *s, uint8_t value)
{
    s += strlen(s);

    *s++ = getHexDigit((value >> 4) & 0xf);
    *s++ = getHexDigit((value >> 0) & 0xf);

    *s++ = '\0';
}

void strcatUInt16Hex(char *s, uint16_t value)
{
    strcatUInt8Hex(s, value >> 8);
    strcatUInt8Hex(s, value & 0xff);
}

void strcatUInt32Hex(char *s, uint32_t value)
{
    strcatUInt16Hex(s, value >> 16);
    strcatUInt16Hex(s, value & 0xffff);
}

void strcatDataHex(char *s,
                   uint8_t *data,
                   uint32_t n)
{
    s += strlen(s);

    for (uint32_t i = 0; i < n; i++)
    {
        uint8_t value = *data++;
        *s++ = getHexDigit((value >> 4) & 0xf);
        *s++ = getHexDigit((value >> 0) & 0xf);
    }

    *s++ = '\0';
}

bool parseToken(const char **s, const char *match)
{
    const char *t = *s;

    // Skip space
    while (*t == ' ')
        t++;

    while (true)
    {
        char c = *t++;
        char cMatch = *match++;

        if (cMatch == '\0')
        {
            if ((c == '\0') || (c == ' '))
            {
                *s = t;

                return true;
            }
            else
                return false;
        }
        else if (c != cMatch)
            return false;
    }
}

bool parseUInt32(const char **s, uint32_t *value)
{
    const char *t = *s;

    // Skip space
    while (*t == ' ')
        t++;

    uint32_t mantissa = 0;
    bool parsedDigit = false;

    while (true)
    {
        char c = *t++;

        if ((c == '\0') || c == ' ')
        {
            if (parsedDigit)
            {
                *s = t;
                *value = mantissa;

                return true;
            }
            else
                return false;
        }
        else if ((c >= '0') && (c <= '9'))
        {
            mantissa = 10 * mantissa + (c - '0');

            parsedDigit = true;
        }
        else
            return false;
    }
}

bool parseFloat(const char **s, float *value)
{
    const char *t = *s;

    // Skip space
    while (*t == ' ')
        t++;

    float mantissa = 0;
    float divisor = 1;
    bool pastDecimalPoint = false;
    bool parsedDigit = false;

    // Parse minus sign
    if (*t == '-')
    {
        t++;
        divisor = -1;
    }

    while (true)
    {
        char c = *t++;

        if ((c == '\0') || c == ' ')
        {
            if (parsedDigit)
            {
                *s = t;
                *value = mantissa / divisor;

                return true;
            }
            else
                return false;
        }
        else if (c == '.')
        {
            if (!parsedDigit)
                return false;

            pastDecimalPoint = true;
        }
        else if ((c >= '0') && (c <= '9'))
        {
            mantissa = 10 * mantissa + (c - '0');

            if (pastDecimalPoint)
                divisor = 10 * divisor;

            parsedDigit = true;
        }
        else
            return false;
    }
}
