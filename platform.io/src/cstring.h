/*
 * Rad Pro
 * Compact formatting
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(FORMAT_H)
#define FORMAT_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include STRINGS

#define getString(s) s

typedef const char *const cstring;

#if defined(__EMSCRIPTEN__)

void strclr(char *s);

#else

inline void strclr(char *s)
{
    *s = '\0';
}

#endif

void strcatChar(char *s, char c);

void strcatUInt32(char *s,
                  uint32_t value,
                  uint32_t length);
void strcatTime(char *s, uint32_t time);
void strcatFloat(char *s,
                 float value,
                 uint32_t fractionalDecimals);
void strcatFloatAsMetricValueAndPrefix(char *s,
                                       char *metricPrefix,
                                       float value,
                                       int32_t minMetricPrefixIndex);
void strcatDecimalPowerWithMetricPrefix(char *s,
                                        int32_t exponent,
                                        int32_t minMetricPrefixIndex);

void strcatUInt8Hex(char *s, uint8_t value);
void strcatUInt16Hex(char *s, uint16_t value);
void strcatUInt32Hex(char *s, uint32_t value);
void strcatDataHex(char *s,
                   uint8_t *data,
                   uint32_t size);

bool parseToken(const char **s, const char *match);
bool parseUInt32(const char **s, uint32_t *value);
bool parseFloat(const char **s, float *value);

#endif
