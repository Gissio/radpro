/*
 * Rad Pro
 * Compact formatting
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(FORMAT_H)
#define FORMAT_H

#include <stdbool.h>
#include <stdint.h>

void strcatUInt32(char *str, uint32_t value, uint32_t length);
bool parseUInt32(char *str, uint32_t *value);

void strcatTime(char *str, uint32_t time);

void strcatFloat(char *str, float value, uint32_t fractionalDecimals);
void strcatFloatWithMetricPrefix(char *str, float value, uint32_t minMetricPower);
void strcatDecimalPowerWithMetricPrefix(char *str, int32_t exponent);

void strcatUInt8Hex(char *str, uint8_t value);
void strcatUInt16Hex(char *str, uint16_t value);
void strcatUInt32Hex(char *str, uint32_t value);
void strcatDataHex(char *str, uint8_t *data, uint32_t size);
bool parseHexString(char *str, char *dest);

#endif
