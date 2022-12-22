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

void formatHex(unsigned int value, char *buffer);
void formatUnsignedLongLong(unsigned long long value, char *buffer);
void formatMantissaAndCharacteristic(const char *unitName, float value, int minExponent,
                                     char *mantissaBuffer, char *characteristicBuffer);
void formatValue(const char *unitName, float value, int minExponent,
                 char *buffer);
void formatMultiplier(const char *unitName, int exponent, int minExponent,
                      char *buffer);
void formatRate(float rate,
                char *mantissa, char *characteristic);
void formatDose(unsigned int count,
                char *mantissa, char *characteristic);
void formatTime(unsigned int time,
                char *buffer);

#endif
