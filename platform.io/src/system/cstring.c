/*
 * Rad Pro
 * String formatting
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../system/cmath.h"
#include "../system/cstring.h"

void strcatChar(char *s, char c)
{
    s += strlen(s);
    s[0] = c;
    s[1] = '\0';
}

// Appends value with at least minDigits digits (at most 10) and, if
// pointDigits is non-zero, a decimal point before the last pointDigits
// digits (needs minDigits > pointDigits)
NOINLINE static void strcatDecimal(char *s, uint32_t value, uint32_t minDigits, uint32_t pointDigits)
{
    char buffer[12]; // 10 digits, point, terminator
    char *p = buffer + sizeof(buffer) - 1;

    *p = '\0';

    if (minDigits > 10)
        minDigits = 10;

    uint32_t digits = 0;

    do
    {
        // Remainder by multiplication: one division call on MCUs without a
        // hardware divider
        uint32_t quotient = value / 10;
        *--p = '0' + (value - 10 * quotient);
        value = quotient;

        if (++digits == pointDigits)
            *--p = '.';
    } while (value || (digits < minDigits));

    strcat(s, p);
}

void strcatUInt32(char *s, uint32_t value, uint32_t minDigits)
{
    strcatDecimal(s, value, minDigits, 0);
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
    // Sign bit as integer: no soft-float compare calls
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    bool negative = bits >> 31;
    bits &= ~(1U << 31);
    memcpy(&value, &bits, sizeof(value));

    // 10^9 is the largest decimal power that fits in 32 bits
    if (fractionalDecimals > 9)
        fractionalDecimals = 9;

    // Drop fractional decimals until the rounded value fits in 32 bits
    while (true)
    {
        float scaledValue = value * (float)getDecimalPower(fractionalDecimals) + 0.5F;

        if (scaledValue < 4294967296.0F)
        {
            uint32_t scaled = (uint32_t)scaledValue;

            // Values that round to zero print without sign (no "-0.00")
            if (negative && scaled)
                strcatChar(s, '-');

            strcatDecimal(s, scaled, fractionalDecimals + 1, fractionalDecimals);

            return;
        }

        if (!fractionalDecimals)
        {
            // >= 2^32, inf or NaN (exponent all ones, non-zero mantissa)
            strcat(s, (bits > 0x7f800000) ? "nan" : &"-inf"[!negative]);

            return;
        }

        fractionalDecimals--;
    }
}

#define METRIC_PREFIX_INDEX_MIN -3
#define METRIC_PREFIX_INDEX_MAX 3

static cstring metricPrefixStrings[] = {
    STRING_NANO,
    STRING_MICRO,
    STRING_MILLI,
    STRING_EMPTY,
    STRING_KILO,
    STRING_MEGA,
    STRING_GIGA,
};

// Clamps to minPrefixIndex (>= METRIC_PREFIX_INDEX_MIN) and to the n..G range
// of metricPrefixStrings, so the value is scaled to the prefix that is printed
NOINLINE static int32_t clampMetricPrefixIndex(int32_t prefixIndex, int32_t minPrefixIndex)
{
    if (prefixIndex < minPrefixIndex)
        prefixIndex = minPrefixIndex;
    if (prefixIndex > METRIC_PREFIX_INDEX_MAX)
        prefixIndex = METRIC_PREFIX_INDEX_MAX;

    return prefixIndex;
}

NOINLINE static void strcatMetricPrefix(char *s, int32_t prefixIndex)
{
    strcat(s, getString(metricPrefixStrings[prefixIndex - METRIC_PREFIX_INDEX_MIN]));
}

void strcatMetricValue(char *s, char *u, float value, int32_t minPrefixIndex)
{
    int32_t prefixIndex = 0;

    if (value == 0.0F)
        strcat(s, "‒.‒‒‒"); // Uses figure dash (U+2012)
    else
    {
        int32_t valueExponent = roundDown(LOG10E * logf(value / 0.99995F));
        prefixIndex = clampMetricPrefixIndex(divideDown(valueExponent, 3), minPrefixIndex);

        // 4 significant digits, at least one integer digit
        int32_t fractionalDecimals = 3 - (valueExponent - 3 * prefixIndex);
        if (fractionalDecimals > 3)
            fractionalDecimals = 3;
        if (fractionalDecimals < 0)
            fractionalDecimals = 0;

        strcatFloat(s, value / getDecimalPowerFloat(3 * prefixIndex), fractionalDecimals);
    }

    strcatMetricPrefix(u, prefixIndex);
}

void strcatMetricPower(char *s, int32_t exponent, int32_t minPrefixIndex)
{
    int32_t prefixIndex = clampMetricPrefixIndex(divideDown(exponent, 3), minPrefixIndex);
    int32_t mantissaExponent = exponent - 3 * prefixIndex;
    uint32_t fractionalDecimals = (mantissaExponent < 0) ? -mantissaExponent : 0;

    // 10^mantissaExponent (getDecimalPower() is 1 for negative exponents)
    strcatDecimal(s, getDecimalPower(mantissaExponent), fractionalDecimals + 1, fractionalDecimals);
    if (prefixIndex)
        strcatChar(s, ' ');
    strcatMetricPrefix(s, prefixIndex);
}

void strcatUInt8Hex(char *s, uint8_t value)
{
    static const char hexDigits[] = "0123456789abcdef";

    strcatChar(s, hexDigits[value >> 4]);
    strcatChar(s, hexDigits[value & 0xf]);
}

// Space and string end terminate a token
static inline bool isTokenEnd(char c)
{
    return !(c & ~' ');
}

bool parseToken(const char **s, const char *match)
{
    const char *t = *s;

    while (*t == ' ')
        t++;

    while (*match && (*t == *match))
    {
        t++;
        match++;
    }

    if (*match || !isTokenEnd(*t))
        return false;

    *s = t;

    return true;
}

bool parseUInt32(const char **s, uint32_t *value)
{
    const char *t = *s;

    while (*t == ' ')
        t++;

    const char *start = t;
    uint32_t v = 0;

    while (true)
    {
        uint32_t digit = (uint32_t)(*t - '0');
        if (digit > 9)
            break;

        t++;

        // Reject values that do not fit in 32 bits: v * 10 cannot overflow,
        // and an overflowing + digit wraps to less than digit
        if (v > (UINT32_MAX / 10))
            return false;

        v = v * 10 + digit;

        if (v < digit)
            return false;
    }

    if ((t == start) || !isTokenEnd(*t))
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

    bool negate = (*t == '-');
    if (negate)
        t++;

    // Digits go into an integer mantissa: one conversion and one division by
    // an exact power of ten at the end
    bool hasDigits = false;
    bool hasPoint = false;
    uint32_t mantissa = 0;
    uint32_t fractionalDecimals = 0;

    for (;; t++)
    {
        uint32_t digit = (uint32_t)(*t - '0');

        if (digit <= 9)
        {
            hasDigits = true;

            // Keeps the mantissa in 32 bits and the decimal power <= 10^9:
            // further fractional digits are ignored, integer parts
            // above about 4E9 are rejected
            if ((mantissa <= ((UINT32_MAX - 9) / 10)) && (fractionalDecimals < 9))
            {
                mantissa = mantissa * 10 + digit;
                fractionalDecimals += hasPoint;
            }
            else if (!hasPoint)
                return false;
        }
        else if ((*t == '.') && !hasPoint)
            hasPoint = true;
        else
            break;
    }

    if (!hasDigits || !isTokenEnd(*t))
        return false;

    float v = (float)mantissa / (float)getDecimalPower(fractionalDecimals);

    *s = t;
    *value = negate ? -v : v;

    return true;
}
