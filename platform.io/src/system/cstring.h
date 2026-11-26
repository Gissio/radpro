/*
 * Rad Pro
 * String formatting
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(CSTRING_H)
#define CSTRING_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include STRINGS

#define getString(s) s

typedef const char *const cstring;

// static: a plain C99 inline definition emits no external symbol, so any call
// the compiler does not inline would fail to link
static inline void strclr(char *s)
{
    s[0] = '\0';
}

void strcatChar(char *s, char c);

// minDigits is capped at 10
void strcatUInt32(char *s, uint32_t value, uint32_t minDigits);
void strcatTime(char *s, uint32_t time);
// Rounds; fractionalDecimals is capped at 9; values >= 2^32 print as inf
void strcatFloat(char *s, float value, uint32_t fractionalDecimals);
// minPrefixIndex: -3 (nano) .. 3 (giga)
void strcatMetricValue(char *s, char *u, float value, int32_t minPrefixIndex);
// Also needs 3 * minPrefixIndex - 9 <= exponent <= 18
void strcatMetricPower(char *s, int32_t exponent, int32_t minPrefixIndex);

void strcatUInt8Hex(char *s, uint8_t value);

bool parseToken(const char **s, const char *match);
bool parseUInt32(const char **s, uint32_t *value);
// Integer parts above about 4E9 are rejected, digits beyond 1E-9 ignored
bool parseFloat(const char **s, float *value);

#endif
