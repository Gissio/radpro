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

#ifdef __EMSCRIPTEN__

void strclr(char *str);

#else

inline void strclr(char *str)
{
    *str = '\0';
}

#endif

void strcatChar(char *str,
                char c);

void strcatUInt32(char *str,
                  uint32_t value, uint32_t length);
void strcatTime(char *str,
                uint32_t time);
void strcatFloat(char *str,
                 float value,
                 uint32_t fractionalDecimals);
void strcatFloatAsMetricValueAndPrefix(char *str,
                                       char *metricPrefix,
                                       float value,
                                       int32_t minMetricPrefixIndex);
void strcatFloatAsMetricValueWithPrefix(char *str,
                                        float value, int32_t minMetricPrefix);
void strcatDecimalPowerWithMetricPrefix(char *str,
                                        int32_t exponent,
                                        int32_t minMetricPrefixIndex);

void strcatUInt8Hex(char *str,
                    uint8_t value);
void strcatUInt16Hex(char *str,
                     uint16_t value);
void strcatUInt32Hex(char *str,
                     uint32_t value);
void strcatDataHex(char *str,
                   uint8_t *data,
                   uint32_t size);

bool parseUInt32(char *str,
                 uint32_t *value);
bool parseFloat(char *str,
                float *value);

#endif
