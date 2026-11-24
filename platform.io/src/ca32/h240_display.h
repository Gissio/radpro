/* Rad Pro H240 panel initialization. License: MIT */

#ifndef H240_DISPLAY_H
#define H240_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#define H240_FACTORY_CLOCK_HZ 64000000U
// The 63 px font box needs a 63 px pitch to fit a wide glyph after a narrow one.
#define H240_DISPLAY_TEXTBUFFER_SIZE (63U * 63U)

// Initialize the panel and SPI with an idle bus. Backlight ownership is separate.
bool h240InitDisplayPanel(uint32_t peripheralClockHz);
bool h240DisplayHealthy(void);

#endif
