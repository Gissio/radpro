/*
 * Rad Pro
 * String formatting
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <float.h>

#include "../system/cmath.h"
#include "../system/cstring.h"

#if defined(__EMSCRIPTEN__)

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

void strcatUInt32(char *s, uint32_t value, uint32_t minDigits)
{
    char buffer[16];
    char *p = buffer + sizeof(buffer) - 1;

    *p-- = '\0';

    uint32_t digits = 0;

    do
    {
        *p-- = '0' + (value % 10);
        value /= 10;
        digits++;
    } while (value || (digits < minDigits));

    strcat(s, p + 1);
}

void strcatTime(char *s, uint32_t time)
{
    uint32_t hours = time / 3600;
    uint32_t hoursRemainder = time % 3600;
    uint32_t minutes = hoursRemainder / 60;
    uint32_t seconds = hoursRemainder % 60;

    if (hours)
    {
        strcatUInt32(s, hours, 0);
        strcatChar(s, ':');
    }

    strcatUInt32(s, minutes, 2);
    strcatChar(s, ':');

    strcatUInt32(s, seconds, 2);
}

void strcatFloat(char *s, float value, uint32_t fractionalDecimals)
{
    if (value < 0.0F)
    {
        strcatChar(s, '-');
        value = -value;
    }

    uint32_t decimalPower = getDecimalPower(fractionalDecimals);

    uint32_t scaled = (uint32_t)(value * (float)decimalPower + 0.5F);

    uint32_t integerPart = scaled / decimalPower;
    uint32_t fractionalPart = scaled % decimalPower;

    strcatUInt32(s, integerPart, 0);

    if (fractionalDecimals)
    {
        strcatChar(s, '.');
        strcatUInt32(s, fractionalPart, fractionalDecimals);
    }
}

cstring metricPrefixStrings[8] = {
    STRING_NANO,
    STRING_MICRO,
    STRING_MILLI,
    STRING_EMPTY,
    STRING_KILO,
    STRING_MEGA,
    STRING_GIGA,
};

static void strcatMetricPrefix(char *s, int32_t prefixIndex)
{
    uint32_t index = prefixIndex + 3;
    if (index > 6)
        index = 3;
    strcat(s, getString(metricPrefixStrings[index]));
}

void strcatMetricValue(char *s, char *u, float value, int32_t minPrefixIndex)
{
    int32_t prefixIndex;

    if (value == 0.0F)
    {
        strcat(s, "‒.‒‒‒"); // Uses figure dash (U+2012)
        prefixIndex = 0;
    }
    else
    {
        int32_t valueExponent = roundDown(log10f(value / 0.99995F));
        prefixIndex = divideDown(valueExponent, 3);

        if (prefixIndex < minPrefixIndex)
            prefixIndex = minPrefixIndex;

        float scaled = value / powf(10.0F, 3.0F * prefixIndex);

        int32_t integerDigits = valueExponent - 3 * prefixIndex + 1;
        if (integerDigits < 1)
            integerDigits = 1;

        int32_t fractionalDecimals = 4 - integerDigits;
        if (fractionalDecimals < 0)
            fractionalDecimals = 0;

        strcatFloat(s, scaled, fractionalDecimals);
    }

    strcatMetricPrefix(u, prefixIndex);
}

void strcatMetricPower(char *s, int32_t exponent, int32_t minPrefixIndex)
{
    int32_t prefixIndex = divideDown(exponent, 3);
    uint32_t fractionalDecimals = 0;
    if (prefixIndex < minPrefixIndex)
    {
        prefixIndex = minPrefixIndex;
        fractionalDecimals = minPrefixIndex * 3 - exponent;
    }
    strcatFloat(s, powf(10.0F, exponent - 3 * prefixIndex), fractionalDecimals);
    if (prefixIndex)
        strcatChar(s, ' ');
    strcatMetricPrefix(s, prefixIndex);
}

const char *hexDigits = "0123456789abcdef";

static inline char getHexDigit(uint32_t value)
{
    return hexDigits[value];
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

void strcatHexData(char *s, uint8_t *data, uint32_t n)
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

    while (*t == ' ')
        t++;

    const char *m = match;

    while (*m && (*t == *m))
    {
        t++;
        m++;
    }

    if (!*m && (*t == '\0' || *t == ' '))
    {
        *s = t;

        return true;
    }

    return false;
}

bool parseUInt32(const char **s, uint32_t *value)
{
    const char *t = *s;

    while (*t == ' ')
        t++;

    uint32_t v = 0;
    const char *start = t;

    while (*t >= '0' && *t <= '9')
        v = v * 10 + (*t++ - '0');

    if (t == start)
        return false;
    if (*t != '\0' && *t != ' ')
        return false;

    *s = t;
    *value = v;

    return true;
}

bool parseFloat(const char **s, float *value)
{
    const char *t = *s;

    while (*t == ' ')
        t++;

    bool negate = 0;
    if (*t == '-')
    {
        negate = 1;
        t++;
    }

    const char *start = t;
    float m = 0.0F;
    float p = 1.0F;

    while (*t >= '0' && *t <= '9')
        m = m * 10.0F + (*t++ - '0');

    if (*t == '.')
    {
        t++;
        while (*t >= '0' && *t <= '9')
        {
            m = m * 10.0F + (*t++ - '0');
            p *= 10.0F;
        }
    }

    if (t == start)
        return false;
    if (*t != '\0' && *t != ' ')
        return false;

    *s = t;
    *value = (negate ? -m / p : m / p);

    return true;
}

static uint8_t parseHexNibble(char c)
{
    c -= '0';
    if (c > 9)
        c += 10 + ('0' - 'a');
    return c;
}

uint32_t parseHexData(const char **s, uint8_t *data)
{
    const char *t = *s;
    uint32_t n = 0;

    while (*t == ' ')
        t++;

    while (true)
    {
        uint8_t high = parseHexNibble(*t);
        if (high >= 16)
            break;
        t++;
        uint8_t low = parseHexNibble(*t);
        if (low >= 16)
            break;
        t++;
        data[n++] = (high << 4) | low;
    }

    *s = t;
    return n;
}
