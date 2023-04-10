/*
 * FS2011 Pro
 * Number formatting
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>

void formatHex(uint32_t value, char *buffer);
void formatUnsignedLongLong(uint64_t value, char *buffer);
void formatMantissaAndCharacteristic(const char *unitName, float value, int minExponent,
                                     char *mantissaBuffer, char *characteristicBuffer);
void formatValue(const char *unitName, float value, int minExponent,
                 char *buffer);
void formatMultiplier(const char *unitName, int exponent, int minExponent,
                      char *buffer);
void formatRate(float rate,
                char *mantissa, char *characteristic);
void formatDose(uint32_t count,
                char *mantissa, char *characteristic);
void formatTime(uint32_t time,
                char *buffer);

#endif
