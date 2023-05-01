/*
 * FS2011 Pro
 * Number formatting
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>

void strcatNumber(char *buffer, uint64_t value, uint32_t length);
void strcatTime(char *buffer, uint32_t time);

void formatMantissaAndCharacteristic(const char *unitName, float value, int32_t minExponent,
                                     char *mantissaBuffer, char *characteristicBuffer);
void formatValue(const char *unitName, float value, char *buffer);
void formatMultiplier(const char *unitName, int32_t exponent, char *buffer);
void formatRate(float rate,
                char *mantissa, char *characteristic);
void formatDose(uint32_t count,
                char *mantissa, char *characteristic);

#endif
