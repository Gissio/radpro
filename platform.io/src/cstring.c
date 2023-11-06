/*
 * Rad Pro
 * Compact formatting
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <float.h>
#include <string.h>

#include "cstring.h"

void strcatUInt32(char *str, uint32_t value, uint32_t length)
{
    str += strlen(str);

    uint32_t n = 0;
    uint32_t x = value;
    do
    {
        n++;
        x /= 10;
    } while (x);

    if (n > length)
        length = n;

    for (int32_t i = (length - 1); i >= 0; i--)
    {
        str[i] = '0' + (value % 10);
        value /= 10;
    }

    str[length] = '\0';
}

bool parseUInt32(char *str, uint32_t *value)
{
    uint32_t shiftRegister = 0;

    while (true)
    {
        char c = *str++;

        if (c < 0x20)
        {
            *value = shiftRegister;

            return true;
        }
        else if (c < '0' || c > '9')
            return false;
        else if (shiftRegister > (UINT32_MAX / 10))
            return false;

        shiftRegister *= 10;
        shiftRegister += c - '0';
    }
}

void strcatTime(char *str, uint32_t time)
{
    str += strlen(str);

    uint32_t seconds = time % 60;
    uint32_t minutes = (time / 60) % 60;
    uint32_t hours = time / 3600;

    if (hours > 0)
    {
        strcatUInt32(str, hours, 0);
        strcat(str, ":");
    }

    strcatUInt32(str, minutes, 2);
    strcat(str, ":");
    strcatUInt32(str, seconds, 2);
}

static uint32_t getDecimalPower(uint32_t exponent)
{
    uint32_t value = 1;

    for (; exponent > 0; exponent--)
        value *= 10;

    return value;
}

void strcatFloat(char *str, float value, uint32_t fractionalDecimals)
{
    str += strlen(str);

    float decimalPower = (float)getDecimalPower(fractionalDecimals);

    value += 0.5F / decimalPower;

    uint32_t valueInt = (uint32_t)value;
    uint32_t valueFrac = (uint32_t)(decimalPower * (value - valueInt));

    strcatUInt32(str, valueInt, 0);
    if (fractionalDecimals)
    {
        strcat(str, ".");
        strcatUInt32(str, valueFrac, fractionalDecimals);
    }
}

struct MetricPower
{
    float decimalPower;
    float maxValue;
    char prefix[4];
};

const struct MetricPower metricPowers[] = {
    {1E-6F, 999.95E-6F, "\xb5"},
    {1E-3F, 999.95E-3F, "m"},
    {1E0F, 999.95E0F, ""},
    {1E3F, 999.95E3F, "k"},
    {1E6F, 999.95E6F, "M"},
    {1E9F, 999.95E9F, "G"},
    {FLT_MAX, FLT_MAX, ""},
};

#define METRIC_POWERS_NUM (sizeof(metricPowers) / sizeof(struct MetricPower))

void strcatFloatWithMetricPrefix(char *str, float value, uint32_t minMetricPower)
{
    str += strlen(str);

    float decimalPower = 0;
    const char *prefix = NULL;
    for (uint32_t i = minMetricPower; i < METRIC_POWERS_NUM; i++)
    {
        if (value < metricPowers[i].maxValue)
        {
            decimalPower = metricPowers[i].decimalPower;
            prefix = metricPowers[i].prefix;

            break;
        }
    }

    value /= decimalPower;

    uint32_t fractionalDecimals;
    if (value >= 99.995F)
        fractionalDecimals = 1;
    else if (value >= 9.9995F)
        fractionalDecimals = 2;
    else
        fractionalDecimals = 3;

    strcatFloat(str, value, fractionalDecimals);
    strcat(str, " ");
    strcat(str, prefix);
}

void strcatDecimalPowerWithMetricPrefix(char *str, int32_t exponent)
{
    str += strlen(str);

    uint32_t metricPowerIndex;
    if (exponent < -6)
    {
        metricPowerIndex = 0;
        strcat(str, "0.");
        for (int32_t i = 0; i < (-exponent - 7); i++)
            strcat(str, "0");
        strcat(str, "1");
    }
    else
    {
        uint32_t shiftedExponent = exponent + 6;
        metricPowerIndex = shiftedExponent / 3;
        if (metricPowerIndex >= METRIC_POWERS_NUM)
            metricPowerIndex = METRIC_POWERS_NUM - 1;
        int32_t decimals = shiftedExponent - metricPowerIndex * 3;

        strcat(str, "1");
        for (int32_t i = 0; i < decimals; i++)
            strcat(str, "0");
    }

    strcat(str, " ");
    strcat(str, metricPowers[metricPowerIndex].prefix);
}

static char getHexDigit(uint32_t value)
{
    return (value < 10) ? value + '0' : value + ('a' - 10);
}

static int32_t parseHexDigit(char c)
{
    if ((c >= '0') && (c <= '9'))
        return c - '0';
    else if ((c >= 'A') && (c <= 'F'))
        return c - 'A';
    else if ((c >= 'a') && (c <= 'f'))
        return c - 'a';
    else
        return -1;
}

void strcatUInt8Hex(char *str, uint8_t value)
{
    str += strlen(str);

    *str++ = getHexDigit((value >> 4) & 0xf);
    *str++ = getHexDigit((value >> 0) & 0xf);

    *str++ = '\0';
}

void strcatUInt16Hex(char *str, uint16_t value)
{
    strcatUInt8Hex(str, value >> 8);
    strcatUInt8Hex(str, value & 0xff);
}

void strcatUInt32Hex(char *str, uint32_t value)
{
    strcatUInt16Hex(str, value >> 16);
    strcatUInt16Hex(str, value & 0xffff);
}

void strcatDataHex(char *str, uint8_t *data, uint32_t n)
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

bool parseHexString(char *str, char *dest)
{
    while (true)
    {
        char c1, c2;

        c1 = *str++;
        if (c1 == '\0')
            return true;

        c2 = *str++;

        int32_t v1 = parseHexDigit(c1);
        if (v1 < 0)
            return false;

        int32_t v2 = parseHexDigit(c2);
        if (v2 < 0)
            return false;

        *dest++ = (v1 << 4) | v2;
    }
}
